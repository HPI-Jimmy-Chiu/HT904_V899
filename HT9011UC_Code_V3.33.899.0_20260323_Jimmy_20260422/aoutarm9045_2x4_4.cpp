#include "MachineDefine.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include "aoutarm9045_2x4_4.h"

#include "aArmHeader.h"
#include "MyKitSuck.h"
#include "cprod.h"
#include "mymotor.h"
#include "mycylin.h"
#include "cSortCT.h"
#include "mysensor.h"
#include "main.h"
#include "note.h"
#include "csystem.h"
#include "acatchtray.h"
#include "cinitial.h"
#include "atester.h"
#include "aRotateKIT.h"
#include "fRotate.h"
#include "cOffSet.h"
#include "fAOI.h"
#include "cmydef.h"
#include "ProductionInfo.h"
#include "AutoTeach.h"
#include "FixAICCD.h"
#include "Magazine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//==============================================================================
//個位數為Y座標, 2=標準, 0=A排, 1=B排
//==============================================================================
int GetNowShuttleMode_2x4_4(int iSht)
{
    ptrOutSHT=(iSht==0)?&FRCarryKit:&BRCarryKit;
    int iKit=0;

    if(OutArmSuck.iXStep==1 && OutArmSuck.iYStep==1)
    {
        //AI(ht9045-v899) 20260810: 原本這裡一律 return 2, 在判斷左右半 kit 前就早退, 使 iModeCol 恆為 0, OutArm 取右半 kit 時算出的 X 與左半同一點(差 2 個 Site X-Pitch), 右半 IC 必吸空 -> JAM0202 重複發生, SKIP 又把沒去過的 Shuttle 格清成 NULL_IC -> 殘料回 Index 疊成 double IC; 改成沒料才回標準值 2, 有料時比照 iOutArmiWhichKit 的同一判斷式 LeftSideNoIC() 回 10002(左)/11002(右), 讓 CheckOutArmXYPitch_2x4_4() 既有的 iModeCol==110 分支生效(原為死碼), 行為對齊 V3.32
        if(ptrOutSHT->UseSiteNoIC())
            return 2;
        else if(ptrOutSHT->LeftSideNoIC(OutArmSuck.iShtKitStep))
            return 11002;                                                   //右半四顆(kit1)
        else
            return 10002;                                                   //左半四顆(kit0)
    }
    else if(ptrOutSHT->UseSiteNoIC())
    {
        if(OutArmSuck.iXStep==1)
        {
            return 0;
        }
        else
        {
            if(OutArmSuck.iYStep==1)
                return 10002;                                                   //左邊兩顆
            else
                return 10000;                                                   //左上一顆
        }
    }
    else
    {
        if(ptrOutSHT->LeftSideNoIC(OutArmSuck.iShtKitStep))
            iKit=OutArmSuck.iShtKitStep;
        else
            iKit=0;
        if(OutArmSuck.iXStep==1)
        {
            if(OutArmSuck.iYStep==1)
            {
                if(ptrOutSHT->Item[0][0]>=HAS_IC || ptrOutSHT->Item[0][1]>=HAS_IC ||
                   ptrOutSHT->Item[1][0]>=HAS_IC || ptrOutSHT->Item[1][1]>=HAS_IC)
                    return 10002;                                               //左邊四顆
                else //if(ptrOutSHT->[0][2]>=HAS_IC || ptrOutSHT->[0][3]>=HAS_IC ||
                     //   ptrOutSHT->[1][2]>=HAS_IC || ptrOutSHT->[1][3]>=HAS_IC)
                    return 11002;
            }
            else
            {
                if(ptrOutSHT->Item[0][0]>=HAS_IC || ptrOutSHT->Item[0][1]>=HAS_IC)
                    return 10000;                                               //左上兩顆
                else if(ptrOutSHT->Item[1][0]>=HAS_IC || ptrOutSHT->Item[1][1]>=HAS_IC)
                    return 10001;                                               //左下兩顆
                else if(ptrOutSHT->Item[0][2]>=HAS_IC || ptrOutSHT->Item[0][3]>=HAS_IC)
                    return 11000;                                               //右上兩顆
                else //if(ptrOutSHT->Item[1][2]>=HAS_IC || ptrOutSHT->Item[1][3]>=HAS_IC)
                    return 11001;
            }
        }
        else if(OutArmSuck.iXStep==2)
        {
            if(OutArmSuck.iYStep==1)
            {
                if(ptrOutSHT->Item[0][0+iKit]>=HAS_IC || ptrOutSHT->Item[1][0+iKit]>=HAS_IC)
                    return 10002;                                               //左邊兩顆
                else //if(ptrOutSHT->[0][1+iKit]>=HAS_IC || ptrOutSHT->[1][1+iKit]>=HAS_IC ||
                    return 11002;                                               //右邊兩顆
            }
            else
            {
                if(ptrOutSHT->Item[0][0+iKit]>=HAS_IC)
                    return 10000;                                               //左上
                else if(ptrOutSHT->Item[1][0+iKit]>=HAS_IC)
                    return 10001;                                               //左下
                else if(ptrOutSHT->Item[0][1+iKit]>=HAS_IC)
                    return 11000;                                               //右上
                else //if(ptrOutSHT->Item[1][1+iKit]>=HAS_IC)
                    return 11001;                                               //右下
            }
        }
    }
    return 2;
}
//==============================================================================
bool OutArmZNeedDown_2x4_4(int iSht)
{
    if(InitialOutArmNeedSuck(iSht)==false)
        return false;

    ptrOutSHT=(iSht==0)?&FRCarryKit:&BRCarryKit;
    int iMode=GetNowShuttleMode_2x4_4(iSht);
    int iModeRow=iMode%100;
    int iModeCol=iMode/100;
    int iShtJ=0, iKit=0;
    if(ptrOutSHT->LeftSideNoIC(OutArmSuck.iShtKitStep))
        iKit=OutArmSuck.iShtKitStep;
    else
        iKit=0;

    for(int i=0; i<OutArmSuck.iPickRow; i++)
    {
        for(int j=0; j<OutArmSuck.iPickCol; j++)
        {
            iShtJ=j+iKit;
            if(iModeCol==0)
            {
                SetOutArmNeedDestory(i, j, i, iShtJ);
            }
            else if(iModeCol==100 && //j==0 &&
                    (iModeRow==2 || iModeRow==i))
            {
                SetOutArmNeedDestory(i, j, i, iShtJ);
            }
            else if(iModeCol==110 && //j==1 &&
                    (iModeRow==2 || iModeRow==i))
            {
                SetOutArmNeedDestory(i, j, i, iShtJ);
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
//AI(ht9045-v899) 20260810: OutArm 逐輪診斷 Log 用的暫存值(純記錄, 不參與任何判斷)
static int iOutArmRoundTgtX=0, iOutArmRoundTgtY=0, iOutArmRoundMode=0;
static int iOutArmRoundOfsPos=0, iOutArmRoundPitchX=0;
bool CheckOutArmXYPitch_2x4_4(int *iX, int *iY, int iSht, int iKit, int iMovePitchX, int iMovePitchY)
{
    int iMode=GetNowShuttleMode_2x4_4(iSht);
    int iModeRow=iMode%100;
    int iModeCol=iMode/100;
    int iOffsetPos=GetOutArmToShuttleOffset_9045(iSht, iModeRow, iModeCol+iKit*10, false);  //Steven 20240826 : Fixed for offset
    double dMovePitchX;

    if(iSht==0)
    {
        *iX+=Prod.XOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase];
        *iY+=Prod.YOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase];
    }
    else
    {
        *iX+=Prod.XOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase];
        *iY+=Prod.YOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase];
    }

    if(iModeRow==0)                                                             //Row A
    {
        if(bUseTwoArm32Site &&
           TestIF_File.iYPitchOffsetMode==0)                                    //向舊版本相容
        {
            if(iOutArmYBase==1)                                                 //RogerYang 20260302 : Fixed for Y Pitch
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
            else                                                                //RogerYang 20260302 : Fixed for Y Pitch
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
        else if(bUseTwoArm32Site &&
                TestIF_File.iYPitchOffsetMode==2)                               //Steven 20201013 : NN mode add Y-Pitch offset selection
        {
            if(USE_OUT_Y_IS_AUTO_PITCH==true)                                   //JerryYang 20251218 : IN/OUT ARM支援不同模組
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
            if(USE_OUT_Y_IS_AUTO_PITCH==true)                                   //JerryYang 20251218 : IN/OUT ARM支援不同模組
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
            if(iOutArmYBase==1)                                                 //RogerYang 20260302 : Fixed for Y Pitch
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
            else                                                                //RogerYang 20260302 : Fixed for Y Pitch
            {
                if(iSht==0)
                {
                    *iY=*iY+iMovePitchY                                         //B排到中間
                            +7000.0-                                             //B排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0)-                          //B排移動半個Center Pitch
                            TestIF.dSiteYPitch;                                 //B排移動到Row B
                }
                else
                {
                    *iY=*iY+iMovePitchY                                         //B排到中間
                            -7000.0+                                            //B排到Shuttle邊緣 (Socket中線)
                            (TestIF.dSiteYOffset/2.0);                          //B排移動半個Center Pitch
                }
            }
        }
        else if(bUseTwoArm32Site &&
                TestIF_File.iYPitchOffsetMode==2)                               //Steven 20201013 : NN mode add Y-Pitch offset selection
        {
            if(USE_OUT_Y_IS_AUTO_PITCH==true)                                   //JerryYang 20251218 : IN/OUT ARM支援不同模組
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
            if(USE_OUT_Y_IS_AUTO_PITCH==true)                                   //JerryYang 20251218 : IN/OUT ARM支援不同模組
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
            if(USE_OUT_Y_IS_AUTO_PITCH==true)                                   //JerryYang 20251218 : IN/OUT ARM支援不同模組
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
            if(USE_OUT_Y_IS_AUTO_PITCH==true)                                   //JerryYang 20251218 : IN/OUT ARM支援不同模組
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
            if(USE_OUT_Y_IS_AUTO_PITCH==true)                               //JerryYang 20251218 : IN/OUT ARM支援不同模組
                *iY=*iY-TestIF.dSiteYPitch/2;
            else
                *iY=*iY+TestIF.dSiteYPitch/2;
        }
    }

    if(USE_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                                 //Ztex 2023.12.06 Add HT-1032  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        dMovePitchX=double(iMovePitchX)/7.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*4.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*4.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==110)
        {
//            if(iInArmType==e9045_2x4_4_14)                                      //Ag --> Ab
//                *iX=*iX-(dMovePitchX*2.0+TestIF.dSiteXPitch*0.5);
//            else if(iInArmType==e9045_2x4_4_13)                                 //Ae --> Ab
                *iX=*iX-(dMovePitchX*0.0+TestIF.dSiteXPitch*0.5);
        }
    }
    else if(USE_PICKER_COUNT==ep16Picker)                                       //基準為第四隻吸嘴
    {
        dMovePitchX=double(iMovePitchX)/7.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==110)
        {
//            if(iInArmType==e9045_2x4_4_14)                                      //Ag --> Ab
//                *iX=*iX-(dMovePitchX*3.0+TestIF.dSiteXPitch*0.5);
//            else if(iInArmType==e9045_2x4_4_13)                                 //Ae --> Ab
                *iX=*iX-(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
    }
    else if(USE_OUT_ARM_Y_PITCH==iXYPitchVariable)                           //基準為第二隻吸嘴  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        dMovePitchX=double(iMovePitchX)/3.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==110)
        {
//            if(iInArmType==e9045_2x4_4_14)                                      //Aa --> Ac
//                *iX=*iX-(dMovePitchX*2.0+TestIF.dSiteXPitch*0.5);
//            else                                                                //Aa --> Ac
                *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
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
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==110)                                                  //Aa --> Ac
        {
//            if(iInArmType==e9045_2x4_4_14)                                      //Aa --> Ac
//                *iX=*iX-(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
//            else                                                                //Aa --> Ac
                *iX=*iX+(dMovePitchX*2.0+TestIF.dSiteXPitch*0.5);
        }
    }

    *iY=*iY+GetOutArmYToShuttleOffset_9045(iSht, iOffsetPos);
    *iX=*iX+GetOutArmXToShuttleOffset_9045(iSht, iOffsetPos);
    TransferOutShuttleRatio(iSht, iX, iY, iOutArmYBase, iOutArmXBase);
    if(OutArmZNeedDown_2x4_4(iSht)==false)
        return false;
    return true;
}
//==============================================================================
bool MoveOutArmToShuttleIncludeZ_9045_2x4_4(int iSht, int iKit, bool bZDown)
{
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXVariable[X_PITCH_COUNT];
    int iXPos               =iOutArmShtXCenterPos;                              //Steven 20141029 : XY-Pitch for Shuttle Center position for base Suck
    int iYPos               =iOutArmShtYCenterPos;
    int iMode               =GetNowShuttleMode_2x4_4(iSht);
    int iModeRow            =iMode%100;
    int iModeCol            =iMode/100;
    int iMovePitchX         =iXpitchMaxX3;
    int iMovePitchY         =GetVariableYOutShuttleData();
    int iOffsetPos          =GetOutArmToShuttleOffset_9045(iSht, iModeRow, iModeCol+iKit*10, true); //Steven 20240826 : Fixed for offset
    int iYVariable          =GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);
    ZeroMemory(iXVariable,  sizeof(iXVariable));
    ZeroMemory(bZFlag,      sizeof(bZFlag));
    ZeroMemory(iZPos,       sizeof(iZPos));

    if(USE_16PICKER_TYPE==1)
    {
        if(OutArmSuck.iXStep==1)
//            iMovePitchX=TestIF.dSiteXPitch/2*7;
//        else if(OutArmSuck.iXStep==2)
            iMovePitchX=TestIF.dSiteXPitch/4*7;
        else
            iMovePitchX=iXpitchMaxX7;
    }
    else
    {
        if(OutArmSuck.iXStep==1)
//            iMovePitchX=TestIF.dSiteXPitch*3;
//        else if(OutArmSuck.iXStep==2)
            iMovePitchX=TestIF.dSiteXPitch/2*3;
        else
            iMovePitchX=iXpitchMaxX3;
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    //AI(ht9045-v899) 20260810: 把算好的目標座標與 mode 暫存起來, 供 DoPickFromShuttle 記錄「目標 vs 實際」
    iOutArmRoundTgtX   =iXPos;
    iOutArmRoundTgtY   =iYPos;
    iOutArmRoundMode   =iMode;
    iOutArmRoundOfsPos =iOffsetPos;
    iOutArmRoundPitchX =iMovePitchX;
    if(CheckOutArmXYPitch_2x4_4(&iXPos, &iYPos, iSht, iKit, iMovePitchX, iMovePitchY)==false && bZDown==true)
        return false;

    GetOutShuttleStatus_9045(iSht, iZPos, bZDown, bZFlag);
    if(IsCheckOutArmDestroyActiveFinish(iXPos, iYPos)==false)                   //2013-08-02    Dell     add Pick 前先確認CheckInArmDestroyActive 已完成
        return false;

    DoOutArmSuckPreOn(iSht, iKit, iXPos, iYPos, bZDown);
    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, bZDown))
        return true;

    return false;
}
//==============================================================================
// 輸出臂從 Shuttle 1 吸取 IC 流程動作
//  Ver : 2003_07
//==============================================================================
//AI(ht9045-v899) 20260810: 逐輪診斷 Log 用。把吸嘴 -> Shuttle 欄位的對應、Shuttle 上的 Item、
//  以及每支吸嘴的真空結果一次寫成一行。純讀取, 不改任何狀態
static void OutArmRoundLog_SuckMap(int iSht, int iPickKit, AnsiString sTag)
{
    if(OutArmRoundLog_Enabled()==false)
        return;
    try
    {
        TMyKitSuck *ptrSht=(iSht==0)?&FRCarryKit:&BRCarryKit;
        AnsiString sLog=sTag+" iPickKit="+IntToStr(iPickKit)+" ";
        AnsiString sOne;
        for(int i=0; i<OutArmSuck.iPickRow; i++)
        {
            for(int j=0; j<OutArmSuck.iPickCol; j++)
            {
                int iShtCol=j+iPickKit;
                sOne.sprintf("[N%d%d->Sht%d%d Sht.Item=%d Arm.Item=%d Err=%d Name=%s] ",
                             i, j, i, iShtCol,
                             ptrSht->Item[i][iShtCol], OutArmSuck.Item[i][j],
                             (int)OutArmSuck.Suck[i][j].Error,
                             OutArmSuck.Suck[i][j].sName.c_str());
                sLog+=sOne;
            }
        }
        OutArmRoundLog_Line(sLog);
        for(int i=0; i<ptrSht->iShtRow && i<MAX_ARM_Row; i++)
        {
            sLog.sprintf("SHT%d r%d Item:", iSht+1, i);
            for(int j=0; j<ptrSht->iShtCol; j++)
            {
                sOne.sprintf(" %d", ptrSht->Item[i][j]);
                sLog+=sOne;
            }
            OutArmRoundLog_Line(sLog);
        }
    }
    catch(...)
    {
    }
}
//==============================================================================
bool DoPickFromShuttle_9045_2x4_4(int iSht)
{
    int ret;
    int &Task=(iSht==0)?iPickFromShuttle1Task:iPickFromShuttle2Task;
    static int iWitchErrBin=0;
    AnsiString ErrPart="";                                                      //Steven 20110216 : 合併Alarm
    bool bHasDuplicateErr=false;                                                //Steven 20110216 : 合併Alarm
    bool bHasErr=false;                                                         //Steven 20110216 : 合併Alarm
    bool flag=false;
    int iSuckRow, iSuckCol, iShtRow, iShtCol;
    static bool bDoLeftShuttle=true;
    ptrOutSHT=(iSht==0)?&FRCarryKit:&BRCarryKit;
    int iPickKit=(iOutArmiWhichKit==0)?0:OutArmSuck.iPickKitStep;

    if(CheckTesterZ(iSht))
    {
        //MoveOutArmToAutoSafe();                                               //Steven 20150408 : Mark,避免吸嘴上上下下
        return false;
    }

    switch(Task)
    {
        case 1:
            if(ArmSpeed[OutArm].dWaitOnSH!=0)                                   //kevin 20210525 out arm 起動在shuttle 上等待 時間
            {
                if(OutArmSuck.NoIC())                                           //kevin 20210615 第一次 吸取IC 需等待
                {
                    if(MoveOutArmToShuttleIncludeZ_9045_2x4_4(iSht, iOutArmiWhichKit, false))  //移到 Shuttle 上等待
                    {
                        PickFromShuttle(iOutArmiWhichKit);                      //kevin 20210612 add int iwhichKit=0
                        Task=2;
                    }
                }
                else
                {
                    Task=10;
                }
            }
            else
            {
                Task=10;
            }
            break;
        case 2:
            if(bOutShtwaitPick())                                               //kevin 20210602 Out arm sht on wait
                Task=10;
            break;
        case 10:
            iWitchErrBin=0;
            //AI(ht9045-v899) 20260810: 一趟取料開始 -> 開新的一輪紀錄(新輪會覆蓋 ring 上最舊的一輪)
            OutArmRoundLog_Begin(iSht, iOutArmiWhichKit);
            Task=200;
        case 200:
            if(MoveOutArmToShuttleIncludeZ_9045_2x4_4(iSht, iOutArmiWhichKit, true))        //kevin 20210717 change function
            {
                //AI(ht9045-v899) 20260810: 到位後記錄目標與實際座標。kit 沒位移這類問題只要比這兩行就看得出來
                {
                    AnsiString sLog;
                    sLog.sprintf("MOVE iMode=%d iModeRow=%d iModeCol=%d iOffsetPos=%d iMovePitchX=%d "
                                 "TgtX=%d TgtY=%d ActX=%d ActY=%d",
                                 iOutArmRoundMode, iOutArmRoundMode%100, iOutArmRoundMode/100,
                                 iOutArmRoundOfsPos, iOutArmRoundPitchX,
                                 iOutArmRoundTgtX, iOutArmRoundTgtY,
                                 MOT[MOutArmX].ReadPos(), MOT[MOutArmY].ReadPos());
                    OutArmRoundLog_Line(sLog);
                }
                fAutoTeach->iATOutArmWhichKit=iOutArmiWhichKit;                 //JimmyChiu 20211020 : Auto alignment mode
                if(OutArmNeedCheckOffset(false, iSht))                          //Steven 20230531 : 簡化判斷式
                {
                    Task=500;
                    break;
                }
                OutArmSuck.ResetAll();                                          //Steven 20160323 : 避免未開啟真空
                Task=iOUTARM_SUCK;
            }
            break;
        case 500:
            if(bEnterOffset)
            {
                bEnterOffset=false;
                bOutArmManualStepPress=true;
                Task=1;
                return false;
            }
            else
            {
                if(fAutoTeach->IsRun())                                         //JimmyChiu 20211020 : Auto alignment mode
                    fAutoTeach->DoNext();
                Task=iOUTARM_SUCK;
                OutArmSuck.ResetAll();                                          //Steven 20160323 : 避免未開啟真空
            }
            break;
        case iOUTARM_SUCK:
            flag=true;
            if(iOutArmiWhichKit==1)
            {
                bDoLeftShuttle=false;
            }
            else
            {
                bDoLeftShuttle=true;
            }

            bHasErr=false;
            for(int i=0; i<OutArmSuck.iPickRow; i++)
            {
                for(int j=0; j<OutArmSuck.iPickCol; j++)
                {
                    iSuckRow  =i;
                    iSuckCol  =j;
                    iShtRow   =i;
                    iShtCol   =j+iPickKit;

                    if(ptrOutSHT->Item[iShtRow][iShtCol] &&
                       OutArmSuck.Suck[iSuckRow][iSuckCol].GetNeedSuckStatus())
                    {
                        if(OutArmSuck.Suck[iSuckRow][iSuckCol].Suck())
                        {
                            //模擬ic不見了
                            #ifdef SOFT_SIMULTE
                            if(fMain->chkInPickLoadError->Checked==true)
                            {
                                if(i==1 && j==0)
                                {
                                    OutArmSuck.Suck[1][0].Error=true;
                                    continue;
                                }
                            }
                            #endif

                            bHasErr=SwapShuttleDataToOutArm(iSht, iShtRow, iShtCol, iSuckRow, iSuckCol, &iWitchErrBin);   //Steven 20170428 : 統一Out Arm從Shuttle吸料後的資料交換
                            bOutSuckShtDupErr[iSuckRow][iSuckCol]=false;
                        }
                        else if(OutArmSuck.Suck[iSuckRow][iSuckCol].Error==false)
                        {
                            flag=false;
                        }
                    }
                }
            }

            if(bHasErr)
            {
                ShowOutputShuttleDataMiss();
            }

            if(flag==false)                                                     //KEVIN 20130421 按RETRY資料尚未交換會將其他IC丟掉
                break;

            for(int i=0; i<OutArmSuck.iPickRow; i++)
            {
                for(int j=0; j<OutArmSuck.iPickCol; j++)
                {
                    if(OutArmSuck.Suck[i][j].Error)
                    {
                        //AI(ht9045-v899) 20260810: 吸取失敗 -> 記錄每支吸嘴對到的 Shuttle 欄位與該欄位的 Item
                        OutArmRoundLog_SuckMap(iSht, iPickKit, "SUCK-FAIL");
                        iOutShtRetryCount++;
                        Task=2000;
                        return false;
                    }
                }
            }

            for(int i=0; i<OutArmSuck.iPickRow; i++)
            {
                for(int j=0; j<OutArmSuck.iPickCol; j++)
                    if(OutArmSuck.Item[i][j]==NULL_IC &&
                       OutArmSuck.Suck[i][j].GetNeedSuckStatus())
                        return false;
            }

            if(iHWFix_BinBox==1 && ErrorBinBoxDetect(iWitchErrBin)==false)
                return false;                                                   //kevin 20160906 add error bin box

            if(USE_Fix_AI_CCD==true && TestIF_File.bEnableFix2BGAAICCD==true)   //KaiChen 20190124 ：矽格湖口 AI CCD Function
            {
                fFixAICCD->OutArmCycleCounterUpdate();
            }

            //AI(ht9045-v899) 20260810: 吸取成功 -> 同樣記一份, 才能跟失敗的那一輪對照
            OutArmRoundLog_SuckMap(iSht, iPickKit, "SUCK-OK");
            iOutShtRetryCount=0;
            Task=1;
            return true;
        case 2000:
            if(MoveOutArmToAutoSafe())
            {
                bHasErr=false;
                ErrPart=" ";
                bHasDuplicateErr=false;                                         //Steven 20110216 Start : 整合Alarm
                for(int i=0; i<OutArmSuck.iPickRow; i++)
                {
                    for(int j=0; j<OutArmSuck.iPickCol; j++)
                    {
                        if(bOutSuckShtDupErr[i][j])
                            bHasDuplicateErr=true;
                        if(OutArmSuck.Suck[i][j].Error)
                        {
                            ErrPart+=OutArmSuck.Suck[i][j].sName;
                            bHasErr=true;
                        }
                        else
                        {
                            bOutSuckShtDupErr[i][j]=false;
                        }
                    }
                }

                if(bHasErr)
                {
                    if(iOutShtRetryCount>ArmSpeed[OutArm].iRetryCT)
                    {
                        if(!MoveOutArmXY_ToShuttleAlarmArea())
                            return false;

                        ret=OutArmPickShuttleAlarm(iSht, bHasDuplicateErr, ErrPart);
                        //AI(ht9045-v899) 20260810: 發 alarm 是最需要保留現場的時刻, 記錄操作員的選擇並落地一次
                        {
                            AnsiString sLog;
                            sLog.sprintf("ALARM JAM020%d ErrPart=%s ret=%d(1=RETRY 2=SKIP 4=HOME) RetryCT=%d",
                                         iSht+1, ErrPart.c_str(), ret, iOutShtRetryCount);
                            OutArmRoundLog_Line(sLog);
                            OutArmRoundLog_Flush("OutArm pick alarm");
                        }

                        for(int i=0; i<OutArmSuck.iPickRow; i++)
                        {
                            for(int j=0; j<OutArmSuck.iPickCol; j++)
                            {
                                if(OutArmSuck.Suck[i][j].Error)
                                {
                                    bOutSuckShtDupErr[i][j]=true;
                                }
                                else
                                {
                                    bOutSuckShtDupErr[i][j]=false;
                                }
                            }
                        }
                    }
                    else
                    {
                        ret=K_RETRY;
                    }

                    if(ret==K_SKIP)
                    {
                        for(int i=0; i<OutArmSuck.iPickRow; i++)
                        {
                            for(int j=0; j<OutArmSuck.iPickCol; j++)
                            {
                                if(OutArmSuck.Suck[i][j].Error)
                                {
                                    iSuckRow  =i;
                                    iSuckCol  =j;
                                    iShtRow   =i;
                                    iShtCol   =j+iPickKit;
                                    PorcessJAM0201OutArmPickUpErrorSkip(iSht, iShtRow, iShtCol, iSuckRow, iSuckCol); //Steven 20161214 : 加上OutArm吸取異常Skip的ErrorLog
                                }
                            }
                        }
                        iOutShtRetryCount=0;
                        if((bDoLeftShuttle==true  && ptrOutSHT->LeftSideNoIC(OutArmSuck.iShtKitStep)) ||
                           (bDoLeftShuttle==false && ptrOutSHT->RightSideNoIC(OutArmSuck.iShtKitStep)))        //Steven 20170327 (Jou) : 修改Out放料判斷方式
                        {
                            ZeroMemory(bOutSuckShtDupErr, sizeof(bOutSuckShtDupErr));
                            Task=1;
                            return true;
                        }
                    }
                    else                                                        //Steven 20190313 : Fixed for out arm pick error need home
                    {
                        if(iOutShtRetryCount>ArmSpeed[OutArm].iRetryCT)
                        {
                            iOutShtRetryCount=0;
                            if(IniConfig.bInOutArmCanPushHome)                  //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home
                            {
                                if(ret==K_HOME)
                                    Task=2200;
                                else
                                    Task=1;
                            }
                            else
                            {
                                if(ret==K_RETRY)
                                    Task=2200;
                                else
                                    Task=1;
                            }
                            return false;
                        }
                    }
                }
                else
                {
                    ZeroMemory(bOutSuckShtDupErr, sizeof(bOutSuckShtDupErr));
                }
                Task=1;
            }
            break;
        case 2200:
            //AI(ht9045-v899) 20260810: 標出回 HOME 的斷點。ring 在 RAM 不會被 HOME 清掉, 所以前後輪可以接著看
            OutArmRoundLog_Line("---- SetOutArmHome (round log continues across HOME) ----");
            SetOutArmHome();
            iOutShtRetryCount=0;
            Task=1;
            break;
    }
    return false;
}
//==============================================================================
void DoOutArm_9045_2x4_4()
{
    int ret, ct;
    static int OldPos=0;
    static bool FirstEnter=true, bCheckSpeed=false;
    int &Task=OutArmTask;
    static int iWhichShuttle=0;
    bool flag3;
    static bool bCheckSpeed1=false;                                             //KaiChen 20171225 (Steven)：Add Speed Display

    if(bCarryControlOutarm1 || bCarryControlOutarm2)                            //jou 2011-11-21 bCarryControlOutarm -> bCarryControlOutarm1 不應該共用一個flag
    {
        return;
    }

    switch(Task)
    {
        case 1:
            bSortingAllBinTrayFinish=false;                                     //JerryYang 20150910 是否所有的Bin Tray IC都整盤完成
            Task=5;
        case 5:
            if(MoveOutArmToAutoSafe())
                Task=10;

            if(Task!=10)                                                        //Steven 20180813 : add Out arm speed
                break;
        case 10:
            if(CheckOutArmInitState())
                Task=50;

            if(Task!=50)                                                        //Steven 20180813 : add Out arm speed
                break;
        case 50:
            FirstEnter=true;                                                    // for judge which shuttle to pick
            IsCatchTrayReadySupplyNewTray();
            if(MoveOutArmToAutoSafe())
            {
//#ifdef Carry4
//                MOT[MOutShuttle1].fCanMoveR=true;
//                MOT[MOutShuttle2].fCanMoveR=true;
//#else
                MOT[MInShuttle1].fCanMoveR=true;
                MOT[MInShuttle2].fCanMoveR=true;
//#endif
                if(OutArmSuck.HasIC())
                    Task=3000;
                else
                    Task=100;
            }

            if(Task!=100)                                                       //Steven 20180813 : add Out arm speed
                break;
        case 100:
            IsCatchTrayReadySupplyNewTray();
            if(FirstEnter)
            {
                if((FRCarryKit.UseSiteHasIC()) && OutSHT1InRT())
                {
                    OldPos=1;
                    FirstEnter=false;
                }
                else if((BRCarryKit.UseSiteHasIC()) && OutSHT2InRT())
                {
                    OldPos=0;
                    FirstEnter=false;
                }
            }
            if(FirstEnter)
            {
                Task=200;
                break;
            }

            if(FRCarryKit.UseSiteHasIC() && OutSHT1InRT())
            {
                if(OldPos==0 && BRCarryKit.UseSiteNoIC() && OutSHT2InRT()==false)
                    OldPos=1;
            }
            else if(BRCarryKit.UseSiteHasIC() && OutSHT2InRT())
            {
                if(OldPos==1 && FRCarryKit.UseSiteNoIC() && OutSHT1InRT()==false)
                    OldPos=0;
            }

            if(FRCarryKit.UseSiteHasIC() && OutSHT1InRT())
            {
                OldPos=1;
            }
            else if(BRCarryKit.UseSiteHasIC() && OutSHT2InRT())
            {
                OldPos=0;
            }

            if(TestIF.iShuttleMode==1)
            {
                if(TestIF.iShuttle_Sel==0)
                    OldPos=1;                                                   // shuttle 1
                else
                    OldPos=0;                                                   // shuttle 2
            }

            if(CheckRotateOutNotFinish())                                       //kevin 20130524 發生alarm inarm到等待點
            {
                Task=7000;
                break;
            }

            if(OldPos==1)
            {
                iWhichShuttle=0;
                Task=1000;
            }
            else
            {
                iWhichShuttle=1;
                Task=2000;
            }
            OldPos=1-OldPos;
            break;
        case 200:
            if(MoveOutArmToAutoSafe())
                Task=300;
            break;
        case 300:
            if(TestIF.iTestMode==_16Site4X4)                                    //Jou 20150909 : For Add 32Site UPH, 讓Shuttle 2先回去補Input   0-->1
            {
                if(MoveOutArmToShuttleIncludeZ_9045_2x4_4(1, false))
                {
                    Task=CheckOutArmCleanOut();
                }
            }
            else if(MoveOutArmXY_ToFix_Tray_Full())                             //Steven 20230602 : 統一out arm 讓位位置
            {
                Task=CheckOutArmCleanOut();
            }

            if(Task==5000)                                                      //Sam 20250204 : 修正 P27 整盤功能 Hangeup
            {
                DoOutArmAfterPlaceToAuto(bCheckSpeed, true);
                Task=3500;
            }
            break;
        case 310:                                                               //kevin 20171103 (wei) Outarm Z Auto High
            if(AutoTeachLoadTrayZ(false, OutArm, iOutArmZTeachTask))
            {
                MoveOutArmXY_ToFix_Tray_Full();
                Task=311;
            }
            break;
        case 311:                                                               //kevin 20171103 (wei) Outarm Z Auto High
            if(MoveOutArmXY_ToFix_Tray_Full())
            {
                bOutarmAutoHigh=false;
                Task=1;
            }
            break;
        case 1000:
            if(USE_OUT_Y_IS_AUTO_PITCH==true && IniConfig.bE57YPitchHome)              //kevin 20180827 (Steven) : add put shuttle Y pitch home AutoYPitch  //JerryYang 20251218 : IN/OUT ARM支援不同模組
            {
                Task=1010;
                return;
            }
            Task=1140;
            break;
       case 1010:
            if(MoveOutArmToAutoSafe())
            {
                hOutArmYpitchHomeTimer.SetSecAndOn(10);
                Task=1020;
            }
            break;
       case 1020:
            if(bCheckYPitchRunHomeSen(1))
            {
                Task=1140;
            }
            else if(hOutArmYpitchHomeTimer.Off())
            {
                InitProcessSingleMotorTask(MOutArmPitchY);
                hOutArmYpitchHomeTimer.SetSecAndOn(30);
                bYpitchNeddHome=true;
                Task=1030;
            }
            break;
       case 1030:
            if(bCheckYPitchHome(1))
            {
                SetMotorSpeed();
                bYpitchNeddHome=false;
                Task=1140;
            }
            else if(hOutArmYpitchHomeTimer.Off())
            {
                ShowErrorMessage("WAR0227", K_RETRY, MInArmPitchY);
                Task=1020;
            }
            break;
       case 1140:
            if(CheckOutArmToTask50(iWhichShuttle))
            {
                Task=50;
                break;
            }

            if((iCleanOut==1 && HasICUnderMachine()==false) ||
               (iCleanOut==1 && LastSet.iTemperature==Tempture_Hot &&
                                (TrayForm.bAutoFeed==false && bMustCleanAllTray==false) &&
                                HasICUnderHotPlate()==false))                   //JerryYang 20190807 fix 快速clean out hang up
            {
                FirstEnter=true;
                Task=1;
                break;
            }

            if(OutSHT1InRT() && bCheckShuttle1Flag==false)
            {
                if(FRCarryKit.UseSiteHasIC())
                {
                    if(CheckShuttleICPos(1))
                    {
                        Task=1150;
                    }
                    else
                    {
                        if(FRCarryKit.LeftSideNoIC(2)==false)
                            iOutArmiWhichKit=0;
                        else
                            iOutArmiWhichKit=1;
                        InitPickFromShuttle1Task();                             //Sam 20230208 : 修正 Fix AOI 未拍照問題
                        Task=1200;
//                        #ifdef Carry4
//                            MOT[MOutShuttle1].fCanMoveR=false;
//                        #else
                            MOT[MInShuttle1].fCanMoveR=false;
//                        #endif
                    }
                }
                else
                {
                    Task=1150;
                }
            }
            else
            {
                if(FRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep)==false)
                    iOutArmiWhichKit=0;
                else
                    iOutArmiWhichKit=1;
                InitPickFromShuttle1Task();                                     //Sam 20230208 : 修正 Fix AOI 未拍照問題
                Task=1150;
            }

            if(Task!=1150)                                                      //Steven 20180813 : add Out arm speed
                break;
        case 1150:
            if(bCheckSpeed)                                                     //Steven 20110525 : Auto Speed
            {
                bCheckSpeed=false;
                OutArmSubSpeed();
            }

            if(bCheckSpeed1)
            {
                bCheckSpeed1=false;
                OutArmSubSpeedDisplay();
            }

            if(MoveOutArmToShuttleIncludeZ_9045_2x4_4(iWhichShuttle, iOutArmiWhichKit))
            {
                IsCatchTrayReadySupplyNewTray();
                if(IniConfig.bA26MotorSpeedSortDisplay)                         //KaiChen 20171225 (Steven)：Add Speed Display
                    bCheckSpeed1=true;
                else
                    bCheckSpeed1=false;
                Task=1100;
            }

            if(Task!=1100)                                                      //Steven 20180813 : add Out arm speed
                break;
        case 1100:
            IsCatchTrayReadySupplyNewTray();

            if(WhichAutoNeedTray()!=0)                                          //JerryYang 20170731 (wei) Out arm要讓位才不會hang up
            {
                MoveOutArmXY_ToFix_Tray_Full();                                 //Steven 20231027 : 先檢查有沒有要換Tray
                Task=1160;
            }
            else if(FRCarryKit.UseSiteHasIC() && OutSHT1InRT() && bCheckShuttle1Flag==false)
            {
//#ifdef Carry4
//                MOT[MOutShuttle1].fCanMoveR=false;
//#else
                MOT[MInShuttle1].fCanMoveR=false;
//#endif
                InitPickFromShuttle1Task();
                if(FRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep))
                {
                    iOutArmiWhichKit=1;
                    Task=1200;
                }
                else
                {
                    iOutArmiWhichKit=0;
                    Task=1200;
                }
                break;
            }
            else if(BRCarryKit.UseSiteHasIC() && OutSHT2InRT() && bCheckShuttle2Flag==false) // change another shuttle
            {
                Task=50;
                break;
            }
            else if(IsOutArmCleanOutFinish())                                   //Steven 20230323 : fixed for out arm hang up
            {
                Task=50;
            }
            break;
        case 1160:
            if(MoveOutArmXY_ToFix_Tray_Full())
            {
//jou 20231106 : 修正out arm 1x1 hang up
//                if(WhichAutoNeedTray()==0)                                      //JerryYang 20170731 (wei) Out arm要讓位才不會hang up
                    Task=1100;
            }
            break;
        case 1200:
            if(bCheckSpeed)                                                     //Steven 20110525 : Auto Speed
            {
                bCheckSpeed=false;
                OutArmAddSpeed();
            }

            if(bCheckSpeed1)                                                    //KaiChen 20171225 (Steven)：Add Speed Display
            {
                bCheckSpeed1=false;
                OutArmAddSpeedDisplay();
            }

            if(DoPickFromShuttle_9045_2x4_4(0))
            {
                PreSetOutAdditionalFlag();                                      //Steven 20220614 : Fixed for out arm additional function
                FRCarryKit.SetUnuseAndHasNullICToNullIC();
                if((iOutArmiWhichKit==0 && FRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep)==false) ||
                   (iOutArmiWhichKit==1 && FRCarryKit.RightSideNoIC(OutArmSuck.iShtKitStep)==false))
                {
                    InitPickFromShuttle1Task();
                    return;
                }

                if(OutArmSuck.HasIC()==false)
                {
                    Task=50;
                }
                else
                {
                    iOutRotateFinish=0;                                         //kevin 20130524 目前放到 auto tray
//                    iOutArmPickPlaceCnt[OutOfsOutSh1]++;                      //JerryYang 20230523 : 沒在用, Mark掉  //JerryYang 20180921 Setup Teach功能
                    bPickSH1Flag=true;
                    bPickSH2Flag=false;
                    Task=3000;
                }
            }
            else
            {
                if(bOutArmManualStepPress)
                    Task=1235;
            }
            break;
        case 1235:
            bOutArmManualStepPress=false;
            if(MoveOutArmToAutoSafe())
                Task=1150;
            break;
        case 2000:
            if(USE_OUT_Y_IS_AUTO_PITCH==true && IniConfig.bE57YPitchHome)              //kevin 20180827 (Steven) : add put shuttle Y pitch home AutoYPitch  //JerryYang 20251218 : IN/OUT ARM支援不同模組
            {
                Task=2010;
                return;
            }
            Task=2040;
            break;
        case 2010:   //kevin 20180827
            if(MoveOutArmToAutoSafe())
            {
                hOutArmYpitchHomeTimer.SetSecAndOn(10);
                Task=2020;
            }
            break;
        case 2020:
            if(bCheckYPitchRunHomeSen(1))
            {
                 Task=2040;
            }
            else if(hOutArmYpitchHomeTimer.Off())
            {
                InitProcessSingleMotorTask(MOutArmPitchY);
                hOutArmYpitchHomeTimer.SetSecAndOn(30);
                bYpitchNeddHome=true;
                Task=2030;
            }
            break;
        case 2030:
            if(bCheckYPitchHome(1))
            {
                SetMotorSpeed();
                bYpitchNeddHome=false;
                 Task=2040;
            }
            else if(hOutArmYpitchHomeTimer.Off())
            {
                ShowErrorMessage("WAR0227", K_RETRY, MInArmPitchY);
                Task=2020;
            }
            break;
        case 2040:
            if(CheckOutArmToTask50(iWhichShuttle))
            {
                Task=50;
                break;
            }

            if((iCleanOut==1 && HasICUnderMachine()==false) ||
               (iCleanOut==1 && LastSet.iTemperature==Tempture_Hot &&
                               (TrayForm.bAutoFeed==false && bMustCleanAllTray==false) &&
                               HasICUnderHotPlate()==false))                    //JerryYang 20190807 fix 快速clean out hang up
            {
                FirstEnter=true;
                Task=1;
                break;
            }

            if(OutSHT2InRT() && bCheckShuttle2Flag==false)
            {
                if(BRCarryKit.UseSiteHasIC())
                {
                    if(CheckShuttleICPos(2))
                    {
                        Task=2050;
                    }
                    else
                    {
                        if(BRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep)==false)
                            iOutArmiWhichKit=0;
                        else
                            iOutArmiWhichKit=1;
                        InitPickFromShuttle2Task();                             //Sam 20230208 : 修正 Fix AOI 未拍照問題
                        Task=2200;
//                        #ifdef Carry4
//                            MOT[MOutShuttle2].fCanMoveR=false;
//                        #else
                            MOT[MInShuttle2].fCanMoveR=false;
//                        #endif
                    }
                }
                else
                {
                    Task=2050;
                }
            }
            else
            {
                if(BRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep)==false)
                    iOutArmiWhichKit=0;
                else
                    iOutArmiWhichKit=1;
                InitPickFromShuttle2Task();                                     //Sam 20230208 : 修正 Fix AOI 未拍照問題
                Task=2050;
            }

            if(Task!=2050)                                                      //Steven 20180813 : add Out arm speed
                break;
        case 2050:
            if(bCheckSpeed)                                                     //Steven 20110525 : Auto Speed
            {
                bCheckSpeed=false;
                OutArmSubSpeed();
            }

            if(bCheckSpeed1)                                                    //KaiChen 20171225 (Steven)：Add Speed Display
            {
                bCheckSpeed1=false;
                OutArmSubSpeedDisplay();
            }

            if(MoveOutArmToShuttleIncludeZ_9045_2x4_4(iWhichShuttle, iOutArmiWhichKit))
            {
                IsCatchTrayReadySupplyNewTray();

                Task=2100;
            }

            if(Task!=2100)                                                      //Steven 20180813 : add Out arm speed
                break;
        case 2100:
            IsCatchTrayReadySupplyNewTray();

            if(BRCarryKit.UseSiteHasIC() && OutSHT2InRT() && bCheckShuttle2Flag==false)
            {
//#ifdef Carry4
//                MOT[MOutShuttle2].fCanMoveR=false;
//#else
                MOT[MInShuttle2].fCanMoveR=false;
//#endif
                InitPickFromShuttle2Task();

                if(BRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep)==false)
                    iOutArmiWhichKit=0;
                else
                    iOutArmiWhichKit=1;
                InitPickFromShuttle2Task();                                     //Sam 20230208 : 修正 Fix AOI 未拍照問題
                Task=2200;
                break;
            }
            else if(FRCarryKit.UseSiteHasIC() && OutSHT1InRT() && bCheckShuttle1Flag==false)
            {
                Task=50;
                break;
            }
            break;
        case 2200:
            if(bCheckSpeed)                                                     //Steven 20110525 : Auto Speed
            {
                bCheckSpeed=false;
                OutArmAddSpeed();
            }

            if(bCheckSpeed1)                                                    //KaiChen 20171225 (Steven)：Add Speed Display
            {
                bCheckSpeed1=false;
                OutArmAddSpeedDisplay();
            }

            IsCatchTrayReadySupplyNewTray();

            if(DoPickFromShuttle_9045_2x4_4(1))
            {
                PreSetOutAdditionalFlag();                                      //Steven 20220614 : Fixed for out arm additional function
                BRCarryKit.SetUnuseAndHasNullICToNullIC();
                if((iOutArmiWhichKit==0 && BRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep)==false) ||
                   (iOutArmiWhichKit==1 && BRCarryKit.RightSideNoIC(OutArmSuck.iShtKitStep)==false))
                {
                    InitPickFromShuttle2Task();
                    return;
                }

                if(OutArmSuck.HasIC()==false)
                {
                    Task=50;
                }
                else
                {
                    iOutRotateFinish=0;                                         //kevin 20130524 目前放到 auto tray
//                    iOutArmPickPlaceCnt[OutOfsOutSh2]++;                      //JerryYang 20230523 : 沒在用, Mark掉  //JerryYang 20180921 Setup Teach功能
                    bPickSH1Flag=false;
                    bPickSH2Flag=true;
                    Task=3000;
                    bNeedCheckOurArmDeviceSuck=true;                            //jou 2010-09-07 start : 確認Out arm有確實將device吸走
                }
            }
            else
            {
                if(bOutArmManualStepPress)
                    Task=2235;
            }
            break;
        case 2235:
            bOutArmManualStepPress=false;
            if(MoveOutArmToAutoSafe())
                Task=2050;
            break;
        case 3000:
            IsCatchTrayReadySupplyNewTray();
            if(MoveOutArmToAutoSafe())
            {
                CheckOutArmSuckICFallDown();
//#ifdef Carry4
//                MOT[MOutShuttle1].fCanMoveR=true;
//                MOT[MOutShuttle2].fCanMoveR=true;
//#else
                MOT[MInShuttle1].fCanMoveR=true;
                MOT[MInShuttle2].fCanMoveR=true;
//#endif
                if(CheekNeedToDoOutArmAdditionalFunction())                     //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
                {
                    Task=7000;
                }
                else
                {
                    Task=3010;
                    AseIcRecord="@e02001";                                      //kevin 20161228 (jou) 傳送給 ASE 抬頭資料
                }
            }
            break;
        case 3010:
            IsCatchTrayReadySupplyNewTray();
            if(OutArmSuck.HasIC()==false)
            {
                Task=100;
                break;
            }
            iOutRotateFinish=3;                                                 //kevin 20130524 目前放到 auto tray
            iWhichAuto=SearchTrayToPlace_9045();                                //Steven 20160524 : 簡化Out Arm程式碼

            if(AUTO3_IS_MAGAZINE==1 &&
               iWhichAuto==2 &&
               (bMagCatchTrayfalg || bChaneMagTrayflag))                        //Ifor 20231114 add:
            {
                if(iCatchTrayControlManual>=2 || WhichAutoNeedTray()!=0)        // catch tray busy
                {
                    MoveOutArmXY_ToFix_Tray_Full();
                }
                break;
            }

            ret=VerifyFixTrayLink();
            if(ret==4000 || ret==10000)
            {
                Task=ret;
                break;
            }
            else if(ret==1)
            {
                return;
            }

            if(MOT[MTrayX].ReadPos()>Prod.iXTrayColor)
            {
                MoveOutArmXY_ToFix_Tray_Full();
                break;
            }
            InitPlaceToAutoTask();

            if(iWhichAuto<=iAutoRight)
            {
                if(MOT[iMMAuto[iWhichAuto]].fHasTray==false ||
                   MOT[iMMAuto[iWhichAuto]].FullIC())
                    break;
            }
            else
            {
                if(AUTO3_IS_MAGAZINE==1             &&                          //JerryYang 20221215 : Magazine把fix區當buffer區功能
                   TestIF_File.iMagFixTrayType==1   &&
                   iWhichAuto>=iMagMin)
                {

                }
                else
                {
                    SetFixTrayFullIC();
                    if(MOT[iMMAuto[iWhichAuto]].fHasTray==false ||
                       MOT[iMMAuto[iWhichAuto]].FullIC())
                    {
                        if(iHWFix_BinBox==1 && iWhichAuto==eBulkBox)
                            DoFixTrayFullAlarm();                               //kevin 20201207 check Error bin box Fix 1 put tray
                        break;
                    }
                }
            }

            if(iCatchTrayControlManual>=2 || WhichAutoNeedTray()!=0)            //Sam 20180822 : OutArm 是否在安全位置，是否讓位，不然會與 TrayArm 補 Tray 動作互卡 Hange up。
            {
                MoveOutArmXY_ToFix_Tray_Full();
                break;
            }
            Task=3100;
            break;
        case 3020:                                                              //jou 2010-01-25 start : one cycle後，可以將tray盤拉出來
            IsCatchTrayReadySupplyNewTray();
            if(MoveOutArmXY_ToFix_Tray_Full())
            {
                if(MOT[iMMAuto[iWhichAuto]].fHasTray==true)
                    Task=3100;
            }
            break;
        case 3050:
            if(AutoTrayReCheck(iWhichAuto))                                     //Ifor 20171031 add Auto Tray ReCheck
            {
                Task=3100;
            }
            break;
        case 3100:
            if(iWhichAuto<=iAutoRight)
            {
                Task=VerifyTrayStatus();
                if(Task!=3300)
                    break;
            }
            Task=3300;                                                          //kevin 20151203 沒有fixtray放ic
        case 3300:
            if(AUTO3_IS_MAGAZINE==1             &&                              //JerryYang 20221215 : Magazine把fix區當buffer區功能
               TestIF_File.iMagFixTrayType==1   &&
               iWhichAuto>=iMagMin              &&
               iWhichBuff>=iFixMin              &&
               iWhichBuff<=iFixMax)
            {
                ct=iAutoIndex[iWhichBuff];
                if(iWhichBuff>=iFixMin &&
                   Sen[SnFixedTrayDetect[ct]].IsOff() &&
                   LastSet.iRealDummy!=DUMMY)                                   //wei 20151217 add iWhichAuto>=3避免沒要放FIX盤也偵測有沒有FIX盤
                {
                    Task=4000;
                    break;
                }
            }
            else
            {
                ct=iAutoIndex[iWhichAuto];
                if(iWhichAuto>=iAutoCnt &&
                   Sen[SnFixedTrayDetect[ct]].IsOff() &&
                   LastSet.iRealDummy!=DUMMY)                                   //wei 20151217 add iWhichAuto>=3避免沒要放FIX盤也偵測有沒有FIX盤
                {
                    Task=4000;
                    break;
                }
            }
            Task=3301;
            if(Task!=3301)                                                      //Steven 20180813 : add Out arm speed
                break;
        case 3301:
            if(DoOutArmPlaceToAuto_9045())                                      // 吸嘴可能還在下方
            {
                DoOutArmAfterPlaceToAuto(bCheckSpeed, true);                    //Steven 20220526 : 針對放下IC到Unloader後的動作做整合
                Task=3500;
            }
            else
            {
                break;
            }
        case 3500:
            ret=DoOutArmAfterPlaceToAuto(bCheckSpeed);                          //Steven 20220526 : 針對放下IC到Unloader後的動作做整合
            if(ret!=0)
            {
                Task=ret;
                if(Task==100)
                {
                    if(iWhichShuttle==0)
                    {
                        if(iOutArmiWhichKit==0 && FRCarryKit.RightSideNoIC(OutArmSuck.iShtKitStep)==false)
                        {
                            Task=1000;
                            return;
                        }
                    }
                    else
                    {
                        if(iOutArmiWhichKit==0 && BRCarryKit.RightSideNoIC(OutArmSuck.iShtKitStep)==false)
                        {
                            Task=2000;
                            return;
                        }
                    }
                }
            }
            break;
        case 4000:
            if(MoveOutArmXY_ToFix_Tray_Full(true))                              //Ifor 20250926 add:Fix Tray 取放時移開Out Arm
                Task=4100;
            break;
        case 4100:
            if(DoFixTrayFullAlarm())                                            //Steven 20181113 : 修正Fix Link Alarm的問題
            {
                Task=3010;
            }
            break;
        case 7000:
            flag3=DoOutArmAdditionalFunction();                                 //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
            if(flag3)
            {
                AseIcRecord="@e02001";                                          //kevin 20161228 (jou) 傳送給 ASE 抬頭資料
                Task=3010;
            }
            break;
        case 10000:                                                             //JerryYang 20220909 : add magazine
            if(CheckPlaceToMagazineTray(iWhichMag))
            {
                Task=12000;                                                     // To Auto 3
            }
            break;
        case 11100:
            if(DoPickFromMagazineBuffer())
            {
                iWhichAuto=SearchTrayToPlace_9045();
                iOutArmWhichAuto=iMagAtAuto;
                if(iAuto3MagazineIndex!=iWhichMag && iWhichMag!=-1)
                {
                    bMagNeedChangeTray=true;
                }
                Task=12000;
            }
            break;
        case 12000:
            IsCatchTrayReadySupplyNewTray();
            if(iAuto3MagazineIndex!=iWhichMag ||
               MOT[iMMAuto[iMagAtAuto]].fHasTray==false ||
               MOT[iMMAuto[iMagAtAuto]].FullIC())
            {
                MoveOutArmXY_ToFix_Tray_Full();                                 // change Magazine Tray
                break;
            }

            bMagNeedChangeTray=false;
            if(bMagCatchTrayfalg)                                               //JerryYang 20221121 : AUTOI3 Magazine還在夾TRAY的旗標
            {
                break;
            }

            if(MOT[MTrayX].ReadPos()>Prod.iXTrayColor)
            {
                MoveOutArmXY_ToFix_Tray_Full();
                break;
            }

            InitPlaceToAutoTask();
            if(iCatchTrayControlManual>=2)                                      // catch tray busy
            {
                MoveOutArmXY_ToFix_Tray_Full();
                break;
            }
            Task=3100;
            break;
    }
}
//---------------------------------------------------------------------------
