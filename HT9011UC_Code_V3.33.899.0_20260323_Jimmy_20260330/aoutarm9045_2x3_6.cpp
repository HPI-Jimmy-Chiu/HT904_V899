#include "MachineDefine.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include "aoutarm9045_2x3_6.h"

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
//使用吸嘴 Aa, Ab, Ac
//個位數為Y座標, 2=標準, 0=A排, 1=B排
//==============================================================================
int GetNowShuttleMode_2x3_6(int iSht)
{
    ptrOutSHT=(iSht==0)?&FRCarryKit:&BRCarryKit;

    if(OutArmSuck.iXStep==1 && OutArmSuck.iYStep==1)
    {
        return 2;
    }
    else if(ptrOutSHT->UseSiteNoIC())
    {
        if(OutArmSuck.iXStep==1)
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
        if(OutArmSuck.iXStep==1)
        {
            for(int j=0; j<OutArmSuck.iPickCol; j++)
            {
                if(ptrOutSHT->Item[0][j]>=HAS_IC)
                    return 0;                                                   //A排四顆
            }
            return 1;                                                           //B排四顆
        }
        else if(OutArmSuck.iXStep==2)
        {
            if(OutArmSuck.iYStep==1)
            {
                if(ptrOutSHT->Item[0][0]>=HAS_IC || ptrOutSHT->Item[0][1]>=HAS_IC ||
                   ptrOutSHT->Item[1][0]>=HAS_IC || ptrOutSHT->Item[1][1]>=HAS_IC)
                    return 10002;                                               //左邊四顆
                else //if(ptrOutSHT->[0][2]>=HAS_IC ||
                     //   ptrOutSHT->[1][2]>=HAS_IC ||)
                    return 10102;                                               //右邊四顆
            }
            else
            {
                if(ptrOutSHT->Item[0][0]>=HAS_IC || ptrOutSHT->Item[0][1]>=HAS_IC)
                    return 10000;                                               //左上兩顆
                else if(ptrOutSHT->Item[1][0]>=HAS_IC || ptrOutSHT->Item[1][1]>=HAS_IC)
                    return 10001;                                               //左下兩顆
                else if(ptrOutSHT->Item[0][2]>=HAS_IC)
                    return 10100;                                               //右上兩顆
                else //if(ptrOutSHT->Item[1][2]>=HAS_IC)
                    return 10101;                                               //右下兩顆
            }
        }
        else //if(OutArmSuck.iXStep==4)
        {
            if(OutArmSuck.iYStep==1)
            {
                if(ptrOutSHT->Item[0][0]>=HAS_IC ||
                   ptrOutSHT->Item[1][0]>=HAS_IC)
                    return 20002;
                else if(ptrOutSHT->Item[0][1]>=HAS_IC ||
                        ptrOutSHT->Item[1][1]>=HAS_IC)
                    return 20102;
                else //if(ptrOutSHT->Item[0][2]>=HAS_IC ||
                     //   ptrOutSHT->Item[1][2]>=HAS_IC)
                    return 20202;
            }
            else
            {
                if(ptrOutSHT->Item[0][0]>=HAS_IC)
                    return 20000;
                else if(ptrOutSHT->Item[1][0]>=HAS_IC)
                    return 20001;
                else if(ptrOutSHT->Item[0][1]>=HAS_IC)
                    return 20100;
                else if(ptrOutSHT->Item[1][1]>=HAS_IC)
                    return 20101;
                else if(ptrOutSHT->Item[0][2]>=HAS_IC)
                    return 20200;
                else //if(ptrOutSHT->Item[1][2]>=HAS_IC)
                    return 20201;
            }
        }
    }
}
//==============================================================================
bool OutArmZNeedDown_2x3_6(int iSht)
{
    if(InitialOutArmNeedSuck(iSht)==false)
        return false;

    ptrOutSHT   =(iSht==0)?&FRCarryKit:&BRCarryKit;
    int iMode   =GetNowShuttleMode_2x3_6(iSht);
    int iModeRow=iMode%100;
    int iModeCol=iMode/100;
    int iShtJ   =0;

    for(int i=0; i<OutArmSuck.iPickRow; i++)
    {
        for(int j=0; j<OutArmSuck.iPickCol; j++)
        {
            iShtJ=GetShuttleCol(i, j, true);                                    //Sam 20250218 : 大 Pitch 要用右邊6吸嘴不然行程可能會不夠
            if(iModeCol==0 &&
               (iModeRow==2 || iModeRow==i))                                    //Steven 20240205 : 修正 Out arm 放料
            {
                SetOutArmNeedDestory(i, j, i, iShtJ);
            }
            else if(iModeCol==100 && (j==0 || j==2) &&                          //吸嘴       ==>  蝦頭
                    (iModeRow==2 || iModeRow==i))                               // O X O X        O O X
            {
                SetOutArmNeedDestory(i, j, i, iShtJ);
            }
            else if(iModeCol==101 && (j==1) &&                                  //吸嘴       ==>  蝦頭
                    (iModeRow==2 || iModeRow==i))                               // X O X X        X X O
            {
                SetOutArmNeedDestory(i, j, i, iShtJ);
            }
            else if(iModeCol==200 && j==0 &&                                    //吸嘴       ==>  蝦頭
                    (iModeRow==2 || iModeRow==i))                               // O X X X        O X X
            {
                SetOutArmNeedDestory(i, j, i, iShtJ);
            }
            else if(iModeCol==201 && j==1 &&                                    //吸嘴       ==>  蝦頭
                    (iModeRow==2 || iModeRow==i))                               // X O X X        X O X
            {
                SetOutArmNeedDestory(i, j, i, iShtJ);
            }
            else if(iModeCol==202 && j==2 &&                                    //吸嘴       ==>  蝦頭
                    (iModeRow==2 || iModeRow==i))                               // X X O X        X X O
            {
                SetOutArmNeedDestory(i, j, i, iShtJ);
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool CheckOutArmXYPitch_2x3_6(int *iX, int *iY, int iSht, int iMovePitchX, int iMovePitchY)
{
    int iMode       =GetNowShuttleMode_2x3_6(iSht);
    int iModeRow    =iMode%100;
    int iModeCol    =iMode/100;
    int iOffsetPos  =GetOutArmToShuttleOffset_9045(iSht, iModeRow, iModeCol, false);
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
        if(USE_OUT_Y_IS_AUTO_PITCH==true)                                       //JerryYang 20251218 : IN/OUT ARM支援不同模組
            *iY=*iY-iMovePitchY+TestIF.dSiteYPitch/2;
        else
            *iY=*iY+TestIF.dSiteYPitch/2;
    }
    else if(iModeRow==1)                                                        //Row B
    {
        if(USE_OUT_Y_IS_AUTO_PITCH==true)                                       //JerryYang 20251218 : IN/OUT ARM支援不同模組
            *iY=*iY-TestIF.dSiteYPitch/2;
        else
            *iY=*iY+iMovePitchY-TestIF.dSiteYPitch/2;
    }
    else                                                                        //Both
    {
        if(USE_OUT_Y_IS_AUTO_PITCH==true)                                       //JerryYang 20251218 : IN/OUT ARM支援不同模組
            *iY=*iY-TestIF.dSiteYPitch/2;
        else
            *iY=*iY+TestIF.dSiteYPitch/2;
    }

    /*if(USE_PICKER_COUNT==ep16Picker)                                          //基準為第四隻吸嘴
    {
        dMovePitchX=double(iMovePitchX)/7.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==101)                                                  //Ac --> Ac
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==200)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*3.0-TestIF.dSiteXPitch*1.5);
        }
        else if(iModeCol==201)                                                  //Ac --> Ab
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==202)                                                  //Ae --> Ac
        {
            *iX=*iX+(-dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
    }
    else */if(USE_OUT_ARM_Y_PITCH==iXYPitchVariable)                         //基準為第二隻吸嘴  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        dMovePitchX=double(iMovePitchX)/3.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*1.0);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*1.0);
        }
        else if(iModeCol==101)                                                  //Ab --> Ac
        {
            *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*1.0);
        }
        else if(iModeCol==200)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0-TestIF.dSiteXPitch*1.0);
        }
        else if(iModeCol==201)                                                  //Ab --> Ab
        {
            *iX=*iX+(dMovePitchX*0.0-TestIF.dSiteXPitch*0.0);
        }
        else if(iModeCol==202)                                                  //Ac --> Ac
        {
            *iX=*iX+(-dMovePitchX*1.0+TestIF.dSiteXPitch*0.0);
        }
    }
    else                                                                        //基準為第三隻吸嘴
    {
        dMovePitchX=double(iMovePitchX)/3.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*1.0);
        }
        else if(iModeCol==100)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*1.0);
        }
        else if(iModeCol==101)                                                  //Ab --> Ac
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*1.0);
        }
        else if(iModeCol==200)                                                  //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0-TestIF.dSiteXPitch*1.0);
        }
        else if(iModeCol==201)                                                  //Ab --> Ab
        {
            *iX=*iX+(dMovePitchX*0.0-TestIF.dSiteXPitch*0.0);
        }
        else if(iModeCol==202)                                                  //Ac --> Ac
        {
            *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*1.0);
        }
    }

    *iY=*iY+GetOutArmYToShuttleOffset_9045(iSht, iOffsetPos);
    *iX=*iX+GetOutArmXToShuttleOffset_9045(iSht, iOffsetPos);
    TransferOutShuttleRatio(iSht, iX, iY, iOutArmYBase, iOutArmXBase);
    if(OutArmZNeedDown_2x3_6(iSht)==false)
        return false;
    return true;
}
//==============================================================================
bool MoveOutArmToShuttleIncludeZ_9045_2x3_6(int iSht, bool bZDown)
{
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXVariable[X_PITCH_COUNT];
    int iXPos               =iOutArmShtXCenterPos;                              //Steven 20141029 : XY-Pitch for Shuttle Center position for base Suck
    int iYPos               =iOutArmShtYCenterPos;
    int iMode               =GetNowShuttleMode_2x3_6(iSht);
    int iModeRow            =iMode%100;
    int iModeCol            =iMode/100;
    int iMovePitchX         =iXpitchMaxX3;
    int iMovePitchY         =GetVariableYOutShuttleData();
    int iOffsetPos          =GetOutArmToShuttleOffset_9045(iSht, iModeRow, iModeCol, true);
    int iYVariable          =GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);
    ZeroMemory(iXVariable,  sizeof(iXVariable));
    ZeroMemory(bZFlag,      sizeof(bZFlag));
    ZeroMemory(iZPos,       sizeof(iZPos));

    if(USE_16PICKER_TYPE==1)
    {
        if(OutArmSuck.iXStep==1)
            iMovePitchX=TestIF.dSiteXPitch/2*7;
        else if(OutArmSuck.iXStep==2)
            iMovePitchX=TestIF.dSiteXPitch/4*7;
        else
            iMovePitchX=iXpitchMaxX7;
    }
    else
    {
        if(OutArmSuck.iXStep==1)
            iMovePitchX=TestIF.dSiteXPitch*3;
        else if(OutArmSuck.iXStep==2)
            iMovePitchX=TestIF.dSiteXPitch/2*3;
        else
            iMovePitchX=iXpitchMaxX3;
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    if(CheckOutArmXYPitch_2x3_6(&iXPos, &iYPos, iSht, iMovePitchX, iMovePitchY)==false && bZDown==true)
        return false;

    GetOutShuttleStatus_9045(iSht, iZPos, bZDown, bZFlag);
    if(IsCheckOutArmDestroyActiveFinish(iXPos, iYPos)==false)                   //2013-08-02    Dell     add Pick 前先確認CheckInArmDestroyActive 已完成
        return false;

    DoOutArmSuckPreOn(iSht, 0, iXPos, iYPos, bZDown);
    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, bZDown))
        return true;

    return false;
}
//==============================================================================
// 輸出臂從 Shuttle 1 吸取 IC 流程動作
//  Ver : 2003_07
//==============================================================================
bool DoPickFromShuttle_9045_2x3_6(int iSht)
{
    int ret;
    int &Task=(iSht==0)?iPickFromShuttle1Task:iPickFromShuttle2Task;
    static int iWitchErrBin=0;
    AnsiString ErrPart="";                                                      //Steven 20110216 : 合併Alarm
    bool bHasDuplicateErr=false;                                                //Steven 20110216 : 合併Alarm
    bool bHasErr=false;                                                         //Steven 20110216 : 合併Alarm
    bool flag=false;
    int iSuckRow=0, iSuckCol=0, iShtRow=0, iShtCol=0;                           //JerryYang 20231004 : single site suck XY固定00
    ptrOutSHT=(iSht==0)?&FRCarryKit:&BRCarryKit;

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
                    if(MoveOutArmToShuttleIncludeZ_9045_2x3_6(iSht, false))     //移到 Shuttle 上等待
                    {
                        PickFromShuttle(0);                                     //kevin 20210612 add int iwhichKit=0
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
            if(MoveOutArmToShuttleIncludeZ_9045_2x3_6(iSht, true))              //kevin 20210717 change function
            {
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
                Task=iOUTARM_SUCK;
                OutArmSuck.ResetAll();                                          //Steven 20160323 : 避免未開啟真空
            }
            break;
        case iOUTARM_SUCK:
            flag=true;
            bHasErr=false;
            for(int i=0; i<OutArmSuck.iPickRow; i++)
            {
                for(int j=0; j<OutArmSuck.iPickCol; j++)
                {
                    iSuckRow  =i;
                    iSuckCol  =j;
                    iShtRow   =i;
                    iShtCol   =GetShuttleCol(i, j, true);                       //Sam 20250218 : 大 Pitch 要用右邊6吸嘴不然行程可能會不夠
                    if(ptrOutSHT->Item[iShtRow][iShtCol] &&
                       OutArmSuck.Suck[iSuckRow][iSuckCol].GetNeedSuckStatus())
                    {
                        if(OutArmSuck.Suck[iSuckRow][iSuckCol].Suck())
                        {
                            //模擬ic不見了
                            #ifdef SOFT_SIMULTE
                            if(fMain->chkInPickLoadError->Checked==true)
                            {
                                if(i==0 && j==0)
                                {
                                    OutArmSuck.Suck[0][0].Error=true;
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

            if(iHWFix_BinBox==1 && ErrorBinBoxDetect(iWitchErrBin)==false)
                return false;                                                   //kevin 20160906 add error bin box

            if(USE_Fix_AI_CCD==true && TestIF_File.bEnableFix2BGAAICCD==true)   //KaiChen 20190124 ：矽格湖口 AI CCD Function
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
                                    iShtCol=GetShuttleCol(i, j , true);         //Sam 20250218 : 大 Pitch 要用右邊6吸嘴不然行程可能會不夠
                                    PorcessJAM0201OutArmPickUpErrorSkip(iSht, i, iShtCol, i, j);      //Steven 20161214 : 加上OutArm吸取異常Skip的ErrorLog
                                }
                                bOutSuckShtDupErr[i][j]=false;
                            }
                        }

                        if(USE_Fix_AI_CCD==true &&                              //KaiChen 20190124 ：矽格湖口 AI CCD Function
                           TestIF_File.bEnableFix2BGAAICCD==true)
                        {
                            fFixAICCD->OutArmCycleCounterUpdate();
                        }

                        iOutShtRetryCount=0;
                        ZeroMemory(bOutSuckShtDupErr, sizeof(bOutSuckShtDupErr));
                        Task=1;
                        return true;
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
void DoOutArm_9045_2x3_6()
{
    int ret, ct, iMode1=0, iMode2=0;
    static int OldPos=1;
    static bool FirstEnter=true, bCheckSpeed=false;
    int &Task=OutArmTask;
    static int iWhichShuttle=0, iBackMode1=0, iBackMode2=0;
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

            if(TestIF.iShuttleMode==1)
            {
                if(TestIF.iShuttle_Sel==0)
                    OldPos=1;
                else
                    OldPos=0;
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
            if(MoveOutArmXY_ToFix_Tray_Full())                                  //Steven 20230602 : 統一out arm 讓位位置
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
            if(USE_OUT_Y_IS_AUTO_PITCH==true &&                                 //JerryYang 20251218 : IN/OUT ARM支援不同模組
               IniConfig.bE57YPitchHome)                                        //kevin 20180827 (Steven) : add put shuttle Y pitch home AutoYPitch
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

            if(MoveOutArmToShuttleIncludeZ_9045_2x3_6(iWhichShuttle))
            {
                IsCatchTrayReadySupplyNewTray();

                if(OutArmSuck.iXStep!=1 || OutArmSuck.iYStep!=1)
                {
                    iBackMode1=GetNowShuttleMode_2x3_6(0);
                }

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
                Task=1200;

                if(OutArmSuck.iXStep!=1 || OutArmSuck.iYStep!=1)
                {
                    iMode1=GetNowShuttleMode_2x3_6(0);
                    if(iMode1!=iBackMode1)
                    {
                        Task=1150;
                        break;
                    }
                    else
                    {
                        iMode1=0;
                        iBackMode1=0;
                    }
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

            if(DoPickFromShuttle_9045_2x3_6(0))
            {
                PreSetOutAdditionalFlag();                                      //Steven 20220614 : Fixed for out arm additional function
                FRCarryKit.SetUnuseAndHasNullICToNullIC();
                if(FRCarryKit.UseSiteHasIC())
                {
                    InitPickFromShuttle1Task();                                 //Sam 20230208 : 修正 Fix AOI 未拍照問題
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

            if(MoveOutArmToShuttleIncludeZ_9045_2x3_6(iWhichShuttle, false))
            {
                IsCatchTrayReadySupplyNewTray();

                if(OutArmSuck.iXStep!=1 || OutArmSuck.iYStep!=1)
                {
                    iBackMode2=GetNowShuttleMode_2x3_6(1);
                }
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
                Task=2200;

                if(OutArmSuck.iXStep!=1 || OutArmSuck.iYStep!=1)
                {
                    iMode2=GetNowShuttleMode_2x3_6(1);
                    if(iMode2!=iBackMode2)
                    {
                        Task=2050;
                        break;
                    }
                    else
                    {
                        iMode2=0;
                        iBackMode2=0;
                    }
                }
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

            if(DoPickFromShuttle_9045_2x3_6(1))
            {
                PreSetOutAdditionalFlag();                                      //Steven 20220614 : Fixed for out arm additional function
                BRCarryKit.SetUnuseAndHasNullICToNullIC();
                if(BRCarryKit.UseSiteHasIC())
                {
                    InitPickFromShuttle2Task();                                 //Sam 20230208 : 修正 Fix AOI 未拍照問題
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
