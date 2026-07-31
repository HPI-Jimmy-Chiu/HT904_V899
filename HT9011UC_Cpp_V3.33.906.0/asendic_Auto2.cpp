// =============================================================================
//  asendic_Auto2.cpp  --  Auto2-tray stack-feed state machine
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/asendic_Auto2.cpp (749 ln)
//  Translation wave: W906-W7-L1 (asendic_* feed-SM family, Auto2 cluster)
//  Translator: AI(W906-W7-L1) 20260729
//
//  FAITHFUL TRANSLATION
//  --------------------
//  Names, 1-based numeric Task states, switch(Task) semantics, and the case
//  fall-throughs are preserved VERBATIM, same idiom as the asendic_Empty.cpp
//  W6.1 canary this wave follows.  Each Do* owns its own extern cursor int.
//
//  ALL 12 golden functions are translated (golden asendic_Auto2.cpp:33, 38, 178,
//  183, 315, 321, 409, 414, 595, 600, 710, 715 -- 6 Init* + 6 Do*).
//
//  AI(W906-W7-L1fix) 20260729: the fall-through enumeration here used to name
//  only TWO and read as exhaustive.  There are exactly THREE in the golden file
//  (verified by scanning every `case N:` whose preceding statement is not
//  break/return):
//    (1) DoAuto2TrayToFront  case 1 -> case 100  (golden :194)
//    (2) DoAuto2TrayToRear   case 1 -> case 100  (golden :333)
//    (3) DoAuto2             case 1 -> case 2    (golden :433)  <-- was unlabelled
//  (1) and (2) are behaviourally LIVE -- the next case's body genuinely runs in
//  the same tick, and tests/test_w7_l1_auto2.cpp pins both by asserting the
//  one-tick cursor jump (1 -> 101 and 1 -> 200 respectively).  (3) is
//  behaviourally INERT: case 1's only action is arming DoAuto2Delay to 1000 ms
//  and case 2's only action is gated on that same timer's Off(), which cannot be
//  true in the tick that just armed it -- so it is NOT pinned by a test (a test
//  that cannot fail would be worse than none).  All three are preserved verbatim
//  regardless; the distinction is documented, not acted on.
//
//  DEPENDENCY STATUS (see plan SS6 W7-L1 + SS10 correction note):
//  asendic_Auto2.cpp has ZERO outgoing calls into any of its 6 asendic_*
//  siblings (verified: grepped every function this file calls against the
//  full defined-symbol set of all 7 golden asendic_*.cpp files -- no hit
//  outside this file).  It is therefore translatable standalone, unlike the
//  Loader<->Loader_RT<->Scanner and Auto<->Auto_RT<->Color clusters.  The
//  header DOES extern-declare InitUnLoadNewAutoTrayTask(int)/
//  DoUnLoadNewAutoToStack(int) (owned by asendic_Auto_RT.cpp), but this .cpp
//  never calls them -- see asendic_Auto2.h banner.
//
//  INCLUDES (BCB6 original vs this tree, same substitution table as
//  asendic_Empty.cpp):
//    BCB6 pulled: MachineDefine.h, asendic_Auto2.h, asendic.h, MyMotor.h,
//    mycylin.h, main.h, mymessbox.h, note.h, mysensor.h, myswitch.h,
//    acatchtray.h, cprod.h, OCR.h, OCRInsp.h, cMyDB.h, cmydef.h, common.h,
//    AGV.h.  Replaced by:
//      Motor/mymotor.h   (MOT[])               -- W4 HAL
//      mycylin.h         (Cylinder[])           -- W6.0 HAL gap
//      mysensor.h        (Sen[])                -- W4-IO HAL
//      myswitch.h        (SW[])                 -- W4-IO HAL
//      asendic.h          (golden declares AutoCylinderUp/Middle/Lower here at
//                          asendic.h:19-21; in THIS tree those three decls were
//                          relocated to acatchtray_shims.h, so this header is
//                          included only for the sibling include chain golden uses)
//      acatchtray.h        (WhichAutoNeedTray only -- golden acatchtray.h:8)
//      FormsFacade.h       (fAGV->IsATK_AMR())    -- non-VCL satellite stub
//      canary_support.h    (LastSet shim, ShowErrorMessage/ShowMyMessage)
//      common.h            (MySleepEx)
//      cprod.h/cpublic.h/cmydef.h (Prod/TrayForm/enums/globals) -- W0-TAIL globals
//    Not needed offline: main.h (TfMain), mymessbox.h (MyMessageBox VCL -- no
//    MyMessageBox call in this file), note.h (ShowErrorMessage/ShowMyMessage
//    now live in canary_support.h), OCR.h/OCRInsp.h/cMyDB.h/AGV.h (no OCR/DB/
//    AGV symbol referenced in this file's body).
//
//  GATED THIS WAVE: none -- every branch in this file's 12 functions is either
//  plain global/HAL logic or an #ifdef SOFT_SIMULTE / #ifndef SOFT_SIMULTE
//  pair (both arms already present in golden; SOFT_SIMULTE is not defined in
//  this build, matching the Empty canary's posture, so the #ifndef arm compiles).
//
//  STAND-IN THIS FILE DEPENDS ON -- AutoCylinderUp / AutoCylinderMiddle /
//  AutoCylinderLower ARE NO-OP STUBS, NOT REAL BODIES.
//  AI(W906-W7-L1fix) 20260729: the include line below used to claim "real bodies,
//  acatchtray.cpp".  Both halves of that were wrong and are corrected here:
//    * WHERE GOLDEN DEFINES THEM: asendic.cpp:562 / :767 / :937 (NOT acatchtray.cpp
//      -- acatchtray.cpp only CALLS them, e.g. golden acatchtray.cpp's Auto lifter
//      block).  They are the Auto1..Auto6 lifter state machines, siblings of the
//      CylinderUp/Middle/Lower trio for Load/Empty/Color.
//    * WHAT THIS TREE ACTUALLY LINKS: acatchtray_shims.cpp's
//      `bool AutoCylinderUp(int,int,int,bool) { return true; }` (and Middle/Lower,
//      identical).  This tree's asendic.cpp is a declared SIM-SHAPE partial that
//      carries only the non-Auto CylinderUp/Middle/Lower; the Auto* lifter SMs are
//      NOT translated (they need iLifterTask[][]/LifterTime[][]/iTrayZMotor[]/
//      LOAD_Z_USE_MOTOR[]/Prod.TrayZ_Up[], same god-stack asendic.cpp's own banner
//      defers).
//  CONSEQUENCE FOR THIS FILE: every `if(AutoCylinderXxx(...))` guard here is
//  unconditionally true offline, so the lifter waits at cases 50/201/410 (feed)
//  and 50/200/401 (unload) complete in one tick and no lifter timing/alarm
//  behaviour is exercised.  tests/test_w7_l1_auto2.cpp asserts only that these
//  call sites are REACHED, and says so explicitly.  Replacing the stubs with the
//  real golden bodies is a separate wave (see docs/W7-UI-SKIPPED.md, W7-L1).
//
//  Big5: every Chinese comment decoded cleanly via cp950 and is preserved as
//  UTF-8.  NO U+FFFD is emitted.
// =============================================================================
#include "asendic_Auto2.h"
#include "csystem.h"
//---------------------------------------------------------------------------
#include "asendic.h"
#include "Motor/mymotor.h"
#include "mycylin.h"
#include "mysensor.h"
#include "myswitch.h"
#include "acatchtray.h"         // WhichAutoNeedTray (golden acatchtray.h:8)
// AI(W906-W7-L1fix) 20260729: corrected attribution -- these are OFFLINE NO-OP
// STUBS (acatchtray_shims.cpp: `return true;`), not real bodies.  Golden defines
// them in asendic.cpp:562/767/937, declared golden asendic.h:19-21.  See the
// STAND-IN block in the banner above for what that costs this file's coverage.
#include "acatchtray_shims.h"   // AutoCylinderUp/Middle/Lower -- no-op stubs
#include "cprod.h"
#include "cmydef.h"
#include "cpublic.h"
#include "common.h"             // MySleepEx
#include "FormsFacade.h"        // fAGV (was AGV.h / VCL TfAGV)
#include "canary_support.h"     // LastSet, ShowErrorMessage, ShowMyMessage
// BCB6 also pulled: MachineDefine.h, MyMotor.h, main.h, mymessbox.h, note.h,
//                   OCR.h, OCRInsp.h, cMyDB.h  -- all swapped/gated above.

// ---------------------------------------------------------------------------
//  golden asendic_Auto2.cpp:29-31
// ---------------------------------------------------------------------------
int iAuto2ReceiveTask=1;                                                        //kevin 20120718
int iLoadNewAuto2TrayToCarTask=1;
TQPF_Timer Auto2Delay;

// ---------------------------------------------------------------------------
//  InitLoadNewAuto2TrayToCarTask -- golden asendic_Auto2.cpp:33-36
// ---------------------------------------------------------------------------
void InitLoadNewAuto2TrayToCarTask()
{
    iLoadNewAuto2TrayToCarTask=1;
}
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  DoLoadNewAuto2TrayToCar -- golden asendic_Auto2.cpp:38-173
//  Car-stack lifter feed: rise to top, split to middle, feed under the loader
//  Z-select cylinder, lower back down, and confirm the Auto2 stack got a tray.
// ---------------------------------------------------------------------------
bool DoLoadNewAuto2TrayToCar()
{
    int ret;
    int &Task=iLoadNewAuto2TrayToCarTask;
    static bool bAuto2HasDuplicateError=false;                                  //Steven 20120220 : Empty Tray重複Alarm Flag
    switch(Task)
    {
        case 1:
            if(MOT[MMAuto2].fHasTray)
            {
                Auto2Delay.SetMSAndOn(1000);
                Task=400;
                break;
            }

            if(Sen[SnAuto2TrayHasTray].IsOff()==false ||
               LastSet.iRealDummy==DUMMY)
            {
                if(Sen[SnAuto2PreDete].IsOff()==false   ||
                   Sen[SnAuto2_Tray_Car].IsOff()==false)
                {
                    MOT[MMAuto2_Car].SetTray(NULL_IC, __FUNC__);
                    MOT[MMAuto2Z].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }
                Task=50;
                AutoCylinderUp(1, C_Auto2_Selector, C_Auto2_Up, true);          //Steven 20140409 : AutoCylinderXX Add Reset
            }
            else
            {
                if(MOT[MMAuto2_Car].fHasTray==false)                            //jou 2011-12-21 有發生Tray feed完,汽缸還在上面,所以做完下降
                {
                    if(Cylinder[C_Auto2_Up].OffStatus()==false)
                    {
                        if(Sen[SnAuto2_Tray_Car].IsOff() && Sen[SnAuto2PreDete].IsOff())
                        {
                            Cylinder[C_Auto2_Up].Off();
                            Cylinder[C_Auto2_Selector].Off();
                            break;
                        }
                    }
                }
                ShowErrorMessage("MES1024", K_RETRY, MMAuto2_Car);
            }
            break;
        case 50:
            if(AutoCylinderUp(1, C_Auto2_Selector,C_Auto2_Up))                  //升到最高位置
            {
                MOT[MMAuto2Z].ClearTray(__FUNC__);
                Task=100;
            }
            break;
        case 100:
            if(LastSet.iRealDummy==DUMMY)                                       //Steven 20100618
                Task=200;
            else if(Cylinder[C_Auto2LoaderZ_Select].Push())
                Task=200;
            Auto2Delay.SetMSAndOn(1000);
            break;
        case 200:
            if(Auto2Delay.Off())
            {
                AutoCylinderMiddle(1, C_Auto2_Selector, C_Auto2_Up, true);      //Steven 20140409 : AutoCylinderXX Add Reset
                Task=201;
            }
            break;
        case 201:
            if(AutoCylinderMiddle(1, C_Auto2_Selector, C_Auto2_Up))             //升到分離高度
            {
                if(bEmptyPause)
                {
                    bEmptyPause=false;
                    ShowMyMessage("Auto2 split wait", "Auto2 分離暫停", "DoLoadNewAuto2TrayToCar 200");
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
            else if(Cylinder[C_Auto2LoaderZ_Select].Pop())
            {
                Task=400;
            }
            Auto2Delay.SetMSAndOn(1000);                                        //Steven 20130904 Start: 加入Time Out 避免Hang Up
            break;
        case 400:
            if(Auto2Delay.Off())
            {
                AutoCylinderLower(1, C_Auto2_Selector, C_Auto2_Up, true);       //Steven 20140409 : AutoCylinderXX Add Reset
                Task=410;
            }
            break;
        case 410:
            if(AutoCylinderLower(1, C_Auto2_Selector, C_Auto2_Up))              //降到最低位置
            {
                Auto2Delay.SetMSAndOn(1000);
                Task=420;
            }
            break;
        case 420:
            if(bHandlerPause)                                                                                                                                   //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                Auto2Delay.SetMSAndOn(1000);
            }

            if((Sen[SnAuto2PreDete].IsOn() && Sen[SnAuto2_Tray_Car].IsOn())    ||
               (Sen[SnAuto2PreDete].IsOn() && Sen[SnAutoUpSafedetect1].IsOn()) ||
                LastSet.iRealDummy==DUMMY)
            {
                MOT[MMAuto2_Car].SetTray(NULL_IC, __FUNC__);
                MOT[MMAuto2Z].ClearTray(__FUNC__);
                bAuto2HasDuplicateError=false;
                Task=1;
                return true;
            }
            else if(Auto2Delay.Off())
            {
                ret=ShowErrorMessage("JAM1213", K_RETRY|K_SKIP, MMAuto2_Car, bAuto2HasDuplicateError, "DoLoadNewAuto2TrayToCar");                               //Steven 20150108 : JAM1201 --> JAM1213
                bAuto2HasDuplicateError=true;
                if(ret==K_SKIP)
                {
                    MOT[MMAuto2Z].ClearTray(__FUNC__);
                    bAuto2HasDuplicateError=false;
                    Task=1;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  golden asendic_Auto2.cpp:175-177
// ---------------------------------------------------------------------------
int iAuto2TrayToFrontTask=1;
TQPF_Timer hAuto2TrayToFront;
TQPF_Timer hAuto2TrayToFrontForDummy;
// ---------------------------------------------------------------------------
//  InitAuto2TrayToFrontTask -- golden asendic_Auto2.cpp:178-181
// ---------------------------------------------------------------------------
void InitAuto2TrayToFrontTask()
{
    iAuto2TrayToFrontTask=1;
}
//------------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  DoAuto2TrayToFront -- golden asendic_Auto2.cpp:183-310
//  Feed the Auto2 tray forward under the CW belt to the front edge stop.
//  GOLDEN QUIRK: case 1 falls through into case 100 with no break (verbatim --
//  the timer arm at case 1 and the guard body at case 100 run in the same tick).
// ---------------------------------------------------------------------------
bool DoAuto2TrayToFront()
{
    int &Task=iAuto2TrayToFrontTask;
    int ret;
    static bool bAuto2HasDuplicateError=false;                                  //Steven 20120220 : Empty Tray重複Alarm Flag
    switch(Task)
    {
        case 1:
            hAuto2TrayToFront.SetMSAndOn(20000);
            hAuto2TrayToFrontForDummy.SetMSAndOn(5000);
            Task=100;
        case 100:
            if(bHandlerPause)                                                                                           //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                Task=1;
                break;
            }

            Cylinder[C_Auto2Side_Fixer].Off();
            Cylinder[C_Auto2EdgePush].Off();
            if(SUPPORT_2_EMPTY_EMPTY)
            {
                SW[SwACAuto2].Off();
                MySleepEx(100,false);
                SW[SwACAuto2CW].Off();
                MOT[MMAuto2_Car].SetTray(NULL_IC, __FUNC__);
                MOT[MMAuto2].ClearTray(__FUNC__);
                bAuto2HasDuplicateError=false;                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                Task=1;
                return true;
            }

            SW[SwACAuto2].On();
            SW[SwACAuto2CW].On();

            if(Sen[SnAuto2_Tray_Car].IsOn()==false &&
               Sen[SnAuto2TrayDetect].IsOff()==false)
            {
                SW[SwACAuto2].Off();
                MySleepEx(100, false);
                SW[SwACAuto2CW].Off();
                bAuto2HasDuplicateError=false;                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                Task=101;
                hAuto2TrayToFrontForDummy.SetMSAndOn(10000);                                                            //kevin 20120904
                break;
            }

            if(hAuto2TrayToFrontForDummy.Off())
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    SW[SwACAuto2].Off();
                    MySleepEx(100, false);
                    SW[SwACAuto2CW].Off();
                    MOT[MMAuto2_Car].ClearTray(__FUNC__);
                    MOT[MMAuto2].ClearTray(__FUNC__);
                    bAuto2HasDuplicateError=false;                                                                      //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                    return true;
                }
            }

            if(hAuto2TrayToFront.Off())
            {
                ret=ShowErrorMessage("JAM1212", K_SKIP|K_RETRY, MMAuto2, bAuto2HasDuplicateError, __FUNC__);            //Steven 20120925 : JAM1009 -> JAM1212
                if(ret==K_RETRY)
                {
                    bAuto2HasDuplicateError=true;                                                                       //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                }
                else
                {
                    SW[SwACAuto2].Off();
                    MySleepEx(100,false);
                    SW[SwACAuto2CW].Off();
                    bAuto2HasDuplicateError=false;                                                                      //Steven 20120220 : Empty Tray重複Alarm Flag
                    MOT[MMAuto2].ClearTray(__FUNC__);
                    MOT[MMAuto2_Car].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }
            }
            break;
        case 101:
            if(Sen[SnAuto2CWPreDetect].IsOff()==false &&
               Sen[SnAuto2TrayDetect].IsOff()==false)
            {
                Cylinder[C_Auto2EdgePush].On();
                Task=102;
                break;
            }
            else if(hAuto2TrayToFrontForDummy.Off())                                                                    //kevin 20120904
            {
                ShowErrorMessage("JAM1212", K_RETRY, MMAuto2, bAuto2HasDuplicateError, __FUNC__);                       //Steven 20120925 : JAM1209 -> JAM1212
                Task=100;
            }
            break;
        case 102:
            if(Cylinder[C_Auto2Side_Fixer].Push())
            {
                hAuto2TrayToFrontForDummy.SetMSAndOn(100);
                Task=103;
            }
            break;
        case 103:
            if(hAuto2TrayToFrontForDummy.Off())
            {
                hAuto2TrayToFrontForDummy.SetMSAndOn(100);
                Cylinder[C_Auto2Side_Fixer].Off();
                Task=104;
            }
            break;
        case 104:
            if(hAuto2TrayToFrontForDummy.Off())
            {
               Cylinder[C_Auto2EdgePush].Off();
               Task=105;
            }
            break;
        case 105:
            MOT[MMAuto2].ClearTray(__FUNC__);
            MOT[MMAuto2_Car].ClearTray(__FUNC__);
            bAuto2HasDuplicateError=false;                                      //Steven 20120220 : Empty Tray重複Alarm Flag
            Task=1;
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  golden asendic_Auto2.cpp:312-314
// ---------------------------------------------------------------------------
int iAuto2TrayToRearTask=1;
TQPF_Timer hAuto2TrayToRear;
TQPF_Timer hAuto2TrayToRearForDummy;
// ---------------------------------------------------------------------------
//  InitAuto2TrayToRearTask -- golden asendic_Auto2.cpp:315-318
// ---------------------------------------------------------------------------
void InitAuto2TrayToRearTask()
{
    iAuto2TrayToRearTask=1;
}
//------------------------------------------------------------------------------
//---- 收空TRAY----
// ---------------------------------------------------------------------------
//  DoAuto2TrayToRear -- golden asendic_Auto2.cpp:321-404
//  Pull an empty Auto2 tray back off the fixer under the CW belt.
//  GOLDEN QUIRK: case 1 falls through into case 100 with no break (same idiom
//  as DoAuto2TrayToFront -- verbatim, not an omission).
// ---------------------------------------------------------------------------
bool DoAuto2TrayToRear()
{
    static bool bAuto2HasDuplicateError=false;                                  //Steven 20120220 : Empty Tray重複Alarm Flag
    int &Task=iAuto2TrayToRearTask;
    int ret;
    switch(Task)
    {
        case 1:
            hAuto2TrayToRearForDummy.SetMSAndOn(5000);
            hAuto2TrayToRear.SetMSAndOn(20000);
            Cylinder[C_Auto2EdgePush].Off();
            Task=100;
        case 100:
            if(Cylinder[C_Auto2Side_Fixer].Pop())
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

            SW[SwACAuto2].On();
            SW[SwACAuto2CW].Off();
            if(Sen[SnAuto2TrayDetect].IsOn())
            {
                Task=450;
                break;
            }

            if(hAuto2TrayToRearForDummy.Off())
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    Task=450;
                    break;
                }
            }

            if(hAuto2TrayToRear.Off())
            {
                ret=ShowErrorMessage("JAM1201", K_SKIP|K_RETRY, MMAuto2_Car, bAuto2HasDuplicateError, "DoAuto2TrayToRear_200");                                 //Steven 20120925 : JAM1009 -> JAM1201
                if(ret==K_RETRY)
                {
                    bAuto2HasDuplicateError=true;                                                                                                               //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                }
                else
                {
                    bAuto2HasDuplicateError=false;                                                                                                              //Steven 20120220 : Empty Tray重複Alarm Flag
                    MOT[MMAuto2_Car].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }
            }
            break;
        case 450:
            Cylinder[C_Auto2EdgePush].Off();
            hAuto2TrayToRear.SetSecAndOn(Ld_UldDelayTime.LD_TrayArrivalDely);
            Task=460;
            break;
        case 460:
            if(hAuto2TrayToRear.Off())
            {
                SW[SwACAuto2CW].Off();
                SW[SwACAuto2].Off();
                Task=500;
            }
            break;
        case 500:
            if(Cylinder[C_Auto2Side_Fixer].Pop())
            {
                MOT[MMAuto2].SetTray(NULL_IC, __FUNC__);
                MOT[MMAuto2_Car].ClearTray(__FUNC__);                           //Steven 20111109 : 已入Tray卻未清掉資料
                Task=1;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  golden asendic_Auto2.cpp:406-408
// ---------------------------------------------------------------------------
int iAutoAuto2Task=1;                                                           //kevin 20120718 auto2 送空 tray
TQPF_Timer DoAuto2Delay;
TQPF_Timer CheckAuto2TrayDelay;
// ---------------------------------------------------------------------------
//  InitAuto2Task -- golden asendic_Auto2.cpp:409-412
// ---------------------------------------------------------------------------
void InitAuto2Task()
{
    iAutoAuto2Task=1;
}
//------------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  DoAuto2 -- golden asendic_Auto2.cpp:414-592
//  Top Auto2 dispatcher: receive-gate, then supply-new-tray state machine that
//  drives DoLoadNewAuto2TrayToCar() (feed) / DoAuto2TrayToRear() (recycle).
//  GOLDEN QUIRK: case 1 falls through into case 2 with no break (verbatim --
//  golden :433, the THIRD fall-through in this file; see banner).  Behaviourally
//  inert: case 1 arms DoAuto2Delay to 1000 ms and case 2 only acts on that same
//  timer's Off(), which cannot be true in the arming tick.  AI(W906-W7-L1fix)
//  20260729: labelled -- it was preserved but unlabelled, while the banner's
//  two-item fall-through list read as exhaustive.
// ---------------------------------------------------------------------------
void DoAuto2()
{
    int i;
    int &Task=iAutoAuto2Task;
    static bool bAuto2HasDuplicateError=false;                                  //Steven 20120220 : Empty Tray重複Alarm Flag
    if(iReceiveAuto2Tray!=0)
    {
        DoAuto2Receive();                                                       //收auto2 空tray
        return;
    }

    if(fAuto2CanSupplyNewTray==false)
        return;

    switch(Task)
    {
        case 1:
            DoAuto2Delay.SetMSAndOn(1000);
            Task=2;
        case 2:
            if(DoAuto2Delay.Off())
               Task=3;
            break;
        case 3:
            if(MOT[MMAuto2_Car].fHasTray ||
               Sen[SnAuto2TrayDetect].IsOn() ||
               LastSet.iRealDummy==DUMMY)
            {
                Task=20;
                break;
            }
            else
            {
                MOT[MMAuto2].ClearTray(__FUNC__);
            }

            if(MOT[MMAuto2].fHasTray==false)
            {
                Task=100;

                i=WhichAutoNeedTray();                                          //which auto need tray
                if(MOT[MMTrayY].fHasTray && MOT[MMTrayY].HasIC()==false && i==0)
                {
                    Task=1;
                    break;
                }
            }
            break;
        case 20:
            Cylinder[C_Auto2Side_Fixer].Off();
            Cylinder[C_Auto2EdgePush].Off();
            Task=30;
            break;
        case 30:
            if(Cylinder[C_Auto2Side_Fixer].Pop())
                Task=40;
            break;
        case 40:
            if(Sen[SnAuto2FixCyPush].IsOff()==false)
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    bAuto2HasDuplicateError=false;                                                                      //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=50;
                }
                else
                {
                    Cylinder[C_Auto2Side_Fixer].Off();
                    ShowErrorMessage("JAM1203", K_RETRY, MMAuto2_Car, bAuto2HasDuplicateError, __FUNC__);               //Steven 20120925 : JAM1009 -> JAM1203
                    bAuto2HasDuplicateError=true;                                                                       //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                }
            }
            else
            {
                bAuto2HasDuplicateError=false;                                                                          //Steven 20120220 : Auto2 Tray重複Alarm Flag
                Task=50;
            }
            break;
        case 50:
            if(Cylinder[C_Auto2Side_Fixer].Pop())
            {
                #ifdef SOFT_SIMULTE
                MOT[MMAuto2_Car].ClearTray(__FUNC__);
                MOT[MMAuto2].SetTray(NULL_IC, __FUNC__);
                #endif
                Task=60;
            }
            break;
        case 60:
            if(MOT[MMAuto2].fHasTray==false)
            {
                DoAuto2Delay.SetMSAndOn(1000);
                Task=70;
            }
            else
            {
                CheckAuto2TrayDelay.SetMSAndOn(100);
                Task=65;
            }
            break;
        case 65:
            if(CheckAuto2TrayDelay.Off())
            {
                if(MOT[MMAuto2].fHasTray==true &&
                   LastSet.iRealDummy>=HAS_TRAY &&
                   Sen[SnAuto2TrayDetect].IsOff() &&
                   Sen[SnAuto2PreDete].IsOff())
                {
                    MOT[MMAuto2].ClearTray(__FUNC__);
                }
                Task=60;

                if(MOT[MMTrayY].fHasTray &&
                   MOT[MMTrayY].HasIC()==false &&
                   MOT[MMAuto2].fHasTray &&
                   MOT[MMAuto2_Car].fHasTray)
                {
                    iReceiveAuto2Tray=2;
                    iAuto2ReceiveTask=200;
                }
            }
            break;
        case 70:
            if(DoAuto2Delay.Off())
            {
                if((Sen[SnAuto2TrayDetect].IsOn() &&
                    MOT[MMAuto2].fHasTray==false) ||
                   (LastSet.iRealDummy==DUMMY     &&
                    fAGV->IsATK_AMR()==false &&
                    MOT[MMAuto2].fHasTray==false))
                {
                    MOT[MMAuto2_Car].ClearTray(__FUNC__);
                    MOT[MMAuto2].SetTray(NULL_IC, __FUNC__);
                    fAuto2CanSupplyNewTray=false;
                    break;
                }
                Task=100;
            }
            break;
        case 100:
            if(MOT[MMAuto2_Car].fHasTray)
            {
                InitAuto2TrayToRearTask();
                Task=300;
            }
            else
            {
                InitLoadNewAuto2TrayToCarTask();
                Task=200;
                if((MOT[MTrayX].fHasTray &&
                    MOT[MTrayX].ReadPos()==Prod.iXTrayAuto[1]) ||
                    MOT[MMAuto2].fHasTray)
                {
                    DoAuto2Delay.SetMSAndOn(2000);
                    Task=150;
                }
            }
            break;
        case 150:
            if(DoAuto2Delay.Off())
                Task=1;
            break;
        case 200:
            if(DoLoadNewAuto2TrayToCar())
            {
                InitLoadNewAuto2TrayToCarTask();
                iReceiveAuto2Tray=1;
                Task=1;
            }
            break;
        case 300:
            if(DoAuto2TrayToRear())
            {
                Task=50;
            }
            break;
    }
}
//-------------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  golden asendic_Auto2.cpp:594
// ---------------------------------------------------------------------------
int iUnLoadNewAuto2TrayTask=1;
// ---------------------------------------------------------------------------
//  InitUnLoadNewAuto2TrayTask -- golden asendic_Auto2.cpp:595-598
// ---------------------------------------------------------------------------
void InitUnLoadNewAuto2TrayTask()
{
    iUnLoadNewAuto2TrayTask=1;
}
//-------------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  DoUnLoadNewAuto2ToStack -- golden asendic_Auto2.cpp:600-706
//  Send an empty tray from the Auto2 car back down onto the stack.
// ---------------------------------------------------------------------------
bool DoUnLoadNewAuto2ToStack()
{
    int ret;
    int &Task=iUnLoadNewAuto2TrayTask;
    static bool bAuto2HasDuplicateError=false;                                  //Steven 20120220 : Empty Tray重複Alarm Flag
    switch(Task)
    {
        case 1:
            #ifdef SOFT_SIMULTE
                Task=50;
            #else
                if(Sen[SnAuto2PreDete].IsOn() &&
                   Sen[SnAuto2_Tray_Car].IsOn() &&
                   Sen[SnAutoUpSafedetect1].IsOn() ||
                   LastSet.iRealDummy==DUMMY)
                    Task=50;
            #endif
            AutoCylinderMiddle(1, C_Auto2_Selector, C_Auto2_Up, true);          //Steven 20140409 : AutoCylinderXX Add Reset
            break;
        case 50:
            if(AutoCylinderMiddle(1, C_Auto2_Selector, C_Auto2_Up))             //升到分離高度
            {
                SW[SwACAuto2].Off();
                SW[SwACAuto2CW].Off();
                Task=100;
            }
            break;
        case 100:
            if(LastSet.iRealDummy==DUMMY ||
               Cylinder[C_Auto2LoaderZ_Select].Push())                          //Steven 20111212
            {
                Auto2Delay.SetSecAndOn(Ld_UldDelayTime.ULD_TrayBackDelay);
                Task=150;
            }
            break;
        case 150:
            if(Auto2Delay.Off())
            {
                AutoCylinderUp(1, C_Auto2_Selector, C_Auto2_Up, true);          //Steven 20140409 : AutoCylinderXX Add Reset
                Task=200;
            }
            break;
        case 200:
            if(AutoCylinderUp(1, C_Auto2_Selector, C_Auto2_Up))                 //升到最高位置
                Task=300;
            break;
        case 300:
            if(LastSet.iRealDummy==DUMMY ||
               Cylinder[C_Auto2LoaderZ_Select].Pop())                           //Steven 20111212
            {
                Task=310;
            }
            break;
        case 310:                                                               //kevin 20161129  Auto2 收tray sensor沒調好 需alarm
            if(Sen[SnAuto2PreDete].IsOff() &&
               Sen[SnAuto2_Tray_Car].IsOff() ||
               LastSet.iRealDummy==DUMMY)                                       //kevin 20161129 沒有tray在軌道
            {
                Auto2Delay.SetSecAndOn(Ld_UldDelayTime.ULD_TrayBackDelay);
                MOT[MMAuto2].ClearTray(__FUNC__);
                Task=400;
            }
            else
            {
                #ifdef SOFT_SIMULTE
                Task=400;
                #else
                ShowErrorMessage("MES1024", K_RETRY, MMAuto2_Car);
                #endif
            }
            break;
        case 400:
            if(Auto2Delay.Off())
            {
                AutoCylinderLower(1, C_Auto2_Selector, C_Auto2_Up, true);       //Steven 20140409 : AutoCylinderXX Add Reset
                Task=401;
            }
            break;
        case 401:
            if(AutoCylinderLower(1, C_Auto2_Selector, C_Auto2_Up))              //降到最低位置
            {
                Task=402;
                Auto2Delay.SetSecAndOn(3);
            }
            break;
        case 402:
            if(Sen[SnAuto2PreDete].IsOn() && Sen[SnAuto2_Tray_Car].IsOn())
            {
                ret=ShowErrorMessage("JAM1113", K_RETRY|K_SKIP, MMAuto2_Car,bAuto2HasDuplicateError,__FUNC__);          //Steven 20120926 : JAM1169 -> JAM1113
                bAuto2HasDuplicateError=true;
                if(ret ==K_SKIP)
                {
                    bAuto2HasDuplicateError=false;
                    Task=1;
                }
            }
            else if(Auto2Delay.Off())
            {
                bAuto2HasDuplicateError=false;
                Task=1;
                fAuto2CanSupplyNewTray=true;
                return true;
            }
            break;
    }
    return false;
}
//-------------------------------------------------------------------------------
// kevin 20120718 送空 tray 去放ic
//-------------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  InitAuto2ReceiveTask -- golden asendic_Auto2.cpp:710-713
// ---------------------------------------------------------------------------
void InitAuto2ReceiveTask()
{
    iAuto2ReceiveTask=1;
}
//-------------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  DoAuto2Receive -- golden asendic_Auto2.cpp:715-749
//  Feed the tray forward to the front, hand it to DoUnLoadNewAuto2ToStack.
// ---------------------------------------------------------------------------
void DoAuto2Receive()
{
    int &Task=iAuto2ReceiveTask;
    switch(Task)
    {
        case 1:
            InitAuto2TrayToFrontTask();
            Task=100;
            break;
        case 100:
            if(DoAuto2TrayToFront())
            {
                iReceiveAuto2Tray=2;
                if(MOT[MMAuto2_Car].fHasTray)
                {
                    InitUnLoadNewAuto2TrayTask();
                    Task=200;
                }
                else
                {
                    Task=1;
                    iReceiveAuto2Tray=0;
                }
            }
            break;
        case 200:
            if(DoUnLoadNewAuto2ToStack())
            {
                MOT[MMAuto2_Car].ClearTray(__FUNC__);
                iReceiveAuto2Tray=0;
                Task=1;
            }
            break;
    }
}
//-------------------------------------------------------------------------------
