// =============================================================================
//  asendic_Auto_RT.cpp  --  Auto-tray ROUND-TRIP (auto-retest tray recycle) SM
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/asendic_Auto_RT.cpp
//                   (1047 decoded lines; count re-derived from the cp950 decode
//                    in this pass, `wc -l` agrees)
//  Translation wave: W906-W7-L1 Wave 2 (AUTO_RT track)
//  Translator: AI(W906-W7-L1-AutoRT) 20260802
//
//  FAITHFUL TRANSLATION
//  --------------------
//  Everything from golden :27 to golden :1047 is transcribed VERBATIM: names,
//  1-based numeric Task states, switch(Task) semantics, the case fall-throughs,
//  every SOFT_SIMULTE pair, the 193-line commented-out DoAutoTray block, and
//  every Sen[]/Cylinder[]/SW[]/MOT[] spelling.  ALL 12 golden top-level functions
//  are translated -- golden :34, :39, :197, :202, :318, :324, :672, :677, :906,
//  :911, :949, :954 (6 Init* + 6 Do*), plus the whole file-scope cursor/timer block.
//  Nothing in this file is `#if 0`.  Zero facade members are needed.
//
//  RELATIONSHIP TO asendic_Auto.cpp's DoAuto123TrayToRear -- MEASURED, NOT ASSUMED
//  ------------------------------------------------------------------------------
//  A W7-L1 planner measured this file's DoAutoTrayToRear (golden :324-465) as
//  84.3% identical to DoAuto123TrayToRear in golden asendic_Auto.cpp
//  (golden asendic_Auto.cpp:2300-2436, re-derived by grep in this pass).  That
//  pairing HOLDS -- the two share their cursor alphabet 1/100/200/300/400/450/460
//  /500/600 and most statements.  It was used only as a cross-check; every line
//  below was transcribed from THIS file's own golden.  The six places the twins
//  differ are exactly where a copy-from-Auto translation would have gone wrong,
//  and all six are re-derived here:
//    (1) case 200's arrival test.  RT: `Sen[SnAutoTrayDetect].IsOff()==false`
//        ALONE (golden :362).  Auto ANDs a second sensor,
//        `Sen[SnAutoCWPreDetect].IsOff()==false` (golden asendic_Auto.cpp:2337-2338).
//    (2) case 200's SKIP arm.  Auto calls fProductionInfo->CalTrayICCount(Pos)
//        (asendic_Auto.cpp:2363); RT does NOT.  This is the ONLY reason this file
//        needs no facade member at all.
//    (3) case 300.  Auto additionally raises C_AutoUpPress (asendic_Auto.cpp:2374);
//        RT does not.
//    (4) case 400's alarm arm.  Auto additionally drops C_AutoUpPress
//        (asendic_Auto.cpp:2396); RT does not.
//    (5) the belt stop moves.  Auto does `TrayMoveIn(false, Pos+3)` in case 460
//        (asendic_Auto.cpp:2417); RT does it in case 500 instead, AFTER the fixer
//        Push succeeds, and adds a 0.5 s settle -> case 510 (golden :440-451).
//        golden's own comment on :442 says why: "先後勾再停止,避免off的時候tray
//        盤被往回帶" (hook first, then stop the belt, so the tray is not dragged
//        back when the belt switches off).
//    (6) case 600.  RT latches the tray onto the Auto motor and clears the car --
//        `MOT[iMMAuto[Pos]].SetTray(HAS_IC,...)` + `MOT[iMMAuto_Car[Pos]].ClearTray(...)`
//        (golden :456-457).  Auto's case 600 does NEITHER.
//
//  INCLUDES (BCB6 original vs this tree)
//  -------------------------------------
//  BCB6 pulled: MachineDefine.h, asendic_Auto_RT.h, asendic.h, MyMotor.h,
//  mycylin.h, main.h, mymessbox.h, note.h, mysensor.h, myswitch.h, acatchtray.h,
//  cprod.h, OCR.h, OCRInsp.h, cMyDB.h, cmydef.h, common.h.  Replaced by the same
//  substitution table asendic_Auto2.cpp / asendic_Loader_RT.cpp document:
//    Motor/mymotor.h      MOT[]
//    mycylin.h            Cylinder[]
//    mysensor.h           Sen[]
//    myswitch.h           SW[]
//    asendic.h            TrayMoveIn / TrayMoveOut / TrayMoveStatus
//    acatchtray_shims.h   AutoCylinderUp/Middle/Lower  (golden declares these in
//                         asendic.h:19-21; this tree relocated the three decls
//                         there -- see that header's Wave-0 banner)
//    cprod.h              Ld_UldDelayTime (LD_TrayArrivalDely / ULD_TrayBackDelay)
//    cmydef.h             the Auto index tables, alarm-code tables, NULL_IC /
//                         HAS_IC / DUMMY / K_RETRY / K_SKIP, iReceiveAutoTray,
//                         iAutoTrayCount, UNLOADER_ART, bNoAutoZSelect,
//                         USE_AUTO_RETEST, USE_LdUldCassetteMode, CUSTOMER_CODE
//    MachineType.h        MAX_AUTO_TRAY, CC_ASE_KaohSiung, eartInstall
//    Config.h             IniConfig.bP37bAutoCylinderUP / .bLastLoaderNoInSide
//    CosFunction.h        CosFunction.bUseSCKART
//    canary_support.h     LastSet, __FUNC__, ShowErrorMessage, ShowMyMessage
//  Not needed offline: main.h (TfMain -- this file touches no form), mymessbox.h
//  and note.h (ShowMyMessage / ShowErrorMessage now live in canary_support.h),
//  acatchtray.h (no WhichAutoNeedTray / tray-arm symbol in this file's body),
//  OCR.h / OCRInsp.h / cMyDB.h / common.h (no OCR, DB or MySleepEx symbol here).
//
//  SOFT_SIMULTE is NOT defined: the `#ifndef SOFT_SIMULTE` (real-machine)
//  branches are compiled, the `#ifdef SOFT_SIMULTE` branches are not.  Both arms
//  are kept verbatim.  There are THREE such regions, at golden :167-189
//  (DoLoadNewEmptyTrayToCar_RT case 420 -- note the `break;` and the whole
//  `else if` JAM1113 arm live INSIDE the `#ifndef`), :399-404 (DoAutoTrayToRear
//  case 400) and :689-738 / :755-764 / :843-847 (DoUnLoadNewAutoToStack); plus
//  one more, :549-552, inside the commented-out DoAutoTray block.
//
//  GOLDEN FALL-THROUGHS PRESERVED (every `case N:` whose preceding statement is
//  not break/return, re-derived by scanning the decoded golden in this pass):
//    (1) DoAutoTrayToFront case 1 -> case 100 (golden :214-215)
//    (2) DoAutoTrayToRear  case 1 -> case 100 (golden :340-341)
//  There are NO other fall-throughs in this file.
//
//  TU-LOCAL STAND-INS + `#define` REDIRECTS -- READ THIS BEFORE EDITING
//  --------------------------------------------------------------------
//  TWO free functions this file calls have no linkable home in the ported tree.
//  Rather than gate their nine call sites (which would destroy the verbatim
//  switch(Task) bodies this translation exists to preserve), each is redirected
//  to a transparently-named TU-local stand-in with a `#define` -- the same idiom
//  asendic_Loader.cpp and csystem.cpp already use.  The redirects live BELOW all
//  #includes, so no header declaration is perturbed, and they are TU-local, so no
//  duplicate symbol can ever be created.
//
//   1. bARTUnloaderUseOneCylin(int)   golden asendic.h:36 / asendic.cpp:131
//   2. bARTUnloaderUseTwoCylin(int)   golden asendic.h:37 / asendic.cpp:139
//      Both are golden asendic.cpp free functions.  acatchtray_shims.h's Wave-0
//      SHIM RETIREMENT DEBT REGISTER states explicitly that these land as REAL
//      translated bodies in Wave 3 (asendic.cpp / L1a) and that "no shim for them
//      exists here to retire, and none should be added" -- so they are stood in
//      TU-locally here instead of being added to a shared header.
//      OFFLINE VALUE: NOT a constant and NOT an approximation.  Both bodies below
//      are the golden bodies transcribed LINE FOR LINE from asendic.cpp:131-148,
//      and every input they read (USE_AUTO_RETEST, eartInstall, UNLOADER_ART[],
//      bNoAutoZSelect, USE_LdUldCassetteMode) already exists in this tree
//      (cmydef.h:2985 / MachineType.h:908 / cmydef.h:2986 / cmydef.h:4707 /
//      cmydef.h:2902 -- all verified present in this pass).  A constant stand-in
//      would have been the wrong choice here for a specific reason: golden :781-795
//      picks BETWEEN TWO DIFFERENT AutoCylinderMiddle ARGUMENT ORDERS on
//      bARTUnloaderUseOneCylin's answer, so freezing it would have made one of the
//      two orders untestable -- see the AutoCylinder note below.
//      COST: none.  With the machine defaults (USE_AUTO_RETEST==eartUninstall)
//      both answer false, exactly as golden would.
//      RETIRED BY: Wave 3 (asendic.cpp L1a).  Delete the two statics AND the two
//      `#define`s then, and add `#include "asendic.h"`-reachable real decls;
//      the bodies below are byte-equivalent so no behaviour changes.
//
//  AutoCylinderUp / AutoCylinderMiddle / AutoCylinderLower ARE STILL NO-OP STUBS
//  ----------------------------------------------------------------------------
//  acatchtray_shims.cpp defines all three as `{ return true; }`.  Their real
//  golden bodies (asendic.cpp:562 / :767 / :937) are Wave 3.  This file has 25
//  call sites (counted in this pass: 6 in DoLoadNewEmptyTrayToCar_RT, 13 in
//  DoUnLoadNewAutoToStack, 6 in DoTrayZAutoTrayToWait) and PARAMETERS 2 AND 3 ARE
//  NOT SYMMETRIC -- `(Part, CylinderName, CylinderNameMid, bReset)`.  Every site
//  reproduces golden's argument order EXACTLY and MUST NOT be "normalised":
//    * 21 sites use the common order (C_Auto_Up[Pos], C_Auto_Selector[Pos]).
//    * FOUR sites use the REVERSED order (C_Auto_Selector[Pos], C_Auto_Up[Pos]),
//      all inside DoUnLoadNewAutoToStack and all as the bARTUnloaderUseOneCylin
//      arm of a RUNTIME swap against the bARTUnloaderUseTwoCylin arm right above
//      it: golden :783 (vs :791), :816 (vs :814/:818), :856 (vs :854/:858),
//      :872 (vs :870/:874).  golden :783-vs-:791 is one of the five runtime swaps
//      catalogued in acatchtray_shims.h's AutoCylinder* banner.
//    * ONE site is a golden BUG, translated faithfully: golden :983 passes the
//      literal `1` as Part instead of `Pos` -- see GOLDEN DEFECTS below.
//
//  GOLDEN DEFECTS FOUND AND TRANSLATED FAITHFULLY ANYWAY
//  -----------------------------------------------------
//  Each was re-derived from the decoded golden in this pass.  None is "fixed"
//  here; all are reported.
//   A. InitTrayZAutoTrayToWaitTask() (golden :949-952) resets
//      `iTrayZLoadTrayToWaitTask` -- the LOADER's cursor (golden asendic.h:14,
//      defined asendic_Loader.cpp:51) -- but the SM it names,
//      DoTrayZAutoTrayToWait, binds `int &Task=iTrayZAutoTrayToWaitTask[Pos]`
//      (golden :957).  So the Init does NOT reset the machine it is named for,
//      and it DOES clobber a different, live SM's cursor.  Its only caller is
//      golden :68.
//   B. DoTrayZAutoTrayToWait case 200 (golden :983) calls
//      `AutoCylinderMiddle(1, C_Auto_Up[Pos], C_Auto_Selector[Pos], true)` --
//      hard-coded Part `1` with Pos-indexed cylinders.  The very next case
//      (golden :988) carries the comment "kevin 20170810 (Steven) 升到分離高度
//      1-->Pos", i.e. the identical bug WAS fixed one case later and this site
//      was missed.  Correct only for Pos==1 (Auto2).
//   C. DoAutoTrayToRear cases 300 and 400 (golden :394-427) form an UNREACHABLE
//      ISLAND under normal flow: 300 is only ever assigned by case 400 and 400
//      only by case 300; case 200 hands off to 450, and 450->460->500->510->600.
//      Verified by grepping every caller in the golden tree -- AutoRetest.cpp:1108
//      /:1129/:1142 only call InitAutoTrayToRearTask (which writes 1).  The one
//      way in is the operator Task List UI (golden main.cpp:9870-9873 registers
//      &iAutoTrayToRearTask[0..2] as writable QueueTaskList entries), so "dead"
//      means dead-under-normal-flow, not unreferenced.
//   D. `iReceiveAutoTray` is dimensioned [3] (golden cmydef.cpp:2884) but this
//      file indexes it with `Pos`, which the rest of the file treats as
//      0..MAX_AUTO_TRAY-1 (=0..5): golden :923, :932, :940.  Auto4/5/6 write out
//      of bounds.  (The other four indexings, golden :481/:598/:605/:641, are
//      inside the commented-out block and so are inert.)
//   E. `int iAutoReceiveTask[MAX_AUTO_TRAY]={1};` (golden :27) is DEAD -- grepped
//      the whole cp950-decoded golden tree, this line is its ONLY occurrence.
//      Kept verbatim; it costs 24 bytes of .data.
//   F. DoLoadNewEmptyTrayToCar_RT returns `int`, but golden :63 returns `true`
//      (==1) on the "car already has a tray, nothing to do" path -- the SAME code
//      the "new tray successfully loaded" path returns at golden :176.  The two
//      outcomes are indistinguishable to the caller (AutoRetest.cpp:1208).
//   G. `int iLoadNewAutoTrayToCarTask[MAX_AUTO_TRAY]={1};` (golden :28),
//      `iAutoTrayToFrontTask` (:194), `iAutoTrayToRearTask` (:315) and
//      `iUnLoadNewAutoTrayTask` (:669) initialise only element [0] to 1; [1]..[5]
//      get 0, which is NOT a valid cursor for any of the four switch(Task)
//      bodies (no `case 0:`), so Auto2..Auto6 fall straight through to
//      `return false`/`return 0` until their Init* runs.  Two of the siblings on
//      the same page (:29, :30) DO spell out all six.  Kept verbatim.
//   H. `TQPF_Timer AutoRTDelay;` (golden :32) is a SCALAR while every other timer
//      in this file is `[MAX_AUTO_TRAY]` (:31, :195, :196, :316, :317, :670,
//      :671).  Its only user is DoTrayZAutoTrayToWait, which is a PER-Pos state
//      machine (`int &Task=iTrayZAutoTrayToWaitTask[Pos]`, golden :957) and arms
//      it at :978 / reads it at :981.  Two Autos running the last-tray probe at
//      the same time therefore share one deadline and can each cut the other's
//      1 s settle short.  Kept verbatim.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  ZERO U+FFFD.
// =============================================================================
#include "asendic_Auto_RT.h"

//---------------------------------------------------------------------------
#include "asendic.h"            // TrayMoveIn / TrayMoveOut / TrayMoveStatus
#include "Motor/mymotor.h"      // MOT[]
#include "mycylin.h"            // Cylinder[]   (also pulls myTimer.h -> TQPF_Timer)
#include "mysensor.h"           // Sen[]
#include "myswitch.h"           // SW[]
#include "acatchtray_shims.h"   // AutoCylinderUp / AutoCylinderMiddle / AutoCylinderLower
#include "cprod.h"              // Ld_UldDelayTime
#include "cmydef.h"             // Auto index tables / alarm tables / run-mode globals
#include "MachineType.h"        // MAX_AUTO_TRAY / CC_ASE_KaohSiung / eartInstall
#include "Config.h"             // IniConfig
#include "CosFunction.h"        // CosFunction
#include "canary_support.h"     // LastSet, __FUNC__, ShowErrorMessage, ShowMyMessage

// =============================================================================
//  TU-LOCAL STAND-INS FOR SYMBOLS WITH NO PORTED HOME  (see banner, items 1-2)
//  Bodies transcribed LINE FOR LINE from golden asendic.cpp:131-148.
// =============================================================================
static bool W7L1ART_bARTUnloaderUseOneCylin(int Part)                           //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
{
    if(USE_AUTO_RETEST==eartInstall && UNLOADER_ART[Part]==eartInstall && bNoAutoZSelect)
        return true;
    else
        return false;
}
static bool W7L1ART_bARTUnloaderUseTwoCylin(int Part)                           //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
{
    if(USE_AUTO_RETEST==eartInstall &&
        UNLOADER_ART[Part]==eartInstall &&
        bNoAutoZSelect==false &&
        USE_LdUldCassetteMode!=1)                                               //RogerYang 20260207 : Add fot 9046 CR
        return true;
    else
        return false;
}
#define bARTUnloaderUseOneCylin             W7L1ART_bARTUnloaderUseOneCylin
#define bARTUnloaderUseTwoCylin             W7L1ART_bARTUnloaderUseTwoCylin

// =============================================================================
//  Everything below this line is golden asendic_Auto_RT.cpp:27-1047, VERBATIM.
// =============================================================================
int iAutoReceiveTask            [MAX_AUTO_TRAY]={1};
int iLoadNewAutoTrayToCarTask   [MAX_AUTO_TRAY]={1};
int iTrayZAutoTrayToWaitTask    [MAX_AUTO_TRAY]={1, 1, 1, 1, 1, 1};             //kevin 20170809 (Steven) add 20150610 判斷是否最後一盤
int iAutoTrayReceiveTask        [MAX_AUTO_TRAY]={1, 1, 1, 1, 1, 1};
TQPF_Timer AutoDelay            [MAX_AUTO_TRAY];
TQPF_Timer AutoRTDelay;                                                         //kevin 20150610
//---------------------------------------------------------------------------
void InitLoadNewAutoTrayToCarTask(int Pos)
{
    iLoadNewAutoTrayToCarTask[Pos]=1;
}
//---------------------------------------------------------------------------
int DoLoadNewEmptyTrayToCar_RT(int Pos)
{
    AnsiString Str;
    int ret;
    int &Task=iLoadNewAutoTrayToCarTask[Pos];
    switch(Task)
    {
        case 1:
            if(MOT[iMMAuto_Car[Pos]].fHasTray)
            {
                AutoDelay[Pos].SetMSAndOn(1000);                                                                        //Steven 20130904 : 加入Time Out 避免Hang Up
                Task=400;
                break;
            }

            if(Sen[SnAutoTrayHasTray[Pos]].IsOff()==false ||
               LastSet.iRealDummy==DUMMY)
            {
                if(Sen[SnAutoPreDete[Pos]].IsOn() ||
                   Sen[SnAutoTrayCar[Pos]].IsOn())
                {
                    MOT[iMMAuto_Car[Pos]].SetTray(NULL_IC, __FUNC__);
                    MOT[iAutoZMot[Pos]].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }

                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                                                     //kevin 20150610  偵測是否最後一盤
                {
                    InitTrayZAutoTrayToWaitTask();                                                                      //kevin 20150610
                    Task=50;
                }
                else
                {
                    Task=51;
                }
                AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                        //Steven 20140409 : AutoCylinderXX Add Reset
            }
            else
            {
                if(MOT[iMMAuto_Car[Pos]].fHasTray==false)                                                               //jou 2011-12-21 有發生Tray feed完,汽缸還在上面,所以做完下降
                {
                    if(Cylinder[C_Auto_Selector[Pos]].OffStatus()==false)                                               //汽缸在上面
                    {
                        if(Sen[SnAutoTrayCar[Pos]].IsOff() && Sen[SnAutoPreDete[Pos]].IsOff())
                        {
                            Cylinder[C_Auto_Up[Pos]].Off();
                            Cylinder[C_Auto_Selector[Pos]].Off();
                            break;
                        }
                    }
                }

                ShowErrorMessage(sMES1121[Pos], K_RETRY, iMMAuto_Car[Pos]);                                             //Steven 20150108 : MES1021 --> MES1121, MES1221, MES1321
            }
            break;
        case 50:
            ret=DoTrayZAutoTrayToWait(Pos);
            if(ret==1)
            {
                Task=300;
            }
            else if(ret==2)                                                     //kevin 20150613 尾盤不入tray
            {
                MOT[iAutoZMot[Pos]].ClearTray(__FUNC__);
                Task=1;
                return 2;
            }
            break;
        case 51:
            if(AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))
            {
                MOT[iAutoZMot[Pos]].ClearTray(__FUNC__);
                Task=100;
            }
            break;
        case 100:
            if(LastSet.iRealDummy==DUMMY)                                                                               //Steven 20100618
                Task=200;
            else if(Cylinder[C_AutoZ_Select[Pos]].Push())
                Task=200;

            AutoCylinderMiddle(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                        //Steven 20140409 : AutoCylinderXX Add Reset
            break;
        case 200:
            if(AutoCylinderMiddle(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))
            {
                if(bEmptyPause)
                {
                    bEmptyPause=false;
                    ShowMyMessage("Auto split wait", "Auto 分離暫停", "DoLoadNewEmptyTrayToCar 200");
                    Task=200;
                    break;
                }
                Task=300;
            }
            break;
        case 300:
            if(LastSet.iRealDummy==DUMMY)                                       //Steven 20111212
            {
                Task=400;
            }
            else if(Cylinder[C_AutoZ_Select[Pos]].Pop())
            {
                Task=400;
            }
            AutoDelay[Pos].SetMSAndOn(1000);                                    //Steven 20130904 Start: 加入Time Out 避免Hang Up
            break;
        case 400:
            if(AutoDelay[Pos].Off())
            {
                AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                     //Steven 20140409 : AutoCylinderXX Add Reset
                Task=410;
            }
            break;
        case 410:
            if(AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))
            {
                AutoDelay[Pos].SetMSAndOn(1000);
                Task=420;
            }
            break;
        case 420:
            if(bHandlerPause)                                                                                           //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                AutoDelay[Pos].SetMSAndOn(1000);
            }

#ifndef SOFT_SIMULTE
            if((Sen[SnAutoPreDete[Pos]].IsOn()  ||
                Sen[SnAutoTrayCar[Pos]].IsOn()) ||
               LastSet.iRealDummy==DUMMY)
#endif
            {
                MOT[iMMAuto_Car[Pos]].SetTray(NULL_IC, __FUNC__);
                MOT[iAutoZMot[Pos]].ClearTray(__FUNC__);
                Task=1;
                return 1;
            }
#ifndef SOFT_SIMULTE
            else if(AutoDelay[Pos].Off())
            {
                ret=ShowErrorMessage(sJAM1113[Pos], K_RETRY|K_SKIP, iMMAuto_Car[Pos]);                                  //Steven 2010108 : JAM1001 --> JAM1113, JAM1213, JAM1313
                if(ret==K_SKIP)
                {
                    MOT[iAutoZMot[Pos]].ClearTray(__FUNC__);
                    Task=1;
                }
            }
            break;
#endif
    }
    return 0;
}
//------------------------------------------------------------------------------
int iAutoTrayToFrontTask            [MAX_AUTO_TRAY]={1};
TQPF_Timer hAutoTrayToFront         [MAX_AUTO_TRAY];
TQPF_Timer hAutoTrayToFrontForDummy [MAX_AUTO_TRAY];
void InitAutoTrayToFrontTask(int Pos)
{
    iAutoTrayToFrontTask[Pos]=1;
}
//------------------------------------------------------------------------------
bool DoAutoTrayToFront(int Pos)
{
    AnsiString Str;
    int iMot=iMMAuto[Pos];
    int &Task=iAutoTrayToFrontTask[Pos];
    int ret;
    static bool bAutoHasDuplicateError[MAX_AUTO_TRAY]={false, false, false, false, false, false};                       //Steven 20120220 : Empty Tray重複Alarm Flag
    switch(Task)
    {
        case 1:
            hAutoTrayToFront[Pos].SetMSAndOn(20000);
            hAutoTrayToFrontForDummy[Pos].SetMSAndOn(5000);
            Task=100;
        case 100:
            if(bHandlerPause)                                                                                                                                   //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                Task=1;
                break;
            }

            Cylinder[C_AutoSide_Fixer[Pos]].Off();
            Cylinder[C_AutoEdgePush[Pos]].Off();
            Cylinder[C_AutoUpPress[Pos]].Off();                                                                                                                 //JerryYang 20190423 新增unloader壓tray

            TrayMoveOut(true, Pos+3);

            if(Sen[SnAutoTrayCar[Pos]].IsOff()==false)
            {
                TrayMoveOut(false, Pos+3);
                bAutoHasDuplicateError[Pos]=false;                                                                                                              //Steven 20120220 : Empty Tray重複Alarm Flag
                Task=101;
                hAutoTrayToFrontForDummy[Pos].SetMSAndOn(10000);
                break;
            }

            if(hAutoTrayToFrontForDummy[Pos].Off())
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    TrayMoveOut(false, Pos+3);
                    MOT[iMMAuto_Car[Pos]].SetTray(NULL_IC, __FUNC__);
                    MOT[iMot].ClearTray(__FUNC__);
                    bAutoHasDuplicateError[Pos]=false;                                                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                    return true;
                }
            }

            if(hAutoTrayToFront[Pos].Off())
            {
                ret=ShowErrorMessage(sJAM1112[Pos], K_SKIP|K_RETRY, iMot, bAutoHasDuplicateError[Pos], "DoAutoTrayToFront_100");                                //Steven 20150108 : JAM1012 --> JAM1112, JAM1212, JAM1312
                if(ret==K_RETRY)
                {
                    bAutoHasDuplicateError[Pos]=true;                                                                                                           //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                }
                else
                {
                    TrayMoveOut(false, Pos+3);
                    bAutoHasDuplicateError[Pos]=false;                                                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                    MOT[iMot].ClearTray(__FUNC__);
                    MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }
            }
            break;
        case 101:
            if(Sen[SnAutoCWPreDetect[Pos]].IsOff()==false &&
               Sen[SnAutoTrayDetect[Pos]].IsOff()==false)
            {
                Cylinder[C_AutoEdgePush[Pos]].On();
                Task=102;
                break;
            }
            else if(hAutoTrayToFrontForDummy[Pos].Off())                                                                //kevin 20120904
            {
                ShowErrorMessage(sJAM1112[Pos], K_RETRY, iMot, bAutoHasDuplicateError[Pos], "DoAutoTrayToFront_101");   //Steven 20150108 : JAM1212 --> JAM1112, JAM1212, JAM1312
                Task=100;
            }
            break;
        case 102:
            if(Cylinder[C_AutoSide_Fixer[Pos]].Push())
            {
                hAutoTrayToFrontForDummy[Pos].SetMSAndOn(100);
                Task=103;
            }
            break;
        case 103:
            if(hAutoTrayToFrontForDummy[Pos].Off())
            {
                hAutoTrayToFrontForDummy[Pos].SetMSAndOn(100);
                Cylinder[C_AutoSide_Fixer[Pos]].Off();
                Task=104;
            }
            break;
        case 104:
            if(hAutoTrayToFrontForDummy[Pos].Off())
            {
               Cylinder[C_AutoEdgePush[Pos]].Off();
               Task=105;
            }
            break;
        case 105:
            MOT[iMot].ClearTray(__FUNC__);
            MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);
            bAutoHasDuplicateError[Pos]=false;                                  //Steven 20120220 : Empty Tray重複Alarm Flag
            Task=1;
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int iAutoTrayToRearTask             [MAX_AUTO_TRAY]={1};
TQPF_Timer hAutoTrayToRear          [MAX_AUTO_TRAY];
TQPF_Timer hAutoTrayToRearForDummy  [MAX_AUTO_TRAY];
void InitAutoTrayToRearTask(int Pos)
{
    iAutoTrayToRearTask[Pos]=1;
}
//------------------------------------------------------------------------------
//---- 收空TRAY----
bool DoAutoTrayToRear(int Pos)
{
    static bool bAutoHasDuplicateError[MAX_AUTO_TRAY]={false, false, false, false, false, false};                       //Steven 20120220 : Empty Tray重複Alarm Flag

    int &Task       =iAutoTrayToRearTask[Pos];
    int ret         =0;
    bool bFlag      =false;
    AnsiString Str  ="";

    switch(Task)
    {
        case 1:
            hAutoTrayToRearForDummy[Pos].SetMSAndOn(5000);
            hAutoTrayToRear[Pos].SetMSAndOn(20000);
            Cylinder[C_AutoEdgePush[Pos]].Off();
            Cylinder[C_AutoUpPress[Pos]].Off();                                 //JerryYang 20190423 新增unloader壓tray
            Task=100;
        case 100:
            if(Cylinder[C_AutoSide_Fixer[Pos]].Pop())                           //ChungHung 20140625 add
            {
                Task=200;
            }
            break;
        case 200:
            if(bHandlerPause)                                                                                                                                   //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                Task=1;
                break;
            }

            if(MOT[iMMAuto[Pos]].fHasTray &&
               MOT[iMMAuto_Car[Pos]].fHasTray)                                                                                                                  //Steven 20130917 : Double Tray問題,測試 Q4
            {
                Task=1;
                return true;
            }

            TrayMoveIn(true, Pos+3);
            if(Sen[SnAutoTrayDetect[Pos]].IsOff()==false)
            {
                Task=450;
                break;
            }

            if(hAutoTrayToRearForDummy[Pos].Off())
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    Task=450;
                    break;
                }
            }

            if(hAutoTrayToRear[Pos].Off())
            {
                ret=ShowErrorMessage(sJAM1101[Pos], K_SKIP|K_RETRY, iMMAuto_Car[Pos], bAutoHasDuplicateError[Pos], "DoAutoTrayToRear_200");                     //Steven 20150108 : JAM1001 -> JAM1101, JAM1201, JAM1301
                if(ret==K_RETRY)
                {
                    bAutoHasDuplicateError[Pos]=true;                                                                                                           //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                }
                else
                {
                    bAutoHasDuplicateError[Pos]=false;                                                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                    MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }
            }
            break;
        case 300:
            if(Cylinder[C_AutoSide_Fixer[Pos]].Push())
                Task=400;
            break;
        case 400:
            #ifndef SOFT_SIMULTE
                bFlag=(Sen[SnAutoFixCyPush[Pos]].IsOn() ||
                       Cylinder[C_AutoSide_Fixer[Pos]].OffSensor());                                                                                            //wei 20241011 新增後勾氣缸off sensor,避免氣缸沒作動
            #else
                bFlag=false;
            #endif

            if(bFlag)
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    bAutoHasDuplicateError[Pos]=false;                                                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=450;
                }
                else
                {
                    Cylinder[C_AutoSide_Fixer[Pos]].Off();
                    ret=ShowErrorMessage(sJAM1103[Pos], K_RETRY, iMMAuto[Pos], bAutoHasDuplicateError[Pos], "DoAutoTrayToRear_400");                            //Steven 20150108 : JAM1003 -> JAM1103, JAM1203, JAM1303
                    bAutoHasDuplicateError[Pos]=false;                                                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=300;
                    break;
                }
            }
            else
            {
                bAutoHasDuplicateError[Pos]=false;                                                                                                              //Steven 20120220 : Empty Tray重複Alarm Flag
                Task=500;
            }
            break;
        case 450:
            Cylinder[C_AutoEdgePush[Pos]].Off();
            hAutoTrayToRear[Pos].SetSecAndOn(Ld_UldDelayTime.LD_TrayArrivalDely);
            Task=460;
            break;
        case 460:
            if(hAutoTrayToRear[Pos].Off())
            {
                Task=500;
            }
            break;
        case 500:                                                               //ChungHung 20140625 add start
            if(Cylinder[C_AutoSide_Fixer[Pos]].Push())
            {
                TrayMoveIn(false, Pos+3);                                       //JerryYang 20201230 先後勾再停止,避免off的時候tray盤被往回帶
                hAutoTrayToRear[Pos].SetSecAndOn(0.5);
                Task=510;
            }
            break;
        case 510:
            if(hAutoTrayToRear[Pos].Off())
            {
                Task=600;
            }
            break;
        case 600:
            if(Cylinder[C_AutoSide_Fixer[Pos]].Pop())
            {
                MOT[iMMAuto[Pos]].SetTray(HAS_IC, __FUNC__);
                MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);                      //Steven 20111109 : 已入Tray卻未清掉資料
                Task=1;
                return true;
            }
            break;
            //ChungHung 20140625 add End
    }
    return false;
}
//------------------------------------------------------------------------------
//int iAutoTrayTask[MAX_AUTO_TRAY]={1};
//TQPF_Timer DoAutoTrayDelay[MAX_AUTO_TRAY];
//TQPF_Timer CheckAutoTrayDelay[MAX_AUTO_TRAY];
//void InitAutoTrayTask(int Pos)
//{
//    iAutoTrayTask[Pos]=1;
//}
//------------------------------------------------------------------------------
/*void DoAutoTray(int Pos)
{
    AnsiString Str;
    int iMot=iMMAuto[Pos];
    int &Task=iAutoTrayTask[Pos];
    static bool bAutoHasDuplicateError[MAX_AUTO_TRAY]={false, false, false, false, false, false};                       //Steven 20120220 : Auto Tray重複Alarm Flag
    if(iReceiveAutoTray[Pos]!=0)
    {
        DoAutoReceiveBinTray(Pos);
        return;
    }

    if(fAutoCanSupplyNewTray[Pos]==false)
        return;

    switch(Task)
    {
        case 1:
            if(MOT[iMot].fHasTray || Sen[SnAutoTrayDetect[Pos]].IsOn())
            {
                Task=20;
                break;
            }
            else
            {
                MOT[iMot].ClearTray(__FUNC__);
            }

            if(MOT[iMot].fHasTray==false)
            {
                Task=100;
                if(MOT[MMTrayY].fHasTray && MOT[MMTrayY].HasIC()==false)
                {
                    Task=1;
                    break;
                }
            }
            break;
        case 20:
            Cylinder[C_AutoSide_Fixer[Pos]].Off();
            Cylinder[C_AutoEdgePush[Pos]].Off();
            Cylinder[C_AutoUpPress[Pos]].Off();                                 //JerryYang 20190423 新增unloader壓tray
            Task=30;
            break;
        case 30:
            if(Cylinder[C_AutoSide_Fixer[Pos]].Pop())
                Task=40;
            break;
        case 40:
            if(Sen[SnAutoFixCyPush[Pos]].IsOn() ||
               Cylinder[C_AutoSide_Fixer[Pos]].OffSensor())                                                             //wei 20241011 新增後勾氣缸off sensor,避免氣缸沒作動
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    bAutoHasDuplicateError[Pos]=false;                                                                  //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=50;
                }
                else
                {
                    Cylinder[C_AutoSide_Fixer[Pos]].Off();
                    ShowErrorMessage(sJAM1103[Pos], K_RETRY, MMEmpty, bAutoHasDuplicateError[Pos], __FUNC__);           //Steven 20150108 : JAM1003 -> JAM1103, JAM1203, JAM1303
                    bAutoHasDuplicateError[Pos]=true;                                                                   //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                }
            }
            else
            {
                bAutoHasDuplicateError[Pos]=false;                                                                      //Steven 20120220 : Empty Tray重複Alarm Flag
                Task=50;
            }
            break;
        case 50:
            if(Cylinder[C_AutoSide_Fixer[Pos]].Pop())
            {
                #ifdef SOFT_SIMULTE
                MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);
                MOT[iMot].SetTray(NULL_IC);                                     //Steven 20130909
                #endif
                Task=60;
            }
            break;
        case 60:
            if(MOT[iMot].fHasTray==false)
            {
                //Question ASE_G
                DoAutoTrayDelay[Pos].SetMSAndOn(1000);
                Task=70;
            }
            else
            {
                CheckAutoTrayDelay[Pos].SetMSAndOn(100);
                Task=65;
            }
            break;
        case 65:
            if(CheckAutoTrayDelay[Pos].Off())
            {
                if(MOT[iMot].fHasTray==true && LastSet.iRealDummy>=HAS_TRAY &&
                   Sen[SnAutoTrayDetect[Pos]].IsOff() && Sen[SnAutoCWPreDetect[Pos]].IsOff())
                {
                    MOT[iMot].ClearTray(__FUNC__);                              //Steven 20130909
                }
                Task=60;

                if(MOT[iMot].fHasTray==false        &&
                   LastSet.iRealDummy>=HAS_TRAY &&
                   Sen[SnAutoTrayDetect[Pos]].IsOff()==false &&
                   Sen[SnAutoCWPreDetect[Pos]].IsOff()==false)                  //Steven 20130930
                {
                    MOT[iMot].SetTray(NULL_IC);
                }

                if(MOT[iMMAuto_Car[Pos]].fHasTray==false    &&
                   LastSet.iRealDummy>=HAS_TRAY &&
                   Sen[SnAutoTrayCar[Pos]].IsOff()==false &&
                   Sen[SnAutoPreDete[Pos]].IsOff()==false)                      //Steven 20130930
                {
                    MOT[iMMAuto_Car[Pos]].SetTray(NULL_IC);
                }

                if(MOT[MMTrayY].fHasTray && MOT[MMTrayY].HasIC()==false &&
                    MOT[iMot].fHasTray && MOT[iMMAuto_Car[Pos]].fHasTray)
                {
                    iReceiveAutoTray[Pos]=2;
                    iAutoTrayReceiveTask[Pos]=200;
                }

                //Steven 20130917 : Double Tray問題,測試 Q4
                if(MOT[iMot].fHasTray && MOT[iMMAuto_Car[Pos]].fHasTray)
                {
                    iReceiveAutoTray[Pos]=2;
                    iAutoTrayReceiveTask[Pos]=200;
                }
            }
            break;
        case 70:
            if(CheckAutoTrayDelay[Pos].Off())
            {
                if((Sen[SnAutoTrayDetect[Pos]].IsOn() && MOT[iMot].fHasTray==false) ||
                   (LastSet.iRealDummy==DUMMY && MOT[iMot].fHasTray==false))
                {
                    MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);
                    MOT[iMot].SetTray(NULL_IC);                                 //Steven 20130909
                }
                Task=100;
            }
            break;
        case 100:
            if(MOT[iMMAuto_Car[Pos]].fHasTray)
            {
                if((MOT[MTrayX].fHasTray && MOT[MTrayX].ReadPos()==Prod.iXTrayAuto[Pos]))
                {
                    CheckAutoTrayDelay[Pos].SetMSAndOn(2000);
                    Task=150;
                }
                else
                {
                    InitAutoTrayToRearTask(Pos);
                    Task=300;
                }
            }
            else
            {
                InitLoadNewAutoTrayToCarTask(Pos);
                Task=200;
                //jou 980421 empty tray no need relay one tray in empty_car
                if(iReceiveAutoTray[Pos]!=0 || (MOT[MTrayX].fHasTray && MOT[MTrayX].ReadPos()==Prod.iXTrayAuto[Pos]) ||
                    MOT[iMot].fHasTray)
                {
                    CheckAutoTrayDelay[Pos].SetMSAndOn(2000);
                    Task=150;
                }
            }
            break;
        case 150:
            if(CheckAutoTrayDelay[Pos].Off())
                Task=1;
            break;
        case 200:
            if(DoLoadNewAutoTrayToCar(Pos))
            {
                InitLoadNewAutoTrayToCarTask(Pos);
                Task=1;
            }
            break;
        case 300:
            if(DoAutoTrayToRear(Pos))
            {
                Task=1;
            }
            break;
    }
}   */
//------------------------------------------------------------------------------
int iUnLoadNewAutoTrayTask[MAX_AUTO_TRAY]={1};
TQPF_Timer DoUnLoadNewAutoToStackDelay[MAX_AUTO_TRAY];
TQPF_Timer hUnLoadSafetyCheckTimer[MAX_AUTO_TRAY];                             //AI(general) 20260323 (RogerYang) : UpSafe sensor 5秒保護計時
void InitUnLoadNewAutoTrayTask(int Pos)                                         //ChungHung 20140317 add Auto Retest
{
    iUnLoadNewAutoTrayTask[Pos]=1;
}
//------------------------------------------------------------------------------
bool DoUnLoadNewAutoToStack(int Pos)                                            //ChungHung 20140317 add Auto Retest
{
    int ret;
    AnsiString Str;
    int iMot=iMMAuto[Pos];
    int &Task=iUnLoadNewAutoTrayTask[Pos];
    static bool bAlreadyIn[MAX_AUTO_TRAY]={false, false, false, false, false, false};                                   //JerryYang 20180628 (wei) : add二段氣缸先on off一次避免爆衝
    static bool bUnLoadSafetyTimerStarted[MAX_AUTO_TRAY]={false, false, false, false, false, false};  //AI(general) 20260323 (RogerYang) : UpSafe sensor 5秒保護計時旗標
    bool bflag=false;
    switch(Task)
    {
        case 1:
            #ifdef SOFT_SIMULTE
                Task=50;
            #else
            {
                bool bPreDeteOn =Sen[SnAutoPreDete[Pos]].IsOn();                //AI(general) 20260323 (RogerYang) : 三顆 sensor 到位，重置保護計時
                bool bTrayCarOn =Sen[SnAutoTrayCar[Pos]].IsOn();
                bool bUpSafeOn  =Sen[SnAutoUpSafe[Pos]].IsOn();

                if((bPreDeteOn &&
                    bTrayCarOn &&
                    bUpSafeOn) ||
                   LastSet.iRealDummy==DUMMY)
                {
                    bUnLoadSafetyTimerStarted[Pos]=false;
                    Task=50;
                }
                else                                                            //sensor 未全部到位
                {
                    if(bUnLoadSafetyTimerStarted[Pos]==false)
                    {
                        hUnLoadSafetyCheckTimer[Pos].SetSecAndOn(5);            //啟動5秒保護計時
                        bUnLoadSafetyTimerStarted[Pos]=true;
                    }
                    else if(hUnLoadSafetyCheckTimer[Pos].Off())                 //超過5秒仍未到位
                    {
                        AnsiString sAlarmSensorAlias="";
                        if(bPreDeteOn==false)
                            sAlarmSensorAlias=Sen[SnAutoPreDete[Pos]].Name;

                        if(bTrayCarOn==false)
                        {
                            if(sAlarmSensorAlias!="")
                                sAlarmSensorAlias+="/";
                            sAlarmSensorAlias+=Sen[SnAutoTrayCar[Pos]].Name;
                        }

                        if(bUpSafeOn==false)
                        {
                            if(sAlarmSensorAlias!="")
                                sAlarmSensorAlias+="/";
                            sAlarmSensorAlias+=Sen[SnAutoUpSafe[Pos]].Name;
                        }

                        bUnLoadSafetyTimerStarted[Pos]=false;
                        ShowErrorMessage(sJAM1158[Pos], K_RETRY, iMMAuto[Pos], false,
                                         sAlarmSensorAlias);                    //帶出異常 sensor alias/name
                    }
                }
            }
            #endif
            AutoCylinderMiddle(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                        //Steven 20140409 : AutoCylinderXX Add Reset
            if(bARTUnloaderUseTwoCylin(Pos))                                                                            //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
            {
                Cylinder[C_Auto_Selector[Pos]].On();
                DoUnLoadNewAutoToStackDelay[Pos].SetMSAndOn(1000);
                bAlreadyIn[Pos]=false;
            }

            break;
        case 45:
            if(DoUnLoadNewAutoToStackDelay[Pos].Off()==true)                    //Sam 20211115 : 確保汽缸上前的狀態皮帶是一直再帶動轉
            {
                Task=50;
            }
            break;
        case 50:
            #ifndef SOFT_SIMULTE
            if(TrayMoveStatus(Pos+3,__FUNC__)==0  &&
               LastSet.iRealDummy!=DUMMY)                                                                               //Sam 20211115 : 確保汽缸上前的狀態皮帶是一直再帶動轉
            {
                TrayMoveOut(true, Pos+3);
                Task=45;
                DoUnLoadNewAutoToStackDelay[Pos].SetSecAndOn(2);
                break;
            }
            #endif
            if(bARTUnloaderUseTwoCylin(Pos))                                                                            //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
            {
                if(bAlreadyIn[Pos]==false)
                {
                    if(DoUnLoadNewAutoToStackDelay[Pos].Off()==true)
                    {
                        Cylinder[C_Auto_Selector[Pos]].Off();
                        bAlreadyIn[Pos]=true;
                    }
                    else
                    {
                        bAlreadyIn[Pos]=false;
                    }
                    break;
                }
            }
            else if(bARTUnloaderUseOneCylin(Pos))                                                                       //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
            {
                if(AutoCylinderMiddle(Pos, C_Auto_Selector[Pos], C_Auto_Up[Pos]))                                       //升到分離高度
                {
                    TrayMoveIn(false, Pos+3);
                    Task=100;
                }
                break;
            }

            if(AutoCylinderMiddle(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))                                           //升到分離高度
            {
                TrayMoveIn(false, Pos+3);
                Task=100;
            }
            break;
        case 100:
            if(LastSet.iRealDummy==DUMMY ||
               Cylinder[C_AutoZ_Select[Pos]].Push())
            {
                DoUnLoadNewAutoToStackDelay[Pos].SetSecAndOn(Ld_UldDelayTime.ULD_TrayBackDelay);
                Task=150;
            }
            break;
        case 150:
            if(DoUnLoadNewAutoToStackDelay[Pos].Off())
            {
                AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                        //Steven 20140409 : AutoCylinderXX Add Reset
                Task=200;
            }
            break;
        case 200:
            if(bARTUnloaderUseTwoCylin(Pos))                                                                            //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                bflag=AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]);                                        //Steven 20140409 : AutoCylinderXX Add Reset
            else if(bARTUnloaderUseOneCylin(Pos))                                                                       //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                bflag=AutoCylinderUp(Pos, C_Auto_Selector[Pos], C_Auto_Up[Pos]);                                        //Steven 20140409 : AutoCylinderXX Add Reset
            else
                bflag=AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]);                                        //Steven 20140409 : AutoCylinderXX Add Reset

            if(bflag)
            {
                Task=300;
            }
            break;
        case 300:
            if(Cylinder[C_AutoZ_Select[Pos]].Pop())
            {
                Task=350;
            }
            break;
        case 350:                                                               //kevin 20161129  Auto 收tray sensor沒調好 需alarm
            if((Sen[SnAutoPreDete[Pos]].IsOff() &&
                Sen[SnAutoTrayCar[Pos]].IsOff() &&
                Sen[SnAutoUpSafe[Pos]].IsOff()) ||
               LastSet.iRealDummy==DUMMY)
            {
                DoUnLoadNewAutoToStackDelay[Pos].SetSecAndOn(Ld_UldDelayTime.ULD_TrayBackDelay);
                MOT[iMot].ClearTray(__FUNC__);                                  //ChungHung 20140519 開啟AutoRtest時 Auto2在Tray End 後 會被清掉
                Task=400;
            }
            else
            {
                #ifdef SOFT_SIMULTE
                Task=400;
                #else
                ShowErrorMessage(sMES1121[Pos], K_RETRY, iMMAuto_Car[Pos]);
                #endif
            }
            break;
        case 400:
            if(DoUnLoadNewAutoToStackDelay[Pos].Off())
            {
                if(bARTUnloaderUseTwoCylin(Pos))                                                                        //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                    AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                 //Steven 20140409 : AutoCylinderXX Add Reset
                else if(bARTUnloaderUseOneCylin(Pos))                                                                   //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                    AutoCylinderLower(Pos, C_Auto_Selector[Pos], C_Auto_Up[Pos], true);                                 //Steven 20140409 : AutoCylinderXX Add Reset
                else
                    AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                 //Steven 20140409 : AutoCylinderXX Add Reset
                Task=401;
            }
            break;
        case 401:
            if(IniConfig.bP37bAutoCylinderUP)                                                                           //JerryYang 20200916 : tray feed時常態在上
            {
                bflag=true;
            }
            else
            {
                if(bARTUnloaderUseTwoCylin(Pos))                                                                        //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                    bflag=AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]);                                 //Steven 20140409 : AutoCylinderXX Add Reset
                else if(bARTUnloaderUseOneCylin(Pos))                                                                   //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                    bflag=AutoCylinderLower(Pos, C_Auto_Selector[Pos], C_Auto_Up[Pos]);                                 //Steven 20140409 : AutoCylinderXX Add Reset
                else
                    bflag=AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]);                                 //Steven 20140409 : AutoCylinderXX Add Reset
            }

            if(bflag)
            {
                Task=402;
                DoUnLoadNewAutoToStackDelay[Pos].SetSecAndOn(3);
            }
            break;
        case 402:
            if(bHandlerPause)                                                   //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                DoUnLoadNewAutoToStackDelay[Pos].SetSecAndOn(3);
            }

            if(Sen[SnAutoPreDete[Pos]].IsOn() && Sen[SnAutoTrayCar[Pos]].IsOn())
            {
                ret=ShowErrorMessage(sJAM1113[Pos], K_RETRY|K_SKIP, iMMAuto_Car[Pos]);
                if(ret==K_SKIP)
                    Task=1;
            }
            else if(DoUnLoadNewAutoToStackDelay[Pos].Off())
            {
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
// 送空 tray
//------------------------------------------------------------------------------
void InitAutoTrayReceiveTask(int Pos)
{
    iAutoTrayReceiveTask[Pos]=1;
}
//------------------------------------------------------------------------------
void DoAutoTrayReceive(int Pos)
{
    int &Task=iAutoTrayReceiveTask[Pos];
    switch(Task)
    {
        case 1:
            InitAutoTrayToFrontTask(Pos);
            Task=100;
            break;
        case 100:
            if(DoAutoTrayToFront(Pos))
            {
                iReceiveAutoTray[Pos]=2;
                if(MOT[iMMAuto_Car[Pos]].fHasTray)
                {
                    InitUnLoadNewAutoTrayTask(Pos);
                    Task=200;
                }
                else
                {
                    Task=1;
                    iReceiveAutoTray[Pos]=0;
                }
            }
            break;
        case 200:
            if(DoUnLoadNewAutoToStack(Pos))
            {
                MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);
                iReceiveAutoTray[Pos]=0;
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
//kevin 20150610 ART 判斷是否最後一盤
//------------------------------------------------------------------------------
void InitTrayZAutoTrayToWaitTask()
{
    iTrayZLoadTrayToWaitTask=1;
}
//--------------------------------------------------------------------------------
int DoTrayZAutoTrayToWait(int Pos)
{
    bool bAutoLoadHasTray=false;
    int &Task=iTrayZAutoTrayToWaitTask[Pos];                                    //kevin 20170809 (Steven) add
    AnsiString Str1, Str2;

    switch(Task)
    {
        case 1:
            AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);    //Steven 20140409 : AutoCylinderXX Add Reset
            Task++;

            break;
        case 2:
            if(AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))       //升到接盤位置
            {
                Task=100;
            }
            break;
        case 100:
            if(LastSet.iRealDummy==DUMMY)                                       //Steven 20100618 : Dummy Run時，不要放下Tray
                Task=200;
            else if(Cylinder[C_AutoZ_Select[Pos]].Push())
                Task=200;
            AutoRTDelay.SetMSAndOn(1000);
            break;
        case 200:
            if(AutoRTDelay.Off())
            {
                AutoCylinderMiddle(1, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                      //Steven 20140409 : AutoCylinderXX Add Reset
                Task=201;
            }
            break;
        case 201:
            if(AutoCylinderMiddle(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))   //kevin 20170810 (Steven) 升到分離高度 1-->Pos
            {
                if(LastSet.iRealDummy!=DUMMY && IniConfig.bLastLoaderNoInSide)
                {
                    bAutoLoadHasTray=Sen[SnAutoTrayHasTray[Pos]].IsOn();
                    if(bAutoLoadHasTray)                                        //kevin 20150613 尾盤不下去,需到分離高度去確認一盤不能偵測到
                    {
                        Task=300;
                        break;
                    }
                    else if(CosFunction.bUseSCKART==false &&
                            iAutoTrayCount[Pos]>0)                              //kevin 20170810 (Steven) add 放空盤
                    {
                        Str1.sprintf("Auto %d Tray, Please Cover One Tray", Pos+1);
                        Str2.sprintf("Auto %d Tray, 請放空盤", Pos+1);
                        ShowMyMessage(Str1,Str2);
                        Task=1;
                        break;
                    }
                }
                Task=1000;
            }
            break;
        case 300:
            if(Cylinder[C_AutoZ_Select[Pos]].Pop())
            {
                Task=400;
            }
            break;
        case 400:
            if(SW[SwACTrayY].Status()==false)
            {
                MOT[MMTrayZ].fHasTray=true;
                Task=1;
                return 1;
            }
            break;
        case 1000:                                                              //jou 2010-01-24 start : 如果Loader是最後一盤，不再load進去。
            if(AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))       //升到接盤位置
            {
                Task=1100;
            }
            break;
        case 1100:
            if(Cylinder[C_AutoZ_Select[Pos]].Pop())
            {
                Task=1200;
            }
            break;
        case 1200:
            if(AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))
            {
                Task=1;
                return 2;
            }
            break;
    }
    return 0;
}
//==============================================================================
