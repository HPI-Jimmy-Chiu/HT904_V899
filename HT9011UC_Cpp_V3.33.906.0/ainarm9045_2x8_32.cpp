// =============================================================================
//  ainarm9045_2x8_32.cpp  --  In-Arm 2x8 / 32-site variant (DEAD module)
//
//  FAITHFUL TRANSLATION of golden
//    HT9011UC_Code_V3.33.906.0_20260618/ainarm9045_2x8_32.cpp  (7739 ln)
//  Translator: AI(W6.2b-INARM) 20260626
//
//  -------------------------------------------------------------------------
//  WHY THE WHOLE BODY IS GATED (#if 0 // TODO(W7)) -- VERIFIED, NOT ASSUMED
//  -------------------------------------------------------------------------
//  1. The engine never calls these functions.  Golden ainarm9045.cpp routes
//     iInArmType==e9045_2x8_32 to the 2x8_8 family:
//       golden :4639-4641  DoInArm_9045_2x8_32  -> DoInArm_9045_2x8_8()
//       golden :4804-4806  ..._SuckerMap         -> DoInArm_9045_2x8_8_SuckerMap()
//       golden :8829-8831  PlaceToShuttle        -> DoInArmPlaceToShuttle_9045_2x8_8()
//     The C++ target engine mirrors this (target ainarm9045.cpp:914).
//
//  2. The body depends pervasively (41 sites) on Prod.fInArmSuckUse_TwoArm32Site
//     -- a cprod.h member that is ABSENT FROM GOLDEN BY DESIGN (golden cprod.h
//     has only fInArmSuckUse / fInArmSuckUse_9To16 / fInArmSuck4x8).  The W6.2b
//     plan EXPLICITLY forbids adding this symbol ("adding them would be inventing
//     symbols, violating the faithful-translation rule").  Without it the
//     hot-path helpers (ArmRow1Finish_2x8_32 / ArmFinish_2x8_32 /
//     SearchLoadTrayUpDown_9045_2x8_32 / DoArmPickFromLoadStage_9045_2x8_32 /
//     the SuckerMap / the SetArmAndHotPlateXItem*_2x8_32 group / the
//     DoInArm_9045_2x8_32 SM itself) cannot be made ACTIVE.
//
//  3. The golden TU is not even self-consistent: GetNowSiteKitMode_2x8_32 is
//     DEFINED 0-arg (golden :194) but CALLED 2-arg (golden :372/:437/:909);
//     CheckXYPitch_2x8_32 is declared 4-arg in the .h but DEFINED 6-arg
//     (golden :426).  So the golden source does not compile as written -- this
//     is abandoned/forked code superseded by the 2x8_8 family.
//
//  CONCLUSION: translate faithfully (all names / cursor values / switch(Task)
//  structure / formulas / fall-throughs / goto preserved VERBATIM for
//  reverse-lookup) but keep the whole verbatim body behind the gate, and supply
//  ACTIVE offline stubs for the header-declared entry points so the TU LINKS
//  cleanly once registered.  Honest active SM-case coverage for this dead
//  variant is 0 / 116 (the 116 case bodies are present, verbatim, inside #if 0).
//
//  No substrate symbol is added by this file (every gap the 2x8_8 LIVE family
//  needs is added by that variant; this dead file invents nothing).
//
//  ENCODING: golden read via cp950; every Big5 comment preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
//  TRANSLATION MECHANICS: golden had no __fastcall / __property / PACKAGE /
//  new / delete in its bodies; VCL AnsiString is satisfied by vclcompat.
// =============================================================================
#include "ainarm9045_2x8_32.h"

// --- Active include surface (mirrors golden include block, mapped to substrate)
#include "vclcompat/vcl_compat.h"   // AnsiString (1-based) / TStringList / SysUtils

// GOLDEN include block (verbatim, for provenance):
//   #include "MachineDefine.h"  ; #pragma hdrstop
//   #include "ainarm9045_2x8_32.h"
//   #include "aArmHeader.h"      (W6.0 god-header SHIM)
//   #include "MyKitSuck.h" "cprod.h" "mymotor.h" "mycylin.h" "mysensor.h"
//   #include "cSortCT.h" "note.h" "main.h" "csystem.h" "uhome.h" "mymessbox.h"
//   #include "cinitial.h" "atester.h" "aRotateKIT.h" "fRotate.h" "cOffSet.h"
//   #include "cMyDB.h" "cObserver.h" "cmydef.h" "uYieldMonitoring.h" "BarCode.h"
//   #pragma package(smart_init)

// =============================================================================
//  ACTIVE offline stubs for the header-declared entry points.
//  These exist so the TU LINKS (the verbatim golden definitions are inert inside
//  the #if 0 gate below).  Each is offline-safe: it performs no hardware action
//  and returns a value that keeps the (never-reached) engine path inert.
//  Each cites its golden definition site.
// =============================================================================
void DoInArm_9045_2x8_32()                       {}              // golden :6642 (gated body below)
void DoInArm_9045_2x8_32_SuckerMap()             {}              // golden :6595
void InArmZNeedDown_2x8_32(int, bool)            {}              // golden :365
void CheckXYPitch_2x8_32(int *, int *, int, int) {}              // golden :426 (def is 6-arg; .h is 4-arg)
bool InArmSideAllClose_32(int, int)              { return true;  } // golden :33
bool DoPlaceToShuttle_9045_2x8_32()              { return false; } // golden :4610
bool MoveInArmXYPickHotPlate_2x8_32(bool)        { return false; } // golden :1959
bool MoveArmXYToLoaderStage_9045_2x8_32()        { return false; } // golden :1381
bool CleanPad_PlaceToShuttle_2x8_32(int)         { return false; } // golden :7635
int  GetShuttleState_4x8_32(int, bool)           { return 0;     } // golden :7703
bool DoArmPickFromLoadStage_9045_2x8_32()        { return false; } // golden :1394

// =============================================================================
//  VERBATIM GOLDEN BODY (faithful transcription from golden cp950, lines 29..7739)
//  GATED: this dead variant depends on Prod.fInArmSuckUse_TwoArm32Site (absent
//  from golden by design -- must NOT be invented) and is internally
//  signature-inconsistent; the engine never reaches it (routes to 2x8_8).
//  Kept verbatim for reverse-lookup; un-gating is a W7 task that would require
//  the 32-site substrate the golden itself never shipped.
// =============================================================================
#if 0 // TODO(W7): needs Prod.fInArmSuckUse_TwoArm32Site (absent from golden cprod.h by design); engine routes e9045_2x8_32 -> 2x8_8; dead variant
//==============================================================================
bool HasHotReadyIC_9045_2x8_32();
extern bool ShuttleMustFullfor16Site(int iSht);                                 //Steven 20160530 : Add for 16Site full test    //JerryYang 20180814 (Steven) : 新增by Shuttle判斷
//==============================================================================
bool InArmSideAllClose_32(int iSht, int iKit)
{
    iSht=(iSht==0)?2:0;
    iKit=(iKit==0)?0:4;

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(Prod.fInArmSuckUse_TwoArm32Site[i+iSht][j+iKit]==true)
            {
                return false;
            }
        }
    }
    return true;
}
//==============================================================================
void SetShuttleToHasNullIC(int iSht, int iKit)
{
    iKit=(iKit==0)?0:4;

    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(iSht==0)
            {
                if(FLCarryKit.Item[i][j+iKit]==NULL_IC)
                    FLCarryKit.SetItemData(i, j+iKit, HAS_NULL_IC);
            }
            else
            {
                if(BLCarryKit.Item[i][j+iKit]==NULL_IC)
                    BLCarryKit.SetItemData(i, j+iKit, HAS_NULL_IC);
            }
        }
    }
}
//==============================================================================
void iWhichRowAllHASNULLIC_2x8_32(int iSht, int iKit)
{
    iSht=(InArmSuck.iWhichShtPickFor32==0)?2:0;
    iKit=(iInArmiWhichKit==0)?0:4;

    for(int i=0; i<2; i++)
    {
        if(Prod.fInArmSuckUse_TwoArm32Site[i+iSht][0+iKit]==false &&
           Prod.fInArmSuckUse_TwoArm32Site[i+iSht][1+iKit]==false &&
           Prod.fInArmSuckUse_TwoArm32Site[i+iSht][2+iKit]==false &&
           Prod.fInArmSuckUse_TwoArm32Site[i+iSht][3+iKit]==false)
        {
            for(int j=0; j<4; j++)
            {
                if(InArmSuck.Item[i][j]==HAS_NULL_IC)
                    InArmSuck.SetItemData(i, j, NULL_IC);                       //Steven 20140710 : Add
            }
        }
    }
}
//==============================================================================
int GetVariableInHotPlateData_2x8_32(bool bPick)
{
    int iPlateX=0, iPitch=0;
    bPitchOver12000=false;

    if(HotPlateForm.XDivision==4)
    {
        iPitch=HotPlateForm.XPitch*3;
        return iPitch;
    }
    else if(HotPlateForm.XDivision==6)
    {
        if(i8PickerHPMode!=iHPNoUse8suck)                                       //JerryYang 20161007 簡化Hotplate判斷式
        {
            return HotPlateForm.XPitch*3;
        }
        else
        {
            return HotPlateForm.XPitch*3/2*3;
        }
    }
    else if(HotPlateForm.XDivision==8)
    {
        iPitch=HotPlateForm.XPitch*2*3;                                         //吸嘴1跟吸嘴2間，佔了2個HotPlateForm.XPitch
        if(iPitch>(iXpitchMaxX3+300))                                           //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            iPitch=HotPlateForm.XPitch*3;
            bPitchOver12000=true;
        }
        return iPitch;
    }
    else if(HotPlateForm.XDivision==10)
    {
        if(bPick==false)
            iPlateX=iPlacePlateX[0];
        else
            iPlateX=iPickPlateX[0];

        if(iPlateX!=8)
            return HotPlateForm.XPitch*3*2;
        else
            return HotPlateForm.XPitch*3;
    }
    else if(HotPlateForm.XDivision==12)                                         //ChungHung 20150513 add for ATK
    {
        if(bPick==false)
            iPlateX=iPlacePlateX[0];
        else
            iPlateX=iPickPlateX[0];

        if(iPlateX!=8)
        {
            iPitch=HotPlateForm.XPitch*2*3;                                     //Steven 20140624 : HP X-Item=12, X-Pitch=26.66, In Arm Pitch異常修正
            if(iPitch>(iXpitchMaxX3+300))                                       //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
            {
                iPitch=HotPlateForm.XPitch*3;
                bPitchOver12000=true;
            }
            return iPitch;
        }
        else
        {
            bPitchOver12000=true;
            return HotPlateForm.XPitch*3;
        }
    }
    else if(HotPlateForm.XDivision==16)                                         //Steven 20150826 : 16x24 Hot Plate for 32Site
    {
        iPitch=HotPlateForm.XPitch*4*3;                                         //吸嘴1跟吸嘴2間，佔了4個HotPlateForm.XPitch
        if(iPitch>(iXpitchMaxX3+300))                                           //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            iPitch=HotPlateForm.XPitch*3;
            bPitchOver12000=true;
        }
        return iPitch;
    }
    else
    {
        return iXpitchMaxX3;                                                    //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
    }
}
//==============================================================================
void SetShuttleStatus_2x8_32(int SHT, int Row, int Col)
{
    if(XPitchIsStand())
    {
        if(SHT==0)
            FLCarryKit.MoveSuckData(InArmSuck, Row, Col);
        else
            BLCarryKit.MoveSuckData(InArmSuck, Row, Col);
    }
    else
    {
        if(SHT==0)
            FLCarryKit.MoveSuckDataDiff(InArmSuck, Row, Col, Row, XPHNotStander[Col]);
        else
            BLCarryKit.MoveSuckDataDiff(InArmSuck, Row, Col, Row, XPHNotStander[Col]);
    }
}
//==============================================================================
int GetNowSiteKitMode_2x8_32()
{
    int iX=HAS_TESTING_IC;
    if(Zteach->fShow)                                                           //kevin 20210416 add Autoteach Z offset shuttle pick
        iX=NULL_IC;
    else if(bRunAutoClean)                                                      //kevin 20220927  IC >3500 //kevin 20220922//kevin 20121430
        iX=CLEAN_FINISH_IC;

    if(bPlace==true)
    {
        if(TestIF.iAutoClean_Function && bUse8Picker==false && bRunAutoClean)
        {
            if(InArmSuck.iXStep==1)
            {
                return (iShuttleRowKit==1 || iShuttleRowKit==3)?0:1;
            }
            else if(InArmSuck.iXStep==2)
            {
                if(InArmSuck.Item[1][0]>=HAS_IC || InArmSuck.Item[1][2]>=HAS_IC)
                    return (iShuttleRowKit==1 || iShuttleRowKit==3)?10000:10001;
                else                                                            //if(InArmSuck.Item[1][1]>=HAS_IC || InArmSuck.Item[1][3]>=HAS_IC)
                    return (iShuttleRowKit==1 || iShuttleRowKit==3)?10100:10101;
            }
            else                                                                //if(InArmSuck.iXStep==4)
            {
                if(InArmSuck.Item[1][0]>=HAS_IC)
                    return (iShuttleRowKit==1 || iShuttleRowKit==3)?20000:20001;
                else if(InArmSuck.Item[1][1]>=HAS_IC)
                    return (iShuttleRowKit==1 || iShuttleRowKit==3)?20100:20101;
                else if(InArmSuck.Item[1][2]>=HAS_IC)
                    return (iShuttleRowKit==1 || iShuttleRowKit==3)?20200:20201;
                else                                                            //if(InArmSuck.Item[1][3]>=HAS_IC)
                    return (iShuttleRowKit==1 || iShuttleRowKit==3)?20300:20301;
            }
        }
        else
        {
            if(InArmSuck.iXStep==1)
            {
                if(InArmSuck.iYStep==1 && bRunAutoClean==false)
                {
                    return 2;
                }
                else
                {
                    for(int j=0; j<InArmSuck.iPickCol; j++)
                    {
                        if(InArmSuck.Item[0][j]>=HAS_IC)
                            return 0;                                           //A排四顆
                    }
                    return 1;                                                   //B排四顆
                }
            }
            else if(InArmSuck.iXStep==2)
            {
                if(InArmSuck.iYStep==1 && bRunAutoClean==false)
                {
                    if(InArmSuck.Item[0][0]>=HAS_IC || InArmSuck.Item[0][2]>=HAS_IC ||
                       InArmSuck.Item[1][0]>=HAS_IC || InArmSuck.Item[1][2]>=HAS_IC)
                        return 10002;                                           //左邊四顆
                    else                                                        //if(InArmSuck.Item[0][1]>=HAS_IC || InArmSuck.Item[0][3]>=HAS_IC ||
                         //   InArmSuck.Item[1][1]>=HAS_IC || InArmSuck.Item[1][3]>=HAS_IC)
                        return 10102;                                           //右邊四顆
                }
                else
                {
                    if(InArmSuck.Item[0][0]>=HAS_IC || InArmSuck.Item[0][2]>=HAS_IC)
                        return 10000;                                           //左上兩顆
                    else if(InArmSuck.Item[1][0]>=HAS_IC || InArmSuck.Item[1][2]>=HAS_IC)
                        return 10001;                                           //左下兩顆
                    else if(InArmSuck.Item[0][1]>=HAS_IC || InArmSuck.Item[0][3]>=HAS_IC)
                        return 10100;                                           //右上兩顆
                    else                                                        //if(InArmSuck.Item[1][1]>=HAS_IC || InArmSuck.Item[1][3]>=HAS_IC)
                        return 10101;                                           //右下兩顆
                }
            }
            else                                                                //if(InArmSuck.iXStep==4)
            {
                if(InArmSuck.iYStep==1 && bRunAutoClean==false)
                {
                    if(InArmSuck.Item[0][0]>=HAS_IC ||
                       InArmSuck.Item[1][0]>=HAS_IC)
                        return 20002;
                    else if(InArmSuck.Item[0][1]>=HAS_IC ||
                            InArmSuck.Item[1][1]>=HAS_IC)
                        return 20102;
                    else if(InArmSuck.Item[0][2]>=HAS_IC ||
                            InArmSuck.Item[1][2]>=HAS_IC)
                        return 20202;
                    else                                                        //if(InArmSuck.Item[0][3]>=HAS_IC ||
                         //   InArmSuck.Item[1][3]>=HAS_IC)
                        return 20302;
                }
                else
                {
                    if(InArmSuck.Item[0][0]>=HAS_IC)
                        return 20000;
                    else if(InArmSuck.Item[1][0]>=HAS_IC)
                        return 20001;
                    else if(InArmSuck.Item[0][1]>=HAS_IC)
                        return 20100;
                    else if(InArmSuck.Item[1][1]>=HAS_IC)
                        return 20101;
                    else if(InArmSuck.Item[0][2]>=HAS_IC)
                        return 20200;
                    else if(InArmSuck.Item[1][2]>=HAS_IC)
                        return 20201;
                    else if(InArmSuck.Item[0][3]>=HAS_IC)
                        return 20300;
                    else                                                        //if(InArmSuck.Item[1][3]>=HAS_IC)
                        return 20301;
                }
            }
        }
    }
    else
    {
        for(int i=0; i<InArmSuck.iShtRow; i++)
        {
            for(int j=0; j<InArmSuck.iShtCol; j++)
            {
                if(iSht==0)
                    ptrInSHTBackup.SetItemData(i, j, (FLCarryKit.Item[i][j] || FTestSuck.Item[i][j])?iX:0);             //kevin 20220928 change shuttle for autoclean pick data   //ChungHung 20131120 AutoClean use Hotplate1
                else
                    ptrInSHTBackup.SetItemData(i, j, (BLCarryKit.Item[i][j] || BTestSuck.Item[i][j])?iX:0);             //kevin 20220928 change shuttle for autoclean pick data  //ChungHung 20131120 AutoClean use Hotplate1
            }
        }

        int iKit=(ptrInSHTBackup.LeftSideNoIC(InArmSuck.iShtKitStep))?InArmSuck.iShtKitStep:0;                          //Steven 20220906 : 修正2x8 Auto Clean
        if(InArmSuck.iXStep==1)
        {
            for(int j=0; j<InArmSuck.iPickCol; j++)
            {
                if(ptrInSHTBackup.Item[0][j+iKit]==iX)
                    return 0;                                                   //A排四顆
            }
            return 1;                                                           //B排四顆
        }
        else if(InArmSuck.iXStep==2)
        {
            if(ptrInSHTBackup.Item[0][0+iKit]==iX || ptrInSHTBackup.Item[0][1+iKit]==iX)
                return 10000;
            else if(ptrInSHTBackup.Item[1][0+iKit]==iX || ptrInSHTBackup.Item[1][1+iKit]==iX)
                return 10001;
            else if(ptrInSHTBackup.Item[0][2+iKit]==iX || ptrInSHTBackup.Item[0][3+iKit]==iX)
                return 10100;
            else                                                                //if(ptrInSHTBackup.Item[1][2+iKit]==iX || ptrInSHTBackup.Item[1][3+iKit]==iX)
                return 10101;
        }
        else                                                                    //if(InArmSuck.iXStep==4)
        {
            if(ptrInSHTBackup.Item[0][0+iKit]==iX)
                return 20000;
            else if(ptrInSHTBackup.Item[1][0+iKit]==iX)
                return 20001;
            else if(ptrInSHTBackup.Item[0][1+iKit]==iX)
                return 20100;
            else if(ptrInSHTBackup.Item[1][1+iKit]==iX)
                return 20101;
            else if(ptrInSHTBackup.Item[0][2+iKit]==iX)
                return 20200;
            else if(ptrInSHTBackup.Item[0][2+iKit]==iX)
                return 20201;
            else if(ptrInSHTBackup.Item[1][3+iKit]==iX)
                return 20300;
            else                                                                //if(ptrInSHTBackup.Item[1][3+iKit]==iX)
                return 20301;
        }
    }
}
//==============================================================================
void InArmZNeedDown_2x8_32(int iSht, bool bPlace)
{
    if(InitialInArmNeedSuck(iSht, bPlace)==false)
        return;

    int iKit=0;                                                                 //JerryYang 20191122 STM 8 site Auto clean支援前後排一起丟
    int iSuckRow, iShtCol;
    int iMode   =GetNowSiteKitMode_2x8_32(iSht, bPlace);
    int iModeRow=iMode%100;
    int iModeCol=iMode/100;

    if(bPlace)
        iKit=(ptrInSHTBackup.LeftSideNoIC(InArmSuck.iShtKitStep))?0:InArmSuck.iPickKitStep;
    else
        iKit=(ptrInSHTBackup.LeftSideNoIC(InArmSuck.iShtKitStep))?InArmSuck.iPickKitStep:0;

        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                iSuckRow=(TestIF.iAutoClean_Function && bUse8Picker==false && bRunAutoClean)?1:i;
                iShtCol =XPHNotStander_2x8_8[InArmSuck.iModeX][j+iKit];
                if(iModeCol==0 &&                                               //全部一起
                   (iModeRow==2 || iModeRow==i))
                {
                   SetInArmNeedDestory(bPlace, i, iShtCol, iSuckRow, j);
                }
                else if(iModeCol==100 && (j==0 || j==2) &&                      //吸嘴     ==>  蝦頭
                        (iModeRow==2 || iModeRow==i))                           // O X O X      O O X X
                {                                                               // O X O X      O O X X
                    SetInArmNeedDestory(bPlace, i, iShtCol, iSuckRow, j);
                }
                else if(iModeCol==101 && (j==1 || j==3) &&                      //吸嘴     ==>  蝦頭
                        (iModeRow==2 || iModeRow==i))                           // X O X O      X X O O
                {                                                               // X O X O      X X O O
                    SetInArmNeedDestory(bPlace, i, iShtCol, iSuckRow, j);
                }
                else if(iModeCol==200 && j==0 &&                                //吸嘴     ==>  蝦頭
                        (iModeRow==2 || iModeRow==i))                           // O X X X      O X X X
                {                                                               // O X X X      O X X X
                    SetInArmNeedDestory(bPlace, i, iShtCol, iSuckRow, j);
                }
                else if(iModeCol==201 && j==1 &&                                //吸嘴     ==>  蝦頭
                        (iModeRow==2 || iModeRow==i))                           // X O X X      X O X X
                {                                                               // X O X X      X O X X
                   SetInArmNeedDestory(bPlace, i, iShtCol, iSuckRow, j);
                }
                else if(iModeCol==202 && j==2 &&                                //吸嘴     ==>  蝦頭
                        (iModeRow==2 || iModeRow==i))                           // X X O X      X X O X
                {                                                               // X X O X      X X O X
                    SetInArmNeedDestory(bPlace, i, iShtCol, iSuckRow, j);
                }
                else if(iModeCol==203 && j==3 &&                                //吸嘴     ==>  蝦頭
                        (iModeRow==2 || iModeRow==i))                           // X X X O      X X X O
                {                                                               // X X X O      X X X O
                    SetInArmNeedDestory(bPlace, i, iShtCol, iSuckRow, j);
                }
            }
        }
}
//==============================================================================
void CheckXYPitch_2x8_32(int *iX, int *iY, int iSht, bool bPlace, int iMovePitchX, int iMovePitchY)
{
        if(LastSet.iTemperature!=Tempture_Hot)
        {
            iWhichKit32=InArmSuck.iWhichKitPickFor32;
        }
        else
        {
            iWhichKit32=InArmSuck.iWhichKitPickFor32;
        }

    int iMode       =GetNowSiteKitMode_2x8_32(iSht, bPlace);
    int iModeRow    =iMode%100;
    int iModeCol    =iMode/100;
    int iOffsetPos  =GetInArmToShuttleOffset_9045(iSht, iModeRow, iModeCol, false);
    double dMovePitchX;

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

    if(iModeRow==0)                                                             //Row A
    {
        if(bUseTwoArm32Site &&
           TestIF_File.iYPitchOffsetMode==0)                                    //向舊版本相容
        {
            if(USE_Y_AUTO_PITCH==true)
            {
                if(iSht==0)
                {
                    *iY=*iY-iMovePitchY+                                        //A排到中間
                            7000.0-                                             //A排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0);                          //A排移動半個Center Pitch
                }
                else
                {
                    *iY=*iY-iMovePitchY-                                        //A排到中間
                            7000.0+                                             //A排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0)+                          //A排移動半個Center Pitch
                            TestIF.dSiteYPitch;                                 //A排移動一個Y-Pitch
                }
            }
            else
            {
                if(iSht==0)                                                     //Steven 20140512 : For HT-9047
                {
                    *iY+=TestIF.dSiteYOffset;
                    *iY+=TestIF.dSiteYPitch;
                }
                else
                {
                    *iY-=TestIF.dSiteYOffset;
                }
            }
        }
        else if(bUseTwoArm32Site &&
                TestIF_File.iYPitchOffsetMode==2)                               //Steven 20201013 : NN mode add Y-Pitch offset selection
        {
            if(USE_Y_AUTO_PITCH==true)
            {
                if(iSht==0)                                                     //jou 2016-05-05 修正 32 Sites In & Out Arm 吸放點位異常
                {
                    *iY=*iY-iMovePitchY+                                        //A排到中間
                            7000.0-                                             //A排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0);                          //A排移動半個Center Pitch
                }
                else
                {
                    *iY=*iY-iMovePitchY-                                        //A排到中間
                            7000.0+                                             //A排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0)+                          //A排移動半個Center Pitch
                            TestIF.dSiteYPitch;                                 //A排移動一個Y-Pitch
                }
            }
            else
            {
                if(iSht==0)
                {
                    *iY=*iY+7000.0-                                             //A排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0);                          //A排移動半個Center Pitch
                }
                else
                {
                    *iY=*iY-7000.0+                                             //A排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0)+                          //A排移動半個Center Pitch
                            TestIF.dSiteYPitch;                                 //A排移動一個Y-Pitch
                }
            }
        }
        else
        {
            if(USE_Y_AUTO_PITCH==true)
                *iY=*iY-iMovePitchY+TestIF.dSiteYPitch/2;
            else
                *iY=*iY+TestIF.dSiteYPitch/2;
        }
    }
    else if(iModeRow==1)                                                        //Row B
    {
        if(bUseTwoArm32Site &&
           TestIF_File.iYPitchOffsetMode==0)                                    //向舊版本相容
        {
            if(USE_Y_AUTO_PITCH==true)
            {
                if(iSht==0)
                {
                    *iY=*iY+7000.0-                                             //B排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0)-                          //B排移動半個Center Pitch
                            TestIF.dSiteYPitch;                                 //B排移動到Row B
                }
                else
                {
                    *iY=*iY-7000.0+                                             //B排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0);                          //B排移動半個Center Pitch
                }
            }
            else
            {
                if(iSht==0)                                                     //Steven 20140512 : For HT-9047
                {
                    *iY+=TestIF.dSiteYOffset;
                    *iY+=TestIF.dSiteYPitch;
                }
                else
                {
                    *iY-=TestIF.dSiteYOffset;
                }
            }
        }
        else if(bUseTwoArm32Site &&
                TestIF_File.iYPitchOffsetMode==2)                               //Steven 20201013 : NN mode add Y-Pitch offset selection
        {
            if(USE_Y_AUTO_PITCH==true)
            {
                if(iSht==0)
                {
                    *iY=*iY+7000.0-                                             //B排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0)-                          //B排移動半個Center Pitch
                            TestIF.dSiteYPitch;                                 //B排移動到Row B
                }
                else
                {
                    *iY=*iY-7000.0+                                             //B排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0);                          //B排移動半個Center Pitch
                }
            }
            else
            {
                if(iSht==0)
                {
                    *iY=*iY+iMovePitchY+                                        //B排到中間
                           +7000.0-                                             //B排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0)-                          //B排移動半個Center Pitch
                            TestIF.dSiteYPitch;                                 //B排移動到Row B
                }
                else
                {
                    *iY=*iY+iMovePitchY-                                        //B排到中間
                            7000.0+                                             //B排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0);                          //B排移動半個Center Pitch
                }
            }
        }
        else
        {
            if(USE_Y_AUTO_PITCH==true)
                *iY=*iY-TestIF.dSiteYPitch/2;
            else
                *iY=*iY+iMovePitchY-TestIF.dSiteYPitch/2;
        }
    }
    else                                                                        //Both
    {
        if(bUseTwoArm32Site &&
           TestIF_File.iYPitchOffsetMode==0)                                    //向舊版本相容
        {
            if(USE_Y_AUTO_PITCH==true)
            {
                if(iSht==0)
                {
                    *iY=*iY-iMovePitchY+                                        //A排到中間
                            7000.0-                                             //A排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0);                          //A排移動半個Center Pitch
                }
                else
                {
                    *iY=*iY-iMovePitchY-                                        //A排到中間
                            7000.0+                                             //A排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0)+                          //A排移動半個Center Pitch
                            TestIF.dSiteYPitch;                                 //A排移動一個Y-Pitch
                }
            }
            else
            {
                if(iSht==0)                                                     //Steven 20140512 : For HT-9047
                {
                    *iY+=TestIF.dSiteYOffset;
                    *iY+=TestIF.dSiteYPitch;
                }
                else
                {
                    *iY-=TestIF.dSiteYOffset;
                }
            }
        }
        else if(bUseTwoArm32Site &&
                TestIF_File.iYPitchOffsetMode==2)                               //Steven 20201013 : NN mode add Y-Pitch offset selection
        {
            if(USE_Y_AUTO_PITCH==true)
            {
                if(iSht==0)                                                     //jou 2016-05-05 修正 32 Sites In & Out Arm 吸放點位異常
                {
                    *iY=*iY-iMovePitchY+                                        //A排到中間
                            7000.0-                                             //A排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0);                          //A排移動半個Center Pitch
                }
                else
                {
                    *iY=*iY-iMovePitchY-                                        //A排到中間
                            7000.0+                                             //A排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0)+                          //A排移動半個Center Pitch
                            TestIF.dSiteYPitch;                                 //A排移動一個Y-Pitch
                }
            }
            else
            {
                if(iSht==0)
                {
                    *iY=*iY+7000.0-                                             //A排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0);                          //A排移動半個Center Pitch
                }
                else
                {
                    *iY=*iY-7000.0+                                             //A排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0)+                          //A排移動半個Center Pitch
                            TestIF.dSiteYPitch;                                 //A排移動一個Y-Pitch
                }
            }
        }
        else
        {
            if(USE_Y_AUTO_PITCH==true)
                *iY=*iY-TestIF.dSiteYPitch/2;
            else
                *iY=*iY+TestIF.dSiteYPitch/2;
        }
    }

    if(USE_PICKER_COUNT==ep16Picker)                                            //基準為第四隻吸嘴
    {
        dMovePitchX=double(iMovePitchX)/7.0;
        /*if(iModeCol==0)                                                       //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==101)                                                  //Ac --> Ac
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==200)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==201)                                                  //Ac --> Ab
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*2.5);
        }
        else if(iModeCol==202)                                                  //Ae --> Ac
        {
            *iX=*iX+(-dMovePitchX*1.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==203)                                                  //Ag --> Ad
        {
            *iX=*iX+(-dMovePitchX*3.0-TestIF.dSiteXPitch*0.5);
        }*/
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchBb)                                 //基準為第二隻吸嘴 //Steven for HT7080
    {
        dMovePitchX=double(iMovePitchX)/3.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==101)                                                  //Ab --> Ac
        {
            *iX=*iX+(dMovePitchX*0.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==200)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==201)                                                  //Ab --> Ab
        {
            *iX=*iX+(dMovePitchX*0.0-TestIF.dSiteXPitch*2.5);
        }
        else if(iModeCol==202)                                                  //Ac --> Ac
        {
            *iX=*iX+(-dMovePitchX*1.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==203)                                                  //Ad --> Ad
        {
            *iX=*iX+(-dMovePitchX*2.0-TestIF.dSiteXPitch*0.5);
        }
    }
    else                                                                        //基準為第三隻吸嘴
    {
        dMovePitchX=double(iMovePitchX)/3.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==101)                                                  //Ab --> Ac
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==200)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==201)                                                  //Ab --> Ab
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*2.5);
        }
        else if(iModeCol==202)                                                  //Ac --> Ac
        {
            *iX=*iX+(dMovePitchX*0.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==203)                                                  //Ad --> Ad
        {
            *iX=*iX+(-dMovePitchX*1.0-TestIF.dSiteXPitch*0.5);
        }
    }

    *iY=*iY+GetInArmYToShuttleOffset_9045(iSht, iOffsetPos);
    *iX=*iX+GetInArmXToShuttleOffset_9045(iSht, iOffsetPos);
    TransferInShuttleRatio(iSht, iX, iY, iInArmYBase, iInArmXBase);
    InArmZNeedDown_9045(iSht, bPlace);
}
//==============================================================================
void GetHPStatus_2x8_32(bool *flag)
{
    int j2, iStepX=0;
    int iXItem10Step=0;

    for(int j=0; j<4; j++)
    {
        flag[j*2]=false;
        flag[1+j*2]=false;
    }

    if(HotPlateForm.XDivision==6)
    {
        if(i8PickerHPMode!=iHPNoUse8suck)                                       //JerryYang 20161007 簡化Hotplate判斷式
        {
            if(iPlacePlateX[0]==4)
                iStepX=2;
            else
                iStepX=4;
        }
        else
        {
            iStepX=2;
        }
    }
    else if(HotPlateForm.XDivision==10)
    {
        if(iPlacePlateX[0]==8)
            iStepX=2;
        else
            iStepX=4;
    }
    else                                                                        //item = 4 or 8
    {
        iStepX=4;
    }

    for(int j=0; j<iStepX; j++)
    {
        if(iPlaceHPOrder==0)
        {
            if(HotPlateForm.XDivision==6)
            {
                if(i8PickerHPMode!=iHPNoUse8suck)                               //JerryYang 20161007 簡化Hotplate判斷式
                {
                    j2=j+iForPlaceHPX6Step*2;
                    if(InArmSuck.Item[0][j2]!=NULL_IC && InArmSuck.Item[0][j2]!=HAS_NULL_IC)
                        flag[j*2+iForPlaceHPX6Step*4]=true;
                    if(Row2CanPutHP(PlaceMode))
                    {
                        if(InArmSuck.Item[1][j2]!=NULL_IC && InArmSuck.Item[1][j2]!=HAS_NULL_IC)
                            flag[1+j*2+iForPlaceHPX6Step*4]=true;
                    }
                }
                else
                {
                    j2=j*2+iForPlaceHPX6Step;
                    if(InArmSuck.Item[0][j2]!=NULL_IC && InArmSuck.Item[0][j2]!=HAS_NULL_IC)
                        flag[j*4+iForPlaceHPX6Step*2]=true;
                    if(Row2CanPutHP(PlaceMode))
                    {
                        if(InArmSuck.Item[1][j2]!=NULL_IC && InArmSuck.Item[1][j2]!=HAS_NULL_IC)
                            flag[1+j*4+iForPlaceHPX6Step*2]=true;
                    }
                }
            }
            else if(HotPlateForm.XDivision==10)
            {
                iXItem10Step=0;
                if(iPlacePlateX[0]==8 && InArmLeftSideNoIC())
                    iXItem10Step=4;

                j2=j+iXItem10Step/2;
                if(InArmSuck.Item[0][j2]!=NULL_IC && InArmSuck.Item[0][j2]!=HAS_NULL_IC)
                    flag[j*2+iXItem10Step]=true;
                if(Row2CanPutHP(PlaceMode))
                {
                    if(InArmSuck.Item[1][j2]!=NULL_IC && InArmSuck.Item[1][j2]!=HAS_NULL_IC)
                        flag[1+j*2+iXItem10Step]=true;
                }
            }
            else                                                                //item = 4 or 8
            {
                if(InArmSuck.Item[0][j]!=NULL_IC && InArmSuck.Item[0][j]!=HAS_NULL_IC)
                    flag[j*2]=true;
                if(Row2CanPutHP(PlaceMode))
                {
                    if(InArmSuck.Item[1][j]!=NULL_IC && InArmSuck.Item[1][j]!=HAS_NULL_IC)
                        flag[1+j*2]=true;
                }
            }
        }
        else
        {
            if(HotPlateForm.XDivision==6)
            {
                //jou 2012-01-09 IniConfig.bHotPlateMove1CM=true,Hotplate 6x11吸嘴不會下去丟
                if(i8PickerHPMode!=iHPNoUse8suck)                               //JerryYang 20161007 簡化Hotplate判斷式
                {
                    j2=j+iForPlaceHPX6Step*2;
                    if(InArmSuck.Item[1][j2]!=NULL_IC && InArmSuck.Item[1][j2]!=HAS_NULL_IC)
                        flag[1+j*2+iForPlaceHPX6Step*4]=true;
                }
                else
                {
                    j2=j*2+iForPlaceHPX6Step;
                    if(InArmSuck.Item[1][j2]!=NULL_IC && InArmSuck.Item[1][j2]!=HAS_NULL_IC)
                        flag[1+j*4+iForPlaceHPX6Step*2]=true;
                }
            }
            else                                                                //item = 4 or 8
            {
                if(InArmSuck.Item[1][j]!=NULL_IC && InArmSuck.Item[1][j]!=HAS_NULL_IC)
                    flag[1+j*2]=true;
            }
        }
    }
}
//==============================================================================
// in arm x y to shuttle position
//==============================================================================
bool MoveInArm2XYToShuttle_9045_2x8_32(int iSht, int iKit, bool IncludeZ, bool bPlace)
{
    int iXPos               =iInArmShtXCenterPos;                               //Steven 20141029 : XY-Pitch for Shuttle Center position for base Suck
    int iYPos               =iInArmShtYCenterPos;
    int iMode               =GetNowSiteKitMode_2x8_32(iSht, bPlace);
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
            iMovePitchX=TestIF.dSiteXPitch/2.0*7.0;
        else if(InArmSuck.iXStep==2)
            iMovePitchX=TestIF.dSiteXPitch/4.0*7.0;                             //四隻吸嘴佔一個site pitch, 所以每隻吸嘴是半個site pitch
        else
            iMovePitchX=iXpitchMaxX7;
    }
    else
    {
        if(InArmSuck.iXStep==1)
            iMovePitchX=TestIF.dSiteXPitch*3.0;
        else if(InArmSuck.iXStep==2)
            iMovePitchX=TestIF.dSiteXPitch/2.0*3.0;                             //兩隻吸嘴佔一個site pitch, 所以每隻吸嘴是半個site pitch
        else
            iMovePitchX=iXpitchMaxX3;
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iInXPToSht[i]=GetInArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    if(iKit!=0)                                                                 //JerryYang 20190729 STM 8 site交錯模式
    {
        iXPos+=TestIF.dSiteXPitch*4.0;
    }

    CheckXYPitch_2x8_32(&iXPos, &iYPos, iSht, bPlace, iMovePitchX, iMovePitchY);
    GetInArmZShtDownPos_9045(iSht, bPlace, IncludeZ);

    if(bRunAutoClean && TestIF.iAutoClean_Function)
    {
        if(bUse8Picker==false)                                                  //Steven 20201014 : 整合8吸嘴auto clean
        {
            if(iShuttleRowKit==1 || iShuttleRowKit==3)
                iYPos+=iMovePitchY;
        }

        if(TestIF_File.iAutoClean_Tray==eCKPos_HP2)                             //kevin 20170727 for AutoClean
        {
            iXPos+=iArmXShuttle1OffsetPos;                                      //Steven 20141029 : XY-Pitch for Shuttle Center position for base Suck
            iYPos+=iArmYShuttle1OffsetPos;
        }
    }

    if(InArmContinuousMove_9045(iXPos, iYPos, iInXPToSht, iYVariable, bZFlgToSht, iZPosToSht, IncludeZ))
    {
        ResetInToShtFlag();
        bCheckZSafe=false;
        return true;
    }
    return false;
}
// =============================================================================
bool DoMoveArmXYToLoaderStage_9045_2x8_32(bool &bCanPick2ICAtOnceTime, int iSelRow, int &ix, int &iy, int &iStep, bool realMove, bool ZNeedDown=true)           //kevin 20210305 add  ZNeedDown define true
{
    int Xpos=0, Ypos=0, Ppos=6000, i, j;
    int iX, iY;
    int iXVariable =0;
    int iXVariable2=0;                                                          //Steven 20131002 : XY變距
    int iYVariable =0;                                                          //ChungHung 20131231 alter AutoYPitch
    int iMovePitchX=0;
    int iMovePitchY=TestIF.iARM_Y_PITCH;
    bool ZDownFlag[8]={false, false, false, false, false, false, false, false};
    int ZDownPos[8]={0, 0, 0, 0, 0, 0, 0, 0};

    if(iSelRow==2)                                                              //ChungHung 20131231 alter AutoYPitch
    {
        iSelRow=0;
        if(bCanPick2ICAtOnceTime==false)                                        //Steven 20141029 : XY-Pitch with Fix Pitch mode
        {
            for(j=0; j<MAX_ARM_Col; j++)
                InArmSuckUse[1][j]=false;
        }
    }

    if(bCanPick2ICAtOnceTime==true)
    {
        if(iSelRow==1)
        {
//            Xpos=Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+(ix+iStep*2)*Prod.LoadForm.iXPitch-InArmOffSet[InOfsLoader]->GetX()+InArmOffSet[InOfsLoaderB]->GetX();
//            Ypos=Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]-iy*Prod.LoadForm.iYPitch-InArmOffSet[InOfsLoader]->GetY()+InArmOffSet[InOfsLoaderB]->GetY();
            if(USE_Y_AUTO_PITCH==true)                                          //Steven 20160203
            {
                Xpos=Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+(ix+iStep*2)*Prod.LoadForm.iXPitch;
                Ypos=Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]-iy*Prod.LoadForm.iYPitch;
            }
            else
            {
                Xpos=Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+(ix+iStep*2)*Prod.LoadForm.iXPitch+InArmOffSet[InOfsLoaderB]->GetX();
                Ypos=Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]-iy*Prod.LoadForm.iYPitch+InArmOffSet[InOfsLoaderB]->GetY();
            }
        }
        else
        {
            Xpos=Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+(ix+iStep*2)*Prod.LoadForm.iXPitch;
            Ypos=Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]-iy*Prod.LoadForm.iYPitch;
        }

        Ppos=InArmClose_Pitch*3;
        iXPosition[0]=ix;
        iXPosition[1]=ix+iStep;
        iXPosition[2]=ix+2*iStep;
        iXPosition[3]=ix+3*iStep;
        iYPosition=iy;
    }
    else                                                                        //one by one suck
    {
        for(i=0; i<4; i++)
        {
            if(InArmSuckUse[iSelRow][i]==true)
            {
                for(j=(i+1); j<MAX_ARM_Col; j++)
                    InArmSuckUse[iSelRow][j]=false;
                Xpos=Prod.XInArm_Tray_Pick[iSelRow][i]+ix*Prod.LoadForm.iXPitch+(2-i)*iXpitchMaxX3/3;                   //Isaac 20171204 (Steven) : Xpitch40->50mm, KIT_PITCH:12000->iXpitchMaxX3
                if(Xpos>(MOT[MInArmX].Motor->PSoftLimitP-100))
                {
                    if(Prod.LoadForm.iXPitch<=iXpitchMax && Prod.LoadForm.iXPitch>=iXpitchMin)                          //Steven 20140520 : 大IC使用Fix時,吸嘴B吸Loader最右側可能會Close Pitch導致飛料
                    {
                        Ppos=Prod.LoadForm.iXPitch*3;
                    }
                    else if(Prod.LoadForm.iXPitch*2<=iXpitchMax && Prod.LoadForm.iXPitch*2>=iXpitchMin)                 //Steven 20230508 : 針對小IC縮pitch
                    {
                        Ppos=Prod.LoadForm.iXPitch*2*3;
                    }
                    else if(Prod.LoadForm.iXPitch*3<=iXpitchMax && Prod.LoadForm.iXPitch*3>=iXpitchMin)
                    {
                        Ppos=Prod.LoadForm.iXPitch*3*3;
                    }
                    else
                    {
                        Ppos=iXpitchMaxX3;
                    }

                    Xpos=Prod.XInArm_Tray_Pick[iSelRow][i]+ix*Prod.LoadForm.iXPitch+(2-i)*Ppos/3;                       // close pitch
                }
                else
                {
                    Ppos=iXpitchMaxX3;                                          //Isaac 20171204 (Steven) : Xpitch40->50mm, KIT_PITCH:12000->iXpitchMaxX3
                }
                Ypos=Prod.YInArm_Tray_Pick[iSelRow][i]-iy*Prod.LoadForm.iYPitch;
                iXPosition[i]=ix;
                iYPosition=iy;
                break;
            }
        }
    }

    if(realMove==false)
    {
        if(Xpos>=MOT[MInArmX].Motor->PSoftLimitP || Xpos<=MOT[MInArmX].Motor->PSoftLimitN)
            return false;
        if(Ypos>=MOT[MInArmY].Motor->PSoftLimitP || Ypos<=MOT[MInArmY].Motor->PSoftLimitN)
            return false;
        return true;
    }
    /*
    for(i=0; i<MAX_ARM_Row; i++)
    {
        for(j=0; j<MAX_ARM_Col; j++)
        {
            ZDownFlag[i+j*2]=InArmSuckUse[i][j];
            ZDownPos[i+j*2]=Prod.ZInArm_Tray_Pick[i][j];
        }
    } */

    DoZdownflag(ZNeedDown, &ZDownFlag[0], &ZDownPos[0]);                        //kevin 20210305

    iMovePitchX=Ppos;
    iMovePitchY=AutoCalculateInArmYClosePitch();

    iX=Xpos+Prod.iINARM_X_PITCH*iInArmOrder;
    if(USE_Y_AUTO_PITCH==true)                                                  //ChungHung 20140304 add for AutoYPitch
    {
        if(iInArmOrder==0)
            iY=Ypos-iMovePitchY;
        else
            iY=Ypos;
    }
    else
    {
        iY=Ypos+Prod.iINARM_Y_PITCH*iInArmOrder;
    }

    iXVariable =GetInArmPitch_9045(iMovePitchX);
    iYVariable =GetInArmPitchY_9045(iMovePitchY);                               //ChungHung 20131231 alter AutoYPitch
    iXVariable2=GetInArmPitch2_9045(iMovePitchX);

    //jou 980512 add loader pitch offset
    if(USE_IN_OUT_ARM_Y_PITCH!=iXYPitchVariable && iSelRow==1 &&                //Steven 20151103 : Fixed for 32Site YPitch
       TestIF.bEnableAutoAlignment==false)                                      //KenHsieh 20220923 : add AOA功能開啟不補償
    {
        iXVariable +=InArmOffSet[InOfsLoaderB]->GetVariable();
        iYVariable +=InArmOffSet[InOfsLoaderB]->GetVariableY();
        iXVariable2+=InArmOffSet[InOfsLoaderB]->GetVariable2();
    }
    else
    {
        iXVariable +=InArmOffSet[InOfsLoader]->GetVariable();
        iYVariable +=InArmOffSet[InOfsLoader]->GetVariableY();
        iXVariable2+=InArmOffSet[InOfsLoader]->GetVariable2();
    }

    if(iY<(MOT[MInArmY].Motor->PSoftLimitN+10))                                 //JerryYang 20180221 (Steven) : fix y-pitch 最小25mm會有行程的問題, y-pitch要縮到最小
    {
        if(USE_Y_AUTO_PITCH==true)
        {
            if(iInArmOrder==0)
            {
                iY=iY+iMovePitchY;
                iMovePitchY=IN_OUT_ARM_Y_PITCH_MIN;
                iYVariable =GetInArmPitchY_9045(iMovePitchY);
                iY=iY-iMovePitchY;
                if(IniConfig.bE46_LoaderUse2Offset && iSelRow==1 &&             //Steven 20140827 : Loader可以分別調前後排的Offset
                   TestIF.bEnableAutoAlignment==false)                          //KenHsieh 20220923 : add AOA功能開啟不補償
                {
                    iYVariable+=InArmOffSet[InOfsLoaderB]->GetVariableY();
                }
                else
                {
                    iYVariable+=InArmOffSet[InOfsLoader]->GetVariableY();
                }
            }
        }
    }

    TransferLoaderRatio(&iX, &iY);                                              //Steven 20141029 : Loader的軟體齒輪比

    DoInArmSuckPreOn(iX, iY);

    if(InArmContinuousMove(iX, iY, iXVariable, &ZDownFlag[0], &ZDownPos[0], ZAxisDown, iYVariable, iXVariable2, true))  //ChungHung 20131231 alter AutoYPitch  //JerryYang 20190729 二段速功能可選擇only at loader
    {
        if(bRecordUPH && ix<=0 && iy==0)
        {
            bRecordUPH=false;
            CalculateUPH(false);
        }
        return true;
    }
    return false;
}
// =============================================================================
bool ArmRow1Finish_2x8_32()
{
    int iSht, iKit;

    iSht=(InArmSuck.iWhichShtPickFor32==0)?2:0;
    iKit=(iInArmiWhichKit==0)?0:4;

    for(int i=0; i<MAX_ARM_Col; i++)
    {
        if(Prod.fInArmSuckUse_TwoArm32Site[0+iSht][i+iKit])
        {
            if(InArmSuck.Item[0][i]==NULL_IC)
                return false;
        }
    }
    return true;
}
// =============================================================================
bool ArmRow1FinishPickFromHotPlate_2x8_32()
{
    int iSht=(InArmSuck.iWhichShtPickFor32==0)?2:0;
    int iKit=(InArmSuck.iWhichKitPickFor32)?0:4;

    for(int i=0; i<MAX_ARM_Col; i++)
    {
        if(IniConfig.bI28_OnOffSiteOnTheFly ||                                  //Steven 20150924 : 隨時開關Site功能
           Prod.fInArmSuckUse_TwoArm32Site[0+iSht][i+iKit])
        {
            if(InArmSuck.Item[0][i]==NULL_IC)
                return false;
        }
    }
    return true;
}
// =============================================================================
bool ArmRow2Finish_2x8_32()
{
    for(int i=0; i<MAX_ARM_Col; i++)
    {
        //Ifor 20171003 (Steven) : Fix Auto Site Mapping Use Hotplat hang up
        //==>
        if(IniConfig.bI21EnableASM==true && IniConfig.bI21AutoSiteMappingUseHotplate==true && bSiteMappingCHKOK==false)
        {
            if(InArmSuck.Item[1][i]==NULL_IC  &&
               Prod.fInArmSuckUse[InArmSuck.iWhichShtPickFor32][1][i]==true)
            {
                return false;
            }
        }
        else
        {
            if(InArmSuck.Item[1][i]==NULL_IC)
            {
                return false;
            }
        }
        //<==
        //Ifor 20171003 (Steven) : Fix Auto Site Mapping Use Hotplat hang up
    }
    iPickHPOrder=0;
    return true;
}
// =============================================================================
bool ArmFinish_2x8_32()                                                         //check Arm finish and  can go shuttle
{
    int iSht=(InArmSuck.iWhichShtPickFor32==0)?2:0;
    int iKit=(iInArmiWhichKit==0)?0:4;

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(InArmSuckUse[i][j] && InArmSuck.Item[i][j]==NULL_IC)
            {
                if(iCleanOut &&
                   MOT[MMTrayY].HasIC()==false &&
                   MOT[MMTrayY_Car].fHasTray==false)
                    return true;
                else
                    return false;
            }
            else
            {
                if(Prod.fInArmSuckUse_TwoArm32Site[i+iSht][j+iKit] &&
                   InArmSuck.Item[i][j]==NULL_IC)
                    return false;
            }
        }
    }
    return true;
}
// =============================================================================
bool SearchLoadTrayUpDown_9045_2x8_32(bool &bCanPick2ICAtOnceTime, int &iStep, int &ix, int &iy, int &NowRow)
{
    bool flag[2][4], CanMove=false;
    int pitchstep=0, isStart=0, iX, iY;
    AdjustInArmClosePitchCondition(bCanPick2ICAtOnceTime, iStep);
    int ArmMaxSuck=4;
    int iRowCT=0;

    int iSht=(InArmSuck.iWhichShtPickFor32==0)?2:0;
    int iKit=(iInArmiWhichKit==0)?0:4;

    for(int i=0; i<MAX_ARM_Row; i++)
        for(int j=0; j<MAX_ARM_Col; j++)
            flag[i][j]=false;
    //----------------------------------------
    //Set Arm Suck Status
    //----------------------------------------
    while(CanMove==false)
    {
        if(TrayYDirForArmYPitch() && ArmRow1Finish_2x8_32()==false)             //是否一次能吸8顆
        {
            iRowCT=2;
            iInArmOrder=0;                                                      //吸嘴組，第一排  //Steven 20150917 : Fixed for 32Site Y-Pitch
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    flag[i][j]=Prod.fInArmSuckUse_TwoArm32Site[i+iSht][j+iKit];
                }
            }
        }
        else
        {
            if(ArmRow1Finish_2x8_32()==false)
            {
                iInArmOrder=0;                                                  //吸嘴組，第一排
                iRowCT=0;
            }
            else
            {
                iInArmOrder=1;                                                  //吸嘴組，第二排
                iRowCT=1;
            }

            for(int j=0; j<MAX_ARM_Col; j++)
            {
                flag[iRowCT][j]=Prod.fInArmSuckUse_TwoArm32Site[iRowCT+iSht][j+iKit];
            }
        }
        NowRow=iRowCT;
        //----------------------------------------------------------
        //check inArm Close pitch is legal
        if(bCanPick2ICAtOnceTime)
        {
            if(AutoCalculateInArmClosePitch()==-1)
                bCanPick2ICAtOnceTime=false;
        }
        //----------------------------------------------------------
        //Set Arm Suck whith tray
        //----------------------------------------------------------
        if(bCanPick2ICAtOnceTime==true)
        {
            Find_InArm_PickerMaxUseCountOnTime(&flag[0][0], iRowCT, isStart, iX, iY, pitchstep, iStep, TestIF.bSearchLastMode);
        }
        else
        {
            Find_InArm_Single(&flag[0][0], ArmMaxSuck, iX, iY);
            isStart=0;
        }
        //-----------------------------------------------------------

        for(int i=0; i<MAX_ARM_Row; i++)
            for(int j=0; j<MAX_ARM_Col; j++)
                InArmSuckUse[i][j]=flag[i][j];
        //iX===>Tray has IC in x direct isStart==>need suck unit  pitchstep===>suck two ic pitch
        ix=iX-(isStart*pitchstep);
        iy=iY;

        if(IniConfig.bUseTrayBlockMode &&
           IniConfig.bP06_LoaderUseCarrierTray==true)                           //Ifor 20181011 (Steven) : add P06 功能馬達極限位置
        {
            if((ix<(-3*pitchstep) ||
                ix>=(LoadForm->XDivision*LoadForm->BlockXItem)) ||
               (iy<0 || iy>=(LoadForm->YDivision*LoadForm->BlockYItem)))
            {
                CanMove=false;
            }
            else
            {
                CanMove=DoMoveArmXYToLoaderStage_9045_2x8_32(bCanPick2ICAtOnceTime, iRowCT, ix, iy, iStep, false);
            }
        }
        else if((ix<(-3*pitchstep) || ix>=LoadForm->XDivision) ||               //jou 2011-04-15 最左邊最多不能超過(-3*pitchstep)
                (iy<0 || iy>=LoadForm->YDivision))                              //Hung 20110407 : 2x2吸取異常時會跳舞解除
        {
            CanMove=false;
        }
        else
        {
            CanMove=DoMoveArmXYToLoaderStage_9045_2x8_32(bCanPick2ICAtOnceTime, iRowCT, ix, iy, iStep, false);
        }

        if(CanMove==false)
        {
            if(bCanPick2ICAtOnceTime==true)
            {
                bCanPick2ICAtOnceTime=false;
            }
            else
            {
                ShowErrorMessage("WAR0152", 0, MInArmX, 0, __FUNC__);           //粹盤參數錯誤，馬達將會超出限制
                return false;
            }
        }
    }

    return true;
}
//==============================================================================
bool MoveArmXYToLoaderStage_9045_2x8_32()
{
    bool bCanPick2ICAtOnceTime;
    int  iStep, ix, iy, iSelRow;
    if(SearchLoadTrayUpDown_9045_2x8_32(bCanPick2ICAtOnceTime, iStep, ix, iy, iSelRow)==false)
        return false;

    if(bLoadAutoTeachInarm)                                                     //kevin 20210305 add inarm Auto teach move X Y PITCH Z 軸不動
        return DoMoveArmXYToLoaderStage_9045_2x8_32(bCanPick2ICAtOnceTime, iSelRow, ix, iy, iStep, true, false);
    else
        return DoMoveArmXYToLoaderStage_9045_2x8_32(bCanPick2ICAtOnceTime, iSelRow, ix, iy, iStep, true);
}
//==============================================================================
bool DoArmPickFromLoadStage_9045_2x8_32()
{
    IN_ARM_LOADER_2x8_32:

    QueueTaskList[13].CheckTaskChange();                                        //Steven 20200821 : 使用Goto也要記錄Task變化
    int ret;
    int iFlag;
    bool flag1, bTrayEndCleanOut, bPickFallDown;
    static bool bTrayDuplicateErr=false;
    int &Task=iPickFromLoadStageTask;
    static int iRetryCT=0, iBuf=0;
    AnsiString ErrPart="", ErrTray="", sbuffer="";                              //kevin 20220521 tray pos   //Steven 20100104 : 整合AlarmCodde
    bool bHasDuplicateErr=false, bLoadpickerrorMoveWait=false;                  //kevin 20220708            //Steven 20100104 : 整合AlarmCodde
    bool bflag=false;
    int iYpos=0;

    int iSht=(InArmSuck.iWhichShtPickFor32==0)?2:0;
    int iKit=(iInArmiWhichKit==0)?0:4;

    switch(Task)
    {
        case 1:
            if(CUSTOMER_CODE==CC_ASE_KaohSiung ||
               CUSTOMER_CODE==CC_ASE_KaohSiung_K12)                             //Steven 20131101 : Add ASE-K12
            {
                if(bLoadInarmAutoHigh)                                          //kevin 20171106 (wei) initial inarm 校正高度 Z軸先不要下去吸
                {                                                               //jou 2011-02-08 start : OneCycle偷拿料
                    AutoTeachLoadTrayZ(true, 0, iInArmZTeachTask);
                    Task=5;
                    break;
                }
            }

            if(MoveInArmZToPlateSafe(Task))
            {
                if(bNeedOneCycle())                                             //Steven 20131029 : 解決Index Position Error
                {
                    return true;
                }

                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(Prod.fInArmSuckUse_TwoArm32Site[i+iSht][j+iKit] &&
                           InArmSuck.Item[i][j]==NULL_IC)
                        {
                            bflag=true;
                        }
                    }
                }

                if(!bflag)                                                      //InArm 已吸滿
                {
                    return true;
                }
                MyInArmAtShuttleTimer.LatchCycleTime(true);                     //Steven 20151201
                Task=10;
                goto IN_ARM_LOADER_2x8_32;
            }
            break;
        case 5:                                                                 //kevin 20171103 (wei) Inarm Auto High //kevin 20170929 test4 不讓z軸往下
            if(AutoTeachLoadTrayZ(false, InArm, iInArmZTeachTask))
            {
                bLoadInarmAutoHigh=false;
                Task=1;
            }
            break;
        case 10:
            if(MOT[MMTrayY].HasIC())
            {
                iFlag=CheckLoaderHasTray();                                     //Steven 20161223 (jou) : 確認LoaderSensor統一成Function
                if(iFlag>0)
                {
                    Task=20;
                    break;
                }
                else
                {
                    bTrayDuplicateErr=false;
                }

                if(bNeedOneCycle())                                             //Steven 20131029 : 解決Index Position Error
                {
                    return true;
                }

                if(bNeedPickupErrorICToRecycleBin())                            //JimmyChiu 20220908 add Pickup Error Placement
                {
                    Task=10;
                    break;
                }
                Task=12;
                goto IN_ARM_LOADER_2x8_32;                                      //Steven 20231214 : For add speed
            }
            else
            {
                if(bNeedOneCycle())                                             //Steven 20131029 : 解決Index Position Error
                {
                    return true;
                }

                if((TrayForm.iManualRemoveLoader!=0 &&                          //Steven 20130819 : 加入開門檢查的功能  (==true --> !=0)
                    bLoaderHasSkip==true) ||                                    //jou 2010-12-20 修正需要手動拿Loader Tray時,In Arm Y移至shuttle 2上,方便OP拿Tray
                    bNeedManualRemoveTray==true)                                //Ifor 20200331 : add Edit Loader Tray Need Manual Remove Tray
                {
                    if(MoveInArm2XYToShuttle2Wait())
                    {
                        if(DoAutoSkipCheck())
                            return true;
                        else
                            Task=15;
                    }
                }
                else
                {
                    if(MoveInArm2XYToWait())
                    {
                        if(DoAutoSkipCheck())
                            return true;
                        else
                            Task=15;
                    }
                }
            }

            if(Task!=12)                                                        //Steven 20180813 : add in arm speed
                break;
        case 12:
            if(IniConfig.bUseAutoSiteMapping)                                   //jou 2011-03-24 start : Auto Site Mapping
            {
                if(MOT[MMTrayY].fHasTray==false)
                {
                    Task=10;
                    break;
                }
            }

            if(MOT[MMTrayY].HasIC())
            {
                if(MoveArmXYToLoaderStage_9045_2x8_32())
                {
                    if(IsCheckInArmDestroyActiveFinish())                       //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
                    {
                        Task=200;
                        goto IN_ARM_LOADER_2x8_32;                              //Steven 20231214 : For add speed
                    }
                }
            }
            else
            {
                Task=10;
            }
            break;
        case 15:
            if(MOT[MMTrayY_Car].fHasTray || MOT[MMTrayY].fHasTray || MOT[MMTrayZ].fHasTray)
            {
                Task=10;
                return false;
            }
            else
            {
                return true;
            }
        case 20:
            if(MoveInArm2XYToWait())
                Task=30;
            break;
        case 30:
            if(Cylinder[C_TrayY_Fixer].Push())
                Task=40;
            break;
        case 40:
            iBuf++;
            iFlag=CheckLoaderHasTray(true, iBuf, bTrayDuplicateErr);            //Steven 20161223 (jou) : 確認LoaderSensor統一成Function
            if(iFlag>0)
            {
                if(iFlag==2)
                {
                    bTrayDuplicateErr=true;
                    iBuf=0;
                }
            }
            else
            {
                bTrayDuplicateErr=false;
            }

            if(Cylinder[C_LoaderEdgePush].Enable)                               //kevin 20210830 add side push
            {
                Cylinder[C_LoaderEdgePush].On();                                //jou 2011-02-23 先打出去0.15sec
            }
            Task=10;
            break;
        case 200:
            if(MoveInArmZToLoaderPick(iRetryCT))
            {
                if(InArmNeedCheckOffset(false, 0))                              //Steven 20230531 : 簡化判斷式
                {
                    Task=220;
                    break;
                }

                if(USE_Y_AUTO_PITCH==true &&                                    //ChungHung 20131231 alter AutoYPitch
                   ArmRow1Finish_2x8_32()==true)
                {
                    iLoadPitchStepY=0;
                }
                else
                {
                    iLoadPitchStepY=GetLoaderYPitchStep();
                }
                sLoadPickupClean();                                             //kevin 20220723 initial data
                Task=1000;
                goto IN_ARM_LOADER_2x8_32;                                      //Steven 20231214 : For add speed
            }
            break;
        case 220:
            if(bEnterOffset==true)
            {
                bEnterOffset=false;
                Task=1;
            }
            else
            {
                Task=200;
            }
            break;
        case 1000:
            bPickFromLoader=true;                                                                                       //Steven 20171226 (Wei) : 確認Loader吸取完成
            flag1=true;
            iBuf=0;
            sAskStartDetect="";
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(InArmSuckUse[i][j] && InArmSuck.Item[i][j]==NULL_IC)
                    {
                        if(ArmSpeed_File[InArm].bUseHPVacuum)                                                           //Steven 20180125 (Jou) : 加熱盤的真空等待時間
                            InArmSuck.Suck[i][j].OnDelayTime=ArmSpeed[InArm].dVacuumTI*100;

                        iYpos=iYPosition+iLoadPitchStepY*i;
                        if(bSuckEnd[i][j]==false && InArmSuck.Suck[i][j].Suck())                                        //wei 20150318 suck 判斷是否動作
                        {
                            AddLoadingCount(i, j, iXPosition[j], iYpos);                                                //Steven 20161123 (jou) : 將Loader吸取的數量計算統一成Function
                        }
                        else if(InArmSuck.Suck[i][j].Error==false)
                        {
                            flag1=false;
                        }
                        else if(InArmSuck.Suck[i][j].Error==true)
                        {
                            bSuckEnd[i][j]=true;
                            sLoadPickupErrorTrayPos[i][j].sprintf("(%d,%d);",iXPosition[j]+1,iYpos+1);                  //kevin 20220521 Load pick error record X, Y 座標
                        }
                    }
                    else
                    {
                        bSuckEnd[i][j]=true;
                    }
                }
            }

            if(flag1==false)                                                                                            //KEVIN 20130421 按RETRY資料尚未交換會將其他IC丟掉
            {
                break;
            }

            ShowAutoSkipError();                                                                                        //kevin 20170904 記錄每一盤 SKIP 沒吸到IC位置

            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(InArmSuck.Suck[i][j].Error)
                    {
                        bInArmSuckErr=true;
                        iRetryCT++;
                        if(iRetryCT<=ArmSpeed[InArm].iRetryCT)                                                          //Steven 20141113 : Loader SKIP時在原地Retry
                        {
                            Task=1010;
                        }
                        else
                        {
                            Task=1050;
                        }
                        return false;
                    }
                }
            }

            iRetryCT=0;
            if(bCanRunSCKART==true &&                                                                                   //Steven 20161223 (wei) : For SCK ART loading count
               iSCKARTLoadingStatus>=1 &&                                                                               //數量到了要放下來,然後 Alarm
               TestIF_File.bSCKART_LotDeviceCheck &&
               bSCKARTLoadCntAlarm==true)
            {
                ProcessSCKARTLoadingCount(true);
                Task=4000;
            }
            else
            {
                Task=2000;
            }
            break;
        case 1010:
            if(MoveInArmZToPlateSafe(Task))
            {
                if(IniConfig.bE56LoaderRetryAtSamePosition)                     //Steven 20170828 (wei) : Loader吸取異常時,要在同一個位置作Retry
                {
                    Task=1020;
                }
                else
                {
                    bInArmPickErrFromLoader=true;                               //Steven 20170828 (wei) : 確認是否有在Loader吸料異常
                    Task=10;
                }
            }
            break;
        case 1020:
            if(MoveInArmZToLoaderPick(iRetryCT))                                //Steven 20141113 : Loader SKIP時在原地Retry
            {
                Task=1000;
            }
            break;
        case 1050:
            if(MoveInArmZToPlateSafe(Task))
            {
                bPickFromLoader=false;                                          //Steven 20180417 (Jou) : 確認Loader吸取完成
                Task=1100;
            }
            break;
        case 1100:
            bTrayEndCleanOut=false;                                                                                     //不要重復問2次
            iRetryCT=0;

            ErrPart=" ";
            ErrTray="";                                                                                                 //kevin 20220521 tray pos
            bHasDuplicateErr=false;
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(bPickLoaderDuplicateErr[i][j])
                    {
                        bHasDuplicateErr=true;
                    }

                    if(InArmSuck.Suck[i][j].Error)
                    {
                        CheckTrayMapData(iXPosition[j], iYPosition+iLoadPitchStepY*i);                                  //wei 20170302 (Steven) Tray map data error
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                        bPickLoaderDuplicateErr[i][j]=true;
                        iYpos=iYPosition+iLoadPitchStepY*i;                                                             //kevin 20171026 (wei) add Tray iYPos
                        PickErrorData(iXPosition[j], iYpos);                                                            //kevin 20170904 (Steven) auto tray end 記錄一盤ＸＹ位置
                        ErrTray+=sLoadPickupErrorTrayPos[i][j];                                                         //kevin 20220521 add Tray X Y  Pos      //kevin 20220521 tray pos
                        //sLoadPickupErrorTrayPos[i][j]="";                     //kevin 20220823 mark kevin 20220521 add Tray X Y  Pos      //kevin 20220521 tray pos
                        bLoadpickerrorMoveWait=true;                                                                    //kevin 20220708
                    }
                    else
                    {
                        bPickLoaderDuplicateErr[i][j]=false;
                    }
                }
            }
            //Steven 20100104 End

            if(bLoadpickerrorMoveWait &&
               (IniConfig.bE67_LoadPickerrorMoveWaitpos ||                                                              //kevin 20220708 load pick error move wait
                CUSTOMER_CODE==CC_ASE_KaohSiung))
            {
                if(MoveInArm2XYToWait()==false)
                   return false;
            }

            if(bTrayEndCleanOut==false)                                                                                 // if is TRAY END or CLEAN OUT don't ask again
            {
                if(IniConfig.b17bUseLoadCCDTrayMap)                                                                     //kevin 20220825 change function for load ccd check pickup error show Array
                {
                    DoInArmLoadPickUP_9045(ErrPart, ErrTray);                                                           //kevin 20220823 add Tray X Y  Pos
                }
                ret=ProcessMES0101InArmPickLoaderError(bHasDuplicateErr, ErrPart);                                      //Steven 20141121 : 整合InArm吸取Loader異常Alarm
            }

            if(ret==K_TRAY_END)
            {
                bTrayEndCleanOut=true;
                return DoTrayEndProcess_9045();                                                                         //Steven 20190703 : 整合Tray End動作
            }
            else if(ret==K_CLEAN_OUT)                                                                                   // maybe is SKIP ,so can not include under line
            {
                fMain->BtnCleanOutClick(fMain);
                return true;                                                                                            //Steven 20130612 : 要可以Clean Out
            }
            else if(ret==K_SKIP)
            {
                bflag=DoTraySkipProcess_9045();                                                                         //Steven 20190703 : 整合Load Tray Skip動作
                if(bflag==false)
                {
                    Task=1400;
                    return false;
                }
            }
            else if(ret==K_HOME)
            {
                InArmSuck.ResetAll();                                                                                   //Steven 20160323 : 避免未開啟真空
                Task=1200;
                return false;
            }
            else                                                                                                        // if(ret==K_RETRY)
            {
                if(IniConfig.bSPILFunction==true &&                                                                     //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                   ret==K_RETRY)                                                                                        //jou 2012-11-28 SPIL Manual Tray remove功能, 要包含在Loader上按 Retry
                {
                    bLoaderHasSkip=true;
                    if(CosFunction.bNextTrayNeedManualRemoveTray==true)                                                 //Sam 20190731 : 當 Load 發生 Pickup Error Skip 時，當前這盤除了要手動移除，下一盤也要手動移除。
                    {
                        MaualRemoveTray3Alarm.SetTakePickUpError();
                    }
                }
                InArmSuck.ResetAll();                                                                                   //Steven 20160323 : 避免未開啟真空

                if(IniConfig.bInOutArmCanPushHome)
                {
                    Task=1400;                                                                                          //不回Home
                }
                else
                {
                    Task=1200;
                }
                return false;
            }

            if(InArmSuckUse[0][0] || InArmSuckUse[0][1] || InArmSuckUse[0][2] || InArmSuckUse[0][3] ||
               InArmSuckUse[1][0] || InArmSuckUse[1][1] || InArmSuckUse[1][2] || InArmSuckUse[1][3])
            {
                Task=10;
            }
            else
            {
                iRetryCT=0;
                Task=2000;
            }
            break;
        case 1200:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=1300;
            }
            break;
        case 1300:
            SetInArmHome();
            //iRetryCT=0;
            Task=10;
            break;
        case 1400:
            if(MoveInArmZToPlateSafe(Task))                                     //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home
            {
                Task=10;
            }
            break;
        case 2000:
            if(MoveInArmZToPlateSafe(Task))
            {
                bPickFallDown=CheckInArmSuckFromLoaderICFallDown();             //Steven 20110516
                if(bPickFallDown)
                {
                    if(CosFunction.bAutoSkipNoDropError && IniConfig.bE60PickLoaderDropAutoSkip &&
                       (bDropRetry==true && ArmSpeed[InArm].bAutoSKIP==true && IniConfig.bRecordSkipPosition && bFirstRecordLoaderData==true))
                    {
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung)                     //kevin 20221008 add drop again pickup
                            Task=1200;
                        else
                            Task=1010;
                    }
                    else
                    {
                        Task=1;
                    }
                }
                else
                {
                    Task=2100;
                }
                goto IN_ARM_LOADER_2x8_32;                                      //Steven 20231214 : For add speed
            }
            break;
        case 2100:
            if(ArmFinish_2x8_32())
            {
                InArmSuck.SetNullIcToHasNullIc();
                fObserver->AddTimeData(1, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);                               //JerryYang 20151209
                return true;
            }
            else
            {
                if(IniConfig.bP17InArmFullPickFromLoader==false)                                                        //Steven 20111220 : 讓每盤都是滿的
                {
                    if((CUSTOMER_CODE==CC_KYEC_JCTHIU ||
                        CUSTOMER_CODE==CC_KYEC_LEE) &&                                                                  //Ifor 20181009 (Steven) : KYEC add FIFO Function
                       IniConfig.bI37_EnableFIFOMode &&
                       LastSet.iRunStartMode==rsmFIFOMode)
                    {
                        if(bNeedOneCycle())
                        {
                            return true;
                        }
                        else
                        {
                            Task=10;
                        }
                    }
                    else
                    {
                        if(iOneCycle==1)                                                                                //kevin 20110425 有吸取到ic就放shuttle 沒吸到吸嘴就不吸
                        {
                            return true;
                        }
                        else
                        {
                            Task=10;
                        }
                    }
                }
                else
                {
                    Task=10;
                }
            }
            break;
        case 2500:
            if(DoArmPickFromLoadStage_Delay.Off())                              //jou 2011-02-08 start : OneCycle偷拿料
            {
                Task=3000;
            }
            break;
        case 3000:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=1;

                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(InArmSuck.Item[i][j]==NULL_IC)
                        {
                            InArmSuck.Suck[i][j].OffDestroy();
                        }
                    }
                }
                return true;
            }
            break;
        case 4000:
            if(ProcessSCKARTLoadingCount())                                     //Steven 20161223 (wei) : For SCK ART loading count
            {
                Task=10;
            }
            break;
    }
    return false;
}
//==============================================================================
bool MoveInArmXYPickHotPlate_2x8_32(bool OnlyRow2)
{
    int Xpos=0, Ypos=0;
    int iMovePitchX=GetVariableInHotPlateData_2x8_32(true);
    int iMovePitchY=GetVariableYInHotPlateData();
    int iXVariable =GetInArmPitch_9045(iMovePitchX);
    int iXVariable2=GetInArmPitch2_9045(iMovePitchX);                           //ChungHung 20131231 alter AutoYPitch
    int iYVariable =GetInArmPitchY_9045(iMovePitchY);
    int ZDownPos[8]={0, 0, 0, 0, 0, 0, 0, 0};
    bool ZDownFlag[8]={false, false, false, false, false, false, false, false};
    int iLeap=0;
    double AlignSuck=0.0 ,OverSuck=0.0;

    if(LastSet.iRunStartMode==rsmAutoSiteMap && CosFunction.bUSEJCETSiteMapMode==true)                                  //Steven 20220527 : for JCET Auto Site Map
        return MoveInArmXYPickHotPlate_AutoSiteMap();

    if(HotPlateForm.XDivision==4)
    {
        if(iPickPlate[0]==0)                                                    //iPickFromPlate=0 先丟2號加熱盤也就上上加熱盤
        {
            Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
            Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPickPlateY[0]*Prod.HotPlateForm[0].iYPitch;
        }
        else
        {
            Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
            Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPickPlateY[0]*Prod.HotPlateForm[0].iYPitch;
        }
    }
    else if(HotPlateForm.XDivision==6)
    {
        if(i8PickerHPMode!=iHPNoUse8suck)                                       //JerryYang 20161007 簡化Hotplate判斷式
        {
            AlignSuck=2.0;
            OverSuck =2.0;
        }
        else
        {
            AlignSuck =3;
            OverSuck =3.0/2.0;                                                  //Steven 20120313 : 要用浮點運算,不然會變整數
        }

        if(iPickPlate[0]==0)                                                    //iPickFromPlate=0 先丟2號加熱盤也就上上加熱盤
        {
            Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+HotPlateForm.XPitch * AlignSuck;
            Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPickPlateY[0]*Prod.HotPlateForm[0].iYPitch;
        }
        else
        {
            Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+HotPlateForm.XPitch * AlignSuck;
            Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPickPlateY[0]*Prod.HotPlateForm[0].iYPitch;
        }

        if(iForPickHPX6Step==1)                                                 //Step 2
            Xpos-=HotPlateForm.XPitch * OverSuck;
    }
    else if(HotPlateForm.XDivision==8)                                          //ChungHung 20150513 add for ATK
    {
        if(iPickPlate[0]==0)                                                    //iPickFromPlate=0 先丟2號加熱盤也就上上加熱盤
        {
            if(bPitchOver12000)                                                 //Steven 20100307
                Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
            else
                Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+4*Prod.HotPlateForm[0].iXPitch;
            Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPickPlateY[0]*Prod.HotPlateForm[0].iYPitch;
        }
        else
        {
            if(bPitchOver12000)                                                 //Steven 20100307
                Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
            else
                Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+4*Prod.HotPlateForm[0].iXPitch;
            Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPickPlateY[0]*Prod.HotPlateForm[0].iYPitch;
        }
    }
    else if(HotPlateForm.XDivision==10)
    {
        if(iPickPlateX[0]==8)
            iLeap=2;
        else
            iLeap=4;

        if(iPickPlate[0]==0)                                                    //iPickFromPlate=0 先丟2號加熱盤也就上上加熱盤
        {
            Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+iLeap*Prod.HotPlateForm[0].iXPitch;
            Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPickPlateY[0]*Prod.HotPlateForm[0].iYPitch;
        }
        else
        {
            Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+iLeap*Prod.HotPlateForm[0].iXPitch;
            Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPickPlateY[0]*Prod.HotPlateForm[0].iYPitch;
        }
    }
    else if(HotPlateForm.XDivision==12)                                         //Steven 20220317 : 修正2x8跑 12x24加熱盤
    {
        if(iPickPlate[0]==0)                                                    //iPickFromPlate=0 先丟2號加熱盤也就上上加熱盤
        {
            if(iPickPlateX[0]==8)
                Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
            else
                Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+4*Prod.HotPlateForm[0].iXPitch;
            Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPickPlateY[0]*Prod.HotPlateForm[0].iYPitch;
        }
        else
        {
            if(iPickPlateX[0]==8)
                Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
            else
                Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+4*Prod.HotPlateForm[0].iXPitch;
            Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPickPlateY[0]*Prod.HotPlateForm[0].iYPitch;
        }
    }
    else if(HotPlateForm.XDivision==16)                                         //Steven 20150826 : 16x24 Hot Plate for 32Site
    {
        if(iPickPlate[0]==0)                                                    //iPickFromPlate=0 先丟2號加熱盤也就上上加熱盤
        {
            if(bPitchOver12000)                                                 //Steven 20100307
                Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
            else
                Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+8*Prod.HotPlateForm[0].iXPitch;
            Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPickPlateY[0]*Prod.HotPlateForm[0].iYPitch;
        }
        else
        {
            if(bPitchOver12000)                                                 //Steven 20100307
                Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
            else
                Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+8*Prod.HotPlateForm[0].iXPitch;
            Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPickPlateY[0]*Prod.HotPlateForm[0].iYPitch;
        }
    }

    Xpos+=Prod.HotPlateForm[0].iXPitch*iPickPlateX[0];

    if(HotPlateForm.XDivision==10 && iPickPlateX[0]==8 && InArmLeftSideHasIC())
    {
        Xpos-=Prod.HotPlateForm[0].iXPitch*2;
    }

    //jou 2010-07-06  00000x0x 造成Hotplat疊料
    //                0000x00x
    if(USE_Y_AUTO_PITCH==true)                                                  //ChungHung 20140304 add for AutoYPitch
    {
        if(OnlyRow2==false || ArmRow1AllHasIC()==false)
        {
            Ypos-=GetVariableYInHotPlateData();
        }
    }
    else
    {
        if(OnlyRow2 && ArmRow1AllHasIC())
        {
            Ypos+=TestIF.iARM_HP_Y_PITCH;
        }
    }

    //jou 2010-12-20 Pitch & Z 縮減為一個
    if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)
    {
        iXVariable +=InArmOffSet[InOfsInSh1]->GetVariable();
        iYVariable +=InArmOffSet[InOfsInSh1]->GetVariableY();                   //ChungHung 20131231 alter AutoYPitch
        iXVariable2+=InArmOffSet[InOfsInSh1]->GetVariable2();
    }
    else
    {
        if(iPickPlate[0]==0)
        {
            iXVariable +=InArmOffSet[InOfsHP2]->GetVariable();
            iYVariable +=InArmOffSet[InOfsHP2]->GetVariableY();                 //ChungHung 20131231 alter AutoYPitch
            iXVariable2+=InArmOffSet[InOfsHP2]->GetVariable2();
        }
        else
        {
            iXVariable +=InArmOffSet[InOfsHP1]->GetVariable();
            iYVariable +=InArmOffSet[InOfsHP1]->GetVariableY();                 //ChungHung 20131231 alter AutoYPitch
            iXVariable2+=InArmOffSet[InOfsHP1]->GetVariable2();
        }
    }

    TransferHotPlateRatio(true, &Xpos, &Ypos);                                  //Steven 20110324

    if(InArmContinuousMove(Xpos, Ypos, iXVariable, &ZDownFlag[0], &ZDownPos[0], false, iYVariable, iXVariable2))        //ChungHung 20131231 alter AutoYPitch
        return true;
    return false;
}
//==============================================================================
bool MoveInArmZ_To_Pick_HotPlate_9045_2x8_32(int iCT)
{
    bool flag[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(InArmSuckUse[i][j])
            {
                //Steven 20140217 : 兩段速移動
                if(iPickPlate[0]==0)                                            //上上加熱盤
                {
                    flag[i][j]=MOT[InArmZIndex[i][j]].MotorMove2SpeedForPicker(Prod.ZInArm_Plate2_Pick[i][j]+iCT*ArmSpeed[InArm].dRetryDown, &ArmSpeed[InArm]);
                }
                else
                {
                    flag[i][j]=MOT[InArmZIndex[i][j]].MotorMove2SpeedForPicker(Prod.ZInArm_Plate1_Pick[i][j]+iCT*ArmSpeed[InArm].dRetryDown, &ArmSpeed[InArm]);
                }
            }
        }
    }

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(InArmSuckUse[i][j])
            {
                if(flag[i][j]==false)
                    return false;
            }
        }
    }
    return true;
}
//==============================================================================
void GetPlaceHotPlate_4_2x8_32(int *X, int *Y)
{
    int Xpos=0, Ypos=0;
    if(iPlacePlate[0]==0)                                                       // iPlate=0 先丟2號加熱盤也就上上加熱盤
    {
        Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
        Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }
    else
    {
        Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
        Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }

    if(RowCanDualSite())
    {
        //jou 980324 Hotplate ItemX 8 X axis soft limit error
        Xpos+=Prod.HotPlateForm[0].iXPitch*iPlacePlateX[0];
    }

    if(USE_Y_AUTO_PITCH==true)                                                  //ChungHung 20140304 add for AutoYPitch
    {
        if(ArmRow1NotRealIC()==false)
            Ypos-=GetVariableYInHotPlateData();
    }
    else
    {
        if(ArmRow1NotRealIC())
            Ypos+=TestIF.iARM_HP_Y_PITCH;
    }

    *X=Xpos;
    *Y=Ypos;
}
//==============================================================================
void GetPlaceHotPlate_16_2x8_32(int *X, int *Y)
{
    int Xpos=0, Ypos=0;
    if(iPlacePlate[0]==0)                                                       // iPlate=0 先丟2號加熱盤也就上上加熱盤
    {
        if(bPitchOver12000)                                                     //Steven 20100307
            Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
        else
            Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+8*Prod.HotPlateForm[0].iXPitch;
        Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }
    else
    {
        if(bPitchOver12000)                                                     //Steven 20100307
            Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
        else
            Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+8*Prod.HotPlateForm[0].iXPitch;
        Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }

    if(RowCanDualSite())
    {
        //jou 980324 Hotplate ItemX 8 X axis soft limit error
        Xpos+=Prod.HotPlateForm[0].iXPitch*iPlacePlateX[0];
    }
    else
    {
        Xpos+=iPlacePlateX[0]*Prod.HotPlateForm[0].iXPitch;
    }

    if(USE_Y_AUTO_PITCH==true)                                                  //ChungHung 20140304 add for AutoYPitch
    {
        if(ArmRow1NotRealIC()==false)
            Ypos-=GetVariableYInHotPlateData();
    }
    else
    {
        if(ArmRow1NotRealIC())
            Ypos+=TestIF.iARM_HP_Y_PITCH;
    }
    *X=Xpos;
    *Y=Ypos;
}
//==============================================================================
void GetPlaceHotPlate_8_2x8_32(int *X, int *Y)
{
    int Xpos=0, Ypos=0;
    if(iPlacePlate[0]==0)                                                       // iPlate=0 先丟2號加熱盤也就上上加熱盤
    {
        if(bPitchOver12000)                                                     //Steven 20100307
            Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
        else
            Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+4*Prod.HotPlateForm[0].iXPitch;
        Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }
    else
    {
        if(bPitchOver12000)                                                     //Steven 20100307
            Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
        else
            Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+4*Prod.HotPlateForm[0].iXPitch;
        Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }

    if(RowCanDualSite())
    {
        //jou 980324 Hotplate ItemX 8 X axis soft limit error
        Xpos+=Prod.HotPlateForm[0].iXPitch*iPlacePlateX[0];
    }
    else
    {
        Xpos+=iPlacePlateX[0]*Prod.HotPlateForm[0].iXPitch;
    }

    if(USE_Y_AUTO_PITCH==true)                                                  //ChungHung 20140304 add for AutoYPitch
    {
        if(ArmRow1NotRealIC()==false)
            Ypos-=GetVariableYInHotPlateData();
    }
    else
    {
        if(ArmRow1NotRealIC())
            Ypos+=TestIF.iARM_HP_Y_PITCH;
    }
    *X=Xpos;
    *Y=Ypos;
}
//==============================================================================
void GetPlaceHotPlate_10_2x8_32(int *X, int *Y)
{
    int Xpos=0, Ypos=0;
    int iLeap=0;

    if(iPlacePlateX[0]==8)
        iLeap=2;
    else
        iLeap=4;

    if(iPlacePlate[0]==0)                                                       // iPlate=0 先丟2號加熱盤也就上上加熱盤
    {
        Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+iLeap*Prod.HotPlateForm[0].iXPitch;
        Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }
    else
    {
        Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+iLeap*Prod.HotPlateForm[0].iXPitch;
        Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }

    Xpos+=iPlacePlateX[0]*Prod.HotPlateForm[0].iXPitch;

    if(iPlacePlateX[0]==8 && InArmLeftSideNoIC())
    {
        Xpos-=Prod.HotPlateForm[0].iXPitch*2;
    }

    if(USE_Y_AUTO_PITCH==true)                                                  //ChungHung 20140304 add for AutoYPitch
    {
        if(ArmRow1NotRealIC()==false)
            Ypos-=GetVariableYInHotPlateData();
    }
    else
    {
        if(ArmRow1NotRealIC())
            Ypos+=TestIF.iARM_HP_Y_PITCH;
    }
    *X=Xpos;
    *Y=Ypos;
}
//==============================================================================
void GetPlaceHotPlate_12_2x8_32(int *X, int *Y)                                 //Steven 20220317 : 修正2x8跑 12x24加熱盤
{
    int Xpos=0, Ypos=0;
    if(iPlacePlate[0]==0)                                                       // iPlate=0 先丟2號加熱盤也就上上加熱盤
    {
        if(iPlacePlateX[0]==8)
            Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
        else
            Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+4*Prod.HotPlateForm[0].iXPitch;
        Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }
    else
    {
        if(iPlacePlateX[0]==8)
            Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+2*Prod.HotPlateForm[0].iXPitch;
        else
            Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+4*Prod.HotPlateForm[0].iXPitch;
        Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }

    if(RowCanDualSite())
    {
        //jou 980324 Hotplate ItemX 8 X axis soft limit error
        Xpos+=Prod.HotPlateForm[0].iXPitch*iPlacePlateX[0];
    }
    else
    {
        Xpos+=iPlacePlateX[0]*Prod.HotPlateForm[0].iXPitch;
    }

    if(USE_Y_AUTO_PITCH==true)                                                  //ChungHung 20140304 add for AutoYPitch
    {
        if(ArmRow1NotRealIC()==false)
            Ypos-=GetVariableYInHotPlateData();
    }
    else
    {
        if(ArmRow1NotRealIC())
            Ypos+=TestIF.iARM_HP_Y_PITCH;
    }
    *X=Xpos;
    *Y=Ypos;
}
//==============================================================================
void GetPlaceHotPlate_6_2x8_32(int *X, int *Y)
{
    int Xpos=0, Ypos=0;
    if(iPlacePlate[0]==0)                                                       // iPlate=0 先丟2號加熱盤也就上上加熱盤
    {
        Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+HotPlateForm.XPitch*3;
        Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }
    else
    {
        Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+HotPlateForm.XPitch*3;
        Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }

    Xpos+=Prod.HotPlateForm[0].iXPitch*iPlacePlateX[0];

    if(iForPlaceHPX6Step==1)                                                    //Step 2
        Xpos-=HotPlateForm.XPitch*3/2;

    if(USE_Y_AUTO_PITCH==true)                                                  //ChungHung 20140304 add for AutoYPitch
    {
        if(ArmRow1NotRealIC()==false)
            Ypos-=GetVariableYInHotPlateData();
    }
    else
    {
        if(ArmRow1NotRealIC())
            Ypos+=TestIF.iARM_HP_Y_PITCH;
    }
    *X=Xpos;
    *Y=Ypos;
}
//==============================================================================
void GetPlaceHotPlate_6_2x8_32_1CM(int *X, int *Y)
{
    int Xpos=0, Ypos=0;
    if(iPlacePlate[0]==0)                                                       // iPlate=0 先丟2號加熱盤也就上上加熱盤
    {
        Xpos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+HotPlateForm.XPitch*2;                                   //start pos(3)+picth*2
        Ypos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }
    else
    {
        Xpos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+HotPlateForm.XPitch*2;
        Ypos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iPlacePlateY[0]*Prod.HotPlateForm[0].iYPitch;
    }

    Xpos+=Prod.HotPlateForm[0].iXPitch*iPlacePlateX[0];

    if(iForPlaceHPX6Step==1)                                                    //Step 2
        Xpos-=HotPlateForm.XPitch*2;                                            //start pos(1)-pitch*2

    if(USE_Y_AUTO_PITCH==true)                                                  //ChungHung 20140304 add for AutoYPitch
    {
        if(ArmRow1NotRealIC()==false)
            Ypos-=GetVariableYInHotPlateData();
    }
    else
    {
        if(ArmRow1NotRealIC())
            Ypos+=TestIF.iARM_HP_Y_PITCH;
    }
    *X=Xpos;
    *Y=Ypos;
}
//==============================================================================
bool MoveInArmXYToPlatePlace_9045_2x8_32()
{
    int Xpos=0, Ypos=0;
    int iMovePitchX=GetVariableInHotPlateData_2x8_32(false);                    //jou 2013-02-20
    int iMovePitchY=GetVariableYInHotPlateData();
    int iXVariable =GetInArmPitch_9045(iMovePitchX);                            //jou 2013-02-20
    int iXVariable2=GetInArmPitch2_9045(iMovePitchX);
    int iYVariable =GetInArmPitchY_9045(iMovePitchY);                           //ChungHung 20131231 alter AutoYPitch
    int ZDownPos[8]={0, 0, 0, 0, 0, 0, 0, 0};
    bool ZDownFlag[8]={false, false, false, false, false, false, false, false};

    if(IniConfig.bI21AutoSiteMappingUseHotplate==true &&                        //Steven 20220527 : for JCET Auto Site Map
       bAutoSiteMapHasPickHP==true  &&
       CosFunction.bUSEJCETSiteMapMode)
    {
        return MoveInArmXYToPlatePlace_AutoSiteMap();
    }

    if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)                         //jou 2010-12-20 Pitch & Z 縮減為一個
    {
        iXVariable +=InArmOffSet[InOfsInSh1]->GetVariable();
        iYVariable +=InArmOffSet[InOfsInSh1]->GetVariableY();                   //ChungHung 20131231 alter AutoYPitch
        iXVariable2+=InArmOffSet[InOfsInSh1]->GetVariable2();
    }
    else
    {
        if(iPlacePlate[0]==0)
        {
            iXVariable +=InArmOffSet[InOfsHP2]->GetVariable();
            iYVariable +=InArmOffSet[InOfsHP2]->GetVariableY();                 //ChungHung 20131231 alter AutoYPitch
            iXVariable2+=InArmOffSet[InOfsHP2]->GetVariable2();
        }
        else
        {
            iXVariable +=InArmOffSet[InOfsHP1]->GetVariable();
            iYVariable +=InArmOffSet[InOfsHP1]->GetVariableY();                 //ChungHung 20131231 alter AutoYPitch
            iXVariable2+=InArmOffSet[InOfsHP1]->GetVariable2();
        }
    }

    if(HotPlateForm.XDivision==4)
    {
        GetPlaceHotPlate_4_2x8_32(&Xpos, &Ypos);
    }
    else if(HotPlateForm.XDivision==6)
    {
        if(i8PickerHPMode!=iHPNoUse8suck)                                       //JerryYang 20161007 簡化Hotplate判斷式
            GetPlaceHotPlate_6_2x8_32_1CM(&Xpos, &Ypos);
        else
            GetPlaceHotPlate_6_2x8_32(&Xpos, &Ypos);
    }
    else if(HotPlateForm.XDivision==8)
    {
        GetPlaceHotPlate_8_2x8_32(&Xpos, &Ypos);
    }
    else if(HotPlateForm.XDivision==10)
    {
        GetPlaceHotPlate_10_2x8_32(&Xpos, &Ypos);
    }
    else if(HotPlateForm.XDivision==12)                                         //Steven 20220317 : 修正2x8跑 12x24加熱盤
    {
        GetPlaceHotPlate_12_2x8_32(&Xpos, &Ypos);
    }
    else if(HotPlateForm.XDivision==16)                                         //Steven 20150826 : 16x24 Hot Plate for 32Site
    {
        GetPlaceHotPlate_16_2x8_32(&Xpos, &Ypos);
    }

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(iPlacePlate[0]==0)                                               //上上加熱盤
                ZDownPos[i+j*2]=Prod.ZInArm_Plate2_Place[i][j];
            else
                ZDownPos[i+j*2]=Prod.ZInArm_Plate1_Place[i][j];
        }
    }

    GetHPStatus_2x8_32(&ZDownFlag[0]);

    if(ZDownFlag[0]==false && ZDownFlag[1]==false && ZDownFlag[2]==false && ZDownFlag[3]==false &&
       ZDownFlag[4]==false && ZDownFlag[5]==false && ZDownFlag[6]==false && ZDownFlag[7]==false)
        return true;

    TransferHotPlateRatio(false, &Xpos, &Ypos);                                 //Steven 20110324

    if(InArmContinuousMove(Xpos, Ypos, iXVariable, &ZDownFlag[0],&ZDownPos[0], ZAxisDown, iYVariable, iXVariable2))     //ChungHung 20131231 alter AutoYPitch
    {
        HPPlaceLog.CheckPosition(Xpos, Ypos, iXVariable, iYVariable, iXVariable2);                                      //Steven 20211110 : 記錄放料到加熱盤的位置
        return true;
    }
    return false;
}
//==============================================================================
bool MoveInArmZToPlatePlace_9045_2x8_32()
{
    bool flag[2][4]={{true, true, true, true}, {true, true, true, true}};
    int j2, StartX=0, Step=0;

    if(HotPlateForm.XDivision==4 ||
       HotPlateForm.XDivision==8 ||
       HotPlateForm.XDivision==12 ||
       HotPlateForm.XDivision==16)                                              //Steven 20150826 : 16x24 Hot Plate for 32Site
    {
        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<MAX_ARM_Col; j++)
            {
                if(Row2CanPutHP(PlaceMode)==false)
                {
                    if(i==1)
                    {
                        flag[i][j]=true;
                        continue;
                    }
                }

                if(InArmSuck.Item[i][j]!=NULL_IC && InArmSuck.Item[i][j]!=HAS_NULL_IC)
                {
                    //Steven 20140217 : 兩段速移動
                    if(iPlacePlate[0]==0)                                       //上上加熱盤
                    {
                        flag[i][j]=MOT[InArmZIndex[i][j]].MotorMove2SpeedForPicker(Prod.ZInArm_Plate2_Place[i][j], &ArmSpeed[InArm]);
                    }
                    else
                    {
                        flag[i][j]=MOT[InArmZIndex[i][j]].MotorMove2SpeedForPicker(Prod.ZInArm_Plate1_Place[i][j], &ArmSpeed[InArm]);
                    }
                }
                else
                {
                    flag[i][j]=true;
                }
            }
        }
    }
    else                                                                        //if(HotPlateForm.XDivision==6)
    {
        Step=2;
        if(iForPlaceHPX6Step==0)
            StartX=0;
        else
            StartX=1;

        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<2; j++)
            {
                if(Row2CanPutHP(true)==false)
                {
                    if(i==1)
                    {
                        flag[i][j]=true;
                        continue;
                    }
                }

                j2=StartX+j*Step;
                if(InArmSuck.Item[i][j2]!=NULL_IC && InArmSuck.Item[i][j2]!=HAS_NULL_IC)
                {
                    //Steven 20140217 : 兩段速移動
                    if(iPlacePlate[0]==0)                                       //上上加熱盤
                    {
                        flag[i][j2]=MOT[InArmZIndex[i][j2]].MotorMove2SpeedForPicker(Prod.ZInArm_Plate2_Place[i][j2], &ArmSpeed[InArm]);
                    }
                    else
                    {
                        flag[i][j2]=MOT[InArmZIndex[i][j2]].MotorMove2SpeedForPicker(Prod.ZInArm_Plate1_Place[i][j2], &ArmSpeed[InArm]);
                    }
                }
            }
        }
    }

    for(int i=0; i<MAX_ARM_Row; i++)
        for(int j=0; j<MAX_ARM_Col; j++)
            if(flag[i][j]==false)
                return false;
    return true;
}
//------------------------------------------------------------------------------
bool CheckHasSpaceToPlace_9045_2x8_32()
{
    int iCT=0;
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<HotPlateForm.XDivision; j++)
        {
            for(int k=0; k<HotPlateForm.YDivision; k++)
            {
                if(HotPlateForm.iPlateSelect & 2-i)                             //MMPlate1+0=NO 2 HP
                {
                    if(MOT[MMPlate1+i].Tray.Data[j][k]==NULL_IC)
                        iCT++;
                }

                if(bRunAutoSiteMapping==true && MOT[MMPlate1+i].Tray.SiteMapData[j][k]==1)                              //Ifor 20210426 add:Site Mapping 取料後需補回IC
                    return true;
            }
        }
    }

    if(HotPlateForm.XDivision==4 ||
       HotPlateForm.XDivision==8 ||
       HotPlateForm.XDivision==12 ||
       HotPlateForm.XDivision==16)                                              //Steven 20150826 : 16x24 Hot Plate for 32Site
    {
        if(iCT>8)
            return true;
    }
    else if(HotPlateForm.XDivision==10)
    {
        if(iCT>8)
            return true;
    }
    else if(HotPlateForm.XDivision==6)
    {
        if(HotPlateForm.iPlateSelect==3)
        {
            if(iCloseSiteState==1 || iCloseSiteState==2)
            {
                if(iCT>(4+4))
                    return true;
            }
            else
            {
                if(iCT>(4+8))
                    return true;
            }
        }
        else
        {
            if(iCT>10)
                return true;
        }
    }

    if(CosFunction.bAutoSiteMappingUseHotPlate==true    &&
       IniConfig.bI21AutoSiteMappingUseHotplate==true   &&
       LastSet.iRunStartMode==rsmAutoSiteMap            &&
       IniConfig.bI21EnableASM==true                    &&
       bAutoSiteMapHotplateReady==false                 )                       //Ifor 20180221 (Steven) : add 再Auto Site Map模式下才執行
    {
        bAutoSiteMapHotplateReady=true;                                         //Ifor 20170920 (Steven) : add Auto Site Mapping Hotplate 旗標
//        iAutoSiteMapHotplatePlacePlate  =iPlacePlate[0];;   //Ifor 20171005 (Steven) : add Auto Site Mapping Hotplate 備份最後位置
//        iAutoSiteMapHotplatePlacePlateX =iPlacePlateX[0];   //Ifor 20171005 (Steven) : add Auto Site Mapping Hotplate 備份最後位置
//        iAutoSiteMapHotplatePlacePlateY =iPlacePlateY[0];   //Ifor 20171005 (Steven) : add Auto Site Mapping Hotplate 備份最後位置
    }
    return false;
}
//==============================================================================
// In Arm  main process for normal
//==============================================================================
bool DoPlaceToHotPlate_9045_2x8_32()
{
    int &Task=iInArmPlaceToHotPlateTask;
    int ip, ix, iy;
    int StartX=0, j2, iStepX;
    int iXItem10Step=0;
    bool flag=true;                                                             //kevin 20141206

    GetHotPlateYHalfPos();

    switch(Task)
    {
        case 1:
            GetVariableInHotPlateData_2x8_32(false);
            SearchPlateToPlace();
            bPlaceToHotplatePartOK=false;
            Task=100;                                                           //because Z and X-Y is continue move
//            break;
        case 100:
            if(MoveInArmXYToPlatePlace_9045_2x8_32())
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

            if(i8PickerHPMode!=iHPNoUse8suck &&                                 //JerryYang 20161007 簡化Hotplate判斷式  //Steven 20151117 : 2x2 8Picker at Hot mode
               HotPlateForm.XDivision==6)                                       //ChungHung Add 20101025 CC_ASE_CL HotPlate Offset 10mm
            {
                Task=375;
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
        case 350:
            if(LastSet.iRunStartMode==rsmAutoSiteMap &&
               bAutoSiteMapHasPickHP==true &&
               CosFunction.bUSEJCETSiteMapMode==true)                                                                                                           //Steven 20220527 : for JCET Auto Site Map
            {
                if(InArmSuck.Suck[iAutoSiteMapInArmRow][iAutoSiteMapInArmCol].Destroy())
                {
                    DoPlaceToHPBackupData(iAutoSiteMapInArmRow, iAutoSiteMapInArmCol, iAutoSiteMapHPNo, iAutoSiteMapHPX, iAutoSiteMapHPY);
                    InArmSuck.SetAllToNullIC();
                    bAutoSiteMapHasPickHP=false;
                }
                else                                                                                                                                            //if(InArmSuck.Suck[iAutoSiteMapInArmRow][iAutoSiteMapInArmCol].Error==false) // kevin 20141206
                {
                    return false;
                }
            }
            else if(HotPlateForm.XDivision==6)
            {
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<2; j++)
                    {
                        if(i==1 && Row2CanPutHP(PlaceMode)==false && iPlaceHPOrder==0 && bAutoSiteMapHotplateSave==false)                                       //Ifor 20171003 (Steven) : add避免無法放回 Hotplate
                            continue;

                        j2=j*2+iForPlaceHPX6Step;
                        if(InArmSuck.Item[i][j2]==HAS_NULL_IC || InArmSuck.Item[i][j2]==HAS_IC)
                        {
                            ip=iPlacePlate[0];
                            ix=iPlacePlateX[0]+StartX+j*3;
                            if(iPlaceHPOrder==0)
                                iy=iPlacePlateY[0]+iYHalf*i;
                            else
                                iy=iPlacePlateY[0];

                            if(InArmSuck.Item[i][j2]==HAS_NULL_IC || InArmSuck.Suck[i][j2].Destroy())                                                           //jou 980311
                            {
                                DoPlaceToHPSwapData(i, j2, ip, ix, iy);                                                                                         //Steven 20170109 : 將放料到HP資料交換改成Function
                            }
                            else if(InArmSuck.Suck[i][j2].Error==false)                                                                                         // kevin 20141206
                            {
                                flag=false;
                            }
                        }
                    }
                }

                //kevin 20141206  flag=CheckInArmDestroyICFail();  //Steven 20111223 : 檢查破壞錯誤
                if(flag==false)
                    return false;

                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<2; j++)
                    {
                        j2=j*2+iForPlaceHPX6Step;
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
                bPlaceToHotplatePartOK=true;                                                                                                                    //ChungHung 20120109 add
                DoCheckAutoSiteMappingPosition();                                                                                                               //Ifor 20170928 (Steven) : Check Auto Site Mapping Position
                Task=400;
            }
            else
            {
                iXItem10Step=0;
                if(HotPlateForm.XDivision==10 && iPlacePlateX[0]==8 && InArmLeftSideNoIC())
                    iXItem10Step=2;

                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(i==1 && Row2CanPutHP(PlaceMode)==false && iPlaceHPOrder==0 && bAutoSiteMapHotplateSave==false)                                       //Ifor 20171003 (Steven) : add避免無法放回 Hotplate
                        {
                            continue;
                        }

                        if(HotPlateForm.XDivision==4)
                        {
                            ix=iPlacePlateX[0]+j;
                        }
                        else if(HotPlateForm.XDivision==16)                                                                                                     //Steven 20150826 : 16x24 Hot Plate for 32Site
                        {
                            ix=iPlacePlateX[0]+j*4;
                        }
                        else if(HotPlateForm.XDivision==8 || HotPlateForm.XDivision==12)
                        {
                            if(bPitchOver12000)                                                                                                                 //Steven 20100307
                                ix=iPlacePlateX[0]+j;
                            else
                                ix=iPlacePlateX[0]+j*2;
                        }
                        else if(HotPlateForm.XDivision==10)
                        {
                            if(iPlacePlateX[0]!=8)
                                ix=iPlacePlateX[0]+j*2;
                            else
                                ix=iPlacePlateX[0]+j;
                        }

                        if(ix>=HotPlateForm.XDivision && bAutoSiteMapHotplateSave==false)                                                                       //Ifor 20180813 (Steven) : Add Auto Site Mapping 補回IC時不可跳過避免Hangup
                        {
                            continue;
                        }

                        j2=j+iXItem10Step;
                        if(InArmSuck.Item[i][j2]==HAS_IC || InArmSuck.Item[i][j2]==HAS_NULL_IC)
                        {
                            ip=iPlacePlate[0];
                            if(iPlaceHPOrder==0)
                            {
                                iy=iPlacePlateY[0]+iYHalf*i;
                            }
                            else
                            {
                                iy=iPlacePlateY[0];
                            }

                            if(InArmSuck.Item[i][j2]==HAS_NULL_IC || InArmSuck.Suck[i][j2].Destroy())                                                           //jou 980311
                            {
                                DoPlaceToHPSwapData(i, j2, ip, ix, iy);                                                                                         //Steven 20170109 : 將放料到HP資料交換改成Function
                            }
                            else if(InArmSuck.Suck[i][j2].Error==false)                                                                                         //kevin 20141206
                            {
                                flag=false;
                            }
                        }
                    }
                }

                //kevin 20141206 flag=CheckInArmDestroyICFail();  //Steven 20111223 : 檢查破壞錯誤
                if(flag==false)
                    return false;

                if(HotPlateForm.XDivision==10 && iPlacePlateX[0]==8)
                {
                    for(int i=0; i<MAX_ARM_Row; i++)
                    {
                        for(int j=0; j<2; j++)
                        {
                            j2=j+iXItem10Step;
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
                else
                {
                    for(int i=0; i<MAX_ARM_Row; i++)
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                        {
                            if(iPlaceHPOrder==0)
                            {
                                if(Row2CanPutHP(PlaceMode))
                                {
                                    if(InArmSuck.Item[i][j]!=NULL_IC)
                                        return false;
                                }
                                else
                                {
                                    if(InArmSuck.Item[0][j]!=NULL_IC)
                                        return false;
                                }
                            }
                            else
                            {
                                if(InArmSuck.Item[1][j]!=NULL_IC)
                                    return false;
                            }
                        }
                    }
                }
            }
            bPlaceToHotplatePartOK=true;                                                                                                                        //ChungHung 20120412 add
            DoCheckAutoSiteMappingPosition();                                                                                                                   //Ifor 20170928 (Steven) : Check Auto Site Mapping Position
            Task=400;
            break;
        case 375:                                                                                                                                               //20111020  Dell
            if(HotPlateForm.XDivision==6 && iPlacePlateX[0]==4)
                iStepX=2;
            else
                iStepX=4;

            if(LastSet.iRunStartMode==rsmAutoSiteMap &&
               bAutoSiteMapHasPickHP==true &&
               CosFunction.bUSEJCETSiteMapMode==true)                                                                                                           //Steven 20220527 : for JCET Auto Site Map
            {
                if(InArmSuck.Suck[iAutoSiteMapInArmRow][iAutoSiteMapInArmCol].Destroy())
                {
                    DoPlaceToHPBackupData(iAutoSiteMapInArmRow, iAutoSiteMapInArmCol, iAutoSiteMapHPNo, iAutoSiteMapHPX, iAutoSiteMapHPY);
                    InArmSuck.SetAllToNullIC();
                    bAutoSiteMapHasPickHP=false;
                }
                else                                                                                                                                            //if(InArmSuck.Suck[iAutoSiteMapInArmRow][iAutoSiteMapInArmCol].Error==false) // kevin 20141206
                {
                    return false;
                }
            }
            else if(HotPlateForm.XDivision==6)
            {
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<iStepX; j++)
                    {
                        if(i==1 && Row2CanPutHP(PlaceMode)==false && iPlaceHPOrder==0 && bAutoSiteMapHotplateSave==false)                                       //Ifor 20171003 (Steven) : add避免無法放回 Hotplate
                            continue;

                        j2=j+iForPlaceHPX6Step*2;
                        if(InArmSuck.Item[i][j2]==HAS_NULL_IC || InArmSuck.Item[i][j2]==HAS_IC)
                        {
                            ip=iPlacePlate[0];
                            ix=iPlacePlateX[0]+StartX+j;
                            if(iPlaceHPOrder==0)
                                iy=iPlacePlateY[0]+iYHalf*i;
                            else
                                iy=iPlacePlateY[0];

                            if(InArmSuck.Item[i][j2]==HAS_NULL_IC || InArmSuck.Suck[i][j2].Destroy())
                            {
                                DoPlaceToHPSwapData(i, j2, ip, ix, iy);                                                                                         //Steven 20170109 : 將放料到HP資料交換改成Function
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

                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<iStepX; j++)
                    {
                        j2=j+iForPlaceHPX6Step*2;
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
            bPlaceToHotplatePartOK=true;                                                                                                                        //ChungHung 20120412 add
            DoCheckAutoSiteMappingPosition();                                                                                                                   //Ifor 20170928 (Steven) : Check Auto Site Mapping Position
            Task=400;
            break;
        case 400:
            if(InArmSuck.HasIC())
            {
                //jou 2010-01-14 start
                if(HotPlateForm.XDivision==6)
                {
                    if(iForPlaceHPX6Step==0)
                    {
                        if(InArmSuck.Item[0][0]!=NULL_IC &&
                           InArmSuck.Item[1][0]!=NULL_IC &&
                           InArmSuck.Item[0][2]!=NULL_IC &&
                           InArmSuck.Item[1][2]!=NULL_IC)
                        {
                            Task=100;
                            break;
                        }
                    }
                    else
                    {
                        if(InArmSuck.Item[0][1]!=NULL_IC &&
                           InArmSuck.Item[1][1]!=NULL_IC &&
                           InArmSuck.Item[0][3]!=NULL_IC &&
                           InArmSuck.Item[1][3]!=NULL_IC)
                        {
                            Task=100;
                            break;
                        }
                    }
                }
                //jou 2010-01-14 end
                Task=1;
                break;
            }

            if(MoveInArmZToPlateSafe(Task))
            {
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
            break;                                                              //kevin 20131106 end
        case 501:
            if(HotPlateForm.XDivision==4)
            {
                if(HotPlateForm.iPlateSelect==3 &&
                   iPlacePlate[0]!=iPlacePlate[1] &&
                   AllRow1NotUse()==false &&
                   AllRow2NotUse()==false &&
                   HotPlateForm.YDivision%(iYHalf*2)!=0 &&
                   bPlaceFromHotplateAcross==true)
                {
                    bPlaceFromHotplateAcross=false;
                    iPlacePlate[0]=iPlacePlate[1];
                    iPlacePlateX[0]=iPlacePlateX[1];
                    iPlacePlateY[0]=iPlacePlateY[1];

                    iPlacePlate[1]=0;
                    iPlacePlateX[1]=0;
                    iPlacePlateY[1]=0;
                }
            }

            AdjustShuttleWhichKitOrder();                                                                               //Steven 20160201 : 修正Shuttle放料順序
            iHotCount++;                                                                                                //jou 2013-02-08 修正等待 Soak 時間時，畫面沒有顯示倒數計時的問題

            if(LastSet.iTemperature==Tempture_Hot &&
               CheckHasSpaceToPlace_9045_2x8_32())
                SearchPlateToPlace();

            if(USE_LASER_DISTANCE && TestIF_File.bEnableInArmLaser && LaserCheckPos.size()!=0)                          //Steven 20140228 : 雷射測距功能
            {
                CheckInArmFloating(true);
                Task=600;
            }
            else
            {
                return true;
            }
            break;
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
void SetArmAndHotPlateXItem4Para_2x8_32(int IP, int iPickX, int iPickY, bool bSetNullIC)
{
    int Step=4, iSX, i;
    int Chanel=0;

    if(iHotWhichShuttle[IP][iPickX][iPickY]==-1)
        iHotWhichShuttle[IP][iPickX][iPickY]=0;

    if(iHotWhichKit[IP][iPickX][iPickY]==-1)
        iHotWhichKit[IP][iPickX][iPickY]=0;

    int iSht=(iHotWhichShuttle[IP][iPickX][iPickY]==0)?2:0;
    int iKit=(iHotWhichKit[IP][iPickX][iPickY]==0)?0:4;

    if(bSetNullIC==false)
    {
        if(bRunAutoSiteMapping==false)                                          //Ifor 20210423 add:Run Site Mapping Which Shuttle 不使用Hotplate 資料
            InArmSuck.iWhichShtPickFor32=iHotWhichShuttle[IP][iPickX][iPickY];
        InArmSuck.iWhichKitPickFor32=iHotWhichKit[IP][iPickX][iPickY];
    }

    for(i=0; i<Step; i++)
    {
        if(Step==2)
        {
            iSX=iPickX+i*Step;
            if(Prod.fInArmSuckUse_TwoArm32Site[0+iSht][0+iKit])
                iSX+=1;
        }
        else
        {
            iSX=iPickX+i;
        }

        if(OnlyRow2 || iCloseSiteState==2)                                      //only Use Row 2
            Chanel=1;
        else
            Chanel=0;

        if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==NULL_IC      ||
           MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_CLEAN_IC ||
           MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_CLEAN_IC)          //kevin 20130226 add autoclean 不要吸
        {
            InArmSuckUse[Chanel][iSX]=false;
        }
        else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_IC)
        {
            InArmSuckUse[Chanel][iSX]=false;
            if(bSetNullIC && bRunAutoSiteMapping==false)                        //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
            {
                MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY, NULL_IC);
                InArmSuck.SetItemData(Chanel, iSX, HAS_NULL_IC);                //Steven 20140710 : Add
            }
        }
        else
        {
            if(bRunAutoSiteMapping==false)                                      //Ifor 20190618 :add
            {
                if(IniConfig.bI28_OnOffSiteOnTheFly ||                          //Steven 20150924 : 隨時開關Site功能
                   Prod.fInArmSuckUse_TwoArm32Site[Chanel+iSht][iSX+iKit])
                    InArmSuckUse[Chanel][iSX]=true;
            }
        }

        if(OnlyRow2==false && Row2CanPutHP(PickMode))
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==NULL_IC      ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_CLEAN_IC ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_NULL_CLEAN_IC)                                       //kevin 20120217 add autoclean 不要吸
            {
                InArmSuckUse[1][iSX]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_NULL_IC)
            {
                InArmSuckUse[1][iSX]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY+iYHalf, NULL_IC);
                    InArmSuck.SetItemData(1, iSX, HAS_NULL_IC);                 //Steven 20140710 : Add
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[1+iSht][iSX+iKit])
                        InArmSuckUse[1][iSX]=true;
                }
            }
        }
    }
}
//==============================================================================
void SetArmAndHotPlateXItem8Para_2x8_32(int IP, int iPickX, int iPickY, bool bSetNullIC)
{
    int Step=4, iSX, i, iStep;
    int Chanel=0;

    if(iHotWhichShuttle[IP][iPickX][iPickY]==-1)
        iHotWhichShuttle[IP][iPickX][iPickY]=0;

    if(iHotWhichKit[IP][iPickX][iPickY]==-1)
        iHotWhichKit[IP][iPickX][iPickY]=0;

    int iSht=(iHotWhichShuttle[IP][iPickX][iPickY]==0)?2:0;
    int iKit=(iHotWhichKit[IP][iPickX][iPickY]==0)?0:4;

    if(bSetNullIC==false)
    {
        if(bRunAutoSiteMapping==false)                                          //Ifor 20210423 add:Run Site Mapping Which Shuttle 不使用Hotplate 資料
            InArmSuck.iWhichShtPickFor32=iHotWhichShuttle[IP][iPickX][iPickY];
        InArmSuck.iWhichKitPickFor32=iHotWhichKit[IP][iPickX][iPickY];
    }

    if(HotPlateForm.XDivision==16)                                              //Steven 20150826 : 16x24 Hot Plate for 32Site
    {
        iStep=4;
    }
    else
    {
        iStep=2;
    }

    for(i=0; i<Step; i++)
    {
        if(bPitchOver12000)                                                     //Steven 20100307
            iSX=iPickX+i;
        else
            iSX=iPickX+i*iStep;

        if(OnlyRow2)
            Chanel=1;
        else
            Chanel=0;

        if(iCloseSiteState<=1)
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==NULL_IC      ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_CLEAN_IC ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_CLEAN_IC)      //kevin 20120217 add autoclean 不要吸
            {
                InArmSuckUse[Chanel][i]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_IC)
            {
                InArmSuckUse[Chanel][i]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    InArmSuck.SetItemData(Chanel, i, HAS_NULL_IC);              //Steven 20160719 : 改到裡面
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY, NULL_IC);
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[Chanel+iSht][i+iKit])
                        InArmSuckUse[Chanel][i]=true;
                }
            }
        }

        if(OnlyRow2==false && Row2CanPutHP(PickMode) && iCloseSiteState==0)
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==NULL_IC      ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_CLEAN_IC ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_NULL_CLEAN_IC)                                       //kevin 20120217 add autoclean 不要吸
            {
                InArmSuckUse[1][i]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_NULL_IC)
            {
                InArmSuckUse[1][i]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY+iYHalf, NULL_IC);
                    InArmSuck.SetItemData(1, i, HAS_NULL_IC);                   //Steven 20160719 : 改到裡面
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[1+iSht][i+iKit])
                        InArmSuckUse[1][i]=true;
                }
            }
        }

        if(iCloseSiteState==2)
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==NULL_IC      ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_CLEAN_IC ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_CLEAN_IC)      //kevin 20120217 add autoclean 不要吸
            {
                InArmSuckUse[1][i]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_IC)
            {
                InArmSuckUse[1][i]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    InArmSuck.SetItemData(1, i, HAS_NULL_IC);                   //Steven 20160719 : 改到裡面
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY, NULL_IC);
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[1+iSht][i+iKit])
                        InArmSuckUse[1][i]=true;
                }
            }
        }
        //else
          //  InArmSuckUse[1][iSX]=false;
    }
}
//==============================================================================
void SetArmAndHotPlateXItem10Para_2x8_32(int IP, int iPickX, int iPickY, bool bSetNullIC)
{
    int iStart=0, Step=0, iLeap=0, iSX, i;

    if(iHotWhichShuttle[IP][iPickX][iPickY]==-1)
        iHotWhichShuttle[IP][iPickX][iPickY]=0;

    if(iHotWhichKit[IP][iPickX][iPickY]==-1)
        iHotWhichKit[IP][iPickX][iPickY]=0;

    int iSht=(iHotWhichShuttle[IP][iPickX][iPickY]==0)?2:0;
    int iKit=(iHotWhichKit[IP][iPickX][iPickY]==0)?0:4;

    if(bSetNullIC==false)
    {
        if(bRunAutoSiteMapping==false)                                          //Ifor 20210423 add:Run Site Mapping Which Shuttle 不使用Hotplate 資料
            InArmSuck.iWhichShtPickFor32=iHotWhichShuttle[IP][iPickX][iPickY];
        InArmSuck.iWhichKitPickFor32=iHotWhichKit[IP][iPickX][iPickY];

        if(TestIF.iShuttleMode==1)                                              //Steven 20160429 : 修正關Arm Hang Up問題
        {
            if(TestIF.iShuttle_Sel==0)
                InArmSuck.iWhichShtPickFor32=0;
            else if(TestIF.iShuttle_Sel==1)
                InArmSuck.iWhichShtPickFor32=1;
        }
    }

    if(iPickPlateX[0]==8)
    {
        if(InArmLeftSideHasIC() && iPickPlateY[0]%2==1)
        {
            iStart=2;
            Step=4;
        }
        else
        {
            iStart=0;
            Step=2;
        }
        iLeap=1;
    }
    else
    {
        iStart=0;
        Step=4;
        iLeap=2;
    }

    for(i=iStart; i<Step; i++)
    {
        if(iStart==2)
            iSX=iPickX+(i-iStart)*iLeap;
        else
            iSX=iPickX+i*iLeap;

        if(iCloseSiteState<=1)                                                  //row 1 or full site
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==NULL_IC         ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_CLEAN_IC    ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_CLEAN_IC)      //kevin 20130226 add autoclean 不要吸
            {
                InArmSuckUse[0][i]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_IC)
            {
                InArmSuckUse[0][i]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY, NULL_IC);
                    InArmSuck.SetItemData(0, i, HAS_NULL_IC);                   //Steven 20140710 : Add
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[0+iSht][i+iKit])
                        InArmSuckUse[0][i]=true;
                }
            }
        }

        if(Row2CanPutHP(PickMode) && iCloseSiteState==0)                        //full site
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==NULL_IC      ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_CLEAN_IC ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_NULL_CLEAN_IC)                                       //kevin 20130226 add autoclean 不要吸
            {
                InArmSuckUse[1][i]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_NULL_IC)
            {
                InArmSuckUse[1][i]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY+iYHalf, NULL_IC);
                    InArmSuck.SetItemData(1, i, HAS_NULL_IC);                   //Steven 20140710 : Add
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[1+iSht][i+iKit])
                        InArmSuckUse[1][i]=true;
                }
            }
        }

        if(iCloseSiteState==2)                                                  //only row 2
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==NULL_IC      ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_CLEAN_IC ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_CLEAN_IC)      //kevin 20130226 add autoclean 不要吸
            {
                InArmSuckUse[1][i]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_IC)
            {
                InArmSuckUse[1][i]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY, NULL_IC);
                    InArmSuck.SetItemData(1, i, HAS_NULL_IC);                   //Steven 20140710 : Add
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[1+iSht][i+iKit])
                        InArmSuckUse[1][i]=true;
                }
            }
        }
    }
}
//==============================================================================
void SetArmAndHotPlateXItem12Para_2x8_32(int IP, int iPickX, int iPickY, bool bSetNullIC)                               //Steven 20220317 : 修正2x8跑 12x24加熱盤
{
    int Step=4, iSX, i;
    int Chanel=0;

    if(iHotWhichShuttle[IP][iPickX][iPickY]==-1)
        iHotWhichShuttle[IP][iPickX][iPickY]=0;

    if(iHotWhichKit[IP][iPickX][iPickY]==-1)
        iHotWhichKit[IP][iPickX][iPickY]=0;

    int iSht=(iHotWhichShuttle[IP][iPickX][iPickY]==0)?2:0;
    int iKit=(iHotWhichKit[IP][iPickX][iPickY]==0)?0:4;

    if(bSetNullIC==false)
    {
        if(bRunAutoSiteMapping==false)                                          //Ifor 20210423 add:Run Site Mapping Which Shuttle 不使用Hotplate 資料
            InArmSuck.iWhichShtPickFor32=iHotWhichShuttle[IP][iPickX][iPickY];
        InArmSuck.iWhichKitPickFor32=iHotWhichKit[IP][iPickX][iPickY];
    }

    for(i=0; i<Step; i++)
    {
        if(iPickX==8)
            iSX=iPickX+i;
        else
            iSX=iPickX+i*2;

        if(OnlyRow2)
            Chanel=1;
        else
            Chanel=0;

        if(iCloseSiteState<=1)
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==NULL_IC      ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_CLEAN_IC ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_CLEAN_IC)      //kevin 20120217 add autoclean 不要吸
            {
                InArmSuckUse[Chanel][i]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_IC)
            {
                InArmSuckUse[Chanel][i]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY, NULL_IC);
                    InArmSuck.SetItemData(Chanel, i, HAS_NULL_IC);              //Steven 20160719 : 改到裡面
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[Chanel+iSht][i+iKit])
                        InArmSuckUse[Chanel][i]=true;
                }
            }
        }

        if(OnlyRow2==false && Row2CanPutHP(PickMode) && iCloseSiteState==0)
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==NULL_IC      ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_CLEAN_IC ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_NULL_CLEAN_IC)                                       //kevin 20120217 add autoclean 不要吸
            {
                InArmSuckUse[1][i]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_NULL_IC)
            {
                InArmSuckUse[1][i]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY+iYHalf, NULL_IC);
                    InArmSuck.SetItemData(1, i, HAS_NULL_IC);                   //Steven 20160719 : 改到裡面
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[1+iSht][i+iKit])
                        InArmSuckUse[1][i]=true;
                }
            }
        }

        if(iCloseSiteState==2)
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==NULL_IC      ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_CLEAN_IC ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_CLEAN_IC)      //kevin 20120217 add autoclean 不要吸
            {
                InArmSuckUse[1][i]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_IC)
            {
                InArmSuckUse[1][i]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY, NULL_IC);
                    InArmSuck.SetItemData(1, i, HAS_NULL_IC);                   //Steven 20160719 : 改到裡面
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[1+iSht][i+iKit])
                        InArmSuckUse[1][i]=true;
                }
            }
        }
        //else
          //  InArmSuckUse[1][iSX]=false;
    }
}
//==============================================================================
void SetArmAndHotPlateXItem6Para_2x8_32(int IP, int iPickX, int iPickY, bool bSetNullIC)
{
    int iSX, i, i2;

    if(iHotWhichShuttle[IP][iPickX][iPickY]==-1)
        iHotWhichShuttle[IP][iPickX][iPickY]=0;

    if(iHotWhichKit[IP][iPickX][iPickY]==-1)
        iHotWhichKit[IP][iPickX][iPickY]=0;

    int iSht=(iHotWhichShuttle[IP][iPickX][iPickY]==0)?2:0;
    int iKit=(iHotWhichKit[IP][iPickX][iPickY]==0)?0:4;

    if(bSetNullIC==false)
    {
        if(bRunAutoSiteMapping==false)                                          //Ifor 20210423 add:Run Site Mapping Which Shuttle 不使用Hotplate 資料
            InArmSuck.iWhichShtPickFor32=iHotWhichShuttle[IP][iPickX][iPickY];
        InArmSuck.iWhichKitPickFor32=iHotWhichKit[IP][iPickX][iPickY];
    }

    for(i=0; i<2; i++)
    {
        iSX=iPickX+i*3;
        i2=i*2+iForPickHPX6Step;
        if(OnlyRow2 || iCloseSiteState==2)
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==NULL_IC      ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_CLEAN_IC ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_CLEAN_IC)      //kevin 20130226 add autoclean 不要吸
            {
                InArmSuckUse[1][i2]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_IC)
            {
                InArmSuckUse[1][i2]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY, NULL_IC);
                    InArmSuck.SetItemData(1, i2, HAS_NULL_IC);                  //Steven 20140710 : Add
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[1+iSht][i2+iKit])
                        InArmSuckUse[1][i2]=true;
                }
            }
        }
        else
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==NULL_IC      ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_CLEAN_IC ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_CLEAN_IC)      //kevin 20120217 add autoclean 不要吸
            {
                InArmSuckUse[0][i2]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_IC)
            {
                InArmSuckUse[0][i2]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY, NULL_IC);
                    InArmSuck.SetItemData(0, i2, HAS_NULL_IC);                  //Steven 20140710 : Add
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[0+iSht][i2+iKit])
                        InArmSuckUse[0][i2]=true;
                }
            }

            if(OnlyRow2==false && Row2CanPutHP(PickMode))
            {
                if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==NULL_IC      ||
                   MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_CLEAN_IC ||
                   MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_NULL_CLEAN_IC)                                   //kevin 20130226 add autoclean 不要吸
                {
                    InArmSuckUse[1][i2]=false;
                }
                else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_NULL_IC)
                {
                    InArmSuckUse[1][i2]=false;
                    if(bSetNullIC && bRunAutoSiteMapping==false)                //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                    {
                        MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY+iYHalf, NULL_IC);
                        InArmSuck.SetItemData(1, i2, HAS_NULL_IC);              //Steven 20140710 : Add
                    }
                }
                else
                {
                    if(bRunAutoSiteMapping==false)                              //Ifor 20190618 :add
                    {
                        if(IniConfig.bI28_OnOffSiteOnTheFly ||                  //Steven 20150924 : 隨時開關Site功能
                           Prod.fInArmSuckUse_TwoArm32Site[1+iSht][i2+iKit])
                            InArmSuckUse[1][i2]=true;
                    }
                }
            }
            else
            {
                InArmSuckUse[1][i2]=false;
            }
        }
    }
}
//==============================================================================
void SetArmAndHotPlateXItem6Para_2x8_32_1CM(int IP, int iPickX, int iPickY, bool bSetNullIC)
{
    int iSX, i, i2, iStepX;

    if(iHotWhichShuttle[IP][iPickX][iPickY]==-1)
        iHotWhichShuttle[IP][iPickX][iPickY]=0;

    if(iHotWhichKit[IP][iPickX][iPickY]==-1)
        iHotWhichKit[IP][iPickX][iPickY]=0;

    int iSht=(iHotWhichShuttle[IP][iPickX][iPickY]==0)?2:0;
    int iKit=(iHotWhichKit[IP][iPickX][iPickY]==0)?0:4;

    if(bSetNullIC==false)
    {
        if(bRunAutoSiteMapping==false)                                          //Ifor 20210423 add:Run Site Mapping Which Shuttle 不使用Hotplate 資料
            InArmSuck.iWhichShtPickFor32=iHotWhichShuttle[IP][iPickX][iPickY];
        InArmSuck.iWhichKitPickFor32=iHotWhichKit[IP][iPickX][iPickY];
    }

    if(iPickX==4)
        iStepX=2;
    else
        iStepX=4;

    for(i=0; i<iStepX; i++)
    {
        iSX=iPickX+i;
        i2=i+iForPickHPX6Step*2;
        if(OnlyRow2 || iCloseSiteState==2)
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==NULL_IC      ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_CLEAN_IC ||
               MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_CLEAN_IC)      //kevin 20130226 add autoclean 不要吸
            {
                InArmSuckUse[1][i2]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_IC)
            {
                InArmSuckUse[1][i2]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY, NULL_IC);
                    InArmSuck.SetItemData(1, i2, HAS_NULL_IC);                  //Steven 20140710 : Add
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[1+iSht][i2+iKit])
                        InArmSuckUse[1][i2]=true;
                }
            }
        }
        else
        {
            if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==NULL_IC)
            {
                InArmSuckUse[0][i2]=false;
            }
            else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY]==HAS_NULL_IC)
            {
                InArmSuckUse[0][i2]=false;
                if(bSetNullIC && bRunAutoSiteMapping==false)                    //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                {
                    MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY, NULL_IC);
                    InArmSuck.SetItemData(0, i2, HAS_NULL_IC);                  //Steven 20140710 : Add
                }
            }
            else
            {
                if(bRunAutoSiteMapping==false)                                  //Ifor 20190618 :add
                {
                    if(IniConfig.bI28_OnOffSiteOnTheFly ||                      //Steven 20150924 : 隨時開關Site功能
                       Prod.fInArmSuckUse_TwoArm32Site[0+iSht][i2+iKit])
                        InArmSuckUse[0][i2]=true;
                }
            }

            if(OnlyRow2==false && Row2CanPutHP(PickMode))
            {
                if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==NULL_IC      ||
                   MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_CLEAN_IC ||
                   MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_NULL_CLEAN_IC)                                   //kevin 20130226 add autoclean 不要吸
                {
                    InArmSuckUse[1][i2]=false;
                }
                else if(MOT[MMPlate1+IP].Tray.Data[iSX][iPickY+iYHalf]==HAS_NULL_IC)
                {
                    InArmSuckUse[1][i2]=false;
                    if(bSetNullIC && bRunAutoSiteMapping==false)                //Ifor 20171002 (Steven) : add 避免Auto Site Mapping Hotplate資料清除造成Hang up
                    {
                        MOT[MMPlate1+IP].SetTraySingleData(iSX, iPickY+iYHalf, NULL_IC);
                        InArmSuck.SetItemData(1, i2, HAS_NULL_IC);              //Steven 20140710 : Add
                    }
                }
                else
                {
                    if(bRunAutoSiteMapping==false)                              //Ifor 20190618 :add
                    {
                        if(IniConfig.bI28_OnOffSiteOnTheFly ||                  //Steven 20150924 : 隨時開關Site功能
                           Prod.fInArmSuckUse_TwoArm32Site[1+iSht][i2+iKit])
                            InArmSuckUse[1][i2]=true;
                    }
                }
            }
            else
            {
                InArmSuckUse[1][i2]=false;
            }
        }
    }
}
//==============================================================================
bool DoInArmPickFromHotPlate_9045_2x8_32(bool &bDownSpeed)
{
    int &Task=iInArmPickFromHotPlateTask;
    static int iRetryCT=0;
    int &ip=iPickPlate[0], &ix=iPickPlateX[0], &iy=iPickPlateY[0], i, j, ret, iStepX;
    int iYpos=0, iXpos=0;
    int j2;
    bool flag1=0;
    static bool bCheckSpeed=false;

    AnsiString ErrPart="";                                                      //Steven 20100104 : 整合AlarmCodde
    bool bHasDuplicateErr=false;                                                //Steven 20100104 : 整合AlarmCodde

    GetHotPlateYHalfPos();

    if(bUseTwoArm32Site &&
       iOneCycle && bPickFromHotplate==false)                                   //Steven 20220319 : Fixed for 4x8 one cycle hang up
    {
        if(FLCarryKit.HasIC()==false && BLCarryKit.HasIC()==false &&
           FTestSuck.HasIC()==BTestSuck.HasIC())                                //同時都有料或沒料
        {
            return true;
        }
    }

    switch(Task)
    {
        case 1:
            DoInArm_9045_2x8_32_SuckerMap();
            SearchPlateToPick();
            bPickFormHotplatePartOK=false;                                      //ChungHung 20120112 Hang Up 解除
            bPickFormHotplateRetry=false;                                       //Ifor 20160616 add Hot plate Pickup error Retry Hang Up 解除
            Task=50;
//            break;
        case 50:
            if(MoveInArmZToPlateSafe(Task))
            {
                //Task=100;
                Task=60;
            }
            break;
        case 60:
            //Ifor 20161129 Fix Hot plate 連續取料兩次
            //==>
            Task=100;                                                                                                                                           //Steven 20150921 : For HP Skip Hang Up
            if(HotPlateForm.XDivision==6)
            {
                if(ArmRow1AllHasIC())                                                                                                                           //if(ArmRow1Finish())
                {
                    //jou 2090326 only row1 hang
                    if(iCloseSiteState==1)
                    {
                        OnlyRow2=false;
                        Task=100;
                        return false;
                    }

                    OnlyRow2=true;

                    if(ArmRow2Finish_2x8_32())
                    {
                        OnlyRow2=false;
                        Task=100;
                    }
                }
            }
            else if(HotPlateForm.XDivision==4 || HotPlateForm.XDivision==8 || HotPlateForm.XDivision==12 || HotPlateForm.XDivision==16)                         //Steven 20150826 : 16x24 Hot Plate for 32Site
            {
                if(iPickHPOrder==0 && ArmRow2Finish_2x8_32()==false && Row2CanPutHP(PickMode))                                                                  //Steven 20220531 : for NN mode Auto Site Map
                {
                    if(InArmSuckUse[0][0]==false && InArmSuckUse[0][1]==false &&
                       InArmSuckUse[0][2]==false && InArmSuckUse[0][3]==false &&
                       InArmSuckUse[1][0]==false && InArmSuckUse[1][1]==false &&
                       InArmSuckUse[1][2]==false && InArmSuckUse[1][3]==false)
                    {
                        OnlyRow2=false;
                        Task=100;
                    }
                }
                else
                {
                    if(ArmRow1AllHasIC())
                    {
                        OnlyRow2=true;
                        //jou 2090326 only row1 hang
                        if(iCloseSiteState==1)
                        {
                            OnlyRow2=false;
                            Task=100;
                        }

                        if(ArmRow2Finish_2x8_32())
                        {
                            OnlyRow2=false;
                            Task=100;
                        }
                    }
                }
            }
            else if(HotPlateForm.XDivision==10)
            {
                if(InArmSuck.Item[0][0] && InArmSuck.Item[0][1] && InArmSuck.Item[0][2] && InArmSuck.Item[0][3] &&
                   InArmSuck.Item[1][0] && InArmSuck.Item[1][1] && InArmSuck.Item[1][2] && InArmSuck.Item[1][3])
                {
                    Task=100;
                }
            }
            break;
        case 100:
            iRetryCT=0;
            for(i=0; i<MAX_ARM_Row; i++)
                for(j=0; j<MAX_ARM_Col; j++)
                    InArmSuckUse[i][j]=false;

            if(LastSet.iRunStartMode==rsmAutoSiteMap &&
               CosFunction.bUSEJCETSiteMapMode==true)                                                                                                           //Steven 20220527 : for JCET Auto Site Map
            {
                InArmSuckUse[iAutoSiteMapInArmRow][iAutoSiteMapInArmCol]=true;
            }
            else
            {
                if(HotPlateForm.XDivision==4 || HotPlateForm.XDivision==8 || HotPlateForm.XDivision==12 || HotPlateForm.XDivision==16)                          //Steven 20150826 : 16x24 Hot Plate for 32Site
                {
                    if(HotPlateForm.XDivision==4)
                        SetArmAndHotPlateXItem4Para_2x8_32(ip, ix, iy, false);
                    else if(HotPlateForm.XDivision==12)                                                                                                         //Steven 20220317 : 修正2x8跑 12x24加熱盤
                        SetArmAndHotPlateXItem12Para_2x8_32(ip, ix, iy, false);
                    else
                        SetArmAndHotPlateXItem8Para_2x8_32(ip, ix, iy, false);
                    DoInArm_CheckSuckerMap();                                                                                                                   //Ifor 20180514 : add Auto Site Mapping Check Suck Map
                    if(InArmSuckUse[0][0]==false && InArmSuckUse[0][1]==false &&
                       InArmSuckUse[0][2]==false && InArmSuckUse[0][3]==false &&
                       InArmSuckUse[1][0]==false && InArmSuckUse[1][1]==false &&
                       InArmSuckUse[1][2]==false && InArmSuckUse[1][3]==false)
                    {
                        if(HotPlateForm.XDivision==4)
                            SetArmAndHotPlateXItem4Para_2x8_32(ip, ix, iy, true);
                        else if(HotPlateForm.XDivision==12)                                                                                                     //Steven 20220317 : 修正2x8跑 12x24加熱盤
                            SetArmAndHotPlateXItem12Para_2x8_32(ip, ix, iy, true);
                        else
                            SetArmAndHotPlateXItem8Para_2x8_32(ip, ix, iy, true);
                        bPickFormHotplatePartOK=true;                                                                                                           //ChungHung 20120112 Hang Up 解除
                        bPickFromHotplate=true;                                                                                                                 //Steven 20181219 : Fixed Hang up when Row A all close and run 8x16HP.
                        Task=340;
                        return false;
                    }
                }
                else if(HotPlateForm.XDivision==6)
                {
                    if(i8PickerHPMode!=iHPNoUse8suck)                                                                                                           //JerryYang 20161007 簡化Hotplate判斷式
                    {
                        SetArmAndHotPlateXItem6Para_2x8_32_1CM(ip, ix, iy, false);
                        DoInArm_CheckSuckerMap();                                                                                                               //Ifor 20180514 : add Auto Site Mapping Check Suck Map
                        if((iForPickHPX6Step==0 && InArmSuckUse[0][0]==false && InArmSuckUse[0][1]==false && InArmSuckUse[0][2]==false && InArmSuckUse[0][3]==false &&
                                                   InArmSuckUse[1][0]==false && InArmSuckUse[1][1]==false && InArmSuckUse[1][2]==false && InArmSuckUse[1][3]==false) ||
                           (iForPickHPX6Step==1 && InArmSuckUse[0][2]==false && InArmSuckUse[0][3]==false &&
                                                   InArmSuckUse[1][2]==false && InArmSuckUse[1][3]==false))
                        {
                            SetArmAndHotPlateXItem6Para_2x8_32_1CM(ip, ix, iy, true);
                            bPickFormHotplatePartOK=true;                                                                                                       //ChungHung 20120112 Hang Up 解除
                            bPickFromHotplate=true;                                                                                                             //Steven 20181219 : Fixed Hang up when Row A all close and run 8x16HP.
                            Task=340;
                            return false;
                        }
                    }
                    else
                    {
                        SetArmAndHotPlateXItem6Para_2x8_32(ip, ix, iy, false);
                        DoInArm_CheckSuckerMap();                                                                                                               //Ifor 20180514 : add Auto Site Mapping Check Suck Map
                        if((iForPickHPX6Step==0 && InArmSuckUse[0][0]==false && InArmSuckUse[0][2]==false  &&
                                                   InArmSuckUse[1][0]==false && InArmSuckUse[1][2]==false) ||
                           (iForPickHPX6Step==1 && InArmSuckUse[0][1]==false && InArmSuckUse[0][3]==false  &&
                                                   InArmSuckUse[1][1]==false && InArmSuckUse[1][3]==false))
                        {
                            SetArmAndHotPlateXItem6Para_2x8_32(ip, ix, iy, true);
                            bPickFormHotplatePartOK=true;                                                                                                       //ChungHung 20120112 Hang Up 解除
                            bPickFromHotplate=true;                                                                                                             //Steven 20181219 : Fixed Hang up when Row A all close and run 8x16HP.
                            Task=340;
                            return false;
                        }
                    }
                }
                else if(HotPlateForm.XDivision==10)
                {
                    SetArmAndHotPlateXItem10Para_2x8_32(ip, ix, iy, false);
                    DoInArm_CheckSuckerMap();                                                                                                                   //Ifor 20180514 : add Auto Site Mapping Check Suck Map
                    if(InArmSuckUse[0][0]==false && InArmSuckUse[0][1]==false &&
                       InArmSuckUse[0][2]==false && InArmSuckUse[0][3]==false &&
                       InArmSuckUse[1][0]==false && InArmSuckUse[1][1]==false &&
                       InArmSuckUse[1][2]==false && InArmSuckUse[1][3]==false)
                    {
                        SetArmAndHotPlateXItem10Para_2x8_32(ip, ix, iy, true);
                        bPickFormHotplatePartOK=true;                                                                                                           //ChungHung 20120112 Hang Up 解除
                        bPickFromHotplate=true;                                                                                                                 //Steven 20181219 : Fixed Hang up when Row A all close and run 8x16HP.
                        Task=340;
                        return false;
                    }
                }
            }
            Task=110;
            break;
        case 105:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=110;
            }
            break;
        case 110:
            if(MoveInArmXYPickHotPlate_2x8_32(OnlyRow2))
            {
                //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
                if(IsCheckInArmDestroyActiveFinish())
                {
                    //jou 2012-07-16 Auto Speed Hot Mode Fix
                    if(ArmSpeed[InArm].bAutoSpeed && LastSet.iTemperature==Tempture_Hot)
                        bCheckSpeed=true;
                    else
                        bCheckSpeed=false;

                    Task=150;
                }
            }
            break;
        case 150:
            if(InArmSuck.iWhichShtPickFor32==0)
            {
                //jou 2012-07-16 Auto Speed Hot Mode Fix
                if(bCheckSpeed)
                {
                    //ChungHung 20140725 fix 16 site auto speed problem
                    if(FLCarryKit.All_HasIC())
                        InArmSubSpeed();
                    else
                        InArmAddSpeed();

                    bCheckSpeed=false;
                }

                if(InSHT1InLF())
                {
                    if(InArmSuck.iWhichKitPickFor32==0)
                    {
                        if(FLCarryKit.LeftSideNoIC(4)==false)
                        {
                            MOT[MInShuttle1].fCanMoveL=true;
                            FLCarryKit.SetNullIcToHasNullIc();
                        }
                        else
                        {
                            Task=190;                                                                                   //ChungHung 20120724 在Task=150~Task=200時 進入Contact 做動作InArm位置會被修改造成HangUp
                        }
                    }
                    else
                    {
                        if(FLCarryKit.RightSideNoIC(4)==false)
                        {
                            MOT[MInShuttle1].fCanMoveL=true;
                            FLCarryKit.SetNullIcToHasNullIc();
                        }
                        else
                        {
                            Task=190;                                                                                   //ChungHung 20120724 在Task=150~Task=200時 進入Contact 做動作InArm位置會被修改造成HangUp
                        }
                    }
                }
                else if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR && NeedWaitTrayArm==true)                     //Ifor 20180510 : Fix OCR機型IniConfig.bCloseSiteByIndexArm未開啟時會丟同一個蝦頭導致Hang up
                {
                    return true;
                }
            }
            else
            {
                //jou 2012-07-16 Auto Speed Hot Mode Fix
                if(bCheckSpeed)
                {
                    if(BLCarryKit.HasIC())
                        InArmSubSpeed();
                    else
                        InArmAddSpeed();

                    bCheckSpeed=false;
                }

                if(InSHT2InLF())
                {
                    if(InArmSuck.iWhichKitPickFor32==0)
                    {
                        if(BLCarryKit.LeftSideNoIC(4)==false)
                        {
                            MOT[MInShuttle2].fCanMoveL=true;
                            BLCarryKit.SetNullIcToHasNullIc();
                        }
                        else
                        {
                            Task=190;                                                                                   //ChungHung 20120724 在Task=150~Task=200時 進入Contact 做動作InArm位置會被修改造成HangUp
                        }
                    }
                    else
                    {
                        if(BLCarryKit.RightSideNoIC(4)==false)
                        {
                            MOT[MInShuttle2].fCanMoveL=true;
                            BLCarryKit.SetNullIcToHasNullIc();
                        }
                        else
                        {
                            Task=190;                                                                                   //ChungHung 20120724 在Task=150~Task=200時 進入Contact 做動作InArm位置會被修改造成HangUp
                        }
                    }
                }
                else if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR && NeedWaitTrayArm==true)                     //Ifor 20180510 : Fix OCR機型IniConfig.bCloseSiteByIndexArm未開啟時會丟同一個蝦頭導致Hang up
                {
                    return true;
                }
            }
            break;
        case 190:                                                               //ChungHung 20120724 在Task=150~Task=200時 進入Contact 做動作InArm位置會被修改造成HangUp
            if(MoveInArmXYPickHotPlate_2x8_32(OnlyRow2)==false)
            {
                return false;
            }
            Task=200;
        case 200:
            if(LastSet.iRealDummy==REALLY && ArmSpeed[InArm].bSuckOnDown && Sen[SnRKManualTStart].IsOn()==false)
            {
                bPickFromHotplate=true;
                for(i=0; i<MAX_ARM_Row; i++)
                    for(j=0; j<MAX_ARM_Col; j++)
                        if(InArmSuckUse[i][j])
                            InArmSuck.Suck[i][j].On();
            }

            if(MoveInArmZ_To_Pick_HotPlate_9045_2x8_32(iRetryCT))
            {
                if(InArmNeedCheckHotPlateOffset(false))                         //Steven 20230531 : 簡化判斷式
                {
                    Task=210;
                    break;
                }

                InArmSuck.ResetAll();                                           //Steven 20160323 : 避免未開啟真空
                Task=300;
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
                Task=200;
            }
            break;
        case 220:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=110;
            }
            break;
        case 300:
            bPickFromHotplate=true;
            flag1=true;
            BackupHotplatelocation();                                                                                                                           //Ifor 20171006 (Steven) : add 備份Hotplate取料位置
            if(LastSet.iRunStartMode==rsmAutoSiteMap && CosFunction.bUSEJCETSiteMapMode==true)                                                                  //Steven 20220527 : for JCET Auto Site Map
            {
                HotplateDataConversion(iAutoSiteMapInArmRow, iAutoSiteMapInArmCol, iAutoSiteMapHPNo, iAutoSiteMapHPX, iAutoSiteMapHPY, &flag1);
                if(flag1==true)
                    bAutoSiteMapHasPickHP=true;
            }
            else if(HotPlateForm.XDivision==6)
            {
                for(i=0; i<2; i++)
                {
                    for(j=0; j<2; j++)
                    {
                        if(i==1 && Row2CanPutHP(PickMode)==false && iPickHPOrder==0 && bRunAutoSiteMapping==false)                                              //Ifor 20181114 : add 避免執行 Site Mapping 時發生錯亂
                        {
                            continue;
                        }
                        SearchPositionToPick(i, j, ip, ix, iy, &iXpos, &iYpos);                                                                                 //Ifor 20180521 : add 計算 Hotplate取料位置
                        j2=j*2+iForPickHPX6Step;

                        #ifdef SOFT_SIMULTE
                        if(fMain->cb1->Checked==true)
                        {
                            if(ip==fMain->rgHP->ItemIndex && iXpos==atoi(fMain->edHPX->Text.c_str()) && iYpos==atoi(fMain->edHPY->Text.c_str()))                //模擬吸取異常
                            {
                                InArmSuck.Suck[i][j2].Error=true;
                                continue;
                            }
                        }
                        #endif
                        HotplateDataConversion(i, j2, ip, iXpos, iYpos, &flag1);                                                                                //Ifor 20180518 : add 整合Hot plate 資料轉換
                    }
                }
            }
            else                                                                                                                                                //if(HotPlateForm.XDivision==4 || HotPlateForm.XDivision==8 || HotPlateForm.XDivision==12 || HotPlateForm.XDivision==10 || HotPlateForm.XDivision==16) //Steven 20150826 : 16x24 Hot Plate for 32Site
            {
                for(i=0; i<MAX_ARM_Row; i++)
                {
                    for(j=0; j<MAX_ARM_Col; j++)
                    {
                        if(i==1 && Row2CanPutHP(PickMode)==false && iPickHPOrder==0 && bRunAutoSiteMapping==false)                                              //Ifor 20181114 : add 避免執行 Site Mapping 時發生錯亂
                            continue;
                        SearchPositionToPick(i, j, ip, ix, iy, &iXpos, &iYpos);                                                                                 //Ifor 20180521 : add 計算 Hotplate取料位置
                        if(iCloseSiteState==1 && i==1)
                            break;
                        if(iCloseSiteState==2 && i==0)
                            break;

                        #ifdef SOFT_SIMULTE
                        if(fMain->cb1->Checked==true)
                        {
                            if(ip==fMain->rgHP->ItemIndex && iXpos==atoi(fMain->edHPX->Text.c_str()) && iYpos==atoi(fMain->edHPY->Text.c_str()))                //模擬吸取異常
                            {
                                InArmSuck.Suck[i][j].Error=true;
                                continue;
                            }
                        }
                        #endif
                        HotplateDataConversion(i, j, ip, iXpos, iYpos, &flag1);                                                                                 //Ifor 20180518 : add 整合Hot plate 資料轉換
                    }
                }
            }

            if(flag1==false)                                                                                                                                    //KEVIN 20130421 按RETRY資料尚未交換會將其他IC丟掉
                break;

            for(i=0; i<MAX_ARM_Row; i++)
            {
                for(j=0; j<MAX_ARM_Col; j++)
                {
                    if(InArmSuck.Suck[i][j].Error)
                    {
                        Task=320;
                        return false;
                    }
                    else
                    {
                        bPickHPDuplicateErr[i][j]=false;
                    }
                }
            }

            if(LastSet.iRunStartMode==rsmAutoSiteMap && CosFunction.bUSEJCETSiteMapMode==true)                                                                  //Steven 20220527 : for JCET Auto Site Map
            {
            }
            else if(i8PickerHPMode!=iHPNoUse8suck && HotPlateForm.XDivision==6)
            {
                SetArmAndHotPlateXItem6Para_2x8_32_1CM(ip, ix, iy, true);
            }
            else
            {
                if(HotPlateForm.XDivision==4)
                    SetArmAndHotPlateXItem4Para_2x8_32(ip, ix, iy, true);
                else if(HotPlateForm.XDivision==8 || HotPlateForm.XDivision==16)                                                                                //Steven 20150826 : 16x24 Hot Plate for 32Site
                    SetArmAndHotPlateXItem8Para_2x8_32(ip, ix, iy, true);
                else if(HotPlateForm.XDivision==6)
                    SetArmAndHotPlateXItem6Para_2x8_32(ip, ix, iy, true);
                else if(HotPlateForm.XDivision==10)
                    SetArmAndHotPlateXItem10Para_2x8_32(ip, ix, iy, true);
                else if(HotPlateForm.XDivision==12)                                                                                                             //Steven 20220317 : 修正2x8跑 12x24加熱盤
                    SetArmAndHotPlateXItem12Para_2x8_32(ip, ix, iy, true);
            }

            bPickFormHotplatePartOK=true;                                                                                                                       //ChungHung 20120112 Hang Up 解除
            Task=340;
            bAutoSiteMapHotICCanPick=true;                                                                                                                      //Ifor 20171006 (Steven) : 避免Auto site Mapping Hotplate補回資料後發生異常
            break;
        case 320:
            if(MoveInArmZToPlateSafe(Task))
            {
                iRetryCT++;
                Task=330;
            }
            break;
        case 330:
            if(iRetryCT>ArmSpeed[InArm].iRetryCT)  //Steven 20120109 : 整合成一次Skip多顆
            {
                //Steven 20100104 Start
                ErrPart=" ";
                bHasDuplicateErr=false;
                for(i=0; i<MAX_ARM_Row; i++)
                {
                    for(j=0; j<MAX_ARM_Col; j++)
                    {
                        if(bPickHPDuplicateErr[i][j])
                            bHasDuplicateErr=true;

                        if(InArmSuck.Suck[i][j].Error)
                        {
                            bPickHPDuplicateErr[i][j]=true;
                            ErrPart+=InArmSuck.Suck[i][j].sName;
                        }
                    }
                }
                //Steven 20100104 End
                iRetryCT=0;
                ret=0;
                if(IniConfig.bInOutArmCanPushHome)
                {
                    ret=ShowErrorMessage("JAM0109", K_RETRY|K_SKIP|K_HOME, MInArmX, bHasDuplicateErr, ErrPart);  //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home
                }
                else
                {
                    ret=ShowErrorMessage("JAM0109", K_RETRY|K_SKIP, MInArmX, bHasDuplicateErr, ErrPart);  //Steven 20100104
                }

                if(ret==K_SKIP)
                {
                    bAutoSiteMapWaitTestResult=false;  //Ifor 20180115 (Steven) : add Site Mapping SKIP 需清除旗標
                    if(LastSet.iRunStartMode==rsmAutoSiteMap && CosFunction.bUSEJCETSiteMapMode==true)  //Steven 20220527 : for JCET Auto Site Map
                    {
                        PorcessJAM0109HotPlatePickUpErrorSkip(iAutoSiteMapInArmRow, iAutoSiteMapInArmCol, iAutoSiteMapHPNo, iAutoSiteMapHPX, iAutoSiteMapHPY);
                        bPickHPDuplicateErr[iAutoSiteMapInArmRow][iAutoSiteMapInArmCol]=false;
                    }
                    else
                    {
                        if(HotPlateForm.XDivision==4 || HotPlateForm.XDivision==8 || HotPlateForm.XDivision==12 || HotPlateForm.XDivision==10 || HotPlateForm.XDivision==16)  //Steven 20150826 : 16x24 Hot Plate for 32Site
                        {
                            for(i=0; i<MAX_ARM_Row; i++)
                            {
                                for(j=0; j<MAX_ARM_Col; j++)
                                {
                                    if(InArmSuck.Suck[i][j].Error)
                                    {
                                        SearchPositionToPick(i, j, ip, ix, iy, &iXpos, &iYpos);  //Ifor 20180521 : add 計算 Hotplate取料位置 //Ifor 20180530 不太一樣需要驗證
                                        PorcessJAM0109HotPlatePickUpErrorSkip(i, j, ip, iXpos, iYpos);  //Steven 20161214 : 將HP吸取異常SKIP統一成Function
                                        bPickHPDuplicateErr[i][j]=false;
                                    }
                                }
                            }
                        }
                        else if(HotPlateForm.XDivision==6)
                        {
                            if(i8PickerHPMode!=iHPNoUse8suck)  //JerryYang 20161007 簡化Hotplate判斷式
                            {
                                if(iPickPlateX[0]==4)
                                    iStepX=2;
                                else
                                    iStepX=4;
                            }
                            else
                            {
                                iStepX=2;
                            }

                            for(i=0; i<2; i++)
                            {
                                for(j=0; j<iStepX; j++)
                                {
                                    //jou 2012-01-04 j2=j*2+iForPickHPX6Step,j2=0,2,4,6,破壞記憶體
                                    if(i8PickerHPMode!=iHPNoUse8suck)  //JerryYang 20161007 簡化Hotplate判斷式
                                    {
                                        j2=j+iForPickHPX6Step*2;
                                    }
                                    else
                                    {
                                        j2=j*2+iForPickHPX6Step;
                                    }

                                    if(InArmSuck.Suck[i][j2].Error)
                                    {
                                        SearchPositionToPick(i, j, ip, ix, iy, &iXpos, &iYpos);  //Ifor 20180521 : add 計算 Hotplate取料位置 //Ifor 20180530 不太一樣需要驗證
                                        PorcessJAM0109HotPlatePickUpErrorSkip(i, j2, ip, iXpos, iYpos);  //Steven 20161214 : 將HP吸取異常SKIP統一成Function
                                        bPickHPDuplicateErr[i][j2]=false;
                                    }
                                }
                            }
                        }
                    }
                    Task=340;  //kevin 20150514   HOTPLATE不要再下去吸
                    return false;  //kevin 20150514
                }
                else
                {
                    bPickFormHotplateRetry=true;  //Ifor 20160616 add Hot plate Pickup error Retry Hang Up 解除
                    if(IniConfig.bInOutArmCanPushHome)
                    {
                        //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home start
                        if(ret==K_HOME)
                        {
                            Task=335;
                        }
                        else  //if(ret==K_RETRY)
                        {
                            Task=336;
                        }
                        //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home start
                    }
                    else
                    {
                        Task=335;
                    }
                }
                return false;
            }
            Task=200;
            break;
        case 335:
            SetInArmHome();
            iRetryCT=0;
            Task=200;
            break;
        //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home start
        case 336:
            iRetryCT=0;
            Task=200;
            break;
        //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home end
        case 340:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=350;
            }
            break;
        case 350:
            Task=1;                                                                                                                                             //Steven 20150921 : For HP Skip Hang Up
            DoInArm_CheckSuckerMap();                                                                                                                           //Ifor 20180514 : add Auto Site Mapping Check Suck Map
            if(HotPlateForm.XDivision==6)
            {
                if(ArmRow1AllHasIC())                                                                                                                           //if(ArmRow1Finish())
                {
                    //jou 2090326 only row1 hang
                    if(iCloseSiteState==1)
                    {
                        OnlyRow2=false;
                        Task=400;
                        return false;
                    }

                    OnlyRow2=true;

                    if(ArmRow2Finish_2x8_32())
                    {
                        OnlyRow2=false;
                        Task=400;
                    }
                }
            }
            else if(HotPlateForm.XDivision==4 || HotPlateForm.XDivision==8 || HotPlateForm.XDivision==12 || HotPlateForm.XDivision==16)                         //Steven 20150826 : 16x24 Hot Plate for 32Site
            {
                if(iPickHPOrder==0 && ArmRow2Finish_2x8_32()==false && Row2CanPutHP(PickMode))
                {
                    if(InArmSuckUse[0][0]==false && InArmSuckUse[0][1]==false &&
                       InArmSuckUse[0][2]==false && InArmSuckUse[0][3]==false &&
                       InArmSuckUse[1][0]==false && InArmSuckUse[1][1]==false &&
                       InArmSuckUse[1][2]==false && InArmSuckUse[1][3]==false)
                    {
                        OnlyRow2=false;
                        Task=400;
                    }
                }
                else
                {
                    if(ArmRow1AllHasIC())
                    {
                        OnlyRow2=true;
                        //jou 2090326 only row1 hang
                        if(iCloseSiteState==1)
                        {
                            OnlyRow2=false;
                            Task=400;
                        }

                        if(ArmRow2Finish_2x8_32())
                        {
                            OnlyRow2=false;
                            Task=400;
                        }
                    }
                }
            }
            else if(HotPlateForm.XDivision==10)
            {
                if(InArmSuck.Item[0][0] && InArmSuck.Item[0][1] && InArmSuck.Item[0][2] && InArmSuck.Item[0][3] &&
                   InArmSuck.Item[1][0] && InArmSuck.Item[1][1] && InArmSuck.Item[1][2] && InArmSuck.Item[1][3])
                {
                    Task=400;
                }
            }
            break;
        case 400:
            if(HotPlateForm.XDivision==4)
            {
                if(HotPlateForm.iPlateSelect==3 && Row2CanPutHP(PickMode) && bPickFromHotplateAcross==true)
                {
                    bPickFromHotplateAcross=false;
                    iPickPlate[0]=iPickPlate[1];
                    iPickPlateX[0]=iPickPlateX[1];
                    iPickPlateY[0]=iPickPlateY[1];

                    iPickPlate[1]=0;
                    iPickPlateX[1]=0;
                    iPickPlateY[1]=0;
                }

                if(iOneCycle==1)                                                //Eliot 20100819 : OneCycle Hang Up 解除
                    HasHotReadyIC_9045_2x8_32();
            }
            else if(HotPlateForm.XDivision==12)                                 //ChungHung 20150513 add for ATK  //Steven 20160704
            {
                HasHotReadyIC_9045_2x8_32();
            }

            if(iPickPlate[0]==0)                                                //JerryYang 20180921 Setup Teach功能
            {
                iInArmPickPlaceCnt[InOfsHP1]++;
            }
            else
            {
                iInArmPickPlaceCnt[InOfsHP2]++;
            }

            Task=1;
            return true;
    }
    return false;
}
//==============================================================================
bool DoPlaceToShuttle_9045_2x8_32()
{
    IN_ARM_SHUTTLE_2x8_32:

    QueueTaskList[4].CheckTaskChange();                                         //Steven 20200821 : 使用Goto也要記錄Task變化
    int ret;
    int &Task=iInArmPlaceToShuttleTask;
    int iVacCol=0, iShtCol=0;
    bool flag, bCanFreeShuttle=false;
    bool bNeedAdjust2Time=false;                                                //Steven 20191112 : fixed for ASM close site hang up
    static bool bCheckSpeed=false, bFirstIn=false;
    AnsiString ErrPart="";                                                      //Steven 20100104 : 整合AlarmCodde
    static bool bCheckSpeed1=false;                                             //KaiChen 20171225 (Steven)：Add Speed Display
    if(bIndexPickUpErrorWaitRetry)                                              //Ifor 20171119 : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
        return false;

    if(LastSet.iRunStartMode==rsmAutoSiteMap &&
       (CosFunction.bUSEJCETSiteMapMode==true ||                                //Steven 20231113 : 修正 auto site mapping 2x6 hangup
        CUSTOMER_CODE==CC_SCS))                                                 //jou 20230626 : 修正JSCS auto site mapping 2x8 hangup
    {
        iWhichSht32=iAutoSiteMapHotplateiWhichShuttle;
        iWhichKit32    =InArmSuck.iWhichKitPickFor32;
    }
    else if(LastSet.iTemperature==Tempture_Hot &&                               //Steven 20160227 : 改成HOT在上面
            LastSet.iRunStartMode!=rsmAutoSiteMap)                              //kevin 20161124 add hot AutoSitmap hang up
    {
        if(LastSet.iTemperature==Tempture_Hot &&
           LastSet.iRunStartMode==rsmQAMode &&
           bQAModeFinishCleanOut==true)                                         //Steven 20180601 : 修正高溫QA mode放料
        {
            iWhichSht32=InArmSuck.iWhichShtPickFor32;
            iWhichKit32    =InArmSuck.iWhichKitPickFor32;
        }
        else
        {
            iWhichSht32=InArmSuck.iWhichShtPickFor32;
            iWhichKit32    =InArmSuck.iWhichKitPickFor32;
        }
    }
    else
    {
        iWhichSht32=InArmSuck.iWhichShtPickFor32;
        iWhichKit32    =InArmSuck.iWhichKitPickFor32;
    }

    AutoSiteMappingCheckShuttle(true);                                          //Ifor 20180116 (Steven) : add Auto Site Mapping Check iWhich Shuttle
    switch(Task)
    {
        case 1:
            bFirstIn=true;                                                      //Steven 20231115 : fixed for auto site off
            bDestoryOnSht=false;                                                //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            InArmXMoveSafe=false;

            if(bPlaceToShuttle2Step==false)                                     //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
            {
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(InArmSuck.Item[i][j]==NULL_IC)
                        {
                            InArmSuck.SetItemData(i, j, HAS_NULL_IC);           //Steven 20140710 : Add
                        }
                        else if(InArmSuck.Item[i][j]==HAS_TRY_SUCK_IC || InArmSuck.Item[i][j]==HAS_SUCK_IC)
                        {
                            InArmSuck.SetItemData(i, j, HAS_HOT_IC);            //Steven 20140710 : Add
                        }
                    }
                }
            }

            if(IniConfig.bI37_EnableFIFOMode &&                                 //Sam 20220411 : 2X6 補 FIFO
               LastSet.iRunStartMode==rsmFIFOMode)                              //JerryYang 20170729 (Steven) 修正FIFO模式 hang up
            {
            }
            else
            {
                if(InArmSuck.HasRealIC()==false)                                //ChungHung 20120105 如果剛好在Shuttle放完IC時 InArm剛好讓開會HangUp
                {
                    if(MoveInArmZToPlateSafe(Task))
                    {
                        InArmSuck.SetAll(NULL_IC);
                        fObserver->RecordInArmTime();
                        return true;
                    }
                    return false;
                }
            }

            if(ArmSpeed[InArm].bAutoSpeed)
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
            if(iWhichSht32==0)
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
            goto IN_ARM_SHUTTLE_2x8_32;
        case 900:
            if(InSHT1InLF() && FLCarryKit.HasIC()==false)
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
            if(MoveInArm2XYToShuttle_9045_2x8_32(iWhichSht32, iWhichKit32, ZAxisNotDown, true))
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
            MOT[MInShuttle1].fCanMoveL=false;
            if(MoveInArm2XYToShuttle_9045_2x8_32(iWhichSht32, iWhichKit32, ZAxisDown, true))
            {
                Task=1100;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                //Steven 20110516 : 修改成整合式Alarm
            {
                Task=955;                                                       //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 955:
            CheckInArmSuckICFallDownToHasNullIC();                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            Task=950;
            break;
        case 1000:
            bCanFreeShuttle=false;                                              //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            if(InSHT1InLF())
            {
                if(iWhichKit32==0)
                {
                    for(int i=0; i<2; i++)
                        for(int j=0; j<4; j++)
                            if(FLCarryKit.Item[i][j]!=NULL_IC)
                            {
                                bCanFreeShuttle=true;                           //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
                            }
                }
                else if(iWhichKit32==1)
                {
                    for(int i=0; i<2; i++)
                        for(int j=0; j<4; j++)
                            if(FLCarryKit.Item[i][j+4]!=NULL_IC)
                            {
                                bCanFreeShuttle=true;                           //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
                            }
                }

                if(bPlaceToShuttle2Step)                                        //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                    bCanFreeShuttle=false;

                if(bCanFreeShuttle==false)
                {
                    if(bShuttle1HasPickErr==false)                              //Sam 20230103 : 修正 32 Site Pickup error hange up
                        MOT[MInShuttle1].fCanMoveL=false;
                    InArmZNeedDown_2x8_32(0, true);
                    Task=1100;
                    goto IN_ARM_SHUTTLE_2x8_32;
                }
            }

            if(MOT[MInShuttle1].Led[iInposLed]==false &&
               MOT[MInShuttle1].fCanMoveL==false)
            {
                bCanFreeShuttle=true;                                           //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            }

            if(bCanFreeShuttle==true)                                           //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            {
                Task=1050;
            }
            break;
        case 1050:                                                              //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            if(MoveInArmZToPlateSafe(Task))
            {
                if(bFirstIn==true)                                              //Steven 20231115 : fixed for auto site off
                {
                    SetClosedShtKitToHasNullIC_2x8_8(0, iWhichKit32, __FUNC__, "1050");
                    bFirstIn=false;
                }
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

            MOT[MInShuttle1].fCanMoveL=false;
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

                Task=1200;
            }
            break;
        case 1110:
            if(MoveInArmZToPlateSafe(Task))
            {
                bDestoryOnSht=false;                                            //Steven 20180419 (Jou) : 在Shuttle吹氣與資料交換的Flag
                MOT[MInShuttle1].fCanMoveL=true;
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
            if(MoveInArm2XYToShuttle_9045_2x8_32(iWhichSht32, iWhichKit32, ZAxisDown, true))
            {
                Task=1100;
            }
            break;
        case 1200:
            flag=true;
            bPlaceToShuttle2Step=true;                                                                                  //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    iVacCol=j;
                    if(iWhichKit32==0)
                    {
                        iShtCol=j;
                    }
                    else
                    {
                        iShtCol=j+4;
                    }

                    if(InArmSuck.Item[i][iVacCol] &&
                       InArmSuck.Suck[i][iVacCol].GetNeedDestroyStatus())
                    {
                        iBackInArmHotCount=InArmSuck.HotCount;
                        if(InArmSuck.Item[i][iVacCol]==HAS_NULL_IC ||
                           InArmSuck.Suck[i][iVacCol].Destroy())
                        {
                            if(InArmSuck.Item[i][iVacCol]!=HAS_NULL_IC)                                                 //Steven 20170109 : 沒IC的地方不檢查吹氣
                            {
                                bInArmCheckDestroyACT[i][iVacCol]=true;                                                 //jou 981130 確認device確實destroy完成
                                LastSet.lShuttleCount++;                                                                //Steven 20170113 : For count place to shuttle
                            }

                            InArmSuck.PordRec[i][iVacCol].AddTestRecord(1, TestIF.iSiteMap[i+2][iShtCol]);              //Frank 20160505 add  //Steven 20190910 : i --> i+2
                            SetShuttleStatus_9045(0, i, iShtCol, i, iVacCol);
                            FLCarryKit.HotCount=InArmSuck.HotCount;                                                     //JerryYang 20230204 : add hot plate放置順序資料
                        }
                        else if(InArmSuck.Suck[i][iVacCol].Error==false)
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

            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(InArmSuck.Item[i][j]!=NULL_IC && InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                        return false;
                }
            }

            bInArmPlaceToShuttleFinish=true;                                                                            //Ifor 20171121 : add iWhichShuttle & iWhichKit 切換旗標 避免沒切換狀態造成Hangup

            if(InArmSuck.HasRealIC()==false)
            {
                Task=1300;
                bPlaceToShuttle2Step=false;                                                                             //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                fObserver->AddTimeData(2, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);                               //JerryYang 20151209
            }
            else
            {
                bPlaceToShuttle2Step=true;                                                                              //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                Task=1250;
            }
            iInArmPutIcToSH[iWhichSht32][iWhichKit32]=1;                                                                //Ifor 20171121 : Test 查看異常資料
            AddArmSiteRecord(iWhichSht32);                                                                              //Steven 20201204 : 修正Site No在非標準Pitch對應不上的問題
            break;
        case 1250:
            if(MoveInArm2XYToShuttle_9045_2x8_32(iWhichSht32, iWhichKit32, ZAxisDown, true))
            {
                Task=1100;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                //Steven 20110516 : 修改成整合式Alarm
            {
                Task=1255;                                                      //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 1255:
            CheckInArmSuckICFallDownToHasNullIC();                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            Task=1250;
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
            //Eliot 2009_12_25 Start
            if(iWhichKit32==0)
            {
                SetShuttleToHasNullIC(0, 0);
                if(IniConfig.bUseAutoSiteMapping &&
                   LastSet.iRunStartMode==rsmAutoSiteMap &&
                   IniConfig.bI21ASMRunTimeCHeck==false)                        //Steven 20140729 : AutoSiteMapping, 邊生產邊做)
                {
                    SetShuttleToHasNullIC(0, 1);
                    bNeedAdjust2Time=true;
                }

                //jou 2013-08-14 修正關右邊八個site時,in arm 與 shuttle 撞機
                if(InArmSideAllClose_32(0, 1)==true)
                {
                    SetShuttleToHasNullIC(0, 1);
                    bNeedAdjust2Time=true;
                }
            }
            else
            {
                SetShuttleToHasNullIC(0, 0);
                SetShuttleToHasNullIC(0, 1);
            }

            if(In_Shuttle_Auto_Latch==eInSHAutoLtc &&
               bInSHLtcFin[0]==false               &&
               FLCarryKit.UseSiteFullIC())                                      //KenHsieh 20251106 : fix close site don't do In Sht Latch
            {
                Task=1400;
                break;
            }

            if(iWhichKit32==0 &&
               InArmSideAllClose_32(0, 1)==false)
            {
                MOT[MInShuttle1].fCanMoveL=false;
            }
            else
            {
                MOT[MInShuttle1].fCanMoveL=true;
            }

            AdjustShtOrderWhenPlaceToSht(3);                                    //Steven 20180601 : 整合放完蝦頭後的調整
            if(bNeedAdjust2Time)                                                //Steven 20191112 : fixed for ASM close site hang up
                AdjustShtOrderWhenPlaceToSht(3);

            if(InArmSideAllClose_32(1, 0)==true &&                              //如果Arm 2的Site全部被關掉
               InArmSideAllClose_32(1, 1)==true)
            {
                SetShuttleToHasNullIC(1, 0);
                SetShuttleToHasNullIC(1, 1);
            }

            if(IniConfig.bIndexPickupWait==true &&
               (LastSet.iTemperature==Tempture_Hot ||                           //jou 2012-06-29 Index Pick up need wait Soak Time
                LastSet.iTemperature==Tempture_AmbientHot))                     //kevin 20180903 (Steven) : add 恆溫控制
            {
                if(Temperature.iShuttleSoakTimeMode==1 &&
                   Temperature.iInitialStart2Time!=0)
                {
                    FLCarryKit.TSoakTime.SetSecAndOn(Temperature.iInitialStart2Time);
                    dwStartShuttle1Soak=MyTickCount();                          //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
                    iInitialStart2Count=Temperature.iInitialStart2Time;
                }
            }
            fObserver->RecordInArmTime();
            bDestoryOnSht=false;                                                //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            iInArmPickPlaceCnt[InOfsInSh1]++;                                   //JerryYang 20180921 Setup Teach功能
            if(USE_Y_AUTO_PITCH==true &&
               IniConfig.bE57YPitchHome)                                        //kevin 20180822 (Steven) : add put shuttle Y pitch home AutoYPitch)
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
            if(InSHT2InLF() && BLCarryKit.RightSideNoIC(4)==true)
            {
                if(BLCarryKit.LeftSideNoIC(4)==true && bCheckSpeed)                                                     //Steven 20110525 : Auto Speed
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
            if(bCheckSpeed && InSHT2InLF()==false)                                                                      //Steven 20110525 : Auto Speed
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
            if(MoveInArm2XYToShuttle_9045_2x8_32(iWhichSht32, iWhichKit32, ZAxisNotDown, true))
            {
                Task=2000;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                                                        //Steven 20110516 : 修改成整合式Alarm
            {
                Task=1935;                                                                                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 1935:
            CheckInArmSuckICFallDownToHasNullIC();                                                                      //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            Task=1930;
            break;
        case 1950:
            MOT[MInShuttle2].fCanMoveL=false;
            if(MoveInArm2XYToShuttle_9045_2x8_32(iWhichSht32, iWhichKit32, ZAxisDown, true))
            {
                Task=2100;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                                                        //Steven 20110516 : 修改成整合式Alarm
            {
                Task=1955;                                                                                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 1955:
            CheckInArmSuckICFallDownToHasNullIC();                                                                      //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            Task=1950;
            break;
        case 2000:
            bCanFreeShuttle=false;                                                                                      //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            if(InSHT2InLF())
            {
                bCanFreeShuttle=false;
                if(iWhichKit32==0)
                {
                    for(int i=0; i<2; i++)
                        for(int j=0; j<4; j++)
                            if(BLCarryKit.Item[i][j]!=NULL_IC)
                            {
                                bCanFreeShuttle=true;                                                                   //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
                            }
                }
                else if(iWhichKit32==1)
                {
                    for(int i=0; i<2; i++)
                        for(int j=0; j<4; j++)
                            if(BLCarryKit.Item[i][j+4]!=NULL_IC)
                            {
                                bCanFreeShuttle=true;                                                                   //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
                            }
                }

                if(bPlaceToShuttle2Step)                                                                                //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                    bCanFreeShuttle=false;

                if(bCanFreeShuttle==false)
                {
                    if(bShuttle2HasPickErr==false)                                                                      //Sam 20230103 : 修正 32 Site Pickup error hange up
                        MOT[MInShuttle2].fCanMoveL=false;
                    InArmZNeedDown_2x8_32(1, true);
                    Task=2100;
                    goto IN_ARM_SHUTTLE_2x8_32;
                }
            }

            if(MOT[MInShuttle2].Led[iInposLed]==false && MOT[MInShuttle2].fCanMoveL==false)
            {
                bCanFreeShuttle=true;
            }

            if(bCanFreeShuttle==true)                                                                                   //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            {
                Task=2050;
            }
            break;
        case 2050:                                                                                                      //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            if(MoveInArmZToPlateSafe(Task))
            {
                if(bFirstIn==true)                                                                                      //Steven 20231115 : fixed for auto site off
                {
                    SetClosedShtKitToHasNullIC_2x8_8(1, iWhichKit32, __FUNC__, "2050");
                    bFirstIn=false;
                }
                SetShuttlefCanMoveL(1, true, __FUNC__, "2050");
                Task=2000;
            }
            break;
        case 2100:
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
               (bShuttle2MoveToRight ||
                bShuttle2MoveToLeft  ||                                                                                 //Steven 20220620 : 避免index在Kit1吸取異常, In arm偷放料
                bShuttle2HasPickErr))                                                                                   //Steven 20230116 : 避免In arm 偷放料
            {
                Task=1;
                return false;
            }

            MOT[MInShuttle2].fCanMoveL=false;
            if(InShtInLF(1)==false)
            {
                Task=2110;
                break;
            }

            if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
               IniConfig.bF23ShuttleVibration)                                                                          //kevin 20210415 IN Arm Vibrate shuttle
                SW[SwShuttleVibration2].On();

            bDestoryOnSht=true;                                                                                         //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            if(MoveInArmZToShuttlePlace_9045(1))
            {
                if(InArmNeedCheckOffset(true, 1))                                                                       //Steven 20230531 : 簡化判斷式
                {
                    Task=2150;
                    break;
                }

                Task=2200;
            }
            break;
        case 2110:
            if(MoveInArmZToPlateSafe(Task))
            {
                bDestoryOnSht=false;                                                                                    //Steven 20180419 (Jou) : 在Shuttle吹氣與資料交換的Flag
                MOT[MInShuttle2].fCanMoveL=true;
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
            if(MoveInArm2XYToShuttle_9045_2x8_32(iWhichSht32, iWhichKit32, ZAxisDown, true))
            {
                Task=2100;
            }
            break;
        case 2200:
            flag=true;
            bPlaceToShuttle2Step=true;                                                                                  //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    iVacCol=j;
                    if(iWhichKit32==0)
                    {
                        iShtCol=j;                                                                                      //kevin 20141031
                    }
                    else
                    {
                        iShtCol=j+4;                                                                                    //kevin 20141031
                    }

                    if(InArmSuck.Item[i][iVacCol] &&
                       InArmSuck.Suck[i][iVacCol].GetNeedDestroyStatus())
                    {
                        iBackInArmHotCount=InArmSuck.HotCount;
                        if(InArmSuck.Item[i][iVacCol]==HAS_NULL_IC ||
                           InArmSuck.Suck[i][iVacCol].Destroy())
                        {
                            if(InArmSuck.Item[i][iVacCol]!=HAS_NULL_IC)                                                 //Steven 20170109 : 沒IC的地方不檢查吹氣
                            {
                                bInArmCheckDestroyACT[i][iVacCol]=true;                                                 //jou 981130 確認device確實destroy完成
                                LastSet.lShuttleCount++;                                                                //Steven 20170113 : For count place to shuttle
                            }
                            InArmSuck.PordRec[i][iVacCol].AddTestRecord(2, TestIF.iSiteMap[i][iShtCol]);                //Frank 20160505 add
                            SetShuttleStatus_9045(1, i, iShtCol, i, iVacCol);
                            BLCarryKit.HotCount=InArmSuck.HotCount;                                                     //JerryYang 20230204 : add hot plate放置順序資料
                        }
                        else if(InArmSuck.Suck[i][iVacCol].Error==false)
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

            for(int i=0; i<MAX_ARM_Row; i++)
                for(int j=0; j<MAX_ARM_Col; j++)
                    if(InArmSuck.Item[i][j]!=NULL_IC && InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                        return false;

            bInArmPlaceToShuttleFinish=true;                                                                            //Ifor 20171121 : add iWhichShuttle & iWhichKit 切換旗標 避免沒切換狀態造成Hangup

            if(InArmSuck.HasRealIC()==false)
            {
                Task=2300;
                bPlaceToShuttle2Step=false;                                                                             //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                fObserver->AddTimeData(2, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);                               //JerryYang 20151209
            }
            else
            {
                bPlaceToShuttle2Step=true;                                                                              //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                Task=2250;
            }
            iInArmPutIcToSH[iWhichSht32][iWhichKit32]=1;                                                                //Ifor 20171121 : Test 查看異常資料
            AddArmSiteRecord(iWhichSht32);                                                                              //Steven 20201204 : 修正Site No在非標準Pitch對應不上的問題
            break;
        case 2250:
            if(MoveInArm2XYToShuttle_9045_2x8_32(iWhichSht32, iWhichKit32, ZAxisDown, true))
            {
                Task=2100;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                                                        //Steven 20110516 : 修改成整合式Alarm
            {
                Task=2255;                                                                                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 2255:
            CheckInArmSuckICFallDownToHasNullIC();                                                                      //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            Task=2250;
            break;
        case 2300:                                                                                                      //Steven 20220427 : 整合蝦頭搖搖敲敲功能
            InitDoInArmCheckShtFloatTask();
            Task=2400;
        case 2400:
            flag=DoInArmCheckShuttleFloating(1);
            if(flag==true)
            {
                Task=2500;
            }
            else
            {
                break;
            }
        case 2500:
            //Eliot 2009_12_25 Start
            if(iWhichKit32==0)
            {
                SetShuttleToHasNullIC(1, 0);
                if(IniConfig.bUseAutoSiteMapping &&
                   LastSet.iRunStartMode==rsmAutoSiteMap &&
                   IniConfig.bI21ASMRunTimeCHeck==false)                                                                //Steven 20140729 : AutoSiteMapping, 邊生產邊做)
                {
                    SetShuttleToHasNullIC(1, 1);
                    bNeedAdjust2Time=true;
                }

                //jou 2013-08-14 修正關右邊八個site時,in arm 與 shuttle 撞機
                if(InArmSideAllClose_32(1, 1)==true)
                {
                    SetShuttleToHasNullIC(1, 1);
                    bNeedAdjust2Time=true;
                }
            }
            else
            {
                SetShuttleToHasNullIC(1, 0);
                SetShuttleToHasNullIC(1, 1);
            }

            if(In_Shuttle_Auto_Latch==eInSHAutoLtc &&
               bInSHLtcFin[1]==false               &&
               BLCarryKit.UseSiteFullIC())                                      //KenHsieh 20251106 : fix close site don't do In Sht Latch
            {
                Task=2400;
                break;
            }

            if(iWhichKit32==0 &&
               InArmSideAllClose_32(1, 1)==false)
            {
                MOT[MInShuttle2].fCanMoveL=false;
            }
            else
            {
                MOT[MInShuttle2].fCanMoveL=true;
            }

            AdjustShtOrderWhenPlaceToSht(4);                                                                            //Steven 20180601 : 整合放完蝦頭後的調整
            if(bNeedAdjust2Time)                                                                                        //Steven 20191112 : fixed for ASM close site hang up
                AdjustShtOrderWhenPlaceToSht(4);

            if(InArmSideAllClose_32(0, 0)==true &&                                                                      //如果Arm 1的Site全部被關掉
               InArmSideAllClose_32(0, 1)==true)
            {
                if(FLCarryKit.HasIC()==false && FTestSuck.HasIC()==false)                                               //Steven 20190809 : Fixed for Auto Site Mapping hang up
                {
                    SetShuttleToHasNullIC(0, 0);
                    SetShuttleToHasNullIC(0, 1);
                }
            }

            if(IniConfig.bIndexPickupWait==true &&
               (LastSet.iTemperature==Tempture_Hot ||                                                                   //jou 2012-06-29 Index Pick up need wait Soak Time
               LastSet.iTemperature==Tempture_AmbientHot))                                                              //kevin 20180903 (Steven) : add 恆溫控制
            {
                if(Temperature.iShuttleSoakTimeMode==1 &&
                   Temperature.iInitialStart2Time!=0)
                {
                    BLCarryKit.TSoakTime.SetSecAndOn(Temperature.iInitialStart2Time);
                    dwStartShuttle2Soak=MyTickCount();                                                                  //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
                    iInitialStart2Count=Temperature.iInitialStart2Time;
                }
            }
            fObserver->RecordInArmTime();
            bDestoryOnSht=false;                                                                                        //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            iInArmPickPlaceCnt[InOfsInSh2]++;                                                                           //JerryYang 20180921 Setup Teach功能
            if(USE_Y_AUTO_PITCH==true &&
               IniConfig.bE57YPitchHome)                                                                                //kevin 20180822 (Steven) : add put shuttle Y pitch home AutoYPitch
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
//------------------------------------------------------------------------------
//Steven 20220317 : Fixed 2x4 support X=12
//==>
bool HasHotReadyIC_9045_4x8_32_X12()
{
    int &ix=iPickPlateX[0], &iy=iPickPlateY[0], &ip=iPickPlate[0];
    bool bSuccess;
    int spacX=1;
    int spacY=iYHalf;
    int Row=0;
    int Col=0;
    int state=1;

    iCloseSiteState=CloseSiteState();                                           //Steven 20210420 : 修正12site關site跑2x4 mode, HP=6x11 hang up
    if(iCloseSiteState!=0 || HotPlateYPitchCanPutAll()==false)                  //ChungHung 20140429 add   //ChungHung 20140430 add
        iYHalf=1;

    if(bRunAutoSiteMapping==true)                                               //Ifor 20180518 : add 簡化Site Mapping 旗標
    {
        if(CosFunction.bUSEJCETSiteMapMode==true)                               //Steven 20220527 : for JCET Auto Site Map
        {
            if(bAutoSiteMapHasPickHP==false)
                return SearchPickPlateXItem_AutoSiteMap();
            else
                return false;
        }
        else if(bAutoSiteMapHotplateReady==false && iCleanOut==0)               //Ifor 20180212 (Steven) : add 避免Clean Out 時 Hang up
        {
            return false;
        }
    }
    else
    {
        if(LastSet.iRunStartMode==rsmAutoSiteMap)                               //jou 2016-11-07 JCET fix Auto Site mapping Hot mode hangup
        {
            return false;
        }
    }

    GetVariableInHotPlateData_2x8_32(iPickPlateX[0]);                           //jou 2011-02-01  //JerryYang 20171206 (Steven) add iPickPlateX[0]
    if(ip==0 && TestIF_File.iAutoClean_Tray==eCKPos_HP2 && TestIF_File.iAutoClean_Function==1)                          //kevin 20130226 autoclean 放在hotplate
    {
        if(iy<4)                                                                //kevin 20150526
           iy=4;
    }

    if(HotPlateYPitchCanPutAll() && iy+iYHalf<HotPlateForm.YDivision)           //檢查Row2是否可放置 YPitch是否符合 && iy+iYHalf<HotPlateForm.YDivision
    {
        if(ArmRow1NotRealIC() && InArmSuck.HasIC())                             //Only Row2 HAS_IC  //Hung HasRealIC -> HasIC HangUp
        {
            if(ix==8)
            {
                spacX=1;
                spacY=iYHalf;
                Row=1;
                Col=4;
            }
            else
            {
                spacX=2;
                spacY=iYHalf;
                Row=1;
                Col=4;
            }
        }
        else
        {
            //JerryYang 20170215 (Steven) 修正12site跑8site,只開上排4個會hang up
            //==>
            if(CheckSpecialSiteMode()==true && InArm2x6AllDownCloseSite()==true)
            {
                if(ix==8)
                {
                    spacX=1;
                    spacY=0;
                    Row=2;
                    Col=4;
                }
                else
                {
                    spacX=2;
                    spacY=0;
                    Row=2;
                    Col=4;
                }
            }
            else
            //<==
            //JerryYang 20170215 (Steven) 修正12site跑8site,只開上排4個會hang up
            {
                if(ix==8)
                {
                    spacX=1;
                    spacY=iYHalf;
                    Row=2;
                    Col=4;
                }
                else
                {
                    spacX=2;
                    spacY=iYHalf;
                    Row=2;
                    Col=4;
                }
            }
        }
    }
    else
    {
        if(ix==8)
        {
            spacX=1;
            spacY=iYHalf;
            Row=1;
            Col=4;
        }
        else
        {
            spacX=2;
            spacY=iYHalf;
            Row=1;
            Col=4;
        }
    }

    bSuccess=CheckHotPlateHasSpace_9045_8_New_V(ip, iy, ix, spacY, spacX, Row, Col, state);
    if(bSuccess==false)
    {
        if(ix<2)
        {
            ix++;
            if(ix>=2)
            {
                ix=0;
                iy++;
                if(iCloseSiteState!=1 && HotPlateYPitchCanPutAll())             //Steven 20220317 :修正2x2跑12x24 HP hang up
                {
                    if(iy%iYHalf==0)
                        iy+=iYHalf;
                }

                if(iy>=HotPlateForm.YDivision)
                {
                    ix=8;
                    iy=0;
                }
            }
        }
        else if(ix==8)
        {
            iy++;
            if(iCloseSiteState!=1 && HotPlateYPitchCanPutAll())                 //Steven 20220317 :修正2x2跑12x24 HP hang up
            {
                if(iy%iYHalf==0)                                                //Steven 20220317
                    iy+=iYHalf;
            }

            if(iy>=HotPlateForm.YDivision)
            {
                ix=0;
                iy=0;
                if(HotPlateForm.iPlateSelect==0x03)
                {
                    if(ip==0)   ip=1;
                    else        ip=0;
                }
            }
        }
    }

    if(bSuccess)
    {
        if(IsAllSameHotCountHotOK(iHotPlateCount[iPickPlate[0]][ix][iy]))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}
//<==
//Steven 20220317 : 修正2x8跑 12x24加熱盤
//------------------------------------------------------------------------------
bool HasHotReadyIC_9045_2x8_32()
{
    int iSoakTime=0;
    int &ix=iPickPlateX[0], &iy=iPickPlateY[0], &ip=iPickPlate[0];
    int iPosX=0;
    int i, j, iYStep, iStepX=0, iXStepMax;
    int iLimitX=0;
    int surplus=0, iORGy=0;
    bool flag[2][4]={{false, false, false, false}, {false, false, false, false}};
    int Row2Y=0;
    int spacX=1;
    int spacY=iYHalf;
    int Row=0;
    int Col=0;
    int state=1;
    bool bSuccess=false;
    int iSht=(InArmSuck.iWhichShtPickFor32==0)?2:0;
    int iKit=(iInArmiWhichKit==0)?0:4;

    if(bRunAutoSiteMapping==true)                                               //Ifor 20180518 : add 簡化Site Mapping 旗標
    {
        if(CosFunction.bUSEJCETSiteMapMode==true)                               //Steven 20220527 : for JCET Auto Site Map
        {
            if(bAutoSiteMapHasPickHP==false)
                return SearchPickPlateXItem_AutoSiteMap();
            else
                return false;
        }
        else if(bAutoSiteMapHotplateReady==false && iCleanOut==0)               //Ifor 20180212 (Steven) : add 避免Clean Out 時 Hang up
        {
            return false;
        }
    }
    else
    {
        if(LastSet.iRunStartMode==rsmAutoSiteMap)                               //jou 2016-11-07 JCET fix Auto Site mapping Hot mode hangup
        {
            return false;
        }
    }

    if(HotPlateForm_File.XDivision==12)                                         //Steven 20220317 : 修正2x8跑 12x24加熱盤
    {
        return HasHotReadyIC_9045_4x8_32_X12();
    }

    if(HotPlateForm.XDivision==16)                                              //Steven 20150826 : 16x24 Hot Plate for 32Site
    {
        iXStepMax=4;
    }
    else
    {
        iXStepMax=2;
    }

    GetVariableInHotPlateData_2x8_32(true);                                     //jou 2011-02-01
    if(ip==0 && TestIF_File.iAutoClean_Tray==eCKPos_HP2 && TestIF_File.iAutoClean_Function==1)                          //kevin 20130226 autoclean 放在hotplate
    {
        if(iy<=3)
            iy=4;
    }

    for(i=0; i<2; i++)
        for(j=0; j<4; j++)
            flag[i][j]=false;

    iYStep=iYHalf;
    iYStep*=2;

    int iRunRute=iYHalf;
    iSoakTime=Prod.iHotTime;
    if(HotPlateForm.XDivision==4 || HotPlateForm.XDivision==8 || HotPlateForm.XDivision==16)                            //Steven 20150826 : 16x24 Hot Plate for 32Site
    {
        iStepX=4;
        if(HotPlateForm.XDivision==4)
        {
            iLimitX=1;
        }
        else
        {
            if(bPitchOver12000)
            {
                iLimitX=1;
            }
            else
            {
                if(HotPlateForm.XDivision==16)                                  //Steven 20150826 : 16x24 Hot Plate for 32Site
                {
                    iLimitX=4;
                }
                else
                {
                    iLimitX=2;
                }
            }
        }

        for(i=0; i<2; i++)
        {
            for(j=0; j<iStepX; j++)
            {
                iPosX=ix+j*iLimitX;

                if(i==1 && iPickHPOrder==0 && AllRow1NotUse()==false)
                {
                    if(Row2CanPutHP(PickMode))
                    {
                        if(iy+iYHalf>=HotPlateForm.YDivision)
                            flag[i][j]=false;
                        else
                            flag[i][j]=MOT[MMPlate1+ip].Tray.Data[iPosX][iy+iYHalf];

                        //jou 2010-12-30
                        Row2Y=iy+iYHalf;
                    }
                }
                else
                {
                    if(AllRow1NotUse())
                        Row2Y=iy;
                    else
                        Row2Y=iy+iYHalf;                                        //Eliot
                    flag[i][j]=MOT[MMPlate1+ip].Tray.Data[iPosX][iy];
                }
            }
        }

        if((flag[0][0]==false && flag[0][1]==false && flag[0][2]==false && flag[0][3]==false &&
            flag[1][0]==false && flag[1][1]==false && flag[1][2]==false && flag[1][3]==false))
        {
            if(HotPlateForm.XDivision==4)
            {
                ix=0;
                if(USE_Y_AUTO_PITCH==true)                                      //Steven 20230201 : 2x3以上不支援4x11
                {
                    if(iy+2<HotPlateForm.YDivision)
                    {
                        iy=iy+2;
                    }
                    else
                    {
                        ix=0;
                        iy=0;
                        iPickPlate[0]++;

                        if(iPickPlate[0]>=2)
                        {
                            iPickPlate[0]=0;
                            iPickPlate[1]=0;
                        }
                    }
                }
                else
                {
                    iy++;
                }

                iORGy=iy;
            }
            else
            {
                if(bPitchOver12000)                                             //Steven 20100307
                {
                    ix+=4;
                    if(ix>4)
                    {
                        ix=0;
                        iy++;

                        //jou 2012-01-20 Site:16 Hotplate X=8,Y=14,X pitch=22.5,Y pitch=25 hang up解除
                        if(HotPlateYPitchCanPutAll())
                        {
                            if(iy%iRunRute==0)
                                iy=iy+iRunRute;
                        }
                    }
                }
                else
                {
                    ix++;
                    if(ix>=iXStepMax)
                    {
                        ix=0;
                        iy++;

                        if(ip==0 && TestIF_File.iAutoClean_Tray==eCKPos_HP2 && TestIF_File.iAutoClean_Function==1)      //kevin 20130226 autoclean 放在hotplate
                        {
                            if(HotPlateForm.YDivision==18 && iy%iRunRute==0)
                                ix=0;
                            else
                            {
                                if(HotPlateYPitchCanPutAll())
                                {
                                    if(HotPlateForm.YDivision==12)
                                    {
                                        if(iy==6 || iy==10)                     //kevin 20121105 iy==6
                                            iy=iy+iRunRute;
                                    }
                                    else if(HotPlateForm.YDivision==16)         //kevin 20121106
                                    {
                                        if(iy==7 || iy==13)
                                        {
                                            iy=iy+iRunRute;                     //4 排吸完換排
                                        }
                                    }
                                    else if(HotPlateForm.YDivision!=22)
                                    {
                                        if((iy-4)%iRunRute==0)                  //kevin 20150407
                                            iy=iy+iRunRute;
                                    }
                                    else
                                    {
                                        if(iy%iRunRute==0)
                                            iy=iy+iRunRute;
                                    }
                                }
                            }
                        }
                        else
                        {
                            //jou 2012-01-20 Site:16 Hotplate X=8,Y=14,X pitch=22.5,Y pitch=25 hang up解除
                            if(HotPlateYPitchCanPutAll())
                            {
                                if(iy%iRunRute==0)
                                    iy=iy+iRunRute;
                            }
                        }
                    }
                }
            }

            if(HotPlateForm.XDivision==4)
            {
                if(HotPlateYPitchCanPutAll())
                {
                    //jou 980824 add Only Select any one Hotplate 1 or 2
                    if(ip==0 || (Prod.bPlateSelect[0]==true && Prod.bPlateSelect[1]==false) ||
                                (Prod.bPlateSelect[0]==false && Prod.bPlateSelect[1]==true))
                    {
                        //ChungHung 20110905 HangUp
                        while(iy%iYStep>=iYStep/2 )
                            iy++;
                    }
                    else
                    {
                        if(HotPlateForm.YDivision%iYStep<=iYHalf)
                            surplus=HotPlateForm.YDivision%iYStep;
                        else
                            surplus=iYStep-(HotPlateForm.YDivision%iYStep);
                        //ChungHung 20110905 HangUp 16Site 沒有關Site
                        while((iy%iYStep<surplus || iy%iYStep>=iYHalf+surplus))
                            iy++;
                    }
                }

                if(iy>=HotPlateForm.YDivision)
                {
                    iPickPlate[0]++;
                    if(iPickPlate[0]>=2)
                    {
                        iPickPlate[0]=0;
                        iPickPlate[1]=0;
                    }
                    CheckPlateInstall();
                    //ChungHung 20110905 HangUp 16Site 沒有關Site
                    if(HotPlateForm.iPlateSelect==3 && ip==1 && HotPlateYPitchCanPutAll())
                    {
                        if(HotPlateForm.YDivision%(iYHalf*2)<=iYHalf)
                        {
                            if(iy>=HotPlateForm.YDivision)
                                iy=HotPlateForm.YDivision;
                            iy=iy%iYStep;
                        }
                        else
                        {
                            iy=(iYHalf+iORGy)-HotPlateForm.YDivision;
                        }
                        iPickPlate[1]=ip;
                        iPickPlateY[1]=iy;
                        iPickPlateX[1]=ix;
                    }
                    else
                    {
                        iy=0;
                    }

                    if(ip==0 && TestIF_File.iAutoClean_Tray==eCKPos_HP2 && TestIF_File.iAutoClean_Function==1)          //kevin 20130226 autoclean 放在hotplate
                        iy=4;
                    else
                        iy=0;
                }
            }
            else
            {
                if(iy>=HotPlateForm.YDivision)
                {
                    iy=0;
                    ip--;
                    if(ip<0)
                        ip=1;
                    CheckPlateInstall();
                    if(ip==0 && TestIF_File.iAutoClean_Tray==eCKPos_HP2 && TestIF_File.iAutoClean_Function==1)          //kevin 20130226 autoclean 放在hotplate
                       iy=4;
                    else
                       iy=0;
                }
            }
        }

        if((flag[0][0] && HotTime[iPickPlate[0]][ix          ][iy   ]>=iSoakTime) ||
           (flag[0][1] && HotTime[iPickPlate[0]][ix+iLimitX  ][iy   ]>=iSoakTime) ||
           (flag[0][2] && HotTime[iPickPlate[0]][ix+iLimitX*2][iy   ]>=iSoakTime) ||
           (flag[0][3] && HotTime[iPickPlate[0]][ix+iLimitX*3][iy   ]>=iSoakTime) ||
           (flag[1][0] && HotTime[iPickPlate[0]][ix          ][Row2Y]>=iSoakTime) ||
           (flag[1][1] && HotTime[iPickPlate[0]][ix+iLimitX  ][Row2Y]>=iSoakTime) ||
           (flag[1][2] && HotTime[iPickPlate[0]][ix+iLimitX*2][Row2Y]>=iSoakTime) ||
           (flag[1][3] && HotTime[iPickPlate[0]][ix+iLimitX*3][Row2Y]>=iSoakTime))
        {
            if(IsAllSameHotCountHotOK(iHotPlateCount[iPickPlate[0]][ix][iy]))
                return true;
//            else  //Ifor 20171110 (Steven) : 移到下面判斷
//                return false;
        }
//        else      //Ifor 20171110 (Steven) : 移到下面判斷
//        {
//            return false;
//        }
    }
    else if(HotPlateForm.XDivision==10)
    {
        iStepX=4;
        iLimitX=1;

        for(i=0; i<2; i++)
        {
            for(j=0; j<iStepX; j++)
            {
                if(IniConfig.bI28_OnOffSiteOnTheFly==false)                     //Steven 20150924 : 隨時開關Site功能
                {
                    if(Prod.fInArmSuckUse_TwoArm32Site[i+iSht][j+iKit]==false)
                    {
                        flag[i][j]=false;
                        continue;
                    }
                }

                if(ix==8)
                    iPosX=ix+j;
                else
                    iPosX=ix+j*2;

                if(i==1 && iPickHPOrder==0 && AllRow1NotUse()==false)
                {
                    if(Row2CanPutHP(PickMode))
                    {
                        if(iy+iYHalf>=HotPlateForm.YDivision)
                            flag[i][j]=false;
                        else
                            flag[i][j]=MOT[MMPlate1+ip].Tray.Data[iPosX][iy+iYHalf];
                    }
                }
                else
                {
                    flag[i][j]=MOT[MMPlate1+ip].Tray.Data[iPosX][iy];
                }
            }
        }

        if((flag[0][0]==false && flag[0][1]==false && flag[0][2]==false && flag[0][3]==false &&
            flag[1][0]==false && flag[1][1]==false && flag[1][2]==false && flag[1][3]==false))
        {
            if(ix==8)
            {
                iy++;
            }
            else
            {
                ix++;
                if(ix>1)
                {
                    ix=0;
                    iy++;
                    iORGy=iy;
                }
            }

            if(HotPlateYPitchCanPutAll())
            {
                //if(ip==0)
                //jou 980824 add Only Select any one Hotplate 1 or 2
                if(ip==0 || (Prod.bPlateSelect[0]==true  && Prod.bPlateSelect[1]==false) ||
                            (Prod.bPlateSelect[0]==false && Prod.bPlateSelect[1]==true))
                {
                    while(iy%iYStep>=iYStep/2 && AllRow1NotUse()==false && AllRow2NotUse()==false)
                        iy++;
                }
                else
                {
                    if(HotPlateForm.YDivision%iYStep<=iYHalf)
                        surplus=HotPlateForm.YDivision%iYStep;
                    else
                        surplus=iYStep-(HotPlateForm.YDivision%iYStep);
                    while((iy%iYStep<surplus || iy%iYStep>=iYHalf+surplus) && AllRow1NotUse()==false && AllRow2NotUse()==false)
                        iy++;
                }
            }

            if(iy>=HotPlateForm.YDivision)
            {
                if(ix!=8)
                    ix=0;
                iy=0;
                ip++;
                if(ip>1)
                {
                    ip=0;
                    iPickPlate[1]=0;
                    if(ix==8)
                        ix=0;
                    else
                        ix=8;
                    iy=0;
                }

                CheckPlateInstall();
                if(HotPlateForm.iPlateSelect==3 && ip==1 && HotPlateYPitchCanPutAll() &&
                   AllRow1NotUse()==false && AllRow2NotUse()==false)
                {
                    if(HotPlateForm.YDivision%(iYHalf*2)<=iYHalf)
                    {
                        if(iy>=HotPlateForm.YDivision)
                            iy=HotPlateForm.YDivision;
                        iy=iy%iYStep;
                    }
                    else
                    {
                        iy=(iYHalf+iORGy)-HotPlateForm.YDivision;
                    }
                    iPickPlate[1]=ip;
                    iPickPlateX[1]=ix;
                    iPickPlateY[1]=iy;
                }
                else
                {
                    iy=0;
                }

                if(ip==0 && TestIF_File.iAutoClean_Tray==eCKPos_HP2 && TestIF_File.iAutoClean_Function==1)              //kevin 20121126 autoclean 放在hotplate
                    iy=4;
                else
                    iy=0;
            }
        }

        if(CUSTOMER_CODE==CC_ASE_KaohSiung &&                                   //kevin 20201015 add ASE off line no check Temp
           LastSet.iTester==OFF_LINE && LastSet.iTemperature==Tempture_Hot)
        {
            if(flag[0][0] || flag[0][1] || flag[0][2] || flag[0][3] ||
               flag[1][0] || flag[1][1] || flag[1][2] || flag[1][3])
                return true;
        }
        else
        {
            if((flag[0][0] && HotTime[iPickPlate[0]][ix  ][iy   ]>=iSoakTime) ||
               (flag[0][1] && HotTime[iPickPlate[0]][ix+2][iy   ]>=iSoakTime) ||
               (flag[0][2] && HotTime[iPickPlate[0]][ix+4][iy   ]>=iSoakTime) ||
               (flag[0][3] && HotTime[iPickPlate[0]][ix+6][iy   ]>=iSoakTime) ||
               (flag[1][0] && HotTime[iPickPlate[0]][ix  ][Row2Y]>=iSoakTime) ||
               (flag[1][1] && HotTime[iPickPlate[0]][ix+2][Row2Y]>=iSoakTime) ||
               (flag[1][2] && HotTime[iPickPlate[0]][ix+4][Row2Y]>=iSoakTime) ||
               (flag[1][3] && HotTime[iPickPlate[0]][ix+6][Row2Y]>=iSoakTime))
            {
                if(IsAllSameHotCountHotOK(iHotPlateCount[iPickPlate[0]][ix][iy]))
                    return true;
            }
        }
    }
    else if(i8PickerHPMode!=iHPNoUse8suck &&                                    //JerryYang 20161007 簡化Hotplate判斷式  //Steven 20151117 : 2x2 8Picker at Hot mode
            HotPlateForm.XDivision==6)                                          //ChungHung Add 20101025 CC_ASE_CL HotPlate Offset 10mm
    {
        if(ix==0)
        {
            iForPickHPX6Step=0;
        }
        else if(ix==4)
        {
            if(iy%2==0)
                iForPickHPX6Step=0;
            else
                iForPickHPX6Step=1;
        }
        else
        {
            iForPickHPX6Step=1;
        }

        if(ix==4)
            iStepX=2;
        else
            iStepX=4;

        for(i=0; i<2; i++)
        {
            for(j=0; j<iStepX; j++)
            {
                if(i==1 && iPickHPOrder==0 && AllRow1NotUse()==false && iCloseSiteState==0)
                {
                    if(Row2CanPutHP(PickMode))
                    {
                        if(iy+iYHalf>=HotPlateForm.YDivision)
                            flag[i][j+iForPickHPX6Step*2]=false;
                        else
                            flag[i][j+iForPickHPX6Step*2]=MOT[MMPlate1+ip].Tray.Data[ix+j][iy+iYHalf];
                        Row2Y=iy+iYHalf;
                    }
                }
                else
                {
                    Row2Y=iy;
                    flag[i][j+iForPickHPX6Step*2]=MOT[MMPlate1+ip].Tray.Data[ix+j][iy];
                }
            }
        }

        if((iForPickHPX6Step==0 && flag[0][0]==false && flag[0][1]==false && flag[0][2]==false && flag[0][3]==false &&
                                   flag[1][0]==false && flag[1][1]==false && flag[1][2]==false && flag[1][3]==false) ||
           (iForPickHPX6Step==1 && flag[0][2]==false && flag[0][3]==false && flag[1][2]==false && flag[1][3]==false))
        {
            if(ix==4)
            {
                iy++;
                if(iy%iRunRute==0)
                {
                    //jou 2010-07-05 2x8 hotplate只開單排會hang up
                    if(!(iCloseSiteState==1 || iCloseSiteState==2))
                        iy+=iRunRute;
                    ix=0;
                }
            }
            else
            {
                ix+=4;
                if(ix>=4)
                {
                    ix=0;
                    iy++;

                    if(iy%iRunRute==0)
                    {
                        ix=4;
                        iy=iy-iRunRute;
                    }
                }
            }

            if(iy>=HotPlateForm.YDivision)
            {
                if(ip==0)
                    ip=1;
                else
                    ip=0;
                CheckPlateInstall();

                ix=0;
                iy=0;

                if(ip==0 && TestIF_File.iAutoClean_Tray==eCKPos_HP2 && TestIF_File.iAutoClean_Function==1)              //kevin 20130226 autoclean 放在hotplate
                    iy=4;
                else
                    iy=0;
            }
        }

        if(CUSTOMER_CODE==CC_ASE_KaohSiung &&                                   //kevin 20201015 add ASE off line no check Temp
           LastSet.iTester==OFF_LINE && LastSet.iTemperature==Tempture_Hot)
        {
            if(flag[0][0] || flag[0][1] || flag[0][2] || flag[0][3] ||
               flag[1][0] || flag[1][1] || flag[1][2] || flag[1][3])
                return true;
        }
        else
        {
            if((flag[0][0] && HotTime[ip][ix  ][iy   ]>=iSoakTime) ||
               (flag[0][1] && HotTime[ip][ix+1][iy   ]>=iSoakTime) ||
               (flag[0][2] && HotTime[ip][ix+2][iy   ]>=iSoakTime) ||
               (flag[0][3] && HotTime[ip][ix+3][iy   ]>=iSoakTime) ||
               (flag[1][0] && HotTime[ip][ix  ][Row2Y]>=iSoakTime) ||
               (flag[1][1] && HotTime[ip][ix+1][Row2Y]>=iSoakTime) ||
               (flag[1][2] && HotTime[ip][ix+2][Row2Y]>=iSoakTime) ||
               (flag[1][3] && HotTime[ip][ix+3][Row2Y]>=iSoakTime))
            {
                if(IsAllSameHotCountHotOK(iHotPlateCount[iPickPlate[0]][ix][iy]))
                {
                    return true;
                }
            }
        }
    }
    else if(HotPlateForm.XDivision==6)
    {
        if(ix==0)
        {
            iForPickHPX6Step=0;
        }
        else if(ix==2)
        {
            if(iy%2==0)
                iForPickHPX6Step=0;
            else
                iForPickHPX6Step=1;
        }
        else
        {
            iForPickHPX6Step=1;
        }

        for(i=0; i<2; i++)
        {
            for(j=0; j<2; j++)
            {
                if(i==1 && iPickHPOrder==0 && AllRow1NotUse()==false && iCloseSiteState==0)
                {
                    if(Row2CanPutHP(PickMode))
                    {
                        if(iy+iYHalf>=HotPlateForm.YDivision)
                            flag[i][j*2+iForPickHPX6Step]=false;
                        else
                            flag[i][j*2+iForPickHPX6Step]=MOT[MMPlate1+ip].Tray.Data[ix+j*3][iy+iYHalf];
                        Row2Y=iy+iYHalf;
                    }
                }
                else
                {
                    Row2Y=iy;
                    flag[i][j*2+iForPickHPX6Step]=MOT[MMPlate1+ip].Tray.Data[ix+j*3][iy];
                }
            }
        }

        if((iForPickHPX6Step==0 && flag[0][0]==false && flag[0][2]==false && flag[1][0]==false && flag[1][2]==false) ||
           (iForPickHPX6Step==1 && flag[0][1]==false && flag[0][3]==false && flag[1][1]==false && flag[1][3]==false))
        {
            if(ix==2)
            {
                iy++;
                if(iy%iRunRute==0)
                {
                    //jou 2010-07-05 2x8 hotplate只開單排會hang up
                    if(!(iCloseSiteState==1 || iCloseSiteState==2) && Row2CanPutHP(PlaceMode) && iy%(iRunRute*2)!=0)    //ChungHung 20141217 fix hangup //ChungHung 20140526 add Y-Pitch 63.5 hangUp
                        iy+=iRunRute;
                    ix=0;
                }
            }
            else
            {
                ix++;
                if(ix>=2)
                {
                    ix=0;
                    iy++;

                    if(iy%iRunRute==0)
                    {
                        ix=2;
                        iy=iy-iRunRute;
                    }
                }
            }

            if(iy>=HotPlateForm.YDivision)
            {
                if(ip==0)
                    ip=1;
                else
                    ip=0;
                CheckPlateInstall();

                ix=0;
                if(ip==0 && TestIF_File.iAutoClean_Tray==eCKPos_HP2 && TestIF_File.iAutoClean_Function==1)              //kevin 20120411 autoclean 放在hotplate
                    iy=4;
                else
                    iy=0;
            }
        }

        if(CUSTOMER_CODE==CC_ASE_KaohSiung &&                                   //kevin 20201015 add ASE off line no check Temp
           LastSet.iTester==OFF_LINE && LastSet.iTemperature==Tempture_Hot)
        {
            if(flag[0][0+iForPickHPX6Step] || flag[0][2+iForPickHPX6Step] ||
               flag[1][0+iForPickHPX6Step] || flag[1][2+iForPickHPX6Step])
                return true;
        }
        else
        {
            if((flag[0][0+iForPickHPX6Step] && HotTime[ip][ix  ][iy   ]>=iSoakTime) ||
               (flag[0][2+iForPickHPX6Step] && HotTime[ip][ix+3][iy   ]>=iSoakTime) ||
               (flag[1][0+iForPickHPX6Step] && HotTime[ip][ix  ][Row2Y]>=iSoakTime) ||
               (flag[1][2+iForPickHPX6Step] && HotTime[ip][ix+3][Row2Y]>=iSoakTime))
            {
                if(IsAllSameHotCountHotOK(iHotPlateCount[iPickPlate[0]][ix][iy]))
                {
                    return true;
                }
            }
        }
    }
    else if(HotPlateForm.XDivision==12)                                         //ChungHung 20150513 add for ATK
    {
    //JerryYang 20181211 fix 32 site run hot plate X-division=12 hang up
    //==>
        if(ip==0 && TestIF_File.iAutoClean_Tray==eCKPos_HP2 && TestIF_File.iAutoClean_Function==1 && iy<4)              //kevin 20150515 autoclean 放在hotplate
            iy=4;

        if(HotPlateYPitchCanPutAll() && iy+iYHalf<HotPlateForm.YDivision)       //檢查Row2是否可放置 YPitch是否符合 && iy+iYHalf<HotPlateForm.YDivision
        {
            if(ArmRow1NotRealIC() && InArmSuck.HasIC())                         //Only Row2 HAS_IC  //Hung HasRealIC -> HasIC HangUp
            {
                if(ix==8)
                {
                    spacX=1;
                    spacY=iYHalf;
                    Row=1;
                    Col=4;
                }
                else
                {
                    spacX=2;
                    spacY=iYHalf;
                    Row=1;
                    Col=4;
                }
            }
            else
            {
                //JerryYang 20170215 (Steven) 修正12site跑8site,只開上排4個會hang up
                //==>
                if(CheckSpecialSiteMode()==true && InArm2x6AllDownCloseSite()==true)
                {
                    if(ix==8)
                    {
                        spacX=1;
                        spacY=0;
                        Row=2;
                        Col=4;
                    }
                    else
                    {
                        spacX=2;
                        spacY=0;
                        Row=2;
                        Col=4;
                    }
                }
                else
                //<==
                //JerryYang 20170215 (Steven) 修正12site跑8site,只開上排4個會hang up
                {
                    if(ix==8)
                    {
                        spacX=1;
                        spacY=iYHalf;
                        Row=2;
                        Col=4;
                    }
                    else
                    {
                        spacX=2;
                        spacY=iYHalf;
                        Row=2;
                        Col=4;
                    }
                }
            }
        }
        else
        {
            if(ix==8)
            {
                spacX=1;
                spacY=iYHalf;
                Row=1;
                Col=4;
            }
            else
            {
                spacX=2;
                spacY=iYHalf;
                Row=1;
                Col=4;
            }
        }

        bSuccess=CheckHotPlateHasSpace_9045_8_New_V(ip, iy, ix, spacY, spacX, Row, Col, state);
        if(bSuccess==false)
        {
            if(ix<2)
            {
                ix++;
                if(ix>=2)
                {
                    ix=0;
                    iy++;
                    if(iYHalf!=1 && iy%iYHalf==0)
                    {
                        iy+=iYHalf;
                    }

                    if(iy>=HotPlateForm.YDivision)
                    {
                        ix=8;
                        iy=0;
                    }
                }
            }
            else if(ix==8)                                                      //ix=8
            {
                iy++;
                if(iYHalf!=1 && iy%iYHalf==0)
                {
                    iy+=iYHalf;
                }

                if(iy>=HotPlateForm.YDivision)
                {
                    ix=0;
                    iy=0;
                    if(HotPlateForm.iPlateSelect==0x03)
                    {
                        if(ip==0)   ip=1;
                        else        ip=0;
                    }
                }
            }
        }

        if(bSuccess)
        {
            if(CUSTOMER_CODE==CC_ASE_KaohSiung &&                               //kevin 20201015 add ASE off line no check Temp
               LastSet.iTester==OFF_LINE && LastSet.iTemperature==Tempture_Hot)
            {
                return true;
            }
            else
            {
                if(IsAllSameHotCountHotOK(iHotPlateCount[iPickPlate[0]][ix][iy]))
                {
                    return true;
                }
            }
        }
        else
        {
            bInArmToPickHotPlatePos=false;
        }
        return false;
    //<==
    //JerryYang 20181211 fix 32 site run hot plate X-division=12 hang up
    }
    return false;
}
//==============================================================================
void DoInArm_9045_2x8_32_SuckerMap()
{
    bool bSiteMapHotplateReady=false;

    if(LastSet.iTemperature==Tempture_Hot && IniConfig.bI21AutoSiteMappingUseHotplate==true)
    {
        bSiteMapHotplateReady=bAutoSiteMapHotplateReady;
    }
    else
    {
        bSiteMapHotplateReady=true;
    }

    if(IniConfig.bUseAutoSiteMapping            &&                              //JerryYang 20160419 add auto site mapping
       LastSet.iRunStartMode==rsmAutoSiteMap    &&
       bSiteMappingCHKOK==false                 &&
//       iCleanOut==false                         &&                            //Ifor 20210426 add: Site Mapping Clean Out 要執行避免卡死
       bSiteMapHotplateReady==true              &&                              //Ifor 20170920 (Steven) : add Auto Site Mapping Hotplate 擺放旗標
       bRunAutoClean==false)                                                    //jou 20200618 : 修正 Auto Clean 跟 Auto site mapping 衝突
    {
        DoInArmAutoSiteMapping();
    }
    else
    {
        for(i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(j=0; j<MAX_SOCKET_COL; j++)
            {
                Prod.fInArmSuckUse_TwoArm32Site[i][j]=LastSet.bUseTestSocket[0][i][j];
            }
        }
    }

    (InArmSuck.iWhichShtPickFor32==0)?iRow=2:iRow=0;

    for(i=0; i<MAX_ARM_Row; i++)
    {
        for(j=0; j<MAX_ARM_Col; j++)
        {
            Prod.fInArmSuckUse[0][i][j]       = Prod.fInArmSuckUse_TwoArm32Site[i+iRow][j];
            Prod.fInArmSuckUse_9To16[0][i][j] = Prod.fInArmSuckUse_TwoArm32Site[i+iRow][j+4];
            Prod.fInArmSuckUse[1][i][j]       = Prod.fInArmSuckUse_TwoArm32Site[i][j];                                  //kevin 20180227 add
            Prod.fInArmSuckUse_9To16[1][i][j] = Prod.fInArmSuckUse_TwoArm32Site[i][j+4];                                //kevin 20180227 add
        }
    }
}
//==============================================================================
void DoInArm_9045_2x8_32()
{
    int &Task=iArmTask;
    int i, j;
    static int iAdjustSpeed=0;
    static bool bOne=false;
    static bool bDownSpeed=false;                                               //ChungHung 20121227 add
    bool flag=false;
    static int iDisplaySpeed=0;                                                 //KaiChen 20171225 (Steven)：Add Speed Display

    bool bCheckCloseSiteHaveIC=false;                                           //kevin 20130115
    AnsiString ErrPart="";
    bool bHasDuplicateErr=false;                                                //kevin 20130115
//    DoJudgeInputShuttleNeedChangeToNullIC();

    if(DoInArmAutoSiteMapping() ||                                              //jou 2011-03-24 start : Auto Site Mapping
       bIndexAlarmInArmAway)                                                    //kevin 20181102 (Steven) :  index回吸檢測有IC inarm 讓位 inarm先不要動
    {
        return ;                                                                //jou 2016-11-03 JCET 要求Auto Site Mapping 需等待測試結果在繼續入料
    }

    int iShtHP, iKitHP;

    int iSht=InArmSuck.iWhichShtPickFor32;
    int iKit=InArmSuck.iWhichKitPickFor32;

    if(LastSet.iTemperature==Tempture_Hot)                                      //Steven 20150626 : Fixed 32 site 吸取偵測異常    //Steven 20180525 : 修正一直跑Hotplate iWhichKit Close
    {
        iShtHP=InArmSuck.iWhichShtPickFor32;
        iKitHP=InArmSuck.iWhichKitPickFor32;
    }
    else
    {
        iShtHP=InArmSuck.iWhichShtPickFor32;
        iKitHP=InArmSuck.iWhichKitPickFor32;
    }

    switch(Task)
    {
        case 1:
            if(MoveInArm2XYToWait())
            {
                if((iCleanOut && IsInArmCleanOutFinish()) || iOneCycle)
                {
                    if(LastSet.iTemperature==Tempture_Hot &&
                       CosFunction.bUseInitialDelayAsSoakTime &&                //Steven 20170511 (wei) : 使用initial delay當 Soak time
                       Temperature.bUseInitialDelayAsSoakTime &&
                       MOT[MMPlate1].HasIC()==false &&
                       MOT[MMPlate2].HasIC()==false)
                    {
                        bOneTimeWait=false;
                    }

                    MOT[MInShuttle1].fCanMoveL=true;
                    MOT[MInShuttle2].fCanMoveL=true;
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
                else if(InArmSuck.HasType(HAS_NULL_IC) && InArmSuck.HasType(NULL_IC) && bPickFromHotplate==true)
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
                    if(InArmSuck.HasType(HAS_HOT_IC) && IsHaveSameHotCount(InArmSuck.HotCount) && bPickFromHotplate)
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
                        InArmSuck.SetNullIcToHasNullIc();
                        if(HotPlateForm.XDivision==6)
                        {
                            iForPlaceHPX6Step=0;
                        }
                        InitInArmPlaceToHotPlateTask();
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
                    }
                    Task=1100;
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
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                                                                                                 //kevin 20201015 add ASE off line no check Temp
            {
                if(LastSet.iTester==ON_LINE &&
                   LastSet.iTemperature==Tempture_Hot &&
                   fHeaterOK==false)                                                                                                                            //Steven 20111030 : 溫度異常Index就不可以動
                {
                    Task=1;
                    break;
                }
            }
            else
            {
                if(LastSet.iTemperature==Tempture_Hot &&
                   (fHeaterOK==false ||                                                                                                                         //Steven 20120314 : 加熱還沒好,不要亂動
                    fHeaterStableOK==false))                                                                                                                    //JerryYang 20210122 : ASE-CL新增待溫功能
                {
                    Task=1;
                    break;
                }
            }

            InitArmPickFromLoadStageTask();
            Task=100;

            if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)
            {
                if(NeedWaitTrayArm==true)                                                                                                                       //wei 20151002
                {
                    Task=1;                                                                                                                                     //jou 2016-07-14 修正OCR In arm 讓位造成Hotplate疊料issue
                    return;
                }
            }

            if(CosFunction.bRTCAutoModelVerify==true &&                                                                                                         //jou 2014-06-24 RTC 自動進行Model驗證
               IniConfig.bD36EnableRTCAutoModelVerify==true)
            {
                if(bNeedWaitRTCAutoVerify==true)
                {
                    Task=1;
                    return;
                }
            }

            if(MOT[MMTrayY].HasIC()==false             &&                                                                                                       // load and load buffer no any tray
               MOT[MMTrayY].fHasTray==false            &&
               MOT[MMTrayY_Car].Tray.HasIC()==false    &&
               MOT[MMTrayY_Car].fHasTray==false)
            {
                if(iCleanOut && iOneCycle==0 &&
                   LastSet.iTemperature==Tempture_Hot &&
                   (MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC()))
                {
                    DisableAutoSiteMapWhenCleanOut();                                                                                                           //Steven 20220902 : 修正Auto Site Map, Loader沒有IC
                    Task=500;
                    break;
                }

//                if(iOneCycle==0)    //Steven 20220319 : Fixed for 4x8 one cycle hang up
//                {
//                    Task=1;
//                    break;
//                }
            }

            if((MOT[MMTrayY].HasIC()==false &&
                MOT[MMTrayY].fHasTray==true) || bNeedSlapTray)
            {
                Task=1;
                break;
            }

            if(iCleanOut &&
               LastSet.iTemperature==Tempture_Hot &&
               IsInArmCleanOutFinish()==false)                                                                                                                  //Steven 20230309 : fixed for NN mode hang up
            {
                if(bRunAutoSiteMapping==true &&                                                                                                                 //Ifor 20210426 add: Auto Site Mapping Clean Out 補料回Hot plate
                   CheckHasSpaceToPlace_9045_2x8_32())
                {
                    if(MOT[MMTrayY].HasIC() || MOT[MMTrayY_Car].fHasTray)
                    {
                        Task=100;
                        break;
                    }
                }

                if((TrayForm.bAutoFeed ||
                    bMustCleanAllTray) &&
                   CheckHasSpaceToPlace_9045_2x8_32())
                {
                    if(MOT[MMTrayY].HasIC() || MOT[MMTrayY_Car].fHasTray)
                    {
                        Task=100;
                        break;
                    }
                }

                if((MOT[MMPlate1].HasIC() ||
                    MOT[MMPlate2].HasIC()) &&
                   HasHotReadyIC_9045_2x8_32())
                {
                    Task=500;
                    break;
                }

                if(TrayForm.bAutoFeed==false &&                                                                                                                 //jou 2010-08-31 start : 2x8 剛好只吸到奇數時,hang up
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
                    if(CheckHasSpaceToPlace_9045_2x8_32())
                        Task=100;
                    else
                        Task=500;
                }
                else
                {
                    Task=500;
                }
                break;
            }

            if(LastSet.iTemperature==Tempture_Hot &&                                                                                                            //Ifor 20180507 : add 避免Auto Site Mapping 未補料直接取IC造成資料異常
               CheckHasSpaceToPlace_9045_2x8_8()==false)
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

            if((iOneCycle || (iCleanOut && IsInArmCleanOutFinish())) && InArmSuck.HasIC()==false)
            {
                //Steven 20220128 : 修正NN模式 One Cycle死雞
                //==>
                if(LastSet.iRunStartMode!=rsmQAMode)
                {
                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC())
                        {
                            if(((FLCarryKit.HasRealIC() ||
                               (FTestSuck.HasRealIC() && FTestSuck.AlreadyTest()==false))) &&                                                                   //Shuttle 1或Arm 1有IC
                               ((ShuttleMustFullfor16Site(0)==false ||
                                (BLCarryKit.HasRealIC()==false || ShuttleMustFullfor16Site(1)==false) && BTestSuck.NoIC())))                                    //Shuttle 2或Arm 2沒IC
                            {
                                if(InSHT1InLF() && ShuttleMustFullfor16Site(0)==false)
                                {
                                    if(InArmSideAllClose_32(0, 1)==false && FLCarryKit.HasRealIC() && FLCarryKit.RightSideNoIC(4))                              //Steven 20220319 : Fixed for 4x8 one cycle hang up
                                    {
                                        Task=500;
                                        break;
                                    }
                                }
                                else if(InSHT2InLF() &&
                                        (FLCarryKit.HasRealIC() || (FTestSuck.HasRealIC() && FTestSuck.AlreadyTest()==false)) &&
                                        ((BTestSuck.HasRealIC()==false && BLCarryKit.HasRealIC()==false) ||
                                         ShuttleMustFullfor16Site(1)==false))
                                {
                                    if(InArmSideAllClose_32(1, 0)==false && BLCarryKit.LeftSideNoIC(4))
                                    {
                                        Task=500;
                                        break;
                                    }
                                    else if(InArmSideAllClose_32(1, 1)==false && BLCarryKit.RightSideNoIC(4))
                                    {
                                        Task=500;
                                        break;
                                    }
                                }
                            }
                            else if(InSHT2InLF() &&
                                    InArmSideAllClose_32(0, 0)==true &&                                                                                         //如果Arm 1的Site全部被關掉
                                    InArmSideAllClose_32(0, 1)==true &&
                                    BLCarryKit.HasIC() &&
                                    ShuttleMustFullfor16Site(1)==false)
                            {
                                Task=500;
                                break;
                            }
                        }
                        else
                        {
                            flag=true;
                        }
                    }
                    else
                    {
                        if(MOT[MMTrayY].HasIC())
                        {
                            if((FLCarryKit.HasRealIC() || (FTestSuck.HasRealIC() && FTestSuck.AlreadyTest()==false)) &&                                         //Shuttle 1或Arm 1有IC
                               ((ShuttleMustFullfor16Site(0)==false ||
                                 (BLCarryKit.HasRealIC()==false || ShuttleMustFullfor16Site(1)==false) && BTestSuck.NoIC())))                                   //Shuttle 2或Arm 2沒IC
                            {
                                if(InSHT1InLF() && ShuttleMustFullfor16Site(0)==false)
                                {
                                    InArmSuck.iWhichShtPickFor32=0;
                                    if(InArmSideAllClose_32(0, 0)==false && FLCarryKit.LeftSideNoIC(4))
                                    {
                                        InArmSuck.iWhichKitPickFor32=0;
                                        iInArmiWhichKit=0;
                                    }
                                    else
                                    {
                                        InArmSuck.iWhichKitPickFor32=1;
                                        iInArmiWhichKit=1;
                                    }
                                    Task=100;
                                    break;
                                }
                                else if(InSHT2InLF() &&
                                        (FLCarryKit.HasRealIC() || (FTestSuck.HasRealIC() && FTestSuck.AlreadyTest()==false)) &&
                                        ((BTestSuck.HasRealIC()==false && BLCarryKit.HasRealIC()==false) ||
                                         ShuttleMustFullfor16Site(1)==false))
                                {
                                    InArmSuck.iWhichShtPickFor32=1;
                                    if(InArmSideAllClose_32(1, 0) && InArmSideAllClose_32(1, 1))                                                                //Steven 20230306 : 修正NN mode常溫關Site Hang Up
                                    {
                                    }
                                    else
                                    {
                                        if(InArmSideAllClose_32(1, 0)==false && BLCarryKit.LeftSideNoIC(4))
                                        {
                                            InArmSuck.iWhichKitPickFor32=0;
                                            iInArmiWhichKit=0;
                                        }
                                        else
                                        {
                                            InArmSuck.iWhichKitPickFor32=1;
                                            iInArmiWhichKit=1;
                                        }
                                        Task=100;
                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            flag=true;
                        }
                    }
                }
                //<==
                //Steven 20220128 : 修正NN模式 One Cycle死雞

                if(MoveInArm2XYToWait()==false)
                {
                    Task=50;
                    return;
                }

                bInArmWaitOneCycle=true;                                                                                                                        //JerryYang 20220324 : Inarm是否完成ONE CYCLE
                if(bOne)
                {
                    bOne=false;
                    if(flag)
                        SetShuttleToHasNullICWhenCleanOut();                                                                                                    //Steven 20230309 : 修正Clean時, 都沒IC造成的Hang Up

                    MOT[MInShuttle1].fCanMoveL=true;
                    MOT[MInShuttle2].fCanMoveL=true;
                }
                Task=50;
                break;                                                                                                                                          //JerryYang 20220324 : add
            }
            bInArmWaitOneCycle=false;                                                                                                                           //JerryYang 20220324 : Inarm是否完成ONE CYCLE

            if(Task==100 &&
               LastSet.iTemperature==Tempture_Hot &&
               CheckHasSpaceToPlace_9045_2x8_32()==false)
                Task=50;

            if(Task!=100)                                                                                                                                       //Steven 20180813 : add in arm speed
                break;
        case 100:
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                                                         //kevin 20201015 add ASE off line no check Temp
            {
                if(LastSet.iTester==ON_LINE &&
                   LastSet.iTemperature==Tempture_Hot &&
                   fHeaterOK==false &&
                   iPickFromLoadStageTask==1)
                {
                    Task=1;
                    break;
                }
            }
            else
            {
                if(LastSet.iTemperature==Tempture_Hot &&
                   (fHeaterOK==false || fHeaterStableOK==false) &&                                                      //JerryYang 20210122 : ASE-CL新增待溫功能
                   iPickFromLoadStageTask==1)
                {
                    Task=1;
                    break;
                }
            }

            if(CUSTOMER_CODE==CC_ASE_M &&                                                                               //Ifor 20190529 : add 避免 ASEM 常溫Site Mapoing 完成後多入料一次
               LastSet.iRunStartMode==rsmAutoSiteMap &&
               LastSet.iTemperature!=Tempture_Hot &&
               bASMFinishOneCycle==true && iOneCycle==1)
            {
                Task=1;
                break;
            }

            if(USE_TRAY_MAPPING==1 &&
               TestIF_File.bEnableDeviceRemain==true &&                                                                 //Sam 20191113 : 防止殘料檢與 PickFormLoad 打架
               bDoTrayDeviceCheck)
            {
                break;
            }

            flag=DoArmPickFromLoadStage_9045_2x8_32();
            if(flag)
            {
                iInArmPickPlaceCnt[InOfsLoader]++;                                                                      //JerryYang 20180921 Setup Teach功能
                bHangTimePause=true;                                                                                    //Steven 20090827 : Hang Up dectector
                if(InArmSuck.HasIC()==false)
                {
                    if(InArmSideAllClose_32(InArmSuck.iWhichShtPickFor32, iInArmiWhichKit)==true)                       //如果Site全部被關掉
                    {
                        AdjustShuttleWhichKitOrder();
                    }
                    Task=50;
                }
                else
                {
                    bLoaderNeedVibrate=true;                                                                            //JerryYang 20191002 loader震動馬達
                    Task=200;
                }
            }

            if(Task!=200)                                                                                               //Steven 20180813 : add in arm speed
                break;
        case 200:                                                               //kevin 20120115 判斷關site是否吸ic
            CheekNeedToDoInArmAdditionalFunction();                             //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean

            for(i=0; i<MAX_ARM_Row; i++)
            {
                for(j=0; j<MAX_ARM_Col; j++)
                {
                    if(Prod.fInArmSuckUse_TwoArm32Site[i+iSht][j+iKit]==false &&
                       InArmSuck.Item[i][j]==HAS_IC)                            //kevin 20130115 關SITE 吸取IC
                    {
                        Task=300;
                        if(iKit==0)
                            RecordProcess("Load tray Close 0");
                        else
                            RecordProcess("Load tray Close 1");
                        return;
                    }
                }
            }
            Task=400;
            break;
        case 300:
            bCheckCloseSiteHaveIC=false;                                        //kevin 20130115
            for(i=0; i<MAX_ARM_Row; i++)
            {
                for(j=0; j<MAX_ARM_Col; j++)
                {
                    if(iInArmiWhichKit==0)
                    {
                        if(Prod.fInArmSuckUse_TwoArm32Site[i+iSht][j+4]==false &&
                           InArmSuck.Item[i][j]==HAS_IC)                        //kevin 20130115 關SITE 吸取IC
                            bCheckCloseSiteHaveIC=true;                         //kevin 20130115 判斷InArmSuck.iWhichKit是否為1
                    }
                    else
                    {
                        if(Prod.fInArmSuckUse_TwoArm32Site[i+iSht][j]==false &&
                           InArmSuck.Item[i][j]==HAS_IC)                        //kevin 20130115 關SITE 吸取IC
                            bCheckCloseSiteHaveIC=true;
                    }
                }
            }

            if(bCheckCloseSiteHaveIC)
            {
                Task=350;                                                       //不是另一邊資料
            }
            else
            {
                if(iInArmiWhichKit==0)
                    iInArmiWhichKit=1;
                else
                    iInArmiWhichKit=0;

                Task=400;
            }
            break;
        case 350:
            ErrPart=" ";
            bHasDuplicateErr=false;
            bCheckCloseSiteHaveIC=false;                                                                                //kevin 20130115
            for(i=0; i<MAX_ARM_Row; i++)
            {
                for(j=0; j<MAX_ARM_Col; j++)
                {
                    if(Prod.fInArmSuckUse_TwoArm32Site[i+iSht][j+iKit]==false &&
                       InArmSuck.Suck[i][j].GetStatus())                                                                //kevin 20130115 關SITE 吸取IC
                    {
                        bCheckCloseSiteHaveIC=true;
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                        InArmSuck.SetItemData(i, j, HAS_NULL_IC);
                    }
                }
            }

            if(bCheckCloseSiteHaveIC)
            {
                ShowErrorMessage("JAM0114", K_RETRY, MInArmX, bHasDuplicateErr, ErrPart);                               //Steven 20100104
            }
            else
            {
                Task=400;
            }
            break;
        case 400:                                                                                                       //kevin 20130115 判斷關site是否吸ic
            flag=DoInArmAdditionalFunction();                                                                           //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
            if(flag==true)
            {
                SetInArmNullToHasNull_9045();
                InitInArmPlaceToHotPlateTask();
                InitInArmPlaceToShuttleTask();
                if(LastSet.iTemperature==Tempture_Hot)
                {
                    if(IniConfig.bTemp25degControl==true ||
                       IniConfig.bI03AmbientTempControl &&
                       LastSet.iTemperature==Tempture_AmbientHot)                                                       //kevin 20140918 恆溫控制 //jou 2014-06-07 Temperature 25 deg. control
                    {
                        if(Temperature.iIndexHeatMode==HeadOnly && Temperature.fSoakTime==0 && Temperature.fWorkTemperBase<=25.0)
                        {
                            bOneTimeWait=false;
                            Task=2000;                                                                                  // place to shuttle
                        }
                        else
                        {
                            Task=1000;                                                                                  // place to hot plate
                        }
                    }
                    else
                    {
                        Task=1000;                                                                                      // place to hot plate
                    }
                }
                else
                {
                    Task=2000;                                                                                          // place to shuttle
                    if(IniConfig.bQAMode==true && LastSet.iRunStartMode==rsmQAMode)                                     //kevin 20181019 add //Steven 20111005 : QA Mode
                    {
                        if(iQAModeLoaderCT>Prod.iQAModeCount)                                                           //數量比設定值多，表示已經做完，所以就直接送Bin 1
                        {
                            InitInArmPlaceToShuttleTask();
                            Task=2000;
                            return;
                        }
                    }
                }

                if(IniConfig.bUseAutoSiteMapping)                                                                       //jou 2011-03-24 start : Auto Site Mapping
                {
                    if(LastSet.iTemperature==Tempture_Hot && LastSet.iRunStartMode==rsmAutoSiteMap && bSiteMappingCHKOK==false)
                    {
                        bOneTimeWait=false;
                        if(IniConfig.bI21AutoSiteMappingUseHotplate==false)                                             //Ifor 20170919 (Steven) : add Auto Site Mapping Hotplate Mode
                            Task=2000;
                        else
                            Task=1000;
                    }
                }
            }
            break;
        case 500:
            if((MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC()) &&              //Steven 20170511 (wei) : 使用initial delay當 Soak time
               HasHotReadyIC_9045_2x8_32())
            {
                if(MOT[MMTrayY].HasIC()==false && MOT[MMTrayY].fHasTray==true)  //spil 2006_0921
                {
                    Task=1;
                    break;
                }
                iHeaterWaitTime=0;                                              //jou 2012-05-18 先預先在Hotplate上面等待Soak time
                InitInArmPickFromHotPlateTask();
                bOneTimeWait=false;

                if(iDisplaySpeed==1 &&                                          //KaiChen 20171225 (Steven)：Add Speed Display
                   IniConfig.bA26MotorSpeedSortDisplay==true)
                {
                    iDisplaySpeed=2;
                    InArmAddSpeedDisplay();
                }
                Task=1500;
            }
            else
            {
                Task=50;
                iHeaterWaitTime=GetHeaterWaitTime();
                bHangTimePause=true;                                            //Steven 20090827 : Hang Up dectector

                if(iDisplaySpeed==1 &&                                          //KaiChen 20171225 (Steven)：Add Speed Display
                   IniConfig.bA26MotorSpeedSortDisplay==true)
                {
                    iDisplaySpeed=2;
                    InArmSubSpeedDisplay();
                }

                if(CheckHasSpaceToPlace_9045_2x8_32()==false)
                    Task=600;

                if((iCleanOut && IsInArmCleanOutFinish()) || iOneCycle)         //JerryYang 20230808 : fix換arm預熱功能clean out hang up
                {
                    bInArmWaitOneCycle=true;
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
            InArmSuck.SetNullIcToHasNullIc();                                                                           //Steven 20140710 : Add

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

            if(DoPlaceToHotPlate_9045_2x8_32())
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

            if(DoInArmPickFromHotPlate_9045_2x8_32(bDownSpeed))
            {
                bDownSpeed=false;                                               //ChungHung 20121227 add
                bPickFromHotplate=false;
                if(InArmSuck.HasIC()==false)
                {
                    if(bRunAutoSiteMapping==true)                               //Ifor 20171128 (Steven) add 避免Auto Site Mapping 多跑一次InArm安全位置
                    {
                        MOT[MInShuttle1].fCanMoveL=true;
                        MOT[MInShuttle2].fCanMoveL=true;
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

                bInArmHasHotIC=true;                                            //Sam 20211012 : Debug 用
                InitInArmPlaceToShuttleTask();
                bPickFromHotplate=false;
                Task=2000;
            }
            break;
        case 1600:
            bCheckCloseSiteHaveIC=false;                                        //kevin 20130115
            for(i=0; i<MAX_ARM_Row; i++)
            {
                for(j=0; j<MAX_ARM_Col; j++)
                {
                    if(Prod.fInArmSuckUse_TwoArm32Site[i+iShtHP][j+iKitHP]==false &&
                       InArmSuck.Suck[i][j].GetStatus())                        //kevin 20130115 關SITE 吸取IC
                        bCheckCloseSiteHaveIC=true;
                }
            }

            if(bCheckCloseSiteHaveIC)
            {
                Task=1700;                                                      //不是另依邊資料
            }
            else
            {
                InitInArmPlaceToShuttleTask();
                bPickFromHotplate=false;
                Task=2000;
            }
            break;
        case 1700:
            ErrPart=" ";
            bHasDuplicateErr=false;
            bCheckCloseSiteHaveIC=false;                                                                                //kevin 20130115

            for(i=0; i<MAX_ARM_Row; i++)
            {
                for(j=0; j<MAX_ARM_Col; j++)
                {
                    if(Prod.fInArmSuckUse_TwoArm32Site[i+iShtHP][j+iKitHP]==false &&                                    //Steven 20150626 : Fixed 32 site 吸取偵測異常
                       InArmSuck.Suck[i][j].GetStatus())                                                                //kevin 20130115 關SITE 吸取IC
                    {
                        bCheckCloseSiteHaveIC=true;
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                        InArmSuck.SetItemData(i, j, HAS_NULL_IC);
                    }
                }
            }

            if(bCheckCloseSiteHaveIC)
            {
                ShowErrorMessage("JAM0114", K_RETRY, MInArmX, bHasDuplicateErr, ErrPart);                               //Steven 20100104
            }
            else
            {
                InitInArmPlaceToShuttleTask();
                bPickFromHotplate=false;
                Task=2000;
            }
            break;
        case 2000:
            if(CheekNeedToDoInArmAdditionalFunction())                                                                  //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
            {
                Task=200;
                break;
            }
            bPlaceShuttle=true;                                                                                         //ChungHung 20141111 add for SCK junction temp issue
            iInRotateFinish=3;                                                                                          //kevin 20130524 IC已放置SHUTTLE

            if(DoPlaceToShuttle_9045_2x8_32())
            {
                bInArmHasHotIC=false;                                                                                   //Sam 20211012 : Debug 用
                bPlaceShuttle=false;                                                                                    //ChungHung 20120226 add
                fYieldMonitoring->DoAutoCloseSite(0);                                                                   //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
                iInRotateFinish=0;                                                                                      //kevin 20130524 放完SHUTTLE
                iSetShuttleToHasNullIC=0;
                bHangTimePause=true;                                                                                    //Steven 20090827 : Hang Up dectector
                if(LastSet.iRealDummy==DUMMY && iCleanOut)
                {
                    MOT[MMTrayY].InitNewTray(NULL_IC);
                }

                if(LastSet.iTemperature==Tempture_Hot)
                {
                    if(!bTrySuckHotPlateOneCycle)
                        HasHotReadyIC_9045_2x8_32();                                                                    //Steven 20220530 : Add
                    if(iOneCycle || (IniConfig.bUseAutoSiteMapping==true && LastSet.iRunStartMode==rsmAutoSiteMap && bSiteMappingCHKOK==false))
                    {
                        if(LastSet.iTemperature==Tempture_Hot &&
                           IniConfig.bP17InArmFullPickFromLoader &&
                           (MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC()) &&
                           ShuttleMustFullfor16Site(iWhichSht32)==false)                                                //Steven 20180628 : 修正P17InArmFullPickFromLoader  //Steven 20220107 : Fixed for 32site
                        {
                            Task=500;
                        }
                        else
                        {
                            Task=50;
                            HasHotReadyIC_9045_2x8_32();                                                                //Steven 20220530 : Add
                            AdjustShuttlePlaceOrder_AutoSiteMapping();                                                  //jou 2016-11-07 JCET fix Auto Site mapping Hot mode hangup
                        }
                    }
                    else
                    {
                        if(HotPlateForm.XDivision==12)                                                                  //Steven 20171005 (wei) : Fixed for HP hang up
                        {
                            Task=50;
                        }
                        else
                        {
                            if(IniConfig.bTemp25degControl==true)                                                       //jou 2014-06-07 Temperature 25 deg. control
                            {
                                if(Temperature.iIndexHeatMode==HeadOnly && Temperature.fSoakTime==0 && Temperature.fWorkTemperBase<=25.0)
                                    Task=50;
                                else
                                    Task=500;
                            }
                            else
                            {
                                Task=500;
                            }
                        }
                    }
                }
                else if(IniConfig.bA15_1ESDGiveWayFunction==true)                //==> Eastsun 20260521 整合
                {
                    InitDoInArmIonFanGiveWayTask();
                    Task=2100;
                }
                else
                {
                    Task=50;
                }

                //jou 2011-03-24 start : Auto Site Mapping
                if(IniConfig.bUseAutoSiteMapping)
                {
                    if(IniConfig.bUseAutoSiteMapping)
                    {
                        if(IniConfig.bI21ASMRunTimeCHeck)                                                               //Steven 20140729 : AutoSiteMapping, 邊生產邊做
                        {
                            if(bSiteMappingCHKOK==false &&
                               ((iDoSiteMappingStep<=15 && FLCarryKit.All_HasIC() && BLCarryKit.All_HasIC()) ||         //小於15時,只擺Shuttle 1, Shuttle 2會全部都是HAS_NULL_IC
                                (iDoSiteMappingStep>15  && BLCarryKit.All_HasIC())))
                            {
                                DoSiteMappingCHK();
                                if(iDoSiteMappingStep==16)                                                              //16剛好是Shuttle要放第一顆,所以需要把Shuttle上的HAS_NULL_IC清掉
                                {
                                    BLCarryKit.ClearAll();
                                }

                                if(LastSet.iTemperature!=Tempture_Hot)
                                {
                                    iWhichShuttle=0;
                                    InArmSuck.iWhichShtPickFor32=0;
                                    iInArmiWhichKit=0;
                                    InArmSuck.iWhichKitPickFor32=0;
                                }
                            }
                        }
                        else
                        {
                            if(bSiteMappingCHKOK==false)
                                DoSiteMappingCHK();
                        }
                    }
                }
                //jou 2011-03-24 end
            }
            break;
        case 2100:                                                              //Eastsun 20260521 整合
            if(DoInArmIonFanGiveWay())
            {
                Task=50;
            }
            break;
    }
}
//==============================================================================
bool CleanPad_PlaceToShuttle_2x8_32(int iWhichShuttle)
{
    int flag=true;
    int i, j;

    if(bUse8Picker)
    {
        for(i=0; i<2; i++)
        {
            for(j=0; j<4; j++)
            {
                if(InArmSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                   (InArmSuck.Item[i][j] && InArmSuck.Suck[i][j].GetNeedDestroyStatus() && InArmSuck.Suck[i][j].Destroy()))
                {
                    if(iShuttleRowKit==1 || iShuttleRowKit==2)
                    {
                        if(iWhichShuttle==euShuttle1)
                            FLCarryKit.MoveSuckDataDiff(InArmSuck, i, j, i, j);
                        else
                            BLCarryKit.MoveSuckDataDiff(InArmSuck, i, j, i, j);
                    }
                    else if(iShuttleRowKit==3 || iShuttleRowKit==4)
                    {
                        if(iWhichShuttle==euShuttle1)
                            FLCarryKit.MoveSuckDataDiff(InArmSuck, i, j, i, j+4);
                        else
                            BLCarryKit.MoveSuckDataDiff(InArmSuck, i, j, i, j+4);
                    }
                }
                else if(InArmSuck.Item[i][j] && InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                {
                    flag=false;
                }
            }
        }
    }
    else
    {
        for(j=0; j<4; j++)
        {
            if(InArmSuck.Item[1][j]==HAS_NULL_CLEAN_IC ||
               (InArmSuck.Item[1][j] && InArmSuck.Suck[1][j].GetNeedDestroyStatus() && InArmSuck.Suck[1][j].Destroy()))
            {
                if(iShuttleRowKit==1 || iShuttleRowKit==2)
                {
                    if(iWhichShuttle==euShuttle1)
                        FLCarryKit.MoveSuckDataDiff(InArmSuck, 1, j, iShuttleRowKit-1, j);
                    else
                        BLCarryKit.MoveSuckDataDiff(InArmSuck, 1, j, iShuttleRowKit-1, j);
                }
                else if(iShuttleRowKit==3 || iShuttleRowKit==4)
                {
                    if(iWhichShuttle==euShuttle1)
                        FLCarryKit.MoveSuckDataDiff(InArmSuck, 1, j, iShuttleRowKit-3, j+4);
                    else
                        BLCarryKit.MoveSuckDataDiff(InArmSuck, 1, j, iShuttleRowKit-3, j+4);
                }
            }
            else if(InArmSuck.Item[1][j] && InArmSuck.Suck[1][j].GetNeedDestroyStatus())
            {
                flag=false;
            }
        }
    }

    return flag;
}
//==============================================================================
int GetShuttleState_4x8_32(int iWhichShuttle, bool bPick)
{
    if(bPick==bAutoPick)                                                        //Pick
    {
        if(ptrInSHT->ArmUpSideAllTypeIC(NULL_IC, 0, 4)==false)                  //左上有料
            return 1;
        else if(ptrInSHT->ArmDownSideAllTypeIC(NULL_IC, 0, 4)==false)           //左下有料
            return 2;
        else if(ptrInSHT->ArmUpSideAllTypeIC(NULL_IC, 4, 4)==false)             //右上有料
            return 3;
        else if(ptrInSHT->ArmDownSideAllTypeIC(NULL_IC, 4, 4)==false)           //右下有料
            return 4;
        else
            return 0;
    }
    else                                                                        //Place
    {
        if(ptrInSHT->ArmUpSideAllTypeIC(NULL_IC, 0, 4) &&                       //左上無料
           ((bAutoCleanPlaceToSht==false && InArmSuck.RowHasDefineIC(0, NULL_IC)) ||                                    //Steven 20221005 : Fixed for auto clean clean kit吸兩次
            (bAutoCleanPlaceToSht==true  && InArmSuck.ArmUpSideNoIC()==false)))
            return 1;
        else if(ptrInSHT->ArmDownSideAllTypeIC(NULL_IC, 0, 4) &&                //左下無料
                ((bAutoCleanPlaceToSht==false && InArmSuck.RowHasDefineIC(1, NULL_IC)) ||                               //Steven 20221005 : Fixed for auto clean clean kit吸兩次
                 (bAutoCleanPlaceToSht==true  && InArmSuck.ArmDownSideNoIC()==false)))
            return 2;
        else if(ptrInSHT->ArmUpSideAllTypeIC(NULL_IC, 4, 4) &&                  //右上無料
                ((bAutoCleanPlaceToSht==false && InArmSuck.RowHasDefineIC(0, NULL_IC)) ||                               //Steven 20221005 : Fixed for auto clean clean kit吸兩次
                 (bAutoCleanPlaceToSht==true  && InArmSuck.ArmUpSideNoIC()==false)))
            return 3;
        else if(ptrInSHT->ArmDownSideAllTypeIC(NULL_IC, 4, 4) &&                //右下無料
                ((bAutoCleanPlaceToSht==false && InArmSuck.RowHasDefineIC(1, NULL_IC)) ||                               //Steven 20221005 : Fixed for auto clean clean kit吸兩次
                 (bAutoCleanPlaceToSht==true  && InArmSuck.ArmDownSideNoIC()==false)))
            return 4;
        else
            return 0;
    }
}

#endif // 0 -- end verbatim golden body of dead variant ainarm9045_2x8_32.cpp
