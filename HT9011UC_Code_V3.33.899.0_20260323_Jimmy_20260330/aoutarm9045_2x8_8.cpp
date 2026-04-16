#include "MachineDefine.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include "aoutarm9045_2x8_8.h"

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

int XPHSuckToSht_2x8_8_OutArm[e2x8ModeTotal][2][8]={                            //JerryYang 20250711 : fix for 2x8特殊關SITE
                                             {{0, 1, 2, 3, 4, 5, 6, 7},         //一般模式, XStandart
                                              {0, 1, 2, 3, 4, 5, 6, 7}},
                                             {{0, 2, 1, 3, 4, 6, 5, 7},         //一般模式, 倆倆放
                                              {0, 2, 1, 3, 4, 6, 5, 7}},
                                             {{0, 1, 2, 3, 4, 5, 6, 7},         //一般模式, 單隻放
                                              {0, 1, 2, 3, 4, 5, 6, 7}},
                                             {{5, 1, 7, 3, 5, 1, 7, 3},         //JerryYang 20190729 STM 8 site交錯模式 //KevinC 20251031 : 改順序
                                              {0, 4, 2, 6, 0, 4, 2, 6}},
                                             {{0, 4, 2, 6, 0, 4, 2, 6},         //Steven 20220816 : Add for TW153TK spec
                                              {5, 1, 7, 3, 5, 1, 7, 3}},
                                             {{0, 4, 2, 6, 1, 3, 5, 7},         //跳格放    //Steven 20240416 : Add for 2x8關偶數列
                                              {0, 4, 2, 6, 1, 3, 5, 7}},
                                             {{0, 2, 4, 6, 1, 3, 5, 7},         //單顆放
                                              {0, 2, 4, 6, 1, 3, 5, 7}},
                                             {{1, 5, 3, 7, 0, 2, 4, 6},         //跳格放    //Steven 20240416 : Add for 2x8關奇數列
                                              {1, 5, 3, 7, 0, 2, 4, 6}},
                                             {{1, 3, 5, 7, 0, 2, 4, 6},         //單顆放
                                              {1, 3, 5, 7, 0, 2, 4, 6}},
                                             {{0, 1, 2, 3, 4, 5, 6, 7},         //跳格放跑2x2_13
                                              {0, 1, 2, 3, 4, 5, 6, 7}},
                                             {{0, 1, 3, 2, 4, 5, 6, 7},         //跳格放跑2x2_14
                                              {0, 1, 3, 2, 4, 5, 6, 7}},
                                             {{0, 1, 2, 3, 4, 5, 6, 7},         //四顆一起放
                                              {0, 1, 2, 3, 4, 5, 6, 7}},
                                             {{0, 1, 3, 2, 4, 5, 6, 7},         //四顆跳格放
                                              {0, 1, 3, 2, 4, 5, 6, 7}}
                                            };

//==============================================================================
//個位數為Y座標, 2=標準, 0=A排, 1=B排
//百位數以上為X座標, 分配方式如下, 千位數為Offset左右之分
//                    0                     =標準
//         100        |         110         =左右分兩次
//    200       201   |    210       211    =左右分四次
//STM Mode : 900      | TW153TK : 905
//==============================================================================
int GetNowShuttleMode_2x8_8(int iSht, int iKit)
{
    ptrOutSHT=(iSht==0)?&FRCarryKit:&BRCarryKit;

    if(ptrOutSHT->UseSiteNoIC())
    {
        if(iCloseSiteModeFor2x8==e2x8_STMMode)                                  //JerryYang 20191122 STM 8 site Auto clean支援前後排一起丟
        {
            return 30002;
        }
        else if(iCloseSiteModeFor2x8==e2x8_TW153Mode)                           //Steven 20220816 : Add for TW153TK spec
        {
            return 40002;
        }
        else if(iCloseSiteModeFor2x8==e2x8CloseEven)                            //Steven 20240416 : Add for 2x8關偶數列
        {
            return 50002;
        }
        else if(iCloseSiteModeFor2x8==e2x8CloseEven1By1)
        {
            return 60002;
        }
        else if(iCloseSiteModeFor2x8==e2x8CloseOdd)                             //Steven 20240416 : Add for 2x8關奇數列
        {
            return 70002;
        }
        else if(iCloseSiteModeFor2x8==e2x8CloseOdd1By1)
        {
            return 80002;
        }
        else if(iCloseSiteModeFor2x8==e2x8Run2x2_13)
        {
            return 90002;
        }
        else if(iCloseSiteModeFor2x8==e2x8Run2x2_14)
        {
            return 100002;
        }
        else if(iCloseSiteModeFor2x8==e2x8Run2x4Standard)
        {
            return 110002;
        }
        else if(iCloseSiteModeFor2x8==e2x8Run2x4Step2)
        {
            return 120002;
        }
        else if(OutArmSuck.iXStep==1)
        {
            return 0;
        }
        else if(OutArmSuck.iXStep==2)
        {
            if(OutArmSuck.iYStep==1)
                return 10002;                                                   //左邊八顆
            else
                return 10000;                                                   //左上四顆
        }
        else //if(OutArmSuck.iXStep==4)
        {
            if(OutArmSuck.iYStep==1)
                return 20002;                                                   //左邊四顆
            else
                return 20000;                                                   //左上兩顆
        }
    }
    else
    {
        if(ptrOutSHT->LeftSideNoIC(OutArmSuck.iShtKitStep))
            iKit=OutArmSuck.iShtKitStep;
        else
            iKit=0;

        if(iCloseSiteModeFor2x8==e2x8_STMMode)                                  //JerryYang 20191122 STM 8 site Auto clean支援前後排一起丟
        {
            if(ptrOutSHT->Item[0][1]!=NULL_IC || ptrOutSHT->Item[0][3]!=NULL_IC ||
               ptrOutSHT->Item[1][0]!=NULL_IC || ptrOutSHT->Item[1][2]!=NULL_IC)
                return  30002;
            else if(ptrOutSHT->Item[0][5]!=NULL_IC ||
                    ptrOutSHT->Item[0][7]!=NULL_IC)
                return  31000;
            else
                return  31101;
        }
        else if(iCloseSiteModeFor2x8==e2x8_TW153Mode)                           //Steven 20220816 : Add for TW153TK spec
        {
            if(ptrOutSHT->Item[0][0]!=NULL_IC || ptrOutSHT->Item[0][2]!=NULL_IC ||
               ptrOutSHT->Item[1][1]!=NULL_IC || ptrOutSHT->Item[1][3]!=NULL_IC)
                return  40002;
            else if(ptrOutSHT->Item[0][4]!=NULL_IC ||
                    ptrOutSHT->Item[0][6]!=NULL_IC)
                return  41000;
            else
                return  41101;
        }
        else if(iCloseSiteModeFor2x8==e2x8CloseEven)                            //Steven 20240416 : Add for 2x8關偶數列
        {
            if(OutArmSuck.iYStep==1)
            {
                if(ptrOutSHT->Item[0][0]>=HAS_IC || ptrOutSHT->Item[0][2]>=HAS_IC ||
                   ptrOutSHT->Item[1][0]>=HAS_IC || ptrOutSHT->Item[1][2]>=HAS_IC)
                    return 50002;
                else
                    return 51002;
            }
            else
            {
                if(ptrOutSHT->Item[0][0]>=HAS_IC || ptrOutSHT->Item[0][2]>=HAS_IC)
                    return 50000;
                else if(ptrOutSHT->Item[1][0]>=HAS_IC || ptrOutSHT->Item[1][2]>=HAS_IC)
                    return 50001;
                else if(ptrOutSHT->Item[0][4]>=HAS_IC || ptrOutSHT->Item[0][4]>=HAS_IC)
                    return 51000;
                else //if(ptrOutSHT->Item[1][6]>=HAS_IC || ptrOutSHT->Item[1][6]>=HAS_IC)
                    return 51001;
            }
        }
        else if(iCloseSiteModeFor2x8==e2x8CloseEven1By1)
        {
            if(OutArmSuck.iYStep==1)
            {
                if(ptrOutSHT->Item[0][0]>=HAS_IC ||
                   ptrOutSHT->Item[1][0]>=HAS_IC)
                    return 60002;
                else if(ptrOutSHT->Item[0][2]>=HAS_IC ||
                        ptrOutSHT->Item[1][2]>=HAS_IC)
                    return 60102;
                else if(ptrOutSHT->Item[0][4]>=HAS_IC ||
                        ptrOutSHT->Item[1][4]>=HAS_IC)
                    return 61002;
                else //if(ptrOutSHT->Item[0][5]>=HAS_IC ||
                     //   ptrOutSHT->Item[1][5]>=HAS_IC)
                    return 61102;
            }
            else
            {
                if(ptrOutSHT->Item[0][0]>=HAS_IC)
                    return 60000;
                else if(ptrOutSHT->Item[1][0]>=HAS_IC)
                    return 60001;
                else if(ptrOutSHT->Item[0][2]>=HAS_IC)
                    return 60100;
                else if(ptrOutSHT->Item[1][2]>=HAS_IC)
                    return 60101;
                else if(ptrOutSHT->Item[0][4]>=HAS_IC)
                    return 61000;
                else if(ptrOutSHT->Item[0][4]>=HAS_IC)
                    return 61001;
                else if(ptrOutSHT->Item[1][6]>=HAS_IC)
                    return 61100;
                else //if(ptrOutSHT->Item[1][6]>=HAS_IC)
                    return 61101;
            }
        }
        else if(iCloseSiteModeFor2x8==e2x8CloseOdd)                             //Steven 20240416 : Add for 2x8關奇數列
        {
            if(OutArmSuck.iYStep==1)
            {
                if(ptrOutSHT->Item[0][1]>=HAS_IC || ptrOutSHT->Item[0][3]>=HAS_IC ||
                   ptrOutSHT->Item[1][1]>=HAS_IC || ptrOutSHT->Item[1][3]>=HAS_IC)
                    return 70002;
                else
                    return 71002;
            }
            else
            {
                if(ptrOutSHT->Item[0][1]>=HAS_IC || ptrOutSHT->Item[0][3]>=HAS_IC)
                    return 70000;
                else if(ptrOutSHT->Item[1][1]>=HAS_IC || ptrOutSHT->Item[1][3]>=HAS_IC)
                    return 70001;
                else if(ptrOutSHT->Item[0][5]>=HAS_IC || ptrOutSHT->Item[0][7]>=HAS_IC)
                    return 71000;
                else //if(ptrOutSHT->Item[1][5]>=HAS_IC || ptrOutSHT->Item[1][7]>=HAS_IC)
                    return 71001;
            }
        }
        else if(iCloseSiteModeFor2x8==e2x8CloseOdd1By1)
        {
            if(OutArmSuck.iYStep==1)
            {
                if(ptrOutSHT->Item[0][1]>=HAS_IC ||
                   ptrOutSHT->Item[1][1]>=HAS_IC)
                    return 80002;
                else if(ptrOutSHT->Item[0][3]>=HAS_IC ||
                        ptrOutSHT->Item[1][3]>=HAS_IC)
                    return 80102;
                else if(ptrOutSHT->Item[0][5]>=HAS_IC ||
                        ptrOutSHT->Item[1][5]>=HAS_IC)
                    return 81002;
                else //if(ptrOutSHT->Item[0][7]>=HAS_IC ||
                     //   ptrOutSHT->Item[1][7]>=HAS_IC)
                    return 81102;
            }
            else
            {
                if(ptrOutSHT->Item[0][1]>=HAS_IC)
                    return 80000;
                else if(ptrOutSHT->Item[1][1]>=HAS_IC)
                    return 80001;
                else if(ptrOutSHT->Item[0][3]>=HAS_IC)
                    return 80100;
                else if(ptrOutSHT->Item[1][3]>=HAS_IC)
                    return 80101;
                else if(ptrOutSHT->Item[0][5]>=HAS_IC)
                    return 81000;
                else if(ptrOutSHT->Item[0][5]>=HAS_IC)
                    return 81001;
                else if(ptrOutSHT->Item[1][7]>=HAS_IC)
                    return 81100;
                else //if(ptrOutSHT->Item[1][7]>=HAS_IC)
                    return 81101;
            }
        }
        else if(iCloseSiteModeFor2x8==e2x8Run2x2_13)
        {
            if(OutArmSuck.iYStep==1)
            {
                return 90002;
            }
            else
            {
                if(ptrOutSHT->Item[0][0+iCloseSiteStep_2x8]>=HAS_IC || ptrOutSHT->Item[0][2+iCloseSiteStep_2x8]>=HAS_IC)
                    return 90000;
                else
                    return 90001;
            }
        }
        else if(iCloseSiteModeFor2x8==e2x8Run2x2_14)
        {
            if(OutArmSuck.iYStep==1)
            {
                return 100002;
            }
            else
            {
                if(ptrOutSHT->Item[0][0+iCloseSiteStep_2x8]>=HAS_IC || ptrOutSHT->Item[0][3+iCloseSiteStep_2x8]>=HAS_IC)
                    return 100000;
                else
                    return 100001;
            }
        }
        else if(iCloseSiteModeFor2x8==e2x8Run2x4Standard)
        {
            if(OutArmSuck.iYStep==1)
            {
                return 110002;
            }
            else
            {
                if(ptrOutSHT->Item[0][0+iCloseSiteStep_2x8]>=HAS_IC || ptrOutSHT->Item[0][1+iCloseSiteStep_2x8]>=HAS_IC || ptrOutSHT->Item[0][2+iCloseSiteStep_2x8]>=HAS_IC || ptrOutSHT->Item[0][3+iCloseSiteStep_2x8]>=HAS_IC)
                    return 110000;
                else
                    return 110001;
            }
        }
        else if(iCloseSiteModeFor2x8==e2x8Run2x4Step2)
        {
            if(OutArmSuck.iYStep==1)
            {
                if(ptrOutSHT->Item[0][0+iCloseSiteStep_2x8]>=HAS_IC || ptrOutSHT->Item[0][2+iCloseSiteStep_2x8]>=HAS_IC ||
                   ptrOutSHT->Item[1][0+iCloseSiteStep_2x8]>=HAS_IC || ptrOutSHT->Item[1][2+iCloseSiteStep_2x8]>=HAS_IC)
                    return 120002;
                else
                    return 120102;
            }
            else
            {
                if(ptrOutSHT->Item[0][0+iCloseSiteStep_2x8]>=HAS_IC || ptrOutSHT->Item[0][2+iCloseSiteStep_2x8]>=HAS_IC)
                    return 120000;
                else if(ptrOutSHT->Item[1][0+iCloseSiteStep_2x8]>=HAS_IC || ptrOutSHT->Item[1][2+iCloseSiteStep_2x8]>=HAS_IC)
                    return 120001;
                else if(ptrOutSHT->Item[0][1+iCloseSiteStep_2x8]>=HAS_IC || ptrOutSHT->Item[0][3+iCloseSiteStep_2x8]>=HAS_IC)
                    return 120100;
                else if(ptrOutSHT->Item[1][1+iCloseSiteStep_2x8]>=HAS_IC || ptrOutSHT->Item[1][3+iCloseSiteStep_2x8]>=HAS_IC)
                    return 120101;
            }
        }
        else if(OutArmSuck.iXStep==1)
        {
            if(OutArmSuck.iYStep==1)
            {
                return 2;
            }
            else
            {
                for(int j=0; j<OutArmSuck.iPickCol; j++)
                {
                    if(ptrOutSHT->Item[0][j+iKit]>=HAS_IC)
                        return 0;                                               //A排四顆
                }

                return 1;                                                       //B排四顆
            }
        }
        else if(OutArmSuck.iXStep==2)
        {
            if(OutArmSuck.iYStep==1)
            {
                if(ptrOutSHT->Item[0][0+iKit]>=HAS_IC || ptrOutSHT->Item[0][1+iKit]>=HAS_IC ||
                   ptrOutSHT->Item[1][0+iKit]>=HAS_IC || ptrOutSHT->Item[1][1+iKit]>=HAS_IC)
                    return 10002;
                else
                    return 10102;
            }
            else
            {
                if(ptrOutSHT->Item[0][0+iKit]>=HAS_IC || ptrOutSHT->Item[0][1+iKit]>=HAS_IC)
                    return 10000;
                else if(ptrOutSHT->Item[1][0+iKit]>=HAS_IC || ptrOutSHT->Item[1][1+iKit]>=HAS_IC)
                    return 10001;
                else if(ptrOutSHT->Item[0][2+iKit]>=HAS_IC || ptrOutSHT->Item[0][3+iKit]>=HAS_IC)
                    return 10100;
                else //if(ptrOutSHT->Item[1][3+iKit]>=HAS_IC || ptrOutSHT->Item[1][3+iKit]>=HAS_IC)
                    return 10101;
            }
        }
        else if(OutArmSuck.iXStep==4)
        {
            if(OutArmSuck.iYStep==1)
            {
                if(ptrOutSHT->Item[0][0+iKit]>=HAS_IC ||
                   ptrOutSHT->Item[1][0+iKit]>=HAS_IC)
                    return 20002;
                else if(ptrOutSHT->Item[0][1+iKit]>=HAS_IC ||
                        ptrOutSHT->Item[1][1+iKit]>=HAS_IC)
                    return 20102;
                else if(ptrOutSHT->Item[0][2+iKit]>=HAS_IC ||
                        ptrOutSHT->Item[1][2+iKit]>=HAS_IC)
                    return 20202;
                else //if(ptrOutSHT->Item[0][3+iKit]>=HAS_IC ||
                     //   ptrOutSHT->Item[1][3+iKit]>=HAS_IC)
                    return 20302;
            }
            else
            {
                if(ptrOutSHT->Item[0][0+iKit]>=HAS_IC)
                    return 20000;
                else if(ptrOutSHT->Item[1][0+iKit]>=HAS_IC)
                    return 20001;
                else if(ptrOutSHT->Item[0][1+iKit]>=HAS_IC)
                    return 20100;
                else if(ptrOutSHT->Item[1][1+iKit]>=HAS_IC)
                    return 20101;
                else if(ptrOutSHT->Item[0][2+iKit]>=HAS_IC)
                    return 20200;
                else if(ptrOutSHT->Item[0][2+iKit]>=HAS_IC)
                    return 20201;
                else if(ptrOutSHT->Item[1][3+iKit]>=HAS_IC)
                    return 20300;
                else //if(ptrOutSHT->Item[1][3+iKit]>=HAS_IC)
                    return 20301;
            }
        }
    }
    return 2;
}
//==============================================================================
bool OutArmZNeedDown_2x8_8(int iSht, int iKit)
{
    if(InitialOutArmNeedSuck(iSht)==false)
        return false;

    ptrOutSHT=(iSht==0)?&FRCarryKit:&BRCarryKit;
    int iMode=GetNowShuttleMode_2x8_8(iSht, iKit);
    int iModeRow=iMode%100;
    int iModeCol=iMode/100;
    int iPickKit=0;
    int iShtJ=0;
    if(iCloseSiteModeFor2x8<=e2x8OneByOne &&
       ptrOutSHT->LeftSideNoIC(OutArmSuck.iShtKitStep))
        iPickKit=OutArmSuck.iPickKitStep;
    else
        iPickKit=0;

    if(iCloseSiteModeFor2x8<=e2x8OneByOne)
    {
        for(int i=0; i<OutArmSuck.iPickRow; i++)
        {
            for(int j=0; j<OutArmSuck.iPickCol; j++)
            {
                iShtJ=GetShuttleCol(i, j+iPickKit);

                if(iModeCol==0 &&
                   (iModeRow==2 || iModeRow==i))                                //Steven 20240205 : 修正 Out arm 放料
                {
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==100 &&
                        (j==0 || j==2) &&
                        (iModeRow==2 || iModeRow==i))
                {
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==101 &&
                        (j==1 || j==3) &&
                        (iModeRow==2 || iModeRow==i))
                {
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==200 &&
                        j==0 &&
                        (iModeRow==2 || iModeRow==i))
                {
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==201 &&
                        j==1 &&
                        (iModeRow==2 || iModeRow==i))
                {
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==202 &&
                        j==2 &&
                        (iModeRow==2 || iModeRow==i))
                {
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==203 &&
                        j==3 &&
                        (iModeRow==2 || iModeRow==i))
                {
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x8==e2x8_STMMode)                                 //JerryYang 20191122 STM 8 site Auto clean支援前後排一起丟
    {
        if(iModeCol==300)
        {                                                                       //吸嘴       ==>  蝦頭
            SetOutArmNeedDestory(0, 1, 0, 1);                                   // X O X O        X O X O X X X X
            SetOutArmNeedDestory(0, 3, 0, 3);                                   // O X O X        O X O X X X X X
            SetOutArmNeedDestory(1, 0, 1, 0);
            SetOutArmNeedDestory(1, 2, 1, 2);
        }
        else if(iModeCol==310)
        {                                                                       //吸嘴       ==>  蝦頭
            SetOutArmNeedDestory(0, 0, 0, 5);                                   // O X O X        X X X X X O X O
            SetOutArmNeedDestory(0, 2, 0, 7);                                   // X X X X        X X X X X X X X
        }
        else
        {                                                                       //吸嘴       ==>  蝦頭
            SetOutArmNeedDestory(1, 1, 1, 4);                                   // X X X X        X X X X X X X X
            SetOutArmNeedDestory(1, 3, 1, 6);                                   // X O X O        X X X X O X O X
        }
    }
    else if(iCloseSiteModeFor2x8==e2x8_TW153Mode)                               //Steven 20220816 : Add for TW153TK spec
    {
        if(iModeCol==400)
        {                                                                       //吸嘴       ==>  蝦頭
            SetOutArmNeedDestory(0, 0, 0, 0);                                   // O X O X        O X O X X X X X
            SetOutArmNeedDestory(0, 2, 0, 2);                                   // X O X O        X O X O X X X X
            SetOutArmNeedDestory(1, 1, 1, 1);
            SetOutArmNeedDestory(1, 3, 1, 3);
        }
        else if(iModeCol==410)
        {                                                                       //吸嘴       ==>  蝦頭
            SetOutArmNeedDestory(0, 1, 0, 4);                                   // X O X O        X X X X O X O X
            SetOutArmNeedDestory(0, 3, 0, 6);                                   // X X X X        X X X X X X X X
        }
        else
        {                                                                       //吸嘴       ==>  蝦頭
            SetOutArmNeedDestory(1, 0, 1, 5);                                   // X X X X        X X X X X X X X
            SetOutArmNeedDestory(1, 2, 1, 7);                                   // O X O X        X X X X X O X O
        }
    }
    else
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                iShtJ=GetShuttleCol(i, j);

                if(iModeCol==500 && (j==0 || j==2) &&                           //吸嘴    ==>  蝦頭
                       (iModeRow==2 || iModeRow==i))                            //O X O X      O X O X X X X X
                {                                                               //O X O X      O X O X X X X X
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==510 && (j==1 || j==3) &&                      //吸嘴    ==>  蝦頭
                       (iModeRow==2 || iModeRow==i))                            //X O X O      X X X X O X O X
                {                                                               //X O X O      X X X X O X O X
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==600 && j==0 &&                                //吸嘴    ==>  蝦頭
                       (iModeRow==2 || iModeRow==i))                            //O X X X      O X X X X X X X
                {                                                               //O X X X      O X X X X X X X
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==601 && j==1 &&                                //吸嘴    ==>  蝦頭
                       (iModeRow==2 || iModeRow==i))                            //X O X X      X X O X X X X X
                {                                                               //X O X X      X X O X X X X X
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==610 && j==2 &&                                //吸嘴    ==>  蝦頭
                       (iModeRow==2 || iModeRow==i))                            //X X O X      X X X X O X X X
                {                                                               //X X O X      X X X X O X X X
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==611 && j==3 &&                                //吸嘴    ==>  蝦頭
                       (iModeRow==2 || iModeRow==i))                            //X X X O      X X X X X X O X
                {                                                               //X X X O      X X X X X X O X
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==700 && (j==0 || j==2) &&                      //吸嘴    ==>  蝦頭
                       (iModeRow==2 || iModeRow==i))                            //O X O X      X O X O X X X X
                {                                                               //O X O X      X O X O X X X X
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==710 && (j==1 || j==3) &&                      //吸嘴    ==>  蝦頭
                       (iModeRow==2 || iModeRow==i))                            //X O X O      X X X X X O X O
                {                                                               //X O X O      X X X X X O X O
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==800 && j==0 &&                                //吸嘴    ==>  蝦頭
                       (iModeRow==2 || iModeRow==i))                            //O X X X      X O X X X X X X
                {                                                               //O X X X      X O X X X X X X
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==801 && j==1 &&                                //吸嘴    ==>  蝦頭
                       (iModeRow==2 || iModeRow==i))                            //X O X X      X X X O X X X X
                {                                                               //X O X X      X X X O X X X X
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==810 && j==2 &&                                //吸嘴    ==>  蝦頭
                       (iModeRow==2 || iModeRow==i))                            //X X O X      X X X X X O X X
                {                                                               //X X O X      X X X X X O X X
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==811 && j==3 &&                                //吸嘴    ==>  蝦頭
                       (iModeRow==2 || iModeRow==i))                            //X X X O      X X X X X X X O
                {                                                               //X X X O      X X X X X X X O
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==900 && (j==0 || j==2) &&                      //吸嘴
                       (iModeRow==2 || iModeRow==i))                            //O X O X
                {                                                               //O X O X
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==1000 && (j==0 || j==3) &&                     //吸嘴
                       (iModeRow==2 || iModeRow==i))                            //O X X O
                {                                                               //O X X O
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==1100 && (j==0 || j==1 || j==2 || j==3) &&     //吸嘴
                       (iModeRow==2 || iModeRow==i))                            //O O O O
                {                                                               //O O O O
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==1200 && (j==0 || j==2) &&                     //吸嘴
                       (iModeRow==2 || iModeRow==i))                            //O X O X
                {                                                               //O X O X
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
                else if(iModeCol==1201 && (j==1 || j==3) &&                     //吸嘴
                       (iModeRow==2 || iModeRow==i))                            //X O X O
                {                                                               //X O X O
                    SetOutArmNeedDestory(i, j, i, iShtJ);
                }
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool CheckOutArmXYPitch_2x8_8(int *iX, int *iY, int iSht, int iKit, int iMovePitchX, int iMovePitchY)
{
    int iMode=GetNowShuttleMode_2x8_8(iSht, iKit);
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
                if(iSht==0)                                                     //20260302
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

    if(USE_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                                 //Ztex 2023.12.06 Add HT-1032   //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        dMovePitchX=double(iMovePitchX)/7.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*4.0-TestIF.dSiteXPitch*4.5);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*4.0-TestIF.dSiteXPitch*4.5);
        }
        else if(iModeCol==101)                                                  //Ac --> Ac
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*2.5);
        }
        else if(iModeCol==200)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*4.0-TestIF.dSiteXPitch*4.5);
        }
        else if(iModeCol==201)                                                  //Ac --> Ab
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==202)                                                  //Ae --> Ac
        {
            *iX=*iX+(-dMovePitchX*0.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==203)                                                  //Ag --> Ad
        {
            *iX=*iX+(-dMovePitchX*2.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==300)                                                  //Ba --> Ba
        {
            *iX=*iX+(dMovePitchX*4.0-TestIF.dSiteXPitch*4.5);
        }
        else if(iModeCol==301)                                                  //Aa --> Af
        {
            *iX=*iX+(dMovePitchX*4.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==310)                                                  //Bc --> Be
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==400)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*4.0-TestIF.dSiteXPitch*4.5);
        }
        else if(iModeCol==401)                                                  //Ac --> Be
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==410)                                                  //Ba --> Bf
        {
            *iX=*iX+(dMovePitchX*4.0+TestIF.dSiteXPitch*0.5);
        }

    }
    else if(USE_PICKER_COUNT==ep16Picker)                                       //基準為第四隻吸嘴
    {
        dMovePitchX=double(iMovePitchX)/7.0;
        if(iModeCol==0)                                                         //Aa --> Aa
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
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*2.5);
        }
        else if(iModeCol==202)                                                  //Ae --> Ac
        {
            *iX=*iX+(-dMovePitchX*1.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==203)                                                  //Ag --> Ad
        {
            *iX=*iX+(-dMovePitchX*3.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==900)                                                  //Ba --> Ba
        {
            *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==901)                                                  //Aa --> Af
        {
            *iX=*iX+(dMovePitchX*3.0+TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==902)                                                  //Bc --> Be
        {
            *iX=*iX+(dMovePitchX*3.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==905)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==906)                                                  //Ac --> Be
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==907)                                                  //Ba --> Bf
        {
            *iX=*iX+(dMovePitchX*3.0+TestIF.dSiteXPitch*1.5);
        }
    }
    else if(USE_OUT_ARM_Y_PITCH==iXYPitchVariable)                           //基準為第二隻吸嘴  //JerryYang 20251218 : IN/OUT ARM支援不同模組
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
            *iX=*iX+(-dMovePitchX*2-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==300)                                                  //Ba --> Ba
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==310)                                                  //Aa --> Af
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==311)                                                  //Bb --> Be
        {
            *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==400)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==410)                                                  //Ab --> Ae
        {
            *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==411)                                                  //Ba --> Bf
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==500)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==510)                                                  //Ab --> Ae
        {
            *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==600)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==601)                                                  //Ab --> Ac
        {
            *iX=*iX+(dMovePitchX*0.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==610)                                                  //Ac --> Ae
        {
            *iX=*iX+(-dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==611)                                                  //Ad --> Ag
        {
            *iX=*iX+(-dMovePitchX*2.0+TestIF.dSiteXPitch*2.5);
        }
        else if(iModeCol==700)                                                  //Aa --> Ab
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*2.5);
        }
        else if(iModeCol==710)                                                  //Ab --> Af
        {
            *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==800)                                                  //Aa --> Ab
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*2.5);
        }
        else if(iModeCol==801)                                                  //Ab --> Ad
        {
            *iX=*iX+(dMovePitchX*0.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==810)                                                  //Ac --> Af
        {
            *iX=*iX+(-dMovePitchX*1.0+TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==811)                                                  //Ad --> Ah
        {
            *iX=*iX+(-dMovePitchX*2.0+TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==900)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*((double)iCloseSiteStep_2x8-3.5));
        }
        else if(iModeCol==1000)                                                 //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*((double)iCloseSiteStep_2x8-3.5));
        }
        else if(iModeCol==1100)                                                 //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*((double)iCloseSiteStep_2x8-3.5));
        }
        else if(iModeCol==1200)                                                 //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*((double)iCloseSiteStep_2x8-3.5));
        }
        else if(iModeCol==1201)                                                 //Ab --> Aa
        {
            *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*((double)iCloseSiteStep_2x8-3.5));
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
        else if(iModeCol==300)                                                  //Ba --> Ba
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==310)                                                  //Aa --> Af
        {
            *iX=*iX+(dMovePitchX*2.0+TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==311)                                                  //Bb --> Be
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==400)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==410)                                                  //Ab --> Ae
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==411)                                                  //Ba --> Bf
        {
            *iX=*iX+(dMovePitchX*2.0+TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==500)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==510)                                                  //Ab --> Ae
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==600)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==601)                                                  //Ab --> Ac
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==610)                                                  //Ac --> Ae
        {
            *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==611)                                                  //Ad --> Ag
        {
            *iX=*iX+(-dMovePitchX*1.0+TestIF.dSiteXPitch*2.5);
        }
        else if(iModeCol==700)                                                  //Aa --> Ab
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*2.5);
        }
        else if(iModeCol==710)                                                  //Ab --> Af
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==800)                                                  //Aa --> Ab
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*2.5);
        }
        else if(iModeCol==801)                                                  //Ab --> Ad
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==810)                                                  //Ac --> Af
        {
            *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==811)                                                  //Ad --> Ah
        {
            *iX=*iX+(-dMovePitchX*1.0+TestIF.dSiteXPitch*3.5);
        }
        else if(iModeCol==900)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0+TestIF.dSiteXPitch*((double)iCloseSiteStep_2x8-3.5));
        }
        else if(iModeCol==1000)                                                 //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0+TestIF.dSiteXPitch*((double)iCloseSiteStep_2x8-3.5));
        }
        else if(iModeCol==1100)                                                 //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0+TestIF.dSiteXPitch*((double)iCloseSiteStep_2x8-3.5));
        }
        else if(iModeCol==1200)                                                 //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0+TestIF.dSiteXPitch*((double)iCloseSiteStep_2x8-3.5));
        }
        else if(iModeCol==1201)                                                 //Ba --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*((double)iCloseSiteStep_2x8-3.5));
        }
    }

    *iY=*iY+GetOutArmYToShuttleOffset_9045(iSht, iOffsetPos);
    *iX=*iX+GetOutArmXToShuttleOffset_9045(iSht, iOffsetPos);
    TransferOutShuttleRatio(iSht, iX, iY, iOutArmYBase, iOutArmXBase);
    if(OutArmZNeedDown_2x8_8(iSht, iKit)==false)
        return false;
    return true;
}
//==============================================================================
bool MoveOutArmToShuttleIncludeZ_9045_2x8_8(int iSht, int iKit, bool bZDown)
{
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXVariable[X_PITCH_COUNT];
    int iXPos               =iOutArmShtXCenterPos;                              //Steven 20141029 : XY-Pitch for Shuttle Center position for base Suck
    int iYPos               =iOutArmShtYCenterPos;
    int iMode               =GetNowShuttleMode_2x8_8(iSht, iKit);
    int iModeRow            =iMode%100;
    int iModeCol            =iMode/100;
    int iMovePitchX         =iXpitchMaxX3;
    int iMovePitchY         =GetVariableYOutShuttleData();
    int iOffsetPos          =GetOutArmToShuttleOffset_9045(iSht, iModeRow, iModeCol+iKit*10, true); //Steven 20240826 : Fixed for offset
    int iYVariable          =GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);
    ZeroMemory(iXVariable,  sizeof(iXVariable));
    ZeroMemory(bZFlag,      sizeof(bZFlag));
    ZeroMemory(iZPos,       sizeof(iZPos));

    if(USE_PICKER_COUNT==ep16Picker)                                            //基準為第四隻吸嘴
    {
        if(iCloseSiteModeFor2x8==e2x8CloseEven || iCloseSiteModeFor2x8==e2x8CloseOdd)
            iMovePitchX=TestIF.dSiteXPitch/2.0*7.0;
        else if(iCloseSiteModeFor2x8==e2x8CloseEven1By1 || iCloseSiteModeFor2x8==e2x8CloseOdd1By1)
            iMovePitchX=iXpitchMaxX7;
//        else if(iCloseSiteModeFor2x8==e2x8Run2x2_13)
//            iMovePitchX=TestIF.dSiteXPitch/4.0*7.0;
//        else if(iCloseSiteModeFor2x8==e2x8Run2x2_14)
//            iMovePitchX=TestIF.dSiteXPitch/6.0*7.0;
//        else if(iCloseSiteModeFor2x8==e2x8Run2x4Standard)
//            iMovePitchX=TestIF.dSiteXPitch/2.0*7.0;
//        else if(iCloseSiteModeFor2x8==e2x8Run2x4Step2)
//            iMovePitchX=TestIF.dSiteXPitch/4.0*7.0;
        else if(InArmSuck.iXStep==1)
            iMovePitchX=TestIF.dSiteXPitch/2.0*7.0;
        else if(InArmSuck.iXStep==2)
            iMovePitchX=TestIF.dSiteXPitch/4.0*7.0;                             //四隻吸嘴佔一個site pitch, 所以每隻吸嘴是半個site pitch
        else
            iMovePitchX=iXpitchMaxX7;
    }
    else
    {
        if(iCloseSiteModeFor2x8==e2x8CloseEven || iCloseSiteModeFor2x8==e2x8CloseOdd)
            iMovePitchX=TestIF.dSiteXPitch*3.0;
        else if(iCloseSiteModeFor2x8==e2x8CloseEven1By1 || iCloseSiteModeFor2x8==e2x8CloseOdd1By1)
            iMovePitchX=iXpitchMaxX3;
        else if(iCloseSiteModeFor2x8==e2x8Run2x2_13)
            iMovePitchX=TestIF.dSiteXPitch*3.0;                                 //Steven 20240512 : /2*3 --> *3
        else if(iCloseSiteModeFor2x8==e2x8Run2x2_14)
            iMovePitchX=TestIF.dSiteXPitch*2.0;
        else if(iCloseSiteModeFor2x8==e2x8Run2x4Standard)
            iMovePitchX=TestIF.dSiteXPitch*3.0;
        else if(iCloseSiteModeFor2x8==e2x8Run2x4Step2)
            iMovePitchX=TestIF.dSiteXPitch/2.0*3.0;
        else if(InArmSuck.iXStep==1)
            iMovePitchX=TestIF.dSiteXPitch*3.0;
        else if(InArmSuck.iXStep==2)
            iMovePitchX=TestIF.dSiteXPitch/2.0*3.0;                             //兩隻吸嘴佔一個site pitch, 所以每隻吸嘴是半個site pitch
        else
            iMovePitchX=iXpitchMaxX3;
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    if(iKit!=0 && iCloseSiteModeFor2x8<=e2x8OneByOne)                           //JerryYang 20190729 STM 8 site交錯模式
        iXPos+=TestIF.dSiteXPitch*4;

    if(CheckOutArmXYPitch_2x8_8(&iXPos, &iYPos, iSht, iKit, iMovePitchX, iMovePitchY)==false && bZDown==true)
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
bool DoPickFromShuttle_9045_2x8_8(int iSht)
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

    if(iCloseSiteModeFor2x8>=e2x8CloseEven)
        iOutArmiWhichKit=0;

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
                if(iOutArmiWhichKit==0 && OutArmSuck.NoIC())                    //kevin 20210615 第一次 吸取IC 需等待
                {
                    if(MoveOutArmToShuttleIncludeZ_9045_2x8_8(iSht, iOutArmiWhichKit, false))  //移到 Shuttle 上等待
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
            Task=200;
        case 200:
            if(MoveOutArmToShuttleIncludeZ_9045_2x8_8(iSht, iOutArmiWhichKit, true))    //kevin 20210717 change function
            {
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
                    iShtCol   =GetShuttleCol(i, j+iPickKit, true);              //JerryYang 20250711 : fix for 2x8特殊關SITE

                    if(ptrOutSHT->Item[iShtRow][iShtCol] &&
                       OutArmSuck.Suck[iSuckRow][iSuckCol].GetNeedSuckStatus())
                    {
                        if(OutArmSuck.Suck[iSuckRow][iSuckCol].Suck())
                        {
                            //模擬ic不見了
                            #ifdef SOFT_SIMULTE
                            if(fMain->chkInPickLoadError->Checked)
                            {
                                if(iOutArmiWhichKit==0 && iSuckRow==0 && iSuckCol==2)
                                {
                                    OutArmSuck.Suck[0][2].Error=true;
                                    continue;
                                }
                                else if(iOutArmiWhichKit==1 && iSuckRow==0 && iSuckCol==2)
                                {
                                    OutArmSuck.Suck[0][2].Error=true;
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

            if(iHWFix_BinBox==1 &&
               ErrorBinBoxDetect(iWitchErrBin)==false)
                return false;                                                   //kevin 20160906 add error bin box

            if(USE_Fix_AI_CCD==true &&
               TestIF_File.bEnableFix2BGAAICCD==true)                           //KaiChen 20190124 ：矽格湖口 AI CCD Function
            {
                fFixAICCD->OutArmCycleCounterUpdate();
            }

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
                                    iShtCol=GetShuttleCol(i, j+iPickKit);
                                    PorcessJAM0201OutArmPickUpErrorSkip(iSht, i, iShtCol, i, j);          //Steven 20161214 : 加上OutArm吸取異常Skip的ErrorLog
                                    bOutSuckShtDupErr[i][j]=false;
                                }
                            }
                        }

                        iOutShtRetryCount=0;
                        if((bDoLeftShuttle==true  && ptrOutSHT->LeftSideNoIC(OutArmSuck.iShtKitStep)) ||
                           (bDoLeftShuttle==false && ptrOutSHT->RightSideNoIC(OutArmSuck.iShtKitStep)))        //Steven 20170327 (Jou) : 修改Out放料判斷方式
                        {
                            ZeroMemory(bOutSuckShtDupErr, sizeof(bOutSuckShtDupErr));
                            if(USE_Fix_AI_CCD==true && TestIF_File.bEnableFix2BGAAICCD==true)   //Sam 20240329 : 新增 FixAOI 模式
                            {
                                fFixAICCD->OutArmCycleCounterUpdate();
                            }
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
            SetOutArmHome();
            iOutShtRetryCount=0;
            Task=1;
            break;
    }
    return false;
}
//==============================================================================
void GetTrayPosition_AccordingBaseSuck_9045_2x8_8(int iToWhichAuto, int iTrayX, int iTrayY, int &iPosX, int &iPosY)  //JimmyChiu 20211020 : Auto alignment mode
{
    if(iToWhichAuto>=6)
        iToWhichAuto-=3;
    iPosX=Prod.XStart[iToWhichAuto][iOutArmYBase][iOutArmXBase]+iTrayX*AutoForm[iToWhichAuto]->XPitch;
    iPosY=Prod.YStart[iToWhichAuto][iOutArmYBase][iOutArmXBase]-iTrayY*AutoForm[iToWhichAuto]->YPitch;
}
//---------------------------------------------------------------------------
void GetTrayPosition_AccordingTopLeftActiveSuck_9045_2x8_8(int iToWhichAuto, int iActiveSuckX, int iActiveSuckY, int &iPosX, int &iPosY, int *iXVariable, int &iYVariable)  //JimmyChiu 20211020 : Auto alignment mode
{
    int iMovePitchX=AutoCalculateOutArmXClosePitch(iToWhichAuto);
    int iMovePitchY=AutoCalculateOutArmYClosePitch(iToWhichAuto);

    for(int i=0; i<X_PITCH_COUNT; i++)
    {
        iXVariable[i]=GetOutArmPitchX_9045(i, iMovePitchX, OutOfsAuto1+iToWhichAuto);
    }
    //Offset according active suck
    iPosX-=((iActiveSuckX-iOutArmXBase)*dOutArmXPitch_1Step);
    iPosY+=((iActiveSuckY-iOutArmYBase)*iMovePitchY);
}
//---------------------------------------------------------------------------
bool DoMoveOutArmXYToPlaceForAutoTeachOffset_9045_2x8_8(int iwhm, int iTrayX, int iTrayY)  //JimmyChiu 20211020 : Auto alignment mode
{
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));
    iwhm=iwhm>6?iwhm-3:iwhm;
    GetTrayPosition_AccordingBaseSuck_9045_2x8_8(iwhm, iTrayX, iTrayY, iXPos, iYPos);
    GetTrayPosition_AccordingTopLeftActiveSuck_9045_2x8_8(iwhm, 0, 0, iXPos, iYPos, iXVariable, iYVariable);
    if(IsTrayArmMoveAvoidOutArmCrash())
    {
        MOT[MOutArmX].PCIL132_StopMotor();
        MOT[MOutArmY].PCIL132_StopMotor();
        return false;
    }

    GetOutArmZDownPos_9045(iWhichAuto, true, bZFlag, iZPos);
    iOutArmYPosition=iTrayY;
    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
/*
0:unfinish
1:finish without running next step
2:finish and running next step
*/
int DoPlaceToAutoForAutoTeachOffset_9045_2x8_8()                                //JimmyChiu 20211020 : Auto alignment mode
{
    if(fAutoTeach->IsRun()==false)
        return true;
    switch(fAutoTeach->iTaskPlaceToAuto)
    {
        case 0: //no 0 ->1
            switch(fAutoTeach->GetState())
            {
                case fAutoTeach->eATFix3:
                    fAutoTeach->iTaskPlaceToAuto=100;
                    fAutoTeach->iWhichAuto=eFix3;
                    break;
                case fAutoTeach->eATAuto1:                                      //need move to fix3 again, cuz 16 sites per shuttle
                    fAutoTeach->iTaskPlaceToAuto=100;
                    fAutoTeach->iWhichAuto=eAuto1;
                    break;
                case fAutoTeach->eATAuto2:
                    fAutoTeach->iTaskPlaceToAuto=100;
                    fAutoTeach->iWhichAuto=eAuto2;
                    break;
                case fAutoTeach->eATAuto3:
                    fAutoTeach->iTaskPlaceToAuto=100;
                    fAutoTeach->iWhichAuto=eAuto3;
                    break;
                case fAutoTeach->eATFix1:
                    fAutoTeach->iTaskPlaceToAuto=100;
                    fAutoTeach->iWhichAuto=eFix1;
                    break;
                case fAutoTeach->eATFix2:
                    fAutoTeach->iTaskPlaceToAuto=100;
                    fAutoTeach->iWhichAuto=eFix2;
                    break;
                default:
                    fAutoTeach->iTaskPlaceToAuto=0;
                    return 2;
            }
            break;
        case 100:
            if(MoveOutArmToAutoSafe())
            {
                fAutoTeach->iTaskPlaceToAuto=200;
            }
            break;
        case 200:
            if(DoMoveOutArmXYToPlaceForAutoTeachOffset_9045_2x8_8(fAutoTeach->iWhichAuto, 0, 0))
                fAutoTeach->iTaskPlaceToAuto=300;
            break;
        case 300:
            bEnterOffset=false;
            fAutoTeach->iTaskPlaceToAuto=400;
            fMain->Pause("DoPlaceToAutoForAutoTeachOffset_9045_2x8_8");
            break;
        case 400:
            if(bEnterOffset==false)
            {
                fAutoTeach->iTaskPlaceToAuto=0;
                if(fAutoTeach->GetState()==fAutoTeach->eATFix3 ||
                   fAutoTeach->GetState()==fAutoTeach->eATFix2)
                    fAutoTeach->iTaskPlaceToAuto=500;
                else
                    fAutoTeach->DoNext();
            }
            else
            {
                bEnterOffset=false;
                fAutoTeach->iTaskPlaceToAuto=100;
            }
            break;
        case 500:
            fAutoTeach->DoNext();
            return 2;
        default:
            return 1;
    }
    return 0;
}
//---------------------------------------------------------------------------
void DoOutArm_9045_2x8_8()
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
                if(bUseTwoArm32Site==true)
                {
                    if(TestIF_File.iShuttleMode==1)                             //Jou 20150909 : For Add 32Site UPH, 讓Shuttle 2先回去補Input
                    {
                        if(FRCarryKit.UseSiteHasIC() && OutSHT1InRT())
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
                    else
                    {
                        if((BRCarryKit.UseSiteHasIC()) && OutSHT2InRT())
                        {
                            OldPos=0;
                            FirstEnter=false;
                        }
                        else if(FRCarryKit.UseSiteHasIC() && OutSHT1InRT())            //jou 20171227 : 修正32 sites hang up
                        {
                            OldPos=1;
                            FirstEnter=false;
                        }
                    }
                }
                else
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
                iOutArmiWhichKit=0;
                Task=1000;
            }
            else
            {
                iWhichShuttle=1;
                iOutArmiWhichKit=0;
                Task=2000;
            }

            if(iCloseSiteModeFor2x8==e2x8_STMMode ||                        //JerryYang 20190729 STM 8 site交錯模式
               iCloseSiteModeFor2x8==e2x8_TW153Mode)
            {
                OldPos=1-OldPos;
            }
            break;
        case 200:
            if(MoveOutArmToAutoSafe())
            {
                Task=300;
            }
            else
            {
                break;
            }
        case 300:
            if(bUseTwoArm32Site==true)                                          //Jou 20150909 : For Add 32Site UPH, 讓Shuttle 2先回去補Input   0-->1
            {
                if(MoveOutArmToShuttleIncludeZ_9045_2x8_8(1, iOutArmiWhichKit, false))
                    Task=CheckOutArmCleanOut();
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

            if(FRCarryKit.HasRealIC_Left(OutArmSuck.iShtRow, OutArmSuck.iShtKitStep))
            {
                iOutArmiWhichKit=0;
            }
            else if(FRCarryKit.HasRealIC_Right(OutArmSuck.iShtRow, OutArmSuck.iShtKitStep))
            {
                iOutArmiWhichKit=1;
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
                        InitPickFromShuttle1Task();                             //Sam 20230208 : 修正 Fix AOI 未拍照問題

                        if(FRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep))
                        {
                            iOutArmiWhichKit=1;
                        }
                        else
                        {
                            iOutArmiWhichKit=0;
                        }
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

            if(MoveOutArmToShuttleIncludeZ_9045_2x8_8(iWhichShuttle, iOutArmiWhichKit, false))
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
                }
                else
                {
                    iOutArmiWhichKit=0;
                }
                Task=1200;
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

//            if(bPickShuttleError)                                             //Sam 20240904 : mark
//            {
//                bPickShuttleError=false;
//                InitPickFromShuttle1Task();
//                break;
//            }

            if(DoPickFromShuttle_9045_2x8_8(0))
            {
                PreSetOutAdditionalFlag();                                      //Steven 20220614 : Fixed for out arm additional function
                FRCarryKit.SetUnuseAndHasNullICToNullIC();
                if((iOutArmiWhichKit==0 && FRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep)==false) ||
                   (iOutArmiWhichKit==1 && FRCarryKit.RightSideNoIC(OutArmSuck.iShtKitStep)==false))
                {
                    InitPickFromShuttle1Task();
                    return;
                }

                if(iCloseSiteModeFor2x8==e2x8_STMMode ||                        //JerryYang 20190729 STM 8 site交錯模式
                   iCloseSiteModeFor2x8==e2x8_TW153Mode)
                {
                    if(FRCarryKit.UseSiteHasIC())
                    {
                        if(FRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep))
                        {
                            iOutArmiWhichKit=1;
                        }
                        else
                        {
                            iOutArmiWhichKit=0;
                        }
                        InitPickFromShuttle1Task();                             //Sam 20230208 : 修正 Fix AOI 未拍照問題
                        return;
                    }
                }

                if(OutArmSuck.HasIC()==false)
                {
                    Task=50;
                }
                else
                {
                    if(FRCarryKit.UseSiteNoIC())
                    {
                        OldPos=1-OldPos;
                    }
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
            if(USE_OUT_Y_IS_AUTO_PITCH==true &&                                 //JerryYang 20251218 : IN/OUT ARM支援不同模組
               IniConfig.bE57YPitchHome)                                        //kevin 20180827 (Steven) : add put shuttle Y pitch home AutoYPitch
            {
                Task=2010;
                return;
            }
            Task=2040;
            break;
        case 2010:
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

            if(BRCarryKit.HasRealIC_Left(OutArmSuck.iShtRow, OutArmSuck.iShtKitStep))
            {
                iOutArmiWhichKit=0;
            }
            else if(BRCarryKit.HasRealIC_Right(OutArmSuck.iShtRow, OutArmSuck.iShtKitStep))
            {
                iOutArmiWhichKit=1;
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
                        InitPickFromShuttle2Task();                             //Sam 20230208 : 修正 Fix AOI 未拍照問題

                        if(BRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep))
                        {
                            iOutArmiWhichKit=1;
                        }
                        else
                        {
                            iOutArmiWhichKit=0;
                        }
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

            if(MoveOutArmToShuttleIncludeZ_9045_2x8_8(iWhichShuttle, iOutArmiWhichKit, false))
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
                //iPickFromShuttle2Task=1;
                if(BRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep))
                {
                    iOutArmiWhichKit=1;
                }
                else
                {
                    iOutArmiWhichKit=0;
                }
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

//            if(bPickShuttleError)                                             //Sam 20240904 : mark
//            {
//                bPickShuttleError=false;
//                InitPickFromShuttle2Task();
//                Task=2200;
//                break;
//            }

            if(DoPickFromShuttle_9045_2x8_8(1))
            {
                PreSetOutAdditionalFlag();                                      //Steven 20220614 : Fixed for out arm additional function
                BRCarryKit.SetUnuseAndHasNullICToNullIC();
                if((iOutArmiWhichKit==0 && BRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep)==false) ||
                   (iOutArmiWhichKit==1 && BRCarryKit.RightSideNoIC(OutArmSuck.iShtKitStep)==false))
                {
                    InitPickFromShuttle2Task();
                    return;
                }

                if(iCloseSiteModeFor2x8==e2x8_STMMode ||                        //JerryYang 20190729 STM 8 site交錯模式
                   iCloseSiteModeFor2x8==e2x8_TW153Mode)
                {
                    if(BRCarryKit.UseSiteHasIC())
                    {
                        if(BRCarryKit.LeftSideNoIC(OutArmSuck.iShtKitStep))
                        {
                            iOutArmiWhichKit=1;
                        }
                        else
                        {
                            iOutArmiWhichKit=0;
                        }
                        InitPickFromShuttle2Task();                             //Sam 20230208 : 修正 Fix AOI 未拍照問題
                        return;
                    }
                }

                if(OutArmSuck.HasIC()==false)
                {
                    Task=50;
                }
                else
                {
                    if(BRCarryKit.UseSiteNoIC())
                    {
                        OldPos=1-OldPos;
                    }
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
                if(bUseTwoArm32Site==true)
                {
                    if(TestIF_File.iShuttleMode==0)                             //Jou 20150909 : For Add 32Site UPH, 讓Shuttle 2先回去補Input
                    {
                        if(FRCarryKit.UseSiteNoIC())
                            MOT[MInShuttle1].fCanMoveR=true;
                        if(BRCarryKit.UseSiteNoIC())
                            MOT[MInShuttle2].fCanMoveR=true;
                    }
                    else
                    {
                        MOT[MInShuttle1].fCanMoveR=true;
                        MOT[MInShuttle2].fCanMoveR=true;
                    }
                }
                else
                {
                    MOT[MInShuttle1].fCanMoveR=true;
                    MOT[MInShuttle2].fCanMoveR=true;
                }

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

            if(TRAY_ARM_MODE==eAboveCoveyor &&
               MOT[MTrayX].ReadPos()>Prod.iXTrayColor)
            {
                MoveOutArmXY_ToFix_Tray_Full();
                break;
            }
            InitPlaceToAutoTask();

            if(iWhichAuto<=iAutoRight)
            {
                if(TRAY_ARM_MODE==eAboveCoveyor &&
                   (MOT[iMMAuto[iWhichAuto]].fHasTray==false ||
                    MOT[iMMAuto[iWhichAuto]].FullIC()))
                {
                    break;
                }
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
                    if(TRAY_ARM_MODE==eAboveCoveyor &&
                       (MOT[iMMAuto[iWhichAuto]].fHasTray==false ||
                        MOT[iMMAuto[iWhichAuto]].FullIC()))
                    {
                        if(iHWFix_BinBox==1 && iWhichAuto==eBulkBox)
                            DoFixTrayFullAlarm();                               //kevin 20201207 check Error bin box Fix 1 put tray
                        break;
                    }
                }
            }

            if(TRAY_ARM_MODE==eAboveCoveyor &&
               (iCatchTrayControlManual>=2 || WhichAutoNeedTray()!=0))          //Sam 20180822 : OutArm 是否在安全位置，是否讓位，不然會與 TrayArm 補 Tray 動作互卡 Hange up。
            {
                MoveOutArmXY_ToFix_Tray_Full();
                break;
            }
            else if(TRAY_ARM_MODE==eUnderCoveyor&&
                   (iCatchTrayControlManual>=2 || WhichAutoNeedTray()!=0))
            {
                MoveOutArmToAutoSafe_9045();
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

            if(fAutoTeach->IsRun())                                             //JimmyChiu 20211020 : Auto alignment mode
            {
                Task=3305;
                break;
            }
            Task=3310;
            break;
        case 3305:                                                              //JimmyChiu 20211020 : Auto alignment mode
            ret=DoPlaceToAutoForAutoTeachOffset_9045_2x8_8();
            /*
            0:unfinish
            1:finish without running next step
            2:finish and running next step
            */
            if(ret==1)
                Task=100;
            else if(ret==2)
                Task=3310;
            if(Task!=3310)                                                      //Steven 20180813 : add Out arm speed
                break;
        case 3310:
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
                if(fAGV->IsATK_AMR() &&
                   LastSet.iUnloadFixTray==eAtkTfMoveFixIC)                     //Steven 20260202 : for ATK AMR
                {
                    iOutArmWhichAuto=iWhichAuto;
                }
                else
                {
                    iOutArmWhichAuto=iMagAtAuto;
                    if(iAuto3MagazineIndex!=iWhichMag && iWhichMag!=-1)
                    {
                        bMagNeedChangeTray=true;
                    }
                }
                Task=12000;
            }
            break;
        case 12000:
            IsCatchTrayReadySupplyNewTray();
            if(fAGV->IsATK_AMR() &&
               LastSet.iUnloadFixTray==eAtkTfMoveFixIC)                         //Steven 20260202 : for ATK AMR
            {
                if(MOT[iMMAuto[iWhichAuto]].fHasTray==false ||
                   MOT[iMMAuto[iWhichAuto]].FullIC())
                {
                    MoveOutArmXY_ToFix_Tray_Full();                             // change Magazine Tray
                    break;
                }
            }
            else if(iAuto3MagazineIndex!=iWhichMag ||
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
