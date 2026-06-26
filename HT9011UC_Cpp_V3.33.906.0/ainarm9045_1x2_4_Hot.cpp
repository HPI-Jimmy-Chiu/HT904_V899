// =============================================================================
//  ainarm9045_1x2_4_Hot.cpp  --  In-Arm per-site VARIANT (1x2_4_Hot)
//
//  Faithful translation of golden ainarm9045_1x2_4_Hot.cpp (3189 lines, BCB6, Big5).
//  Translator: AI(W6.2c-1x2_4_Hot) 20260626
//  Translation wave: W6.2c (in-arm per-site-config variant: 1x2_4_Hot; the
//  heavily-used 1x2 4-picker Hot-plate layout, with the i1x2_4UseACEGPicker
//  Row-A / dual-row split).  Sibling of the engine ainarm9045.cpp and the other
//  per-site leaves (1x1_1 / 1x3_4 / 2x4_16 / ...).
//
//  ROLE: the per-site in-arm pick/place state machines for the 1x2_4_Hot layout.
//  The engine's dispatch ladder DoInArm_9045() / DoInArm_9045_SuckerMap() routes
//  iInArmType==e9045_1x2_4_Hot here (un-gated at the integrate stage).  This file
//  also DEFINES the file-scope lookup table XPHSuckToSht_1x2_4[2][2][8] which the
//  engine reads at ainarm9045.cpp:513 via the i1x2_4UseACEGPicker index, so this
//  TU MUST be registered in CMake for that engine extern to resolve.  Owns:
//    XPHSuckToSht_1x2_4, SetInOutArmParameter_1x2_4_Hot, GetNowSiteKitMode_1x2_4_Hot,
//    InArmZNeedDown_1x2_4_Hot, CheckXYPitch_1x2_4_Hot,
//    MoveInArm2XYToShuttle_9045_1x2_4_Hot, DoPlaceToHotPlate_9045_1x2_4_Hot,
//    DoInArmPlaceToShuttle_9045_1x2_4_Hot, DoInArm_9045_1x2_4_Hot_SuckerMap,
//    DoInArm_9045_1x2_4_Hot, DoInArmTryPickFromHotPlate_9045_1x2_4_Hot.
//
//  FAITHFUL: every fn/var/cursor value, switch(Task) structure, formula,
//  fall-through and goto/label (IN_ARM_SHUTTLE_1x2_4_Hot /
//  IN_ARM_TRY_PICK_HOTPLATE_1X2_4_Hot) is transcribed VERBATIM from the golden
//  CP950.  All SM case bodies are ACTIVE -- they touch MOT[]/Suck/Sensor/
//  predicates/FormsFacade, all of which resolve over the Sim HAL +
//  aHotPlateSubstrate + atester_shims + canary_support + ainarm9045 engine.
//  No body is #if 0-gated.
//
//  VCL/Borland conversions: AnsiString (1-based, ::sprintf) via vclcompat;
//  __fastcall/__property/__published/PACKAGE NOT present in this file; __FUNC__
//  via the aArmHeader shim.  SOFT_SIMULTE is NOT defined -> the single #ifdef
//  SOFT_SIMULTE block in DoInArmTryPickFromHotPlate_9045_1x2_4_Hot keeps its
//  golden #else (real) path active, reproduced verbatim.  Numeric semantics
//  (int vs double, integer division, double pitch math) kept EXACT.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "ainarm9045_1x2_4_Hot.h"
//   #include "aArmHeader.h" (god-header; W6.0 SHIM) / "MyKitSuck.h" / "cprod.h"
//   #include "mymotor.h" / "mycylin.h" / "mysensor.h" / "cSortCT.h" / "note.h"
//   #include "main.h" / "csystem.h" / "uhome.h" / "mymessbox.h" / "cinitial.h"
//   #include "atester.h" / "aRotateKIT.h" / "fRotate.h" / "cOffSet.h" / "cMyDB.h"
//   #include "cObserver.h" / "cmydef.h" / "uYieldMonitoring.h" / "BarCode.h"
//   #include "InOutArmZteach.h" / "acarry.h" ; #pragma package(smart_init)
// =============================================================================
#include "MachineDefine.h"

#include "ainarm9045_1x2_4_Hot.h"
#include "aArmHeader.h"             // __FUNC__ shim, RecordProcess, etc.
#include "ainarm9045.h"             // engine: GetInArmToShuttleOffset_9045 / InArmNeedCheckOffset / MoveInArmXYToShuttle_9045 / GetShuttleCol / InArmContinuousMove_9045 / ...
#include "ainarm_SearchPickPlate.h" // shared HP-pick externs (bInArmTryPickFromHotPlateFinish / HasHotReadyIC_9045 / iTryPickHP / ...)
#include "ainarm_SearchPlacePlate.h"// shared HP-place externs (SearchPlateToPlace / MoveInArmXYToHotPlatePlace / DoPlaceToHPSwapData / GetPlaceToHotPlateSuckCol / Row2CanPutHP / GetHotPlateColStep / iPlaceHP / ...)
#include "csystem.h"                // CheckInArmSuckICFallDownToHasNullIC / DoInArmAutoSiteMapping / CheckInArmDestroyICFail predicates
#include "Motor/mymotor.h"          // MOT[], InArmContinuousMove_9045
#include "cmydef.h"                 // global scalar universe + IC consts + enums (eInArmType / picker enum / Tempture_* / rsm* / CC_* / i1x2_4UseACEGPicker)
#include "common.h"
#include "mycylin.h"
#include "myswitch.h"               // SW[] (SwShuttleVibration1/2 .On())
#include "cprod.h"                  // Prod / TestIF / TestIF_File / ArmSpeed / ArmSpeed_File / LastSet
#include "cpublic.h"                // CosFunction
#include "aHotPlateSubstrate.h"     // InArmSuck/FLCarryKit/BLCarryKit/OutArmSuck/FTestSuck/BTestSuck + ainarm2 cursors/timers + engine shims + PickFromHPList + ArmUpSideHaveRealIC/ArmDownSideHaveRealIC/ArmLeftSideHaveRealIC/ArmRightSideHaveRealIC
#include "FormsFacade.h"            // HotPlateForm / Zteach / TrayForm / fContact / fObserver / fYieldMonitoring offline stand-ins
#include "atester_shims.h"          // fObserver (RecordInArmTime/AddTimeData) / fContact (IsRun2DCheck)
#include "acatchtray_shims.h"        // InitProcessSingleMotorTask (offline single-motor home)
#include "canary_support.h"         // ShowErrorMessage / ShowMyMessage / RecordProcess / LastSet / __FUNC__
//==============================================================================

const int XPHSuckToSht_1x2_4[2][2][8]={{{0, 2, 1, 3, 4, 5, 6, 7},
                                        {0, 2, 1, 3, 4, 5, 6, 7}},
                                       {{0, 0, 1, 1, 4, 5, 6, 7},
                                        {4, 4, 4, 4, 4, 5, 6, 7}}};

//==============================================================================
bool DoInArmTryPickFromHotPlate_9045_1x2_4_Hot(bool bOneTimeCheckAll, bool bShowErrorMessage);                          //RogerYang 20170120 add   //ChungHung 20120206 Hotplate check
//==============================================================================
void SetInOutArmParameter_1x2_4_Hot()
{
    if(USE_PICKER_COUNT==ep16Picker)
    {
//        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 0, 0, 0);
//        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 2, 0, 1);
//        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 4, 0, 2);
//        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 6, 0, 3);
//        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 1, 0, 4);
//        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 3, 0, 5);
//        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 5, 0, 6);
//        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 7, 0, 7);

        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 0, 0, 0);
        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 2, 0, 1);
        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 4, 0, 2);
        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 6, 0, 3);
        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 1, 0, 4);
        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 3, 0, 5);
        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 5, 0, 6);
        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 7, 0, 7);
    }
    else
    {
//        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 2, 0, 1);
//        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 1, 2, 1, 1);
        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 2, 0, 1);
        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 1, 2, 1, 1);

//        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 1, 0, 2);
//        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 1, 1, 1, 2);
        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 1, 0, 2);
        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 1, 1, 1, 2);
    }

    InArmSuck.SetPickerCount(2, 2, 1, 2, 2, 0, 0);
    OutArmSuck.SetPickerCount(2, 2, 1, 2, 2, 0, 0);
}
//==============================================================================
int GetNowSiteKitMode_1x2_4_Hot(int iSht, bool bPlace)
{
//    int iX=HAS_TESTING_IC;
//    if(Zteach->fShow)                                                           //kevin 20210416 add Autoteach Z offset shuttle pick
//        iX=NULL_IC;
//    else if(bRunAutoClean)                                                      //kevin 20220927  IC >3500 //kevin 20220922//kevin 20121430
//        iX=CLEAN_FINISH_IC;
    if(i1x2_4UseACEGPicker==1)                                                  //Steven 20230530 : 1x2_4改用Row A
    {
        if(TestIF.iAutoClean_Function && bUse8Picker==false && bRunAutoClean)   //Jimmychiu 20250105 : add 1x2_4 in HT9045
        {
            if(InArmSuck.Item[1][0]==HAS_CLEAN_IC ||
               InArmSuck.Item[1][2]==HAS_CLEAN_IC)
            {
                return 11;
            }
            else                                                                //for HT9045 auto clean
                return 12;                                                      //Suck[1][1] and Suck[1][3] Positon in Shuttle2 [0][0] and [0][1]
        }
        else
        {
            if((InArmSuck.Item[0][0]==HAS_IC || InArmSuck.Item[0][0]==HAS_HOT_IC) ||
               (InArmSuck.Item[0][2]==HAS_IC || InArmSuck.Item[0][2]==HAS_HOT_IC))
                return 0;                                                       //Suck[0][0] and Suck[0][2] Positon in Shuttle1 [0][0] and [0][1]
            else if(InArmSuck.Item[1][0]==HAS_CLEAN_IC ||
                    InArmSuck.Item[1][2]==HAS_CLEAN_IC)
            {
                return 11;
            }
            else                                                                //for HT9045 auto clean
                return 3;                                                       //Suck[0][1] and Suck[0][3] Positon in Shuttle2 [0][0] and [0][1]
        }
    }
    else if(XPitchIsStand())
    {
        if((InArmSuck.Item[0][0]==HAS_IC || InArmSuck.Item[0][0]==HAS_HOT_IC) ||
           (InArmSuck.Item[0][2]==HAS_IC || InArmSuck.Item[0][2]==HAS_HOT_IC))
            return 0;                                                           //Suck[0][0] and Suck[0][2] Positon in Shuttle1 [0][0] and [0][1]
        else
            return 9;                                                           //Suck[1][0] and Suck[1][2] Positon in Shuttle2 [0][0] and [0][1]
    }
    else                                                                        // if(YPitchIsStand() && !XPitchIsStand())
    {
        if((InArmSuck.Item[0][0]==HAS_IC || InArmSuck.Item[0][0]==HAS_HOT_IC) ||
           (InArmSuck.Item[0][2]==HAS_IC || InArmSuck.Item[0][2]==HAS_HOT_IC) ||
           (InArmSuck.Item[1][0]==HAS_IC || InArmSuck.Item[1][0]==HAS_HOT_IC) ||
           (InArmSuck.Item[1][2]==HAS_IC || InArmSuck.Item[1][2]==HAS_HOT_IC))
            return 1;                                                           //Suck[0][0] and Suck[0][2] Positon in Shuttle[0][0] Shuttle[0][1]
        else
            return 2;                                                           //Suck[0][1] and Suck[0][3] Positon in Shuttle[0][2] Shuttle[0][3]
    }
}
//==============================================================================
void InArmZNeedDown_1x2_4_Hot(int iSht, bool bPlace)
{
    if(InitialInArmNeedSuck(iSht, bPlace)==false)
        return;
    int iSuckRow=0, iSuckCol=0, iShtRow=0, iShtCol=0;
    int iMode   =GetNowSiteKitMode_1x2_4_Hot(iSht, bPlace);
//    int iModeRow=iMode%100;
//    int iModeCol=iMode/100;

    for(int i=0; i<1; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            iSuckRow=(TestIF.iAutoClean_Function && bUse8Picker==false && bRunAutoClean)?1:i;
            iSuckCol=j;
            iShtCol =GetShuttleCol(i, j);
            iShtRow=i;
            if(iMode==0 && iShtRow==0 && (j==0 || j==2))
            {
                SetInArmNeedDestory(bPlace, iShtRow, iShtCol, iSuckRow, iSuckCol);
            }
            else if(iMode==1 && (j==0|| j==2))
            {
                SetInArmNeedDestory(bPlace, iShtRow, iShtCol, iSuckRow, iSuckCol);
            }
            else if(iMode==2 && (j==1 || j==3))
            {
                SetInArmNeedDestory(bPlace, iShtRow, iShtCol, iSuckRow, iSuckCol);
            }
            else if(iMode==3 && iShtRow==0 && (j==1 || j==3))                   //Steven 20230530 : 1x2_4改用Row A
            {
                SetInArmNeedDestory(bPlace, iShtRow, iShtCol, iSuckRow, iSuckCol);
            }
            else if(iMode==9 && iShtRow==1 && (j==0 || j==2))
            {
                SetInArmNeedDestory(bPlace, iShtRow, iShtCol, iSuckRow, iSuckCol);
            }
            else if(iMode==11 && iShtRow==0 && (j==0 || j==2))                  //Steven 20230530 : 1x2_4改用Row A  for HT9045 auto clean
            {
                SetInArmNeedDestory(bPlace, iShtRow, iShtCol, iSuckRow, iSuckCol);
            }
            else if(iMode==12 && iShtRow==0 && (j==0 || j==2))                  //Steven 20230530 : 1x2_4改用Row A  for HT9045 auto clean
            {
                SetInArmNeedDestory(bPlace, iShtRow, iShtCol, iSuckRow, iSuckCol);
            }
        }
    }
}
//==============================================================================
void CheckXYPitch_1x2_4_Hot(int *iX, int *iY, int iSht, bool bPlace, int iMovePitchX, int iMovePitchY)
{
    int iMode       =GetNowSiteKitMode_1x2_4_Hot(iSht, bPlace);
    int iModeRow    =iMode%100;
    int iModeCol    =iMode/100;
    int iOffsetPos  =GetInArmToShuttleOffset_9045(iSht, iModeRow, iModeCol, false);
//    double dMovePitchX;

    if(iSht==0)
    {
        *iX+=Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
        if(InArmSuck.iXStep==1)                                                 //RogerYang 20250811 Fix AE吸嘴shuttle1放歪
            *iX+=iMovePitchX/3;
        *iY+=Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
    }
    else
    {
        *iX+=Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
        *iY+=Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
    }

    if(iModeRow==1)                                                             //Ba to Row A
    {
        if(USE_IN_Y_IS_AUTO_PITCH==true)                                        //JerryYang 20251218 : IN/OUT ARM支援不同模組
            ;
        else
            *iY=*iY+iMovePitchY;
    }
    else if(iModeRow==0)                                                        //Aa to Row A
    {
        if(USE_IN_Y_IS_AUTO_PITCH==true)                                        //JerryYang 20251218 : IN/OUT ARM支援不同模組
            *iY=*iY-iMovePitchY;
    }

    if(TestIF_File.bQualSite2X2Shift)                                           //kevin 20170512 (wei) add 使用4 site layoutkit 使用1 3 Site shuttle 1X2 位置偏左2000
        *iX=*iX+TestIF_File.dShiftXPitch;

 /*   if(iMode==0 || iMode==9)
    {
        if(iSht==0)
        {
            *iX+=Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
            *iY+=Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
        }
        else
        {
            *iX+=Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
            *iY+=Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
        }
        *iX=*iX+iMovePitchX*2-TestIF.dSiteXPitch*0.5;
        if(i1x2_4UseACEGPicker==1)                                              //Steven 20230530 : 1x2_4改用Row A
        {
            if(USE_Y_AUTO_PITCH==true)                                          //ChungHung 20140304 add for AutoYPitch
                *iY+=iMovePitchY;
        }
        else
        {
            if(USE_Y_AUTO_PITCH==true)                                          //ChungHung 20140304 add for AutoYPitch
            {
                if(Other==false)
                    *iY-=iMovePitchY;                                           //Ifor 20191105 : Fix 1*2 4吸嘴 Shuttle 放錯位置
            }
            else
            {
                *iY+=iMovePitchY/2;
                if(Other==true)
                    *iY+=iMovePitchY;
            }
        }
        TransferInShuttleRatio(iShuttle, iX, iY, iInArmYBase, iInArmXBase);     //Steven 20110324
    }
    else if(iMode==1)
    {
        if(iSht==0)
        {
            *iX+=Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
            *iY+=Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
        }
        else
        {
            *iX+=Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
            *iY+=Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
        }
//        *iX-=TestIF.dSiteXPitch/2;
//        *iY+=TestIF.dSiteYPitch/2;
        //jou 980312
        *iX+=TestIF.dSiteXPitch/2;
//        *iY+=TestIF.dSiteYPitch/2;
        if(USE_Y_AUTO_PITCH==true)                                              //ChungHung 20140304 add for AutoYPitch
        {
            if(Other==false)
                *iY-=GetVariableYInShuttleData();
        }
        else
        {
            if(Other==true)
                *iY+=TestIF.iARM_Y_PITCH;
        }
        TransferInShuttleRatio(iShuttle, iX, iY, iInArmYBase, iInArmXBase);     //Steven 20110324
    }
    else if(iMode==2)
    {
        if(iSht==0)
        {
            *iX+=Prod.XInArm_Shuttle1_Place[0][1];
            *iY+=Prod.YInArm_Shuttle1_Place[0][1];
        }
        else
        {
            *iX+=Prod.XInArm_Shuttle2_Place[0][1];
            *iY+=Prod.YInArm_Shuttle2_Place[0][1];
        }

        if(USE_Y_AUTO_PITCH==true)                                              //ChungHung 20140304 add for AutoYPitch
        {
            if(Other==false)
                *iY-=GetVariableYInShuttleData();
        }
        else
        {
            if(Other==true)
                *iY+=TestIF.iARM_Y_PITCH;
        }
        TransferInShuttleRatio(iShuttle, iX, iY, 0, 1);                         //Steven 20110324
    }
    else if(iMode==3)                                                           //Steven 20230530 : 1x2_4改用Row A
    {
        if(iSht==0)
        {
            *iX+=Prod.XInArm_Shuttle1_Place[0][1];
            *iY+=Prod.YInArm_Shuttle1_Place[0][1];
        }
        else
        {
            *iX+=Prod.XInArm_Shuttle2_Place[0][1];
            *iY+=Prod.YInArm_Shuttle2_Place[0][1];
        }
        *iX=*iX+iMovePitchX*1-TestIF.dSiteXPitch*0.5;
        if(USE_Y_AUTO_PITCH==true)                                              //ChungHung 20140304 add for AutoYPitch
        {
            *iY+=iMovePitchY;
        }
        TransferInShuttleRatio(iShuttle, iX, iY, 0, 1);                         //Steven 20110324
    }
    else if(iMode==5)
    {
        if(iSht==0)
        {
            *iX+=Prod.XInArm_Shuttle1_Place[0][2];
            *iY+=Prod.YInArm_Shuttle1_Place[0][2];
        }
        else
        {
            *iX+=Prod.XInArm_Shuttle2_Place[0][2];
            *iY+=Prod.YInArm_Shuttle2_Place[0][2];
        }
        //jou 980902 : x & y pitch no stantder
//        *iX+=TestIF.dSiteXPitch/2;
//        *iY+=TestIF.dSiteYPitch/2;
        if(USE_Y_AUTO_PITCH==true)                                              //ChungHung 20140304 add for AutoYPitch
        {
            if(Other==false)
                *iY-=GetVariableYInShuttleData();
        }
        else
        {
            if(Other==true)
                *iY+=TestIF.iARM_Y_PITCH;
        }
        TransferInShuttleRatio(iShuttle, iX, iY, 0, 2);                         //Steven 20110324
    }
    else if(iMode==6)
    {
        if(iSht==0)
        {
            *iX+=Prod.XInArm_Shuttle1_Place[1][0];
            *iY+=Prod.YInArm_Shuttle1_Place[1][0];
        }
        else
        {
            *iX+=Prod.XInArm_Shuttle2_Place[1][0];
            *iY+=Prod.YInArm_Shuttle2_Place[1][0];
        }
        //jou 980902 : x & y pitch no stantder
        *iX+=TestIF.dSiteXPitch/2;
        if(USE_Y_AUTO_PITCH==true)                                              //ChungHung 20140304 add for AutoYPitch
        {
            *iY-=GetVariableYInShuttleData()-TestIF.dSiteYPitch/2;
            if(Other==true)
                *iY+=GetVariableYInShuttleData();
        }
        else
        {
            *iY+=6000-TestIF.dSiteYPitch/2;
            if(Other==true)
                *iY+=TestIF.iARM_Y_PITCH;
        }
        TransferInShuttleRatio(iShuttle, iX, iY, 1, 0);                         //Steven 20110324
    }
    else if(iMode==7)
    {
        if(iSht==0)
        {
            *iX+=Prod.XInArm_Shuttle1_Place[0][1];
            *iY+=Prod.YInArm_Shuttle1_Place[0][1];
        }
        else
        {
            *iX+=Prod.XInArm_Shuttle2_Place[0][1];
            *iY+=Prod.YInArm_Shuttle2_Place[0][1];
        }
        *iX+=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;
        if(USE_Y_AUTO_PITCH==true)                                              //ChungHung 20140304 add for AutoYPitch
        {
            *iY-=GetVariableYInShuttleData()/2;
            if(Other==true)
                *iY+=GetVariableYInShuttleData();
        }
        else
        {
            *iY+=TestIF.dSiteYPitch/2;
            if(Other==true)
                *iY+=TestIF.iARM_Y_PITCH;
        }
        TransferInShuttleRatio(iShuttle, iX, iY, 0, 1);                         //Steven 20110324
    }
    else if(iMode==8)
    {
        if(iSht==0)
        {
            *iX+=Prod.XInArm_Shuttle1_Place[1][1];
            *iY+=Prod.YInArm_Shuttle1_Place[1][1];
        }
        else
        {
            *iX+=Prod.XInArm_Shuttle2_Place[1][1];
            *iY+=Prod.YInArm_Shuttle2_Place[1][1];
        }
        *iX+=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;
        if(USE_Y_AUTO_PITCH==true)                                              //ChungHung 20140304 add for AutoYPitch
        {
            *iY-=GetVariableYInShuttleData()-TestIF.dSiteYPitch/2;
            if(Other==true)
                *iY+=GetVariableYInShuttleData();
        }
        else
        {
            *iY+=TestIF.iARM_Y_PITCH-TestIF.dSiteYPitch/2;
            if(Other==true)
                *iY+=TestIF.iARM_Y_PITCH;
        }
        TransferInShuttleRatio(iSht, iX, iY, 1, 1);                             //Steven 20110324
    }*/

    *iY=*iY+GetInArmYToShuttleOffset_9045(iSht, iOffsetPos);
    *iX=*iX+GetInArmXToShuttleOffset_9045(iSht, iOffsetPos);
    TransferInShuttleRatio(iSht, iX, iY, iInArmYBase, iInArmXBase);

    InArmZNeedDown_9045(iSht, 0, bPlace);
}
//==============================================================================
// in arm x y to shuttle position
//==============================================================================
bool MoveInArm2XYToShuttle_9045_1x2_4_Hot(int iSht, bool IncludeZ, bool bPlace)
{
    int iXPos               =iInArmShtXCenterPos;                               //Steven 20141029 : XY-Pitch for Shuttle Center position for base Suck
    int iYPos               =iInArmShtYCenterPos;
    int iMode               =GetNowSiteKitMode_1x2_4_Hot(iSht, bPlace);
    int iModeRow            =iMode%100;
    int iModeCol            =iMode/100;
    int iMovePitchX         =iXpitchMaxX3;
    int iMovePitchY         =GetVariableYInShuttleData();
    int iOffsetPos          =GetInArmToShuttleOffset_9045(iSht, iModeRow, iModeCol, true);
    int iYVariable          =GetInArmPitchY_9045(iMovePitchY, iOffsetPos);
    static bool bCheckZSafe =false;

    if(bCheckZSafe==false)
    {
        if(MoveInArmZToPlateSafe(1111)==false)
            return false;
        else
            bCheckZSafe=true;

        ResetInToShtFlag();
    }

    if(USE_PICKER_COUNT==ep16Picker)                                            //基準為第四隻吸嘴
    {
        if(InArmSuck.iXStep==1)
            iMovePitchX=TestIF.dSiteXPitch/4.0*7.0;
        else
            iMovePitchX=iXpitchMaxX7;
    }
    else
    {
        if(InArmSuck.iXStep==1)
            iMovePitchX=TestIF.dSiteXPitch/2.0*3.0;
        else
            iMovePitchX=iXpitchMaxX3;
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iInXPToSht[i]=GetInArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    if(TestIF.bNS7000kit)                                                       //jou 981208 start : NS7000 bias kit
    {
        if(iSht==0)
            iYPos+=iMovePitchY/2;
        else
            iYPos-=iMovePitchY/2;
    }

    CheckXYPitch_1x2_4_Hot(&iXPos, &iYPos, iSht, bPlace, iMovePitchX, iMovePitchY);
    GetInArmZShtDownPos_9045(iSht, bPlace, IncludeZ);

    if(InArmContinuousMove_9045(iXPos, iYPos, iInXPToSht, iYVariable, bZFlgToSht, iZPosToSht, IncludeZ))
    {
        ResetInToShtFlag();
        bCheckZSafe=false;
        return true;
    }
    return false;
}
//==============================================================================
bool DoPlaceToHotPlate_9045_1x2_4_Hot()
{
    int &Task=iInArmPlaceToHotPlateTask;
    int ip, ix, iy, j2;
    int iStepHP=2;
    bool flag=true;

    GetHotPlateYHalfPos();

    switch(Task)
    {
        case 1:
            GetVariableXInHotPlateData(iPlaceHP);
            SearchPlateToPlace();
            bPlaceToHotplatePartOK=false;
            Task=100;                                                           //because Z and X-Y is continue move
//            break;
        case 100:
            if(MoveInArmXYToHotPlatePlace(iPlaceHP, true))
            {
                Task=200;
            }
            else
            {
                if(CheckInArmSuckICFallDownToHasNullIC(false)==true)            //Steven 20110516 : 修改成整合式Alarm
                {
                    Task=110;                                                   //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
                }
            }
            break;
        case 110:
            CheckInArmSuckICFallDownToHasNullIC();                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            Task=100;
            break;
        case 200:
            if(InArmNeedCheckHotPlateOffset(true))                              //Steven 20230531 : 簡化判斷式
            {
                Task=210;
                break;
            }

            if(LastSet.iRunStartMode==rsmAutoSiteMap &&
               bAutoSiteMapHasPickHP==true &&
               CosFunction.bUSEJCETSiteMapMode==true)                           //Steven 20220527 : for JCET Auto Site Map
            {
                //pass
            }
            else                                                                //Jimmychiu 20230417 : Record the position after placing the IC
            {
                PickFromHPList->UpdateHPSuckGroup(iPlacePlate[0], iPlacePlateY[0], iPlacePlateX[0], InArmSuck.iWhichSht, InArmSuck.iWhichKit);
            }

            if(ArmSpeed[InArm].iEnableReleaseDelay==0)                          //JerryYang 20160127 for TSMC inarm release device前delay
            {
                InArmReleaseDelayToHot.SetSecAndOn(ArmSpeed[InArm].dReleaseDelayTime);
                Task=340;
            }
            else
            {
                Task=350;
            }
            break;
        case 210:
            if(bEnterOffset==true)
            {
                bEnterOffset=false;
                Task=220;
            }
            else
            {
                Task=100;
            }
            break;
        case 220:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=100;
            }
            break;
        case 340:
            if(InArmReleaseDelayToHot.Off())                                    //JerryYang 20160127 for TSMC inarm release device前delay
            {
                Task=350;
            }
            break;
        case 350:
            if(LastSet.iRunStartMode==rsmAutoSiteMap &&
               bAutoSiteMapHasPickHP==true &&
               CosFunction.bUSEJCETSiteMapMode==true)                                                                   //Steven 20220527 : for JCET Auto Site Map
            {
                if(InArmSuck.Suck[iAutoSiteMapInArmRow][iAutoSiteMapInArmCol].Destroy())
                {
                    DoPlaceToHPBackupData(iAutoSiteMapInArmRow, iAutoSiteMapInArmCol, iAutoSiteMapHPNo, iAutoSiteMapHPR, iAutoSiteMapHPC);
                    InArmSuck.SetAllToNullIC();
                }
                else                                                                                                    //if(InArmSuck.Suck[iAutoSiteMapInArmRow][iAutoSiteMapInArmCol].Error==false) // kevin 20141206
                {
                    return false;
                }
            }
            else
            {
                for(int i=0; i<1; i++)
                {
                    for(int j=0; j<iStepHP; j++)
                    {
                        if(i==1 && Row2CanPutHP(PlaceMode)==false &&
                           iPlaceHPOrder==0 &&
                           bAutoSiteMapHotplateSave==false)                                                             //Ifor 20171003 (Steven) : add避免無法放回 Hotplate
                        {
                            continue;
                        }

                        j2=GetPlaceToHotPlateSuckCol(j);

                        if(InArmSuck.Item[i][j2]==HAS_IC ||
                           InArmSuck.Item[i][j2]==HAS_NULL_IC)
                        {
                            ip=iPlacePlate[0];
                            ix=GetPlaceToHotPlateCol(j);

                            if(ix>=HotPlateForm.XDivision &&
                               bAutoSiteMapHotplateSave==false)                                                         //Ifor 20180813 (Steven) : Add Auto Site Mapping 補回IC時不可跳過避免Hangup
                            {
                                continue;
                            }

                            if(iPlaceHPOrder==0)
                            {
                                iy=iPlacePlateY[0]+iYHalf*i;
                            }
                            else
                            {
                                iy=iPlacePlateY[0];
                            }

                            if(InArmSuck.Item[i][j2]==HAS_NULL_IC ||
                               InArmSuck.Suck[i][j2].Destroy())
                            {
                                if(bAutoSiteMapHotplateSave==true)
                                {
                                    DoPlaceToHPBackupData(i, j2, iAutoSiteMapHotplateSource, iy, ix);                   //Ifor 20170928 (Steven) : 將Auto Site Mapping 資料寫回到HP
                                }
                                else
                                {
                                    DoPlaceToHPSwapData(i, j2, ip, iy, ix);                                             //Steven 20170109 : 將放料到HP資料交換改成Function
                                }
                            }
                            else if(InArmSuck.Suck[i][j2].Error==false)
                            {
                                flag=false;
                            }
                        }
                    }
                }

                if(flag==false)
                    return false;

                for(int i=0; i<1; i++)
                {
                    for(int j=0; j<iStepHP; j++)
                    {
                        j2=GetPlaceToHotPlateSuckCol(j);

                        if(iPlaceHPOrder==0)
                        {
                            if(Row2CanPutHP(PlaceMode))
                            {
                                if(InArmSuck.Item[i][j2]!=NULL_IC)
                                    return false;
                            }
                            else
                            {
                                if(InArmSuck.Item[0][j2]!=NULL_IC)
                                    return false;
                            }
                        }
                        else
                        {
                            if(InArmSuck.Item[1][j2]!=NULL_IC)
                                return false;
                        }
                    }
                }
            }

            bPlaceToHotplatePartOK=true;                                                                                //ChungHung 20120412 add
            DoCheckAutoSiteMappingPosition();                                                                           //Ifor 20170928 (Steven) : Check Auto Site Mapping Position
            Task=400;
            break;
        case 360:                                                                                                       //20111015  Dell    HotPlateForm.XDivision == 3
            if(LastSet.iRunStartMode==rsmAutoSiteMap &&
               bAutoSiteMapHasPickHP==true &&
               CosFunction.bUSEJCETSiteMapMode==true)                                                                   //Steven 20220527 : for JCET Auto Site Map
            {
                if(InArmSuck.Suck[iAutoSiteMapInArmRow][iAutoSiteMapInArmCol].Destroy())
                {
                    DoPlaceToHPBackupData(iAutoSiteMapInArmRow, iAutoSiteMapInArmCol, iAutoSiteMapHPNo, iAutoSiteMapHPR, iAutoSiteMapHPC);
                    InArmSuck.SetAllToNullIC();
                }
                else
                {
                    return false;
                }
            }
            else
            {
                for(int i=0; i<1; i++)
                {
                    for(int j=0; j<iStepHP; j++)
                    {
                        j2=GetPlaceToHotPlateSuckCol(j);
                        if(i==1 && Row2CanPutHP(PlaceMode)==false &&
                           iPlaceHPOrder==0 &&
                           bAutoSiteMapHotplateSave==false)                                                             //Ifor 20171003 (Steven) : add避免無法放回 Hotplate
                        {
                            continue;
                        }

                        if(InArmSuck.Item[i][j2]==HAS_NULL_IC ||
                           InArmSuck.Item[i][j2]==HAS_IC)
                        {
                            //Steven 20161213 : 修改1x2關Ab擺滿的判斷方式
                            if(i==0 && j==1 && TestIF.iSiteMap[0][1]==0 && IniConfig.bDualSiteCloseAbCanFullHotplate)   //Jou 20110708 : 關Site要擺滿   //ChungHung 20130910 alter for SCK can close site by Index
                            {
                            }
                            else
                            {
                                ip=iPlacePlate[0];
                                if(i==0 && j==1)
                                {
                                    if(j==1 && iPlacePlateX[0]==2 && iPlacePlateY[0]%2==0)
                                        continue;

                                    if(iPlacePlateX[0]==2)
                                    {
                                        ix=iPlacePlateX[0];
                                    }
                                    else
                                    {
                                        ix=iPlacePlateX[0]+1;
                                    }
                                }
                                else
                                {
                                    ix=iPlacePlateX[0];
                                }
                                iy=iPlacePlateY[0];

                                if((ix<HotPlateForm.XDivision)==false)
                                    continue;

                                if(ix<2)
                                {
                                    if(MOT[MMPlate1+ip].Tray.Data[ix][iy]==NULL_IC)
                                    {
                                        flag1_1x2_2[j]=true;
                                        if(InArmSuck.Item[i][j2]==HAS_NULL_IC || InArmSuck.Suck[i][j2].Destroy())
                                        {
                                            DoPlaceToHPSwapData(i, j2, ip, iy, ix);                                     //Steven 20170109 : 將放料到HP資料交換改成Function
                                        }
                                        else if(InArmSuck.Suck[i][j2].Error==false)                                     //kevin 20141206  //Steven 20150818 : 換位置
                                        {
                                            flag=false;
                                        }
                                    }
                                }
                                else
                                {
                                    if(j==0 && iForPlaceHPX3Step==0)
                                    {
                                        if(MOT[MMPlate1+ip].Tray.Data[ix][iy]==NULL_IC)
                                        {
                                            flag1_1x2_2[j]=true;
                                            if(InArmSuck.Item[i][j2]==HAS_NULL_IC || InArmSuck.Suck[i][j2].Destroy())
                                            {
                                                DoPlaceToHPSwapData(i, j2, ip, iy, ix);                                 //Steven 20170109 : 將放料到HP資料交換改成Function
                                            }
                                            else if(InArmSuck.Suck[i][j2].Error==false)                                 //kevin 20141206  //Steven 20150818 : 換位置
                                            {
                                                flag=false;
                                            }
                                        }
                                    }
                                    else if(j==1 && iForPlaceHPX3Step==1)
                                    {
                                        if(MOT[MMPlate1+ip].Tray.Data[ix][iy]==NULL_IC)
                                        {
                                            flag1_1x2_2[j]=true;
                                            if(InArmSuck.Item[i][j2]==HAS_NULL_IC || InArmSuck.Suck[i][j2].Destroy())
                                            {
                                                DoPlaceToHPSwapData(i, j2, ip, iy, ix);                                 //Steven 20170109 : 將放料到HP資料交換改成Function
                                            }
                                            else if(InArmSuck.Suck[i][j2].Error==false)                                 //kevin 20141206  //Steven 20150818 : 換位置
                                            {
                                                flag=false;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if(flag==false)
                    return false;

                if(flag1_1x2_2[0] && InArmSuck.Item[0][0]!=NULL_IC)
                    return false;
                if(flag1_1x2_2[1] && InArmSuck.Item[0][2]!=NULL_IC)
                    return false;
            }

            bPlaceToHotplatePartOK=true;                                                                                //ChungHung 20120412 add
            DoCheckAutoSiteMappingPosition();                                                                           //Ifor 20170928 (Steven) : Check Auto Site Mapping Position
            Task=400;
            break;
        case 400:
            if(MoveInArmZToPlateSafe(Task))
            {
                if(InArmSuck.HasIC())                                           //Steven 20260402 : Fixed Step400 use HasIC() instead of GetPlaceToHotPlateSuckCol loop
                {
                    Task=1;
                    return false;
                }

                Task=500;
            }
            break;
        case 500:                                                               //kevin 20131106 start   黏貨判斷
            flag=CheckInArmDestroyICFail();                                     //Steven 20111223 : 檢查吹氣IC是否粘黏錯誤
            if(flag==false)
            {
                RecordProcess("VOFTask=500");
                return false;
            }
            Task=501;
            break;
        case 501:
            SetInArm_Unuse_SuckToNullICForHP();

            if(TestIF.iTestMode==DualSite &&
               TestIF.iSiteMap[0][1]==0)                                        //ChungHung 20130910 alter for SCK can close site by Index
            {
                if(InArmSuck.Item[0][InArmSuck.iPickStep]==HAS_NULL_IC)
                {
                    InArmSuck.SetItemData(0, InArmSuck.iPickStep, NULL_IC);
                }
            }

            AdjustShuttleWhichKitOrder();                                       //Steven 20160201 : 修正Shuttle放料順序
            iHotCount++;                                                        //jou 2013-02-08 修正等待 Soak 時間時，畫面沒有顯示倒數計時的問題

            if(LastSet.iTemperature==Tempture_Hot &&
               CheckHasSpaceToPlace_9045())
                SearchPlateToPlace();

            if(LastSet.iRunStartMode==rsmAutoSiteMap &&
               bAutoSiteMapHasPickHP==true &&
               CosFunction.bUSEJCETSiteMapMode==true)                           //Steven 20220527 : for JCET Auto Site Map
            {
                bAutoSiteMapHasPickHP=false;
            }
            else
            {
                PickFromHPList->AddHPSuckGroup();                               //放完了就加入一個新的Group
            }

            if(USE_LASER_DISTANCE &&
               TestIF_File.bEnableInArmLaser &&
               LaserCheckPos.size()!=0)                                         //Steven 20140228 : 雷射測距功能
            {
                CheckInArmFloating(true);
                Task=600;
                break;
            }
            else
            {
                return true;
            }
        case 600:
            if(CheckInArmFloating())                                            //Steven 20140228 : 雷射測距功能
            {
                return true;
            }
            break;
    }
    return false;
}
//==============================================================================
bool DoInArmPlaceToShuttle_9045_1x2_4_Hot()
{
    IN_ARM_SHUTTLE_1x2_4_Hot:

    QueueTaskList[4].CheckTaskChange();                                         //Steven 20200821 : 使用Goto也要記錄Task變化
    int ret;
    int &Task=iInArmPlaceToShuttleTask, iFlag=0, j2;
    int iSuckRow=0, iSuckCol=0, iShtRow=0, iShtCol=0;
    bool flag, bCanFreeShuttle=false;
    static bool bCheckSpeed=false;
    AnsiString ErrPart="", str="";
    static bool bCheckSpeed1;                                                   //KaiChen 20171225 (Steven)：Add Speed Display
    static bool bPlaceOtherShuttle=false;                                       //ChungHung 20110321 add
    int iOtherShuttleOffset=0;
    AutoSiteMappingCheckShuttle(false);                                         //Ifor 20180116 (Steven) : add Auto Site Mapping Check iWhich Shuttle
    int iRow=(i1x2_4UseACEGPicker==1)?1:2;                                      //Steven 20230530 : 1x2_4改用Row A
    int iCol=(i1x2_4UseACEGPicker==1)?4:2;                                      //Steven 20230530 : 1x2_4改用Row A

    if(bIndexPickUpErrorWaitRetry)                                              //Ifor 20171119 : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup    //Steven 20220327 : Add for hang up
        return false;

    int iSht=InArmSuck.iWhichSht;//, iKit=0;
//    int iPickKit32=0;

    switch(Task)
    {
        case 1:
            bDestoryOnSht=false;                                                //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            InArmXMoveSafe=false;
            bPlaceOtherShuttle=false;

            for(int i=0; i<iRow; i++)
            {
                for(int j=0; j<iCol; j++)
                {
                    j2=(i1x2_4UseACEGPicker==1)?j:j*2;
                    if(InArmSuck.Item[i][j2]==NULL_IC)
                    {
                        InArmSuck.SetItemData(i, j2, HAS_NULL_IC);              //Steven 20140710 : Add
                    }
                }
            }

            if(InArmSuck.Item[0][0]==HAS_NULL_IC &&                             //Steven 20180420 (Jou) : 修正只放一半剛好JAM, 吸嘴會跑下去壓傷IC
               InArmSuck.Item[0][2]==HAS_NULL_IC)
            {
                bPlaceOtherShuttle=true;
                InArmSuck.SetItemData(0, 0, NULL_IC);
                InArmSuck.SetItemData(0, 2, NULL_IC);
            }

            if(InArmSuck.HasRealIC()==false)                                    //ChungHung 20120105 如果剛好在Shuttle放完IC時 InArm剛好讓開會HangUp
            {
                if(MoveInArmZToPlateSafe(Task))
                {
                    InArmSuck.SetAll(NULL_IC);
                    fObserver->RecordInArmTime();
                    return true;
                }
                return false;
            }

            if(ArmSpeed[InArm].bAutoSpeed &&
               LastSet.iTemperature!=Tempture_Hot)
                bCheckSpeed=true;
            else
                bCheckSpeed=false;

            if(IniConfig.bA26MotorSpeedSortDisplay==true &&
               LastSet.iTemperature!=Tempture_Hot)
                bCheckSpeed1=true;
            else
                bCheckSpeed1=false;

            Task=100;
        case 100:
            if(iSht==0)
            {
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
                   (bShuttle1MoveToRight ||
                    bShuttle1MoveToLeft  ||                                     //Steven 20220620 : 避免index在Kit1吸取異常, In arm偷放料
                    bShuttle1HasPickErr))                                       //Steven 20230116 : 避免In arm 偷放料
                {
                    Task=1;
                    return false;
                }
                else
                {
                    if(IniConfig.bA26MotorSpeedSortDisplay==true)
                    {
                        if(InShtInLF(0))
                            iInShuttleSpeed1--;
                        else
                            iInShuttleSpeed1++;
                    }
                    Task=900;
                }
            }
            else
            {
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
                   (bShuttle2MoveToRight ||
                    bShuttle2MoveToLeft  ||                                     //Steven 20220620 : 避免index在Kit1吸取異常, In arm偷放料
                    bShuttle2HasPickErr))                                       //Steven 20230116 : 避免In arm 偷放料
                {
                    Task=1;
                    return false;
                }
                else
                {
                    if(IniConfig.bA26MotorSpeedSortDisplay==true)
                    {
                        if(InShtInLF(1))
                            iInShuttleSpeed1--;
                        else
                            iInShuttleSpeed1++;
                    }
                    Task=1900;
                }
            }
            goto IN_ARM_SHUTTLE_1x2_4_Hot;
        case 900:
            if(InSHT1InLF() &&
               FLCarryKit.UseSiteFullIC()==false)
            {
                if(bCheckSpeed)                                                 //Steven 20110525 : Auto Speed
                {
                    bCheckSpeed=false;
                    InArmAddSpeed();
                }

                if(bCheckSpeed1)
                {
                    bCheckSpeed1=false;
                    InArmAddSpeedDisplay();
                }
                Task=950;
                break;
            }
            else
            {
                if(bCheckSpeed)                                                 //Steven 20110525 : Auto Speed
                {
                    bCheckSpeed=false;
                    InArmSubSpeed();
                }

                if(bCheckSpeed1)
                {
                    bCheckSpeed1=false;
                    InArmSubSpeedDisplay();
                }
                Task=930;
            }
        case 930:
            if(MoveInArmXYToWaitTrayArm(iSht, 0, ZAxisNotDown, true)!=0)
            {
                Task=1000;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                //Steven 20110516 : 修改成整合式Alarm
            {
                Task=935;                                                       //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 935:
            CheckInArmSuckICFallDownToHasNullIC();                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            Task=930;
            break;
        case 950:
            SetShuttlefCanMoveL(0, false, __FUNC__, "950");
            iFlag=MoveInArmXYToWaitTrayArm(iSht, 0, ZAxisDown, true);
            if(iFlag!=0)
            {
                Task=iFlag;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                //Steven 20110516 : 修改成整合式Alarm
            {
                Task=955;                                                       //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 955:
            CheckInArmSuckICFallDownToHasNullIC(true);                          //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            if(MoveInArmZToPlateSafe(Task))
            {
                str.sprintf("Please take out the drop device from Shuttle%d", iSht+1);
                ShowMyMessage(str);
                Task=950;
            }
            break;
        case 1000:
            if(InSHT1InLF())
            {
                bCanFreeShuttle=true;                                                                                                                           //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
                for(int i=0; i<InArmSuck.iShtRow; i++)
                {
                    for(int j=0; j<InArmSuck.iShtCol; j++)
                    {
                        if(FLCarryKit.Item[i][j]==NULL_IC)
                        {
                            bCanFreeShuttle=false;                                                                                                              //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
                        }
                    }
                }

                if(bPlaceToShuttle2Step)                                                                                                                        //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                    bCanFreeShuttle=false;

                if(bCanFreeShuttle==false)
                {
                    if(bShuttle1HasPickErr==false)                                                                                                              //Sam 20230103 : 修正 32 Site Pickup error hange up
                        SetShuttlefCanMoveL(0, false, __FUNC__, "1000");
                    InArmZNeedDown_9045(0, 0, true);
                    Task=950;
                    goto IN_ARM_SHUTTLE_1x2_4_Hot;
                }
            }
            else if(NeedWaitTrayArm && (IniConfig.bP56TrayArmWaitAtColorTrack || (INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)))                          //Steven 20240516 : Tray Arm等待位置改到Color        //wei 20150925 待機位置改道 Color
            {
                InitInOCRWaitTask();                                                                                                                            //wei 20170901 Place To Shuttle
                Task=1030;
            }

            if(MOT[MInShuttle1].Led[iInposLed]==false &&
               MOT[MInShuttle1].fCanMoveL==false)
            {
                bCanFreeShuttle=true;                                                                                                                           //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            }

            if(bCanFreeShuttle==true)                                                                                                                           //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            {
                Task=1050;
            }
            break;
        case 1030:
            if(OCRMoveInArm2XYToWait())                                         //wei 20170901 Place To Shuttle
            {
                Task=1;
            }
            break;
        case 1050:                                                              //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            if(MoveInArmZToPlateSafe(Task))
            {
                SetShuttlefCanMoveL(0, true, __FUNC__, "1050");
                Task=1000;
            }
            break;
        case 1100:
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
               (bShuttle1MoveToRight ||
                bShuttle1MoveToLeft  ||                                         //Steven 20220620 : 避免index在Kit1吸取異常, In arm偷放料
                bShuttle1HasPickErr))                                           //Steven 20230116 : 避免In arm 偷放料
            {
                Task=1;
                return false;
            }

            SetShuttlefCanMoveL(0, false, __FUNC__, "1100");
            if(InShtInLF(0)==false)
            {
                Task=1110;
                break;
            }

            if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
               IniConfig.bF23ShuttleVibration)                                  //kevin 20210415 IN Arm Vibrate shuttle
                SW[SwShuttleVibration1].On();

            bDestoryOnSht=true;                                                 //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            if(MoveInArmZToShuttlePlace_9045(0))
            {
                if(InArmNeedCheckOffset(true, 0))                               //Steven 20230531 : 簡化判斷式
                {
                    Task=1150;
                    break;
                }
                InArmSuck.ResetAll();                                           //Steven 20160323 : 避免未開啟真空
                Task=1200;
                if(ArmSpeed[InArm].iEnableReleaseDelay==0)                      //JerryYang 20160127 for TSMC inarm release device前delay
                {
                    InArmReleaseDelay.SetSecAndOn(ArmSpeed[InArm].dReleaseDelayTime);
                    Task=1180;
                }
            }
            break;
        case 1110:
            if(MoveInArmZToPlateSafe(Task))
            {
                bDestoryOnSht=false;                                            //Steven 20180419 (Jou) : 在Shuttle吹氣與資料交換的Flag
                SetShuttlefCanMoveL(0, true, __FUNC__, "1110");
                Task=1;
            }
            break;
        case 1150:
            if(bEnterOffset==true)
            {
                bEnterOffset=false;
                Task=1160;
            }
            else
            {
                Task=1100;
            }
            break;
        case 1160:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=1170;
            }
            break;
        case 1170:
            if(MoveInArmXYToShuttle_9045(iSht, 0, ZAxisDown, true))
            {
                Task=1100;
            }
            break;
        case 1180:
            if(InArmReleaseDelay.Off())                                         //JerryYang 20160127 for TSMC inarm release device前delay
            {
                Task=1200;
            }
            break;
        case 1200:
            flag=true;

            if(bPlaceOtherShuttle)
                iOtherShuttleOffset=1;
            else
                iOtherShuttleOffset=0;

            if(i1x2_4UseACEGPicker==0)                                                                                  //Steven 20230530 : 1x2_4改用Row A
                iSuckRow=iOtherShuttleOffset;
            else
                iSuckRow=0;
            {
                for(int j=0; j<2; j++)
                {
                    iSuckRow=iSuckRow;
                    iSuckCol=(i1x2_4UseACEGPicker==0)?(j*2):(j*2+iOtherShuttleOffset);                                  //Steven 20230530 : 1x2_4改用Row A
                    iShtRow =0;
                    iShtCol =GetShuttleCol(iSuckRow, iSuckCol);

                    if(InArmSuck.Item[iSuckRow][iSuckCol] &&
                       InArmSuck.Suck[iSuckRow][iSuckCol].GetNeedDestroyStatus())
                    {
                        iBackInArmHotCount=InArmSuck.HotCount;
                        if(InArmSuck.Item[iSuckRow][iSuckCol]==HAS_NULL_IC ||
                           InArmSuck.Suck[iSuckRow][iSuckCol].Destroy())
                        {
                            SetShuttleStatus_9045(iSht, iShtRow, iShtCol, iSuckRow, iSuckCol);
                        }
                        else if(InArmSuck.Suck[iSuckRow][iSuckCol].Error==false)
                        {
                            flag=false;
                        }
                    }
                }
            }

            if(flag==false)                                                                                             //kevin 20131011 在下真空誤判, 換到上面來
                break;

            ret=CheckInArmDestroyICFail();                                                                              //Steven 20111223 : 檢查破壞錯誤
            if(ret==false)
                return false;

            for(int i=0; i<2; i++)
            {
                for(int j=0; j<4; j++)
                {
                    if(InArmSuck.Item[i][j]!=NULL_IC &&
                       InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                        return false;
                }
            }

            if(i1x2_4UseACEGPicker==0)                                                                                  //Steven 20230530 : 1x2_4改用Row A
            {
                if((InArmSuck.ArmUpSideHaveRealIC()==false && bPlaceOtherShuttle==false) ||
                   (InArmSuck.ArmDownSideHaveRealIC()==false && bPlaceOtherShuttle==true))
                {
                    Task=1300;
                    fObserver->AddTimeData(2, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);                           //JerryYang 20151209
                }
                else
                {
                    Task=1250;
                }
            }
            else
            {
                if((InArmSuck.ArmLeftSideHaveRealIC(2)==false && bPlaceOtherShuttle==false) ||
                   (InArmSuck.ArmRightSideHaveRealIC(2)==false && bPlaceOtherShuttle==true))
                {
                    Task=1300;
                    fObserver->AddTimeData(2, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);                           //JerryYang 20151209
                }
                else
                {
                    Task=1250;
                }
            }
            break;
        case 1250:
            if(MoveInArmXYToShuttle_9045(iSht, 0, ZAxisDown, true))
            {
                Task=1100;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                //Steven 20110516 : 修改成整合式Alarm
            {
                Task=1255;                                                      //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 1255:
            CheckInArmSuckICFallDownToHasNullIC(true);                          //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            if(MoveInArmZToPlateSafe(Task))
            {
                str.sprintf("Please take out the drop device from Shuttle%d", iSht+1);
                ShowMyMessage(str);
                Task=1250;
            }
            break;
        case 1300:                                                              //Steven 20220427 : 整合蝦頭搖搖敲敲功能
            InitDoInArmCheckShtFloatTask();
            Task=1400;
        case 1400:
            flag=DoInArmCheckShuttleFloating(0);
            if(flag==true)
            {
                Task=1500;
            }
            else
            {
                break;
            }
        case 1500:
            SetShuttleToHasNullIC_9045(0);

            if(InArmSideAllClose(1)==true)                                                                              //如果Arm 2的Site全部被關掉
            {
                SetShuttleToHasNullIC_9045(1);
            }

            if(In_Shuttle_Auto_Latch==eInSHAutoLtc &&
               bInSHLtcFin[0]==false               &&
               FLCarryKit.UseSiteFullIC())                                      //KenHsieh 20251106 : fix close site don't do In Sht Latch
            {
                Task=1400;
                break;
            }

            MOT[MInShuttle1].fCanMoveL=true;

            AdjustShtOrderWhenPlaceToSht(1);                                                                            //Steven 20180601 : 整合放完蝦頭後的調整

            bDestoryOnSht=false;                                                                                        //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            fObserver->RecordInArmTime();
            if((i1x2_4UseACEGPicker==0 && InArmSuck.ArmDownSideHaveRealIC()==true) ||                                   //Steven 20230530 : 1x2_4改用Row A
               (i1x2_4UseACEGPicker==1 && InArmSuck.ArmRightSideHaveRealIC(2)==true))
            {
                bPlaceOtherShuttle=true;
                Task=1900;
                return false;
            }
            else
            {
                if(i1x2_4UseACEGPicker==0)                                                                              //Steven 20230530 : 1x2_4改用Row A
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                        if(InArmSuck.Item[1][j]==HAS_NULL_IC)
                            InArmSuck.SetItemData(1, j, NULL_IC);
                }
                else
                {
                    for(int j=0; j<2; j++)
                        if(InArmSuck.Item[0][1+j*2]==HAS_NULL_IC)
                            InArmSuck.SetItemData(1, 1+j*2, NULL_IC);
                }
            }
            bPlaceOtherShuttle=false;

            if(IniConfig.bIndexPickupWait==true &&
               (LastSet.iTemperature==Tempture_Hot ||                                                                   //jou 2012-06-29 Index Pick up need wait Soak Time
                LastSet.iTemperature==Tempture_AmbientHot))                                                             //kevin 20180903 (Steven) : add 恆溫控制
            {
                if(Temperature.iShuttleSoakTimeMode==1 &&
                   Temperature.iInitialStart2Time!=0)
                {
                    FLCarryKit.TSoakTime.SetSecAndOn(Temperature.iInitialStart2Time);
                    dwStartShuttle1Soak=MyTickCount();                                                                  //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
                    iInitialStart2Count=Temperature.iInitialStart2Time;
                }
            }
            fObserver->RecordInArmTime();
            bDestoryOnSht=false;                                                                                        //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            iInArmPickPlaceCnt[InOfsInSh1]++;                                                                           //JerryYang 20180921 Setup Teach功能
            if(USE_IN_Y_IS_AUTO_PITCH==true &&                                                                          //JerryYang 20251218 : IN/OUT ARM支援不同模組
               IniConfig.bE57YPitchHome)                                                                                //kevin 20180822 (Steven) : add put shuttle Y pitch home AutoYPitch)
            {
                 hInArmYpitchHomeTimer.SetSecAndOn(10);
                 Task=1600;
                 return false;
            }
            return true;
       case 1600:
            if(bCheckYPitchRunHomeSen(0))
            {
                return true;
            }
            else if(hInArmYpitchHomeTimer.Off())
            {
                InitProcessSingleMotorTask(MInArmPitchY);
                hInArmYpitchHomeTimer.SetSecAndOn(10);
                bYpitchNeddHome=true;
                Task=1610;
            }
            break;
       case 1610:
            if(bCheckYPitchHome(0))
            {
                bYpitchNeddHome=false;
                SetMotorSpeed();
                return true;
            }
            else if(hInArmYpitchHomeTimer.Off())
            {
                ShowErrorMessage("WAR0123", K_RETRY, MInArmPitchY);
                Task=1600;
            }
            break;
        //---------------------
        //place shuttle1 finish
        //---------------------
        case 1900:
            if(InSHT2InLF() &&
               BLCarryKit.UseSiteFullIC()==false)
            {
                if(bCheckSpeed)                                                                                                                                 //Steven 20110525 : Auto Speed
                {
                    bCheckSpeed=false;
                    InArmAddSpeed();
                }

                if(bCheckSpeed1)
                {
                    bCheckSpeed1=false;
                    InArmAddSpeedDisplay();
                }
                Task=1950;
                break;
            }
            else
            {
                Task=1910;
            }
            break;
        case 1910:
            if(bCheckSpeed)                                                                                                                                     //Steven 20110525 : Auto Speed
            {
                bCheckSpeed=false;
                InArmSubSpeed();
            }

            if(bCheckSpeed1)
            {
                bCheckSpeed1=false;
                InArmSubSpeedDisplay();
            }
            Task=1930;
        case 1930:
            if(MoveInArmXYToWaitTrayArm(iSht, 0, ZAxisNotDown, true)!=0)
            {
                Task=2000;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                                                                                                //Steven 20110516 : 修改成整合式Alarm
            {
                Task=1935;                                                                                                                                      //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 1935:
            CheckInArmSuckICFallDownToHasNullIC();                                                                                                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            Task=1930;
            break;
        case 1950:
            SetShuttlefCanMoveL(1, false, __FUNC__, "1950");
            if(MoveInArmXYToShuttle_9045(iSht, 0, ZAxisDown, true))
            {
                Task=2100;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                                                                                                //Steven 20110516 : 修改成整合式Alarm
            {
                Task=1955;                                                                                                                                      //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 1955:
            CheckInArmSuckICFallDownToHasNullIC(true);                                                                                                          //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            if(MoveInArmZToPlateSafe(Task))
            {
                str.sprintf("Please take out the drop device from Shuttle%d", iSht+1);
                ShowMyMessage(str);
                Task=1950;
            }
            break;
        case 2000:
            if(InSHT2InLF())
            {
                bCanFreeShuttle=true;                                                                                                                           //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
                for(int i=0; i<InArmSuck.iShtRow; i++)
                {
                    for(int j=0; j<InArmSuck.iShtCol; j++)
                    {
                        if(BLCarryKit.Item[i][j]==NULL_IC)
                        {
                            bCanFreeShuttle=false;                                                                                                              //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
                        }
                    }
                }

                if(bPlaceToShuttle2Step)                                                                                                                        //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                    bCanFreeShuttle=false;

                if(bCanFreeShuttle==false)
                {
                    if(bShuttle2HasPickErr==false)                                                                                                              //Sam 20230103 : 修正 32 Site Pickup error hange up
                        SetShuttlefCanMoveL(1, false, __FUNC__, "2000");
                    InArmZNeedDown_9045(1, 0, true);
                    Task=1950;
                    goto IN_ARM_SHUTTLE_1x2_4_Hot;
                }
            }
            else if(NeedWaitTrayArm && (IniConfig.bP56TrayArmWaitAtColorTrack || (INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)))                          //Steven 20240516 : Tray Arm等待位置改到Color        //wei 20150925 待機位置改道 Color
            {
                InitInOCRWaitTask();                                                                                                                            //wei 20170901 Place To Shuttle
                Task=2030;
            }

            if(MOT[MInShuttle2].Led[iInposLed]==false &&
               MOT[MInShuttle2].fCanMoveL==false)
            {
                bCanFreeShuttle=true;
            }

            if(bCanFreeShuttle==true)                                                                                                                           //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            {
                Task=2050;
            }
            break;
        case 2030:
            if(OCRMoveInArm2XYToWait())                                                                                                                         //wei 20170901 Place To Shuttle
            {
                Task=1;
            }
            break;
        case 2050:                                                                                                                                              //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            if(MoveInArmZToPlateSafe(Task))
            {
                SetShuttlefCanMoveL(1, true, __FUNC__, "2050");
                Task=2000;
            }
            break;
        case 2100:
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
               (bShuttle2MoveToRight ||
                bShuttle2MoveToLeft  ||                                                                                                                         //Steven 20220620 : 避免index在Kit1吸取異常, In arm偷放料
                bShuttle2HasPickErr))                                                                                                                           //Steven 20230116 : 避免In arm 偷放料
            {
                Task=1;
                return false;
            }

            SetShuttlefCanMoveL(1, false, __FUNC__, "2100");
            if(InShtInLF(1)==false)
            {
                Task=2110;
                break;
            }

            if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
               IniConfig.bF23ShuttleVibration)                                                                                                                  //kevin 20210415 IN Arm Vibrate shuttle
                SW[SwShuttleVibration2].On();

            bDestoryOnSht=true;                                                                                                                                 //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            if(MoveInArmZToShuttlePlace_9045(1))
            {
                if(InArmNeedCheckOffset(true, 1))                                                                                                               //Steven 20230531 : 簡化判斷式
                {
                    Task=2150;
                    break;
                }

                InArmSuck.ResetAll();                                                                                                                           //Steven 20160323 : 避免未開啟真空
                Task=2200;
                if(ArmSpeed[InArm].iEnableReleaseDelay==0)                                                                                                      //JerryYang 20160127 for TSMC inarm release device前delay
                {
                    InArmReleaseDelay.SetSecAndOn(ArmSpeed[InArm].dReleaseDelayTime);
                    Task=2180;
                }
            }
            break;
        case 2110:
            if(MoveInArmZToPlateSafe(Task))
            {
                bDestoryOnSht=false;                                                                                                                            //Steven 20180419 (Jou) : 在Shuttle吹氣與資料交換的Flag
                SetShuttlefCanMoveL(1, true, __FUNC__, "2110");
                Task=1;
            }
            break;
        case 2150:
            if(bEnterOffset==true)
            {
                bEnterOffset=false;
                Task=2160;
            }
            else
            {
                Task=2100;
            }
            break;
        case 2160:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=2170;
            }
            break;
        case 2170:
            if(MoveInArmXYToShuttle_9045(iSht, 0, ZAxisDown, true))
            {
                Task=2100;
            }
            break;
        case 2180:
            if(InArmReleaseDelay.Off())                                                                                                                         //JerryYang 20160127 for TSMC inarm release device前delay
            {
                Task=2200;
            }
            break;
        case 2200:
            flag=true;

            if(bPlaceOtherShuttle)
                iOtherShuttleOffset=1;
            else
                iOtherShuttleOffset=0;

            if(i1x2_4UseACEGPicker==0)                                                                                                                          //Steven 20230530 : 1x2_4改用Row A
                iSuckRow=iOtherShuttleOffset;
            else
                iSuckRow=0;
            {
                for(int j=0; j<2; j++)
                {
                    iSuckRow=iSuckRow;
                    iSuckCol=(i1x2_4UseACEGPicker==0)?(j*2):(j*2+iOtherShuttleOffset);                                                                          //Steven 20230530 : 1x2_4改用Row A
                    iShtRow =0;
                    iShtCol =GetShuttleCol(iSuckRow, iSuckCol);

                    if(InArmSuck.Item[iSuckRow][iSuckCol] &&
                       InArmSuck.Suck[iSuckRow][iSuckCol].GetNeedDestroyStatus())
                    {
                        iBackInArmHotCount=InArmSuck.HotCount;
                        if(InArmSuck.Item[iSuckRow][iSuckCol]==HAS_NULL_IC ||
                           InArmSuck.Suck[iSuckRow][iSuckCol].Destroy())
                        {
                            SetShuttleStatus_9045(iSht, iShtRow, iShtCol, iSuckRow, iSuckCol);
                        }
                        else if(InArmSuck.Suck[iSuckRow][iSuckCol].Error==false)
                        {
                            flag=false;
                        }
                    }
                }
            }

            if(flag==false)                                                                                                                                     //kevin 20131011 在下真空誤判, 換到上面來
                break;

            ret=CheckInArmDestroyICFail();                                                                                                                      //Steven 20111223 : 檢查破壞錯誤
            if(ret==false)
                return false;

            for(int i=0; i<2; i++)
            {
                for(int j=0; j<4; j++)
                {
                    if(InArmSuck.Item[i][j]!=NULL_IC &&
                       InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                        return false;
                }
            }

            if(i1x2_4UseACEGPicker==0)                                                                                                                          //Steven 20230530 : 1x2_4改用Row A
            {
                if((InArmSuck.ArmUpSideHaveRealIC()==false && bPlaceOtherShuttle==false) ||
                   (InArmSuck.ArmDownSideHaveRealIC()==false && bPlaceOtherShuttle==true))
                {
                    Task=2300;
                    fObserver->AddTimeData(2, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);                                                                   //JerryYang 20151209
                }
                else
                {
                    Task=2250;
                }
            }
            else
            {
                if((InArmSuck.ArmLeftSideHaveRealIC(2)==false && bPlaceOtherShuttle==false) ||
                   (InArmSuck.ArmRightSideHaveRealIC(2)==false && bPlaceOtherShuttle==true))
                {
                    Task=2300;
                    fObserver->AddTimeData(2, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);                                                                   //JerryYang 20151209
                }
                else
                {
                    Task=2250;
                }
            }
            break;
        case 2250:
            if(MoveInArmXYToShuttle_9045(iSht, 0, ZAxisDown, bPlaceOtherShuttle))
            {
                Task=2100;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                                                                                                //Steven 20110516 : 修改成整合式Alarm
            {
                Task=2255;                                                                                                                                      //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 2255:
            CheckInArmSuckICFallDownToHasNullIC(true);                                                                                                          //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            if(MoveInArmZToPlateSafe(Task))
            {
                str.sprintf("Please take out the drop device from Shuttle%d", iSht+1);
                ShowMyMessage(str);
                Task=2250;
            }
            break;
        case 2300:                                                                                                                                              //Steven 20220427 : 整合蝦頭搖搖敲敲功能
            InitDoInArmCheckShtFloatTask();
            Task=2400;
        case 2400:
            flag=DoInArmCheckShuttleFloating(1, bPlaceOtherShuttle);                                                                                            //Steven 20220522 : fixed for 8吸嘴模式
            if(flag==true)
            {
                Task=2500;
            }
            else
            {
                break;
            }
        case 2500:
            SetShuttleToHasNullIC_9045(1);

            if(InArmSideAllClose(0)==true)                                                                                                                      //如果Arm 2的Site全部被關掉
            {
                SetShuttleToHasNullIC_9045(0);
            }

            if(In_Shuttle_Auto_Latch==eInSHAutoLtc &&
               bInSHLtcFin[1]==false               &&
               BLCarryKit.UseSiteFullIC())                                      //KenHsieh 20251106 : fix close site don't do In Sht Latch
            {
                Task=2400;
                break;
            }

            MOT[MInShuttle2].fCanMoveL=true;

            AdjustShtOrderWhenPlaceToSht(2);                                                                                                                    //Steven 20180601 : 整合放完蝦頭後的調整

            if((i1x2_4UseACEGPicker==0 && InArmSuck.ArmDownSideHaveRealIC()==true) ||                                                                           //Steven 20230530 : 1x2_4改用Row A
               (i1x2_4UseACEGPicker==1 && InArmSuck.ArmRightSideHaveRealIC(2)==true))
            {
                bPlaceOtherShuttle=true;
                Task=900;
                return false;
            }
            else
            {
                if(i1x2_4UseACEGPicker==0)                                                                                                                      //Steven 20230530 : 1x2_4改用Row A
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                        if(InArmSuck.Item[1][j]==HAS_NULL_IC)
                            InArmSuck.SetItemData(1, j, NULL_IC);
                }
                else
                {
                    for(int j=0; j<2; j++)
                        if(InArmSuck.Item[0][1+j*2]==HAS_NULL_IC)
                            InArmSuck.SetItemData(1, 1+j*2, NULL_IC);
                }
            }
            bPlaceOtherShuttle=false;

            if(IniConfig.bIndexPickupWait==true &&
               (LastSet.iTemperature==Tempture_Hot ||                                                                                                           //jou 2012-06-29 Index Pick up need wait Soak Time
               LastSet.iTemperature==Tempture_AmbientHot))                                                                                                      //kevin 20180903 (Steven) : add 恆溫控制
            {
                if(Temperature.iShuttleSoakTimeMode==1 &&
                   Temperature.iInitialStart2Time!=0)
                {
                    BLCarryKit.TSoakTime.SetSecAndOn(Temperature.iInitialStart2Time);
                    dwStartShuttle2Soak=MyTickCount();                                                                                                          //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
                    iInitialStart2Count=Temperature.iInitialStart2Time;
                }
            }
            fObserver->RecordInArmTime();
            bDestoryOnSht=false;                                                                                                                                //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            iInArmPickPlaceCnt[InOfsInSh2]++;                                                                                                                   //JerryYang 20180921 Setup Teach功能
            if(USE_IN_Y_IS_AUTO_PITCH==true &&                                                                                                                  //JerryYang 20251218 : IN/OUT ARM支援不同模組
               IniConfig.bE57YPitchHome)                                                                                                                        //kevin 20180822 (Steven) : add put shuttle Y pitch home AutoYPitch
            {
                 hInArmYpitchHomeTimer.SetSecAndOn(10);
                 Task=2600;
                 return false;
            }
            return true;
       case 2600:
            if(bCheckYPitchRunHomeSen(0))
            {
                return true;
            }
            else if(hInArmYpitchHomeTimer.Off())
            {
                InitProcessSingleMotorTask(MInArmPitchY);
                hInArmYpitchHomeTimer.SetSecAndOn(10);
                bYpitchNeddHome=true;
                Task=2610;
            }
            break;
       case 2610:
            if(bCheckYPitchHome(0))
            {
                bYpitchNeddHome=false;
                SetMotorSpeed();
                return true;
            }
            else if(hInArmYpitchHomeTimer.Off())
            {
                ShowErrorMessage("WAR0123", K_RETRY, MInArmPitchY);
                Task=2600;
            }
            break;
    }
    return false;
}
//==============================================================================
void DoInArm_9045_1x2_4_Hot_SuckerMap()
{
    if(TestIF.dSiteXPitch<=iXpitchMaxX2)
    {
        i1x2_4UseACEGPicker=1;                                                  //Steven 20230530 : 1x2_4改用Row A
    }
    else
    {
        i1x2_4UseACEGPicker=0;
    }

    if(TestIF.iTestMode==QualSite2X2N)
    {
        if(i1x2_4UseACEGPicker==1)                                              //Steven 20230530 : 1x2_4改用Row A
        {
            Prod.fInArmSuck4x8[0][0][0]=LastSet.bUseTestSocket[1][0][0];
            Prod.fInArmSuck4x8[0][0][1]=LastSet.bUseTestSocket[0][0][0];
            Prod.fInArmSuck4x8[0][0][2]=LastSet.bUseTestSocket[1][0][1];
            Prod.fInArmSuck4x8[0][0][3]=LastSet.bUseTestSocket[0][0][1];

            Prod.iSiteMap[0][0][0]     =TestIF.iSiteMap[1][0];
            Prod.iSiteMap[0][0][1]     =TestIF.iSiteMap[1][0];
            Prod.iSiteMap[1][0][2]     =TestIF.iSiteMap[0][1];
            Prod.iSiteMap[1][0][3]     =TestIF.iSiteMap[0][1];
        }
        else
        {
            Prod.fInArmSuck4x8[0][0][0]=LastSet.bUseTestSocket[1][0][0];
            Prod.fInArmSuck4x8[0][0][2]=LastSet.bUseTestSocket[1][0][1];
            Prod.fInArmSuck4x8[0][1][0]=LastSet.bUseTestSocket[0][0][0];
            Prod.fInArmSuck4x8[0][1][2]=LastSet.bUseTestSocket[0][0][1];

            Prod.iSiteMap[0][0][0]     =TestIF.iSiteMap[1][0];
            Prod.iSiteMap[0][0][0]     =TestIF.iSiteMap[0][0];
            Prod.iSiteMap[0][0][2]     =TestIF.iSiteMap[1][1];
            Prod.iSiteMap[0][0][2]     =TestIF.iSiteMap[0][1];
        }
    }
    else
    {
        if(i1x2_4UseACEGPicker==1)                                              //Steven 20230530 : 1x2_4改用Row A
        {
            Prod.fInArmSuck4x8[0][0][0]=LastSet.bUseTestSocket[0][0][0];
            Prod.fInArmSuck4x8[0][0][1]=LastSet.bUseTestSocket[1][0][0];
            Prod.fInArmSuck4x8[0][0][2]=LastSet.bUseTestSocket[0][0][1];
            Prod.fInArmSuck4x8[0][0][3]=LastSet.bUseTestSocket[1][0][1];

            Prod.iSiteMap[0][0][0]     =TestIF.iSiteMap[0][0];
            Prod.iSiteMap[0][0][1]     =TestIF.iSiteMap[0][0];
            Prod.iSiteMap[0][0][2]     =TestIF.iSiteMap[0][1];
            Prod.iSiteMap[0][0][3]     =TestIF.iSiteMap[0][1];
            Prod.bInSuckUse[0][0][0]=true;
            Prod.bInSuckUse[0][0][2]=true;
            Prod.bInSuckUse[1][0][0]=true;
            Prod.bInSuckUse[1][0][2]=true;
        }
        else
        {
            Prod.fInArmSuck4x8[0][0][0]=LastSet.bUseTestSocket[0][0][0];
            Prod.fInArmSuck4x8[0][0][2]=LastSet.bUseTestSocket[0][0][1];
            Prod.fInArmSuck4x8[0][1][0]=LastSet.bUseTestSocket[1][0][0];
            Prod.fInArmSuck4x8[0][1][2]=LastSet.bUseTestSocket[1][0][1];

            Prod.iSiteMap[0][0][0]     =TestIF.iSiteMap[0][0];
            Prod.iSiteMap[0][0][0]     =TestIF.iSiteMap[0][0];
            Prod.iSiteMap[0][0][2]     =TestIF.iSiteMap[0][1];
            Prod.iSiteMap[0][0][2]     =TestIF.iSiteMap[0][1];
        }
    }
}
//==============================================================================
void DoInArm_9045_1x2_4_Hot()
{
    int &Task=iArmTask;
    static int iAdjustSpeed=0;
    static bool bOne=false;
    bool flag=false;
    static int iDisplaySpeed=0;                                                 //KaiChen 20171225 (Steven)：Add Speed Display
    int iRow=(i1x2_4UseACEGPicker==1)?1:2;                                      //Steven 20230530 : 1x2_4改用Row A
    int iCol=(i1x2_4UseACEGPicker==1)?4:2;                                      //Steven 20230530 : 1x2_4改用Row A
    bool bCheckCloseSiteHaveIC=false;                                           //kevin 20130115
    AnsiString ErrPart="";
    bool bHasDuplicateErr=false;                                                //kevin 20130115
//    DoJudgeInputShuttleNeedChangeToNullIC();

    if(DoInArmAutoSiteMapping() ||                                              //jou 2011-03-24 start : Auto Site Mapping
       bIndexAlarmInArmAway)                                                    //kevin 20181102 (Steven) :  index回吸檢測有IC inarm 讓位 inarm先不要動
    {
        return ;                                                                //jou 2016-11-03 JCET 要求Auto Site Mapping 需等待測試結果在繼續入料
    }

//    int iShtHP, iKitHP;
    int iSht  =InArmSuck.iWhichSht;
    int iKit  =0;
//    int iKit32=(iKit==1)?4:0;

//    if(LastSet.iTemperature==Tempture_Hot)                                      //Steven 20150626 : Fixed 32 site 吸取偵測異常    //Steven 20180525 : 修正一直跑Hotplate iWhichKit Close
//    {
//        iShtHP=InArmSuck.iWhichShtPickFor32;
//        iKitHP=0;
//    }
//    else
//    {
//        iShtHP=InArmSuck.iWhichSht;
//        iKitHP=0;
//    }
//    int iKitHP32=(iKitHP==0)?0:4;

    switch(Task)
    {
        case 1:
            if(MoveInArm2XYToWait())
            {
                if((iOneCycle && IsInArmOneCycleFinish()) ||
                   (iCleanOut && IsInArmCleanOutFinish()))
                {
                    if(LastSet.iTemperature==Tempture_Hot &&
                       CosFunction.bUseInitialDelayAsSoakTime &&                //Steven 20170511 (wei) : 使用initial delay當 Soak time
                       Temperature.bUseInitialDelayAsSoakTime &&
                       MOT[MMPlate1].HasIC()==false &&
                       MOT[MMPlate2].HasIC()==false)
                    {
                        bOneTimeWait=false;
                    }

                    SetShuttlefCanMoveL(0, true, __FUNC__, "1");
                    SetShuttlefCanMoveL(1, true, __FUNC__, "1");
                    bInArmWaitOneCycle=true;                                    //JerryYang 20220324 : Inarm是否完成ONE CYCLE
                }
                else
                {
                    bInArmWaitOneCycle=false;                                   //JerryYang 20220324 : Inarm是否完成ONE CYCLE
                }
                Task=10;
                bOne=true;
            }

            if(Task!=10)                                                        //Steven 20180813 : add in arm speed
                break;
        case 10:
            if(CheckInArmSuckInitial()==false || bIndexDropVacuumError)         //kevin 20190418 避免 inarm 來回跑
                break;

            if(InArmSuck.HasIC() || bPlaceToHotplate==true)                     //ChungHung 20131010 fix Hotplate 跨排Hangup
            {
                if(bPlaceToHotplate==false &&
                   CheekNeedToDoInArmAdditionalFunction())                      //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
                {
                    Task=200;
                }
                else if(InArmSuck.HasType(HAS_TRY_SUCK_IC) ||                   //ChungHung 20120206 Hotplate check
                        InArmSuck.HasType(HAS_SUCK_IC))
                {
                    if(bInArmTryPickFromHotPlateFinish==false)
                    {
                        InitInArmTryPickFromHotPlateTask100();
                    }
                    else
                    {
                        InitInArmTryPickFromHotPlateTask();
                    }
                    Task=15000;
                }
                else if(InArmSuck.HasType(HAS_NULL_IC) &&
                        InArmSuck.HasType(NULL_IC) &&
                        bPickFromHotplate==true)
                {
                    iHeaterWaitTime=0;
                    if(bPickFormHotplatePartOK==true)
                    {
                        InitInArmPickFromHotPlateTask340();                     //ChungHung 20120502 Hang Up 解除
                    }
                    else
                    {
                        InitInArmPickFromHotPlateTask50();                      //ChungHung 20120112 Hang Up 解除
                    }
                    Task=1500;
                }
                else if(LastSet.iTemperature==Tempture_Ambient ||
                        LastSet.iTemperature==Tempture_AmbientHot ||            //kevin 20141104 恆溫不放hotplate
                        InArmSuck.HasType(HAS_HOT_IC))
                {
                    if(InArmSuck.HasType(HAS_HOT_IC) &&
                       bPickFromHotplate)
                    {
                        iHeaterWaitTime=0;
                        if(bPickFormHotplatePartOK==true)
                        {
                            InitInArmPickFromHotPlateTask340();                 //ChungHung 20120502 Hang Up 解除
                        }
                        else
                        {
                            InitInArmPickFromHotPlateTask50();                  //ChungHung 20120112 Hang Up 解除
                        }
                        Task=1500;
                    }
                    else
                    {
                        InitInArmPlaceToShuttleTask();
                        Task=2000;
                    }
                }
                else
                {
                    if(bPlaceToHotplate==false)
                    {
                        if(HotPlateForm.XDivision==6)
                        {
                            iForPlaceHPX6Step=0;
                        }
                        InitInArmPlaceToHotPlateTask();
                        Task=400;                                               //Steven 20250107 : fixed for hot mode tray end hang up
                    }
                    else
                    {
                        if(bPlaceToHotplatePartOK==true)                        //ChungHung 20120109 add 6x11 HangeUp
                        {
                            InitInArmPlaceToHotPlateTask400();                  //ChungHung 20120502 HangUp 解除
                        }
                        else
                        {
                            InitInArmPlaceToHotPlateTask100();
                        }

                        if(i1x2_4UseACEGPicker==0)                              //Steven 20230530 : 1x2_4改用Row A
                        {                                                       //Steven 20250107 : Move up
                            if(InArmSuck.Item[0][0]==NULL_IC && InArmSuck.Item[0][2]==NULL_IC &&
                               InArmSuck.Item[1][0]==NULL_IC && InArmSuck.Item[1][2]==NULL_IC)
                                Task=50;
                            else
                                Task=1100;
                        }
                        else
                        {
                            if(InArmSuck.Item[0][0]==NULL_IC && InArmSuck.Item[0][1]==NULL_IC &&
                               InArmSuck.Item[0][2]==NULL_IC && InArmSuck.Item[0][3]==NULL_IC)
                                Task=50;
                            else
                                Task=1100;
                        }
                    }
                }
            }
            else
            {
                if(CheekNeedToDoInArmAdditionalFunction())                      //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
                {
                    Task=200;
                }
                else
                {
                    bPlaceToHotplate=false;
                    bPickFromHotplate=false;
                    Task=50;
                }
            }

            if(Task!=50)
                break;
        case 50:
            if(CheckHeaterOK()==false)                                          //Steven 20250116 : 確認HeaterOK
            {
                Task=1;
                break;
            }

            InitArmPickFromLoadStageTask();
            Task=75;                                                            //ChungHung 20120206 Hotplate check

            if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)
            {
                if(NeedWaitTrayArm==true)                                       //wei 20151002
                {
                    Task=1;                                                     //jou 2016-07-14 修正OCR In arm 讓位造成Hotplate疊料issue
                    return;
                }
            }

            if(CosFunction.bRTCAutoModelVerify==true &&                         //jou 2014-06-24 RTC 自動進行Model驗證
               IniConfig.bD36EnableRTCAutoModelVerify==true)
            {
                if(bNeedWaitRTCAutoVerify==true)
                {
                    Task=1;
                    return;
                }
            }

            if(LastSet.iTemperature==Tempture_Hot &&
               (IniConfig.bE39CheckHotPlateAfterCleanOutAndBeforeTrayFeed==true ||
                IniConfig.bO01_ResetNeedClearAndCheckHP) &&                     //ChungHung 20120206 Hotplate check
               bCanUseHotPlateCheck==true &&
               (bNeedTrySuckHotPlate==true || bHPCleanout==true) &&             //wei 20160624 Hotplate clean out
               bOneTimeHotPlateCheckAll==true)                                  //Steven 20120319 : Clean Out後的檢查
            {
                ;
            }
            else
            {
                if(MOT[MMTrayY].HasIC()==false             &&                   // load and load buffer no any tray
                   MOT[MMTrayY].fHasTray==false            &&
                   MOT[MMTrayY_Car].Tray.HasIC()==false    &&
                   MOT[MMTrayY_Car].fHasTray==false)
                {
                    if(iCleanOut && iOneCycle==0 &&                             //ChungHung 20110829 add
                       LastSet.iTemperature==Tempture_Hot &&
                       (MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC()))
                    {
                        DisableAutoSiteMapWhenCleanOut();                       //Steven 20220902 : 修正Auto Site Map, Loader沒有IC
                        if(TestIF_File.UseRotateForHT7000HPKit==false)          //Sam 20210416 : 新增特殊模式 For Rotate Function HT7000 HP Kit
                            iInRotateFinish=3;                                  //wei 20160126 不在去ROTATE(ASEM)
                        Task=500;
                        break;
                    }
                }

                if(IniConfig.bI37_EnableFIFOMode &&
                   LastSet.iRunStartMode==rsmFIFOMode)                          //Sam 20230830 : 修正FIFO跨盤時排序問題
                {
                    if(MOT[MMTrayY].HasIC()==false)
                    {
                        if(InArmSuck.HasIC())
                        {
                            if((iSht==0 && FLCarryKit.UseSiteFullIC()==false) ||
                                iSht==1 && BLCarryKit.UseSiteFullIC()==false)
                            {
                                SetInArmUseSuckToHasNullIC(iSht, iKit);
                                Task=1;
                                break;
                            }
                        }
                    }
                }
            }

            if((MOT[MMTrayY].HasIC()==false &&
                MOT[MMTrayY].fHasTray==true) || bNeedSlapTray)
            {
                Task=1;
                break;
            }

            if(iOneCycle &&                                                     //JerryYang 20250726 : fix clean out in arm loop
               IsInArmOneCycleFinish() &&
               InArmSuck.HasIC()==false)
            {
            }
            else if(iCleanOut &&
                    LastSet.iTemperature==Tempture_Hot &&
                    IsInArmCleanOutFinish()==false)                             //Steven 20230309 : fixed for NN mode hang up
            {
                if(bRunAutoSiteMapping==true &&                                 //Ifor 20210426 add: Auto Site Mapping Clean Out 補料回Hot plate
                   CheckHasSpaceToPlace_9045())
                {
                    if(MOT[MMTrayY].HasIC() || MOT[MMTrayY_Car].fHasTray)
                    {
                        Task=75;
                        break;
                    }
                }

                if((TrayForm.bAutoFeed ||
                    bMustCleanAllTray) &&
                   CheckHasSpaceToPlace_9045())
                {
                    if(MOT[MMTrayY].HasIC() || MOT[MMTrayY_Car].fHasTray)
                    {
                        Task=75;                                                //ChungHung 20120206 Hotplate check
                        break;
                    }
                }

                if((MOT[MMPlate1].HasIC() ||
                    MOT[MMPlate2].HasIC()) &&
                   HasHotReadyIC_9045())
                {
                    Task=500;
                    break;
                }

                if(TrayForm.bAutoFeed==false &&                                 //jou 2010-08-31 start : 2x8 剛好只吸到奇數時,hang up
                   MOT[MMPlate1].HasIC()==false &&
                   MOT[MMPlate2].HasIC()==false &&
                   InArmSuck.HasIC()==false &&
                   (MOT[MInShuttle1].fCanMoveL==false ||
                    MOT[MInShuttle2].fCanMoveL==false))
                {
                    Task=1;
                    break;
                }

                if(TrayForm.bAutoFeed ||
                   (MOT[MMTrayY].fHasTray==false &&
                    MOT[MMTrayY_Car].fHasTray==false) ||
                   bMustCleanAllTray)
                {
                    if(CheckHasSpaceToPlace_9045())
                        Task=75;                                                //ChungHung 20120206 Hotplate check
                    else
                        Task=500;
                }
                else
                {
                    Task=500;
                }
                break;
            }

            if(LastSet.iTemperature==Tempture_Hot &&                            //Ifor 20180507 : add 避免Auto Site Mapping 未補料直接取IC造成資料異常
               CheckHasSpaceToPlace_9045()==false)
            {
                if(LastSet.iRunStartMode==rsmAutoSiteMap)
                {
                    if(bAutoSiteMapHotplateReady==true &&
                       bAutoSiteMapHotplateSave==false)
                    {
                        Task=500;
                    }
                }
                else
                {
                    Task=500;
                }
            }

            if(LastSet.iTemperature==Tempture_Hot &&
               iAdjustSpeed==1 &&
               ArmSpeed[InArm].bAutoSpeed && Task!=500)
            {
                iAdjustSpeed=2;
            }

            if(LastSet.iTemperature==Tempture_Hot &&
               iDisplaySpeed==1 &&
               IniConfig.bA26MotorSpeedSortDisplay==true)
            {
                iDisplaySpeed=2;
                InArmAddSpeedDisplay();
            }

            if((iOneCycle && IsInArmOneCycleFinish()) ||
               (iCleanOut && IsInArmCleanOutFinish()) &&
               InArmSuck.HasIC()==false)
            {
                bInArmWaitOneCycle=true;                                        //JerryYang 20220324 : Inarm是否完成ONE CYCLE
                Task=CheckOneCycleAction(50);                                   //Steven 20240326 : 判斷one cycle的時候要不要繼續放料
                if(Task==100 || Task==500)
                    break;

                if(MoveInArm2XYToWait()==false)
                {
                    Task=50;
                    return;
                }

                bInArmWaitOneCycle=true;                                        //JerryYang 20220324 : Inarm是否完成ONE CYCLE
                if(bOne)
                {
                    bOne=false;

                    SetShuttlefCanMoveL(0, true, __FUNC__, "50");
                    SetShuttlefCanMoveL(1, true, __FUNC__, "50");
                }
                Task=50;
                break;                                                          //JerryYang 20220324 : add
            }
            bInArmWaitOneCycle=false;                                           //JerryYang 20220324 : Inarm是否完成ONE CYCLE

            if(Task==75 &&
               LastSet.iTemperature==Tempture_Hot &&
               CheckHasSpaceToPlace_9045()==false &&                            //ChungHung 20120206 Hotplate check
               LastSet.iRunStartMode!=rsmAutoSiteMap)                           //jou 2016-11-07 JCET fix Auto Site mapping Hot mode hangup
            {
                Task=50;
            }

            if(bBackupCleanOut==true && iOneCycle==1  &&                        //Steven 20240709 : One Cycle沒tray Hang up
               MOT[MMTrayY].HasIC()==false            &&
               MOT[MMTrayY].fHasTray==false           &&
               MOT[MMTrayY_Car].Tray.HasIC()==false   &&
               MOT[MMTrayY_Car].fHasTray==false)
            {
                if(LastSet.iTemperature==Tempture_Hot)
                {
                    Task=CheckOneCycleAction(50);                               //Steven 20240326 : 判斷one cycle的時候要不要繼續放料
                    if(Task==100 || Task==500)
                        break;
                }
            }
            break;
        case 75:                                                                //ChungHung 20120206 Hotplate check start
            if(LastSet.iTemperature==Tempture_Hot &&                            //Steven 20170718 (wei) : HP Check加上保護
               bNeedTrySuckHotPlate &&
               bAutoSiteMapHasPickHP==false)                                    //jou 20231018 : 修正E38與autositemapping衝突造成hotplate疊料問題
            {
                SetInArmUseSuckToHasTrySuckIC(iSht, iKit);
                InitInArmTryPickFromHotPlateTask();
                Task=15000;
            }
            else
            {
                TryHotClearRecBufMMPlate();                                     //ChungHung 20120206 Hotplate check
                InitArmPickFromLoadStageTask();

                if(i1x2_4UseACEGPicker==1)                                      //RogerYang 20250814 修正非加熱模式只使用A排吸嘴，無法Onecycle
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(InArmSuck.Item[1][j]==HAS_NULL_IC)
                        {
                            InArmSuck.SetItemData(1, j, NULL_IC);
                        }
                    }
                }

                Task=100;
            }

            if(Task!=100)                                                       //Steven 20180813 : add in arm speed
                break;
        case 100:
            if(CheckHeaterOK()==false)                                          //Steven 20250116 : 確認HeaterOK
            {
                if(iPickFromLoadStageTask==1)
                {
                    Task=1;
                    break;
                }
            }

            if(CUSTOMER_CODE==CC_ASE_M &&                                       //Ifor 20190529 : add 避免 ASEM 常溫Site Mapoing 完成後多入料一次
               LastSet.iRunStartMode==rsmAutoSiteMap &&
               LastSet.iTemperature!=Tempture_Hot &&
               bASMFinishOneCycle==true && iOneCycle==1)
            {
                Task=1;
                break;
            }

            if(USE_TRAY_MAPPING==etmInstall &&
               TestIF_File.bEnableDeviceRemain==true &&                         //Sam 20191113 : 防止殘料檢與 PickFormLoad 打架
               bDoTrayDeviceCheck)
            {
                break;
            }

            flag=DoInArmPickFromLoadStage_9045();
            if(flag)
            {
                iInArmPickPlaceCnt[InOfsLoader]++;                              //JerryYang 20180921 Setup Teach功能
                bHangTimePause=true;                                            //Steven 20090827 : Hang Up dectector
                if(InArmSuck.HasIC()==false)
                {
                    if(InArmSideAllClose(iSht))                                 //Steven 20230331 : 避免關Site死雞
                    {
                        AdjustShuttleWhichKitOrder();
                    }
                    Task=50;
                }
                else
                {
                    bLoaderNeedVibrate=true;                                    //JerryYang 20191002 loader震動馬達
                    Task=200;
                }
            }

            if(Task!=200)                                                       //Steven 20180813 : add in arm speed
                break;
        case 200:                                                               //kevin 20120115 判斷關site是否吸ic
            CheekNeedToDoInArmAdditionalFunction();                             //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean

            for(int i=0; i<iRow; i++)
            {
                for(int j=0; j<iCol; j++)
                {
                    int j2=(i1x2_4UseACEGPicker==1)?j:j*2;                      //Steven 20230530 : 1x2_4改用Row A
                    if(Prod.fInArmSuck4x8[iWhichShuttle0000][i][j]==false && InArmSuck.Item[i][j2]==HAS_IC)
                    {
                        Task=300;                                               //kevin 20130115
                        return;
                    }
                }
            }
            Task=400;
            break;
        case 300:
            ErrPart=" ";
            bHasDuplicateErr=false;
            bCheckCloseSiteHaveIC=false;                                        //kevin 20130115

            for(int i=0; i<iRow; i++)
            {
                for(int j=0; j<2; j++)
                {
                    int j2=(i1x2_4UseACEGPicker==1)?j:j*2;                      //Steven 20230530 : 1x2_4改用Row A
                    if(Prod.fInArmSuck4x8[iWhichShuttle0000][i][j]==false && InArmSuck.Suck[i][j2].GetStatus())
                    {
                        bHasDuplicateErr=true;
                        bCheckCloseSiteHaveIC=true;
                        ErrPart+=InArmSuck.Suck[i][j2].sName;
                        InArmSuck.SetItemData(i, j2, HAS_NULL_IC);
                    }
                }
            }

            if(bCheckCloseSiteHaveIC)
            {
                ShowErrorMessage("JAM0114", K_RETRY, MInArmX, bHasDuplicateErr, ErrPart);
                bHasDuplicateErr=true;
                break;
            }
            else
            {
                Task=400;
            }

            if(Task!=400)
                break;
        case 400:                                                               //kevin 20130115 判斷關site是否吸ic
            flag=DoInArmAdditionalFunction();                                   //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
            if(flag==true)
            {
                SetInArmUseSuckToHasNullIC(iSht, iKit);
                InitInArmPlaceToHotPlateTask();
                InitInArmPlaceToShuttleTask();
                if(LastSet.iTemperature==Tempture_Hot)
                {
                    Task=1000;                                                  // place to hot plate
                }
                else
                {
                    Task=2000;                                                  // place to shuttle
                }

                if(IniConfig.bUseAutoSiteMapping)                               //jou 2011-03-24 start : Auto Site Mapping
                {
                    if(LastSet.iTemperature==Tempture_Hot &&
                       LastSet.iRunStartMode==rsmAutoSiteMap &&
                       bSiteMappingCHKOK==false)
                    {
                        bOneTimeWait=false;
                        if(IniConfig.bI21AutoSiteMappingUseHotplate==false)     //Ifor 20170919 (Steven) : add Auto Site Mapping Hotplate Mode
                            Task=2000;
                        else
                            Task=1000;
                    }
                }
            }
            break;
        case 500:
            if((MOT[MMPlate1].HasIC() ||                                                                                //Steven 20170511 (wei) : 使用initial delay當 Soak time
                MOT[MMPlate2].HasIC()) &&
               CheckHasSpaceToPlace_9045()==true)                                                                       // 有空間放置
            {
                Task=50;
                iHeaterWaitTime=GetHeaterWaitTime();
                bHangTimePause=true;                                                                                    //Steven 20090827 : Hang Up dectector
            }
            else
            {
                if(HasHotReadyIC_9045())                                                                                //有加熱完成 && 沒有空間放置
                {
                    if(MOT[MMTrayY].HasIC()==false && MOT[MMTrayY].fHasTray==true)                                      //spil 2006_0921
                    {
                        Task=1;
                        break;
                    }
                    iHeaterWaitTime=0;                                                                                  //jou 2012-05-18 先預先在Hotplate上面等待Soak time
                    InitInArmPickFromHotPlateTask();
                    bOneTimeWait=false;

                    if(iDisplaySpeed==1 &&                                                                              //KaiChen 20171225 (Steven)：Add Speed Display
                       IniConfig.bA26MotorSpeedSortDisplay==true)
                    {
                        iDisplaySpeed=2;
                        InArmAddSpeedDisplay();
                    }
                    Task=1500;
                }
                else
                {
                    iHeaterWaitTime=GetHeaterWaitTime();
                    bHangTimePause=true;                                                                                //Steven 20090827 : Hang Up dectector

                    if(iDisplaySpeed==1 &&                                                                              //KaiChen 20171225 (Steven)：Add Speed Display
                       IniConfig.bA26MotorSpeedSortDisplay==true)
                    {
                        iDisplaySpeed=2;
                        InArmSubSpeedDisplay();
                    }
                    Task=600;

                    if((iCleanOut && IsInArmCleanOutFinish()) || iOneCycle)                                             //JerryYang 20230808 : fix換arm預熱功能clean out hang up
                    {
                        bInArmWaitOneCycle=true;
                    }
                }
            }
            break;
        case 600:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=50;
            }
            break;
        case 999:                                                               //Eastsun 20260521 整合
            if(DoInArmIonFanGiveWay())
            {
                Task=1550;
            }
            break;
        case 1000:
            if(LastSet.iTemperature==Tempture_Hot)                                                                      //ChungHung 20141023 add for ATK QA mode after QA Clean out direct send to Shuttle
            {
                if(CheckPlaceToShuttle())                                                                               //Steven 20170511 (wei) : 使用initial delay當 Soak time
                {
                    Task=2000;
                    return;
                }

                if(IniConfig.bQAMode==true && LastSet.iRunStartMode==rsmQAMode)                                         //Steven 20111005 : QA Mode
                {
                    if(iQAModeLoaderCT>Prod.iQAModeCount)                                                               //數量比設定值多，表示已經做完，所以就直接送Bin 1
                    {
                        InitInArmPlaceToShuttleTask();
                        Task=2000;
                        return;
                    }
                }
            }

            Task=1100;
            InitInArmPlaceToHotPlateTask();
//jou 2010-09-20 add in arm speed
//            break;
        case 1100:
            bPlaceToHotplate=true;
            bPickFromHotplate=false;

            if(DoPlaceToHotPlate_9045())
            {
                if(LastSet.iTemperature==Tempture_Hot &&
                   LastSet.iRunStartMode!=rsmAutoSiteMap)                       //Steven 20220527 : for JCET Auto Site Map
                    HPPlaceLog.SetPosition();                                   //Steven 20211110 : 記錄放料到加熱盤的位置
                bPlaceToHotplate=false;
                bHangTimePause=true;                                            //Steven 20090827 : Hang Up dectector
                Task=50;
                iAdjustSpeed=1;
                iDisplaySpeed=1;                                                //KaiChen 20171225 (Steven)：Add Speed Display
                iInRotateFinish=0;                                              //Ifor 20180112 (Steven) : add 避免加熱模式不跑Rotate
                bSiteMappingPlaceHotplate=true;                                 //Ifor 20190308 : add 隨時開關 Site Mapping
            }

            if(Task!=1500)                                                      //Steven 20180813 : add in arm speed
                break;
        case 1500:
            iHeaterWaitTime=0;
            InitInArmPickFromHotPlateTask();
            bOneTimeWait=false;

            if(IniConfig.bA15_1ESDGiveWayFunction==true)                        //==> Eastsun 20260521 整合
            {
                InitDoInArmIonFanGiveWayTask();
                Task=999;
            }
            else
            {
            Task=1550;
            }

            if(Task!=1550)                                                      //Eastsun 20260521 整合
                break;
        case 1550:
            bOneTimeWait=false;
            bPlaceToHotplate=false;

            if(IniConfig.bI23HotTestWaitingMode && bChangeToInitStartMode)      //Chunghung 20111230 Hot Test Waiting Mode
            {
                bChangeToInitStartMode=false;
                ShowMyMessage("Tester Ready?", "", "", true);
            }

            if(DoInArmPickFromHotPlate_9045())
            {
                bPickFromHotplate=false;
                if(InArmSuck.HasIC()==false)
                {
                    if(bRunAutoSiteMapping==true)                               //Ifor 20171128 (Steven) add 避免Auto Site Mapping 多跑一次InArm安全位置
                    {
                        SetShuttlefCanMoveL(0, true, __FUNC__, "1500");
                        SetShuttlefCanMoveL(1, true, __FUNC__, "1500");
                        bOne=true;
                        Task=10;
                    }
                    else
                    {
                        Task=1;
                    }
                    break;
                }
                else                                                            //Steven 20190508 : 改到下面, 避免HP異常hang up一直被重置
                {
                    bHangTimePause=true;                                        //Steven 20090827 : Hang Up dectector
                }

                if(IniConfig.bI28_OnOffSiteOnTheFly==false &&                   //Steven 20230131 : I28與E54衝突
                   IniConfig.bE54CheckCloseSiteNoIC)                            //Steven 20160922 : 因為OneCycle永遠先跑蝦頭1, 檢查加熱盤錯誤功能與ByArmCloseSite衝突
                {
                    for(int i=0; i<iRow; i++)
                    {
                        for(int j=0; j<iCol; j++)
                        {
                            int j2=(i1x2_4UseACEGPicker==1)?j:j*2;              //Steven 20230530 : 1x2_4改用Row A
                            if(Prod.fInArmSuck4x8[0][i][j]==false &&
                               InArmSuck.Item[i][j2]==HAS_HOT_IC)
                            {
                                Task=1600;
                                return;
                            }
                        }
                    }
                }

                InitInArmPlaceToShuttleTask();
                Task=2000;
            }
            break;
        case 1600:
            ErrPart=" ";
            bHasDuplicateErr=false;
            bCheckCloseSiteHaveIC=false;

            for(int i=0; i<iRow; i++)
            {
                for(int j=0; j<iCol; j++)
                {
                    int j2=(i1x2_4UseACEGPicker==1)?j:j*2;                                                              //Steven 20230530 : 1x2_4改用Row A
                    if(Prod.fInArmSuck4x8[0][i][j]==false && InArmSuck.Suck[i][j2].GetStatus())
                    {
                        bCheckCloseSiteHaveIC=true;
                        ErrPart+=InArmSuck.Suck[i][j2].sName;
                        InArmSuck.SetItemData(i, j2, HAS_NULL_IC);
                    }
                }
            }

            if(bCheckCloseSiteHaveIC)
            {
                ShowErrorMessage("JAM0114", K_RETRY, MInArmX, bHasDuplicateErr, ErrPart);                               //Steven 20100104
                bHasDuplicateErr=true;
            }
            else
            {
                InitInArmPlaceToShuttleTask();
                bPickFromHotplate=false;
                Task=2000;
            }
            break;
        case 2000:
            if(CheekNeedToDoInArmAdditionalFunction())                          //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
            {
                Task=200;
                break;
            }
            bPlaceShuttle=true;                                                 //ChungHung 20120226 add
            iInRotateFinish=3;                                                  //kevin 20130524 IC已放置SHUTTLE

            if(DoInArmPlaceToShuttle_9045())
            {
                bPlaceShuttle=false;                                            //ChungHung 20120226 add
                fYieldMonitoring->DoAutoCloseSite(0);                           //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
                iInRotateFinish=0;                                              //kevin 20130524 放完SHUTTLE
                iSetShuttleToHasNullIC=0;
                bHangTimePause=true;                                            //Steven 20090827 : Hang Up dectector
                if(LastSet.iRealDummy==DUMMY && iCleanOut)
                {
                    MOT[MMTrayY].InitNewTray(NULL_IC, false, __FUNC__);
                }

                if(LastSet.iTemperature==Tempture_Hot)
                {
                    if(!bTrySuckHotPlateOneCycle)
                        HasHotReadyIC_9045();                                   //Steven 20220530 : Add

                    if(iOneCycle ||
                       (IniConfig.bUseAutoSiteMapping==true &&
                        LastSet.iRunStartMode==rsmAutoSiteMap &&
                        bSiteMappingCHKOK==false))
                    {
                        Task=50;
                        AdjustShuttlePlaceOrder_AutoSiteMapping();              //jou 2016-11-07 JCET fix Auto Site mapping Hot mode hangup
                    }
                    else
                    {
                        if(bNeedTrySuckHotPlate)                                //ChungHung 20120206 Hotplate check
                        {
                            if(bOneTimeHotPlateCheckAll)
                                Task=75;
                            else
                                Task=50;
                        }
                        else
                        {
                            Task=50;
                        }
                    }
                }
                else if(IniConfig.bA15_1ESDGiveWayFunction==true)               //==> Eastsun 20260521 整合
                {
                    InitDoInArmIonFanGiveWayTask();
                    Task=2100;
                }
                else
                {
                    Task=50;
                }

                if(IniConfig.bUseAutoSiteMapping)                               //jou 2011-03-24 start : Auto Site Mapping
                {
                    if(bSiteMappingCHKOK==false)
                        DoSiteMappingCHK();
                }
            }
            break;
        case 2100:                                                              //Eastsun 20260521 整合
            if(DoInArmIonFanGiveWay())
            {
                Task=50;
            }
            break;
        case 15000:                                                             //ChungHung 20120206 Hotplate check
            if(CheckHeaterOK()==false)                                          //Steven 20250116 : 確認HeaterOK
            {
                if(iInArmTryPickFromHotPlateTask==1)
                {
                    Task=1;
                    break;
                }
            }

            if(DoInArmTryPickFromHotPlate_9045_1x2_4_Hot(bOneTimeHotPlateCheckAll, bTryPickFromHotPlateShowError))
            {
                for(int i=0; i<iRow; i++)
                {
                    for(int j=0; j<iCol; j++)
                    {
                        int j2=(i1x2_4UseACEGPicker==1)?j:j*2;                  //Steven 20230530 : 1x2_4改用Row A
                        if(InArmSuck.Item[i][j2]==HAS_TRY_SUCK_IC)
                            InArmSuck.SetItemData(i, j2, NULL_IC);
                    }
                }

                bHangTimePause=true;                                            //Steven 20090827 : Hang Up dectector
                if(InArmSuck.HasIC()==false)
                {
                    InitArmPickFromLoadStageTask();

                    if(bOneTimeHotPlateCheckAll)
                    {
                        if(bNeedTrySuckHotPlate)
                        {
                            Task=75;
                        }
                        else
                        {
                            Task=50;
                        }
                    }
                    else
                    {
                        Task=100;
                    }
                    break;
                }

                iHeaterWaitTime=0;
                bOneTimeWait=false;
                InitInArmPlaceToShuttleTask();
                Task=2000;
            }
            break;
    }
}
//==============================================================================
//RogerYang 20170120 add  //ChungHung 20120206 Hotplate check start
bool DoInArmTryPickFromHotPlate_9045_1x2_4_Hot(bool bOneTimeCheckAll, bool bShowErrorMessage)
{
    IN_ARM_TRY_PICK_HOTPLATE_1X2_4_Hot:
    int &Task=iInArmTryPickFromHotPlateTask;
    int ip=iPlacePlate[0], ix=iPlacePlateX[0], iy=iPlacePlateY[0];
    int j2;
    AnsiString ErrPart="";
    bool bHasDuplicateErr;
    bool bHasError;
    static bool bSuckFinish[2][4]={{false, false, false, false}, {false, false, false, false}};

    GetHotPlateYHalfPos();

    switch(Task)
    {
        case 1:
            if(bInArmTryPickFromHotPlateFinish)                                 //搜尋目標位置
            {
                BackupPlacePos();                                               //ChungHung 20120206 Hotplate check
            }
            bInArmTryPickFromHotPlateFinish=false;                              //ChungHung 20120206 Hotplate check
            SearchPlateToPlace();
            if(IsHotPlateCheckFinsih())                                         //ChungHung 20120816 Hotplate check add
            {
                InArmSuck.SetAll(NULL_IC);
                Task=400;
                return false;
            }
            Task=100;
        case 100:
            if(MoveInArmXYToHotPlatePlace(iTryPickHP, true))
            {
                InArmSuck.ResetAll();                                           //Steven 20160323 : 避免未開啟真空
                Task=350;
            }
            break;
        case 350:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col/2; j++)
                {
                    if(i==1 && Row2CanPutHP(PlaceMode)==false && iPlaceHPOrder==0)
                    {
                        continue;
                    }
                    j2=j*2;
                    if(InArmSuck.Item[i][j2]==HAS_TRY_SUCK_IC)
                    {
                        ip=iPlacePlate[0];
                        if(bPitchOver12000)
                            ix=iPlacePlateX[0]+j*(HotPlateForm.XDivision/4);
                        else
                            ix=iPlacePlateX[0]+j*(HotPlateForm.XDivision/2);

                        //jou 980326 all row1 no use hand
                        if(((Prod.fInArmSuck4x8[0][0][0]==false && Prod.fInArmSuck4x8[1][0][0]==false) &&
                            (Prod.fInArmSuck4x8[0][0][1]==false && Prod.fInArmSuck4x8[1][0][1]==false))||               //ChungHung 20130910 alter for SCK can close site by Index
                           ((Prod.fInArmSuck4x8[0][1][0]==false && Prod.fInArmSuck4x8[1][1][0]==false) &&
                            (Prod.fInArmSuck4x8[0][1][1]==false && Prod.fInArmSuck4x8[1][1][1]==false)))
                        {
                            iy=iPlacePlateY[0];
                        }
                        else
                        {
                            if(iPlaceHPOrder==0)
                                iy=iPlacePlateY[0]+iYHalf*i;
                            else
                                iy=iPlacePlateY[0];
                        }

                        if(ArmSpeed_File[InArm].bUseHPVacuum)                                                           //Steven 20180125 (Jou) : 加熱盤的真空等待時間
                            InArmSuck.Suck[i][j2].OnDelayTime=ArmSpeed[InArm].dHPVacuumTI*100;
                        if(InArmSuck.Suck[i][j2].Suck() || InArmSuck.Suck[i][j2].Error)
                        {
                            MOT[MMPlate1+ip].SetTraySingleData(ix, iy, HAS_TRY_SUCK_IC);
                            #ifdef SOFT_SIMULTE
                                InArmSuck.SetItemData(i, j2, NULL_IC);
                            #else
                                if(InArmSuck.Suck[i][j2].Error==false && LastSet.iRealDummy==REALLY)
                                {
                                    if(bHPCleanout)                                                                     //wei 20160624 Hotplate clean out
                                    {
                                        if(iHPShuttle==0)
                                            InArmSuck.iWhichShtPickFor32=0;
                                        else
                                            InArmSuck.iWhichShtPickFor32=1;

                                        if(TestIF.iShuttleMode==1)
                                        {
                                            if(TestIF.iShuttle_Sel==0)
                                                InArmSuck.iWhichShtPickFor32=0;
                                            else if(TestIF.iShuttle_Sel==1)
                                                InArmSuck.iWhichShtPickFor32=1;
                                        }
                                    }

                                    InArmSuck.SetItemData(i, j2, HAS_SUCK_IC);
                                }
                                else
                                {
                                    InArmSuck.SetItemData(i, j2, NULL_IC);
                                }
                            #endif
                        }
                    }
                    else if(InArmSuck.Item[i][j2]==HAS_NULL_IC)
                    {
                        ip=iPlacePlate[0];
                        if(bPitchOver12000)
                            ix=iPlacePlateX[0]+j*(HotPlateForm.XDivision/4);
                        else
                            ix=iPlacePlateX[0]+j*(HotPlateForm.XDivision/2);

                        //jou 980326 all row1 no use hand
                        if(((Prod.fInArmSuck4x8[0][0][0]==false && Prod.fInArmSuck4x8[1][0][0]==false) &&
                            (Prod.fInArmSuck4x8[0][0][1]==false && Prod.fInArmSuck4x8[1][0][1]==false))||               //ChungHung 20130910 alter for SCK can close site by Index
                           ((Prod.fInArmSuck4x8[0][1][0]==false && Prod.fInArmSuck4x8[1][1][0]==false) &&
                            (Prod.fInArmSuck4x8[0][1][1]==false && Prod.fInArmSuck4x8[1][1][1]==false)))
                        {
                            iy=iPlacePlateY[0];
                        }
                        else
                        {
                            if(iPlaceHPOrder==0)
                                iy=iPlacePlateY[0]+iYHalf*i;
                            else
                                iy=iPlacePlateY[0];
                        }

                        MOT[MMPlate1+ip].SetTraySingleData(ix, iy, HAS_TRY_SUCK_IC);
                        InArmSuck.SetItemData(i, j2, NULL_IC);
                    }
                }
            }

            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col/2; j++)
                {
                    j2=j*2;
                    if(iPlaceHPOrder==0)
                    {
                        if(Row2CanPutHP(PlaceMode))
                        {
                            if(InArmSuck.Item[i][j2]==HAS_TRY_SUCK_IC)
                                return false;
                        }
                        else
                        {
                            if(InArmSuck.Item[0][j2]==HAS_TRY_SUCK_IC)
                                return false;
                        }
                    }
                    else
                    {
                        if(InArmSuck.Item[1][j2]==HAS_TRY_SUCK_IC)
                            return false;
                    }
                }
            }

            Task=400;
            goto IN_ARM_TRY_PICK_HOTPLATE_1X2_4_Hot;
        case 400:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col/2; j++)
                {
                    if(InArmSuck.Item[i][j*2]==HAS_TRY_SUCK_IC)
                    {
                        Task=1;
                        return false;
                    }
                }
            }

            if(MoveInArmZToPlateSafe(Task))
            {
                //檢查是否有 HAS_SUCK_IC
                //有HAS_SUCK_IC 秀出Error Message
                //將HAS_SUCK_IC 改為HAS_TRY_SUCK_IC
                bHasError=false;
                if(bShowErrorMessage)
                {
                    ErrPart=" ";
                    bHasDuplicateErr=false;

                    for(int i=0; i<2; i++)
                    {
                        for(int j=0; j<4; j++)
                        {
                            InArmSuck.Suck[i][j].Error=false;
                            bSuckFinish[i][j]=false;
                            if(InArmSuck.Item[i][j]==HAS_SUCK_IC)
                            {
                                ErrPart+=InArmSuck.Suck[i][j].sName;
                                bHasError=true;
                            }

                            if(bTryPickHPDuplicateErr[i][j])
                            {
                                bHasDuplicateErr=true;
                            }
                        }
                    }

                    if(bHasError)
                    {
                        ShowErrorMessage("WAR0170", K_RETRY, MInArmX, bHasDuplicateErr, ErrPart);
                        for(int i=0; i<2; i++)
                        {
                            for(int j=0; j<4; j++)
                            {
                                bTryPickHPDuplicateErr[i][j]=true;
                            }
                        }

                        Task=450;
                    }
                    else
                    {
                        for(int i=0; i<2; i++)
                        {
                            for(int j=0; j<4; j++)
                            {
                                bTryPickHPDuplicateErr[i][j]=false;
                            }
                        }

                        Task=500;
                    }
                }
                else
                {
                    if(bOneTimeCheckAll)
                    {
                        if(InArmSuck.HasType(HAS_SUCK_IC))
                        {
                            for(int i=0; i<2; i++)
                            {
                                for(int j=0; j<4; j++)
                                {
                                    if(InArmSuck.Item[i][j]==HAS_SUCK_IC)
                                    {
                                        InArmSuck.SetItemData(i, j, HAS_HOT_IC);
                                    }
                                    else
                                    {
                                        InArmSuck.SetItemData(i, j, HAS_NULL_IC);
                                    }
                                }
                            }
                        }
                    }
                    Task=500;
                }
            }
            break;
        case 450:
            for(int i=0; i<2; i++)
            {
                for(int j=0; j<4; j++)
                {
                    if(bSuckFinish[i][j]==true)
                    {
                        continue;
                    }
                    else
                    {
                        if(ArmSpeed_File[InArm].bUseHPVacuum)                   //Steven 20180125 (Jou) : 加熱盤的真空等待時間
                            InArmSuck.Suck[i][j].OnDelayTime=ArmSpeed[InArm].dHPVacuumTI*100;
                        if((InArmSuck.Suck[i][j].Suck() || InArmSuck.Suck[i][j].Error) && bSuckFinish[i][j]==false)
                        {
                            bSuckFinish[i][j]=true;
                            if(InArmSuck.Suck[i][j].Error==false && LastSet.iRealDummy==REALLY)
                            {
                                InArmSuck.SetItemData(i, j, HAS_SUCK_IC);
                            }
                            else
                            {
                                InArmSuck.SetItemData(i, j, NULL_IC);
                            }
                        }
                    }
                }
            }

            if(bSuckFinish[0][0] && bSuckFinish[0][1] && bSuckFinish[0][2] && bSuckFinish[0][3] &&
               bSuckFinish[1][0] && bSuckFinish[1][1] && bSuckFinish[1][2] && bSuckFinish[1][3])
            {
                if(InArmSuck.HasType(HAS_SUCK_IC))
                {
                    Task=400;
                }
                else
                {
                    TryHotClearRecBufMMPlate();
                    RestorePlacePos();
                    bInArmTryPickFromHotPlateFinish=true;
                    for(int i=0; i<MAX_ARM_Row; i++)
                        for(int j=0; j<MAX_ARM_Col; j++)
                            if(InArmSuck.Item[i][j]==NULL_IC)
                                InArmSuck.SetItemData(i, j, HAS_TRY_SUCK_IC);
                    Task=1;
                }
            }
            break;
        case 500:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col/2; j++)
                {
                    if(InArmSuck.Item[i][j*2+1]==HAS_NULL_IC)
                    {
                        InArmSuck.SetItemData(i, j*2+1, NULL_IC);               //Steven 20140710 : Add
                    }
                }
            }

            if(IsHotPlateCheckFinsih())
            {
                if(bOneTimeCheckAll==true)
                {
                    bNeedTrySuckHotPlate=false;
                    TryHotClearRecBufMMPlate();

                    if(bHotPlateCheckNeedTrayFeed==true)
                    {
                        bTrySuckHotPlateCleanOut=true;
                        iCleanOut=1;
                    }
                    else
                    {
                        bTrySuckHotPlateOneCycle=true;
                        iOneCycle=1;
                    }
                    ResetHotPlateSearchParameter();
                }
                else
                {
                    if(InArmSuck.HasIC()==false)
                    {
                        bNeedTrySuckHotPlate=false;
                    }
                }
            }

            if(bOneTimeCheckAll==false)
                TryHotClearRecBufMMPlate();                                     //ChungHung 20120206 Hotplate check

            RestorePlacePos();                                                  //ChungHung 20120206 Hotplate check
            bInArmTryPickFromHotPlateFinish=true;                               //ChungHung 20120206 Hotplate check

            return true;
    }
    return false;
}
