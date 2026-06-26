// =============================================================================
//  ainarm9045.cpp  --  In-Arm ENGINE  (W6.2b translation)
//
//  Faithful translation of golden  ainarm9045.cpp (9222 lines, BCB6, Big5/cp950).
//  Translator: AI(W6.2b-INARM) 20260626
//
//  ROLE (golden header comment): "9045 2x4 吸嘴組 / 支援 2x4 (每次 4 隻 都用)".
//  This is the in-arm ENGINE: it owns the central pick state machine, the
//  iInArmType dispatch ladders into 25 site-variant modules, the pure
//  pitch/offset geometry, and several auxiliary task SMs.
//
//  WAVE SCOPE (W6.2b) -- what is ACTIVE vs GATED (see each gate's golden cite):
//
//   ACTIVE (faithful, verbatim formulas / branches / cursor flow):
//     * pure geometry/picker helpers:
//         GetInArmPitch_9045, GetInArmPitchX_9045, GetInArmPitch2_9045,
//         GetInArmPitchY_9045, GetInArmXToShuttleOffset_9045,
//         GetInArmYToShuttleOffset_9045, GetInArmToShuttleOffset_9045,
//         GetInArmToLoaderOffset_9045, GetInArmPitchXMM_9045,
//         GetInArmPitchYMM_9045, GetInArmCellPos, GetInArmToShtCellPos,
//         GetInOffsetFromWhichTarget, bUseAxExPicker, bUseAxxGPicker,
//         bUseACEGPicker, GetJStep, GetShuttleCol (ep1 path),
//         FindCentorPointIndex, GetShtStartPos(x2).
//     * the 2 iInArmType dispatch ladders DoInArm_9045 / DoInArm_9045_SuckerMap
//       (pre-dispatch guards + if-ladder structure + final else Program-Error).
//     * 3 task SMs whose cursor flow pumps over the Sim substrate:
//         DoInArmAdditionalFunction, DoInArmIonFanGiveWay,
//         ProcessSCKARTLoadingCount.
//     * the 11 functions previously stubbed in aHotPlateSubstrate.cpp
//       (now REAL-defined here; the substrate stubs were removed -- pre-step).
//
//   GATED (compiling ACTIVE stub right after; documented at the site):
//     * the 50 per-variant `DoInArm_9045_<layout>()` / `_SuckerMap()` call-arms
//       (`#if 0 // TODO(W6.2b variants)`) -- variants out of scope this wave;
//       the final else Program-Error path stays the live offline behavior.
//     * GetShuttleCol per-iInArmType XPHSuckToSht_* lookup branches
//       (`#if 0 // TODO(W7 variant tables)`) -- those arrays live in the
//       per-layout headers, undefined in target; ep1Picker early-return active,
//       default iShtCol=0.
//     * the dense hardware/UI SMs DoInArmPickFromLoadStage_9045 /
//       DoInArmCheckShuttleFloating / CheckInShuttleSensor_Latch /
//       DoInArmAutoCalSuckZ -- bodies are MOT[]/sensor/fProductionInfo-bound
//       with no translated home (`// TODO(W7)`); ACTIVE stubs preserve the
//       cursor var + a sane terminal value so the engine links.
//     * every other golden function (Loader search, tray-end, UPH, alarm sinks,
//       etc.) -- ACTIVE offline stub, documented in the STUBS section.
//
//  VCL/Borland conversions: AnsiString (1-based) via vclcompat; __fastcall
//  removed from CalculateUPH; Now().FormatString / AnsiString::sprintf kept
//  verbatim (vclcompat supports them); __FUNC__ via aArmHeader.h W6.0 shim; the
//  2 goto/label pairs are inside GATED SMs so they are not reproduced here (the
//  stub bodies do not need them); ChangeToFloatNonPcnt kept (zero-guarded).
//  SOFT_SIMULTE is NOT defined -> #ifndef paths stay active (none reproduced in
//  the kept bodies).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
#include "MachineDefine.h"

#include "ainarm9045.h"
#include "aArmHeader.h"             // __FUNC__ shim, RecordProcess, ShowErrorMessage, etc.

#include "cprod.h"                 // Prod / TestIF / TestIF_File / InArmOffSet / ArmSpeed
#include "Motor/mymotor.h"         // MOT[], TTrayMotor
#include "csystem.h"               // InSHT1InLF / InSHT2InLF / state predicates
#include "cmydef.h"                // global scalar universe + IC consts + ASET_StartTimeNAME
#include "cpublic.h"               // CosFunction
#include "common.h"
#include "aHotPlateSubstrate.h"    // InArmSuck/FLCarryKit/BLCarryKit, fBarCode, cursors, tRotate
#include "FormsFacade.h"           // fMain/fSortCT/fSCKART offline stand-ins
#include "canary_support.h"        // LastSet / ShowMyMessage / ShowErrorMessage / RecordProcess

//---------------------------------------------------------------------------
//  golden file globals (verbatim)  -- golden ainarm9045.cpp:46-59
//---------------------------------------------------------------------------
const int XDir=0;
const int YDir=1;
// NOTE: bInArmCheckDestroyACT (golden :48) is ALREADY defined in
// aHotPlateSubstrate.cpp (the place-swap leaf reads it).  Not redefined here to
// avoid a multiple-definition link error.  AI(W6.2b-INARM) 20260626.

// NOTE: bPickFormHotplatePartOK / bPickFormHotplateRetry /
// bInArmTryPickFromHotPlateFinish / bInArmToPickHotPlatePos are golden-defined
// here (:51-57) but are ALREADY defined in aHotPlateSubstrate.cpp because the
// already-translated leaf modules (ainarm_SearchPickPlate.cpp) need them.  To
// avoid an ODR collision we DO NOT redefine them here -- they keep their W6.2
// home.  AI(W6.2b-INARM) 20260626.

int iTempSuckX=-1;
int iTempSuckY=-1;

// golden ainarm9045.h externs that have no other home this wave (offline data).
int  iZPosToCleanKit[MAX_ARM_Row][MAX_ARM_Col]={{0}};
bool bZFlgToCleanKit[MAX_ARM_Row][MAX_ARM_Col]={{false}};
int  iMoveToShuttle=0;
bool bCheckInArmDestroyActive[MAX_ARM_Row][MAX_ARM_Col]={{false}};
int  iProcessSCKARTLoadingCountTask=1;

//==============================================================================
//==  ACTIVE: pure geometry / pitch helpers (faithful, verbatim formulas)
//==============================================================================

//==============================================================================
// 取得判斷入料變 pitch 距離  -- golden ainarm9045.cpp:129
//==============================================================================
int GetInArmPitch_9045(int w)
{
    double m,r;
    m=ChangeToFloatNonPcnt((double)(((double)(Prod.iInArmX120Pitch-Prod.iInArmX40Pitch))), (double)(iPitch_Max_minus_Min));
    r=(double)Prod.iInArmX40Pitch+m*((double)w-double(iXpitchMinX3));
    return (int)r;
}
//==============================================================================
int GetInArmPitchX_9045(int w, int iX, int iOffsetPos)                          //Steven 20230323 : For HT1032  -- golden :137
{
    int iPos=0;
    double m, r;
    double dX120[4]={Prod.iInArmX120Pitch, Prod.iInArmX120Pitch2, Prod.iInArmX120Pitch3, Prod.iInArmX120Pitch4};
    double dX_40[4]={Prod.iInArmX40Pitch,  Prod.iInArmX40Pitch2,  Prod.iInArmX40Pitch3,  Prod.iInArmX40Pitch4};

    m=ChangeToFloatNonPcnt((double)(((double)(dX120[iX]-dX_40[iX]))), (double)(iPitch_Max_minus_Min));
    if(USE_16PICKER_TYPE==1)                                                    //Ztex 2023.12.06 Add HT-1032
        r=(double)dX_40[iX]+m*((double)w-double(iXpitchMinX7));
    else
        r=(double)dX_40[iX]+m*((double)w-double(iXpitchMinX3));

    if(iOffsetPos>=0)
        iPos=r+InArmOffSet[iOffsetPos]->GetXPitch(iX);
    else
        iPos=r;

    if(bRunAutoClean && TestIF.iAutoClean_Function &&
       IniConfig.bE48_ShuttleUse4Offset_Autoclean==false)                       //Steven 20180920 : #P180920-ATK-H9-01 , InArm put the Auto Clean pad wrong position on the shuttle.
    {
        iPos=iPos+TestIF_File.ShuttlePitchOffset;
        if(CosFunction.bAutoCleanOffsetUseSingleSetting && IniConfig.bE47_ShuttleUse4Offset==false)                     //Sam 20220720 : AutoClean Offset 僅使用 Clean 設定 Offset 來補償
            iPos=iPos-InArmOffSet[iOffsetPos]->GetXPitch(iX);
    }
    return iPos;
}
//==============================================================================
int GetInArmPitch2_9045(int w)                                                  // golden :165
{
    double m, r;
    m=ChangeToFloatNonPcnt((double)(((double)(Prod.iInArmX120Pitch2-Prod.iInArmX40Pitch2))), (double)(iPitch_Max_minus_Min));                                   //Steven 20131002 : XY變距        //Isaac 20171204 (Steven) : Xpitch40->50mm, 新增150mm的Teaching點位，內差法公式計算
    r=(double)Prod.iInArmX40Pitch2+m*((double)w-double(iXpitchMinX3));
    return (int)r;
}
//==============================================================================
int GetInArmPitchY_9045(int w, int iOffsetPos)                                  //ChungHung 20131231 alter AutoYPitch  -- golden :173
{
    int iPos=0;
    double m, r;
    if(w>IN_OUT_ARM_Y_PITCH_MAX)                                                //Steven 20201205 : 加上保護
        w=IN_OUT_ARM_Y_PITCH_MAX;                                               //因為Teaching做的是6000
    m=ChangeToFloatNonPcnt((double)(Prod.iInArmY60Pitch-Prod.iInArmY15Pitch), (double)(6000-IN_OUT_ARM_Y_PITCH_MIN)); //Steven 20260505 : add zero-guard
    r=(double)Prod.iInArmY15Pitch+m*((double)w-(double)IN_OUT_ARM_Y_PITCH_MIN);
    if(iOffsetPos>=0)
        iPos=r+InArmOffSet[iOffsetPos]->GetVariableY();
    else
        iPos=r;
    return iPos;
}
//==============================================================================
int GetInArmXToShuttleOffset_9045(int iSht, int iOffsetPos)                     // golden :188
{
    int iPos=0;
    if(iOffsetPos>=0)
    {
        if(iSht==0)
            iPos=InArmOffSet[iOffsetPos]->GetX();
        else
            iPos=InArmOffSet[iOffsetPos]->GetX();
    }
    return iPos;
}
//==============================================================================
int GetInArmYToShuttleOffset_9045(int iSht, int iOffsetPos)                     // golden :201
{
    int iPos=0;
    if(iOffsetPos>=0)
    {
        if(iSht==0)
            iPos=InArmOffSet[iOffsetPos]->GetY();
        else
            iPos=InArmOffSet[iOffsetPos]->GetY();
    }
    return iPos;
}
//==============================================================================
int GetInArmToShuttleOffset_9045(int iSht, int iModeRow, int iModeCol, bool bPitch)   // golden :214
{
    int iOffsetPos=-1;
    if(TestIF.bEnableAutoAlignment==false)                                      //KenHsieh 20220923 : add AOA功能開啟不補償
    {
        if(bRunAutoClean && TestIF.iAutoClean_Function)
        {
            if(IniConfig.bE48_ShuttleUse4Offset_Autoclean)
            {
                if(iSht==0)
                {
                    if(iShuttleRowKit==2)
                        iOffsetPos=InOfsInSh1LB_AutoClean;
                    else if(iShuttleRowKit==3)
                        iOffsetPos=InOfsInSh1RA_AutoClean;
                    else if(iShuttleRowKit==4)
                        iOffsetPos=InOfsInSh1RB_AutoClean;
                    else
                        iOffsetPos=InOfsInSh1_AutoClean;
                }
                else
                {
                    if(iShuttleRowKit==2)
                        iOffsetPos=InOfsInSh2LB_AutoClean;
                    else if(iShuttleRowKit==3)
                        iOffsetPos=InOfsInSh2RA_AutoClean;
                    else if(iShuttleRowKit==4)
                        iOffsetPos=InOfsInSh2RB_AutoClean;
                    else //if(iShuttleRowKit==1)                                //Steven 20210304 : 修正1x1沒有計算Pitch的問題
                        iOffsetPos=InOfsInSh2_AutoClean;
                }
            }
            else
            {
                if(iSht==0)
                    iOffsetPos=InOfsInSh1;
                else
                    iOffsetPos=InOfsInSh2;
            }
        }
        else if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true && bPitch)
        {
            iOffsetPos=InOfsInSh1;
        }
        else
        {
            if(IniConfig.bE47_ShuttleUse4Offset)                                //Steven 20140827 : Shuttle可以分別調前後排的Offset
            {
                if(iModeRow==1)
                {
                    if(iModeCol%100<10)
                    {
                        if(iSht==0)
                            iOffsetPos=InOfsInSh1LB;
                        else
                            iOffsetPos=InOfsInSh2LB;
                    }
                    else
                    {
                        if(iSht==0)
                            iOffsetPos=InOfsInSh1RB;
                        else
                            iOffsetPos=InOfsInSh2RB;
                    }
                }
                else if(iModeRow==0)
                {
                    if(iModeCol%100<10)
                    {
                        if(iSht==0)
                            iOffsetPos=InOfsInSh1;
                        else
                            iOffsetPos=InOfsInSh2;
                    }
                    else
                    {
                        if(iSht==0)
                            iOffsetPos=InOfsInSh1RA;
                        else
                            iOffsetPos=InOfsInSh2RA;
                    }
                }
                else
                {
                    if(iModeCol%100<10)                                         //Steven 20240826 : 補上Offset
                    {
                        if(iSht==0)
                            iOffsetPos=InOfsInSh1;
                        else
                            iOffsetPos=InOfsInSh2;
                    }
                    else
                    {
                        if(iSht==0)
                            iOffsetPos=InOfsInSh1RA;
                        else
                            iOffsetPos=InOfsInSh2RA;
                    }
                }
            }
            else
            {
                if(iSht==0)
                    iOffsetPos=InOfsInSh1;
                else
                    iOffsetPos=InOfsInSh2;
            }
        }
    }
    return iOffsetPos;
}
//==============================================================================
int GetInArmToLoaderOffset_9045(int iSelRow)                                    // golden :326
{
    int iOffsetPos=-1;
    if(TestIF.bEnableAutoAlignment==false)                                      //KenHsieh 20220923 : add AOA功能開啟不補償
    {
        if(IniConfig.bE46_LoaderUse2Offset && iSelRow==1)                       //Steven 20140827 : Loader可以分別調前後排的Offset
        {
            iOffsetPos=InOfsLoaderB;
        }
        else
        {
            iOffsetPos=InOfsLoader;
        }
    }
    return iOffsetPos;
}
//==============================================================================
int GetInArmPitchXMM_9045(int iMotPulse)                                        // golden :8507
{
    double m=0.0, r=0.0;
    m=ChangeToFloatNonPcnt((double)(((double)(Prod.iInArmX120Pitch-Prod.iInArmX40Pitch))), (double)(iPitch_Max_minus_Min));
    r=(double)ChangeToFloatNonPcnt((double)((iMotPulse-Prod.iInArmX40Pitch)), (double)(m));
    r+=double(iXpitchMinX3);
    return (int)r;
}
//==============================================================================
int GetInArmPitchYMM_9045(int iMotPulse)                                        // golden :8516
{
    double m=0.0,r=0.0;
    if(USE_IN_Y_IS_AUTO_PITCH==true)                                            //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        m=ChangeToFloatNonPcnt((double)(Prod.iInArmY60Pitch-Prod.iInArmY15Pitch), (double)(6000-IN_OUT_ARM_Y_PITCH_MIN)); //Steven 20260505 : add zero-guard
        r=(double)ChangeToFloatNonPcnt((double)((iMotPulse-Prod.iInArmY15Pitch)), (double)(m));
        r+=double(IN_OUT_ARM_Y_PITCH_MIN);
    }
    else
    {
        r=TestIF.iARM_Y_PITCH;
    }

    return (int)r;
}
//==============================================================================
void GetInArmCellPos(int iRow, int iCol, int &Ypos, int &Xpos)                  // golden :8533
{
    int iMovePitchX=GetInArmPitchXMM_9045(MOT[MInArmPitch].ReadPos());
    int iMovePitchY=GetInArmPitchYMM_9045(MOT[MInArmPitchY].ReadPos());
    iMovePitchX/=3;

    if(USE_PICKER_COUNT==ep1Picker)
        iMovePitchX=0;

    Xpos=MOT[MInArmX].ReadPos()+(iMovePitchX*(iCol-iInArmXBase));
    Ypos=MOT[MInArmY].ReadPos()-(iMovePitchY*(iRow-iInArmYBase));
}
//==============================================================================
void GetInArmToShtCellPos(int iRow, int iCol, int &Ypos, int &Xpos)             // golden :8546
{
    int iMovePitchX=GetInArmPitchXMM_9045(MOT[MInArmPitch].ReadPos());
//    int iMovePitchY=GetInArmPitchYMM_9045(MOT[MInArmPitchY].ReadPos());
    iMovePitchX/=3;
    Xpos=MOT[MInArmX].ReadPos()+(iMovePitchX*(iCol-iInArmXBase));
    Ypos=MOT[MInArmY].ReadPos();
}
//==============================================================================
int GetInOffsetFromWhichTarget(int iWhichTarget)                                //Jimmychiu 20240731 : for out arm offset value  -- golden :8555
{
    if(iWhichTarget==MMTrayY)
    {
        return InOfsLoader;
    }
    else if(iWhichTarget==MMPlate1)
    {
        return InOfsHP2;
    }
    else if(iWhichTarget==MMPlate2)
    {
        return InOfsHP1;
    }
    else if(iWhichTarget==MMAutoCleanKit)
    {
        if(IniConfig.bE43AutoCleanUseHotplate)                                  //Steven 20160629 : for AutoClean use Hotplate1
        {
            return InOfsHP1;
        }
        else
        {
            return InOfsAutoClean;
        }
    }
    else if(iWhichTarget==MInRotateKit)                                         //待確認
    {
        return iRotate_In_Index+InOfsHP1;
    }
    else if(iWhichTarget==MPreciser)                                            //待確認
    {
        return InOfsPreciser;
    }
    return 0;
}

//==============================================================================
//==  ACTIVE: picker-mode helpers + GetJStep (faithful, verbatim)
//==============================================================================
bool bUseAxExPicker()                                                           // golden :8281 (REAL-defined here; substrate stub removed)
{
    if(iInArmType==e9045_1x2_2_13 ||
       iInArmType==e9045_1x4_4_13 ||
       iInArmType==e9045_2x1_2_13 ||
       iInArmType==e9045_2x2_4_13 ||
       iInArmType==e9045_2x4_4_13 ||
       iCloseSiteModeFor2x8==e2x8Run2x2_13)
    {
        return true;
    }
    else if(bRunAutoClean)                                                      //Sam 20250203 : 修正 1X4 8 吸嘴模式
    {
        if(iInArmType==e9045_2x2_8_Hot &&
           iXpitchMinX2_MM<TestIF_File.dSiteXPitch &&
           TestIF_File.dSiteXPitch<=iXpitchMaxX2_MM)                            //SitePitchX 26-80 之間跑 1 3 吸嘴
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool bUseAxxGPicker()                                                           // golden :8304 (REAL-defined here; substrate stub removed)
{
    if(iInArmType==e9045_1x2_2_14 ||
       iInArmType==e9045_1x3_2_14 ||                                            //Steven 20220425 : 1x3_14
       iInArmType==e9045_1x4_2_14 ||
       iInArmType==e9045_2x2_2_14 ||
       iInArmType==e9045_2x2_4_14 ||
       iInArmType==e9045_2x3_6_14 ||                                            //Steven 20200219 : 2x3mode, Layout: 120x70mm
       iInArmType==e9045_2x4_4_14 ||
       iCloseSiteModeFor2x8==e2x8Run2x2_14)
    {
        return true;
    }
    else if(bRunAutoClean)                                                      //Sam 20250212 : 修正 2X2 8 吸嘴模式 AutoClean
    {
        if(iInArmType==e9045_2x2_8_Hot &&
           TestIF_File.dSiteXPitch>iXpitchMaxX2_MM)                             //SitePitchX >80 之間跑 1 4 吸嘴
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool bUseACEGPicker()                                                           // golden :8326
{
    if(iInArmType==e9045_1x4_4 ||
       iInArmType==e9045_1x4_4_Back ||                                          //Steven 20220425 : 1x3_14
       iInArmType==e9045_1x4_8_Hot ||
       iInArmType==e9045_2x4_8 ||
       iInArmType==e9045_2x8_8 ||
       iInArmType==e9045_2x8_32 ||
       iInArmType==e9045_2x2_8_Hot)
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int GetJStep()                                                                  // golden :8341
{
    int jStep=1;
    if(bUseAxExPicker())
    {
        jStep=2;
    }
    else if(bUseAxxGPicker() ||
           iCloseSiteModeFor1x4==e1x4CloseAbAc)                                 //Steven 20241111 : for 1x4 close 2 site)
    {
        jStep=3;
    }
    else
    {
        jStep=1;
    }
    return jStep;
}
//---------------------------------------------------------------------------
int GetShuttleCol(int iSuckRow, int iSuckCol, bool bOutArm)                     // golden :8360
{
    int iShtCol=0;

    if(USE_PICKER_COUNT==ep1Picker)
    {
        return iSuckCol;
    }

    // golden :8369-8462 dispatches into the per-iInArmType XPHSuckToSht_*
    // lookup tables (XPHSuckToSht_2x4_8 / _2x2_13 / _2x8_8 / ...).  Those arrays
    // live in the per-layout headers (ainarm9045_<layout>.h) which are NOT in
    // the target tree this wave -> the ladder cannot compile.  Gate it; the
    // ep1Picker early-return above stays ACTIVE and the default iShtCol=0 is the
    // live offline value.
#if 0 // TODO(W7 variant tables) -- golden ainarm9045.cpp:8369 (XPHSuckToSht_* undefined in target)
    if(iInArmType==e9045_1x4_1_Ac)
    {
        iShtCol=2;
    }
    else if(iInArmType==e9045_1x1_1)
    {
        iShtCol=0;
    }
    else if(iInArmType==e9045_1x2_4_Hot)
    {
        iShtCol=XPHSuckToSht_1x2_4[i1x2_4UseACEGPicker][iSuckRow][iSuckCol];
    }
    /* ... 23 further per-iInArmType lookup-table branches (golden :8381-8462) ... */
    else if(iInArmType==e9045_2x8_8 ||
            iInArmType==e9045_2x8_32)
    {
        if(bOutArm)
            iShtCol=XPHSuckToSht_2x8_8_OutArm[InArmSuck.iModeX][iSuckRow][iSuckCol];
        else
            iShtCol=XPHSuckToSht_2x8_8[InArmSuck.iModeX][iSuckRow][iSuckCol];
    }
#else
    (void)iSuckRow; (void)bOutArm;       // W7-gated lookup tables
#endif

    return iShtCol;
}
//------------------------------------------------------------------------------
bool DoPickLoaderOK(int iSht, int iKit)                                         // golden :8466
{
    bool bflag=false;
    if(IniConfig.bI28_OnOffSiteOnTheFly &&
       LastSet.iTemperature==Tempture_Hot &&
       InArmSideAllCloseWithKit(iSht, iKit))
    {
#if 0 // TODO(W7) -- golden :8472 SetInArmUseSuckToHasNullIC (no translated home)
        SetInArmUseSuckToHasNullIC(iSht, iKit);
#endif
    }

    int iKit32=(iKit==0)?0:4;

    if(USE_PICKER_COUNT==ep1Picker)                                             //JerryYang 20250904 : add
    {
        if(InArmSuck.Item[0][0]==NULL_IC)
        {
            bflag=true;
        }
    }
    else
    {
        for(int i=0; i<InArmSuck.iPickRow; i++)
        {
             for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(Prod.fInArmSuck4x8[iSht][i][j+iKit32] &&
                   InArmSuck.Item[i][j]==NULL_IC)
                {
                    bflag=true;
                }
            }
        }
    }

    if(!bflag)                                                                  //InArm 已吸滿
    {
        return true;
    }
    return false;
}

//==============================================================================
//==  ACTIVE: shuttle cell geometry (pure arithmetic; faithful)
//==============================================================================
double FindCentorPointIndex(int iItemCount)                                     // golden :9071
{
    int iPitchCount=iItemCount-1;
    double dret=(double)iPitchCount/2;
    return dret;
}
//---------------------------------------------------------------------------
int GetShtStartPos(int iCentorABSPostion, double dCentorIndex, int iGap)        // golden :9078
{
    int iret=iCentorABSPostion-(int)(dCentorIndex*iGap);
    return iret;
}
//---------------------------------------------------------------------------
int GetShtStartPos(int iItemCount,                                              // golden :9084
                   int iCentorABSPostion,
                   int iGap)
{
    double dCentorIndex=FindCentorPointIndex(iItemCount);
    int iStart=GetShtStartPos(iCentorABSPostion, dCentorIndex, iGap);
    return iStart;
}
//---------------------------------------------------------------------------
void GetShtRowColStartPos(int iTarget, int &iColStart, int &iRowStart)          // golden :9093
{
    // golden reads TestSocket.iShtRow / .iShtCol -- TestSocket is a cross-module
    // object with no translated home in the target this wave.  Gate the
    // TestSocket reads (default the grid to 0x0); keep the arithmetic + the
    // per-target Prod offset additions ACTIVE.  TODO(W7) restore TestSocket.
    int iRow=0;                                                                 // golden: TestSocket.iShtRow
    int iCol=0;                                                                 // golden: TestSocket.iShtCol
#if 0 // TODO(W7) -- golden :9095 TestSocket.iShtRow/iShtCol (TestSocket no home)
    iRow=TestSocket.iShtRow;
    iCol=TestSocket.iShtCol;
#endif
    iColStart=iRowStart=0;
    int HardwarePosX=iInArmShtXCenterPos;
    int HardwarePosY=iInArmShtYCenterPos;
    if(bUseTwoArm32Site==true)                                                  //RogerYang 20250815 修正32Site計算Y異常
        iRow=iRow/2;
    if(iTarget==MOutShuttle1 ||
       iTarget==MOutShuttle2)
    {
        HardwarePosX=iOutArmShtXCenterPos;
        HardwarePosY=iOutArmShtYCenterPos;
    }

    if(iTarget==MInShuttle1)
    {
        HardwarePosX+=Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
        HardwarePosY+=Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
    }
    else if(iTarget==MInShuttle2)
    {
        HardwarePosX+=Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
        HardwarePosY+=Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
    }
    else if(iTarget==MOutShuttle1)
    {
        HardwarePosX+=Prod.XOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase];
        HardwarePosY+=Prod.YOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase];
    }
    else if(iTarget==MOutShuttle2)
    {
        HardwarePosX+=Prod.XOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase];
        HardwarePosY+=Prod.YOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase];
    }
    else
    {
        return;
    }
    iColStart=GetShtStartPos(iCol, HardwarePosX, (int)(TestIF.dSiteXPitch));
    iRowStart=GetShtStartPos(iRow, HardwarePosY, (int)(TestIF.dSiteYPitch)*(-1));                                       //反向補償*(-1)
}
//---------------------------------------------------------------------------
void GetShtRowColPos(int iTarget, int iColIndex, int iRowindex, int &HardwarePosX, int &HardwarePosY)   // golden :9137
{
    GetShtRowColStartPos(iTarget,HardwarePosX,HardwarePosY);
    HardwarePosX+=(int)(TestIF.dSiteXPitch)*iColIndex;
    HardwarePosY-=(int)(TestIF.dSiteYPitch)*iRowindex;

    if(TestIF.bNS7000kit)                                                       //jou 981208 start : NS7000 bias kit
    {
        int iShiftY=(TestIF.dSiteYPitch==0)?(6000/2):(TestIF.dSiteYPitch/2);
        if(iTarget==MInShuttle1)
            HardwarePosY+=iShiftY;
        else
            HardwarePosY-=iShiftY;
    }
    else if(bUseTwoArm32Site==true)                                             //RogerYang 20250815 修正32Site計算Y異常
    {
        if(iTarget==MInShuttle1)
            HardwarePosY+=1000;
        else
            HardwarePosY-=1000;
    }
}

//==============================================================================
//==  ACTIVE: InspectInArmPosition (REAL-defined here; substrate stub removed)
//==  golden :8591.  Early-return path is the live offline behavior (the debug
//==  flag bE74_InspectArmPosition is off by default); the body's coordinate
//==  comparison + alarm sink is gated (touches encoders + ShowErrorMessage).
//==============================================================================
void InspectInArmPosition(int iTarget, int iSuckRow, int iSuckCol, int iTargetRow, int iTargetCol, bool bPlace)
{
    if(IniConfig.bE74_InspectArmPosition==false)                                //Jimmychiu 20240408 : debug for inarm position
        return;

#if 0 // TODO(W7) -- golden :8597-8697 encoder compare + ShowErrorMessage alarm sink
    // Full coordinate-deviation check: GetInArmCellPos vs the per-target
    // Prod.*_Pick hardware position, then ShowErrorMessage("WAR...") on mismatch.
    // Gated: reads MOT encoders & dispatches an alarm with no offline home.
#else
    (void)iTarget; (void)iSuckRow; (void)iSuckCol; (void)iTargetRow; (void)iTargetCol; (void)bPlace;
#endif
}

//==============================================================================
//==  ACTIVE: left-side predicates (REAL-defined here; substrate stubs removed)
//==  golden :4193 / :4212.
//==============================================================================
bool InArmLeftSideNoIC(int iRow)                                                // golden :4193
{
    // golden walks InArmSuck.Item[iRow][0..iModeX] checking for !=HAS_IC.
    // Offline grid is empty -> "left side has no IC" is true.  Keep faithful to
    // the empty-grid result without dereferencing the (unsized) iModeX walk.
    for(int j=0; j<InArmSuck.iMaxCol; j++)
    {
        (void)iRow;
        // offline: Item defaults to 0 (==NULL_IC region); treat as no real IC.
    }
    return InArmSuck.NoIC();
}
//------------------------------------------------------------------------------
bool InArmLeftSideHasIC(int iRow)                                               // golden :4212
{
    (void)iRow;
    return InArmSuck.HasRealIC();
}

//==============================================================================
//==  ACTIVE: IsCheckInArmDestroyActiveFinish (REAL-defined here; stub removed)
//==  golden :1153.  Offline: no destroy pending -> finished.
//==============================================================================
bool IsCheckInArmDestroyActiveFinish()                                          //ChungHung 20111229
{
    for(int i=0; i<MAX_ARM_Row; i++)
        for(int j=0; j<MAX_ARM_Col; j++)
            if(bCheckInArmDestroyActive[i][j]==true)
                return false;
    return true;
}

//==============================================================================
//==  ACTIVE: AddInArmPickerCount / SetShuttleToHasNullIC_9045 (REAL-defined
//==  here; substrate stubs removed).  golden :2584 / :343.  The golden bodies
//==  drive prod-record counters / shuttle-item grids; offline the counters live
//==  in Prod and the shuttle item-grid write is a no-op (no shuttle HAL yet).
//==============================================================================
void AddInArmPickerCount(int iSuckRow, int iSuckCol)                            //JerryYang 20220331 : add 吸真空次數計數  -- golden :2584
{
#if 0 // TODO(W7) -- golden :2586 InArmSuck.PordRec[][].Add* prod-record counter
    InArmSuck.PordRec[iSuckRow][iSuckCol].AddInArmPickRecord();
#else
    (void)iSuckRow; (void)iSuckCol;
#endif
}
//------------------------------------------------------------------------------
void SetShuttleToHasNullIC_9045(int iSht, int iKit)                             // golden :343
{
#if 0 // TODO(W7) -- golden :345-384 FLCarryKit/BLCarryKit shuttle-grid item writes
    // Marks the closed shuttle/kit cells to HAS_NULL_IC across the carry-kit
    // grids; offline the carry-kit grids have no IC so this is a no-op.
#else
    (void)iSht; (void)iKit;
#endif
}

//==============================================================================
//==  ACTIVE: iInArmType dispatch ladder #1  DoInArm_9045   -- golden :4416
//==  Pre-dispatch guards + if-ladder STRUCTURE kept ACTIVE; the 25 per-variant
//==  call-arms are GATED (variants out of scope) so the live offline behavior
//==  is the final else Program-Error path.
//==============================================================================
// golden :4391-4415 declares the 25 variant externs.  W6.2b-canary: only the
// e9045_1x1_1 / e9045_1x4_1_Ac arm is un-gated this wave -> declare ONLY the two
// now-live callees (defined in ainarm9045_1x1_1.cpp -- file-local in the golden,
// consumed via this dispatch ladder, so NOT in ainarm9045_1x1_1.h).  golden :4391
// `extern void DoInArm_9045_1x1_1();` + golden :4650 `extern void
// DoInArm_9045_1x1_1_SuckerMap();`.  The remaining 23 externs are still inside
// the #if 0 gate below, so they need no declaration.
extern void DoInArm_9045_1x1_1();                                               //AI(W6.2b-canary) 20260626: golden :4391 -- now-live dispatch callee
extern void DoInArm_9045_1x1_1_SuckerMap();                                     //AI(W6.2b-canary) 20260626: golden :4650 -- now-live SuckerMap callee
// W6.2c batch-1: 5 variants made live -- declare their now-live dispatch callees
// (defined in ainarm9045_<v>.cpp).  AI(W6.2c-INARM) 20260626.
extern void DoInArm_9045_1x2_2();                                               //golden :4392
extern void DoInArm_9045_1x2_2_SuckerMap();                                     //golden :4651
extern void DoInArm_9045_1x2_2_14();                                            //golden :4393
extern void DoInArm_9045_1x2_2_14_SuckerMap();                                  //golden :4652
extern void DoInArm_9045_1x2_4_Hot();                                           //golden :4394
extern void DoInArm_9045_1x2_4_Hot_SuckerMap();                                 //golden :4653
extern void DoInArm_9045_1x3_4();                                               //golden :4395
extern void DoInArm_9045_1x3_4_SuckerMap();                                     //golden :4655
extern void DoInArm_9045_1x3_2_14();                                            //golden :4396
extern void DoInArm_9045_1x3_2_14_SuckerMap();                                  //golden :4654
// W6.2c batch-2: 6 variants made live -- declare their now-live dispatch callees
// (defined in ainarm9045_<v>.cpp).  AI(W6.2c-INARM-batch2) 20260626.
extern void DoInArm_9045_1x4_2();                                               //golden :4397
extern void DoInArm_9045_1x4_2_SuckerMap();                                     //golden :4657
extern void DoInArm_9045_1x4_8_Hot();                                           //golden :4401
extern void DoInArm_9045_1x4_8_Hot_SuckerMap();                                 //golden :4661
extern void DoInArm_9045_2x1_2();                                              //golden :4402
extern void DoInArm_9045_2x1_2_SuckerMap();                                     //golden :4662
extern void DoInArm_9045_2x2_4();                                              //golden :4403
extern void DoInArm_9045_2x2_4_SuckerMap();                                     //golden :4663
extern void DoInArm_9045_2x2_4_12();                                            //golden :4404
extern void DoInArm_9045_2x2_4_12_SuckerMap();                                  //golden :4664
extern void DoInArm_9045_2x2_4_14();                                            //golden :4405
extern void DoInArm_9045_2x2_4_14_SuckerMap();                                  //golden :4665
// W6.2c batch-3: 6 variants made live -- declare their now-live dispatch callees.
// AI(W6.2c-INARM-batch3) 20260626.
extern void DoInArm_9045_1x4_4();                                               //golden :4408
extern void DoInArm_9045_1x4_4_SuckerMap();                                     //golden :4658
extern void DoInArm_9045_1x4_4_Back();                                          //golden :4407
extern void DoInArm_9045_1x4_4_Back_SuckerMap();                                //golden :4659
extern void DoInArm_9045_2x3_6();                                               //golden :4411
extern void DoInArm_9045_2x3_6_SuckerMap();                                     //golden :4666
extern void DoInArm_9045_2x3_6_14();                                            //golden :4412
extern void DoInArm_9045_2x3_6_14_SuckerMap();                                  //golden :4667
extern void DoInArm_9045_2x4_4();                                               //golden :4409
extern void DoInArm_9045_2x4_4_SuckerMap();                                     //golden :4669
extern void DoInArm_9045_All_1Pick();                                           //golden :4415
extern void DoInArm_9045_All_1Pick_SuckerMap();                                 //golden :4675 (declared; NEVER dispatched -- faithful to golden)
// W6.2c batch-4: 5 variants made live -- declare their now-live dispatch callees
// (defined in ainarm9045_<v>.cpp).  AI(W6.2c-INARM-batch4) 20260626.  NOTE:
// e9045_2x8_32 has NO own callee -- the golden engine routes it to the 2x8_8 family
// (golden :4639/:4804), so no extra extern is needed for it.
extern void DoInArm_9045_2x2_8_Hot();                                           //golden :4406
extern void DoInArm_9045_2x2_8_Hot_SuckerMap();                                 //golden :4666
extern void DoInArm_9045_2x4_8();                                               //golden :4411
extern void DoInArm_9045_2x4_8_SuckerMap();                                     //golden :4671
extern void DoInArm_9045_2x5_8();                                               //golden :4412
extern void DoInArm_9045_2x5_8_SuckerMap();                                     //golden :4672
extern void DoInArm_9045_2x6_8();                                               //golden :4413
extern void DoInArm_9045_2x6_8_SuckerMap();                                     //golden :4673
extern void DoInArm_9045_2x8_8();                                               //golden :4414
extern void DoInArm_9045_2x8_8_SuckerMap();                                     //golden :4674
// W6.2c batch-5: 2 S-family (HT-9045S AutoClean) variants made live.  IRREGULAR
// func names -- copy EXACTLY: file ainarm9045S_1x4_4 defines DoInArm_9045S_1x4_4
// (S in func), file ainarm9045S_2x4_4_13 defines DoInArm_9045_2x4_4_13 (NO S in func).
extern void DoInArm_9045S_1x4_4();              //golden ainarm9045.cpp:4569 callee (def ainarm9045S_1x4_4.cpp:1528)
extern void DoInArm_9045S_1x4_4_SuckerMap();    //golden :4734 callee (def ainarm9045S_1x4_4.cpp:1497)
extern void DoInArm_9045_2x4_4_13();            //golden :4617 callee (def ainarm9045S_2x4_4_13.cpp:1818)
extern void DoInArm_9045_2x4_4_13_SuckerMap();  //golden :4782 callee (def ainarm9045S_2x4_4_13.cpp:1783)

void DoInArm_9045()                                                             //Steven 20240223 : 重新整理DoInArm_9045
{
    AnsiString Str;
    static int iD44CheckIndexICDestroy=0;                                       //kevin 20190201 detetect Count

    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&                                        //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
       TestIF.bEnableAutoAlignment==true &&
       (LastSet.iRealDummy==HAS_TRAY ||
        LastSet.iRealDummy==REALLY))
    {
        if(bRunInArmAutoAlignment || lOutArmAutoAlignmentFlag)
        {
            return;
        }
    }

    if(CosFunction.bAutoSiteMappingUseHotPlate==true    &&                      //Ifor 20180518 : add 簡化Site Mapping 旗標
       IniConfig.bI21AutoSiteMappingUseHotplate==true   &&
       bAutoSiteMapHotplateReady==true                  &&
       LastSet.iRunStartMode==rsmAutoSiteMap            &&
       IniConfig.bI21EnableASM==true                    &&
       bSiteMappingCHKOK==false                         )
    {
        bRunAutoSiteMapping=true;
    }
    else
    {
        bRunAutoSiteMapping=false;
    }

    if(IniConfig.bEnableTestingNeedStopAllMotor==true &&
       IniConfig.bI24TestingNeedStopAllMotor==true &&
       bTestingStopAllMotor==true)                                              //jou 2013-09-25 Testing Need Stop All Motor
    {
        StopAllMotor();
#if 0 // TODO(W7) -- golden :4451 MOT[MTestZ1].Gali_Command("ST") motion stop
        MOT[MTestZ1].Gali_Command("ST", __FUNC__);
#endif
        return;
    }

    if(IniConfig.bAlarmNeedServoOff==true)                                      //JerryYang 20180514 新增保護避免servo off後推動in arm, 再重新start會發生點位錯誤
    {
#if 0 // TODO(W7) -- golden :4456 fNote->bMyServoOffInArm (TfNote VCL form, no home)
        if(fNote->bMyServoOffInArm==true)
        {
            return;
        }
#endif
    }

    if(ASET_StartTimeNAME=="")                                                  //kevin 20150212 高雄日月光IC履歷記錄 先給檔案初始時間    //Steven 20160531 : modify
    {
        ASET_StartTimeNAME=FormatDateTime("yyyymmdd", Now());                   //kevin 20161228 記錄天數為檔名  // golden: Now().FormatString("yyyymmdd") -> vclcompat FormatDateTime
    }

    if(iD44CheckIndexICDestroy>100)                                             //kevin 20190201 detetect Count
        bIndexplaceCheck=true;                                                  //kevin 20190201 index 回溪檢測中

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        if(ArmSpeed_File[InArm].bDevicConfirm)                                  //kevin 20170510 (wei) 回吸功能開關
            CheckInArmDestroyActive();                                          //jou 981130 start : 確認device確實destroy完成

        if(IniConfig.bD44CheckIndexICDestroy &&
           iIndexTakeDeviceChk2!=0 &&
           (iArmTask==2000 || bIndexAlarmInArmAway))                            //kevin 20190201 回吸檢測狀態 避免inarm來回跑
        {
            iD44CheckIndexICDestroy++;                                          //kevin 20190201 detetect Count
            return;
        }
    }
    else
    {
        CheckInArmDestroyActive();                                              //jou 981130 start : 確認device確實destroy完成
    }
    iD44CheckIndexICDestroy=0;                                                  //kevin 20190201 detetect Count
    bIndexplaceCheck=false;                                                     //kevin 20190201 index 回溪檢測中

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(bCheckInArmDestroyActive[i][j]==true)
            {
                return;
            }
        }
    }

#if 0 // TODO(W7) -- golden :4503 bEject (cmydef.h extern, no definition in target yet)
    if(bEject)                                                                  //JerryYang 20251020 : 渠梁半清機功能
    {
        return;
    }
#endif

    if(iPauseBackUp!=-1 &&
       InArmSuck.IsPickSuckFinish()==true &&
       InArmSuck.IsPickDestroyFinish()==true)
    {
        return;
    }

    if(bResetInArmTask==true)
    {
        bResetInArmTask=false;
        InitInArmTask();
        return;
    }

    if(bInArmLaserActionflag[0]==true ||                                        //==> Eastsun 20260525 laser 整合
       bInArmLaserActionflag[1]==true)
    {
        return;
    }

    // --- iInArmType dispatch ladder ---------------------------------------
    // STRUCTURE kept ACTIVE.  W6.2b-canary UN-GATES ONLY the e9045_1x1_1 /
    // e9045_1x4_1_Ac arm -- the SOLE per-site variant translated + buildable
    // this wave (ainarm9045_1x1_1.cpp; golden :4536/:4563 both route here).
    // The other 23 arms (DoInArm_9045_<layout>()) are NOT translated yet -> they
    // stay GATED (#if 0) below so the engine still links; for any still-gated
    // iInArmType the final else Program-Error path is the live offline behavior.
    // NOTE (verified, not assumed): the golden ladder has NO e9045_2x4_16 enum
    // and routes e9045_2x8_32 -> DoInArm_9045_2x8_8() (golden :4639-4641), so the
    // ainarm9045_2x4_16.cpp / _2x8_32.cpp files are DEAD -- registered to compile,
    // never dispatched (see CMakeLists ht9045_sm + each variant file head).
    //AI(W6.2c-INARM-batch3) 20260626: All_1Pick is the FIRST golden ladder arm
    //(golden :4535-4537), evaluated by picker-count BEFORE the iInArmType chain.
    if(USE_PICKER_COUNT==ep1Picker)             { DoInArm_9045_All_1Pick();  }  //AI(W6.2c-INARM-batch3) 20260626: golden :4535
    else if(iInArmType==e9045_1x1_1 ||                                          //AI(W6.2b-canary) 20260626: un-gate ONLY the translated 1x1_1 arm
       iInArmType==e9045_1x4_1_Ac)              { DoInArm_9045_1x1_1();      }  //golden :4536 (e9045_1x1_1) + :4563 (e9045_1x4_1_Ac both call DoInArm_9045_1x1_1)
    // --- W6.2c batch-1: 5 variant arms pulled out of the #if 0 gate (ACTIVE) ---
    else if(iInArmType==e9045_1x2_2_13)          { DoInArm_9045_1x2_2();      }  //AI(W6.2c-INARM) 20260626: golden :4538
    else if(iInArmType==e9045_1x2_2_14)          { DoInArm_9045_1x2_2_14();   }  //AI(W6.2c-INARM) 20260626: golden :4539
    else if(iInArmType==e9045_1x2_4_Hot)         { DoInArm_9045_1x2_4_Hot();  }  //AI(W6.2c-INARM) 20260626: golden :4540
    else if(iInArmType==e9045_1x3_2_14)          { DoInArm_9045_1x3_2_14();   }  //AI(W6.2c-INARM) 20260626: golden :4541
    else if(iInArmType==e9045_1x3_4)             { DoInArm_9045_1x3_4();      }  //AI(W6.2c-INARM) 20260626: golden :4559
    // --- W6.2c batch-2: 6 variant arms pulled out of the #if 0 gate (ACTIVE) ---
    else if(iInArmType==e9045_1x4_2_14)          { DoInArm_9045_1x4_2();      }  //AI(W6.2c-INARM-batch2) 20260626: golden :4571
    else if(iInArmType==e9045_1x4_8_Hot)         { DoInArm_9045_1x4_8_Hot();  }  //AI(W6.2c-INARM-batch2) 20260626: golden :4583
    else if(iInArmType==e9045_2x1_2_13)          { DoInArm_9045_2x1_2();      }  //AI(W6.2c-INARM-batch2) 20260626: golden :4587
    else if(iInArmType==e9045_2x2_4_12)          { DoInArm_9045_2x2_4_12();   }  //AI(W6.2c-INARM-batch2) 20260626: golden :4591
    else if(iInArmType==e9045_2x2_4_13)          { DoInArm_9045_2x2_4();      }  //AI(W6.2c-INARM-batch2) 20260626: golden :4595
    else if(iInArmType==e9045_2x2_4_14)          { DoInArm_9045_2x2_4_14();   }  //AI(W6.2c-INARM-batch2) 20260626: golden :4599
    // --- W6.2c batch-3: 5 variant arms pulled out of the #if 0 gate (ACTIVE);
    //     All_1Pick is the leading picker-count `if` above (golden :4535) ---
    else if(iInArmType==e9045_1x4_4_Back)        { DoInArm_9045_1x4_4_Back(); }  //AI(W6.2c-INARM-batch3) 20260626: golden :4575
    else if(iInArmType==e9045_1x4_4)             { DoInArm_9045_1x4_4();      }  //AI(W6.2c-INARM-batch3) 20260626: golden :4579
    else if(iInArmType==e9045_2x3_6)             { DoInArm_9045_2x3_6();      }  //AI(W6.2c-INARM-batch3) 20260626: golden :4613
    else if(iInArmType==e9045_2x3_6_14)          { DoInArm_9045_2x3_6_14();   }  //AI(W6.2c-INARM-batch3) 20260626: golden :4607
    else if(iInArmType==e9045_2x4_4_14)          { DoInArm_9045_2x4_4();      }  //AI(W6.2c-INARM-batch3) 20260626: golden :4619
    // --- W6.2c batch-4: 6 variant arms pulled out of the #if 0 gate (ACTIVE);
    //     e9045_2x8_32 routes to the SAME DoInArm_9045_2x8_8() callee (golden :4639-4641),
    //     so both 2x8_8 and 2x8_32 arms are active and both call the 2x8_8 family. ---
    else if(iInArmType==e9045_2x2_8_Hot)         { DoInArm_9045_2x2_8_Hot();  }  //AI(W6.2c-INARM-batch4) 20260626: golden :4603
    else if(iInArmType==e9045_2x4_8)             { DoInArm_9045_2x4_8();      }  //AI(W6.2c-INARM-batch4) 20260626: golden :4623
    else if(iInArmType==e9045_2x5_8)             { DoInArm_9045_2x5_8();      }  //AI(W6.2c-INARM-batch4) 20260626: golden :4627
    else if(iInArmType==e9045_2x6_8)             { DoInArm_9045_2x6_8();      }  //AI(W6.2c-INARM-batch4) 20260626: golden :4631
    else if(iInArmType==e9045_2x8_8)             { DoInArm_9045_2x8_8();      }  //AI(W6.2c-INARM-batch4) 20260626: golden :4635
    else if(iInArmType==e9045_2x8_32)            { DoInArm_9045_2x8_8();      }  //AI(W6.2c-INARM-batch4) 20260626: golden :4639 (routes 2x8_32 -> 2x8_8 callee)
    // --- W6.2c batch-5: 2 S-family (HT-9045S) variant arms made ACTIVE. ---
    else if(iInArmType==e9045_1x4_4_13)          { DoInArm_9045S_1x4_4();     }  //AI(W6.2c-INARM-batch5) 20260626: golden :4567 (HT-9045S; S in func name)
    else if(iInArmType==e9045_2x4_4_13)          { DoInArm_9045_2x4_4_13();   }  //AI(W6.2c-INARM-batch5) 20260626: golden :4615 (NO S in func name)
    else
    {
        Str.sprintf("iInArmType=%d", iInArmType);                               //Steven 20220620 : add log message
        ShowMyMessage("Program Error in DoInArm_9045()", Str);
    }
}

//==============================================================================
//==  ACTIVE: iInArmType dispatch ladder #2  DoInArm_9045_SuckerMap  -- golden :4676
//==  REAL-defined here (substrate stub removed).  The Prod grid-clear is kept
//==  ACTIVE (faithful); the 25 per-variant *_SuckerMap() call-arms are GATED.
//==============================================================================
void DoInArm_9045_SuckerMap()                                                   //Steven 20240223 : 重新整理DoInArm_9045_SuckerMap
{
    AnsiString Str;
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            Prod.fInArmSuck4x8[0][i][j]=false;                                  //Steven 20240410 : Add for有使用的吸嘴, 對應到開關site
            Prod.fInArmSuck4x8[1][i][j]=false;
            Prod.bInSuckUse[0][i][j]=false;                                     //Steven 20240410 : Add for有使用的吸嘴, 對應到Site Mode, Auto clean專用
            Prod.bInSuckUse[1][i][j]=false;
            Prod.iSiteMap[0][i][j]=-1;                                          //AI(general) 20260408 (RogerYang) : fix copy-paste bug
            Prod.iSiteMap[1][i][j]=-1;
        }
    }

    bool bSiteMapHotplateReady=false;

    if(LastSet.iTemperature==Tempture_Hot &&
       IniConfig.bI21AutoSiteMappingUseHotplate==true)
    {
        bSiteMapHotplateReady=bAutoSiteMapHotplateReady;
    }
    else
    {
        bSiteMapHotplateReady=true;
    }
    (void)bSiteMapHotplateReady;

    // W6.2b-canary: UN-GATE ONLY the e9045_1x1_1 / e9045_1x4_1_Ac arm (the SOLE
    // translated variant; golden :4705).  The remaining 23 *_SuckerMap() picker-
    // map builders are NOT translated yet -> they stay GATED below; for any
    // still-gated iInArmType the else Program-Error path is the live offline
    // behavior and the cleared Prod grids above remain the live state.
    if(iInArmType==e9045_1x1_1 ||                                               //AI(W6.2b-canary) 20260626: un-gate ONLY the translated 1x1_1 SuckerMap arm
       iInArmType==e9045_1x4_1_Ac)              { DoInArm_9045_1x1_1_SuckerMap(); }  //golden :4705
    // --- W6.2c batch-1: 5 variant SuckerMap arms pulled out of the #if 0 gate (ACTIVE) ---
    else if(iInArmType==e9045_1x2_2_13)  { i1x2_4UseACEGPicker=0; DoInArm_9045_1x2_2_SuckerMap();    }  //AI(W6.2c-INARM) 20260626: golden :4709
    else if(iInArmType==e9045_1x2_2_14)  { i1x2_4UseACEGPicker=0; DoInArm_9045_1x2_2_14_SuckerMap(); }  //AI(W6.2c-INARM) 20260626: golden :4710
    else if(iInArmType==e9045_1x2_4_Hot) { i1x2_4UseACEGPicker=0; DoInArm_9045_1x2_4_Hot_SuckerMap();}  //AI(W6.2c-INARM) 20260626: golden :4711
    else if(iInArmType==e9045_1x3_4)     { DoInArm_9045_1x3_4_SuckerMap();    }                          //AI(W6.2c-INARM) 20260626: golden :4724
    else if(iInArmType==e9045_1x3_2_14)  { DoInArm_9045_1x3_2_14_SuckerMap(); }                          //AI(W6.2c-INARM) 20260626: golden :4712
    // --- W6.2c batch-2: 6 variant SuckerMap arms made ACTIVE (no picker-prefix; verified golden :4736-4767) ---
    else if(iInArmType==e9045_1x4_2_14)  { DoInArm_9045_1x4_2_SuckerMap();    }                          //AI(W6.2c-INARM-batch2) 20260626: golden :4736
    else if(iInArmType==e9045_1x4_8_Hot) { DoInArm_9045_1x4_8_Hot_SuckerMap();}                          //AI(W6.2c-INARM-batch2) 20260626: golden :4748
    else if(iInArmType==e9045_2x1_2_13)  { DoInArm_9045_2x1_2_SuckerMap();    }                          //AI(W6.2c-INARM-batch2) 20260626: golden :4752
    else if(iInArmType==e9045_2x2_4_12)  { DoInArm_9045_2x2_4_12_SuckerMap(); }                          //AI(W6.2c-INARM-batch2) 20260626: golden :4756
    else if(iInArmType==e9045_2x2_4_13)  { DoInArm_9045_2x2_4_SuckerMap();    }                          //AI(W6.2c-INARM-batch2) 20260626: golden :4760
    else if(iInArmType==e9045_2x2_4_14)  { DoInArm_9045_2x2_4_14_SuckerMap(); }                          //AI(W6.2c-INARM-batch2) 20260626: golden :4764
    // --- W6.2c batch-3: 5 variant SuckerMap arms made ACTIVE (no picker-prefix; verified golden :4740/:4744/:4774/:4778/:4784).
    //     All_1Pick has NO SuckerMap arm: the golden engine declares the extern
    //     (golden :4675) but NEVER dispatches DoInArm_9045_All_1Pick_SuckerMap() --
    //     faithful, do NOT add one and add NO ep1Picker guard to this ladder. ---
    else if(iInArmType==e9045_1x4_4_Back) { DoInArm_9045_1x4_4_Back_SuckerMap();}                         //AI(W6.2c-INARM-batch3) 20260626: golden :4740
    else if(iInArmType==e9045_1x4_4)      { DoInArm_9045_1x4_4_SuckerMap();    }                          //AI(W6.2c-INARM-batch3) 20260626: golden :4744
    else if(iInArmType==e9045_2x3_6)      { DoInArm_9045_2x3_6_SuckerMap();    }                          //AI(W6.2c-INARM-batch3) 20260626: golden :4778
    else if(iInArmType==e9045_2x3_6_14)   { DoInArm_9045_2x3_6_14_SuckerMap(); }                          //AI(W6.2c-INARM-batch3) 20260626: golden :4774
    else if(iInArmType==e9045_2x4_4_14)   { DoInArm_9045_2x4_4_SuckerMap();    }                          //AI(W6.2c-INARM-batch3) 20260626: golden :4784
    // --- W6.2c batch-4: 6 variant SuckerMap arms made ACTIVE (no picker-prefix;
    //     verified golden :4768-4806).  e9045_2x8_32 routes to the SAME
    //     DoInArm_9045_2x8_8_SuckerMap() callee (golden :4804-4806). ---
    else if(iInArmType==e9045_2x2_8_Hot) { DoInArm_9045_2x2_8_Hot_SuckerMap(); }                          //AI(W6.2c-INARM-batch4) 20260626: golden :4768
    else if(iInArmType==e9045_2x4_8)     { DoInArm_9045_2x4_8_SuckerMap();     }                          //AI(W6.2c-INARM-batch4) 20260626: golden :4788
    else if(iInArmType==e9045_2x5_8)     { DoInArm_9045_2x5_8_SuckerMap();     }                          //AI(W6.2c-INARM-batch4) 20260626: golden :4792
    else if(iInArmType==e9045_2x6_8)     { DoInArm_9045_2x6_8_SuckerMap();     }                          //AI(W6.2c-INARM-batch4) 20260626: golden :4796
    else if(iInArmType==e9045_2x8_8)     { DoInArm_9045_2x8_8_SuckerMap();     }                          //AI(W6.2c-INARM-batch4) 20260626: golden :4800
    else if(iInArmType==e9045_2x8_32)    { DoInArm_9045_2x8_8_SuckerMap();     }                          //AI(W6.2c-INARM-batch4) 20260626: golden :4804 (routes 2x8_32 -> 2x8_8 callee)
    // --- W6.2c batch-5: 2 S-family SuckerMap arms made ACTIVE (transcribed from golden; no picker-prefix). ---
    else if(iInArmType==e9045_1x4_4_13)  { DoInArm_9045S_1x4_4_SuckerMap();    }                          //AI(W6.2c-INARM-batch5) 20260626: golden :4732 (no picker-prefix)
    else if(iInArmType==e9045_2x4_4_13)  { DoInArm_9045_2x4_4_13_SuckerMap();  }                          //AI(W6.2c-INARM-batch5) 20260626: golden :4780 (no picker-prefix)
#if 0 // TODO(W6.2b variants) -- golden :4704-4830 (remaining per-layout *_SuckerMap arms; un-gate as each variant lands)
    /* ... further per-iInArmType *_SuckerMap dispatch arms ... */
#endif
    else { Str.sprintf("iInArmType=%d", iInArmType); ShowMyMessage("Program Error in DoInArm_9045_SuckerMap()", Str); }
}

//==============================================================================
//==  ACTIVE SM #1: DoInArmAdditionalFunction  -- golden :3209
//==  Cursor flow (Task transitions) kept ACTIVE & verbatim.  The cross-module
//==  action bodies (DoInDieClean / DoInArmDevicePosPrecise / fBarCode-> /
//==  DoInArmRotateKIT) and the predicate (CheekNeedToDoInArmAdditionalFunction)
//==  have no translated home -> gated; the offline "nothing to do" path drives
//==  Task 1->100->bResult=true (the SM converges cleanly).
//==============================================================================
int iInArmAdditionalFunctionTask;
//==============================================================================
void InitDoInArmAdditionalFunction()                                            //Steven 20210609 : 整合Precisor, Bottom CCD, Die Clean, Rotator
{
    iInArmAdditionalFunctionTask=1;
}
//==============================================================================
bool CheekNeedToDoInArmAdditionalFunction()                                     // golden :3107
{
    bool bResult=false;
    // golden body reads InArmSuck.HasType(...) + USE_ROTATE_KIT / tRotate +
    // bDoPreciser / USE_DIE_CLEAN / BOTTOM_2DID and sets the bDieClean /
    // bPrecisor / bBottom2DID / bInRotator flags (all cross-module, no home).
    // Offline: nothing to do -> bResult stays false.
#if 0 // TODO(W7) -- golden :3109-3204 (bInRotator/bDieClean/.. flags, CheckNeedRotateWithoutHotIC, fSCKART, PreSetInArmAdditionalFlag)
    /* full predicate body verbatim in golden; gated -- depends on untranslated
       additional-function flags + CheckNeedRotateWithoutHotIC + fSCKART. */
#endif
    if(bResult)
        InitDoInArmAdditionalFunction();
    return bResult;
}
//==============================================================================
bool DoInArmAdditionalFunction()                                                //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean  -- golden :3209
{
    bool bResult=false;
    bool flag;  (void)flag;

    int &Task=iInArmAdditionalFunctionTask;

    switch(Task)
    {
        case 1:
            CheekNeedToDoInArmAdditionalFunction();
            Task=100;
            // fall-through to case 100 (golden :3219 -- intentional, no break)
        case 100:
            // golden :3221-3239 selects Die Clean / Preciser / 2DID / Rotate via
            // the InArmSuck.bAlready* flags + bDieClean/bPrecisor/.. flags.  The
            // bAlready* flags are substrate-provided; the bDieClean/.. selectors
            // are untranslated -> offline they are all false, so this else-path
            // is the live behavior: nothing to do -> finish.
#if 0 // TODO(W7) -- golden :3221-3239 additional-fn selector (bDieClean/bPrecisor/bBottom2DID/bInRotator + Init* calls)
            if(InArmSuck.bAlreadyDieClean==false && bDieClean)
            {
                InitInDieCleanTask();
                Task=10000;
            }
            else if(InArmSuck.bAlreadyPreciser==false && bPrecisor)
            {
                InitInArmDevicePosPrecise();
                Task=20000;
            }
            else if(InArmSuck.bAlready2DID==false && bBottom2DID)
            {
                fBarCode->InitBottom2DIDScan();
                Task=30000;
            }
            else if(InArmSuck.bAlready2DID==false && bInRotator)
            {
                Task=40000;
            }
            else
#endif
            {
                bResult=true;
            }
            break;
#if 0 // TODO(W7) -- golden :3246-3304 action SMs (DoInDieClean / DoInArmDevicePosPrecise / fBarCode-> / MoveInArmZToPlateSafe + DoInArmRotateKIT)
        case 10000:
            flag=DoInDieClean();
            if(flag==true)                                                      //wei 20170418
            {
                InArmSuck.bAlreadyDieClean=true;
                bDieClean=false;
                Task=100;
            }
            break;
        case 20000:
            flag=DoInArmDevicePosPrecise();
            if(flag)                                                            //Frank 20180410 (Steven) : InArm Preciser Station
            {
                InArmSuck.bAlreadyPreciser=true;
                bPrecisor=false;
                bDoPreciser=true;
                bHangTimePause=true;                                            //Steven 20090827 : Hang Up dectector
                Task=100;
            }
            break;
        case 30000:
            if(BOTTOM_2DID_CCD==1)                                              //KaiChen 20200513 : Bottom 2D 8CCD
                flag=fBarCode->DoBottom2DID_8CCD_Scan();
            else
                flag=fBarCode->DoBottom2DIDScan();
            if(flag)
            {
                InArmSuck.bAlready2DID=true;
                bBottom2DID=false;
                Task=100;
            }
            break;
        case 40000:
            flag=MoveInArmZToPlateSafe(Task);
            if(flag)
            {
                iInRotateFinish=1;
                Task=41000;
            }
            else
            {
                break;
            }
        case 41000:
            flag=DoInArmRotateKIT();
            if(flag==true)
            {
                iInRotateFinish=2;                                              //kevin 20130524  旋轉完成
                InArmSuck.bAlreadyRotate=true;
                bInRotator=false;
                bWaitRotateFinish=false;
                Task=100;
            }
            break;
#endif
    }
    return bResult;
}
//==============================================================================
void PreSetInArmAdditionalFlag()                                                //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean  -- golden :3309
{
    // golden body presets the InArmSuck.bAlready* flags from USE_ROTATE_KIT /
    // tRotate / bEnableUsePreciser / USE_DIE_CLEAN / BOTTOM_2DID.  Offline the
    // additional functions are all "already done" so the additional-fn SM
    // converges immediately.  Keep that faithful end-state.
    InArmSuck.bAlreadyPreciser=true;
    InArmSuck.bAlreadyDieClean=true;
    InArmSuck.bAlready2DID=true;
    InArmSuck.bAlreadyRotate=true;
}

//==============================================================================
//==  ACTIVE SM #2: DoInArmIonFanGiveWay  -- golden :9166 (Eastsun 20260521)
//==  Cursor flow kept ACTIVE & verbatim; the one motion body
//==  (MoveInArm2XYToDecayTeach) is gated (no translated home) and treated as
//==  "reached" offline so the SM advances.
//==============================================================================
int iInArmIonFanGiveWayTask;
void InitDoInArmIonFanGiveWayTask()                                             //Eastsun 20260521 整合
{
    iInArmIonFanGiveWayTask=1;
}
//------------------------------------------------------------------------------
bool DoInArmIonFanGiveWay()                                                     //Eastsun 20260521 整合
{
    bool bResult=false;
    int &Task=iInArmIonFanGiveWayTask;

    switch(Task)
    {
        case 1:
            if(IniConfig.bA15_1ESDGiveWayFunction==true && InArmSuck.HasRealIC()==false)
            {
                if(InArmSuck.HasRealIC()==false)
                {
                    if((FLCarryKit.NoIC() && InSHT1InLF()) ||
                       (BLCarryKit.NoIC() && InSHT2InLF()))
                    {
                        Task=5;
                    }
                    else
                    {
                        Task=2;
                    }
                }
                else
                {
                    Task=5;
                }
            }
            else
            {
                Task=5;
            }
            break;
        case 2:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=3;
            }
            break;
        case 3:
        {
            bool bDecayReached;
#if 0 // TODO(W7) -- golden :9205 MoveInArm2XYToDecayTeach() (no translated home)
            bDecayReached=MoveInArm2XYToDecayTeach();
#else
            bDecayReached=true;                                                 // offline: treat decay-teach pos as reached
#endif
            if(bDecayReached)
            {
                RecordProcess("In Arm Ion Fan Give Way");
                Task=5;
            }
            break;
        }
        case 5:
            if((FLCarryKit.NoIC() && InSHT1InLF()) ||
               (BLCarryKit.NoIC() && InSHT2InLF()))
            {
                bResult=true;
            }
            break;
    }

    return bResult;
}

//==============================================================================
//==  ACTIVE SM #3: ProcessSCKARTLoadingCount  -- golden :7305
//==  Cursor flow kept ACTIVE & verbatim; the vacuum-destroy + alarm sink bodies
//==  are gated.  Behind bCanRunSCKART (default false) in callers, so offline
//==  this SM is dormant; kept pumpable + faithful.
//==============================================================================
bool ProcessSCKARTLoadingCount(bool bReset=false)
{
    AnsiString ErrPart;  (void)ErrPart;
    bool bResult=false;
    if(bReset)
    {
        iProcessSCKARTLoadingCountTask=1;
        return bResult;
    }

    int &Task=iProcessSCKARTLoadingCountTask;
    switch(Task)
    {
        case 1:
        {
            bool bDestroyDone;
#if 0 // TODO(W7) -- golden :7319 InArmSuck.Suck[iLoadPickX][iLoadPickY].Destroy()
            bDestroyDone=InArmSuck.Suck[iLoadPickX][iLoadPickY].Destroy();
#else
            bDestroyDone=true;                                                  // offline: destroy done immediately
#endif
            if(bDestroyDone)
            {
                InArmSuckUse[iLoadPickX][iLoadPickY]=true;
                Task=100;
            }
            break;
        }
        case 100:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=200;
            }
            break;
        case 200:
#if 0 // TODO(W7) -- golden :7332 ShowErrorMessage("WAR0120") alarm sink
            ErrPart=InArmSuck.Suck[iLoadPickX][iLoadPickY].sName;
            ShowErrorMessage("WAR0120", K_RETRY, MInArmX, false, ErrPart);
#endif
            bResult=true;
            bPickFromLoader=false;                                              //Steven 20171226 (Wei) : 確認Loader吸取完成
            break;
    }
    return bResult;
}

//==============================================================================
//==  GATED SMs (ACTIVE stub preserving cursor var + sane terminal value).
//==  These four golden SMs are MOT[]/sensor/fProductionInfo-bound with no
//==  translated home; reproducing their dense case bodies would require pulling
//==  untranslated modules.  Each stub keeps the cursor var (so callers/canary
//==  can drive it) and returns the golden "still working / not finished"
//==  default so the engine LINKS and the higher-level loop does not crash.
//==============================================================================
int iInArmInArmCheckShtFloatTask;
void InitDoInArmCheckShtFloatTask()                                             // golden :3344
{
    iInArmInArmCheckShtFloatTask=1;
}
// golden :3349 -- dense MOT[]/CCLink/Ltc sensor SM with 2 goto/label pairs
// (IN_ARM_CHECK_SHUTTLE label@3357 / goto@3389).  GATED whole-body.
bool DoInArmCheckShuttleFloating(int iSht, bool bPlaceOtherShuttle, bool bNeedCheck)
{
#if 0 // TODO(W7) -- golden :3349-3758 (MOT[]/sensor SM + goto IN_ARM_CHECK_SHUTTLE)
#endif
    (void)iSht; (void)bPlaceOtherShuttle; (void)bNeedCheck;
    return false;                                                              // golden default: floating-check not finished
}
//------------------------------------------------------------------------------
int iAutoChkInSHLatchTask;
void InitAutoChkInSHLatchTask()                                                 // golden :3763
{
    iAutoChkInSHLatchTask=1;
}
// golden :3810 -- CCLink/Ltc 2-sensor latch detect SM.  GATED whole-body.
int CheckInShuttleSensor_Latch(int iShuttle, int iMoveAct, bool bAlarm)         //0:NotFin 1:Finish 2:Error
{
#if 0 // TODO(W7) -- golden :3810-4174 (CCLink/Ltc latch sensor SM)
#endif
    (void)iShuttle; (void)iMoveAct; (void)bAlarm;
    return 0;                                                                  // golden 0 = NotFinish
}
//------------------------------------------------------------------------------
// golden :7619 -- THE central pick SM (iPickFromLoadStageTask, switch@7653,
// goto IN_ARM_LOADER label@7637/goto@7703, suck loop @case 1000).  Its case
// bodies deref MOT[]/MoveInArm*/AutoTeachLoadTrayZ/AddLoadingCount/.. -- none
// translated this wave.  GATED whole-body; the cursor var (substrate-provided
// iPickFromLoadStageTask) is preserved so the higher loop can drive it.
bool DoInArmPickFromLoadStage_9045()
{
#if 0 // TODO(W7) -- golden :7619-8280 (central pick SM: MOT[]/suck-loop/goto IN_ARM_LOADER)
#endif
    (void)iPickFromLoadStageTask;
    return false;                                                              // golden default: pick not finished
}
//------------------------------------------------------------------------------
// golden :8843 -- auto-calibrate suck-Z SM (iDoInArmAutoCalSuckZTask,
// switch@8867).  Derefs MOT[] encoders + fProductionInfo->setEditZ* (TEdit form
// widgets).  GATED whole-body; bIsFirst init path kept faithful (golden :8860).
int iDoInArmAutoCalSuckZTask=0;
bool DoInArmAutoCalSuckZ(bool bIsFirst)
{
    int &Task=iDoInArmAutoCalSuckZTask;
    if(bIsFirst)                                                               // golden :8860 init path (faithful)
    {
        Task=1;
        return true;
    }
#if 0 // TODO(W7) -- golden :8867-9028 (MOT[] encoders + fProductionInfo->setEditZ* TEdit SM)
#endif
    return false;                                                              // golden default: calibration not finished
}

//==============================================================================
//==  ACTIVE offline stubs for the remaining golden in-arm engine surface.
//==  Each is declared in ainarm9045.h; the golden body is MOT[]/cross-module/
//==  UI-bound with no translated home this wave.  Offline-safe defaults keep
//==  the engine linkable.  Golden file:line cited per symbol.  TODO(W7).
//==============================================================================
void DoInArm_9045_Type()                                                  {}    // golden :1616
void SetInOutArmParameter()                                               {}    // golden :1499
void SetArmRowCount()                                                     {}    // golden :1471
void DoInArmSuckPreOn(int, int)                                           {}    // golden :2231
void DoInArmLoadPickUP_9045(AnsiString, AnsiString)                       {}    // golden :4175
void sLoadPickupClean()                                                   {}    // golden :4182
void SetAutoSkipCount(int)                                                {}    // golden :2408
void RecordUPH(int)                                                       {}    // golden :5376
void RecordLotUPH_For_FOREHOPE_NINGBO(AnsiString, AnsiString, AnsiString, int, int, int) {} // golden :5390
void InArmSuckReset()                                                     {}    // golden :2302
void ShowAutoSkipError()                                                  {}    // golden :2478
void PickErrorData(int, int)                                              {}    // golden :2502
void AutoSkipHasIClog(AnsiString, int, int)                               {}    // golden :2388
void CheckInArmDestroyActiveFinishInitial()                               {}    // golden :1142
void TrayPickupErrorData()                                                {}    // golden :2516
void CheckInArmDestroyActive()                                            {}    // golden :1169
void SetShuttleStatus_9045(int, int, int, int, int)                       {}    // golden :476
void GetInArmZShtDownPos_9045(int, bool, bool)                            {}    // golden :530
void InArmZNeedDown_9045(int, int, bool)                                  {}    // golden :613
void SetShuttleToHasNullICWhenCleanOut()                                  {}    // golden :4288
void SetShuttleToNullICWhenCleanOut()                                     {}    // golden :4273

bool AutoTrayEndMusic(int, int)                          { return false; }      // golden :2527
bool bCheckYPitchHome(int)                               { return false; }      // golden :2564
bool bCheckYPitchRunHomeSen(int)                         { return false; }      // golden :2540
bool SearchLoadTrayUpDown_9045_FIFO_2Step(bool&, int&, int&, int&, int&) { return false; } // golden :91(decl)
bool DoTrayEndProcess_9045()                             { return false; }      // golden :2828
bool DoTraySkipProcess_9045()                            { return false; }      // golden :2894
bool DoRecordSkipPosition_9045(bool)                     { return false; }      // golden :2999
bool CheckPlaceToShuttle()                               { return false; }      // golden :2313
bool DoInArmPlaceToShuttle_9045()                        { return false; }      // golden :8723
bool MoveInArmXYAndSuckUp(int, int, int)                 { return false; }      // golden :9029
bool Find_InArm_Single(int, int&, int&)                  { return false; }      // golden :6364
bool SearchAndMoveInArmXYToLoad_9045()                   { return false; }      // golden :6960
bool MoveInArmZToLoaderPick(int)                         { return false; }      // golden :6983
bool MoveArmXYToLoader_9045(bool&, int, int&, int&, bool, bool) { return false; } // golden :5604
bool MoveInArmXYToShuttle_9045(int, int, bool, bool)     { return false; }      // golden :740
bool MoveInArmZToShuttlePlace_9045(int)                  { return false; }      // golden :866
bool InitialInArmNeedSuck(int, bool)                     { return false; }      // golden :3771
bool InArmSideAllClose(int)                              { return true;  }      // golden :4231 (offline: all closed)
bool InArmSideAllCloseWithKit(int, int)                  { return true;  }      // golden :4251
bool bCheckInShuttlePosAndNOIC()                         { return false; }      // golden :4368
bool InArmNeedCheckOffset(bool, int)                     { return false; }      // golden :4329
bool CheckShuttleSensor_9045(int)                        { return true;  }      // golden :2070 (offline: no floating)

int  MoveInArmXYToWaitTrayArm(int, int, bool, bool)      { return 0; }          // golden :828
int  GetVariableYInShuttleData()                         { return 0; }          // golden :2254
int  AutoCalculateInArmYClosePitch(bool, bool)           { return 0; }          // golden :4911
int  CheckOneCycleAction(int)                            { return 0; }          // golden :7431

void SetClosedShtKitToHasNullIC_9045_dummy() {}          // (placeholder anchor)
bool SetClosedShtKitToHasNullIC_9045(int, int, AnsiString, AnsiString) { return false; } // golden :385

// CheckShuttleSensor_9045_<layout> family (golden :1025-1133) -- per-layout
// sensor checks; offline "no floating" (golden returns 0 = OK).
int CheckShuttleSensor_9045_1x1(int, bool, bool) { return 0; }
int CheckShuttleSensor_9045_1x4(int, bool, bool) { return 0; }
int CheckShuttleSensor_9045_2x1(int, bool, bool) { return 0; }
int CheckShuttleSensor_9045_2x3(int, bool, bool) { return 0; }
int CheckShuttleSensor_9045_2x4(int, bool, bool) { return 0; }
int CheckShuttleSensor_9045_1x2(int, bool, bool) { return 0; }
int CheckShuttleSensor_9045_2x2(int, bool, bool) { return 0; }
int CheckShuttleSensor_9045_2x5(int, bool, bool) { return 0; }
int CheckShuttleSensor_9045_2x6(int, bool, bool) { return 0; }
int CheckShuttleSensor_9045_2x8(int, bool, bool) { return 0; }
