// =============================================================================
//  AutoClean/AutoClean.cpp  --  W906-AutoCleanFoundation wave
//
//  Faithful translation of a curated subset of golden AutoClean.cpp (9,137
//  lines, BCB6, Big5/cp950) -- see AutoClean.h's banner for the full in-scope
//  function list + line-number map, and this wave's own report for the exact
//  golden line ranges each function below was read from.
//  Translator: AI(W906-AutoCleanFoundation) 20260721
//
//  EXPLICITLY OUT OF SCOPE (next wave): DoAutoCleanKit, DoAutoCleanPickfromCleanKit,
//  DoPlaceToShuttle, DoPickFromShuttle, DoAutoCleanPlaceToCleanKit,
//  DoShuttle1AutoClean(+_Arm1PickArm2Test variant), DoShuttle2AutoClean,
//  DoIndexAutoClean(+_Arm1PickArm2Test variant) -- the 4 named core engines and
//  their variants. Also out of scope: SetAutoCleanICCount, SetAutoCleanTrayPosition,
//  SearchiAutoCleanNum, SetAutoCleanStringGrid's OWN declaration site (still
//  translated below -- see note at its definition), ReadWriteAutoCleanCount,
//  SearchCleanNum, AutoCleanWriteData, ResetAutoClean, EnableAutoclean,
//  GetMotFunc -- none of these are in the task brief's Part A/HAL-helper list,
//  and (verified by grepping every fMain-> touch site against golden line
//  numbers) none of them are called BY an in-scope function either.
//
//  cCleanKitPickPlan.h -- confirmed (grepped, zero .cpp implementation, zero
//  call sites anywhere in golden) dead/unwired scaffolding.  NOT ported.
// =============================================================================
#include "MachineDefine.h"

#include "AutoClean.h"

#include "aArmHeader.h"             // __FUNC__ shim, RecordProcess, ShowErrorMessage, ShowMyMessage, K_RETRY/K_SKIP
#include "Motor/mymotor.h"          // MOT[], TTrayMotor, TMyTray
#include "cprod.h"                  // Prod / TestIF / TestIF_File / InArmOffSet / ArmSpeed / SHSpeed / AOA_AutoClean
#include "cmydef.h"                 // global scalar universe + IC consts
#include "cpublic.h"                // CosFunction
#include "common.h"
// AI(W906-AutoCleanFoundation) 20260721: ainarm9045.h / ainarm9045_2x6_8.h MUST
// be included BEFORE aHotPlateSubstrate.h in this TU. Both headers carry a
// `#ifndef ainarm9045H` / `#ifndef ainarm9045_2x6_8H` order-dependent guard
// (aHotPlateSubstrate.h:574/595) that only suppresses ITS OWN placeholder
// redeclarations (InArmLeftSideHasIC/NoIC default args; the bare
// `enum{e2x6OneByOne=2}`) when the real header's include-guard macro is
// *already* defined at that point -- i.e. only when the real header was
// included FIRST. Getting this backwards produces a genuine
// "default argument given twice" / "redeclaration of e2x6OneByOne" compile
// error (hit + fixed this wave).
#include "ainarm9045.h"             // bUseAxExPicker / bUseAxxGPicker / GetJStep
#include "ainarm9045_2x6_8.h"       // iCloseSiteStep_2x6 / e2x6* enum
#include "ainarm9045_2x8_8.h"       // iCloseSiteStep_2x8 (e2x8* enum already in MachineType.h -- no ordering hazard)
#include "ainarm9045_All_1Pick.h"   // GetNowInShuttleRowCol_All_1Picker
#include "aHotPlateSubstrate.h"     // InArmSuck/FLCarryKit/BLCarryKit/FTestSuck/BTestSuck, uPlateInfo (PlaceToCleanList), ainarm2 cursors
#include "atester_shims.h"          // fContact (TfContactShim)
#include "FormsFacade.h"            // fMain / fCleaning / fNote stand-ins
#include "canary_support.h"         // LastSet / ShowMyMessage / ShowErrorMessage / RecordProcess

// AI(W906-AutoCleanFoundation) 20260721: golden AutoClean.cpp:44-53 -- forward
// decls this TU needs from the 16 per-site-variant GetShuttleState_* leaves
// (each already linkable in ht9045_sm from its own ainarm9045_<variant>.cpp;
// see acarry_shims.h:201-210 for the identical "plain forward declaration,
// real body binds at link time" idiom used repeatedly elsewhere in this tree
// -- pulling in all 16 per-variant headers here would be unnecessary coupling
// for one call each).  Signatures verified via grep against each variant's
// own header.
int GetShuttleState_1x1_1(int iSht, bool bPick);
int GetShuttleState_1x2_2_14(int iSht, bool bPick);
int GetShuttleState_1x2_2(int iSht, bool bPick);
int GetShuttleState_1x3_2_14(int iSht, bool bPick);
int GetShuttleState_1x3_4(int iSht, bool bPick);
int GetShuttleState_1x4_2_14(int iSht, bool bPick);
int GetShuttleState_1x4_4(int iSht, bool bPick);
int GetShuttleState_2x2_4_14(int iSht, bool bPick);
int GetShuttleState_2x2_4(int iSht, bool bPick);
int GetShuttleState_2x3_6(int iSht, bool bPick);
int GetShuttleState_2x3_6_14(int iSht, bool bPick);
int GetShuttleState_2x4_4(int iSht, bool bPick);
int GetShuttleState_2x4_8(int iSht, bool bPick);
int GetShuttleState_2x5_8(int iSht, bool bPick);
// GetShuttleState_2x6_8 / _2x8_8 come from the ainarm9045_2x6_8.h / _2x8_8.h
// includes above (already declared there).

// AI(W906-AutoCleanFoundation) 20260721: golden cContact.cpp:83
// `const int CONTACT_DEVICE_MAP_CHECK=9;` -- a plain `extern` forward decl
// left this undefined at link time (cContact.h/.cpp are not linked into
// ht9045_sm yet, and are being touched by a concurrent, unrelated wave today
// -- SECSGEM leaf-extraction -- so deliberately not added as a dependency
// here). A file-local constant with golden's exact real value is
// self-contained and avoids any coupling to that file's in-flight edits.
static const int CONTACT_DEVICE_MAP_CHECK = 9;

// AI(W906-AutoCleanFoundation) 20260721: golden ainarm9045.cpp's real
// DoInArm_SuckerMap (rebuilds the in-arm sucker map after a site-mapping
// change) has NO link-visible home anywhere in this tree yet -- the only
// existing reference (csystem.cpp) is its OWN TU-local
// `static void W7C2_DoInArm_SuckerMap(){}` macro'd stub, invisible outside
// that TU. File-local no-op mirrors the same posture (offline: the sucker
// map does not need rebuilding without a live site-mapping change).
static void DoInArm_SuckerMap() {}

// golden AutoClean.cpp:46-47 -- file-scope globals (declared in AutoClean.h).
int iInXPos;
int iInYPos;
int iAutoCleanPlaceToShuttleTask=1;

//------------------------------------------------------------------------------
//  golden AutoClean.cpp:51 -- file-scope timer DoInArmPineRelease alone uses.
//------------------------------------------------------------------------------
TQPF_Timer tCleanInArmServoOnDelay;

// ---------------------------------------------------------------------------
//  Cross-module SYMBOLS that already have a LINK-VISIBLE definition in a
//  sibling translated TU but whose declaring header this file does not
//  include (same "plain forward declaration, real body binds at link time"
//  idiom already established by acarry_shims.h:206-210 for the identical
//  pair). AI(W906-AutoCleanFoundation) 20260721.
// ---------------------------------------------------------------------------
void SetMotorScaleSpeed(int Index, int ScaleSpeed); // golden cinitial.h:50 (body in acatchtray_shims.cpp)
void SetMotorAccelSpeed(int Index, int AccelSpeed); // golden cinitial.h:51 (body in acatchtray_shims.cpp, this wave)

// ---------------------------------------------------------------------------
//  Cross-module SYMBOLS with NO translated home ANYWHERE in this tree yet
//  (verified by grep): golden cinitial.cpp -- an entire not-yet-translated
//  module (reads Mot_Table.csv/IO_Table.csv, sets up every motor's real
//  accel/speed profile) that CleanSetSpeed's bBackup==false ("restore normal
//  speed") branch calls into. Standing up cinitial.cpp is a substantial
//  separate translation front, well beyond this wave's AutoClean-foundation
//  scope; minimal offline no-op stubs here (file-local, `static`) keep
//  CleanSetSpeed linkable without inventing cinitial.cpp's real logic.
//  MNetLog: golden's REAL body is Motor/myMN200motor.cpp (not in this tree);
//  Motor/mymotor.cpp already has its OWN static no-op precedent for the exact
//  same reason (see that file's "TODO(W5): replace with real MNetLog" note) --
//  mirrored here (file-local, so no cross-TU collision risk with whichever
//  wave eventually lands the real one).
// ---------------------------------------------------------------------------
static void SetOutArmSpeed(bool /*bShow*/) {}                  // golden cinitial.cpp:5300 -- not yet translated
static void SetSortArmSpeed(bool /*bShow*/) {}                  // golden cinitial.cpp:5425 -- not yet translated
static void UpdateMyKitSuckDelayTimeToProd() {}                  // golden cinitial.cpp:6497 -- not yet translated
static bool MNetLog(AnsiString /*Message*/) { return false; }   // golden Motor/myMN200motor.cpp -- not yet translated

// AI(W906-AutoCleanFoundation) 20260721: golden AutoAlignment/AutoAlignment.h:230
// `void CheckInArmXYScaleByAutoTeach(int &iXPos, int &iYPos, int iArea);` --
// the whole AutoAlignment/ (AOA) subsystem is untranslated anywhere in this
// tree yet (verified by grep -- zero hits). MoveInArmXYPickCleanKit's only
// call site is double-gated (MACHINE_HAS_AUTO_ALIGNMENT_CCD && bEnableAutoAlignment,
// both false by default/offline), so a faithful-shape no-op (leaves iXPos/iYPos
// untouched, matching "no AOA scale correction available") keeps that branch
// linkable without inventing AOA logic that belongs to its own future wave.
static void CheckInArmXYScaleByAutoTeach(int &/*iXPos*/, int &/*iYPos*/, int /*iArea*/) {}

//==============================================================================
//  Part A -- pure calc / config
//==============================================================================

//------------------------------------------------------------------------------
int GetAutoCleanPickCount()
{
    int iCount=4;
    if(TestIF.iTestMode==SingleSite ||                                          //Steven 20140614 : for Auto Clean Single Site
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        iCount=1;
    }
    else if(bUseAxExPicker())
    {
        iCount=2;
    }
    else if(bUseAxxGPicker() ||                                                 //Steven 20240515 : 齊平吸嘴
            iCloseSiteModeFor1x4==e1x4CloseAbAc)                                //Steven 20241111 : for 1x4 close 2 site)
    {
        iCount=2;
    }
    else
    {
        if(bCleanKitPitchLess4000==true)                                        //Steven 20250923 : HT9045CN 治具最小是20x3mm
            iCount=1;
        else
            iCount=4;
    }
    return iCount;
}
//------------------------------------------------------------------------------
int GetAutoCleanPickStep(int iCol)
{
    int iSuckCol=-1;                                                            //Steven 20240918 : fixed for auto clean
    if(TestIF.iTestMode==SingleSite ||                                          //Steven 20140614 : for Auto Clean Single Site
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        if(iCol==0)
        {
            if(Prod.bSingleUseOtherSuck || Prod.bSingleInArmUseOtherSuck)       //JerryYang 20260414 : fix 1x1 auto clean
                iSuckCol=1;
            else
                iSuckCol=0;
        }
    }
    else if(bUseAxExPicker() ||
            iInArmType==e9045_1x2_4_Hot)                                        //Jimmychiu 20260109 Add 1x2_4 auto clean error
    {
        if(iCol==0)
            iSuckCol=0;
        else if(iCol==1)
            iSuckCol=2;
    }
    else if(bUseAxxGPicker() ||                                                 //Steven 20240515 : 齊平吸嘴
            iCloseSiteModeFor1x4==e1x4CloseAbAc)                                //Steven 20241111 : for 1x4 close 2 site)
    {
        if(iCol==0)
            iSuckCol=0;
        else if(iCol==1)
            iSuckCol=3;
    }
    else
    {
        iSuckCol=iCol;
    }
    return iSuckCol;
}
//------------------------------------------------------------------------------
int CalculateAutoCleanXPitch()
{
    int iAutoCleanXPitch=1;

    if(TestIF.iTestMode==SingleSite ||
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        iAutoCleanXPitch=1;
    }
    else if(bUseAxExPicker() ||
            i1x2_4UseACEGPicker==1)                                             //Jimmychiu 20260109 Add 1x2_4 auto clean error
    {
        iAutoCleanXPitch=(int)ceil(TestIF_File.iAutoClean_XDivision/2.0);
    }
    else if(bUseAxxGPicker() ||                                                 //Steven 20240515 : 齊平吸嘴
            iCloseSiteModeFor1x4==e1x4CloseAbAc)                                //Steven 20241111 : for 1x4 close 2 site)
    {
        iAutoCleanXPitch=(int)ceil(TestIF_File.iAutoClean_XDivision/2.0);
    }
    else if(TestIF.iTestMode==QualSite1X4 ||
            TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        iAutoCleanXPitch=(int)ceil(TestIF_File.iAutoClean_XDivision/4.0);
    }
    else
    {
        iAutoCleanXPitch=(int)ceil(TestIF_File.iAutoClean_XDivision/4.0);
    }
    return iAutoCleanXPitch;
}
//------------------------------------------------------------------------------
int GetXPitchOfCleanKit_HP()
{
    double dPitch=6000;

    bCleanKitPitchOver12000=false;

    if(TestIF.iTestMode==SingleSite ||                                          //wei 20220823 Single Site使用C治具判斷
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        iAutoCleanUseXPitch=1;
        dPitch=iXpitchMaxX3;
    }
    else if(bUseAxExPicker())
    {
        if(TestIF.iAutoClean_XDivision==2 &&
            TestIF_File.iTestMode==QualSite2X2 &&
            TestIF.dAutoClean_XPitch>=iXpitchMinX2 &&
            TestIF.dAutoClean_XPitch<=iXpitchMaxX2)                                 //KevinCheng 20260421 : 2x2 cleankit only 2 cols
        {
            iAutoCleanUseXPitch=1;
        }
        else
        {
            iAutoCleanUseXPitch=2;
        }

        dPitch=double(TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch)/2.0*3.0;    //治具1顆治具3欄，比對4欄HotPlateForm.XPitch
        if(dPitch>(iXpitchMaxX3+300))                                           //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            iAutoCleanUseXPitch=1;
            dPitch=double(TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch)/2.0*3.0;
            bCleanKitPitchOver12000=true;
        }
    }
    else if(bUseAxxGPicker() ||                                                 //Steven 20240515 : 齊平吸嘴
            iCloseSiteModeFor1x4==e1x4CloseAbAc)                                //Steven 20241111 : for 1x4 close 2 site)
    {
        iAutoCleanUseXPitch=2;
        dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch;
        if(dPitch>(iXpitchMaxX3+300))                                           //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            iAutoCleanUseXPitch=1;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch;
            bCleanKitPitchOver12000=true;
        }
    }
    else if(TestIF.iAutoClean_DeveicePices<=4)                                  //jou 2013-10-02 修正Auto Clean Qual site 1x4 pitch <= 20mm , 會超出安全範圍.
    {
        iAutoCleanUseXPitch=1;                                                  //kevin 20120229
        dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;                  //治具1顆治具2欄，比對2欄HotPlateForm.XPitch
    }
    else
    {
        if(TestIF.iAutoClean_XDivision>=8 &&
           TestIF.dAutoClean_XPitch<=2000)                                      //Steven 20251023 : fixed for 8x4 x-pitch=20mm auto clean
        {
            iAutoCleanUseXPitch=2;
        }
        else if(TestIF.dAutoClean_XPitch<2000)
        {
            iAutoCleanUseXPitch=2;                                              //Steven 20250930 : fixed for small pitch auto clean
        }
        else
        {
            iAutoCleanUseXPitch=1;                                              //kevin 20120229
        }
        dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;                  //治具1顆治具2欄，比對2欄HotPlateForm.XPitch
        if(dPitch>(iXpitchMaxX3+300))                                           //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            iAutoCleanUseXPitch=1;
            dPitch=iXpitchMaxX3;                                                //JerryYang 20200622 : x-pitch超過12000會撞機
            bCleanKitPitchOver12000=true;
        }
    }
    return int(dPitch);
}
//------------------------------------------------------------------------------
int GetXPitchOfCleanKit_Kit()
{
    double dPitch=6000;
    bCleanKitPitchOver12000=false;
    bCleanKitPitchLess4000=false;                                               //jou 2015-03-24 tray x pitch太小導致無法治具

    if(TestIF.iTestMode==SingleSite ||
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        iAutoCleanUseXPitch=1;
        dPitch=iXpitchMaxX3;                                                    //Steven 20140614 : for Auto Clean Single Site   //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
    }
    else if(bUseAxExPicker() ||
            i1x2_4UseACEGPicker==1)                                             //Jimmychiu 20260109 Add 1x2_4 auto clean error
    {
        if(TestIF.iAutoClean_XDivision==2)
        {
            iAutoCleanUseXPitch=1;
        }
        else if(TestIF.iAutoClean_XDivision>2 &&                                //Steven 20240902 : 修正1x2 auto clean
                TestIF.iAutoClean_DeveicePices==2)
        {
            iAutoCleanUseXPitch=1;
        }
        else if(TestIF.bAutoClean_UseTray)                                      //Steven 20240902 : 修正1x2 auto clean
        {
            iAutoCleanUseXPitch=CalculateAutoCleanXPitch();
        }
        else
        {
            iAutoCleanUseXPitch=2;                                              //jou 2014-06-13 4 -> 2 修正 Dual site Clean kit 2x8 start:33 pitch:22 超過安全範圍
        }

        dPitch=double(TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch)/2.0*3.0;    //治具1顆治具3欄，比對4欄HotPlateForm.XPitch
        if(dPitch>(iXpitchMaxX3+300))                                           //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            iAutoCleanUseXPitch=1;
            dPitch=double(TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch)/2.0*3.0;
            bCleanKitPitchOver12000=true;
        }
        else if(dPitch<iXpitchMax)
        {
            if(TestIF.iAutoClean_XDivision_Kit==2)                              //JerryYang 20210224 : 修正dual site clean kit 1x2 x-pitch 25mm點位小數異常
            {
                iAutoCleanUseXPitch=2;
                dPitch=double(TestIF.dAutoClean_XPitch)*3.0;
            }
            else
            {
                iAutoCleanUseXPitch=4;                                          //Alick 20161107 修正使用CLEAN TRAY,TRAY X-PITCH<13.33會造成點位計算異常,iAutoCleanUseXPitch=4,規劃CleanIC數量4&8
                dPitch=double(TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch)/2.0*3.0;
            }
            bCleanKitPitchLess4000=true;
        }
    }
    else if(bUseAxxGPicker() ||                                                 //Steven 20240515 : 齊平吸嘴
            iCloseSiteModeFor1x4==e1x4CloseAbAc)                                //Steven 20241111 : for 1x4 close 2 site
    {
        if(TestIF.iAutoClean_XDivision==2)
        {
            iAutoCleanUseXPitch=1;
        }
        else if(TestIF.iAutoClean_XDivision==8)                                 //Steven 20250318 : fix for 1x2 / 2x2 auto clean with 8 col, x-pitch 25mm
        {
            iAutoCleanUseXPitch=4;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch;
            if(dPitch>iXpitchMaxX3)                                             //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
            {
                iAutoCleanUseXPitch=2;
            }
        }
        else if(TestIF.iAutoClean_XDivision>2 &&                                //Steven 20240902 : 修正1x2 auto clean
                TestIF.iAutoClean_DeveicePices==2)
        {
            iAutoCleanUseXPitch=1;
        }
        else if(TestIF.bAutoClean_UseTray)
        {
            iAutoCleanUseXPitch=CalculateAutoCleanXPitch();
        }
        else
        {
            iAutoCleanUseXPitch=2;                                              //jou 2014-06-13 4 -> 2 修正 Dual site Clean kit 2x8 start:33 pitch:22 超過安全範圍
        }

        dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch;                    //治具1顆治具4欄，比對2欄HotPlateForm.XPitch  //Steven 20180829 : #P180827-ATK-H9-01 Pickup position was wrong during Auto Clean in 2x2 mode.
        if(dPitch>iXpitchMaxX3)                                                 //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            iAutoCleanUseXPitch=1;
            dPitch=iXpitchMaxX3;                                                //Steven 20241111 : for 1x4 run 3 col tray auto clean
            bCleanKitPitchOver12000=true;
        }
        else if(dPitch<iXpitchMax)
        {
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch;
            bCleanKitPitchLess4000=true;                                        //jou 2015-03-24 tray x pitch太小導致無法治具
        }
    }
    else
    {
        if(InArmSuck.iPickCol==4 &&
           TestIF.iAutoClean_XDivision==8 &&                                    //JerryYang 20241002 : X 數量8 齊平治具
           TestIF.iAutoClean_DeveicePices%8==0 &&                               //Sam 20250207 : 要齊平治具 Clean Pad 數量也要對
           TestIF.dAutoClean_XPitch*2<iXpitchMin)                               //Steven 20250923 : HT9045CN 治具最小是20x3mm
        {
            iAutoCleanUseXPitch=4;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;              //治具1顆治具2欄，比對2欄HotPlateForm.XPitch
            bCleanKitPitchLess4000=true;

            if(dPitch>iXpitchMaxX3)
            {
                int dSumXPitch=0.0;
                iAutoCleanUseXPitch=2;
                for(int igap=0; igap<5; igap++)
                {
                    dSumXPitch=TestIF.dAutoClean_XPitch*igap;
                    if(dSumXPitch>(double)iXpitchMin)
                    {
                        iAutoCleanUseXPitch=igap;
                        break;
                    }
                }
                bCleanKitPitchLess4000=true;
                dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;          //JimmyChiu 20240415 : modified for auto clean
            }
        }
        else if(InArmSuck.iPickCol==4 &&
                TestIF.iAutoClean_XDivision==8 &&                               //JerryYang 20241002 : X 數量8 齊平治具
                TestIF.iAutoClean_DeveicePices%8==0 &&                          //Sam 20250207 : 要齊平治具 Clean Pad 數量也要對
                TestIF.dAutoClean_XPitch*2<=iXpitchMax)
        {
            iAutoCleanUseXPitch=2;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;              //治具1顆治具2欄，比對2欄HotPlateForm.XPitch

            if(dPitch<iXpitchMax)
            {
                dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*2*3;
                bCleanKitPitchLess4000=true;
            }
        }
        else if(TestIF.bAutoClean_UseTray &&
                TestIF.dAutoClean_XPitch>=iXpitchMin)
        {
            iAutoCleanUseXPitch=1;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;              //治具1顆治具2欄，比對2欄HotPlateForm.XPitch

            if(dPitch>iXpitchMaxX3)                                             //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
            {
                iAutoCleanUseXPitch=1;
                dPitch=iXpitchMaxX3;                                            //治具1顆治具2欄，比對2欄HotPlateForm.XPitch
                bCleanKitPitchOver12000=true;
            }
        }
        else if(TestIF.dAutoClean_XPitch<iXpitchMin)                            //Steven 20241111 : 1333 -> iXpitchMin
        {
            int dSumXPitch=0.0;
            iAutoCleanUseXPitch=2;
            for(int igap=0; igap<5; igap++)
            {
                dSumXPitch=TestIF.dAutoClean_XPitch*igap;
                if(dSumXPitch>(double)iXpitchMin)
                {
                    iAutoCleanUseXPitch=igap;
                    break;
                }
            }
            bCleanKitPitchLess4000=true;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;              //JimmyChiu 20240415 : modified for auto clean
        }
        else if(TestIF.dAutoClean_XPitch>iXpitchMax)                            //Steven 20240305 : 修正Auto Clean Pitch判斷
        {
            iAutoCleanUseXPitch=2;
            bCleanKitPitchOver12000=true;
            dPitch=TestIF.dAutoClean_XPitch*2*3;
        }
        else
        {
            if(InArmSuck.iPickCol==4 &&                                         //JerryYang 20241002 : X 數量8 齊平治具
               TestIF.iAutoClean_XDivision==8 &&
               TestIF.iAutoClean_DeveicePices%8==0 &&                           //Sam 20250207 : 要齊平治具 Clean Pad 數量也要對
               TestIF.dAutoClean_XPitch*2*3<iXpitchMaxX3)                       //Sam 20251020 : 齊平治具要在最大 PitcX 內
            {
                iAutoCleanUseXPitch=2;
            }
            else if(InArmSuck.iPickCol==4 &&                                    //JerryYang 20241002 : X 數量8 齊平治具
                    TestIF.iAutoClean_XDivision==12 &&
                    TestIF.iAutoClean_DeveicePices%12==0 &&                     //Sam 20250207 : 要齊平治具 Clean Pad 數量也要對
                    TestIF.dAutoClean_XPitch*2*3<iXpitchMaxX3)                  //Sam 20251020 : 齊平治具要在最大 PitcX 內
            {
                iAutoCleanUseXPitch=2;
            }
            else
            {
                iAutoCleanUseXPitch=1;
            }

            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;              //治具1顆治具2欄，比對2欄HotPlateForm.XPitch

            if(dPitch<iXpitchMax)
            {
                dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*2*3;
                bCleanKitPitchLess4000=true;
            }
        }
    }
    return int(dPitch);
}
//------------------------------------------------------------------------------
int GetXPitchOfCleanKit()
{
    if(IniConfig.bE43AutoCleanUseHotplate ||
       TestIF_File.iTestMode==_6Site2X3)                                        //Steven 20171220 (Wei) : Fixed for 2x3 Auto Clean Hang Up
        return GetXPitchOfCleanKit_HP();
    else
        return GetXPitchOfCleanKit_Kit();
}
//------------------------------------------------------------------------------
int GetYPitchOfCleanKit()
{
    int iYVariable=TestIF.iARM_Y_PITCH;

    if(IniConfig.bE43AutoCleanUseHotplate && USE_IN_Y_IS_AUTO_PITCH==true)            //JerryYang 20251218 : IN/OUT ARM支援不同模式  //Steven 20160629
    {
        if(TestIF_File.dAutoClean_YPitch<IN_OUT_ARM_Y_PITCH_MIN)                //Steven 20180824 : 修正Y-Pitch小於15mm
        {
            iYVariable=TestIF_File.iARM_Y_PITCH;
        }
        else if(TestIF_File.dAutoClean_YPitch>IN_OUT_ARM_Y_PITCH_MAX)           //Steven 20240801 : fixed for auto clean.
        {
            iYVariable=TestIF_File.iARM_Y_PITCH;
        }
        else
        {
            iYVariable=TestIF_File.dAutoClean_YPitch;
        }
    }

    return iYVariable;
}
//------------------------------------------------------------------------------
bool RunAutoCleanByArmPickArm2Test()                                            //Jimmychiu 20230710 : Auto Clean 用 Arm1 下料 arm2 測試
{
    return (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
            TestIF_File.bArm1PickPlaceArm2Test==true &&
            TestIF_File.bArm1PickPlaceArm2Test_RunAutoClean==true);
}
//------------------------------------------------------------------------------
bool Special_2X6_Tray_XItem7()                                                  //Sam 20250712 : 新增特殊流程 2X6 AutoClean Tray XItem=7
{                                                                               //使用 Auto Clean Tray 模式且Tray的 XItem 為 7，最右邊 8 欄的物理位置沒有 IC，只當作補位用
    if(iInArmType==e9045_2x6_8                  &&
       TestIF_File.bAutoClean_UseTray           &&
       TrayForm.Loader.XDivision==7             &&
       TestIF_File.iAutoClean_XDivision_Tray==8 &&
       TestIF_File.iAutoClean_YDivision_Tray==2 &&
       LastSet.bUseTestSocket[0][0][5]==true    &&
       LastSet.bUseTestSocket[0][1][5]==true)
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void GetInarmSuckRow(int iShtRowKit,int &isuckRow,int &ikitStep)
{
    if(iShtRowKit==1 || iShtRowKit==2)                                          //JerryYang 20160411 當X-Pitch > 40mm,要拆成4欄用shuttle
    {
        ikitStep=0;
        isuckRow=iShtRowKit-1;
    }
    else
    {
        ikitStep=4;
        isuckRow=iShtRowKit-3;
    }

    if(isuckRow<0)
        isuckRow=0;
}

//==============================================================================
//  Part A -- HAL-only state-machine helpers
//==============================================================================

//------------------------------------------------------------------------------
void InitialAutoCleanAllTask()                                                  //Sam 20230504 : 整理 InitialAutoCleanTask
{
    InitialAutoCleanTask();
    InitialShuttleAutoCleanTask();
    InitialIndexAutoCleanTask();
    if(iOneCycle==1 && bIsAutoOneCycle==false && bManualOneCycle)               //Sam 20230309 : 避免觸發 OneCycle 後，OneCycle 完成的時又觸發 AutoClean 動作，AutoClean 完成才會執行 OneCycle Finish
    {
        bBackupOneCycle_ByAutoClean=true;
        bManualOneCycle=false;
    }
    bIsAutoOneCycle=true;
    fMain->BtnOneCycleClick(fMain);
}
//------------------------------------------------------------------------------
void InitialAutoCleanTask()
{
    iDoAutoCleanTask=1;
    bPlaceToShuttleByAutoClean=false;                                           //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
    bPickFromShuttleByAutoClean=false;                                          //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
    bLockPlaceToShuttleByAutoClean=false;                                       //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
    bLockPickFromShuttleByAutoClean=false;                                      //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion

    if(CUSTOMER_CODE==CC_SPIL_TAICHUNG_LOGIC &&
       IniConfig.bFTPJamCodeUpload &&
       IniConfig.bEnableFTP)                                                    //Steven 20140917 : 台中SPIL要求Auto Clean要傳送Alarm Code
    {
        // AI(W906-AutoCleanFoundation) 20260721: golden body here is
        // `fNote->aJamCodeFilePath=fFTPClient->SaveJamCodeFile(...); fFTPClient->
        // UploadFileToServer2(...);` -- the KYECFTP translation wave deliberately
        // "demoted" fFTPClient away from a singleton (see KYECFTP/FTPClient_Transfer.h
        // -- UploadFileToServer2 is now a free function, no `fFTPClient->` surface
        // exists anymore), and SaveJamCodeFile was never translated at all
        // (verified by grep -- zero hits). Re-introducing the singleton pattern
        // that wave deliberately removed is out of scope here; gated (matches
        // this tree's established fNote/fFTPClient-no-home precedent -- see
        // FormsFacade.h's TfNote banner). CUSTOMER_CODE==CC_SPIL_TAICHUNG_LOGIC
        // is a narrow single-customer condition, default unreachable elsewhere.
        //   fNote->aJamCodeFilePath=fFTPClient->SaveJamCodeFile(IniConfig.SocketHandlerID, Now(), "MES1607", "Autoclean Start");
        //   fFTPClient->UploadFileToServer2(IniConfig.FtpUplaodPath, fNote->aJamCodeFilePath);    //Steven 20140513 : 移動到Form Show之前
    }
}
//------------------------------------------------------------------------------
void InitialShuttleAutoCleanTask()
{
    iDoShuttle1AutoCleanTask=1;
    iDoShuttle2AutoCleanTask=1;
    iDoShuttleAutoCleanTask=1;
}
//------------------------------------------------------------------------------
void InitialIndexAutoCleanTask()
{
    iDoIndexAutoCleanTask=1;
}
//------------------------------------------------------------------------------
void InitPickFromShuttleTask()
{
    iAutoCleanPickFromShuttleTask=1;
}
//------------------------------------------------------------------------------
void InitPlaceToShuttleTask()
{
    iAutoCleanPlaceToShuttleTask=1;
}
//------------------------------------------------------------------------------
void CleanSetSpeed(bool bBackup)
{
    UpdateMyKitSuckDelayTimeToProd();                                           //Steven 20250319 : 治具OnDelayTime轉換獨立function
    if(bBackup==true)                                                           //Store and Set System Original Speed
    {
        ArmSpeed[InArm].iBodySP     =TestIF.iAutoClean_MotorSpeed[0];           //In Arm
        ArmSpeed[InArm].iACDCBodySP =TestIF.iAutoClean_MotorSpeed[0];           //In Arm
        ArmSpeed[InArm].iVariSP     =TestIF.iAutoClean_MotorSpeed[0];           //In arm pitch
        ArmSpeed[InArm].iACDCVariSP =TestIF.iAutoClean_MotorSpeed[0];           //In arm pitch
        SHSpeed.iSH1Sp              =TestIF.iAutoClean_MotorSpeed[1];           //Shuttle 1
        SHSpeed.iSH1ACDCSp          =TestIF.iAutoClean_MotorSpeed[1];           //Shuttle 1
        SHSpeed.iSH2Sp              =TestIF.iAutoClean_MotorSpeed[1];           //Shuttle 2
        SHSpeed.iSH2ACDCSp          =TestIF.iAutoClean_MotorSpeed[1];           //Shuttle 2
        ArmSpeed[InArm].iACDCZSP    =TestIF.iAutoClean_MotorSpeed[3];           //In Arm Z
        ArmSpeed[InArm].iZSP        =TestIF.iAutoClean_MotorSpeed[3];           //In Arm Z
    }
    else
    {
        ArmSpeed[InArm].iBodySP     =ArmSpeed_File[InArm].iBodySP;              //In Arm
        ArmSpeed[InArm].iACDCBodySP =ArmSpeed_File[InArm].iACDCBodySP;          //In Arm
        ArmSpeed[InArm].iVariSP     =ArmSpeed_File[InArm].iVariSP;              //In arm pitch
        ArmSpeed[InArm].iACDCVariSP =ArmSpeed_File[InArm].iACDCVariSP;          //In arm pitch
        SHSpeed.iSH1Sp              =SHSpeed_File.iSH1Sp;                       //Shuttle 1
        SHSpeed.iSH1ACDCSp          =SHSpeed_File.iSH1ACDCSp;                   //Shuttle 1
        SHSpeed.iSH2Sp              =SHSpeed_File.iSH2Sp;                       //Shuttle 2
        SHSpeed.iSH2ACDCSp          =SHSpeed_File.iSH2ACDCSp;                   //Shuttle 2
        ArmSpeed[InArm].iACDCZSP    =ArmSpeed_File[InArm].iACDCZSP;             //In Arm Z
        ArmSpeed[InArm].iZSP        =ArmSpeed_File[InArm].iZSP;                 //In Arm Z
        SetOutArmSpeed(false);                                                  //Steven 20130620 : 重置Out Arm速度
        if(USE_OUT_SORT_ARM!=eartUninstall)                                     //RogerYang 20250515 add for 9046AU
            SetSortArmSpeed(false);
    }

    SetMotorAccelSpeed(MInArmX      ,ArmSpeed[InArm].iACDCBodySP);
    SetMotorAccelSpeed(MInArmY      ,ArmSpeed[InArm].iACDCBodySP);
    SetMotorScaleSpeed(MInArmX      ,ArmSpeed[InArm].iBodySP);
    SetMotorScaleSpeed(MInArmY      ,ArmSpeed[InArm].iBodySP);

    SetMotorAccelSpeed(MInArmPitch  ,ArmSpeed[InArm].iACDCVariSP);
    SetMotorScaleSpeed(MInArmPitch  ,ArmSpeed[InArm].iVariSP);

    if(USE_IN_Y_IS_AUTO_PITCH==true)                                                  //JerryYang 20251218 : IN/OUT ARM支援不同模式  //Steven 20160630 : 限制Y-Pitch最低速度為80
    {
        if(ArmSpeed[InArm].iACDCVariSP<80)
            SetMotorAccelSpeed(MInArmPitchY, 80);
        else
            SetMotorAccelSpeed(MInArmPitchY, ArmSpeed[InArm].iACDCVariSP);

        if(ArmSpeed[InArm].iVariSP<80)
            SetMotorScaleSpeed(MInArmPitchY, 80);
        else
            SetMotorScaleSpeed(MInArmPitchY, ArmSpeed[InArm].iVariSP);
        SetMotorAccelSpeed(MInArmPitchX2, ArmSpeed[InArm].iACDCVariSP);
        SetMotorScaleSpeed(MInArmPitchX2, ArmSpeed[InArm].iVariSP);
    }

    int iMot=MInArmZA;
    if(InOutArmPickerUseMotor==eptUseMotCyn)
    {
        SetMotorAccelSpeed(MInArmZA, ArmSpeed[InArm].iACDCZSP);
        SetMotorScaleSpeed(MInArmZA, ArmSpeed[InArm].iZSP);
    }
    else
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                iMot=InArmSuck.Suck[i][j].iMotNo;
                SetMotorAccelSpeed(iMot, ArmSpeed[InArm].iACDCZSP);
                SetMotorScaleSpeed(iMot, ArmSpeed[InArm].iZSP);
            }
        }
    }

    SetMotorAccelSpeed(MInShuttle1      ,SHSpeed.iSH1Sp);
    SetMotorScaleSpeed(MInShuttle1      ,SHSpeed.iSH1ACDCSp);
    SetMotorAccelSpeed(MInShuttle2      ,SHSpeed.iSH2Sp);
    SetMotorScaleSpeed(MInShuttle2      ,SHSpeed.iSH2ACDCSp);

    fShowMessage->ShowSpeed(IniConfig.bG05ShowSpeedMessage);
}
//------------------------------------------------------------------------------
void InOutArmSuckActiveSet()
{
    ZeroMemory(bInArmSuckActive, sizeof(bInArmSuckActive));
}
//------------------------------------------------------------------------------
// kevin 20120217 Z軸移到 SHUTTLE 2 位置
//------------------------------------------------------------------------------
bool MoveInArmZToShuttlePlace(eWhichShuttle iSht, int iRowSel)                  //20140923 wei : For Shuttle Auto Clean
{
    int iShuttlePlace[MAX_ARM_Row][MAX_ARM_Col];
    bool flag[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
    bool bNeedDown;

    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            if(iSht==euShuttle1)
            {
                if(IniConfig.bEnableAutoCleanFunction &&
                   TestIF_File.iAutoClean_Function &&
                   bRunAutoClean &&
                   IniConfig.bE48_ShuttleUse4Offset_Autoclean &&                //20140923 wei : For Shuttle Auto Clean
                   TestIF.bEnableAutoAlignment==false)                          //KenHsieh 20220923 : add AOA功能開啟不套影
                {
                    iShuttlePlace[i][j]=Prod.ZInArm_Shuttle1_Place[i][j] -
                                        InArmOffSet[InOfsInSh1]->GetPlace() +
                                        InArmOffSet[InOfsInSh1_AutoClean]->GetPlace()+
                                        InArmOffSet[InOfsInSh1_AutoClean+iRowSel-1]->SingleOffSet->dPlaceOffSet[i][j];
                }
                else if(CosFunction.bAutoCleanOffsetUseSingleSetting &&         //Sam 20220720 : AutoClean Offset 值使用 Clean 設定 Offset 來套用
                        TestIF.bEnableAutoAlignment==false)                     //KenHsieh 20220923 : add AOA功能開啟不套影
                {
                    iShuttlePlace[i][j]=Prod.ZInArm_Shuttle1_Place[i][j]+TestIF.iAutoClean_Shuttle1PlaceOffset-InArmOffSet[InOfsInSh1]->GetPlace();
                }
                else
                {
                    iShuttlePlace[i][j]=Prod.ZInArm_Shuttle1_Place[i][j]+TestIF.iAutoClean_Shuttle1PlaceOffset;
                }
            }
            else
            {
                if(IniConfig.bEnableAutoCleanFunction &&
                   TestIF_File.iAutoClean_Function &&
                   bRunAutoClean &&
                   IniConfig.bE48_ShuttleUse4Offset_Autoclean &&                //20140923 wei : For Shuttle Auto Clean
                   TestIF.bEnableAutoAlignment==false)                          //KenHsieh 20220923 : add AOA功能開啟不套影
                {
                    iShuttlePlace[i][j]=Prod.ZInArm_Shuttle2_Place[i][j] -
                                        InArmOffSet[InOfsInSh2]->GetPlace() +
                                        InArmOffSet[InOfsInSh2_AutoClean]->GetPlace()+
                                        InArmOffSet[InOfsInSh2_AutoClean+iRowSel-1]->SingleOffSet->dPlaceOffSet[i][j];
                }
                else if(CosFunction.bAutoCleanOffsetUseSingleSetting &&         //Sam 20220720 : AutoClean Offset 值使用 Clean 設定 Offset 來套用
                        TestIF.bEnableAutoAlignment==false)                     //KenHsieh 20220923 : add AOA功能開啟不套影
                {
                    if(IniConfig.bE34InOutArmPitchZOffsetSameOne)
                        iShuttlePlace[i][j]=Prod.ZInArm_Shuttle2_Place[i][j]+TestIF.iAutoClean_Shuttle2PlaceOffset-InArmOffSet[InOfsInSh1]->GetPlace();
                    else
                        iShuttlePlace[i][j]=Prod.ZInArm_Shuttle2_Place[i][j]+TestIF.iAutoClean_Shuttle2PlaceOffset-InArmOffSet[InOfsInSh2]->GetPlace();
                }
                else
                {
                    iShuttlePlace[i][j]=Prod.ZInArm_Shuttle2_Place[i][j]+TestIF.iAutoClean_Shuttle2PlaceOffset; //JerryYang 20160301 修正Offset,1-->2
                }
            }
        }
    }

    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            bNeedDown=(InArmSuck.Item[i][j]!=NULL_IC &&
                       InArmSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                       InArmSuck.Suck[i][j].GetNeedDestroyStatus());

            flag[i][j]=bNeedDown;
        }
    }

    bool bRet=InArmZMoveDown(flag, iShuttlePlace, false);                       //Steven 20251020 : modify for auto clean
    return bRet;
}
//------------------------------------------------------------------------------
//0 Full
//1 Row1 - Kit1
//2 Row2 - Kit1
//3 Row1 - Kit2
//4 Row2 - Kit2
int GetShuttleState(eWhichShuttle iSht, bool bPick)
{
    if(iSht==euShuttle1)
        ptrInSHT=&FLCarryKit;
    else
        ptrInSHT=&BLCarryKit;

    if(TestIF.iTestMode==SingleSite ||
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        return GetShuttleState_1x1_1((int)iSht, bPick);
    }
    else if(iInArmType==e9045_1x2_2_14)                                         //Steven 20221027 : 修正Auto Clean
    {
        return GetShuttleState_1x2_2_14((int)iSht, bPick);
    }
    else if(TestIF.iTestMode==DualSite || TestIF.iTestMode==QualSite2X2N)
    {
        return GetShuttleState_1x2_2((int)iSht, bPick);
    }
    else if(iInArmType==e9045_1x3_2_14)
    {
        return GetShuttleState_1x3_2_14((int)iSht, bPick);
    }
    else if(iInArmType==e9045_1x3_4)
    {
        return GetShuttleState_1x3_4((int)iSht, bPick);
    }
    else if(iInArmType==e9045_1x4_2_14)
    {
        return GetShuttleState_1x4_2_14((int)iSht, bPick);
    }
    else if(TestIF.iTestMode==QualSite1X4 ||
            TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        return GetShuttleState_1x4_4((int)iSht, bPick);
    }
    else if(iInArmType==e9045_2x2_4_14)                                         //Steven 20221027 : 修正Auto Clean
    {
        return GetShuttleState_2x2_4_14((int)iSht, bPick);
    }
    else if(TestIF_File.iTestMode==QualSite2X2)
    {
        return GetShuttleState_2x2_4((int)iSht, bPick);
    }
    else if(iInArmType==e9045_2x3_6)
    {
        return GetShuttleState_2x3_6((int)iSht, bPick);
    }
    else if(iInArmType==e9045_2x3_6_14)
    {
        return GetShuttleState_2x3_6_14((int)iSht, bPick);
    }
    else if(iInArmType==e9045_2x4_4_14)
    {
        return GetShuttleState_2x4_4((int)iSht, bPick);
    }
    else if(TestIF_File.iTestMode==_8Site2X4 ||
            TestIF_File.iTestMode==_16Site4X4)
    {
        return GetShuttleState_2x4_8((int)iSht, bPick);
    }
    else if(TestIF_File.iTestMode==_10Site2X5)
    {
        return GetShuttleState_2x5_8((int)iSht, bPick);
    }
    else if(TestIF_File.iTestMode==_12Site2X6)
    {
        return GetShuttleState_2x6_8((int)iSht, bPick);
    }
    else if(TestIF_File.iTestMode==_16Site2X8)
    {
        return GetShuttleState_2x8_8((int)iSht, bPick);
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {
        return GetShuttleState_2x8_8((int)iSht, bPick);
    }
    else
    {
        ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "GetShuttleState");
    }
    return 0;
}
//------------------------------------------------------------------------------
bool bCleanZMotToPlace[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
bool bCleanZSuckToPlace[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
bool MoveInOutArmZToKitPickPlace(int Pick, bool bReset, int iSht, int iShuttleRow)  //ChungHung 20150303 add iSht for Hotplate AutoClean
{
    static bool bflag[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};

    bool flag=false;
    int iZPos[MAX_ARM_Row][MAX_ARM_Col], iMot;

    if(bReset)
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                bflag[i][j]=false;
            }
        }
    }

    for(int j=0; j<InArmSuck.iMotCol; j++)
    {
        if(Pick==bAutoPick)
        {
            iZPos[0][j]=Prod.ZInArm_AutoClean_Pick[0][j]+iArmPickTrayPos;
            iZPos[1][j]=Prod.ZInArm_AutoClean_Pick[1][j]+iArmPickTrayPos;
        }
        else
        {
            iZPos[0][j]=Prod.ZInArm_AutoClean_Place[0][j]+iArmPlaceTrayPos;
            iZPos[1][j]=Prod.ZInArm_AutoClean_Place[1][j]+iArmPlaceTrayPos;
        }
    }

    if(bUse8Picker==false && IniConfig.bE43AutoCleanUseHotplate==false)            //Steven 20260504 : bUse8Picker==false only B-row(row1) Z descent
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            if(bInArmSuckActive[0][j])
            {
                // AI(W906-AutoCleanFoundation) 20260721: golden's real
                // MyDBIProcess(AnsiString asTable, AnsiString S1, AnsiString S2="")
                // is 3-arg (cMyDB.h:20); the target's MyDBIProcess is 2-arg
                // (S1,S2) -- every existing call site in this tree already
                // drops golden's 3rd arg the same way (grepped: acarry.cpp
                // etc. all pass exactly 2 args). Folding "Protection" into the
                // message text preserves the information instead of silently
                // dropping it.
                MyDBIProcess("AutoClean",
                    AnsiString().sprintf("MoveInOutArmZToKitPickPlace: bInArmSuckActive[0][%d]=true force false [Protection]", j));
                bInArmSuckActive[0][j]=false;
            }
        }
    }

    if(Pick==bAutoPick)
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                iMot=InArmSuck.Suck[i][j].iMotNo;
                if(bInArmSuckActive[i][j])
                {
                    if(bflag[i][j]==false)
                        bflag[i][j]=MOT[iMot].MotorMove(iZPos[i][j]);
                }
                else
                {
                    bflag[i][j]=true;
                }
            }
        }
    }
    else
    {
        uHPSuckTeam* ppInfo=PlaceToCleanList->ExtractFirstTeam();
        if(PlaceToCleanList->GetHPFirstTeamMotUse(bCleanZMotToPlace)==false)        //RogerYang 20250624 Add HotPlate ErrMessage
        {
            ShowMyMessage(AnsiString().sprintf("%s %s Task=%d", __FUNC__, "No GetHPFirstTeamMotUse", 0));
            return false;
        }

        PlaceToCleanList->GetHPFirstTeamMotUse(bCleanZSuckToPlace);                 //JerryYang 202050813 : fix合併錯誤治具下降
        if(ppInfo!=NULL)
        {
            for(int i=0; i<InArmSuck.iMotRow; i++)
            {
                for(int j=0; j<InArmSuck.iMotCol; j++)
                {
                    iMot=InArmSuck.Suck[i][j].iMotNo;
                    if(bCleanZMotToPlace[i][j])
                    {
                        if(bflag[i][j]==false)
                            bflag[i][j]=MOT[iMot].MotorMove(iZPos[i][j]);
                    }
                    else
                    {
                        bflag[i][j]=true;
                    }
                }
            }
        }
        else
        {
            ShowMyMessage("HPSuckTeamList has no data!");
            return false;
        }
    }

    flag=true;
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            if(bflag[i][j]==false)
                flag=false;
        }
    }

    return flag;
}
//------------------------------------------------------------------------------
// AI(W906-AutoCleanFoundation) 20260721: SetAutoCleanStringGrid itself is NOT
// in the task brief's Part A/HAL-helper list, but 2 in-scope functions
// (RestoreCleanKitData / CheckCleaningCount) call it directly -- translated as
// a required dependency, same treatment the brief gives implicit small
// pass-through helpers.  golden AutoClean.cpp:1171.
//------------------------------------------------------------------------------
void SetAutoCleanStringGrid(int X, int Y, AnsiString Str)                       //Steven 20180524 : Fixed for clean count
{
    fMain->AutoCleanStringGrid->Cells[X][Y]=Str;
    if(Y>0 && Y<4)                                                              //Steven 20200428 : Fixed for auto clean
    {
        if(MOT[MMAutoCleanKit].Tray.Data[X][Y-1]!=NULL_IC)                      //Steven 20210127 : 修正Auto clean新檔時原, 會清掉Count
            MOT[MMAutoCleanKit].Tray.iCleanCount[X][Y-1]=atoi(Str.c_str());
        else
            MOT[MMAutoCleanKit].Tray.iCleanCount[X][Y-1]=0;
    }
}
//------------------------------------------------------------------------------
// AI(W906-AutoCleanFoundation) 20260721: ReadWriteAutoCleanCount itself is NOT
// in the task brief's Part A/HAL-helper list, but RestoreCleanKitData (in
// scope, below) calls it as its final statement -- translated as a required
// dependency. golden AutoClean.cpp:1190.
//------------------------------------------------------------------------------
void ReadWriteAutoCleanCount(bool bRead, bool bReset)                           //Steven 20180524 : Fixed for clean count
{
    AnsiString Str, S="", szDir="";
    int iTemp=0;
    if(bRunAutoClean==false ||                                                  //Ifor 20191024 : add Fix Auto Clean 完成後將的Clean Count錯誤清除
       (bRunAutoClean==true && bReset &&                                        //Steven 20211220 : 修正Clean Count手動歸零的問題
        FLCarryKit.UseSiteNoIC() &&
        BLCarryKit.UseSiteNoIC() &&
        FTestSuck.UseSiteNoIC() &&
        BTestSuck.UseSiteNoIC() &&
        InArmSuck.HasIC()==false))
    {
        if(IniConfig.bE43_1_AutoCleanCountSaveFolder)                           //Steven 20250527 : Save auto clean count to folder
        {
            szDir.sprintf("D:\\HT9045\\IniData\\DefineAutoClean\\AutoCleanCount.Data");
        }
        else
        {
            S=GetLastOpenFN();
            szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
        }

        for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)
        {
            for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)
            {
                if(bRead)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_FINSH_IC ||   //Jimmychiu 20250103 : fixed for auto clean count to 0 issue
                       MOT[MMAutoCleanKit].Tray.Data[X][Y]==CLEAN_FINISH_IC    ||
                       MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_IC       ||   //Isaac 20180417 (jou) : fix auto clean執行一半治具狀態, clean count會歸零
                       MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_NULL_CLEAN_IC  )
                    {
                        Str.sprintf("iAutoCleanPad_CountTime_%d_%d", Y, X);
                        S=ReadIniData(szDir, "Configuration", Str, AnsiString("0"));
                        SetAutoCleanStringGrid(X, Y+1, S);
                    }
                }
                else
                {
                    Str.sprintf("iAutoCleanPad_CountTime_%d_%d", Y, X);
                    if(bReset)                                                  //Steven 20211220 : 修正Clean Count手動歸零的問題
                        SetAutoCleanStringGrid(X, Y+1, AnsiString("0"));

                    S=fMain->AutoCleanStringGrid->Cells[X][Y+1];
                    iTemp=atoi(S.c_str());                                      //Jimmychiu 20250103 : fixed for auto clean count to 0 issue
                    S=IntToStr(iTemp);
                    WriteIniDataNoLog(szDir, "Configuration", Str, S);
                }
            }
        }
        WriteIniDataNoLog(szDir, "Configuration", "iIndexArmAutoCleanCnt", TestIF_File.iIndexArmAutoCleanCnt);  //Sam 20250820 : AutoClean 在 Index Arm 下做過切換就累加一次
    }
}
//------------------------------------------------------------------------------
bool MoveInArmZ_Shuttle_Pick(eWhichShuttle iSht, int iSelRow)
{
    int iShuttlePick[MAX_ARM_Row][MAX_ARM_Col];
    bool flag[MAX_ARM_Row][MAX_ARM_Col]={{true, true, true, true}, {true, true, true, true}};
    bool bNeedDown=false;

    if(iSht==euShuttle1)
        ptrInSHT=&FLCarryKit;
    else
        ptrInSHT=&BLCarryKit;

    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            if(iSht==euShuttle1)
            {
                if((CosFunction.bDeviceMapTest &&
                   fContact->fShow==true &&
                    iContactMode==CONTACT_DEVICE_MAP_CHECK) ||
                   fContact->IsRun2DCheck()==true)                              //JerryYang 20250220 : 2DID預鎖順序檢查功能                      //Steven 20221114 :Add for device map function
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle1_Place[i][j] -
                                       InArmOffSet[InOfsInSh1]->GetPlace() +
                                       InArmOffSet[InOfsInSh1]->GetPickUp() +
                                       InArmOffSet[InOfsInSh1+iSelRow-1]->SingleOffSet->dPickUpOffSet[i][j];
                }
                else if(IniConfig.bEnableAutoCleanFunction &&
                        TestIF_File.iAutoClean_Function &&
                        bRunAutoClean &&
                        IniConfig.bE48_ShuttleUse4Offset_Autoclean &&           //20140923 wei : For Shuttle Auto Clean
                        TestIF.bEnableAutoAlignment==false)                     //KenHsieh 20220923 : add AOA功能開啟不套影
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle1_Place[i][j] -
                                       InArmOffSet[InOfsInSh1]->GetPlace() +
                                       InArmOffSet[InOfsInSh1_AutoClean]->GetPickUp() +
                                       InArmOffSet[InOfsInSh1_AutoClean+iSelRow-1]->SingleOffSet->dPickUpOffSet[i][j];
                }
                else if(CosFunction.bAutoCleanOffsetUseSingleSetting &&         //Sam 20220720 : AutoClean Offset 值使用 Clean 設定 Offset 來套用
                        TestIF.bEnableAutoAlignment==false)                     //KenHsieh 20220923 : add AOA功能開啟不套影
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle1_Place[i][j] +
                                       TestIF.iAutoClean_Shuttle1PickOffset -
                                       InArmOffSet[InOfsInSh1]->GetPlace();
                }
                else
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle1_Place[i][j]+TestIF.iAutoClean_Shuttle1PickOffset;
                }

                if(IniConfig.bE33InOutArmZOffsetSameOne)                        //JerryYang 20210811 : 修正使用E33功能後, shuttle的pick up高度被release offset影響的問題
                {
                    iShuttlePick[i][j]=iShuttlePick[i][j] -
                                       InArmOffSet[InOfsLoader]->GetPlace(i, j) +
                                       InArmOffSet[InOfsLoader]->GetPickUp(i, j);
                }
            }
            else
            {
                if(CosFunction.bDeviceMapTest &&
                   fContact->fShow==true &&
                   iContactMode==CONTACT_DEVICE_MAP_CHECK)                      //Steven 20221114 :Add for device map function
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle2_Place[i][j] -
                                       InArmOffSet[InOfsInSh2]->GetPlace() +
                                       InArmOffSet[InOfsInSh2]->GetPickUp() +
                                       InArmOffSet[InOfsInSh2+iSelRow-1]->SingleOffSet->dPickUpOffSet[i][j];
                }
                else if(IniConfig.bEnableAutoCleanFunction &&
                        TestIF_File.iAutoClean_Function &&
                        bRunAutoClean &&
                        IniConfig.bE48_ShuttleUse4Offset_Autoclean &&           //20140923 wei : For Shuttle Auto Clean
                        TestIF.bEnableAutoAlignment==false)                     //KenHsieh 20220923 : add AOA功能開啟不套影
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle2_Place[i][j] -
                                       InArmOffSet[InOfsInSh2]->GetPlace() +
                                       InArmOffSet[InOfsInSh2_AutoClean]->GetPickUp()+
                                       InArmOffSet[InOfsInSh2_AutoClean+iSelRow-1]->SingleOffSet->dPickUpOffSet[i][j];
                }
                else if(CosFunction.bAutoCleanOffsetUseSingleSetting &&         //Sam 20220720 : AutoClean Offset 值使用 Clean 設定 Offset 來套用
                        TestIF.bEnableAutoAlignment==false)                     //KenHsieh 20220923 : add AOA功能開啟不套影
                {
                    if(IniConfig.bE34InOutArmPitchZOffsetSameOne)
                        iShuttlePick[i][j]=Prod.ZInArm_Shuttle2_Place[i][j] +
                                           TestIF.iAutoClean_Shuttle2PickOffset -
                                           InArmOffSet[InOfsInSh1]->GetPlace();
                    else
                        iShuttlePick[i][j]=Prod.ZInArm_Shuttle2_Place[i][j] +
                                           TestIF.iAutoClean_Shuttle2PickOffset -
                                           InArmOffSet[InOfsInSh2]->GetPlace();
                }
                else
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle2_Place[i][j] +
                                       TestIF.iAutoClean_Shuttle2PickOffset;    //ChungHung 20141117 : 改為iAutoClean_Shuttle2PickOffset
                }

                if(IniConfig.bE33InOutArmZOffsetSameOne)                        //JerryYang 20210811 : 修正使用E33功能後, shuttle的pick up高度被release offset影響的問題
                {
                    iShuttlePick[i][j]=iShuttlePick[i][j] -
                                       InArmOffSet[InOfsLoader]->GetPlace(i, j) +
                                       InArmOffSet[InOfsLoader]->GetPickUp(i, j);
                }
            }
        }
    }

    if(bUse8Picker)                                                             //Steven 20240221 : 齊平8治具auto clean
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                bNeedDown=(InArmSuck.Item[i][j]==NULL_IC &&                     //Steven 20240306 : 改用NeedSuckCleanPad
                           InArmSuck.iNeedSuck[i][j]!=NULL_IC &&
                           InArmSuck.iNeedSuck[i][j]!=HAS_NULL_CLEAN_IC &&
                           InArmSuck.iNeedSuck[i][j]!=HAS_NULL_IC);             //JerryYang 20250220 : 2DID預鎖順序檢查功能

                flag[i][j]=bNeedDown;
            }
        }
    }
    else
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            bNeedDown=(InArmSuck.Item[1][j]==NULL_IC &&                         //Steven 20240306 : 改用NeedSuckCleanPad
                       InArmSuck.iNeedSuck[1][j]!=NULL_IC &&
                       InArmSuck.iNeedSuck[1][j]!=HAS_NULL_CLEAN_IC &&
                       InArmSuck.iNeedSuck[1][j]!=HAS_NULL_IC);                 //JerryYang 20250220 : 2DID預鎖順序檢查功能

            flag[1][j]=bNeedDown;
            flag[0][j]=false;                                                   //RogerYang 20251119 : bUse8Picker==false的情況下，治具都放在下排
        }
    }

    bool bRet=InArmZMoveDown(flag, iShuttlePick, false);                        //Steven 20251020 : modify for auto clean
    return bRet;
}
//------------------------------------------------------------------------------
bool CheckInSuckICFallDown(int KCode)
{
    int ret;
    AnsiString ErrPart="";                                                      //Steven 20110216 : 合併Alarm
    bool bHasErr=false;                                                         //Steven 20110216 : 合併Alarm
    bool bStatus;

    int iSuckRow=0;                                                             //ChungHung 20141121 add for Use HotPlate AutoClean

    if(IniConfig.bE43AutoCleanUseHotplate)
        iSuckRow=0;
    else
        iSuckRow=1;

    if(LastSet.iRealDummy!=REALLY)
        return false;

    for(int i=iSuckRow; i<InArmSuck.iMaxRow; i++)                               //ChungHung 20141121 add for Use HotPlate AutoClean
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            #ifndef SOFT_SIMULTE
                if((InArmSuck.Item[i][j]==HAS_CLEAN_IC ||
                    InArmSuck.Item[i][j]==CLEAN_FINISH_IC ||
                    InArmSuck.Item[i][j]==HAS_CLEAN_FINSH_IC) &&
                    InArmSuck.Suck[i][j].GetStatus()==false)                    //ChungHung 20141121 add for Use HotPlate AutoClean
                {
                    ErrPart+=InArmSuck.Suck[i][j].sName;
                    bHasErr=true;
                }
            #else
                if(fMain->chkCleanPadPickErr->Checked==true)
                    bHasErr=true;
            #endif
        }
    }

    if(bHasErr==true)
    {
        ret=ShowErrorMessage("JAM0128", KCode, MInArmX, false, ErrPart);        //JerryYang 20160511 JAM0126->JAM0128,將IC改Clean pad掉料的alarm code分開

        if(ret==K_RETRY)
        {
            for(int i=iSuckRow; i<InArmSuck.iMaxRow; i++)                       //ChungHung 20141121 add for Use HotPlate AutoClean
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    bStatus=InArmSuck.Suck[i][j].GetStatus();
                    if((InArmSuck.Item[i][j]==HAS_CLEAN_IC ||
                        InArmSuck.Item[i][j]==CLEAN_FINISH_IC ||
                        InArmSuck.Item[i][j]==HAS_CLEAN_FINSH_IC) &&
                        bStatus==false)                                         //ChungHung 20141121 add for Use HotPlate AutoClean
                    {
                        InArmSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);
                    }
                }
            }
            return true;
        }
        else
        {
            for(int i=iSuckRow; i<InArmSuck.iMaxRow; i++)                       //ChungHung 20141121 add for Use HotPlate AutoClean
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    #ifdef SOFT_SIMULTE
                        if(fMain->chkCleanPadPickErr->Checked==true)
                            bStatus=(i==1 && j==0)?false:true;
                    #else
                        bStatus=InArmSuck.Suck[i][j].GetStatus();
                    #endif

                    if((InArmSuck.Item[i][j]==HAS_CLEAN_IC ||
                        InArmSuck.Item[i][j]==CLEAN_FINISH_IC ||
                        InArmSuck.Item[i][j]==HAS_CLEAN_FINSH_IC) &&
                       bStatus==false)                                          //ChungHung 20141121 add for Use HotPlate AutoClean
                    {
                        InArmSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);
                    }
                }
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool TrayHasCleanIC()
{
    bool bFlag=false;

    if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                            //Clean Kit
    {
        for(int Y=0; Y<TestIF_File.iAutoClean_YDivision; Y++)
            for(int X=0; X<TestIF_File.iAutoClean_XDivision; X++)
                if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_IC)
                    return true;

        if(bFlag==false)                                                        //Steven 20160218 : 避免Auto Clean 對Count不同時導致Hang Up
        {
            for(int Y=0; Y<TestIF_File.iAutoClean_YDivision; Y++)
                for(int X=0; X<TestIF_File.iAutoClean_XDivision; X++)
                    if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==CLEAN_FINISH_IC)    //Steven 20211201 : 造成2x4異常資料時原, 會回到原本值
                        MOT[MMAutoCleanKit].SetTraySingleData(X, Y, HAS_CLEAN_IC);
        }
    }
    return bFlag;
}
//------------------------------------------------------------------------------
int TrayHasCleanICCount()                                                       //wei 2015090
{
    int iFlag=0;

    if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit ||
       IniConfig.bE43AutoCleanUseHotplate==true)                                //Clean Kit
    {
        for(int Y=0; Y<TestIF.iAutoClean_YDivision; Y++)
        {
            for(int X=0; X<TestIF.iAutoClean_XDivision; X++)
            {
                if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_IC ||
                   MOT[MMAutoCleanKit].Tray.Data[X][Y]==CLEAN_FINISH_IC ||
                   MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_NULL_CLEAN_IC)      //Steven 20210312 : 修正對第二次auto clean時,出現Clean Pad數量錯誤問題
                {
                    iFlag++;
                }
            }
        }
    }
    return iFlag;
}
//------------------------------------------------------------------------------
// AI(W906-AutoCleanFoundation) 20260721: golden SearchCleanNum (AutoClean.cpp:1245,
// NOT in the task brief's Part A list) -- CheckCleaningCount (in-scope, below)
// calls it, but ONLY inside `if(CosFunction.bCleanCountAlarmByMin)` (a narrow,
// default-false, Gigas-specific gate per its own "Jimmychiu 20260212" comment).
// Translated here as a file-local (`static`) helper: the CORE computation (the
// returned iMin, the minimum per-cell clean count across the whole
// MOT[MMAutoCleanKit] grid) is FAITHFUL and is all CheckCleaningCount's return
// value depends on. Golden's UI-mirror tail is DELIBERATELY DROPPED (does not
// affect the return value, and would need brand-new facade infrastructure
// disproportionate for a function outside this wave's own scope):
//   - fShowBinSelect->ed_AutoCleanCount->Text=... : fShowBinSelect has NO
//     link-visible home in this tree (only a TU-local seam inside csystem.cpp,
//     grepped) -- standing up a whole new facade class for one Text= write
//     that nothing else reads is out of scope here.
//   - fMain->pnlCleanCount->Font->Color=clRed/clNavy : this wave's
//     FormsFacade TfMainPanel stand-in has no ->Font sub-object (golden's
//     nested TFont* shape); Caption is kept (already real, Part B), the
//     Font->Color cosmetic paint is dropped.
//   - the WAR16313 AlarmCount-reached alarm trigger: a real side effect, but
//     one this wave's actual required behavior (CheckCleaningCount's bool
//     return) does not depend on; left for whichever wave gives SearchCleanNum
//     its own full translation.
//------------------------------------------------------------------------------
static int SearchCleanNum()
{
    int iMin=9999;                                                              //Steven 20171211 (Wei) : 修正計算方式
    int iNow=0;
    if(TestIF_File.iAutoClean_Function==0)                                      //Ifor 20191024 : add 避免Auto Clean 未開啟時 Clean Count會歸零
    {
        iMin=atoi(fCleaning->edCleaningCount->Text.c_str());
    }
    else
    {
        for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)
        {
            for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)
            {
                if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_FINSH_IC ||
                   MOT[MMAutoCleanKit].Tray.Data[X][Y]==CLEAN_FINISH_IC    ||
                   MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_IC       ||   //Isaac 20180417 (jou) : fix auto clean執行一半治具狀態, clean count會歸零
                   MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_NULL_CLEAN_IC  )
                {
                    iNow=atoi(fMain->AutoCleanStringGrid->Cells[X][Y+1].c_str());
                    if(iNow!=0 && iNow<iMin)
                    {
                        iMin=iNow;
                    }
                }
            }
        }

        if(iMin==9999)                                                          //Steven 20171211 (Wei) : 修正計算方式
            iMin=0;

        AnsiString sAutoCleanCount=IntToStr(iMin);                              //Jimmychiu 20250103 : fixed for auto clean count to 0 issue
        fCleaning->edCleaningCount->Text=sAutoCleanCount;
        fMain->pnlCleanCount->Caption=AnsiString().sprintf("Cleaned Count %d / %d", iMin, TestIF_File.iAutoClean_AlarmCount);
    }
    return iMin;
}
//------------------------------------------------------------------------------
bool CheckCleaningCount()
{
    int iXpos=-1, iYpos=0, iCleanCount=0;
    int iXItem;
    bool bFlag=false;
    if(CosFunction.bCleanCountAlarmByMin)                                       //Jimmychiu 20260212 : Gigas Clear alarms based on minimum usage count
    {
        iCleanCount=SearchCleanNum();                                           //JerryYang 20170801 (wei) iCleanCount為0也要回傳true
        if(iCleanCount<TestIF.iAutoClean_AlarmCount)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    iXItem=MOT[MMAutoCleanKit].Tray.XItem;

    for(int i=0; i<TestIF.iAutoClean_DeveicePices; i++)
    {
        if(TestIF.iTestMode==QualSite2X2 &&                                     //jou 20210712 : 修正 QualSite2X2 Tray pitch 小於2666 hang up
           bCleanKitPitchLess4000==true &&
           TestIF.iAutoClean_DeveicePices==4 &&
           TestIF.iAutoClean_XDivision==8)
        {
            iXpos=i*2;
        }
        else
        {
            iXpos++;
        }

        if(iXpos>=iXItem)
        {
            iXpos=0;
            iYpos++;
        }

        if((TestIF_File.iTestMode==QualSite1X4 ||                               //Steven 20220218 : 修正1x4 auto clean for 不同X-Pitch
            TestIF_File.iTestMode==_8Site2X4N) &&
            TestIF.dAutoClean_XPitch<1333 &&                                    //Steven 20240305 : 修正Auto Clean Pitch判斷
           iInArmType!=e9045_1x4_1_Ac)
        {
            if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]!=NULL_IC)            //Steven 20210127 : 修正auto clean count到達不會alarm
            {
                iCleanCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                if(iCleanCount<0)                                               //Steven 20210127 : 修正auto clean count到達不會alarm
                    iCleanCount=0;
                if(iCleanCount<TestIF.iAutoClean_AlarmCount)                    //JerryYang 20170801 (wei) iCleanCount為0也要回傳true
                {
                    bFlag=true;
                    break;
                }
            }
        }
        else if((TestIF.iTestMode!=DualSite     &&
                 TestIF.iTestMode!=QualSite2X2  &&
                 TestIF.iTestMode!=TriSite1X3   &&
                 TestIF.iTestMode!=_6Site2X3N)  &&                              //Steven 20220425 : 2X3NN Mode
                (iAutoCleanUseXPitch==2 || iAutoCleanUseXPitch==4) &&
                TestIF_File.iAutoClean_DeveicePices<=4 &&                       //JerryYang 20200701 修正auto clean次數到達時沒有發出alarm的問題
                TestIF_File.iAutoClean_XDivision>4)                             //Steven 20240305 : 修正Auto Clean Pitch判斷
        {
            if(iXpos+iYpos*MOT[MMAutoCleanKit].Tray.XItem<TestIF.iAutoClean_DeveicePices)
            {
                if(InArmSuck.iModeX==9 &&
                   TestIF_File.iAutoClean_DeveicePices==4 &&
                   iCloseSiteStep_2x8==3)                                       //KevinCheng 20260126 : 兩顆治具狀態下增加特殊的Site判斷
                {
                    iCleanCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                    if(iCleanCount<0)
                        iCleanCount=0;
                    if(iCleanCount<TestIF.iAutoClean_AlarmCount)
                    {
                        bFlag=true;
                        break;
                    }
                }
                else if(MOT[MMAutoCleanKit].Tray.Data[iXpos*2][iYpos]==HAS_CLEAN_IC ||
                        MOT[MMAutoCleanKit].Tray.Data[iXpos*2][iYpos]==CLEAN_FINISH_IC)
                {
                    iCleanCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos*2][iYpos+1].c_str());
                    if(iCleanCount<0)                                           //Steven 20210127 : 修正auto clean count到達不會alarm
                        iCleanCount=0;
                    if(iCleanCount<TestIF.iAutoClean_AlarmCount)                //JerryYang 20170801 (wei) iCleanCount為0也要回傳true
                    {
                        bFlag=true;
                        break;
                    }
                }
            }
        }
        else
        {
            if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]!=NULL_IC)            //Steven 20210127 : 修正auto clean count到達不會alarm
            {
                iCleanCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                if(iCleanCount<0)                                               //Steven 20210127 : 修正auto clean count到達不會alarm
                    iCleanCount=0;
                if(iCleanCount<TestIF.iAutoClean_AlarmCount)                    //JerryYang 20170801 (wei) iCleanCount為0也要回傳true
                {
                    bFlag=true;
                    break;
                }
            }
        }
    }

    return bFlag;
}
//------------------------------------------------------------------------------
bool CheckInArmSuckFromCleanKitICFallDown(bool bRetry)
{
    bool bIsSuckICFallDown[MAX_SOCKET_ROW][MAX_SOCKET_COL]={false}, bHasErr=false;
    AnsiString ErrPart="";
    int ret=0;

    if(LastSet.iRealDummy==DUMMY)
        return false;

    if(LastSet.iRealDummy==REALLY)
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                if(InArmSuck.Suck[i][j].Enable       &&                         //ChungHung 20141121 add for Use HotPlate AutoClean 1--->i
                   InArmSuck.Suck[i][j].SenUsing!="" &&
                   InArmSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                   InArmSuck.Item[i][j]!=NULL_IC)                               //wei 20160130
                {
                    if(InArmSuck.Suck[i][j].GetStatus()==false)
                    {
                        bIsSuckICFallDown[i][j]=true;
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                        bHasErr=true;
                    }
                    else
                    {
                        bIsSuckICFallDown[i][j]=false;
                    }
                }
                else
                {
                    bIsSuckICFallDown[i][j]=false;
                }
            }
        }
    }

    if(ret)                                                                     //Debug用
    {
        bIsSuckICFallDown[1][0]=true;
        ErrPart+=InArmSuck.Suck[1][0].sName;
        bHasErr=true;
    }

    if((bUse8Picker ||
        IniConfig.bE43AutoCleanUseHotplate) &&
       bRetry)                                                                  //JerryYang 20160503 第二次吸治具,又要重複第一次選retry
    {
        if(bIsSuckICFallDown[0][0] || bIsSuckICFallDown[0][1] || bIsSuckICFallDown[0][2]|| bIsSuckICFallDown[0][3])
        {
            if(InArmSuck.Item[1][0] && InArmSuck.Item[1][1] && InArmSuck.Item[1][2] && InArmSuck.Item[1][3])
            {
                for(int i=0; i<MAX_ARM_Col ; i++)
                    bIsSuckICFallDown[0][i]=false;

                if(bIsSuckICFallDown[1][0]==false && bIsSuckICFallDown[1][1]==false && bIsSuckICFallDown[1][2]==false && bIsSuckICFallDown[1][3]==false)
                {
                    ErrPart="";
                    bHasErr=false;
                }
            }
        }
    }

    if(bHasErr)
    {
        if(bRetry)
            ret=ShowErrorMessage("JAM0128", K_RETRY, MInArmX, false, ErrPart);  //JerryYang 20160511 JAM0126->JAM0128,將IC改Clean pad掉料的alarm code分開
        else
            ret=ShowErrorMessage("JAM0128", K_SKIP, MInArmX, false, ErrPart);   //JerryYang 20160511 JAM0126->JAM0128,將IC改Clean pad掉料的alarm code分開

        if(SoftStop)
            fMain->Pause("CheckInArmSuckFromCleanKitICFallDown");

        if(bRetry)
        {
            if(ret==K_RETRY)
            {
                for(int i=0; i<2; i++)                                          //ChungHung 20141121 add for Use HotPlate AutoClean 1--->i
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(bIsSuckICFallDown[i][j])
                        {
                            MOT[MMAutoCleanKit].SetTraySingleData(InArmSuck.iAutoCleanRecX[i][j], InArmSuck.iAutoCleanRecY[i][j], HAS_CLEAN_IC);    //JerryYang 20160210 修正auto clean 使用hot plate hang up問題
                            InArmSuck.SetItemData(i, j, NULL_IC);
                            bInArmSuckActive[i][j]=true;                        //JerryYang 20160404 [i][j]-->[1][j],修正Drop error後retry後沒有補治具問題
                            iAutoCleanPickPlateY=InArmSuck.iAutoCleanRecY[i][j]; //JerryYang 20160428 修正重複後hang up,忘記紀錄回原本的 Kit Y
                        }
                    }
                }
                return true;
            }
        }
        else
        {
            if(ret==K_SKIP)
            {
                for(int i=0; i<2; i++)                                          //ChungHung 20141121 add for Use HotPlate AutoClean 1--->i
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(bIsSuckICFallDown[i][j])
                        {
                            InArmSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);     //wei 20160130
                        }
                    }
                }
                return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void DoPlaceToKitSwapData(bool bPick, int iIC_Type, int iSuckRow, int iSuckCol, int iKitRow, int iKitCol)     //Steven 20170109 : 將資料到Kit資料交換改成Function
{
    if(bPick==bAutoPick)
    {
        int iCleanListType=-1;
        if(iIC_Type==HAS_CLEAN_IC)
        {
            iCleanListType=iIC_Type;
        }
        InspectInArmPosition(MMAutoCleanKit, iSuckRow, iSuckCol, iKitRow, iKitCol, true);
        PlaceToCleanList->SetArrPlateXY(iSuckRow, iSuckCol, 0, iKitRow, iKitCol, iCleanListType);      //Jimmychiu 20230417 : Record the position after placing the IC
        MOT[MMAutoCleanKit].SetTraySingleData(iKitCol, iKitRow, iIC_Type);
        AnsiString sTime=AnsiString().sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
        CleanKitRecord[iKitRow][iKitCol]=sTime;                         //Sam 20230619 : 新增 Clean治具時間 Log
        bCleanKitSuckDuplicateErr[iSuckRow][iSuckCol] =false;
        InArmSuck.SetItemData(iSuckRow, iSuckCol, iIC_Type);
        InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol]  =iKitCol;
        InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol]  =iKitRow;
        InArmSuck.PordRec[iSuckRow][iSuckCol].AddPickCleanPad(iSuckRow, iSuckCol, iKitCol, iKitRow, iIC_Type); //Sam 20230616 : Add Auto Clean Record
    }
}
//==============================================================================
bool PickFromCleanKit(int iRowKit)
{
    bool flag1=true;
    int iSuckRow=0, iSuckCol=0, iKitRow=0, iKitCol=0;

    AnsiString sTime="";                                                        //Sam 20230619 : 新增 Clean治具時間 Log
    GetTimeInfo();

    if(bUse8Picker)                                                             //Steven 20201014 : 齊平8治具auto clean
    {
        if(iRowKit==1 || iRowKit==3)
            iSuckRow=0;
        else if(iRowKit==2 || iRowKit==4)
            iSuckRow=1;
    }
    else
    {
        iSuckRow=1;
    }

    for(int j=0; j<4; j++)
    {
        iSuckCol=GetAutoCleanPickStep(j);                                       //Steven 20240918 : fixed for auto clean
        if(iSuckCol==-1)
            continue;
        if(USE_PICKER_COUNT==ep1Picker && j>=1)
            continue;

        iKitCol=iAutoCleanPickPlateX+iAutoCleanUseXPitch*(j-iAutoCleanStart);
        iKitRow=iAutoCleanPickPlateY;

        if(iKitCol<0 || iKitRow<0 ||
           iKitCol>=50 || iKitRow>=50)
        {
            sTime="";
            continue;                                                           //Steven 20240729 : fixed for auto clean pick 2 times from kit
        }

        if(InArmSuck.Suck[iSuckRow][iSuckCol].Error==true)                      //Steven 20220210 : fixed for Auto Clean吸嘴異常
        {
        }
        else if(bInArmSuckActive[iSuckRow][iSuckCol] &&
                InArmSuck.Item[iSuckRow][iSuckCol]==NULL_IC)                    //ChungHung 20131120 AutoClean use Hotplate1
        {
            if(InArmSuck.Suck[iSuckRow][iSuckCol].Suck())                       //ChungHung 20131120 AutoClean use Hotplate1
            {
                if(Special_2X6_Tray_XItem7() && (iRowKit==3 || iRowKit==4))     //Sam 20250712 : 新增特殊流程 2X6 AutoClean Tray XItem=7
                {
                    if(iKitCol>=iAutoCleanUseXPitch)
                    {
                        iKitCol=iKitCol-iAutoCleanUseXPitch;
                    }
                }
                InspectInArmPosition(MMAutoCleanKit, iSuckRow, iSuckCol, iKitRow, iKitCol, true);
                PlaceToCleanList->SetArrPlateXY(iSuckRow, iSuckCol, 0, iKitRow, iKitCol, HAS_CLEAN_IC);      //Jimmychiu 20230417 : Record the position after placing the IC
                bInArmSuckActive[iSuckRow][iSuckCol] =false;
                MOT[MMAutoCleanKit].SetTraySingleData(iKitCol, iKitRow, HAS_NULL_CLEAN_IC);
                sTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
                CleanKitRecord[iKitRow][iKitCol]=sTime;                         //Sam 20230619 : 新增 Clean治具時間 Log
                bCleanKitSuckDuplicateErr[iSuckRow][iSuckCol] =false;
                InArmSuck.SetItemData(iSuckRow, iSuckCol, HAS_CLEAN_IC);
                InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol]  =iKitCol;
                InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol]  =iKitRow;
                InArmSuck.PordRec[iSuckRow][iSuckCol].AddPickCleanPad(iSuckRow, iSuckCol, iKitRow, iKitCol, HAS_CLEAN_IC);  //Sam 20230616 : Add Auto Clean Record
            }
            else if(InArmSuck.Suck[iSuckRow][iSuckCol].Error==false)
            {
                flag1=false;
            }
        }
        else if(bInArmSuckActive[iSuckRow][iSuckCol]==false &&
                InArmSuck.Item[iSuckRow][iSuckCol]==NULL_IC)
        {
            if(MOT[MMAutoCleanKit].Tray.Data[iKitCol][iKitRow]==HAS_CLEAN_IC &&
               (iKitCol<MOT[MMAutoCleanKit].Tray.XItem))
            {
                if(Special_2X6_Tray_XItem7() && (iRowKit==3 || iRowKit==4))     //Sam 20250712 : 新增特殊流程 2X6 AutoClean Tray XItem=7
                {
                    iKitCol=iKitCol+iAutoCleanUseXPitch*3;
                }
                PlaceToCleanList->SetArrPlateXY(iSuckRow, iSuckCol, 0, iKitRow, iKitCol, -1);      //Jimmychiu 20230417 : Record the position after placing the IC
                MOT[MMAutoCleanKit].SetTraySingleData(iKitCol, iKitRow, HAS_NULL_CLEAN_IC);
                sTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
                CleanKitRecord[iKitRow][iKitCol]=sTime;                         //Sam 20230619 : 新增 Clean治具時間 Log
                bCleanKitSuckDuplicateErr[iSuckRow][iSuckCol] =false;
                InArmSuck.SetItemData(iSuckRow, iSuckCol, HAS_NULL_CLEAN_IC);
                InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol]  =iKitCol;
                InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol]  =iKitRow;
                InArmSuck.PordRec[iSuckRow][iSuckCol].AddPickCleanPad(iSuckRow, iSuckCol, iKitCol, iKitRow, HAS_NULL_CLEAN_IC); //Sam 20230616 : Add Auto Clean Record
            }
        }
    }
    return flag1;
}
//------------------------------------------------------------------------------
bool PlaceToCleanKit()
{
    bool flag=true;
    int iSuckRow=0, iSuckCol=0, iKitRow=0, iKitCol=0;

    AnsiString sTime="";
    int iP=0;
    int iPlateR[MAX_ARM_Row][MAX_ARM_Col];                                      //Jimmychiu 20230417 : Record the position after placing the IC
    int iPlateC[MAX_ARM_Row][MAX_ARM_Col];
    bool bSuck[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iPlateR, sizeof(iPlateR));
    ZeroMemory(iPlateC, sizeof(iPlateC));
    ZeroMemory(bSuck, sizeof(bSuck));
    GetTimeInfo();

    if(PlaceToCleanList->GetHPFirstTeam(&iP, iPlateR, iPlateC, bSuck))
    {
        if(PlaceToCleanList->GetHPFirstTeamMotUse(bCleanZMotToPlace)==false)    //RogerYang 20250624 Add HotPlate ErrMessage
        {
            ShowMyMessage(AnsiString().sprintf("%s %s Task=%d", __FUNC__, "No GetHPFirstTeamMotUse", 0));
            return false;
        }
        PlaceToCleanList->GetHPFirstTeamSuckUse(bCleanZSuckToPlace);

        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(bCleanZSuckToPlace[i][j]==true)
                {
                    iSuckRow=i;
                    iSuckCol=j;
                    iKitRow =InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol];
                    iKitCol =InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol];

                    if(iKitCol<0   || iKitRow<0 ||
                       iKitCol>=50 || iKitRow>=50)
                    {
                        ShowMyMessage("CleanKitRecord error1", "CleanKitRecord 錯誤3");
                        InArmSuck.SetItemData(i, j, NULL_IC);
                    }
                    else if((bCleanZMotToPlace[i][j]==false &&                  //資料要交換, 治具不用下去, 就是HAS_NULL_CLEAN_IC
                             InArmSuck.Item[i][j]!=NULL_IC) ||
                            (bCleanZMotToPlace[i][j]==true &&
                             InArmSuck.Item[i][j]!=NULL_IC &&
                             InArmSuck.Suck[i][j].Destroy()))
                    {
                        InArmSuck.SetItemData(i, j, NULL_IC);
                        InArmSuck.PordRec[i][j].AddPlaceCleanPad(i, j, InArmSuck.iAutoCleanRecY[i][j], InArmSuck.iAutoCleanRecX[i][j]);    //Sam 20230616 : Add Auto Clean Record
                        InArmSuck.PordRec[1][j].SaveRecordCleanPad();
                        MOT[MMAutoCleanKit].SetTraySingleData(InArmSuck.iAutoCleanRecX[i][j], InArmSuck.iAutoCleanRecY[i][j], CLEAN_FINISH_IC);   //JerryYang 20160210 修正auto clean 使用hot plate hang up問題
                        sTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
                        CleanKitRecord[InArmSuck.iAutoCleanRecY[i][j]][InArmSuck.iAutoCleanRecX[i][j]]=sTime;   //Sam 20230619 : 新增 Clean治具時間 Log
                        bInArmSuckActive[i][j]=false;
                        bInArmCheckDestroyACT[i][j]=true;                       //Sam 20240124 : AutoClean 時回吹判斷
                    }
                    else
                    {
                        if(InArmSuck.Item[i][j]!=NULL_IC &&
                           InArmSuck.Suck[1][j].Error==false)
                            flag=false;
                    }
                }
            }
        }
    }

    if(flag==false)
        return false;

    for(int j=0; j<InArmSuck.iMaxCol; j++)
    {
        iSuckCol=j;
        if(bInArmSuckActive[iSuckRow][iSuckCol] &&
           InArmSuck.Item[iSuckRow][iSuckCol]!=NULL_IC)
            return false;
    }
    return flag;
}
//==============================================================================
//  搜尋XY到Clean Kit
//==============================================================================
bool MoveInArmXYPickCleanKit(int iPick, int iShuttleRow, eWhichShuttle iSht)    //ChungHung 20140709 add iSht for SCK CloseSiteByArm Autoclean
{
    int iXPos       =0;
    int iYPos       =0;
    int iMovePitchX =GetXPitchOfCleanKit();
    int iMovePitchY =GetYPitchOfCleanKit();
    int iOffsetPos  =InOfsAutoClean;
    int iSuckRow=0, iSuckCol=0;
    int iKitRow=0, iKitCol=0;
    double dMovePitchX=iMovePitchX;

    if(USE_PICKER_COUNT==ep1Picker)
    {
        dMovePitchX=0;
    }
    else if(USE_PICKER_COUNT==ep16Picker)                                            //齊平為第四種治具
    {
        dMovePitchX=double(iMovePitchX)/7.0;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchBb ||                               //齊平為第二種治具 //Steven for HT7080
            USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                       //Ztex 2024.02.24 Add HT-1132
    {
        dMovePitchX=double(iMovePitchX)/3.0;
    }
    else                                                                        //齊平為第三種治具
    {
        dMovePitchX=double(iMovePitchX)/3.0;
    }

    ZeroMemory(bZFlgToCleanKit, sizeof(bZFlgToCleanKit));
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            iZPosToCleanKit[i][j]=ZSafePos;
        }
    }

    {
        if(IniConfig.bE43AutoCleanUseHotplate)
        {
            iOffsetPos=InOfsHP1;
        }
        else
        {
            iOffsetPos=InOfsAutoClean;
        }
    }

    if(iPick==bAutoPick)
    {
        if(iPickerOrder==1 || iPickerOrder==3)
            iSuckRow=1;
        else
            iSuckRow=0;

        iKitRow=iAutoCleanPickPlateY;                                           //Steven 20240701 : 修正Auto Clean Kit Row異常
        iKitCol=iAutoCleanPickPlateX;

        if(Special_2X6_Tray_XItem7() && (iShuttleRow==3 || iShuttleRow==4))     //Sam 20250712 : 新增特殊流程 2X6 AutoClean Tray XItem=7
        {
            iKitCol=iKitCol-iAutoCleanUseXPitch;
        }
        iSuckCol=GetAutoCleanPickStep(iAutoCleanStart);                         //Steven 20240918 : fixed for auto clean
        if(iSuckCol==-1)
            return true;
    }
    else if(iPick==bAutoPlace)
    {
        if(PlaceToCleanList->GetHPFirstTeamMotUse(bCleanZMotToPlace)==false)    //RogerYang 20250624 Add HotPlate ErrMessage
        {
            ShowMyMessage(AnsiString().sprintf("%s %s Task=%d", __FUNC__, "No GetHPFirstTeamMotUse", 0));
            return false;
        }
        PlaceToCleanList->GetHPFirstTeamSuckUse(bCleanZSuckToPlace);

        iSuckCol=-1;
        for(int j=0; j<InArmSuck.iMotCol; j++)                                  //確認第一排最左邊
        {
            if(iSuckCol==-1 &&
               bCleanZSuckToPlace[0][j]==true)
            {
                iSuckRow=0;
                iSuckCol=j;
            }
        }

        if(iSuckCol==-1)                                                        //如果確認第一排最左邊沒有了, 就找第二排
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                if(iSuckCol==-1 &&
                   bCleanZSuckToPlace[1][j]==true)
                {
                    iSuckRow=1;
                    iSuckCol=j;
                }
            }
        }

        if(iSuckCol==-1)
        {
            return true;
        }
        iKitRow=InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol];                   //Steven 20240701 : 修正Auto Clean Kit Row異常
        iKitCol=InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol];

        if(Special_2X6_Tray_XItem7())                                           //Sam 20250712 : 新增特殊流程 2X6 AutoClean Tray XItem=7
        {
            if(iKitCol>0)
                iKitCol=iKitCol-iAutoCleanUseXPitch*4;
        }
    }

    int iYVariable  =GetInArmPitchY_9045(iMovePitchY, iOffsetPos);
    for(int i=0; i<X_PITCH_COUNT; i++)
        iInXPToSht[i]=GetInArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    iXPos=Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]+
          dMovePitchX*(0+iInArmXBase-iSuckCol)+                                //A排使用, 用哪個治具, 就是使用哪個治具的距離
          TestIF_File.dAutoClean_XPitch*iKitCol+                                //使用的治具移動到要治具的位置
          HotplatlXOffset;                                                      //kevin 20150720 kit or TRAY autoclean X offset

    if(USE_IN_Y_IS_AUTO_PITCH==true)                                            //JerryYang 20251218 : IN/OUT ARM支援不同模式
    {
        if(iSuckRow==0)
            iYPos=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-iKitRow*TestIF_File.dAutoClean_YPitch-iMovePitchY;  //JerryYang 20240821 : fix auto clean點位錯誤
        else
            iYPos=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-iKitRow*TestIF_File.dAutoClean_YPitch;              //JerryYang 20240821 : fix auto clean點位錯誤
    }
    else
    {
        if(bUse8Picker==false)                                                  //Steven 20201014 : 齊平8治具auto clean
            iYPos=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-iKitRow*TestIF_File.dAutoClean_YPitch+iMovePitchY;
        else if(iSuckRow==1)
            iYPos=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-iKitRow*TestIF_File.dAutoClean_YPitch+iMovePitchY;
        else
            iYPos=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-iKitRow*TestIF_File.dAutoClean_YPitch;
    }

    iYPos+=HotplatlYOffset;                                                     //kevin 20150720 kit or TRAY autoclean Y offset

    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD && TestIF_File.bEnableAutoAlignment)      //KenHsieh 20211214 : AOA add AutoClean
        CheckInArmXYScaleByAutoTeach(iXPos, iYPos, AOA_AutoClean);
    iInXPos=iXPos;
    iInYPos=iYPos;

    if(InArmContinuousMove_9045(iXPos, iYPos, iInXPToSht, iYVariable, bZFlgToCleanKit, iZPosToCleanKit, false))
    {
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------
void SetShuttleIcForSpecialMode(eWhichShuttle iSht, int iType)                  //Steven 20221006 : 針對特殊模式可能Clean Pad放置
{
    if(iSht==euShuttle1)
        ptrInSHT=&FLCarryKit;
    else
        ptrInSHT=&BLCarryKit;

    if(iCloseSiteModeFor2x8==e2x8Run2x2_13 ||
       iCloseSiteModeFor2x8==e2x8Run2x2_14)
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(j!=0+iCloseSiteStep_2x8 &&
                   j!=2+iCloseSiteStep_2x8)
                {
                    ptrInSHT->SetItemData(i, j, iType);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x8==e2x8CloseEven ||
            iCloseSiteModeFor2x8==e2x8CloseEven1By1)
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(j==1 || j==3 || j==5 || j==7)
                {
                    ptrInSHT->SetItemData(i, j, iType);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x8==e2x8CloseOdd ||
            iCloseSiteModeFor2x8==e2x8CloseOdd1By1)
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(j==0 || j==2 || j==4 || j==6)
                {
                    ptrInSHT->SetItemData(i, j, iType);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x8==e2x8Run2x4Standard)                           //Steven 20240801 : fixed for auto clean.
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(j<iCloseSiteStep_2x8 ||
                   j>=iCloseSiteStep_2x8+4)
                {
                    ptrInSHT->SetItemData(i, j, iType);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x6==e2x6Run2x4)                                   //Steven 20240801 : fixed for auto clean.
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(j<iCloseSiteStep_2x6 ||
                   j>=iCloseSiteStep_2x6+4)
                {
                    ptrInSHT->SetItemData(i, j, iType);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x6==e2x6CloseCenter2x4)                           //Steven 20241025 : fixed for 2x6 close center site auto clean
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(Prod.bInSuckUse[iSht][i][j]==false)
                {
                    int iSiteCol=GetShuttleCol(i, j);
                    ptrInSHT->SetItemData(i, iSiteCol, iType);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x8>e2x8OneByOne ||
            iCloseSiteModeFor2x6>e2x6OneByOne)
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(Prod.bInSuckUse[iSht][i][j]==false)
                {
                    ptrInSHT->SetItemData(i, j, iType);
                }
            }
        }
    }
    else if(iType==HAS_NULL_CLEAN_IC)
    {
        if(fCleaning->b1x2SiteAbClosePutDummy)                                  //Steven 20180903 : 1x2 close Ab Auto Clean //Steven 20190509 : Fixed
        {
            if(bUse8Picker)
            {
                if(iInArmType==e9045_1x2_2_14)
                {
                    if(InArmSuck.Item[0][3]==NULL_IC)
                        InArmSuck.SetItemData(0, 3, iType);
                }
                else if(InArmSuck.Item[0][2]==NULL_IC)
                {
                    InArmSuck.SetItemData(0, 2, iType);
                }
            }
            else
            {
                if(iInArmType==e9045_1x2_2_14)
                {
                    if(InArmSuck.Item[1][3]==NULL_IC)
                        InArmSuck.SetItemData(1, 3, iType);
                }
                else if(InArmSuck.Item[1][2]==NULL_IC)
                {
                    InArmSuck.SetItemData(1, 2, iType);
                }
            }
        }
        else if(iInArmType==e9045_1x4_1_Ac)
        {
            ptrInSHT->SetItemData(0, 0, iType);
            ptrInSHT->SetItemData(0, 1, iType);
            ptrInSHT->SetItemData(0, 3, iType);
        }
    }
    else
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(ptrInSHT->Item[i][j]==HAS_NULL_CLEAN_IC &&
                   (ptrInSHT->iAutoCleanRecX[i][j]==-1 ||
                    ptrInSHT->iAutoCleanRecY[i][j]==-1))                        //Steven 20211221 : 清除因異常留Auto Clean紀錄
                {
                    ptrInSHT->SetItemData(i, j, NULL_IC);
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
bool RestoreCleanKitData()                                                      //ChungHung 20130628 add 回復已被InArm吸走的CleanKit上的IC計算  請勿刪除
{
    int iXpos, iYpos;
    int iCount=0;

    if(InArmSuck.HasType(HAS_CLEAN_IC) || InArmSuck.HasType(HAS_NULL_CLEAN_IC)) //KevinCheng 20251209 : 增加判斷避免InArm資料只有NULL_CLEAN_IC時不會被清除
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                iXpos=InArmSuck.iAutoCleanRecX[i][j];
                iYpos=InArmSuck.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        iCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                        iCount--;
                        SetAutoCleanStringGrid(iXpos, iYpos+1, AnsiString(iCount));
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, HAS_CLEAN_IC);
                    }
                }
            }
        }
    }

    if(InArmSuck.HasType(CLEAN_FINISH_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                iXpos=InArmSuck.iAutoCleanRecX[i][j];
                iYpos=InArmSuck.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, CLEAN_FINISH_IC);
                    }
                }
            }
        }
    }

    if(FLCarryKit.HasType(HAS_CLEAN_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=FLCarryKit.iAutoCleanRecX[i][j];
                iYpos=FLCarryKit.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        iCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                        iCount--;
                        SetAutoCleanStringGrid(iXpos, iYpos+1, AnsiString(iCount));
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, HAS_CLEAN_IC);
                    }
                }
            }
        }
    }

    if(FLCarryKit.HasType(CLEAN_FINISH_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=FLCarryKit.iAutoCleanRecX[i][j];
                iYpos=FLCarryKit.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, CLEAN_FINISH_IC);
                    }
                }
            }
        }
    }

    if(BLCarryKit.HasType(HAS_CLEAN_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=BLCarryKit.iAutoCleanRecX[i][j];
                iYpos=BLCarryKit.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        iCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                        iCount--;
                        SetAutoCleanStringGrid(iXpos, iYpos+1, AnsiString(iCount));
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, HAS_CLEAN_IC);
                    }
                }
            }
        }
    }

    if(BLCarryKit.HasType(CLEAN_FINISH_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=BLCarryKit.iAutoCleanRecX[i][j];
                iYpos=BLCarryKit.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, CLEAN_FINISH_IC);
                    }
                }
            }
        }
    }

    if(BTestSuck.HasType(HAS_CLEAN_IC))                                         //JerryYang 20160825 回復被index arm吸走的CleanKit上CleanKit上的IC計算
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=BTestSuck.iAutoCleanRecX[i][j];
                iYpos=BTestSuck.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        iCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                        iCount--;
                        SetAutoCleanStringGrid(iXpos, iYpos+1, AnsiString(iCount));
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, HAS_CLEAN_IC);
                    }
                }
            }
        }
    }

    if(FTestSuck.HasType(HAS_CLEAN_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=FTestSuck.iAutoCleanRecX[i][j];
                iYpos=FTestSuck.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        iCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                        iCount--;
                        SetAutoCleanStringGrid(iXpos, iYpos+1, AnsiString(iCount));
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, HAS_CLEAN_IC);
                    }
                }
            }
        }
    }

    if(BTestSuck.HasType(CLEAN_FINISH_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=BTestSuck.iAutoCleanRecX[i][j];
                iYpos=BTestSuck.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, CLEAN_FINISH_IC);
                    }
                }
            }
        }
    }

    if(FTestSuck.HasType(CLEAN_FINISH_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=FTestSuck.iAutoCleanRecX[i][j];
                iYpos=FTestSuck.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, CLEAN_FINISH_IC);
                    }
                }
            }
        }
    }
    ReadWriteAutoCleanCount(false);                                             //Steven 20180524 : Fixed for clean count
    return true;
}
//------------------------------------------------------------------------------
bool SearchCleanKitRowCol(int& iKRow,int& iKCol)
{
    for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)
    {
        for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)
        {
            if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_IC)
            {
                iKCol=X;
                iKRow=Y;
                return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void SearchCleanKitRowCol(eWhichShuttle iSht)
{
    if((TestIF.iShuttleMode==1 &&
        TestIF.iShuttle_Sel==1 &&
        TestIF.iAutoClean_SelectArm==1) ||                                      //kevin 20170126 (Steven) 選ARM 使用ARM 2
       (IniConfig.bA09_ByArmCloseSite==1 &&
        TestIF.iAutoClean_SelectArm==1) ||                                      //Isaac 20170601 (wei) add
       (CosFunction.bAutoCleanAutoSelIndexArm==true &&
        TestIF.bCleanIndexOtherArm==false &&
        TestIF_File.iShuttleMode==1 &&
        TestIF_File.iShuttle_Sel==1))                                           //JerryYang 20171017 (wei) 修正auto clean只開ARM2會發生hang up
    {
        iSht=euShuttle2;
    }

    bool bFlag[8];

    int iKitRow=0, iKitCol=0;
    int iRealRow=0;
    int iSuckCol=0, iShtRow=0, iShtCol=0;

    ZeroMemory(bFlag, sizeof(bFlag));
    DoInArm_SuckerMap();                                                        //jou 2013-11-06 開放site 治具使用異常修正

    iRealRow=0;
    iSuckCol=0;

    GetNowInShuttleRowCol_All_1Picker(iSht, &iShtRow, &iShtCol, bAutoPlace);

    if(Prod.bInSuckUse[iSht][iShtRow][iSuckCol]==true)
    {
        bFlag[iSuckCol]=(Prod.fInArmSuck4x8[iSht][iShtRow][iShtCol] &&          //Steven 20241008 : fixed [iSuckCol+iKitStep]
                         InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC);
    }

    if(InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC)
    {
        iAutoCleanStart=0;
    }

    int iKRow=0,iKCol=0;
    if(SearchCleanKitRowCol(iKRow,iKCol))
    {
        iAutoCleanPickPlateX=iKCol;
        iAutoCleanPickPlateY=iKRow;
        iSuckCol=0;                                                             //Steven 20240918 : fixed for auto clean
        iKitCol=iAutoCleanPickPlateX+iAutoCleanUseXPitch*(iSuckCol-iAutoCleanStart);
        iKitRow=iAutoCleanPickPlateY;

        if(MOT[MMAutoCleanKit].Tray.Data[iKitCol][iKitRow]==HAS_CLEAN_IC &&
           iKitCol<MOT[MMAutoCleanKit].Tray.XItem)
        {
            bInArmSuckActive[iRealRow][iSuckCol]=bFlag[iSuckCol];
        }
        else
        {
            bInArmSuckActive[iRealRow][iSuckCol]=false;
        }
        return;
    }
}
//------------------------------------------------------------------------------
void SearchCleanKitUpDown(int iRow, eWhichShuttle iSht)
{
    if(USE_PICKER_COUNT==ep1Picker)
    {
        SearchCleanKitRowCol(iSht);
        return;
    }

    if((TestIF.iShuttleMode==1 &&
        TestIF.iShuttle_Sel==1 &&
        TestIF.iAutoClean_SelectArm==1) ||                                      //kevin 20170126 (Steven) 選ARM 使用ARM 2
       (IniConfig.bA09_ByArmCloseSite==1 &&
        TestIF.iAutoClean_SelectArm==1) ||                                      //Isaac 20170601 (wei) add
       (CosFunction.bAutoCleanAutoSelIndexArm==true &&
        TestIF.bCleanIndexOtherArm==false &&
        TestIF_File.iShuttleMode==1 &&
        TestIF_File.iShuttle_Sel==1))                                           //JerryYang 20171017 (wei) 修正auto clean只開ARM2會發生hang up
    {
        iSht=euShuttle2;
    }

    if(iInArmType==e9045_2x6_8 &&
       iCloseSiteModeFor2x6<e2x6OneByOne &&                                     //Steven 20241113 : for 2x6 auto clean
       (TestIF_File.iAutoClean_DeveicePices==12 ||
        TestIF_File.iAutoClean_DeveicePices==24))
    {
        if(iShuttleRowKit==1 ||
           iShuttleRowKit==2)
        {
            if(bUse8Picker)
            {
                InArmSuck.SetItemData(0, 3, HAS_NULL_CLEAN_IC);
                InArmSuck.SetItemData(1, 3, HAS_NULL_CLEAN_IC);
            }
            else
            {
                InArmSuck.SetItemData(1, 3, HAS_NULL_CLEAN_IC);
            }
        }
        else if(iShuttleRowKit==3 ||
                iShuttleRowKit==4)
        {
            if(bUse8Picker)
            {
                InArmSuck.SetItemData(0, 0, HAS_NULL_CLEAN_IC);
                InArmSuck.SetItemData(1, 0, HAS_NULL_CLEAN_IC);
            }
            else
            {
                InArmSuck.SetItemData(1, 0, HAS_NULL_CLEAN_IC);
            }
        }
    }

    bool bFlag[8];
    bool bRowHasIC=false;
    int iKitRow=0, iKitCol=0;
    int iKitStep=0, iRealRow=1;
    int &iSuckRow=iPickerOrder, iSuckCol=0, iSiteRow=0, iSiteCol=0, iShtRow=0, iShtCol=0;

    ZeroMemory(bFlag, sizeof(bFlag));
    DoInArm_SuckerMap();                                                        //jou 2013-11-06 開放site 治具使用異常修正
    GetInarmSuckRow(iRow,iSuckRow,iKitStep);

    if(IniConfig.bE43AutoCleanUseHotplate ||                                    //ChungHung 20131120 AutoClean use Hotplate1
       bUse8Picker)                                                             //Steven 20201124 : fixed for 2x3 auto clean
    {
        iRealRow=iSuckRow;
    }
    else
    {
        iRealRow=1;
    }

    for(int j=0; j<InArmSuck.iMaxCol; j++)
    {
        iSuckCol=GetAutoCleanPickStep(j);                                       //Steven 20240918 : fixed for auto clean
        if(iSuckCol==-1)
        {
            continue;
        }

        iShtRow=iSuckRow;
        iShtCol=iSuckCol+iKitStep;                                              //Steven 20250917 : iSuckCol+iKitStep --> GetShuttleCol
        iSiteCol=GetShuttleCol(iShtRow, iShtCol);                               //Steven 20250917 : Fixed for 12site auto clean
        if(IsNNMode()==NN_2Row)
        {
            iSiteRow=(iSht==0)?2:0;
        }
        else if(IsNNMode()==NN_1Row)
        {
            iSiteRow=(iSht==0)?1:0;
        }
        else
        {
            iSiteRow=0;
        }

        if(i1x2_4UseACEGPicker==1)                                              //Steven 20230530 : 1x2_4齊平Row A
        {
            if(CosFunction.bUseAutoCleanCloseSiteAlsoDo==true)                  //Ifor 20181222 add 新增Auto Clean Close Site 一樣執行
            {
                bFlag[iSuckCol]=(TestIF.iSiteMap[iSiteRow][iSiteCol]>0 &&
                                 InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC);

                if(InArmSuck.Item[iRealRow][iSuckCol]!=NULL_IC)
                    bRowHasIC=true;
            }
            else
            {
                bFlag[iSuckCol]=(Prod.bInSuckUse[iSht][iShtRow][iShtCol] &&     //Steven 20241008 : fixed [iSuckCol+iKitStep]
                                 InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC);

                if(InArmSuck.Item[iRealRow][iSuckCol]!=NULL_IC)
                    bRowHasIC=true;
            }
        }
        else if(Prod.bInSuckUse[iSht][iShtRow][iShtCol]==true)
        {
            {
                if(CosFunction.bUseAutoCleanCloseSiteAlsoDo==true)                  //Ifor 20181222 add 新增Auto Clean Close Site 一樣執行
                {
                    bFlag[iSuckCol]=(TestIF.iSiteMap[iSiteRow][iSiteCol]>0 &&
                                     InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC);

                    if(InArmSuck.Item[iRealRow][iSuckCol]!=NULL_IC)
                        bRowHasIC=true;
                }
                else
                {
                    bFlag[iSuckCol]=(Prod.fInArmSuck4x8[iSht][iShtRow][iShtCol] &&  //Steven 20241008 : fixed [iSuckCol+iKitStep]
                                     InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC);

                    if(InArmSuck.Item[iRealRow][iSuckCol]!=NULL_IC)
                        bRowHasIC=true;
                }
            }
        }
    }

    for(int j=0; j<InArmSuck.iMaxCol; j++)
    {
        iSuckCol=GetAutoCleanPickStep(j);                                       //Steven 20240918 : fixed for auto clean
        if(iSuckCol==-1)
            continue;

        if((iInArmType==e9045_1x3_4 ||                                          //Steven 20241101 : Fixed for 1x3 auto clean
            iInArmType==e9045_1x3_2_14) &&
           TestIF_File.iAutoClean_DeveicePices%3==0)
        {
            if(iKitStep!=0 && iSuckCol==0)
                continue;
        }

        iShtRow =iSuckRow;
        iShtCol =iSuckCol+iKitStep;                                             //Steven 20250917 : iSuckCol+iKitStep --> GetShuttleCol
        iSiteCol=GetShuttleCol(iShtRow, iShtCol);                               //Steven 20250917 : Fixed for 12site auto clean

        if(bRowHasIC==true)                                                     //Steven 20240829 : 判斷是不是第二次治具
        {
            if((bFlag[iSuckCol] ||                                              //Steven 20240729 : fixed for auto clean pick 2 times from kit
                Prod.bInSuckUse[iSht][iShtRow][iShtCol]) &&                     //Steven 20241010 : iSuckCol --> iShtCol
               InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC)
            {
                iAutoCleanStart=j;
                break;
            }
        }
        else
        {
            if(InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC)
            {
                iAutoCleanStart=j;
                break;
            }
        }
    }
    int iKRow=0,iKCol=0;
    if(SearchCleanKitRowCol(iKRow,iKCol))
    {
        iAutoCleanPickPlateX=iKCol;
        iAutoCleanPickPlateY=iKRow;
        for(int i=0; i<InArmSuck.iMaxCol; i++)
        {
            iSuckCol=GetAutoCleanPickStep(i);                                   //Steven 20240918 : fixed for auto clean
            if(iSuckCol==-1)                                                    //JerryYang 20250326 : fixed
            {
                continue;                                                       //Steven 20260504 : fixed for auto clean
            }
            else if(i-iAutoCleanStart<0)
            {
                {
                    bInArmSuckActive[iRealRow][iSuckCol]=false;
                    continue;
                }
            }

            iKitCol=iAutoCleanPickPlateX+iAutoCleanUseXPitch*(i-iAutoCleanStart);
            iKitRow=iAutoCleanPickPlateY;

            if(MOT[MMAutoCleanKit].Tray.Data[iKitCol][iKitRow]==HAS_CLEAN_IC &&
               iKitCol<MOT[MMAutoCleanKit].Tray.XItem)
            {
                bInArmSuckActive[iRealRow][iSuckCol]=bFlag[iSuckCol];
            }
            else
            {
                bInArmSuckActive[iRealRow][iSuckCol]=false;
            }
        }
        return;
    }
}
//------------------------------------------------------------------------------
bool CleanPad_PlaceToShuttle(int iSht)
{
    int flag=true;
    int iShtRow=0, iShtCol=0;
    int iPickKit32=(iShuttleRowKit==1 || iShuttleRowKit==2)?0:4;

    if(USE_PICKER_COUNT==ep1Picker)
    {
        GetNowInShuttleRowCol_All_1Picker(iSht, &iShtRow, &iShtCol, bAutoPlace);

        if(InArmSuck.Item[0][0]==HAS_NULL_CLEAN_IC ||
           (InArmSuck.Suck[0][0].GetNeedDestroyStatus() &&
            (InArmSuck.Item[0][0] &&
             InArmSuck.Suck[0][0].Destroy())))
        {
            if(iSht==euShuttle1)
                FLCarryKit.MoveSuckDataDiff(InArmSuck, 0, 0, iShtRow, iShtCol);
            else
                BLCarryKit.MoveSuckDataDiff(InArmSuck, 0, 0, iShtRow, iShtCol);
        }
        else if(InArmSuck.Item[0][0] &&
                InArmSuck.Suck[0][0].GetNeedDestroyStatus())
        {
            flag=false;
        }
    }
    else if(bUse8Picker)
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(iCloseSiteModeFor2x8==e2x8_STMMode ||                        //KevinCheng 20251031 : STM與TW153模式共用 +iPickKit32 避免+4的座標算錯
                    iCloseSiteModeFor2x8==e2x8_TW153Mode)
                {
                    iShtRow=i;
                    iShtCol=GetShuttleCol(i, j);

                    if(InArmSuck.Suck[i][j].GetNeedDestroyStatus() &&           //KevinCheng 20251031 : 補判斷條件 避免視為HAS_NULL_CLEAN_IC影響GetShuttleState_2x8_8判斷 造成iShuttleRowKit流程錯誤
                        (InArmSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                        (InArmSuck.Item[i][j] &&
                        InArmSuck.Suck[i][j].Destroy())))
                    {
                        if(iShtCol<8)
                        {
                            if(iSht==euShuttle1)
                                FLCarryKit.MoveSuckDataDiff(InArmSuck, i, j, iShtRow, iShtCol);
                            else
                                BLCarryKit.MoveSuckDataDiff(InArmSuck, i, j, iShtRow, iShtCol);
                        }
                    }
                    else if(InArmSuck.Item[i][j] &&
                            InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                    {
                        flag=false;
                    }
                }
                else
                {
                    iShtRow=i;
                    iShtCol=GetShuttleCol(i, j+iPickKit32);

                    if(InArmSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                       (InArmSuck.Suck[i][j].GetNeedDestroyStatus() &&
                        (InArmSuck.Item[i][j] &&
                         InArmSuck.Suck[i][j].Destroy())))
                    {
                        if(iShtCol<8)
                        {
                            if(iSht==euShuttle1)
                                FLCarryKit.MoveSuckDataDiff(InArmSuck, i, j, iShtRow, iShtCol);
                            else
                                BLCarryKit.MoveSuckDataDiff(InArmSuck, i, j, iShtRow, iShtCol);
                        }
                    }
                    else if(InArmSuck.Item[i][j] &&
                            InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                    {
                        flag=false;
                    }
                }
            }
        }
    }
    else
    {
        iShtRow=(iShuttleRowKit==2 || iShuttleRowKit==4)?1:0;
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            iShtCol=GetShuttleCol(iShtRow, j+iPickKit32);

            if(iCloseSiteModeFor2x8==e2x8_STMMode ||                            //KevinCheng 20251031 : STM與TW153模式共用 +iPickKit32 避免+4的座標算錯
                iCloseSiteModeFor2x8==e2x8_TW153Mode)
            {
                if(InArmSuck.Suck[1][j].GetNeedDestroyStatus() &&               //KevinCheng 20251031 : 補判斷條件 避免視為HAS_NULL_CLEAN_IC影響GetShuttleState_2x8_8判斷 造成iShuttleRowKit流程錯誤
                    (InArmSuck.Item[1][j]==HAS_NULL_CLEAN_IC ||
                   (InArmSuck.Item[1][j] &&
                    InArmSuck.Suck[1][j].Destroy())))
                {
                    if(iShtCol<8)
                    {
                        if(iSht==euShuttle1)
                            FLCarryKit.MoveSuckDataDiff(InArmSuck, 1, j, iShtRow, iShtCol);
                        else
                            BLCarryKit.MoveSuckDataDiff(InArmSuck, 1, j, iShtRow, iShtCol);
                    }
                }
                else if(InArmSuck.Item[1][j] &&
                        InArmSuck.Suck[1][j].GetNeedDestroyStatus())
                {
                    flag=false;
                }
            }
            else
            {
                if(InArmSuck.Item[1][j]==HAS_NULL_CLEAN_IC ||
                   (InArmSuck.Item[1][j] &&
                    InArmSuck.Suck[1][j].GetNeedDestroyStatus() &&
                    InArmSuck.Suck[1][j].Destroy()))
                {
                    if(iShtCol<8)
                    {
                        if(iSht==euShuttle1)
                            FLCarryKit.MoveSuckDataDiff(InArmSuck, 1, j, iShtRow, iShtCol);
                        else
                            BLCarryKit.MoveSuckDataDiff(InArmSuck, 1, j, iShtRow, iShtCol);
                    }
                }
                else if(InArmSuck.Item[1][j] &&
                        InArmSuck.Suck[1][j].GetNeedDestroyStatus())
                {
                    flag=false;
                }
            }
        }
    }

    return flag;
}
//------------------------------------------------------------------------------
bool CleanPad_PickFromShuttle(int iSht, int iSelRow)
{
    bool flag=true;
    int iShtRow, iShtCol;
    int iPickKit32=(iSelRow==1 || iSelRow==2)?0:4;

    if(fContact->IsRun2DCheck()==true)                                          //JerryYang 20260408 : fix 2DID Mapping
    {
        iPickKit32=0;
    }

    if(USE_PICKER_COUNT==ep1Picker)
    {
        GetNowInShuttleRowCol_All_1Picker(iSht, &iShtRow, &iShtCol, bAutoPick);
        if(ptrInSHT->Item[iShtRow][iShtCol] &&
           InArmSuck.iNeedSuck[0][0]!=NULL_IC)
        {
            if(ptrInSHT->Item[iShtRow][iShtCol]==HAS_NULL_CLEAN_IC ||
               ptrInSHT->Item[iShtRow][iShtCol]==HAS_NULL_IC ||         //JerryYang 20250220 : 2DID預鎖順序檢查功能
               InArmSuck.Suck[0][0].Suck())
            {
                InArmSuck.MoveSuckDataDiff(*ptrInSHT, iShtRow, iShtCol, 0, 0);
                InArmSuck.PordRec[0][0].AddPickCleanPadFormShuttle(0, 0, iShtRow, iShtCol);  //Sam 20230616 : Add Auto Clean Record
                ptrInSHT->Item[iShtRow][iShtCol]=NULL_IC;
            }
            else if(InArmSuck.Item[0][0]==NULL_IC &&
                    InArmSuck.Suck[0][0].Error==false)
            {
                flag=false;
            }
        }
    }
    else if(bUse8Picker)                                                             //Steven 20201014 : 齊平8治具auto clean
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                iShtRow=i;
                iShtCol=GetShuttleCol(i, j+iPickKit32);

                if(iShtCol<8 &&
                   ptrInSHT->Item[iShtRow][iShtCol] &&
                   InArmSuck.iNeedSuck[i][j]!=NULL_IC)
                {
                    if(ptrInSHT->Item[iShtRow][iShtCol]==HAS_NULL_CLEAN_IC ||
                       ptrInSHT->Item[iShtRow][iShtCol]==HAS_NULL_IC ||         //JerryYang 20250220 : 2DID預鎖順序檢查功能
                       InArmSuck.Suck[i][j].Suck())
                    {
                        InArmSuck.MoveSuckDataDiff(*ptrInSHT, iShtRow, iShtCol, i, j);
                        InArmSuck.PordRec[i][j].AddPickCleanPadFormShuttle(i, j, iShtRow, iShtCol);  //Sam 20230616 : Add Auto Clean Record
                        ptrInSHT->Item[iShtRow][iShtCol]=NULL_IC;
                    }
                    else if(InArmSuck.Item[i][j]==NULL_IC &&
                            InArmSuck.Suck[i][j].Error==false)
                    {
                        flag=false;
                    }
                }
            }
        }
    }
    else
    {
        iShtRow=(iSelRow==2 || iSelRow==4)?1:0;
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            iShtCol=GetShuttleCol(iShtRow, j+iPickKit32);

            if(iShtCol<8 &&
               ptrInSHT->Item[iShtRow][iShtCol] &&
               InArmSuck.iNeedSuck[1][j]!=NULL_IC)
            {
                if(ptrInSHT->Item[iShtRow][iShtCol]==HAS_NULL_CLEAN_IC ||
                   InArmSuck.Suck[1][j].Suck())
                {
                    InArmSuck.MoveSuckDataDiff(*ptrInSHT, iShtRow, iShtCol, 1, j);
                    InArmSuck.PordRec[1][j].AddPickCleanPadFormShuttle(1, j, iShtRow, iShtCol);  //Sam 20230616 : Add Auto Clean Record
                }
                else if(InArmSuck.Item[1][j]==NULL_IC &&
                        InArmSuck.Suck[1][j].Error==false)
                {
                    flag=false;
                }
            }
        }
    }

    return flag;
}
//------------------------------------------------------------------------------
int CheckShuttleSensor_Clean(eWhichShuttle iSht, bool alarmflag)
{
    int ret=0;
    switch(TestIF.iTestMode)
    {
        case SingleSite:                                                        //1x1
            ret=CheckShuttleSensor_9045_1x1(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;                                                              //Ifor 20180313 (Steven) : add 避免 1x1 治具會亂跳
        case DualSite:                                                          //1x2
        case QualSite2X2N:                                                      //Steven 20201014 : for 2x2 nn mode auto clean
            ret=CheckShuttleSensor_9045_1x2(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        case QualSite1X4:                                                       //1x4
        case _8Site1X4:                                                         //ChungHung 20150528 add for 增加 _8Site1x4
        case _8Site2X4N:                                                        //Wei 20231211 : 2X4NN Mode
            ret=CheckShuttleSensor_9045_1x4(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        case QualSite2X2:                                                       //2x2
            ret=CheckShuttleSensor_9045_2x2(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        case TriSite1X3:                                                        //Frank 20160323 1x3 AutoClean Add
        case _6Site2X3N:                                                        //Steven 20220425 : 2X3NN Mode
        case _6Site2X3:                                                         //ChungHung 20150119 add for 2x3 mode autoclean
            ret=CheckShuttleSensor_9045_2x3(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        case _16Site4X4:                                                        //Sam 20190226 : 16Site4X4
        case _8Site2X4:                                                         //2x4
            ret=CheckShuttleSensor_9045_2x4(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        case _10Site2X5:                                                        //wei 20190614 10 site
            ret=CheckShuttleSensor_9045_2x5(iSht, alarmflag, true);
            break;
        case _12Site2X6:
            ret=CheckShuttleSensor_9045_2x6(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        case _16Site2X8:                                                        //2x8
        case _32Site4X8N:                                                       //Steven 20140512: For HT-9047
            ret=CheckShuttleSensor_9045_2x8(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        default:
            ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "CheckShuttleSensor_Clean");
    }
    return ret;
}
//==============================================================================
bool CheckAutoCleanCloseSite(int iSht)                                          //Steven 20220929 : 判斷開放Site
{
    bool bResult=false;
    if(bUseTwoArm32Site==true)
    {
        bResult=(InArmSideAllClose(iSht));
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool DoInArmPineRelease()
{
    static bool bHaveServoOn=false;
    static bool bXMoveFlag=false;
    static bool bYMoveFlag=false;
    static bool bNeedDelay=false;

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20110802
    {
        if(fNote->bMyServoOffInArm)
        {
            if(bHaveServoOn==false)
            {
                MOT[MInArmX].ServoOnOff(true);
                MOT[MInArmY].ServoOnOff(true);
                tCleanInArmServoOnDelay.SetMSAndOn(500);
                bHaveServoOn=true;
                bXMoveFlag=false;
                bYMoveFlag=false;
            }

            if(InArmZSafe(DETECT_ALL_FLAG)==-1)                                 //如果Z軸在安全內可以走
            {
                if(tCleanInArmServoOnDelay.Off())
                {
                    if(bXMoveFlag==false)
                    {
                        bXMoveFlag=MOT[MInArmX].MotorMove(fNote->iMyServoOffInArmPosX);
                    }

                    if(bYMoveFlag==false)
                    {
                        bYMoveFlag=MOT[MInArmY].MotorMove(fNote->iMyServoOffInArmPosY);
                    }

                    if(bXMoveFlag==true && bYMoveFlag==true)
                    {
                        tCleanInArmServoOnDelay.SetMSAndOn(500);
                        fNote->bMyServoOffInArm=false;
                        bHaveServoOn=false;
                        bNeedDelay=true;
                        return true;
                    }
                }
            }
            else
            {
                fNote->bMyServoOffInArm=false;
                fAllMotorHome=false;

                if(IniConfig.bSPILFunction==true ||                             //JerryYang 20170328 (Jou) 揭示客戶會用一起SPILFunction
                   CUSTOMER_CODE==CC_SIGURD_PeiXing)                            //JerryYang 20160328 for 瑞士德_北京,觸發回home的地方要加上log
                    ShowMyMessage("In Arm PICK UP ALARM,Need home");            //Ifor 20151208 :客戶新增回饋才顯示 Show Alarm Message
                StopAllMotor();
                bNeedDelay=false;
            }
        }
        else
        {
            bHaveServoOn=false;
            bNeedDelay=false;
            return true;
        }
    }
    else
    {
        return true;
    }

    if(bNeedDelay==true &&                                                      //Steven 20110804 End: Servo Off後要回復的
       tCleanInArmServoOnDelay.Off()==false)                                    //Steven 20110809 : ServoOn後要Delay一下
    {
        bNeedDelay=false;
    }
    return false;
}
//------------------------------------------------------------------------------
bool DoInArmMoveToWaitPosByAutoClean()                                          //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
{
    if(CosFunction.bIndexJamInArmMoveSafePostionByAutoClaen)
    {
        if(bPlaceToShuttleByAutoClean || bPickFromShuttleByAutoClean)
        {
            bLockPlaceToShuttleByAutoClean=false;
            bLockPickFromShuttleByAutoClean=false;
            return false;
        }
        else
        {
            bLockPlaceToShuttleByAutoClean=true;
            bLockPickFromShuttleByAutoClean=true;

            if(bPlaceToCleanKit)                                                //Steven 20171204 (Wei) : 避免正在放料還沒回到原本位置放
                return false;

            if(bPickFromKitByAutoClean==true)                                   //Steven 20210603 : 新增治具Clean Kit的Flag
                return false;

            if(MoveInArm2XYToWait())                                            //如果要回到,而且還沒到達安全位
            {
                bLockPlaceToShuttleByAutoClean=false;
                bLockPickFromShuttleByAutoClean=false;
                return true;                                                    //可以開放In Arm
            }
        }
    }
    else
    {
        bLockPlaceToShuttleByAutoClean=false;
        bLockPickFromShuttleByAutoClean=false;
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void RecDebug(TMyKitSuck &tray, int iTask)
{
    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)
        {
            MNetLog(AnsiString().sprintf("Task=%d Row=%d Col=%d Item=%d", iTask, i, j, tray.Item[i][j]));
        }
    }
}
//------------------------------------------------------------------------------
bool DoSocketSensorAlarm(AnsiString sFunc,int Task)
{
    if(DoInArmMoveToWaitPosByAutoClean()==true)                                 //JerryYang 20241118 : fix auto clean hang up   //Steven 20130613 : Index異常後, In Arm要有先讓功能
    {
        RecordProcess(AnsiString().sprintf("%s__%d: iShowSocketSensor=%d",sFunc,Task, iShowSocketSensor));

        if(TestIF_File.bEnSocketSensor && iShowSocketSensor>0)                  //kevin 20130504 socket sensor
        {
            if(iShowSocketSensor==2)                                            //JerryYang 20161024 Socket sensor改成能歸位置零
            {
                ShowErrorMessage("WAR0323", K_RETRY, MTestZ1, false, sSocketSensorErr);  //Socket detect device floting error
            }
            else if(iShowSocketSensor==1)
            {
                ShowErrorMessage("WAR0322", K_RETRY, MTestZ1, false, sSocketSensorErr);
            }
            iShowSocketSensor=0;
        }
    }
    return true;
}

//==============================================================================
//  Part D -- TfCleaning free-function translations (golden AutoClean/uCleaning.cpp)
//==============================================================================

//------------------------------------------------------------------------------
// AI(W906-AutoCleanFoundation) 20260721: golden TfCleaning::SetCleanCellValue
// (uCleaning.cpp:2045) -- NOT in the task brief's named Part D pair, but
// SetDeviceInTray (below) calls it directly, so it is translated as a
// required file-local dependency (same treatment as SetAutoCleanStringGrid
// above). iMode==eAutoCleanUsed routes to the real MOT[MMAutoCleanKit] HAL;
// iMode==eUcleanUsed (0) routes to fMain->tmyAutoClean->SetCellColorIndex, a
// write-only cosmetic grid-paint sink (see FormsFacade.h's TfMainAutoCleanGrid
// banner) -- not exposed in AutoClean.h (golden TfCleaning-private helper).
//------------------------------------------------------------------------------
static void SetCleanCellValue(int iXPos, int iYPos, int iType, int iMode)  //Jimmychiu 20221027 統一Cleanpad配置方式  iMode:0=SetCellColorIndex    1=SetTraySingleData
{
    if(iMode==eAutoCleanUsed)                                                   //1=SetTraySingleData
    {
        MOT[MMAutoCleanKit].SetTraySingleData(iXPos, iYPos, iType);
    }
    else                                                                        //0=SetCellColorIndex
    {
        fMain->tmyAutoClean->SetCellColorIndex(iXPos, iYPos, iType);
    }
}
//------------------------------------------------------------------------------
void SetDeviceInTray(int iXItem, int iYItem, int iDeviceNum, int iMode)  //Jimmychiu 20221027 統一Cleanpad配置方式
{
    int iHadSetDevNum=0;
    for(int iYPos=0; iYPos<iYItem; iYPos++)
    {
        for(int iXPos=0; iXPos<iXItem; iXPos++)
        {
            if(iHadSetDevNum>=iDeviceNum)
            {
                return;
            }

            if(TestIF_File.iTestMode==TriSite1X3 ||                             //Steven 20240112 : fixed for 1x3_14
               TestIF_File.iTestMode==_6Site2X3N)
            {
                if(TestIF_File.iTestMode==TriSite1X3 &&                         //JerryYang 20260408 : fix 1x3 2治具
                   iInArmType==e9045_1x3_2_14 &&
                   TestIF_File.iSiteMap[0][1]<=0)
                {
                }
                else
                {
                    if(iXItem==4 && iXPos==0)
                        continue;
                }
            }

            // AI(W906-AutoCleanFoundation) 20260721: golden's own operator
            // precedence VERBATIM -- parses as (iTestMode==_12Site2X6 &&
            // iDeviceNum==12) || iDeviceNum==24, i.e. iDeviceNum==24 alone
            // (any iTestMode) also qualifies. Compiler flags this shape
            // (-Wparentheses); preserved unchanged per this wave's
            // faithful-translation mandate (don't silently fix golden quirks).
            if(TestIF_File.iTestMode==_12Site2X6 &&
               iDeviceNum==12 || iDeviceNum==24)                                //Steven 20241111 : for 1x4 run 3 col tray auto clean
            {
                if(TestIF_File.iAutoClean_XDivision==4)
                {
                    if(iXPos==0)
                        continue;
                }
                else if(TestIF_File.iAutoClean_XDivision==8)
                {
                    if(iXPos==0 || iXPos==1)
                        continue;
                }
            }

            if(TestIF_File.iTestMode==QualSite1X4 &&
               TestIF_File.iAutoClean_XDivision==3)                             //Steven 20241111 : for 1x4 run 3 col tray auto clean
            {
                if(iXPos==0)
                    continue;
            }

            if(TestIF_File.iTestMode==DualSite ||                               //Steven 20190509 : 修正位置
               TestIF_File.iTestMode==QualSite2X2N)                             //Steven 20201014 : for 2x2 nn mode auto clean
            {
                if(USE_PICKER_COUNT==0)
                {
                    if(iXPos>=2)
                    {
                        SetCleanCellValue((iXPos*2)-3, iYPos, HAS_CLEAN_IC, iMode);
                        iHadSetDevNum++;
                    }
                    else
                    {
                        SetCleanCellValue(iXPos*2, iYPos, HAS_CLEAN_IC, iMode);
                        iHadSetDevNum++;
                    }
                }
                else if(iDeviceNum==2 && iXItem==8)     //JerryYang 20260527 : fix dual site auto clean卡料問題
                {
                    SetCleanCellValue(iXPos*4, iYPos, HAS_CLEAN_IC, iMode);
                    iHadSetDevNum++;
                }
                else
                {
                    SetCleanCellValue(iXPos, iYPos, HAS_CLEAN_IC, iMode);
                    iHadSetDevNum++;
                }
            }
            else
            {
                if(TestIF_File.iTestMode==QualSite2X2 && iAutoCleanUseXPitch==2)
                {
                    SetCleanCellValue(iXPos, iYPos, HAS_CLEAN_IC, iMode);
                    iHadSetDevNum++;
                }
                else if((TestIF_File.iTestMode==_16Site2X8 ||
                         TestIF_File.iTestMode==_32Site4X8N) &&                 //Steven 20250912 : for 16site auto clean with 12x16 Hot plate
                         iXItem==12 && iYItem==4)
                {
                    if(iXPos<8)
                        continue;

                    SetCleanCellValue(iXPos, iYPos, HAS_CLEAN_IC, iMode);
                    iHadSetDevNum++;
                }
                else if(TestIF_File.iAutoClean_XDivision>4 &&
                        ((bCleanKitPitchLess4000 && TestIF_File.iAutoClean_DeveicePices<=4) ||   //Alick 20161107 add 修正CLEAN TRAY XPITCH<13.33 & CleanIC<=4,AUTOCLEAN放料CLEAN IC放置位置錯誤
                         (iAutoCleanUseXPitch==2 && TestIF_File.iAutoClean_DeveicePices<4)))     //Alick 20170322 (wei) add 1*4時，Tray X>4，iAutoCleanUseXPitch=2，修正使用4顆CLEANPAD時取料會變成兩次 //Sam 20180208 "<=4" => "<4"
                {
                    SetCleanCellValue(iXPos*2, iYPos, HAS_CLEAN_IC, iMode);
                    iHadSetDevNum++;
                }
                else
                {
                    SetCleanCellValue(iXPos, iYPos, HAS_CLEAN_IC, iMode);
                    iHadSetDevNum++;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
bool CleanPadCountCanSupport2Arm()                                              //Steven 20221006 : 修正雙arm auto clean只有一半的clean pad
{
    bool bSupport2Arm=true;

    if(TestIF_File.iTestMode==SingleSite ||
       iInArmType==e9045_1x4_1_Ac ||
       fCleaning->b1x2SiteAbClosePutDummy==true)
    {
        if(TestIF_File.iAutoClean_DeveicePices==1)
            bSupport2Arm=false;
    }
    else if(TestIF_File.iTestMode==DualSite     ||
            TestIF_File.iTestMode==DualSite2x1  ||
            TestIF_File.iTestMode==QualSite2X2N)
    {
        if(TestIF_File.iAutoClean_DeveicePices<=2)
            bSupport2Arm=false;
    }
    else if(TestIF_File.iTestMode==TriSite1X3 ||
            TestIF_File.iTestMode==_6Site2X3N)
    {
        if(TestIF_File.iAutoClean_DeveicePices<=3)
            bSupport2Arm=false;
    }
    else if(TestIF_File.iTestMode==QualSite1X4 ||
            TestIF_File.iTestMode==QualSite2X2 ||
            TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
    {
        if(TestIF_File.iAutoClean_DeveicePices<=4)
            bSupport2Arm=false;
    }
    else if(TestIF_File.iTestMode==_6Site2X3 ||
            TestIF_File.iTestMode==_8Site2X4 ||
            TestIF_File.iTestMode==_16Site4X4 ||
            iCloseSiteModeFor2x6>e2x6OneByOne ||
            iCloseSiteModeFor2x8>e2x8OneByOne)
    {
        if(TestIF_File.iAutoClean_DeveicePices<=8)
            bSupport2Arm=false;
    }
    else if(TestIF_File.iTestMode==_12Site2X6)                                  //Steven 20241113 : for 2x6 auto clean
    {
        if(TestIF_File.iAutoClean_DeveicePices<=12)
            bSupport2Arm=false;
    }
    else if(TestIF_File.iTestMode==_10Site2X5  ||
            TestIF_File.iTestMode==_16Site2X8  ||
            TestIF_File.iTestMode==_32Site4X8N)
    {
        if(TestIF_File.iAutoClean_DeveicePices<=16)
            bSupport2Arm=false;
    }
    else
    {
        ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "CleanPadCountCanSupport2Arm");
    }
    return bSupport2Arm;
}
