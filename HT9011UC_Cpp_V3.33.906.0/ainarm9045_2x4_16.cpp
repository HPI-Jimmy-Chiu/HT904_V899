// =============================================================================
//  ainarm9045_2x4_16.cpp  --  in-arm 16-site (2x4_16) per-site-config variant
//
//  Translation wave: W6.2b (in-arm per-site-config variant: 2x4_16, 16-site)
//  Translator: AI(W6.2b-2x4_16) 20260626
//  Golden source: ainarm9045_2x4_16.cpp (mirror name + relative path = ROOT)
//
//  FAITHFUL translation of the golden BCB6/VCL state-machine variant into
//  portable std C++ (MinGW g++ 6.3, C++17) over the Sim HAL.  Every function /
//  variable / class / file NAME, every switch(Task) cursor value, every formula,
//  fall-through and goto is preserved VERBATIM from the golden CP950 source.
//  Per-function provenance cites the golden file:line.
//
//  ACTIVE vs GATED
//  ---------------
//  The SM case bodies are kept ACTIVE wherever they touch the Sim HAL substrate
//  (MOT[]/Suck/Sensor/Cylinder/InArmSuck/FormsFacade/predicates).  Cross-module
//  helpers with no translated home are satisfied by ACTIVE offline shims in
//  ainarm9045_2x4_16_shims.{h,cpp}.
//
//  DEAD-BY-DESIGN symbols.  In the 906 golden tree this variant references four
//  symbols that are DECLARED/DEFINED NOWHERE (commented-out externs / no body):
//      Prod.fInArmSuckUse_TwoArm32Site   (absent from golden cprod -- the plan
//                                          forbids inventing it on Prod)
//      LastSet.bUseTestSocket            (absent from golden LastSet here)
//      XPHSuckToSht_2x4_16 / XPHShtToSuck_2x4_16  (commented-out externs, no
//                                          initializer anywhere in golden)
//  i.e. golden ainarm9045_2x4_16.cpp never compiled/linked as-is (it is a dead
//  variant kept in-tree).  To stay FAITHFUL (keep the source text + logic) AND
//  honest (do NOT invent Prod/LastSet members), those exact statements are
//  GATED `#if 0 // TODO(W7)` and given an ACTIVE file-local equivalent that
//  preserves the SM's intent (a 2x4_16-local site-use map / pitch tables).  The
//  file-local symbols carry the golden dims cited in golden cmydef.h:3123-3124.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  ZERO U+FFFD.
// =============================================================================
// Golden include block (BCB6 god-headers) mapped to the W6.x translated homes:
//   MachineDefine.h / #pragma hdrstop          -> MachineDefine.h
//   ainarm9045_2x4_16.h                         -> own header
//   aArmHeader.h (god-header)                   -> W6.0 SHIM (gated #if 0)
//   MyKitSuck.h (InArmSuck/FLCarryKit/...)       -> aHotPlateSubstrate.h
//   cprod.h / mymotor.h / mycylin.h / mysensor.h-> kept
//   cSortCT.h / main.h (fMain)                   -> FormsFacade.h
//   csystem.h / cinitial.h(->cmydef) / atester.h / cmydef.h -> kept
//   note.h / mymessbox.h (ShowErrorMessage/...)  -> canary_support.h
//   the in-arm engine helpers (DoInArm_9045_SuckerMap/InitInArm*Task/...) ->
//     aHotPlateSubstrate.h + ainarm9045.h + ainarm_SearchPickPlate.h
//   the dead-by-design / call-only helpers       -> ainarm9045_2x4_16_shims.h
#include "MachineDefine.h"

#include "ainarm9045_2x4_16.h"
#include "ainarm9045_2x4_16_shims.h"

#include "aArmHeader.h"             // W6.0 SHIM (god-header gated #if 0; do NOT un-gate)
#include "ainarm9045.h"             // in-arm engine externs (Init*Task / Move*_9045 / GetInArm*Offset / ...)
#include "ainarm_SearchPickPlate.h" // shared HP-pick externs (GetHeaterWaitTime / MoveInArmXYPickHotPlate_9045 / ...)
#include "ainarm_SearchPlacePlate.h"// GetHotPlateYHalfPos / HPPlaceLog / bPlaceToHotplatePartOK
#include "Motor/mymotor.h"          // MOT[], InArmContinuousMove_9045 (W4 sim stub)
#include "cprod.h"                  // Prod / TestIF / TestIF_File / HotPlateForm / ArmSpeed
#include "mycylin.h"
#include "myswitch.h"               // SW[] (SwShuttleVibration1/2)
#include "mysensor.h"
#include "csystem.h"                // predicate API
#include "atester.h"
#include "acatchtray_shims.h"       // MoveInArm2XYToWait / MoveInArm2XYToShuttle2Wait / InitProcessSingleMotorTask
#include "atester_shims.h"          // fObserver
#include "cmydef.h"                 // global type universe + IC consts
#include "common.h"
#include "cpublic.h"                // CosFunction
#include "aHotPlateSubstrate.h"     // InArmSuck/FLCarryKit/.. + PickFromHPList + ainarm2 cursors + engine externs
#include "FormsFacade.h"            // fMain/fSortCT (non-VCL stubs)
#include "canary_support.h"         // ShowErrorMessage / ShowMyMessage / LastSet / __FUNC__

// =============================================================================
//  File-local dead-by-design tables (golden cmydef.h:3123-3124 commented-out
//  externs -- no initializer exists in golden).  Dims are the golden-cited
//  shapes.  Values: the only golden hint is the variant's own usage
//  (XPHSuckToSht_2x4_16[iModeX][j] maps suck-col -> shuttle-col), so we use the
//  identity map for the standard mode (iModeX==0) and leave the other rows as
//  identity too (offline-safe, in-bounds).  These are variant-LOCAL (NOT added
//  to the shared substrate / cmydef) per the wave directive.
// =============================================================================
#ifndef HT9045_XPH_2x4_16_LOCAL
#define HT9045_XPH_2x4_16_LOCAL
static const int XPHSuckToSht_2x4_16[3][8] =
{
    {0, 1, 2, 3, 4, 5, 6, 7},
    {0, 1, 2, 3, 4, 5, 6, 7},
    {0, 1, 2, 3, 4, 5, 6, 7},
};
static const int XPHShtToSuck_2x4_16[2][3][8] =
{
    { {0, 1, 2, 3, 4, 5, 6, 7}, {0, 1, 2, 3, 4, 5, 6, 7}, {0, 1, 2, 3, 4, 5, 6, 7} },
    { {0, 1, 2, 3, 4, 5, 6, 7}, {0, 1, 2, 3, 4, 5, 6, 7}, {0, 1, 2, 3, 4, 5, 6, 7} },
};
// File-local NN-mode site-use map (golden referenced as Prod.fInArmSuckUse_TwoArm32Site,
// absent from golden cprod by design -- kept local, NOT on Prod).  4 rows x 8 cols.
static bool fInArmSuckUse_TwoArm32Site_local[4][8];
#endif

//==============================================================================
//  golden :31
//==============================================================================
void SetInOutArmParameter_2x4_16()
{
    InArmSuck .SetPickerCount(2, 8, 2, 4, 1, 0, 0);
    OutArmSuck.SetPickerCount(2, 8, 2, 4, 1, 0, 0);
}
//==============================================================================
//  golden :37
//==============================================================================
bool InArmSideAllClose_4x4_16(int iSht)
{
    iSht=(iSht==0)?2:0;
    int iKit=0;
    for(int i=0; i<InArmSuck.iPickRow; i++)
    {
        for(int j=0; j<InArmSuck.iPickCol; j++)
        {
            //AI(W6.2b-2x4_16) 20260626: golden reads Prod.fInArmSuckUse_TwoArm32Site
            //(absent-by-design from golden cprod) -> file-local equivalent.
            if(fInArmSuckUse_TwoArm32Site_local[i+iSht][j+iKit]==true)
            {
                return false;
            }
        }
    }
    return true;
}
//==============================================================================
//  golden :59
//該位資料為Y座標, 2=標準, 0=A面, 1=B面
//百位資料以上為X座標, 分配方式如下
//                    0                     =標準
//         100        |         101         =平均左右兩次
//    200       201   |    202       203    =平均左右四次
//==============================================================================
int GetNowSiteKitMode_2x4_16(int iSht, bool bPlace)
{
    if(bPlace==true)
    {
        if(InArmSuck.iXStep==1)
        {
            if(InArmSuck.iYStep==1)
            {
                if(iSht==0)
//                   (InArmSuck.Item[0][0]>=HAS_IC || InArmSuck.Item[0][2]>=HAS_IC || InArmSuck.Item[0][4]>=HAS_IC || InArmSuck.Item[0][6]>=HAS_IC ||
//                    InArmSuck.Item[1][0]>=HAS_IC || InArmSuck.Item[1][2]>=HAS_IC || InArmSuck.Item[1][4]>=HAS_IC || InArmSuck.Item[1][6]>=HAS_IC))
                {
                    return 102;
                }
                else                                                            //if(iSht==1 &&
                     //   (InArmSuck.Item[0][1]>=HAS_IC || InArmSuck.Item[0][3]>=HAS_IC || InArmSuck.Item[0][5]>=HAS_IC || InArmSuck.Item[0][7]>=HAS_IC ||
                     //    InArmSuck.Item[1][1]>=HAS_IC || InArmSuck.Item[1][3]>=HAS_IC || InArmSuck.Item[1][5]>=HAS_IC || InArmSuck.Item[1][7]>=HAS_IC))
                {
                    return 202;
                }
            }
            else
            {
                if(iSht==0)
                {
                    if(InArmSuck.Item[0][0]>=HAS_IC || InArmSuck.Item[0][2]>=HAS_IC || InArmSuck.Item[0][4]>=HAS_IC || InArmSuck.Item[0][6]>=HAS_IC)
                    {
                        return 100;
                    }
                    else                                                        //if(InArmSuck.Item[1][0]>=HAS_IC || InArmSuck.Item[1][2]>=HAS_IC || InArmSuck.Item[1][4]>=HAS_IC || InArmSuck.Item[1][6]>=HAS_IC)
                    {
                        return 101;
                    }
                }
                else                                                            //if(iSht==1)
                {
                    if(InArmSuck.Item[0][1]>=HAS_IC || InArmSuck.Item[0][3]>=HAS_IC || InArmSuck.Item[0][5]>=HAS_IC || InArmSuck.Item[0][7]>=HAS_IC)
                        return 200;
                    else                                                        //if(InArmSuck.Item[1][1]>=HAS_IC || InArmSuck.Item[1][3]>=HAS_IC || InArmSuck.Item[1][5]>=HAS_IC || InArmSuck.Item[1][7]>=HAS_IC))
                        return 201;
                }
            }
        }
        else if(InArmSuck.iXStep==2)
        {
            if(InArmSuck.iYStep==1)
            {
                if(iSht==0)
                {
                    if(InArmSuck.Item[0][0]>=HAS_IC || InArmSuck.Item[0][4]>=HAS_IC ||
                       InArmSuck.Item[1][0]>=HAS_IC || InArmSuck.Item[1][4]>=HAS_IC)
                    {
                        return 10002;
                    }
                    else                                                        //if(InArmSuck.Item[0][2]>=HAS_IC || InArmSuck.Item[0][6]>=HAS_IC ||
                         //   InArmSuck.Item[1][2]>=HAS_IC || InArmSuck.Item[1][6]>=HAS_IC)
                    {
                        return 10102;
                    }
                }
                else                                                            //if(iSht==1)
                {
                    if(InArmSuck.Item[0][1]>=HAS_IC || InArmSuck.Item[0][5]>=HAS_IC ||
                       InArmSuck.Item[1][1]>=HAS_IC || InArmSuck.Item[1][5]>=HAS_IC)
                    {
                        return 10202;
                    }
                    else if(InArmSuck.Item[0][3]>=HAS_IC || InArmSuck.Item[0][7]>=HAS_IC ||
                            InArmSuck.Item[1][3]>=HAS_IC || InArmSuck.Item[1][7]>=HAS_IC)
                    {
                        return 10302;
                    }
                }
            }
            else
            {
                if(iSht==0)
                {
                    if(InArmSuck.Item[0][0]>=HAS_IC || InArmSuck.Item[0][4]>=HAS_IC)
                    {
                        return 10000;
                    }
                    else if(InArmSuck.Item[1][0]>=HAS_IC || InArmSuck.Item[1][4]>=HAS_IC)
                    {
                        return 10001;
                    }
                    else if(InArmSuck.Item[0][2]>=HAS_IC || InArmSuck.Item[0][6]>=HAS_IC)
                    {
                        return 10100;
                    }
                    else                                                        //if(InArmSuck.Item[1][2]>=HAS_IC || InArmSuck.Item[1][6]>=HAS_IC)
                    {
                        return 10101;
                    }
                }
                else                                                            //if(iSht==1)
                {
                    if(InArmSuck.Item[0][1]>=HAS_IC || InArmSuck.Item[0][5]>=HAS_IC)
                    {
                        return 10200;
                    }
                    else if(InArmSuck.Item[1][1]>=HAS_IC || InArmSuck.Item[1][5]>=HAS_IC)
                    {
                        return 10201;
                    }
                    else if(InArmSuck.Item[0][3]>=HAS_IC || InArmSuck.Item[0][7]>=HAS_IC)
                    {
                        return 10300;
                    }
                    else                                                        //if(InArmSuck.Item[1][3]>=HAS_IC || InArmSuck.Item[1][7]>=HAS_IC)
                    {
                        return 10301;
                    }
                }
            }
        }
        else                                                                    //if(InArmSuck.iXStep==4)
        {
            if(InArmSuck.iYStep==1)
            {
                if(iSht==0)
                {
                    if(InArmSuck.Item[0][0]>=HAS_IC ||
                       InArmSuck.Item[1][0]>=HAS_IC)
                    {
                        return 20002;
                    }
                    else if(InArmSuck.Item[0][2]>=HAS_IC ||
                            InArmSuck.Item[1][2]>=HAS_IC)
                    {
                        return 20102;
                    }
                    else if(InArmSuck.Item[0][4]>=HAS_IC ||
                            InArmSuck.Item[1][4]>=HAS_IC)
                    {
                        return 20202;
                    }
                    else                                                        //if(InArmSuck.Item[0][6]>=HAS_IC ||
                         //   InArmSuck.Item[1][6]>=HAS_IC)
                    {
                        return 20302;
                    }
                }
                else                                                            //if(iSht==1)
                {
                    if(InArmSuck.Item[0][1]>=HAS_IC ||
                       InArmSuck.Item[1][1]>=HAS_IC)
                    {
                        return 20402;
                    }
                    else if(InArmSuck.Item[0][3]>=HAS_IC ||
                            InArmSuck.Item[1][3]>=HAS_IC)
                    {
                        return 20502;
                    }
                    else if(InArmSuck.Item[0][5]>=HAS_IC ||
                            InArmSuck.Item[1][5]>=HAS_IC)
                    {
                        return 20602;
                    }
                    else                                                        //if(InArmSuck.Item[0][7]>=HAS_IC ||
                         //   InArmSuck.Item[1][7]>=HAS_IC)
                    {
                        return 20702;
                    }
                }
            }
            else
            {
                if(iSht==0)
                {
                    if(InArmSuck.Item[0][0]>=HAS_IC)
                    {
                        return 20000;
                    }
                    else if(InArmSuck.Item[1][0]>=HAS_IC)
                    {
                        return 20001;
                    }
                    else if(InArmSuck.Item[0][2]>=HAS_IC)
                    {
                        return 20100;
                    }
                    else if(InArmSuck.Item[1][2]>=HAS_IC)
                    {
                        return 20101;
                    }
                    else if(InArmSuck.Item[0][4]>=HAS_IC)
                    {
                        return 20200;
                    }
                    else if(InArmSuck.Item[1][4]>=HAS_IC)
                    {
                        return 20201;
                    }
                    else if(InArmSuck.Item[0][6]>=HAS_IC)
                    {
                        return 20300;
                    }
                    else                                                        //if(InArmSuck.Item[1][6]>=HAS_IC)
                    {
                        return 20301;
                    }
                }
                else                                                            //if(iSht==1)
                {
                    if(InArmSuck.Item[0][1]>=HAS_IC)
                    {
                        return 20400;
                    }
                    else if(InArmSuck.Item[1][1]>=HAS_IC)
                    {
                        return 20401;
                    }
                    else if(InArmSuck.Item[0][3]>=HAS_IC)
                    {
                        return 20500;
                    }
                    else if(InArmSuck.Item[1][3]>=HAS_IC)
                    {
                        return 20501;
                    }
                    else if(InArmSuck.Item[0][5]>=HAS_IC)
                    {
                        return 20600;
                    }
                    else if(InArmSuck.Item[1][5]>=HAS_IC)
                    {
                        return 20601;
                    }
                    else if(InArmSuck.Item[0][7]>=HAS_IC)
                    {
                        return 20700;
                    }
                    else                                                        //if(InArmSuck.Item[1][7]>=HAS_IC)
                    {
                        return 20701;
                    }
                }
            }
        }
    }
    return 102;
}
//==============================================================================
void InArmZNeedDown_2x4_16(int iSht, bool bPlace)
{
    if(InitialInArmNeedSuck(iSht, bPlace)==false)
        return;

    int iMode=GetNowSiteKitMode_2x4_16(iSht, bPlace);
    int iSuckRow, j2;
    int iModeRow=iMode%100;
    int iModeCol=iMode/100;

    for(int i=0; i<InArmSuck.iPickRow; i++)
    {
        for(int j=0; j<InArmSuck.iPickCol; j++)
        {
            iSuckRow=i;
            j2=XPHSuckToSht_2x4_16[InArmSuck.iModeX][j];
            if(iModeCol==1 && (j==0 || j==2 || j==4 || j==6) &&                 //Shuttle 1
               (iModeRow==2 || iModeRow==i))
            {
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==2 && (j==1 || j==3 || j==5 || j==7) &&            //Shuttle 2
                    (iModeRow==2 || iModeRow==i))
            {
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==100 && (j==0 || j==4) &&                          //吸嘴             ==>  蝦頭 1
                    (iModeRow==2 || iModeRow==i))                               // O X X X O X X X      O O X X
            {                                                                   // O X X X O X X X      O O X X
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==101 && (j==2 || j==6) &&                          //吸嘴             ==>  蝦頭 1
                    (iModeRow==2 || iModeRow==i))                               // X X O X X X O X      X X O O
            {                                                                   // X X O X X X O X      X X O O
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==102 && (j==1 || j==5) &&                          //吸嘴             ==>  蝦頭 2
                    (iModeRow==2 || iModeRow==i))                               // X O X X X O X X      O O X X
            {                                                                   // X O X X X O X X      O O X X
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==103 && (j==3 || j==7) &&                          //吸嘴             ==>  蝦頭 2
                    (iModeRow==2 || iModeRow==i))                               // X X X O X X X O      X X O O
            {                                                                   // X X X O X X X O      X X O O
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==200 && j==0 &&                                    //吸嘴             ==>  蝦頭 1
                    (iModeRow==2 || iModeRow==i))                               // O X X X X X X X      O X X X
            {                                                                   // O X X X X X X X      O X X X
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==201 && j==2 &&                                    //吸嘴             ==>  蝦頭 1
                    (iModeRow==2 || iModeRow==i))                               // X X O X X X X X      X O X X
            {                                                                   // X X O X X X X X      X O X X
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==202 && j==4 &&                                    //吸嘴             ==>  蝦頭 1
                    (iModeRow==2 || iModeRow==i))                               // X X X X O X X X      X X O X
            {                                                                   // X X X X O X X X      X X O X
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==203 && j==6 &&                                    //吸嘴             ==>  蝦頭 1
                    (iModeRow==2 || iModeRow==i))                               // X X X X X X O X      X X X O
            {                                                                   // X X X X X X O X      X X X O
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==204 && j==1 &&                                    //吸嘴             ==>  蝦頭 2
                    (iModeRow==2 || iModeRow==i))                               // X O X X X X X X      O X X X
            {                                                                   // X O X X X X X X      O X X X
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==205 && j==3 &&                                    //吸嘴             ==>  蝦頭 2
                    (iModeRow==2 || iModeRow==i))                               // X X X O X X X X      X O X X
            {                                                                   // X X X O X X X X      X O X X
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==206 && j==5 &&                                    //吸嘴             ==>  蝦頭 2
                    (iModeRow==2 || iModeRow==i))                               // X X X X X O X X      X X O X
            {                                                                   // X X X X X O X X      X X O X
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
            else if(iModeCol==207 && j==7 &&                                    //吸嘴             ==>  蝦頭 2
                    (iModeRow==2 || iModeRow==i))                               // X X X X X X X O      X X X O
            {                                                                   // X X X X X X X O      X X X O
                SetInArmNeedDestory(bPlace, i, j2, iSuckRow, j);
            }
        }
    }
}
//==============================================================================
void CheckXYPitch_2x4_16(int *iX, int *iY, int iSht, bool bPlace, int iMovePitchX, int iMovePitchY)
{
    int iMode=GetNowSiteKitMode_2x4_16(iSht, bPlace);
    int iModeRow=iMode%100;
    int iModeCol=iMode/100;
    int iOffsetPos=GetInArmToShuttleOffset_9045(iSht, iModeRow, iModeCol, false);
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
                if(iSht==0)
                {
                    *iY+=7000.0;
                    *iY-=((TestIF.dSiteYOffset/2.0)+(iMovePitchY/2.0));
                    *iY+=TestIF.dSiteYPitch/2;
                }
                else
                {
                    *iY-=7000.0;
                    *iY+=((TestIF.dSiteYOffset/2.0)+(iMovePitchY/2.0));
                    *iY+=TestIF.dSiteYPitch/2;
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
                if(iSht==0)
                {
                    *iY+=7000.0;
                    *iY-=((TestIF.dSiteYOffset/2.0)+(iMovePitchY/2.0));
                    *iY+=TestIF.iARM_Y_PITCH-TestIF.dSiteYPitch/2;
                }
                else
                {
                    *iY-=7000.0;
                    *iY+=((TestIF.dSiteYOffset/2.0)+(iMovePitchY/2.0));
                    *iY+=TestIF.iARM_Y_PITCH-TestIF.dSiteYPitch/2;
                }
            }
        }
        else if(bUseTwoArm32Site &&
                TestIF_File.iYPitchOffsetMode==2)                               //Steven 20201013 : NN mode add Y-Pitch offset selection
        {
            if(USE_Y_AUTO_PITCH==true)
            {
                if(iSht)
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
                if(iSht==0)
                {
                    *iY+=7000.0;
                    *iY-=((TestIF.dSiteYOffset/2.0)+(iMovePitchY/2.0));
                    *iY+=TestIF.dSiteYPitch/2;
                }
                else
                {
                    *iY-=7000.0;
                    *iY+=((TestIF.dSiteYOffset/2.0)+(iMovePitchY/2.0));
                    *iY+=TestIF.dSiteYPitch/2;
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
        if(iModeCol==1)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==2)                                                    //Ab --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXCenterPitch*0.5-TestIF.dSiteXPitch*1.0);
            else
                *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==101)                                                  //Ac --> Ac
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXCenterPitch*0.5);
            else
                *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==102)                                                  //Ab --> Aa
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXCenterPitch*0.5-TestIF.dSiteXPitch*1.0);
            else
                *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==103)                                                  //Ad --> Ac
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXCenterPitch*0.5);
            else
                *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==200)                                                  //Aa --> Aa
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXCenterPitch*0.5-TestIF.dSiteXPitch*1.0);
            else
                *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==201)                                                  //Ac --> Ab
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXCenterPitch*0.5);
            else
                *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==202)                                                  //Ae --> Ac
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(-dMovePitchX*1.0+TestIF.dSiteXCenterPitch*0.5);
            else
                *iX=*iX+(-dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==203)                                                  //Ag --> Ad
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(-dMovePitchX*3.0+TestIF.dSiteXCenterPitch*0.5+TestIF.dSiteXPitch*0.5);
            else
                *iX=*iX+(-dMovePitchX*3.0+TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==204)                                                  //Ab --> Aa
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXCenterPitch*0.5-TestIF.dSiteXPitch*1.0);
            else
                *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==205)                                                  //Ad --> Ab
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*0.0-TestIF.dSiteXCenterPitch*0.5);
            else
                *iX=*iX+(dMovePitchX*0.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==206)                                                  //Af --> Ac
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(-dMovePitchX*2.0+TestIF.dSiteXCenterPitch*0.5);
            else
                *iX=*iX+(-dMovePitchX*2.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==207)                                                  //Ah --> Ad
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(-dMovePitchX*4.0+TestIF.dSiteXCenterPitch*0.5+TestIF.dSiteXPitch*0.5);
            else
                *iX=*iX+(-dMovePitchX*4.0+TestIF.dSiteXPitch*1.5);
        }
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchBb)                                 //基準為第二隻吸嘴 //Steven for HT7080
    {
        dMovePitchX=double(iMovePitchX)/3.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXCenterPitch*0.5-TestIF.dSiteXPitch*1.0);
            else
                *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==101)                                                  //Ab --> Ac
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXCenterPitch*0.5);
            else
                *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==200)                                                  //Aa --> Aa
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXCenterPitch*0.5-TestIF.dSiteXPitch*1.0);
            else
                *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==201)                                                  //Ab --> Ab
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*0.0-TestIF.dSiteXCenterPitch*0.5);
            else
                *iX=*iX+(dMovePitchX*0.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==202)                                                  //Ac --> Ac
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(-dMovePitchX*1.0+TestIF.dSiteXCenterPitch*0.5);
            else
                *iX=*iX+(-dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==203)                                                  //Ad --> Ad
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(-dMovePitchX*2.0+TestIF.dSiteXCenterPitch*0.5+TestIF.dSiteXPitch*1.0);
            else
                *iX=*iX+(-dMovePitchX*2.0+TestIF.dSiteXPitch*1.5);
        }
    }
    else                                                                        //基準為第三隻吸嘴
    {
        dMovePitchX=double(iMovePitchX)/3.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXCenterPitch*0.5-TestIF.dSiteXPitch*1.0);
            else
                *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==101)                                                  //Ab --> Ac
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXCenterPitch*0.5);
            else
                *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==200)                                                  //Aa --> Aa
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXCenterPitch*0.5-TestIF.dSiteXPitch*1.0);
            else
                *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==201)                                                  //Ab --> Ab
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXCenterPitch*0.5);
            else
                *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==202)                                                  //Ac --> Ac
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXCenterPitch*0.5);
            else
                *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==203)                                                  //Ad --> Ad
        {
            if(TestIF_File.bEnableUseXCenterPitch==true)
                *iX=*iX+(-dMovePitchX*1.0+TestIF.dSiteXCenterPitch*0.5+TestIF.dSiteXPitch*1.0);
            else
                *iX=*iX+(-dMovePitchX*1.0+TestIF.dSiteXPitch*1.5);
        }
    }

    *iY=*iY+GetInArmYToShuttleOffset_9045(iSht, iOffsetPos);
    *iX=*iX+GetInArmXToShuttleOffset_9045(iSht, iOffsetPos);
    TransferInShuttleRatio(iSht, iX, iY, iInArmYBase, iInArmXBase);
    InArmZNeedDown_9045(iSht, 0/*iKit -- W6.2b arity*/, bPlace);
}
//==============================================================================
// in arm x y to shuttle position
//==============================================================================
bool MoveInArm2XYToShuttle_9045_2x4_16(int iSht, bool IncludeZ, bool bPlace)
{
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    int iMovePitchX=iXpitchMaxX3;
    int iMovePitchY=GetVariableYInShuttleData();
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));
    int iMode=GetNowSiteKitMode_2x4_16(iSht, bPlace);
    int iModeRow=iMode%100;
    int iModeCol=iMode/100;
    int iOffsetPos=GetInArmToShuttleOffset_9045(iSht, iModeRow, iModeCol, true);
    static bool bCheckZSafe=false;

    if(bCheckZSafe==false)
    {
        if(MoveInArmZToPlateSafe(1111)==false)
            return false;
        else
            bCheckZSafe=true;
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
        iXVariable[i]=GetInArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetInArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos+=iInArmShtXCenterPos;                                                 //Steven 20141029 : XY-Pitch for Shuttle Center position for base Suck
    iYPos+=iInArmShtYCenterPos;

//    if((TestIF_File.iTestMode==_32Site4X8N ||
//        TestIF_File.iTestMode==_16Site4X4 ) &&
//       TestIF_File.iYPitchOffsetMode==0)                                        //Steven 20201013 : NN mode add Y-Pitch offset selection
//    {
//        if(TestIF_File.iYPitchOffsetMode==0)
//        {
//            if(iSht==0)                                                     //jou 2016-05-05 修正 32 Sites In & Out Arm 吸放點位異常
//            {
//                iYPos+=7000.0;
//                iYPos-=((TestIF.dSiteYOffset/2.0)+(iMovePitchY/2.0));
//            }
//            else
//            {
//                iYPos-=7000.0;
//                iYPos+=((TestIF.dSiteYOffset/2.0)+(iMovePitchY/2.0));
//            }
//        }
//    }

//    if(CheckSpecialSiteMode())                                                  //wei 20150702 SPIL 12 Site use 8 Site
//    {
//        if(TestIF.iTestMode==_16Site2X8)                                        //Steven 20191124 : 16Site關Site跑2x4 Mode
//        {
//            if(i12SiteClose4SiteMode==0)
//            {
//                iXPos-=TestIF.dSiteXPitch*2.0;
//            }
//            else if(i12SiteClose4SiteMode==1)
//            {
//                iXPos-=TestIF.dSiteXPitch;
//            }
//            else if(i12SiteClose4SiteMode==3)
//            {
//                iXPos+=TestIF.dSiteXPitch;
//            }
//            else if(i12SiteClose4SiteMode==4)
//            {
//                iXPos+=TestIF.dSiteXPitch*2.0;
//            }
//        }
//        else
//        {
//            if(i12SiteClose4SiteMode==0)
//            {
//                iXPos-=TestIF.dSiteXPitch;
//            }
//            else if(i12SiteClose4SiteMode==2)
//            {
//                iXPos+=TestIF.dSiteXPitch;
//            }
//        }
//    }

    CheckXYPitch_2x4_16(&iXPos, &iYPos, iSht, bPlace, iMovePitchX, iMovePitchY);
    GetInArmZShtDownPos_9045(iSht, bPlace, iZPos);
    GetInShuttleStatus_9045(bZFlag, bPlace, IncludeZ);

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

    if(InArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, IncludeZ))
    {
        bCheckZSafe=false;
        return true;
    }
    return false;
}
// =============================================================================
bool ArmFinishLoader_2x4_16()                                                   //check Arm finish and  can go shuttle
{
    int iSht=(InArmSuck.iWhichSht==0)?2:0;
    int iKit=(InArmSuck.iWhichKit==0)?0:InArmSuck.iPickKitStep;

    for(int i=0; i<InArmSuck.iPickRow; i++)
    {
        for(int j=0; j<InArmSuck.iPickCol; j++)
        {
            if(InArmSuckUse[i][j] && InArmSuck.Item[i][j]==NULL_IC)
            {
                if(iCleanOut && MOT[MMTrayY].HasIC()==false && MOT[MMTrayY_Car].fHasTray==false)
                    return true;
                else
                    return false;
            }
            else if(fInArmSuckUse_TwoArm32Site_local[iSht+i][iKit+j] && InArmSuck.Item[i][j]==NULL_IC)
            {
                return false;
            }
        }
    }
    return true;
}
//==============================================================================
bool DoArmPickFromLoadStage_9045_2x4_16()
{
    //jou 980425 add In Arm Speed
    IN_ARM_LOADER_2x4_16:

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

                for(int i=0; i<InArmSuck.iPickRow; i++)                         //ChungHung 20111230 add 增加保護並解決bP17InArmFullPickFromLoader 所產生的bug InArmSuck上已全部有IC
                {
                    for(int j=0; j<InArmSuck.iPickCol; j++)
                    {
                        if(fInArmSuckUse_TwoArm32Site_local[i][j] && InArmSuck.Item[i][j]==NULL_IC)
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
                goto IN_ARM_LOADER_2x4_16;
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
                goto IN_ARM_LOADER_2x4_16;                                      //Steven 20231214 : For add speed
            }
            else
            {
                if(bNeedOneCycle())                                             //Steven 20131029 : 解決Index Position Error
                {
                    return true;
                }

                if((TrayForm.iManualRemoveLoader!=0 &&                          //Steven 20130819 : 加入開門檢查的功能  (==true --> !=0)
                    bLoaderHasSkip==true) ||                                    //jou 2010-12-20 修正需要手動拿Loader Tray時,In Arm Y移至shuttle 2上,方便OP拿Tray
                   (USE_TRAY_MAPPING && TestIF_File.bEnableTrayMap &&
                    TestIF_File.bEnableSuckMapCheck &&
                    bCheckTrayDevice) ||                                        //wei 20170302 (Steven) Tray map data error
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
                    if(TRAY_ARM_MODE==eUnderCoveyor)
                    {
                        if(MoveArmXYToLoaderStage_9045())
                            Task=12;
                    }
                    else if(MoveInArm2XYToWait())
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
                if(MoveArmXYToLoaderStage_9045())
                {
                    if(IsCheckInArmDestroyActiveFinish())                       //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
                    {
                        Task=200;
                        goto IN_ARM_LOADER_2x4_16;                              //Steven 20231214 : For add speed
                    }
                }
            }
            else
            {
                if(TRAY_ARM_MODE==eUnderCoveyor)
                {
                    if(bNeedOneCycle())
                        return true;
                    else
                        Task=12;
                }
                else
                {
                    Task=10;
                }
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

                iLoadPitchStepY=GetLoaderYPitchStep();

                for(int i=0; i<InArmSuck.iPickRow; i++)
                {
                    for(int j=0; j<InArmSuck.iPickCol; j++)
                    {
                        sLoadPickupErrorTrayPos[i][j]="";                       //kevin 20220521 add Tray X Y Pos
                    }
                }
                sLoadPickupClean();                                             //kevin 20220723 initial data
                Task=1000;
                goto IN_ARM_LOADER_2x4_16;                                      //Steven 20231214 : For add speed
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
            bPickFromLoader=true;                                                                                                                               //Steven 20171226 (Wei) : 確認Loader吸取完成
            flag1=true;
            iBuf=0;
            sAskStartDetect="";
            for(int i=0; i<InArmSuck.iPickRow; i++)
            {
                for(int j=0; j<InArmSuck.iPickCol; j++)
                {
                    if(InArmSuckUse[i][j] && InArmSuck.Item[i][j]==NULL_IC)
                    {
                        #ifdef SOFT_SIMULTE
                        if(fMain->chkInPickLoadError->Checked==true && j==1)
                        {
                            InArmSuck.Suck[i][j].Error=true;
                            bSuckEnd[i][j]=true;
                        }
                        #endif
                        if(ArmSpeed_File[InArm].bUseHPVacuum)                                                                                                   //Steven 20180125 (Jou) : 加熱盤的真空等待時間
                            InArmSuck.Suck[i][j].OnDelayTime=ArmSpeed[InArm].dVacuumTI*100;

                        iYpos=iYPosition+iLoadPitchStepY*i;                                                                                                     //kevin 20220521 往外移一層
                        if(bSuckEnd[i][j]==false && InArmSuck.Suck[i][j].Suck())                                                                                //wei 20150318 suck 判斷是否動作
                        {
                            AddLoadingCount(i, j, iXPosition[j], iYpos);                                                                                        //Steven 20161123 : 將Loader吸取的數量計算統一成Function
                        }
                        else if(InArmSuck.Suck[i][j].Error==false)
                        {
                            flag1=false;
                        }
                        else if(InArmSuck.Suck[i][j].Error==true)
                        {
                            bSuckEnd[i][j]=true;
                            sLoadPickupErrorTrayPos[i][j].sprintf("(%d,%d);", iXPosition[j]+1, iYpos+1);                                                        //kevin 20220521 Load pick error record X, Y 座標
                        }
                    }
                    else
                    {
                        bSuckEnd[i][j]=true;
                    }
                }
            }

            if(flag1==false)                                                                                                                                    //KEVIN 20130421 按RETRY資料尚未交換會將其他IC丟掉
            {
                break;
            }

            if(USE_TRAY_MAPPING && TestIF_File.bEnableTrayMap && TestIF_File.bEnableSuckMapCheck && bTrayMapDataCheckError)                                     //wei 20170302 (Steven) Tray map data error
            {
                ProcessTrayMapDataError(true);
                Task=5000;
                return false;
            }

            ShowAutoSkipError();                                                                                                                                //kevin 20170904 記錄每一盤 SKIP 沒吸到IC位置

            for(int i=0; i<InArmSuck.iPickRow; i++)
            {
                for(int j=0; j<InArmSuck.iPickCol; j++)
                {
                    if(InArmSuck.Suck[i][j].Error)
                    {
                        bInArmSuckErr=true;
                        iRetryCT++;
                        if(iRetryCT<=ArmSpeed[InArm].iRetryCT)                                                                                                  //Steven 20141113 : Loader SKIP時在原地Retry
                        {
                            Task=1010;
                        }
                        else
                        {
                            Task=1050;
                            if(bPickUpHomeFinish==false)
                            {
                                bPickUpHomeFinish=true;
                                Task=1010;
                                SetInArmHome();
                            }
                        }
                        return false;
                    }
                }
            }

            iRetryCT=0;
            if(bCanRunSCKART==true &&                                                                                                                           //Steven 20161223 (wei) : For SCK ART loading count
               iSCKARTLoadingStatus>=1 &&                                                                                                                       //數量到了要放下來,然後 Alarm
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
            bTrayMapDataCheckErrorSkip=false;
            bHasDuplicateErr=false;
            for(int i=0; i<InArmSuck.iPickRow; i++)
            {
                for(int j=0; j<InArmSuck.iPickCol; j++)
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
                    iManualRemoveTrayCnt=2;                                                                             //Sam 20190731 : 當 Load 發生 Pickup Error Skip 時，當前這盤除了要手動移除，下一盤也要手動移除。
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
                goto IN_ARM_LOADER_2x4_16;                                      //Steven 20231214 : For add speed
            }
            break;
        case 2100:
            if(ArmFinishLoader_2x4_16())
            {
                InArmSuck.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                fObserver->AddTimeData(1, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);
                return true;
            }
            else
            {
                if(IniConfig.bP17InArmFullPickFromLoader==false)                //Steven 20111220 : 讓每盤都是滿的
                {
                    if((CUSTOMER_CODE==CC_KYEC_JCTHIU ||
                        CUSTOMER_CODE==CC_KYEC_LEE) &&                          //Ifor 20181009 (Steven) : KYEC add FIFO Function
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
                        if(iOneCycle==1)                                        //kevin 20110425 有吸取到ic就放shuttle 沒吸到吸嘴就不吸
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

                for(int i=0; i<InArmSuck.iPickRow; i++)
                {
                    for(int j=0; j<InArmSuck.iPickCol; j++)
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
        case 5000:
            if(ProcessTrayMapDataError(false))/*W6.2b: bReset default*/                                       //wei 20170302 (Steven) Tray map data error
            {
                bTrayMapDataCheckError=false;
                Task=10;
            }
            break;
    }
    return false;
}
//==============================================================================
bool DoInArmPickFromHotPlate_9045_2x4_16(bool &bDownSpeed)
{
    int &Task=iInArmPickFromHotPlateTask;
    static int iRetryCT=0;
    int ret=0, iP=0;
    bool flag1=false;
    static bool bCheckSpeed=false;
    int iPlateR[MAX_ARM_Row][MAX_ARM_Col];                                      //Jimmychiu 20230417 : Record the position after placing the IC
    int iPlateC[MAX_ARM_Row][MAX_ARM_Col];
    bool bSuck[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iPlateR, sizeof(iPlateR));
    ZeroMemory(iPlateC, sizeof(iPlateC));
    ZeroMemory(bSuck, sizeof(bSuck));
    AnsiString ErrPart="";                                                      //Steven 20100104 : 整合AlarmCodde
    bool bHasDuplicateErr=false;                                                //Steven 20100104 : 整合AlarmCodde
//    bool bflag1=false, bflag2=false;
    GetHotPlateYHalfPos();

    if(iOneCycle && bPickFromHotplate==false)
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
            DoInArm_9045_SuckerMap();                                           //Steven 20220531 : for NN mode Auto Site Map
            SearchPlateToPick();
            bPickFormHotplatePartOK=false;                                      //ChungHung 20120112 Hang Up 解除
            bPickFormHotplateRetry=false;                                       //Ifor 20160616 add Hot plate Pickup error Retry Hang Up 解除
            Task=50;
//            break;
        case 50:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=100;
            }
            break;
        case 100:
            iRetryCT=0;
            if(LastSet.iRunStartMode==rsmAutoSiteMap &&
               CosFunction.bUSEJCETSiteMapMode==true)                           //Steven 20220527 : for JCET Auto Site Map
            {
                InArmSuckUse[iAutoSiteMapInArmRow][iAutoSiteMapInArmCol]=true;
            }
            else
            {
                PickFromHPList->GetHPFirstTeamSuckUse(InArmSuckUse);
            }
            Task=110;
        case 110:
            if(MoveInArmXYPickHotPlate_9045(false)/*W6.2b: engine sig 1-arg*/)
            {
                if(IsCheckInArmDestroyActiveFinish())
                {
                    Task=150;
                }
            }
            break;
        case 150:
            if(InArmSuck.iWhichShtPickFor32==0)                                 //Steven 20160201 : 修正Shuttle放料順序
            {
                if(bCheckSpeed)                                                 //jou 2012-07-16 Auto Speed Hot Mode Fix
                {
                    if(FLCarryKit.HasIC() || bDownSpeed)
                        InArmSubSpeed();
                    else if(InSHT1InLF())
                        InArmAddSpeed();

                    bCheckSpeed=false;
                }

                if(InSHT1InLF())
                {
                    if(FLCarryKit.HasIC())
                    {
                        MOT[MInShuttle1].fCanMoveL=true;
                        FLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                        if(IniConfig.bA09_ByArmCloseSite==true)                 //ChungHung 20130910 alter for SCK can close site by Index
                        {
                            if((BLCarryKit.HasIC()==false || BTestSuck.NoIC()) && InSHT2InLF())
                            {
                                MOT[MInShuttle2].fCanMoveL=true;
                                BLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                            }
                        }
                    }
                    else
                    {
                        Task=190;                                               //ChungHung 20120724 在Task=150~Task=200時 進入Contact 做動作InArm位置會被修改造成HangUp
                    }
                }
                else if(INSTALL_OCR!=eocrUninstal &&
                        CosFunction.bTrayOCR && NeedWaitTrayArm==true)          //Ifor 20180510 : Fix OCR機型IniConfig.bCloseSiteByIndexArm未開啟時會丟同一個蝦頭導致Hang up
                {
                    return true;
                }
                else if(BLCarryKit.HasIC()==false && InSHT2InLF())
                {
                    if(TestIF.iShuttleMode && TestIF.iShuttle_Sel==0)
                        BLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);

                    if(LastSet.iTemperature==Tempture_Hot &&
                       iOneCycle==1 && IndexStatus==Z1Down_Z2Up)                //jou 2010-08-05 start : 當加熱模式下in arm pick up hotplate & out shuttle 1 has ic & arm 2 has ic & 其他沒有ic時 hang up
                    {
                        if(InArmSuck.HasType(HAS_HOT_IC)==true && ShuttleHasIC()==false && IndexHasIC()==false &&
                           MOT[MInShuttle1].fCanMoveM==false && OutArmSuck.HasIC()==false)
                        {
                            MOT[MInShuttle1].fCanMoveM=true;
                        }
                    }
                }
            }
            else
            {
                if(bCheckSpeed)                                                 //jou 2012-07-16 Auto Speed Hot Mode Fix
                {
                    if(BLCarryKit.HasIC() || bDownSpeed)
                        InArmSubSpeed();
                    else if(InSHT2InLF())
                        InArmAddSpeed();
                    bCheckSpeed=false;
                }

                if(InSHT2InLF())
                {
                    if(BLCarryKit.HasIC())
                    {
                        MOT[MInShuttle2].fCanMoveL=true;
                        BLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);

                        if(IniConfig.bA09_ByArmCloseSite==true)
                        {
                            if((FLCarryKit.HasIC()==false || FTestSuck.NoIC()) && InSHT1InLF())
                            {
                                MOT[MInShuttle1].fCanMoveL=true;
                                FLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                            }
                        }
                    }
                    else
                    {
                        Task=190;                                               //ChungHung 20120724 在Task=150~Task=200時 進入Contact 做動作InArm位置會被修改造成HangUp
                    }
                }
                else if(INSTALL_OCR!=eocrUninstal &&                            //Ifor 20180510 : Fix OCR機型IniConfig.bCloseSiteByIndexArm未開啟時會丟同一個蝦頭導致Hang up
                        CosFunction.bTrayOCR && NeedWaitTrayArm==true)
                {
                    return true;
                }
                else if(FLCarryKit.HasIC()==false && InSHT1InLF())
                {
                    if(TestIF.iShuttleMode && TestIF.iShuttle_Sel==1)
                        FLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                }
            }
            break;
        case 190:                                                               //ChungHung 20120724 在Task=150~Task=200時 進入Contact 做動作InArm位置會被修改造成HangUp
            if(MoveInArmXYPickHotPlate_9045(false)/*W6.2b: engine sig 1-arg*/==false)
            {
                return false;
            }
            Task=200;
        case 200:
            if(MoveInArmZToPickHotPlate_9045(iRetryCT))
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
            if(LastSet.iRunStartMode==rsmAutoSiteMap &&
               CosFunction.bUSEJCETSiteMapMode==true)                                                                   //Steven 20220527 : for JCET Auto Site Map
            {
                HotplateDataConversion(iAutoSiteMapInArmRow, iAutoSiteMapInArmCol, iAutoSiteMapHPNo, iAutoSiteMapHPRow, iAutoSiteMapHPCol, &flag1);
            }
            else
            {
                if(PickFromHPList->GetHPFirstTeam(&iP, iPlateR, iPlateC, bSuck))
                {
                    for(int i=0; i<InArmSuck.iPickRow; i++)
                    {
                        for(int j=0; j<InArmSuck.iPickCol; j++)
                        {
                            if(bSuck[i][j]==true)
                            {
                                HotplateDataConversion(i, j, iP, iPlateR[i][j], iPlateC[i][j], &flag1);                 //Ifor 20180518 : add 整合Hot plate 資料轉換
                            }
                        }
                    }
                }
            }

            if(flag1==false)                                                                                            //KEVIN 20130421 按RETRY資料尚未交換會將其他IC丟掉
                break;

            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
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

            Task=340;
            bPickFormHotplatePartOK=true;                                                                               //ChungHung 20120112 Hang Up 解除
            bAutoSiteMapHotICCanPick=true;                                                                              //Ifor 20171006 (Steven) : 避免Auto site Mapping Hotplate補回資料後發生異常
            break;
        case 320:
            if(MoveInArmZToPlateSafe(Task))
            {
                iRetryCT++;
                Task=330;
            }
            break;
        case 330:
            if(iRetryCT>ArmSpeed[InArm].iRetryCT)                                                                       //Steven 20120109 : 整合成一次Skip多顆
            {
                ErrPart=" ";
                bHasDuplicateErr=false;
                for(int i=0; i<InArmSuck.iPickRow; i++)
                {
                    for(int j=0; j<InArmSuck.iPickCol; j++)
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
                iRetryCT=0;
                ret=0;
                if(IniConfig.bInOutArmCanPushHome)
                {
                    ret=ShowErrorMessage("JAM0109", K_RETRY|K_SKIP|K_HOME, MInArmX, bHasDuplicateErr, ErrPart);         //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home
                }
                else
                {
                    ret=ShowErrorMessage("JAM0109", K_RETRY|K_SKIP, MInArmX, bHasDuplicateErr, ErrPart);                //Steven 20100104
                }

                if(ret==K_SKIP)
                {
                    bAutoSiteMapWaitTestResult=false;                                                                   //Ifor 20180115 (Steven) : add Site Mapping SKIP 需清除旗標
                    if(LastSet.iRunStartMode==rsmAutoSiteMap &&
                       CosFunction.bUSEJCETSiteMapMode==true)                                                           //Steven 20220527 : for JCET Auto Site Map
                    {
                        PorcessJAM0109HotPlatePickUpErrorSkip(iAutoSiteMapInArmRow, iAutoSiteMapInArmCol, iAutoSiteMapHPNo, iAutoSiteMapHPRow, iAutoSiteMapHPCol);
                        bPickHPDuplicateErr[iAutoSiteMapInArmRow][iAutoSiteMapInArmCol]=false;
                    }
                    else
                    {
                        PickFromHPList->GetHPFirstTeam(&iP, iPlateR, iPlateC, bSuck);
                        for(int i=0; i<InArmSuck.iPickRow; i++)
                        {
                            for(int j=0; j<InArmSuck.iPickCol; j++)
                            {
                                if(InArmSuck.Suck[i][j].Error)
                                {
                                    PorcessJAM0109HotPlatePickUpErrorSkip(i, j, iP, iPlateR[i][j], iPlateC[i][j]);      //Steven 20161214 : 將HP吸取異常SKIP統一成Function
                                    bPickHPDuplicateErr[i][j]=false;
                                }
                            }
                        }
                    }
                    Task=340;                                                                                           //kevin 20150514   HOTPLATE不要再下去吸
                    return false;                                                                                       //kevin 20150514
                }
                else
                {
                    bPickFormHotplateRetry=true;                                                                        //Ifor 20160616 add Hot plate Pickup error Retry Hang Up 解除
                    if(IniConfig.bInOutArmCanPushHome)
                    {
                        if(ret==K_HOME)                                                                                 //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home start
                        {
                            Task=335;
                        }
                        else                                                                                            //(ret==K_RETRY)
                        {
                            Task=336;
                        }
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
        case 336:
            iRetryCT=0;
            Task=200;
            break;
        case 340:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=350;
            }
            break;
        case 350:
            DoInArm_CheckSuckerMap();                                           //Ifor 20180514 : add Auto Site Mapping Check Suck Map
            if(LastSet.iRunStartMode==rsmAutoSiteMap &&
               CosFunction.bUSEJCETSiteMapMode==true)                           //Steven 20220527 : for JCET Auto Site Map
            {
                Task=400;
            }
            else
            {
                if(PickFromHPList->DataForwardAndNextTeam())
                {
                    Task=1;                                                     //Steven 20150921 : For HP Skip Hang Up
                    break;
                }
                else
                {
                    Task=400;
                }
            }
        case 400:
            Task=1;
            HasHotReadyIC_9045()/*W6.2b: engine sig 0-arg*/;
            if(ArmSpeed[InArm].bAutoSpeed && LastSet.iTemperature==Tempture_Hot && OnlyRow2==false)
                bCheckSpeed=true;
            else
                bCheckSpeed=false;

            if(iPickP==0)                                                       //JerryYang 20180921 Setup Teach功能
            {
                iInArmPickPlaceCnt[InOfsHP1]++;
            }
            else
            {
                iInArmPickPlaceCnt[InOfsHP2]++;
            }
            return true;
    }
    return false;
}
//=============================================================================
bool InArmHasICForSht_2x4_16(int iSht)
{
    for(int i=0; i<InArmSuck.iPickRow; i++)
    {
        for(int j=0; j<InArmSuck.iPickCol; j++)
        {
            if(iSht==j%2 &&
               InArmSuck.Item[i][j]!=NULL_IC)
            {
                return true;
            }
        }
    }
    return false;
}
//==============================================================================
bool DoPlaceToShuttle_9045_2x4_16()
{
    IN_ARM_SHUTTLE_2x4_16:

    QueueTaskList[4].CheckTaskChange();                                         //Steven 20200821 : 使用Goto也要記錄Task變化
    int &Task=iInArmPlaceToShuttleTask;
    bool flag;
    static bool bCheckSpeed=false;                                              //JerryYang 20180711 (wei) Shuttle震動後還是置偏要接著搖
    AnsiString ErrPart="";                                                      //Steven 20100104 : 整合AlarmCodde
    static bool bCheckSpeed1=false;                                             //KaiChen 20171225 (Steven)：Add Speed Display
    static int  iPickRow=0;
    int iYPos=0;
    if(bIndexPickUpErrorWaitRetry)                                              //Ifor 20171119 : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
        return false;

    static bool bPlaceOtherShuttle=false;

    if(LastSet.iRunStartMode==rsmAutoSiteMap &&
       CosFunction.bUSEJCETSiteMapMode==true)                                   //Steven 20220913 : Fixed for ASM function
    {
        iWhichSht32=iAutoSiteMapHotplateiWhichShuttle;
        iWhichKit32=InArmSuck.iWhichKitPickFor32;
    }
    else if(LastSet.iTemperature==Tempture_Hot &&                               //Steven 20160227 : 改成HOT在上面
            LastSet.iRunStartMode!=rsmAutoSiteMap)                              //kevin 20161124 add hot AutoSitmap hang up
    {
        if(LastSet.iTemperature==Tempture_Hot &&
           LastSet.iRunStartMode==rsmQAMode && bQAModeFinishCleanOut==true)     //Steven 20180601 : 修正高溫QA mode放料
        {
            iWhichSht32=InArmSuck.iWhichSht;
            iWhichKit32=InArmSuck.iWhichKit;
        }
        else
        {
            iWhichSht32=InArmSuck.iWhichShtPickFor32;
            iWhichKit32=InArmSuck.iWhichKitPickFor32;
        }
    }
    else
    {
        iWhichSht32=InArmSuck.iWhichSht;
        iWhichKit32=InArmSuck.iWhichKit;
    }

    AutoSiteMappingCheckShuttle(true);                                          //Ifor 20180116 (Steven) : add Auto Site Mapping Check iWhich Shuttle
    switch(Task)
    {
        case 1:
            bDestoryOnSht=false;                                                //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            InArmXMoveSafe=false;
            bPlaceOtherShuttle=false;

            if(InArmHasICForSht_2x4_16(0)==true)                                //放Sht 1
            {
                iPickRow=1;
                InArmSuck.iWhichSht=0;
                InArmSuck.iWhichShtPickFor32=0;
            }
            else if(InArmHasICForSht_2x4_16(1)==true)                           //放Sht 2
            {
                iPickRow=0;
                InArmSuck.iWhichSht=1;
                InArmSuck.iWhichShtPickFor32=0;
            }

            if(bPlaceToShuttle2Step==false)                                     //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
            {
                for(int i=0; i<InArmSuck.iPickRow; i++)
                {
                    for(int j=0; j<InArmSuck.iPickCol; j++)
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

            if(ArmSpeed[InArm].bAutoSpeed && LastSet.iTemperature!=Tempture_Hot)
                bCheckSpeed=true;
            else
                bCheckSpeed=false;

            if(IniConfig.bA26MotorSpeedSortDisplay==true &&                     //KaiChen 20171225 (Steven)：Add Speed Display
               LastSet.iTemperature!=Tempture_Hot)
                bCheckSpeed1=true;
            else
                bCheckSpeed1=false;
            Task=100;
//jou 980424 add In Arm Speed
//            break;
        case 100:
            if(bHPCleanout)                                                     //wei 20160624 Hotplate clean out
            {
                iHPShuttle++;
                if(iHPShuttle>=2)
                    iHPShuttle=0;
            }

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
            goto IN_ARM_SHUTTLE_2x4_16;
        case 900:
            if(InSHT1InLF() && FLCarryKit.HasIC()==false)
            {
                if(bCheckSpeed)                                                 //Steven 20110525 : Auto Speed
                {
                    bCheckSpeed=false;
                    InArmAddSpeed();
                }

                if(bCheckSpeed1)                                                //KaiChen 20171225 (Steven)：Add Speed Display
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

                if(bCheckSpeed1)                                                //KaiChen 20171225 (Steven)：Add Speed Display
                {
                    bCheckSpeed1=false;
                    InArmSubSpeedDisplay();
                }
                Task=930;
            }
        case 930:
            if(MoveInArm2XYToShuttle_9045_2x4_16(0, ZAxisNotDown, true))
            {
                Task=1000;
                goto IN_ARM_SHUTTLE_2x4_16;
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
            if(MoveInArm2XYToShuttle_9045_2x4_16(0, ZAxisDown, true))
            {
                Task=1100;
                goto IN_ARM_SHUTTLE_2x4_16;
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
//            if(iOneCycle==1 && IndexStatus==Z1Down_Z2Up &&                      //jou 2010-01-18 start : place to shuttle時，回Home，會hang
//               BLCarryKit.NoIC() && BTestSuck.NoIC())
//            {
//                BLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
//            }

            if(InSHT1InLF())
            {
                if(IniConfig.bP17InArmFullPickFromLoader)                       //ChungHung 20111230 add Shuttle必須放滿
                {
                    if(IsFLCarrKitAllHasIC() && bPlaceToShuttle2Step==false)    //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                    {
                        MOT[MInShuttle1].fCanMoveL=true;
                        FLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                        if((BLCarryKit.HasIC()==false || BTestSuck.NoIC()) && InSHT2InLF())
                        {
                            Task=1;
                        }
                    }
                    else
                    {
                        MOT[MInShuttle1].fCanMoveL=false;
                        InArmZNeedDown_9045(0, 0/*iKit -- W6.2b arity*/, true);
                        Task=1100;
                        goto IN_ARM_SHUTTLE_2x4_16;
                    }
                }
                else
                {
                    if(FLCarryKit.HasIC()!=NULL_IC &&
                       bPlaceToShuttle2Step==false)                             //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                    {
                        MOT[MInShuttle1].fCanMoveL=true;
                        FLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                        if(IsBLCarrKitAllHasIC()==false)                        //Steven 20120130 : 兩個Shuttle都有IC時,In Arm會來回跑
                        {
                            Task=1;
                        }
                    }
                    else
                    {
                        MOT[MInShuttle1].fCanMoveL=false;
                        InArmZNeedDown_9045(0, 0/*iKit -- W6.2b arity*/, true);
                        Task=1100;
                        goto IN_ARM_SHUTTLE_2x4_16;
                    }
                }
            }

            if(MOT[MInShuttle1].Led[iInposLed]==false && MOT[MInShuttle1].fCanMoveL==false)
                MOT[MInShuttle1].fCanMoveL=true;
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
            if(MoveInArmZToShuttlePlace_9045(0))                                //wait shuttle in Left position
            {
                if(InArmNeedCheckOffset(true, 0))                               //Steven 20230531 : 簡化判斷式
                {
                    Task=1150;
                    break;
                }

                Task=1200;
                if(ArmSpeed[InArm].iEnableReleaseDelay==0)                      //JerryYang 20160127 for TSMC inarm release device前delay
                {
                    InArmReleaseDelay.SetSecAndOn(ArmSpeed[InArm].dReleaseDelayTime);
                    Task=1180;
                }
                #ifndef SOFT_SIMULTE
                goto IN_ARM_SHUTTLE_2x4_16;
                #endif
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
            if(MoveInArm2XYToShuttle_9045_2x4_16(0, ZAxisDown, true))
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
            bPlaceToShuttle2Step=true;                                                                                  //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
            bCheckInarmPlaceShuttle=true;                                                                               //kevin 20180525 20180315 add inarm place shuttle 確認是否有抖動情形
            for(int i=0; i<InArmSuck.iPickRow; i++)
            {
                for(int j=0; j<InArmSuck.iPickCol; j++)
                {
                    iYPos=XPHSuckToSht_2x4_16[InArmSuck.iModeX][j];
                    if(InArmSuck.Item[i][j]!=NULL_IC && InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                    {
                        iBackInArmHotCount=InArmSuck.HotCount;
                        if(InArmSuck.Item[i][j]==HAS_NULL_IC || InArmSuck.Suck[i][j].Destroy())
                        {
                            if(InArmSuck.Item[i][j]!=HAS_NULL_IC)                                                       //Steven 20170109 : 沒IC的地方不檢查吹氣
                            {
                                bInArmCheckDestroyACT[i][j]=true;                                                       //jou 981130 確認device確實destroy完成
                                LastSet.lShuttleCount++;                                                                //Steven 20170113 : For count place to shuttle
                            }

                            if(TestIF_File.iTestMode==_16Site4X4 ||
                               TestIF_File.iTestMode==_32Site4X8N)                                                      //Steven 20230428 : For NN Mode,  i --> i+2
                                InArmSuck.PordRec[i][j].AddTestRecord(1, TestIF.iSiteMap[i+2][iYPos]);
                            else
                                InArmSuck.PordRec[i][j].AddTestRecord(1, TestIF.iSiteMap[i][iYPos]);                    //Frank 20160505 add
                            SetShuttleStatus_9045(iWhichSht32, i, iYPos, i, j);
                            FLCarryKit.HotCount=InArmSuck.HotCount;                                                     //JerryYang 20230204 : add hot plate放置順序資料
                        }
                        else if(InArmSuck.Suck[i][j].Error==false)
                        {
                            flag=false;
                        }
                    }
                }
            }

            if(flag==false)                                                                                             //kevin 20131011 在下真空誤判, 換到上面來
                break;

            for(int i=0; i<InArmSuck.iPickRow; i++)
                for(int j=0; j<InArmSuck.iPickCol; j++)
                    if(InArmSuck.Item[i][j]!=NULL_IC && InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                        return false;

            bCheckInarmPlaceShuttle=false;                                                                              //kevin 20180525 20180315 add inarm place shuttle 確認是否有抖動情形
            if(InArmHasICForSht_2x4_16(0)==false)
            {
                Task=1300;
                bPlaceToShuttle2Step=false;                                                                             //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                fObserver->AddTimeData(2, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);
            }
            else
            {
                bPlaceToShuttle2Step=true;                                                                              //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                Task=1250;
            }
            AddArmSiteRecord(0);                                                                                        //Steven 20201204 : 修正Site No在非標準Pitch對應不上的問題
            break;
        case 1250:
            if(MoveInArm2XYToShuttle_9045_2x4_16(0, ZAxisDown, true))
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
        case 1300:
            InitDoInArmCheckShtFloatTask();                                     //Steven 20220427 : 整合蝦頭搖搖敲敲功能
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
            FLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
            MOT[MInShuttle1].fCanMoveL=true;

            if(In_Shuttle_Auto_Latch==eInSHAutoLtc &&
               bInSHLtcFin[0]==false               &&
               FLCarryKit.UseSiteFullIC())                                      //KenHsieh 20251106 : fix close site don't do In Sht Latch
            {
                Task=1400;
                break;
            }

            AdjustShtOrderWhenPlaceToSht(1);                                    //Steven 20180601 : 整合放完蝦頭後的調整

//            if(TestIF_File.iTestMode==_32Site4X8N ||                            //Steven 20220321 : Fixed for 2x8_16 close site hang up
//               TestIF_File.iTestMode==_16Site4X4)                               //Steven 20220321 : Fixed for 4x4 close site hang up
            {
                if(InArmSideAllClose_4x4_16(1))
                {
                    BLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                }
            }

            if(IniConfig.bIndexPickupWait==true &&                              //jou 2012-06-29 Index Pick up need wait Soak Time
               (LastSet.iTemperature==Tempture_Hot ||
                LastSet.iTemperature==Tempture_AmbientHot))                     //kevin 20180903 (Steven) : add 恆溫控制
            {
                if(Temperature.iShuttleSoakTimeMode==1 && Temperature.iInitialStart2Time!=0)
                {
                    FLCarryKit.TSoakTime.SetSecAndOn(Temperature.iInitialStart2Time);
                    dwStartShuttle1Soak=MyTickCount();                          //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
                    iInitialStart2Count=Temperature.iInitialStart2Time;
                }
            }
            fObserver->RecordInArmTime();
            bDestoryOnSht=false;                                                //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            iInArmPickPlaceCnt[InOfsInSh1]++;                                   //JerryYang 20180921 Setup Teach功能

            if(InArmHasICForSht_2x4_16(1)==true)
            {
                iPickRow=0;
                InArmSuck.iWhichSht=1;
                InArmSuck.iWhichShtPickFor32=1;
                bPlaceOtherShuttle=true;
                Task=1900;
                return false;
            }
            else
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                    if(InArmSuck.Item[1][j]==HAS_NULL_IC)
                        InArmSuck.SetItemData(1, j, NULL_IC);
            }

            bPlaceOtherShuttle=false;

            if(USE_Y_AUTO_PITCH==true && IniConfig.bE57YPitchHome)              //kevin 20180827 (Steven) : add put shuttle Y pitch home AutoYPitch
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
                hInArmYpitchHomeTimer.SetSecAndOn(20);
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
            if(InSHT2InLF() && BLCarryKit.HasIC()==false)
            {
                if(bCheckSpeed)                                                                                         //Steven 20110525 : Auto Speed
                {
                    bCheckSpeed=false;
                    InArmAddSpeed();
                }

                if(bCheckSpeed1)                                                                                        //KaiChen 20171225 (Steven)：Add Speed Display
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

            if(bCheckSpeed1)                                                                                            //KaiChen 20171225 (Steven)：Add Speed Display
            {
                bCheckSpeed1=false;
                InArmSubSpeedDisplay();
            }
            Task=1930;
        case 1930:
            if(MoveInArm2XYToShuttle_9045_2x4_16(1, ZAxisNotDown, true))
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
            if(MoveInArm2XYToShuttle_9045_2x4_16(1, ZAxisDown, true))
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
            if(iOneCycle==1 && IndexStatus==Z1Up_Z2Down &&                                                              //jou 2010-01-18 start : place to shuttle時，回Home，會hang
               FLCarryKit.NoIC() && FTestSuck.NoIC())
            {
                FLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
            }

            if(InSHT2InLF())
            {
                if(IniConfig.bP17InArmFullPickFromLoader)                                                               //ChungHung 20111230 add Shuttle必須放滿
                {
                    if(IsBLCarrKitAllHasIC() && bPlaceToShuttle2Step==false)                                            //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                    {
                        MOT[MInShuttle2].fCanMoveL=true;
                        BLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                        if((FLCarryKit.HasIC()==false || FTestSuck.NoIC()) && InSHT1InLF())
                        {
                            MOT[MInShuttle1].fCanMoveL=true;
                            FLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                        }
                    }
                    else
                    {
                        MOT[MInShuttle2].fCanMoveL=false;
                        InArmZNeedDown_9045(1, 0/*iKit -- W6.2b arity*/, true);
                        Task=2100;
                        goto IN_ARM_SHUTTLE_2x4_16;
                    }
                }
                else
                {
                    if(BLCarryKit.HasIC()!=NULL_IC &&
                       bPlaceToShuttle2Step==false)                                                                     //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                    {
                        MOT[MInShuttle2].fCanMoveL=true;
                        BLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                        if((FLCarryKit.HasIC()==false || FTestSuck.NoIC()) && InSHT1InLF())
                        {
                            MOT[MInShuttle1].fCanMoveL=true;
                            FLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                        }
                    }
                    else
                    {
                        MOT[MInShuttle2].fCanMoveL=false;
                        InArmZNeedDown_9045(1, 0/*iKit -- W6.2b arity*/, true);
                        Task=2100;
                        goto IN_ARM_SHUTTLE_2x4_16;
                    }
                }
            }

            if(MOT[MInShuttle2].Led[iInposLed]==false && MOT[MInShuttle2].fCanMoveL==false)
            {
                MOT[MInShuttle2].fCanMoveL=true;
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
            if(MoveInArmZToShuttlePlace_9045(1))                                                                        //wait shuttle in Left position
            {
                if(InArmNeedCheckOffset(true, 1))                                                                       //Steven 20230531 : 簡化判斷式
                {
                    Task=2150;
                    break;
                }

                Task=2200;
                if(ArmSpeed[InArm].iEnableReleaseDelay==0)                                                              //JerryYang 20160127 for TSMC inarm release device前delay
                {
                    InArmReleaseDelay.SetSecAndOn(ArmSpeed[InArm].dReleaseDelayTime);
                    Task=2180;
                }
                #ifndef SOFT_SIMULTE
                goto IN_ARM_SHUTTLE_2x4_16;
                #endif
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
            if(MoveInArm2XYToShuttle_9045_2x4_16(iWhichSht32, ZAxisDown, true))
            {
                Task=2100;
            }
            break;
        case 2180:
            if(InArmReleaseDelay.Off())                                                                                 //JerryYang 20160127 for TSMC inarm release device前delay
            {
                Task=2200;
            }
            break;
        case 2200:
            flag=true;
            bPlaceToShuttle2Step=true;                                                                                  //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
            for(int i=0; i<InArmSuck.iPickRow; i++)
            {
                for(int j=0; j<InArmSuck.iPickCol; j++)
                {
                    iYPos=XPHSuckToSht_2x4_16[InArmSuck.iModeX][j];
                    if(InArmSuck.Item[i][j]!=NULL_IC && InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                    {
                        iBackInArmHotCount=InArmSuck.HotCount;
                        if(InArmSuck.Item[i][j]==HAS_NULL_IC || InArmSuck.Suck[i][j].Destroy())
                        {
                            if(InArmSuck.Item[i][j]!=HAS_NULL_IC)                                                       //Steven 20170109 : 沒IC的地方不檢查吹氣
                            {
                                bInArmCheckDestroyACT[i][j]=true;                                                       //jou 981130 確認device確實destroy完成
                                LastSet.lShuttleCount++;                                                                //Steven 20170113 : For count place to shuttle
                            }
                            InArmSuck.PordRec[i][j].AddTestRecord(2, TestIF.iSiteMap[i][iYPos]);                        //Frank 20160505 add
                            SetShuttleStatus_9045(iWhichSht32, i, iYPos, i, j);
                            BLCarryKit.HotCount=InArmSuck.HotCount;                                                     //JerryYang 20230204 : add hot plate放置順序資料
                        }
                        else if(InArmSuck.Suck[i][j].Error==false)
                        {
                            flag=false;
                        }
                    }
                }
            }

            if(flag==false)                                                                                             //kevin 20131011 在下真空誤判, 換到上面來
                break;

            for(int i=0; i<InArmSuck.iPickRow; i++)
                for(int j=0; j<InArmSuck.iPickCol; j++)
                    if(InArmSuck.Item[i][j]!=NULL_IC && InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                        return false;

            if(InArmSuck.HasRealIC()==false)
            {
                Task=2300;
                bPlaceToShuttle2Step=false;                                                                             //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                fObserver->AddTimeData(2, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);
            }
            else
            {
                bPlaceToShuttle2Step=true;                                                                              //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                Task=2250;
            }
            AddArmSiteRecord(1);                                                                                        //Steven 20201204 : 修正Site No在非標準Pitch對應不上的問題
            break;
        case 2250:
            if(MoveInArm2XYToShuttle_9045_2x4_16(1, ZAxisDown, true))
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
        case 2300:
            InitDoInArmCheckShtFloatTask();                                                                             //Steven 20220427 : 整合蝦頭搖搖敲敲功能
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
            BLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);

            if(In_Shuttle_Auto_Latch==eInSHAutoLtc &&
               bInSHLtcFin[1]==false               &&
               BLCarryKit.UseSiteFullIC())                                                                              //KenHsieh 20251106 : fix close site don't do In Sht Latch
            {
                Task=2400;
                break;
            }

            AdjustShtOrderWhenPlaceToSht(2);                                                                            //Steven 20180601 : 整合放完蝦頭後的調整

//            if(TestIF_File.iTestMode==_16Site4X4 ||
//               TestIF_File.iTestMode==_32Site4X8N)                              //Steven 20220321 : Fixed for 4x4 close site hang up
            {
                if(InArmSideAllClose_4x4_16(0))
                {
                    FLCarryKit.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                }
            }
            MOT[MInShuttle2].fCanMoveL=true;

            if(IniConfig.bIndexPickupWait==true &&                                                                      //jou 2012-06-29 Index Pick up need wait Soak Time
               (LastSet.iTemperature==Tempture_Hot ||
                LastSet.iTemperature==Tempture_AmbientHot))                                                             //kevin 20180903 (Steven) : add 恆溫控制
            {
                if(Temperature.iShuttleSoakTimeMode==1 && Temperature.iInitialStart2Time!=0)
                {
                    BLCarryKit.TSoakTime.SetSecAndOn(Temperature.iInitialStart2Time);
                    dwStartShuttle2Soak=MyTickCount();                                                                  //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
                    iInitialStart2Count=Temperature.iInitialStart2Time;
                }
            }
            fObserver->RecordInArmTime();
            bDestoryOnSht=false;                                                                                        //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            iInArmPickPlaceCnt[InOfsInSh2]++;                                                                           //JerryYang 20180921 Setup Teach功能
            if(USE_Y_AUTO_PITCH==true && IniConfig.bE57YPitchHome)                                                      //kevin 20180827 (Steven) : add put shuttle Y pitch home AutoYPitch
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
void DoInArm_9045_2x4_16_SuckerMap()
{
    int j1, j2;
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
       bSiteMapHotplateReady==true              &&                              //Ifor 20170920 (Steven) : add Auto Site Mapping Hotplate 擺放旗標
       bRunAutoClean==false)                                                    //jou 20200618 : 修正 Auto Clean 跟 Auto site mapping 衝突
    {
        DoInArmAutoSiteMapping();
    }
    else
    {
        for(int i=0; i<InArmSuck.iShtRow; i++)
        {
            for(int j=0; j<InArmSuck.iShtCol; j++)
            {
                j2=XPHShtToSuck_2x4_16[0][InArmSuck.iModeX][j];
                j1=XPHShtToSuck_2x4_16[1][InArmSuck.iModeX][j];
                fInArmSuckUse_TwoArm32Site_local[i+2][j1]=LastSet.bUseTestSocket[0][i][j];
                fInArmSuckUse_TwoArm32Site_local[i+0][j1]=LastSet.bUseTestSocket[1][i][j];
                fInArmSuckUse_TwoArm32Site_local[i+2][j2]=LastSet.bUseTestSocket[0][i][j];
                fInArmSuckUse_TwoArm32Site_local[i+0][j2]=LastSet.bUseTestSocket[1][i][j];
            }
        }
    }

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            Prod.fInArmSuckUse[0][i][j]=fInArmSuckUse_TwoArm32Site_local[i+2][j];
            Prod.fInArmSuckUse[1][i][j]=fInArmSuckUse_TwoArm32Site_local[i+0][j];
        }
    }
}
//==============================================================================
void DoInArm_9045_2x4_16()
{
    int &Task=iArmTask;
    static int iAdjustSpeed=0;
    static bool bOne=false;
    static bool bDownSpeed=false;                                               //ChungHung 20121227 add
    static bool bCleanOut=false;                                                //ChungHung 20110407 add
    bool flag=false;
    static int iDisplaySpeed=0;                                                 //KaiChen 20171225 (Steven)：Add Speed Display
    bool bCheckCloseSiteHaveIC=false;                                           //kevin 20130115
    AnsiString ErrPart="";
    bool bHasDuplicateErr=false;                                                //kevin 20130115
    DoJudgeInputShuttleNeedChangeToNullIC();

    if(DoInArmAutoSiteMapping() ||                                              //jou 2011-03-24 start : Auto Site Mapping
       bIndexAlarmInArmAway)                                                    //kevin 20181102 (Steven) :  index回吸檢測有IC inarm 讓位 inarm先不要動
    {
        return ;                                                                //jou 2016-11-03 JCET 要求Auto Site Mapping 需等待測試結果在繼續入料
    }

    int iShtHP, iKitHP;
    bool bflag1=false, bflag2=false;
    int iSht=(InArmSuck.iWhichSht==0)?2:0;
    int iKit=0;
    if(LastSet.iTemperature==Tempture_Hot)                                      //Steven 20150626 : Fixed 32 site 吸取偵測異常    //Steven 20180525 : 修正一直跑Hotplate iWhichKit Close
    {
        iShtHP=(InArmSuck.iWhichShtPickFor32==0)?2:0;
        iKitHP=0;
    }
    else
    {
        iShtHP=(InArmSuck.iWhichSht==0)?2:0;
        iKitHP=0;
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
                        InArmSuck.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
                        InitInArmPlaceToHotPlateTask();
                    }
                    else
                    {
                        if(bPlaceToHotplatePartOK==true)
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
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                                                         //kevin 20201015 add ASE off line no check Temp
            {
                if(LastSet.iTester==ON_LINE && LastSet.iTemperature==Tempture_Hot && fHeaterOK==false)
                {
                    Task=1;
                    break;
                }
            }
            else
            {
                if(LastSet.iTemperature==Tempture_Hot &&
                   (fHeaterOK==false ||                                                                                 //Steven 20120314 : 加熱還沒好,不要亂動
                    fHeaterStableOK==false))                                                                            //JerryYang 20210122 : ASE-CL新增待溫功能
                {
                    Task=1;
                    break;
                }
            }

            InitArmPickFromLoadStageTask();
            Task=75;

            if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)
            {
                if(NeedWaitTrayArm==true)                                                                               //wei 20151002
                {
                    Task=1;                                                                                             //jou 2016-07-14 修正OCR In arm 讓位造成Hotplate疊料issue
                    return;
                }
            }

            if(CosFunction.bRTCAutoModelVerify==true &&
               IniConfig.bD36EnableRTCAutoModelVerify==true)                                                            //jou 2014-06-24 RTC 自動進行Model驗證
            {
                if(bNeedWaitRTCAutoVerify==true)
                {
                    Task=1;
                    return;
                }
            }

            if(LastSet.iTemperature==Tempture_Hot &&
               (IniConfig.bE39CheckHotPlateAfterCleanOutAndBeforeTrayFeed==true ||
                IniConfig.bO01_ResetNeedClearAndCheckHP) &&
               bCanUseHotPlateCheck==true &&
               (bNeedTrySuckHotPlate==true || bHPCleanout==true) &&                                                     //wei 20160624 Hotplate clean out
               bOneTimeHotPlateCheckAll==true)                                                                          //Steven 20120319 : Clean Out後的檢查
            {
                ;
            }
            else
            {
                if(MOT[MMTrayY].HasIC()==false             &&                                                           // load and load buffer no any tray
                   MOT[MMTrayY].fHasTray==false            &&
                   MOT[MMTrayY_Car].Tray.HasIC()==false    &&
                   MOT[MMTrayY_Car].fHasTray==false)
                {
                    if(iCleanOut && iOneCycle==0 &&                                                                     //ChungHung 20110829 add
                       LastSet.iTemperature==Tempture_Hot &&
                       (MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC()))
                    {
                        DisableAutoSiteMapWhenCleanOut();                                                               //Steven 20220902 : 修正Auto Site Map, Loader沒有IC
                        Task=500;
                        break;
                    }
                }
            }

            if((MOT[MMTrayY].HasIC()==false  &&
                MOT[MMTrayY].fHasTray==true) || bNeedSlapTray)
            {
                Task=1;
                break;
            }

            if(LastSet.iTemperature==Tempture_Hot)
                flag=CheckHasSpaceToPlace_9045();
            else
                flag=false;

            if(iCleanOut && LastSet.iTemperature==Tempture_Hot &&
               IsInArmCleanOutFinish()==false)                                                                          //Steven 20230309 : fixed for NN mode hang up
            {
                if(bRunAutoSiteMapping==true && flag)                                                                   //Ifor 20210426 add: Auto Site Mapping Clean Out 補料回Hot plate
                {
                    if(MOT[MMTrayY].HasIC() || MOT[MMTrayY_Car].fHasTray)
                    {
                        Task=75;
                        break;
                    }
                }

                if((TrayForm.bAutoFeed || bMustCleanAllTray) && flag)                                                   // 2010/08/24 lee disable mark
                {
                    if(MOT[MMTrayY].HasIC() || MOT[MMTrayY_Car].fHasTray)
                    {
                        Task=75;
                        break;
                    }
                }

                if((MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC()) && HasHotReadyIC_9045()/*W6.2b: engine sig 0-arg*/)
                {
                    if(TrayForm.bAutoFeed==false && bMustCleanAllTray==false)
                        bCleanOut=true;
                    Task=500;
                    break;
                }

                if(TrayForm.bAutoFeed ||
                   (MOT[MMTrayY].fHasTray==false &&
                    MOT[MMTrayY_Car].fHasTray==false) ||
                   bMustCleanAllTray)
                {
                    if(flag)
                        Task=75;
                    else
                        Task=500;
                }
                else
                {
                    Task=500;
                }
                break;
            }

            if(LastSet.iTemperature==Tempture_Hot && flag==false)                                                       //Ifor 20180507 : add 避免Auto Site Mapping 未補料直接取IC造成資料異常
            {
                if(LastSet.iRunStartMode==rsmAutoSiteMap)
                {
                    if(bAutoSiteMapHotplateReady==true && bAutoSiteMapHotplateSave==false)
                    {
                        Task=500;
                    }
                }
                else
                {
                    Task=500;
                }
            }

            if(LastSet.iTemperature==Tempture_Hot && iAdjustSpeed==1 && ArmSpeed[InArm].bAutoSpeed && Task!=500)
            {
                iAdjustSpeed=2;
            }

            if(LastSet.iTemperature==Tempture_Hot && iDisplaySpeed==1 &&
               IniConfig.bA26MotorSpeedSortDisplay==true)                                                               //KaiChen 20171225 (Steven)：Add Speed Display
            {
                iDisplaySpeed=2;
                InArmAddSpeedDisplay();
            }

            if((iOneCycle || (iCleanOut && IsInArmCleanOutFinish())) && InArmSuck.HasIC()==false)
            {
                bInArmWaitOneCycle=true;                                                                                //JerryYang 20220324 : Inarm是否完成ONE CYCLE
                if(LastSet.iRunStartMode!=rsmQAMode)                                                                    //Steven 20220120 : 修正NN模式 One Cycle死雞
                {
                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC())
                        {
                            SearchPlateToPick();                                                                        //確認下一個要做的是哪邊
                            if(InArmSuck.iWhichShtPickFor32==1 ||
                               InArmSuck.iWhichKitPickFor32==1)                                                         //假如跑ARM 2, 並且做了一半
                            {
                                if(TestIF.iShuttleMode==1 &&
                                   TestIF.iShuttle_Sel==1 &&
                                   InArmSuck.iWhichKitPickFor32==0)                                                     //假如只開ARM 2, 並且還沒開始做
                                {
                                }
                                else
                                {
                                    Task=500;
                                    break;
                                }
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
                            if((FLCarryKit.HasRealIC() || (FTestSuck.HasIC() && FTestSuck.AlreadyTest()==false)) &&     //Shuttle 1或Arm 1有IC
                               (BLCarryKit.NoIC() && BTestSuck.NoIC()))                                                 //Shuttle 2或Arm 2沒IC
                            {
                                if(InArmSideAllClose_4x4_16(1))                                                         //Steven 20230306 : 修正NN mode常溫關Site Hang Up
                                {
                                }
                                else
                                {
                                    Task=100;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            flag=true;
                        }
                    }
                }

                if(MoveInArm2XYToWait()==false)
                {
                    Task=50;
                    return;
                }

                if(bOne)
                {
                    if(flag)
                        SetShuttleToHasNullICWhenCleanOut();                                                            //Steven 20230309 : 修正Clean時, 都沒IC造成的Hang Up
                    bOne=false;

                    MOT[MInShuttle1].fCanMoveL=true;
                    MOT[MInShuttle2].fCanMoveL=true;
                }
                Task=50;
                break;                                                                                                  //JerryYang 20220324 : add
            }
            bInArmWaitOneCycle=false;                                                                                   //JerryYang 20220324 : Inarm是否完成ONE CYCLE
            if(Task==75 && LastSet.iTemperature==Tempture_Hot && flag==false && LastSet.iRunStartMode!=rsmAutoSiteMap)
                Task=50;

            if(Task!=75)                                                                                                //Steven 20180813 : add in arm speed
                break;
        case 75:
            if(LastSet.iTemperature==Tempture_Hot &&                            //Steven 20170718 (wei) : HP Check加上保護
               bNeedTrySuckHotPlate &&
               bAutoSiteMapHasPickHP==false)                                    //jou 20231018 : 修正E38與autositemapping衝突造成hotplate疊料問題
            {
                InArmSuck.SetType1ToType2ByPickCol(NULL_IC, HAS_TRY_SUCK_IC);
                InitInArmTryPickFromHotPlateTask();
                Task=15000;
            }
            else
            {
                TryHotClearRecBufMMPlate();                                     //ChungHung 20120206 Hotplate check
                InitArmPickFromLoadStageTask();
                Task=100;
            }

            if(Task!=100)                                                       //Steven 20180813 : add in arm speed
                break;
        case 100:
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                 //kevin 20201015 add ASE off line no check Temp
            {
                if(LastSet.iTester==ON_LINE && LastSet.iTemperature==Tempture_Hot && fHeaterOK==false && iPickFromLoadStageTask==1)
                {
                    Task=1;
                    break;
                }
            }
            else
            {
                if(LastSet.iTemperature==Tempture_Hot &&
                   (fHeaterOK==false ||
                   fHeaterStableOK==false) &&                                   //JerryYang 20210122 : ASE-CL新增待溫功能
                   iPickFromLoadStageTask==1)
                {
                    Task=1;
                    break;
                }
            }

            if(CUSTOMER_CODE==CC_ASE_M &&
               LastSet.iRunStartMode==rsmAutoSiteMap &&                         //Ifor 20190529 : add 避免 ASEM 常溫Site Mapoing 完成後多入料一次
               LastSet.iTemperature!=Tempture_Hot &&
               bASMFinishOneCycle==true && iOneCycle==1)
            {
                Task=1;
                break;
            }

            if(USE_TRAY_MAPPING==1 &&
               TestIF_File.bEnableDeviceRemain==true &&
               bDoTrayDeviceCheck)                                              //Sam 20191113 : 防止殘料檢與 PickFormLoad 打架
            {
                break;
            }

            flag=DoArmPickFromLoadStage_9045_2x4_16();
            if(flag)
            {
                iInArmPickPlaceCnt[InOfsLoader]++;                              //JerryYang 20180921 Setup Teach功能
                bHangTimePause=true;                                            //Steven 20090827 : Hang Up dectector
                if(InArmSuck.HasIC()==false)
                {
//                    if(InArmSideAllClose_4x4_16(InArmSuck.iWhichSht)==true) //Steven 20230331 : 避免關Site死雞
//                        AdjustShuttleWhichKitOrder();
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
        case 200:
            CheekNeedToDoInArmAdditionalFunction();                             //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
            flag=CheckCloseSiteHasIC(0, iSht, 0);                               //kevin 20130115 關SITE 吸取IC
            if(flag)
            {
                Task=300;
                return;
            }
            Task=400;
            break;
        case 300:
            ErrPart=" ";
            bHasDuplicateErr=false;
            bCheckCloseSiteHaveIC=false;
            if(IniConfig.bUseAutoSiteMapping && LastSet.iRunStartMode==rsmAutoSiteMap &&
               (TestIF_File.iTestMode==_16Site4X4 ||                            //kevin 20190516 add _16Site4X4
                TestIF_File.iTestMode==_32Site4X8N))
            {
                Task=400;
            }
            else
            {
                for(int i=0; i<InArmSuck.iPickRow; i++)
                {
                    for(int j=0; j<InArmSuck.iPickCol; j++)
                    {
                        if(fInArmSuckUse_TwoArm32Site_local[iSht+i][iKit+j]==false && InArmSuck.Suck[i][j].GetStatus())
                        {
                            bHasDuplicateErr=true;
                            bCheckCloseSiteHaveIC=true;
                            ErrPart+=InArmSuck.Suck[i][j].sName;
                            InArmSuck.SetItemData(i, j, HAS_NULL_IC);
                        }
                    }
                }

                if(bCheckCloseSiteHaveIC)
                {
                    ShowErrorMessage("JAM0114", K_RETRY, MInArmX, bHasDuplicateErr, ErrPart);
                }
                else
                {
                    Task=400;
                }
            }
//            break;
        case 400:
            flag=DoInArmAdditionalFunction();                                   //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
            if(flag==true)
            {
                InArmSuck.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
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
                    if(LastSet.iTemperature==Tempture_Hot && LastSet.iRunStartMode==rsmAutoSiteMap && bSiteMappingCHKOK==false)
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
            if((MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC()) &&              //Steven 20170511 (wei) : 使用initial delay當 Soak time
               HasHotReadyIC_9045()/*W6.2b: engine sig 0-arg*/)                                            //有加熱完成 && 沒有空間放置
            {
                if(MOT[MMTrayY].HasIC()==false && MOT[MMTrayY].fHasTray==true)
                {
                    Task=1;
                    break;
                }
                iHeaterWaitTime=0;
                InitInArmPickFromHotPlateTask();
                bOneTimeWait=false;

                if(iDisplaySpeed==1 &&
                   IniConfig.bA26MotorSpeedSortDisplay==true)                   //KaiChen 20171225 (Steven)：Add Speed Display
                {
                    iDisplaySpeed=2;
                    InArmAddSpeedDisplay();
                }
                Task=1500;
            }                                                                   //沒有加熱完成 && 沒有空閒位置
            else
            {
                Task=50;
                iHeaterWaitTime=GetHeaterWaitTime();
                bHangTimePause=true;                                            //Steven 20090827 : Hang Up dectector

                if(iDisplaySpeed==1 &&
                   IniConfig.bA26MotorSpeedSortDisplay==true)                   //KaiChen 20171225 (Steven)：Add Speed Display
                {
                    iDisplaySpeed=2;
                    InArmSubSpeedDisplay();
                }

                if(CheckHasSpaceToPlace_9045()==false)
                    Task=600;
            }
            break;
        case 600:
            if(MoveInArmZToPlateSafe(Task))
            {
                if(bInArmToPickHotPlatePos)
                {
                    Task=700;
                }
                else
                {
                    bDownSpeed=true;
                    Task=50;
                }
            }
            break;
        case 700:
            if(MoveInArmXYPickHotPlate_9045(false)/*W6.2b: engine sig 1-arg*/)
            {
                if(IsCheckInArmDestroyActiveFinish())                           //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
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
            InArmSuck.SetType1ToType2ByPickCol(NULL_IC, HAS_NULL_IC);
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

            if(DoInArmPlaceToHotPlate_9045())
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
//            bPickFromHotplate=true;
            bPlaceToHotplate=false;

            if(IniConfig.bI23HotTestWaitingMode && bChangeToInitStartMode)      //Chunghung 20111230 Hot Test Waiting Mode
            {
                bChangeToInitStartMode=false;
                ShowMyMessage("Tester Ready?", "", "", true);
            }
            WhichShuttleReady(false);                                           //判斷哪些Sht ready，並檢視目前能吸取IC狀態
            if(iPickFromPlateMode!=-1)                                          //-1:none 0:shuttle1 1:shuttle2 2:both
            {
                Task=1501;
            }
            break;
        case 1501:
            if(DoInArmPickFromHotPlate_9045_2x4_16(bDownSpeed))
            {
                bDownSpeed=false;
                bPickFromHotplate=false;
                if(InArmSuck.HasIC()==false)
                {
                    if(bRunAutoSiteMapping==true)                               //Ifor 20180518 : add 簡化Site Mapping 旗標
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

                if(IniConfig.bI28_OnOffSiteOnTheFly==false &&                   //Steven 20230131 : I28與E54衝突
                   IniConfig.bE54CheckCloseSiteNoIC)                            //Steven 20160922 : 因為OneCycle永遠先跑蝦頭1, 檢查加熱盤錯誤功能與ByArmCloseSite衝突
                {
                    flag=CheckCloseSiteHasIC(1, iShtHP, iKitHP);                //kevin 20130115 關SITE 吸取IC

                    if(flag)
                    {
                        Task=1600;
                        return;
                    }
                }

                if(InArmSuck.All_HasIC())                                       //全部都有料就是從Shuttle 1開始
                {
                    InArmSuck.iWhichShtPickFor32=0;
                }

                InitInArmPlaceToShuttleTask();
                Task=2000;
            }
            break;
        case 1600:
            ErrPart=" ";
            bHasDuplicateErr=false;
            bCheckCloseSiteHaveIC=false;

            for(int i=0; i<InArmSuck.iPickRow; i++)
            {
                for(int j=0; j<InArmSuck.iPickCol; j++)
                {
                    if(fInArmSuckUse_TwoArm32Site_local[iShtHP+i][iKitHP+j]==false &&
                       InArmSuck.Suck[i][j].GetStatus())                        //kevin 20130115 關SITE 吸取IC
                    {
                        bHasDuplicateErr=true;
                        bCheckCloseSiteHaveIC=true;
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                        InArmSuck.SetItemData(i, j, HAS_NULL_IC);
                    }
                }
            }

            if(bCheckCloseSiteHaveIC)
            {
                ShowErrorMessage("JAM0114", K_RETRY, MInArmX, bHasDuplicateErr, ErrPart);
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
            bPlaceShuttle=true;
            iInRotateFinish=3;                                                  //kevin 20130524 IC已放置SHUTTLE

            if(DoPlaceToShuttle_9045_2x4_16())
            {
                bPlaceShuttle=false;                                            //ChungHung 20120226 add
                fYieldMonitoring->DoAutoCloseSite(false);                       //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
                iInRotateFinish=0;                                              //kevin 20130524 放完SHUTTLE
                iSetShuttleToHasNullIC=0;

//                if(TestIF_File.iTestMode!=_16Site4X4 &&                         //Steven 20220321 : Fixed for 4x4 close site hang up
//                   TestIF_File.iTestMode!=_32Site4X8N)
//                {
//                    if(TestIF.iShuttleMode)                                     //Use signal shuttle
//                    {
//                        if(TestIF.iShuttle_Sel==USE_FRONT_TEST_HEAD)
//                            iSetShuttleToHasNullIC=CLEAR_REAR_TEST_HEAD;
//                        else
//                            iSetShuttleToHasNullIC=CLEAR_FRONT_TEST_HEAD;
//                    }
//                }
                bHangTimePause=true;                                            //Steven 20090827 : Hang Up dectector
                if(LastSet.iRealDummy==DUMMY && iCleanOut)
                {
                    //AI(W6.2b-2x4_16) 20260626: golden 1-arg InitNewTray(NULL_IC) targets a
                    //signature absent in golden mymotor.h (3-arg, no defaults); call the real
                    //3-arg form faithfully (no site-map show, this fn name).
                    MOT[MMTrayY].InitNewTray(NULL_IC, false, "DoInArm_9045_2x4_16");
                }

                Task=50;
                if(LastSet.iTemperature==Tempture_Hot)
                {
                    HasHotReadyIC_9045()/*W6.2b: engine sig 0-arg*/;
                    if(iOneCycle || (IniConfig.bUseAutoSiteMapping==true && LastSet.iRunStartMode==rsmAutoSiteMap && bSiteMappingCHKOK==false))
                    {
                        AdjustShuttlePlaceOrder_AutoSiteMapping();              //jou 2016-11-07 JCET fix Auto Site mapping Hot mode hangup
                    }
                    else
                    {
                        if(bNeedTrySuckHotPlate)                                //ChungHung 20120206 Hotplate check
                        {
                            if(bOneTimeHotPlateCheckAll)
                                Task=75;
                        }
                    }
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
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                 //kevin 20201015 add ASE off line no check Temp
            {
                if(LastSet.iTester==ON_LINE && LastSet.iTemperature==Tempture_Hot && fHeaterOK==false && iInArmTryPickFromHotPlateTask==1)
                {
                    Task=1;
                    break;
                }
            }
            else
            {
                if(LastSet.iTemperature==Tempture_Hot && (fHeaterOK==false || fHeaterStableOK==false) && iInArmTryPickFromHotPlateTask==1)
                {
                    Task=1;
                    break;
                }
            }

            if(DoInArmTryPickFromHotPlate_9045(bOneTimeHotPlateCheckAll, bTryPickFromHotPlateShowError))
            {
                InArmSuck.SetType1ToType2ByPickCol(HAS_TRY_SUCK_IC, NULL_IC);
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
                else                                                            //hotplate有殘留IC
                {
                    iHeaterWaitTime=0;
                    bOneTimeWait=false;
                    InitInArmPlaceToShuttleTask();
                    Task=2000;
                }
            }
            break;
    }
}
//==============================================================================
