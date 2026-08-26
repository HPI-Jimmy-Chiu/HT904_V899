#include "MachineDefine.h"
#pragma hdrstop

#include "ainarm9045_1x1_1.h"

#include "aArmHeader.h"
#include "MyKitSuck.h"
#include "cprod.h"
#include "mymotor.h"
#include "mycylin.h"
#include "mysensor.h"
#include "cSortCT.h"
#include "note.h"
#include "main.h"
#include "csystem.h"
#include "uhome.h"
#include "mymessbox.h"
#include "cinitial.h"
#include "atester.h"
#include "aRotateKIT.h"
#include "fRotate.h"
#include "cOffSet.h"
#include "cMyDB.h"
#include "cObserver.h"
#include "cmydef.h"
#include "uYieldMonitoring.h"
#include "BarCode.h"
#include "InOutArmZteach.h"
#include "acarry.h"
#pragma package(smart_init)
//==============================================================================
void SetInOutArmParameter_1x1_1()
{
    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , i, j, i, j);
//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , i, j, i, j);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, i, j, i, j);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, i, j, i, j);
        }
    }

    if(TestIF_File.bSingleUseOtherSuck)
    {
        if(USE_PICKER_COUNT==ep16Picker)
        {
//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 2, 0, 0);
//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , 1, 2, 1, 0);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 4, 0, 0);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 1, 4, 1, 0);

//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 0, 0, 2);
//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , 1, 0, 1, 2);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 0, 0, 4);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 1, 0, 1, 4);
        }
        else
        {
//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 1, 0, 0);
//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , 1, 1, 1, 0);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 2, 0, 0);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 1, 2, 1, 0);

//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 0, 0, 1);
//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , 1, 0, 1, 1);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 0, 0, 2);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 1, 0, 1, 2);
        }
    }
    else if(Prod.bSingleInArmUseOtherSuck)                                      //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
    {
        if(USE_PICKER_COUNT==ep16Picker)
        {
        }
        else
        {
//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 1, 0, 0);
//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , 1, 1, 1, 0);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 0, 0, 3);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 3, 0, 0);
        }
    }
    else if(iInArmType==e9045_1x1_1)
    {
        if(USE_PICKER_COUNT==ep16Picker ||
           USE_PICKER_COUNT==ep1Picker)
        {
        }
        else
        {
//            CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 0, 0, 0);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 0, 0, 3);
            CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 3, 0, 0);
        }
    }
//    else if(iInArmType==e9045_1x4_1_Ac)
//    {
////        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 0, 0, 2);
////        CopyInitSuck(&InArmSuckBackup , &InArmSuck , 0, 2, 0, 0);
//        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 0, 0, 2);
//        CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, 0, 2, 0, 0);
//    }
    InArmSuck .SetPickerCount(1, 1, 1, 1, 1, 0, 0);
    OutArmSuck.SetPickerCount(1, 1, 1, 1, 1, 0, 0);
}
//==============================================================================
//個位數為Y座標, 2=標準, 0=A排, 1=B排
//百位數以上為X座標, 分配方式如下
//                    0                     =標準
//         100        |         101         =左右分兩次
int GetNowSiteKitMode_1x1_1(int iSht, bool bPlace)
{
    if(iInArmType==e9045_1x4_1_Ac)
    {
        if(TestIF_File.bSingleUseOtherSuck==false &&                            //wei 20220823 Single Site使用C吸嘴判斷
           TestIF_File.bSingleInArmUseOtherSuck==false)                         //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
        {
            return 20000;
        }
        else
        {
            return 30000;
        }
    }
    else
    {
        if(TestIF_File.bSingleUseOtherSuck==false &&                            //wei 20220823 Single Site使用C吸嘴判斷
           TestIF_File.bSingleInArmUseOtherSuck==false)                         //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
        {
            return 0;
        }
        else
        {
            return 10000;
        }
    }
}
//==============================================================================
void InArmZNeedDown_1x1_1(int iSht, bool bPlace)
{
    if(InitialInArmNeedSuck(iSht, bPlace)==false)
        return;

    int iSuckRow;                                                               //, iSuckCol, iShtCol;
    int iMode   =GetNowSiteKitMode_1x1_1(iSht, bPlace);
    int iModeRow=iMode%100;
    int iModeCol=iMode/100;

    iSuckRow=(TestIF.iAutoClean_Function && bUse8Picker==false && bRunAutoClean)?1:0;

    if(iModeCol==0)
    {
        SetInArmNeedDestory(bPlace, iModeRow, 0, iSuckRow, 0);
    }
    else if(iModeCol==100)
    {
        SetInArmNeedDestory(bPlace, iModeRow, 0, iSuckRow, 1);
    }
    else if(iModeCol==200)
    {
        SetInArmNeedDestory(bPlace, iModeRow, 2, iSuckRow, 0);
    }
    else
    {
        SetInArmNeedDestory(bPlace, iModeRow, 2, iSuckRow, 1);
    }
}
//==============================================================================
void CheckXYPitch_1x1_1(int *iX, int *iY, int iSht, bool bPlace, int iMovePitchX, int iMovePitchY)
{
    int iMode       =GetNowSiteKitMode_1x1_1(iSht, bPlace);
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

    if(iModeRow==0)                                                             //Aa to Row A
    {
        if(USE_IN_Y_IS_AUTO_PITCH==true)                                        //JerryYang 20251218 : IN/OUT ARM支援不同模組
            *iY=*iY-iMovePitchY;
    }
    else if(iModeRow==1)                                                        //Ba to Row A
    {
        if(USE_IN_Y_IS_AUTO_PITCH==true)                                        //JerryYang 20251218 : IN/OUT ARM支援不同模組
            ;
        else
            *iY=*iY+iMovePitchY;
    }

    if(USE_PICKER_COUNT==ep1Picker)
    {
    }
    else if(USE_PICKER_COUNT==ep16Picker)                                       //基準為第四隻吸嘴
    {
        dMovePitchX=double(iMovePitchX)/7.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*3.0);
        }
        else if(iModeCol==100)                                                  //Ac --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0);
        }
        else if(iModeCol==200)                                                  //Aa --> Ac
        {
            *iX=*iX+(dMovePitchX*3.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==300)                                                  //Ac --> Ac
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchBb)                                 //基準為第二隻吸嘴 //Steven for HT7080
    {
        dMovePitchX=double(iMovePitchX)/3.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0);
        }
        else if(iModeCol==100)                                                  //Ab --> Aa
        {
            *iX=*iX+(dMovePitchX*0.0);
        }
        else if(iModeCol==200)                                                  //Aa --> Ac
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==300)                                                  //Ab --> Ac
        {
            *iX=*iX+(dMovePitchX*0.0+TestIF.dSiteXPitch*0.5);
        }
    }
    else                                                                        //基準為第三隻吸嘴
    {
        dMovePitchX=double(iMovePitchX)/3.0;
        if(iModeCol==0)                                                         //Aa --> Aa
        {
            *iX=*iX+(dMovePitchX*2.0);
        }
        else if(iModeCol==100)                                                  //Ab --> Aa
        {
            *iX=*iX+(dMovePitchX*1.0);
        }
        else if(iModeCol==200)                                                  //Aa --> Ac
        {
            *iX=*iX+(dMovePitchX*2.0+TestIF.dSiteXPitch*0.5);
        }
        else if(iModeCol==300)                                                  //Ab --> Ac
        {
            *iX=*iX+(dMovePitchX*1.0+TestIF.dSiteXPitch*0.5);
        }
    }

    *iY=*iY+GetInArmYToShuttleOffset_9045(iSht, iOffsetPos);
    *iX=*iX+GetInArmXToShuttleOffset_9045(iSht, iOffsetPos);
    TransferInShuttleRatio(iSht, iX, iY, iInArmYBase, iInArmXBase);
    InArmZNeedDown_9045(iSht, 0, bPlace);
}
//==============================================================================
// in arm x y to shuttle position
//==============================================================================
bool MoveInArm2XYToShuttle_9045_1x1_1(int iSht, bool IncludeZ, bool bPlace)
{
    int iXPos               =iInArmShtXCenterPos;                               //Steven 20141029 : XY-Pitch for Shuttle Center position for base Suck
    int iYPos               =iInArmShtYCenterPos;
    int iMode               =GetNowSiteKitMode_1x1_1(iSht, bPlace);
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
        iMovePitchX=iXpitchMinX7;                                               //Steven 20240918 : iXpitchMaxX3 --> iXpitchMinX3
    }
    else
    {
        iMovePitchX=10000;                                                      //Steven 20240918 : HT9045行程不足, iXpitchMaxX3 --> 10000
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iInXPToSht[i]=GetInArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    if(TestIF.bNS7000kit)                                                       //jou 981208 start : NS7000 bias kit
    {
        if(CosFunction.bNonCenterModeCanUseShtOffset && IniConfig.bA50Enable1x4BiasYOffset && TestIF_File.i1x4SiteYOffset==1)                                   //Steven 20200715 : for Tinton
        {
            if(iSht==0)
                iYPos+=1500;
            else
                iYPos-=1500;
        }
        else
        {
            if(iSht==0)
                iYPos+=iMovePitchY/2;
            else
                iYPos-=iMovePitchY/2;
        }
    }

    CheckXYPitch_1x1_1(&iXPos, &iYPos, iSht, bPlace, iMovePitchX, iMovePitchY);
    GetInArmZShtDownPos_9045(iSht, bPlace, IncludeZ);

    if(bRunAutoClean &&
       TestIF.iAutoClean_Function)
    {
        if(bUse8Picker==false)                                                  //Steven 20201014 : 整合8吸嘴auto clean
        {
            if(iShuttleRowKit==1 || iShuttleRowKit==3)
                iYPos+=iMovePitchY;
        }

        if(iSht==0)                                                             //Steven 20241102 : fixed for Auto Clean offset
        {
            iXPos=iXPos+TestIF_File.iAutoClean_Shuttle1XOffset;
            iYPos=iYPos+TestIF_File.iAutoClean_Shuttle1YOffset;
        }
        else
        {
            iXPos=iXPos+TestIF_File.iAutoClean_Shuttle2XOffset;
            iYPos=iYPos+TestIF_File.iAutoClean_Shuttle2YOffset;
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
//==============================================================================
bool DoPlaceToHotPlate_9045_1x1_1()
{
    int &Task=iInArmPlaceToHotPlateTask;
    int ip, ix, iy;

    bool flag=true;
    int iSuckRow=0, iSuckCol=0;

    if(TestIF_File.bSingleUseOtherSuck ||                                       //wei 20220823 Single Site使用C吸嘴判斷
       TestIF_File.bSingleInArmUseOtherSuck)                                    //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
    {
        iSuckCol=1;
    }

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
            if(InArmSuck.Item[iSuckRow][iSuckCol]==HAS_IC ||
               InArmSuck.Item[iSuckRow][iSuckCol]==HAS_NULL_IC)
            {
                if(InArmSuck.Item[iSuckRow][iSuckCol]==HAS_NULL_IC ||
                   InArmSuck.Suck[iSuckRow][iSuckCol].Destroy())
                {
                    ip=iPlacePlate[0];
                    ix=iPlacePlateX[0];
                    iy=iPlacePlateY[0];
                    DoPlaceToHPSwapData(iSuckRow, iSuckCol, ip, iy, ix);        //Steven 20170109 : 將放料到HP資料交換改成Function
                }
                else if(InArmSuck.Suck[iSuckRow][iSuckCol].Error==false)        //kevin 20141206
                {
                    flag=false;
                }
            }

            if(flag==false)
                return false;

            if(InArmSuck.Item[iSuckRow][iSuckCol]!=NULL_IC)
                return false;

            bPlaceToHotplatePartOK=true;                                        //ChungHung 20120412 add
            Task=400;
            break;
        case 400:
            if(MoveInArmZToPlateSafe(Task))
            {
                if(USE_PICKER_COUNT==ep1Picker)
                {
                }
                else if((Prod.fInArmSuck4x8[0][iSuckRow][iSuckCol] ||
                         Prod.fInArmSuck4x8[1][iSuckRow][iSuckCol]) &&
                         InArmSuck.Item[iSuckRow][iSuckCol]!=NULL_IC)           //ChungHung 20130910 alter for SCK can close site by Index
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

            if(USE_PICKER_COUNT==ep1Picker)
            {
            }
            else
            {
                AdjustShuttleWhichKitOrder();                                   //Steven 20160201 : 修正Shuttle放料順序
            }

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
bool DoInArmPlaceToShuttle_9045_1x1_1()
{
    IN_ARM_SHUTTLE_1x1_1:

    QueueTaskList[4].CheckTaskChange();                                         //Steven 20200821 : 使用Goto也要記錄Task變化
    int ret;                                                                    //AI(ht9045-v899) 20260407: add for CheckInArmDestroyICFail return value
    int &Task=iInArmPlaceToShuttleTask, iFlag=0;
    int iSuckRow=0, iSuckCol=0, iShtRow=0, iShtCol=0;
    bool flag, bCanFreeShuttle=false;

    if(TestIF_File.bSingleUseOtherSuck==true ||                                 //wei 20220823 Single Site使用C吸嘴判斷
       TestIF_File.bSingleInArmUseOtherSuck==true)                              //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
    {
        iSuckCol=1;
    }

    static bool bCheckSpeed=false;
    AnsiString ErrPart="", str="";
    static bool bCheckSpeed1=false;                                             //KaiChen 20171225 (Steven)：Add Speed Display
    if(bIndexPickUpErrorWaitRetry)                                              //Ifor 20171119 : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
        return false;

    int iSht=0;                                                                 //, iKit=0;
    if(LastSet.iRunStartMode==rsmAutoSiteMap &&
       (CosFunction.bUSEJCETSiteMapMode==true ||                                //Steven 20231113 : 修正 auto site mapping 2x6 hangup
        CUSTOMER_CODE==CC_SCS))                                                 //jou 20230626 : 修正JSCS auto site mapping 2x8 hangup
    {
        iSht=iAutoSiteMapHPToSht;
    }
    else if(LastSet.iTemperature==Tempture_Hot &&                               //Steven 20160227 : 改成HOT在上面
            LastSet.iRunStartMode!=rsmAutoSiteMap &&                            //kevin 20161124 add hot AutoSitmap hang up
            fContact->IsRun2DCheck()==false)                                    //JerryYang 20250428 : fix 2DID map
    {
        if(LastSet.iTemperature==Tempture_Hot &&
           LastSet.iRunStartMode==rsmQAMode &&
           bQAModeFinishCleanOut==true)                                         //Steven 20180601 : 修正高溫QA mode放料
        {
            iSht=InArmSuck.iWhichSht;
        }
        else
        {
            iSht=InArmSuck.iWhichShtPickFor32;
        }
    }
    else
    {
        iSht=InArmSuck.iWhichSht;
    }

    if(iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
        iShtCol=2;

    switch(Task)
    {
        case 1:
            bDestoryOnSht=false;                                                //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            InArmXMoveSafe=false;

            if(InArmSuck.Item[iSuckRow][iSuckCol]==NULL_IC)
                InArmSuck.SetItemData(iSuckRow, iSuckCol, HAS_NULL_IC);         //Steven 20140710 : Add

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
            goto IN_ARM_SHUTTLE_1x1_1;
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
                    Task=900;
                    goto IN_ARM_SHUTTLE_1x1_1;
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
            //AI(ht9045-v899) 20260407: align with 2x4_8 baseline -- add bCheckInarmPlaceShuttle + CheckInArmDestroyICFail
            bCheckInarmPlaceShuttle=true;                                       //kevin 20180525 20180315 add inarm place shuttle
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

            if(flag==false)                                                                                             //kevin 20131011 在下真空誤判, 換到上面來
                break;

            ret=CheckInArmDestroyICFail();                                      //Steven 20111223 : check destroy error
            if(ret==false)
                return false;

            if(InArmSuck.Item[iSuckRow][iSuckCol] &&
               InArmSuck.Suck[iSuckRow][iSuckCol].GetNeedDestroyStatus())
                return false;

            bCheckInarmPlaceShuttle=false;                                      //kevin 20180525 20180315 add inarm place shuttle
            if(InArmSuck.HasRealIC()==false)
            {
                Task=1300;
                fObserver->AddTimeData(2, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);                               //JerryYang 20151209
            }
            else
            {
                Task=1250;
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

            if(InArmSideAllClose(1)==true)                                      //如果Arm 2的Site全部被關掉
            {
                SetShuttleToHasNullIC_9045(1);
            }

            MOT[MInShuttle1].fCanMoveL=true;

            AdjustShtOrderWhenPlaceToSht(1);                                    //Steven 20180601 : 整合放完蝦頭後的調整

            fObserver->RecordInArmTime();
            bDestoryOnSht=false;                                                //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            iInArmPickPlaceCnt[InOfsInSh1]++;                                   //JerryYang 20180921 Setup Teach功能
            if(USE_IN_Y_IS_AUTO_PITCH==true &&                                  //JerryYang 20251218 : IN/OUT ARM支援不同模組
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
            if(InSHT2InLF() &&
               BLCarryKit.UseSiteFullIC()==false)
            {
                if(bCheckSpeed)                                                                                         //Steven 20110525 : Auto Speed
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
            if(bCheckSpeed)                                                                                             //Steven 20110525 : Auto Speed
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
            SetShuttlefCanMoveL(1, false, __FUNC__, "1950");
            if(MoveInArmXYToShuttle_9045(iSht, 0, ZAxisDown, true))
            {
                Task=2100;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                                                        //Steven 20110516 : 修改成整合式Alarm
            {
                Task=1955;                                                                                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 1955:
            CheckInArmSuckICFallDownToHasNullIC(true);                                                                  //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
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
                bCanFreeShuttle=true;                                                                                   //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
                for(int i=0; i<InArmSuck.iShtRow; i++)
                {
                    for(int j=0; j<InArmSuck.iShtCol; j++)
                    {
                        if(BLCarryKit.Item[i][j]==NULL_IC)
                        {
                            bCanFreeShuttle=false;                                                                      //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
                        }
                    }
                }

                if(bPlaceToShuttle2Step)                                                                                //Steven 20160721 (Wei) : 避免放一半歸零,會Hangup
                    bCanFreeShuttle=false;

                if(bCanFreeShuttle==false)
                {
                    if(bShuttle2HasPickErr==false)                                                                      //Sam 20230103 : 修正 32 Site Pickup error hange up
                        SetShuttlefCanMoveL(1, false, __FUNC__, "2000");
                    InArmZNeedDown_9045(1, 0, true);
                    Task=1900;
                    goto IN_ARM_SHUTTLE_1x1_1;
                }
            }
            else if(NeedWaitTrayArm &&
                    (IniConfig.bP56TrayArmWaitAtColorTrack ||
                    (INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)))                                               //Steven 20240516 : Tray Arm等待位置改到Color        //wei 20150925 待機位置改道 Color
            {
                InitInOCRWaitTask();                                                                                    //wei 20170901 Place To Shuttle
                Task=2030;
            }

            if(MOT[MInShuttle2].Led[iInposLed]==false &&
               MOT[MInShuttle2].fCanMoveL==false)
            {
                bCanFreeShuttle=true;
            }

            if(bCanFreeShuttle==true)                                                                                   //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            {
                Task=2050;
            }
            break;
        case 2030:
            if(OCRMoveInArm2XYToWait())                                                                                 //wei 20170901 Place To Shuttle
            {
                Task=1;
            }
            break;
        case 2050:                                                                                                      //jou 2014-11-07 防止 16 site place to shuttle Z軸未上升 Shuttle 就移動撞機
            if(MoveInArmZToPlateSafe(Task))
            {
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

            SetShuttlefCanMoveL(1, false, __FUNC__, "2100");
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
                if(ArmSpeed[InArm].iEnableReleaseDelay==0)                                                              //JerryYang 20160127 for TSMC inarm release device前delay
                {
                    InArmReleaseDelay.SetSecAndOn(ArmSpeed[InArm].dReleaseDelayTime);
                    Task=2180;
                }
            }
            break;
        case 2110:
            if(MoveInArmZToPlateSafe(Task))
            {
                bDestoryOnSht=false;                                                                                    //Steven 20180419 (Jou) : 在Shuttle吹氣與資料交換的Flag
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
            if(InArmReleaseDelay.Off())                                                                                 //JerryYang 20160127 for TSMC inarm release device前delay
            {
                Task=2200;
            }
            break;
        case 2200:
            flag=true;
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

            if(flag==false)                                                                                             //kevin 20131011 在下真空誤判, 換到上面來
                break;

            //AI(ht9045-v899) 20260407: align with 2x4_8 baseline -- add CheckInArmDestroyICFail for SHT2
            ret=CheckInArmDestroyICFail();                                      //Steven 20111223 : check destroy error
            if(ret==false)
                return false;

            if(InArmSuck.Item[iSuckRow][iSuckCol] &&
               InArmSuck.Suck[iSuckRow][iSuckCol].GetNeedDestroyStatus())
                return false;

            if(InArmSuck.HasRealIC()==false)
            {
                Task=2300;
                fObserver->AddTimeData(2, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);                               //JerryYang 20151209
            }
            else
            {
                Task=2250;
            }
            break;
        case 2250:
            if(MoveInArmXYToShuttle_9045(iSht, 0, ZAxisDown, true))
            {
                Task=2100;
            }

            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                                                        //Steven 20110516 : 修改成整合式Alarm
            {
                Task=2255;                                                                                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 2255:
            CheckInArmSuckICFallDownToHasNullIC(true);                                                                  //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            if(MoveInArmZToPlateSafe(Task))
            {
                str.sprintf("Please take out the drop device from Shuttle%d", iSht+1);
                ShowMyMessage(str);
                Task=2250;
            }
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
            SetShuttleToHasNullIC_9045(1);

            if(InArmSideAllClose(0)==true)                                                                              //如果Arm 2的Site全部被關掉
            {
                SetShuttleToHasNullIC_9045(0);
            }

            MOT[MInShuttle2].fCanMoveL=true;

            AdjustShtOrderWhenPlaceToSht(2);                                                                            //Steven 20180601 : 整合放完蝦頭後的調整

            fObserver->RecordInArmTime();
            bDestoryOnSht=false;                                                                                        //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
            iInArmPickPlaceCnt[InOfsInSh2]++;                                                                           //JerryYang 20180921 Setup Teach功能
            if(USE_IN_Y_IS_AUTO_PITCH==true &&                                                                          //JerryYang 20251218 : IN/OUT ARM支援不同模組
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
//==============================================================================
void DoInArm_9045_1x1_1_SuckerMap()
{
    int iSuckRow=0, iSuckCol=0, iOutSCol=3;
    if(TestIF_File.bSingleUseOtherSuck ||                                       //wei 20220823 Single Site使用C吸嘴判斷
       TestIF_File.bSingleInArmUseOtherSuck)                                    //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
    {
        iSuckCol=1;
    }

    if(Prod.bSingleUseOtherSuck)                                                //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
    {
        iOutSCol=2;
    }

    int iShtCol=GetShuttleCol(0, 0);

    if(iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        Prod.fInArmSuck4x8[0][iSuckRow][iSuckCol]=LastSet.bUseTestSocket[0][0][2];
        Prod.fInArmSuck4x8[1][iSuckRow][iSuckCol]=LastSet.bUseTestSocket[1][0][2];
        Prod.bInSuckUse[0][iSuckRow][iSuckCol]   =true;
        Prod.bInSuckUse[1][iSuckRow][iSuckCol]   =true;
        Prod.iSiteMap[0][iSuckRow][iSuckCol]     =TestIF.iSiteMap[0][2];
        Prod.iSiteMap[1][iSuckRow][iSuckCol]     =TestIF.iSiteMap[0][2];

        if(USE_ROTATE_KIT==1 &&                                                 //Steven 20250801 : for rotator
           tRotate.ActiveRotate)
        {
            if(iRotate_Type==e2MotRotate2Dut ||
               iRotate_Type==e1MotRotate1Dut)
            {
                if(CosFunction.bRotateUseRTmode==true && tRotate.bRotateUseRTmode==true && Prod.bRunRT==true)           //RT
                {
                    Prod.RotateDutDate[0][iSuckRow][iSuckCol]=tRotate.RotateDutDate[2][0][iShtCol];
                    Prod.RotateDutDate[1][iSuckRow][iSuckCol]=tRotate.RotateDutDate[3][0][iShtCol];
                }
                else
                {
                    Prod.RotateDutDate[0][iSuckRow][iSuckCol]=tRotate.RotateDutDate[0][0][iShtCol];
                    Prod.RotateDutDate[1][iSuckRow][iSuckCol]=tRotate.RotateDutDate[1][0][iShtCol];
                }
            }
        }
    }
    else
    {
        Prod.fInArmSuck4x8[0][iSuckRow][iSuckCol]=LastSet.bUseTestSocket[0][0][0];
        Prod.fInArmSuck4x8[1][iSuckRow][iSuckCol]=LastSet.bUseTestSocket[1][0][0];
        Prod.bInSuckUse[0][iSuckRow][iSuckCol]   =true;
        Prod.bInSuckUse[1][iSuckRow][iSuckCol]   =true;
        Prod.iSiteMap[0][iSuckRow][iSuckCol]     =TestIF.iSiteMap[0][0];
        Prod.iSiteMap[1][iSuckRow][iSuckCol]     =TestIF.iSiteMap[0][0];

        if(USE_ROTATE_KIT==1 &&                                                 //Steven 20250801 : for rotator
           tRotate.ActiveRotate)
        {
            if(iRotate_Type==e2MotRotate2Dut ||
               iRotate_Type==e1MotRotate1Dut)
            {
                if(CosFunction.bRotateUseRTmode==true && tRotate.bRotateUseRTmode==true && Prod.bRunRT==true)           //RT
                {
                    Prod.RotateDutDate[0][iSuckRow][iSuckCol]=tRotate.RotateDutDate[2][0][iShtCol];
                    Prod.RotateDutDate[1][iSuckRow][iOutSCol]=tRotate.RotateDutDate[3][0][iShtCol];
                }
                else
                {
                    Prod.RotateDutDate[0][iSuckRow][iSuckCol]=tRotate.RotateDutDate[0][0][iShtCol];
                    Prod.RotateDutDate[1][iSuckRow][iOutSCol]=tRotate.RotateDutDate[1][0][iShtCol];
                }
            }
        }
    }
}
//==============================================================================
bool DoInArmTryPickFromHotPlate_9045_1Pick(bool bOneTimeCheckAll, bool bShowErrorMessage)                               //Ifor 20260115 add: Single Site Hotplate Check
{
    IN_ARM_TRY_PICK_HOTPLATE_1X1:
    int &Task=iInArmTryPickFromHotPlateTask;
    int ip=iPlacePlate[0], ix=iPlacePlateX[0], iy=iPlacePlateY[0];
//    int StartX=0, iStepX=0;
//    int i, j, j2;
    AnsiString ErrPart="";
    bool bHasDuplicateErr;
    bool bHasError;
//    static bool bSuckDuplicateErr[2][4]={{false, false, false, false}, {false, false, false, false}};
    static bool bSuckFinish[2][4]={{false, false, false, false}, {false, false, false, false}};
    int iSuckCol=0;

    GetHotPlateYHalfPos();

    if(TestIF_File.bSingleUseOtherSuck ||                                       //wei 20220823 Single Site使用C吸嘴判斷
       TestIF_File.bSingleInArmUseOtherSuck)                                    //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
    {
        iSuckCol=1;
    }
    else
    {
        iSuckCol=0;
    }

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
            if(InArmSuck.Item[0][iSuckCol]==HAS_TRY_SUCK_IC)
            {
                ix=iPlacePlateX[0];
                ip=iPlacePlate[0];
                iy=iPlacePlateY[0];

//                if(InArmSuck.Item[0][0]==HAS_NULL_IC || InArmSuck.Suck[0][0].Suck() || InArmSuck.Suck[0][0].Error)
                if(InArmSuck.Suck[0][iSuckCol].Suck() || InArmSuck.Suck[0][iSuckCol].Error)
                {
                    MOT[MMPlate1+ip].SetTraySingleData(ix, iy, HAS_TRY_SUCK_IC);

                    #ifdef SOFT_SIMULTE
                        InArmSuck.SetItemData(0, iSuckCol, NULL_IC);
                    #else
                        if(InArmSuck.Item[0][iSuckCol]==HAS_TRY_SUCK_IC && InArmSuck.Suck[0][iSuckCol].Error==false && LastSet.iRealDummy==REALLY)
                        {
                            if(bHPCleanout)                                     //wei 20160624 Hotplate clean out
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
                            InArmSuck.SetItemData(0, iSuckCol, HAS_SUCK_IC);
                        }
                        else
                        {
                            InArmSuck.SetItemData(0, iSuckCol, NULL_IC);
                        }
                    #endif
                }
            }
            else if(InArmSuck.Item[0][iSuckCol]==HAS_NULL_IC)
            {
                ix=iPlacePlateX[0];
                ip=iPlacePlate[0];
                iy=iPlacePlateY[0];

                MOT[MMPlate1+ip].SetTraySingleData(ix, iy, HAS_TRY_SUCK_IC);
                InArmSuck.SetItemData(0, iSuckCol, NULL_IC);
            }

            if(InArmSuck.Item[0][iSuckCol]==HAS_TRY_SUCK_IC)
                return false;

            Task=400;
            goto IN_ARM_TRY_PICK_HOTPLATE_1X1;
        case 400:
            if(InArmSuck.Item[0][iSuckCol]==HAS_TRY_SUCK_IC)
            {
                Task=1;
                break;
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

                    InArmSuck.Suck[0][iSuckCol].Error=false;
                    bSuckFinish[0][iSuckCol]=false;
                    if(InArmSuck.Item[0][iSuckCol]==HAS_SUCK_IC)
                    {
                        ErrPart+=InArmSuck.Suck[0][iSuckCol].sName;
                        bHasError=true;
                    }

                    if(bTryPickHPDuplicateErr[0][iSuckCol])
                    {
                        bHasDuplicateErr=true;
                    }

                    if(bHasError)
                    {
                        ShowErrorMessage("WAR0170", K_RETRY, MInArmX, bHasDuplicateErr, ErrPart);
                        bTryPickHPDuplicateErr[0][iSuckCol]=true;
                        Task=450;
                    }
                    else
                    {
                        bTryPickHPDuplicateErr[0][iSuckCol]=false;
                        Task=500;
                    }
                }
                else
                {
                    if(bOneTimeCheckAll)
                    {
                        if(InArmSuck.HasType(HAS_SUCK_IC))
                        {
                            if(InArmSuck.Item[0][iSuckCol]==HAS_SUCK_IC)
                            {
                                InArmSuck.SetItemData(0, iSuckCol, HAS_HOT_IC);
                            }
                            else
                            {
                                InArmSuck.SetItemData(0, iSuckCol, HAS_NULL_IC);
                            }
                        }
                    }
                    Task=500;
                }
            }
            break;
        case 450:
            if(ArmSpeed_File[InArm].bUseHPVacuum)                               //Steven 20180125 (Jou) : 加熱盤的真空等待時間
                InArmSuck.Suck[0][iSuckCol].OnDelayTime=ArmSpeed[InArm].dHPVacuumTI*100;
            if((InArmSuck.Suck[0][iSuckCol].Suck() || InArmSuck.Suck[0][iSuckCol].Error) && bSuckFinish[0][iSuckCol]==false)
            {
                bSuckFinish[0][iSuckCol]=true;
                if(InArmSuck.Suck[0][iSuckCol].Error==false && LastSet.iRealDummy==REALLY)
                {
                    InArmSuck.SetItemData(0, iSuckCol, HAS_SUCK_IC);
                }
                else
                {
                    InArmSuck.SetItemData(0, iSuckCol, NULL_IC);
                }
            }

            if(bSuckFinish[0][iSuckCol])
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

                    if(InArmSuck.Item[0][iSuckCol]==NULL_IC)
                        InArmSuck.SetItemData(0, iSuckCol, HAS_TRY_SUCK_IC);
                    Task=1;
                }
            }
            break;
        case 500:
            if(bOneTimeCheckAll==true)
                AdjustShuttleWhichKitOrder();                                   //Steven 20220520 : 修正Try pick HP

            if(IsHotPlateCheckFinsih())
            {
                if(bOneTimeCheckAll==true)
                {
                    bNeedTrySuckHotPlate=false;
                    TryHotClearRecBufMMPlate();

                    if(bHotPlateCheckNeedTrayFeed==true)
                    {
                        bTrySuckHotPlateCleanOut=true;
                        fMain->DebugOneCycleHotPlate("DoInArmTryPickFromHotPlate_9045_2x8_8 500");
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
//==============================================================================
void DoInArm_9045_1x1_1()
{
    static int iDisplaySpeed=0;                                                 //KaiChen 20171225 (Steven)：Add Speed Display
    static int iAdjustSpeed=0;
    static bool bOne=false;

    int &Task=iArmTask;
    int iSuckRow=0, iSuckCol=0;
    bool flag;

    if(TestIF_File.bSingleUseOtherSuck ||                                       //wei 20220823 Single Site使用C吸嘴判斷
       TestIF_File.bSingleInArmUseOtherSuck)                                    //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
    {
        iSuckCol=1;
    }

    if(DoInArmAutoSiteMapping() ||                                              //jou 2011-03-24 start : Auto Site Mapping
       bIndexAlarmInArmAway)                                                    //kevin 20181102 (Steven) :  index回吸檢測有IC inarm 讓位 inarm先不要動
    {
        return ;                                                                //jou 2016-11-03 JCET 要求Auto Site Mapping 需等待測試結果在繼續入料
    }

    int iShtHP;                                                                 //, iKitHP;
    int iSht  =InArmSuck.iWhichSht;
    int iKit  =0;
//    int iKit32=(iKit==0)?0:InArmSuck.iPickKitStep;

    if(LastSet.iTemperature==Tempture_Hot)                                      //Steven 20150626 : Fixed 32 site 吸取偵測異常    //Steven 20180525 : 修正一直跑Hotplate iWhichKit Close
    {
        iShtHP=InArmSuck.iWhichShtPickFor32;
//        iKitHP=0;
    }
    else
    {
        iShtHP=InArmSuck.iWhichSht;
//        iKitHP=0;
    }
//    int iKitHP32=(iKitHP==0)?0:InArmSuck.iPickKitStep;

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
                        Task=1100;                                              //Steven 20250107 : Move up
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
            Task=75;                                                            //Ifor 20260115 add: Hotplate check 100 => 75

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
                        Task=75;                                                //Ifor 20260115 add: Hotplate check 100 => 75
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
                        Task=75;                                                //Ifor 20260115 add: Hotplate check 100 => 75
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

            if(Task!=75)                                                        //Steven 20180813 : add in arm speed
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
            Task=400;
        case 400:
            flag=DoInArmAdditionalFunction();                                   //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
            if(flag==true)
            {
                SetInArmUseSuckToHasNullIC(iSht, iKit);
                InitInArmPlaceToHotPlateTask();
                InitInArmPlaceToShuttleTask();

                if(LastSet.iTemperature==Tempture_Hot)                          //kevin 20190621 change
                    Task=1000;                                                  // place to hot plate
                else
                    Task=2000;                                                  // place to shuttle
            }
            break;
        case 500:
            if((MOT[MMPlate1].HasIC() ||                                        //Steven 20170511 (wei) : 使用initial delay當 Soak time
                MOT[MMPlate2].HasIC()) &&
               HasHotReadyIC_9045())
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

                if(CheckHasSpaceToPlace_9045()==false)
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

            if(Task!=1550)
                break;
        case 1550:
            bOneTimeWait=false;
            bPlaceToHotplate=false;

            if(IniConfig.bI23HotTestWaitingMode && bChangeToInitStartMode)                                              //Chunghung 20111230 Hot Test Waiting Mode
            {
                bChangeToInitStartMode=false;
                ShowMyMessage("Tester Ready?", "", "", true);
            }

            if(DoInArmPickFromHotPlate_9045())
            {
                bPickFromHotplate=false;
                if(InArmSuck.HasIC()==false)
                {
                    Task=1;
                    break;
                }
                else                                                                                                    //Steven 20190508 : 改到下面, 避免HP異常hang up一直被重置
                {
                    bHangTimePause=true;                                                                                //Steven 20090827 : Hang Up dectector
                }

                if(Prod.fInArmSuck4x8[iShtHP][iSuckRow][iSuckCol]==false &&
                   InArmSuck.Item[iSuckRow][iSuckCol]==HAS_HOT_IC)                                                      //ChungHung 20130910 alter for SCK can close site by Index
                {
                    ShowErrorMessage("WAR0153", 0, MInArmX, 0, "DoInArm_9045_1x1_1");                                   //加熱盤的資料錯誤
                }

                InitInArmPlaceToShuttleTask();
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
                iInRotateFinish=0;                                              //kevin 20130524 放完SHUTTLE
                iSetShuttleToHasNullIC=0;
                bHangTimePause=true;                                            //Steven 20090827 : Hang Up dectector
                if(LastSet.iRealDummy==DUMMY && iCleanOut)
                {
                    MOT[MMTrayY].InitNewTray(NULL_IC, false, __FUNC__);
                }

                HasHotReadyIC_9045();                                           //Steven 20220530 : Add

                if(IniConfig.bA15_1ESDGiveWayFunction==true)                    //==> Eastsun 20260521 整合
                {
                    InitDoInArmIonFanGiveWayTask();
                    Task=2100;
                }
                else
                {
                    Task=50;
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

            if(DoInArmTryPickFromHotPlate_9045_1Pick(bOneTimeHotPlateCheckAll, bTryPickFromHotPlateShowError))
            {
                if(TestIF_File.bSingleUseOtherSuck ||                           //wei 20220823 Single Site使用C吸嘴判斷
                   TestIF_File.bSingleInArmUseOtherSuck)                        //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
                {
                    if(InArmSuck.Item[0][1]==HAS_TRY_SUCK_IC)
                        InArmSuck.SetItemData(0, 1, NULL_IC);
                }
                else
                {
                    if(InArmSuck.Item[0][0]==HAS_TRY_SUCK_IC)
                        InArmSuck.SetItemData(0, 0, NULL_IC);
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
int GetShuttleState_1x1_1(int iSht, bool bPick)
{
    if(bPick==bAutoPick)                                                        //Pick
    {
        if(ptrInSHT->ArmUpSideAllTypeIC(NULL_IC, 0, 1)==false)                  //左上有料
            return 1;
        else
            return 0;
    }
    else                                                                        //Place
    {
        if(bUse8Picker)
        {
            if(ptrInSHT->ArmUpSideAllTypeIC(NULL_IC, 0, 1) &&                   //左上無料
               ((bAutoCleanPlaceToSht==false && InArmSuck.ArmUpSideNoIC()) ||
                (bAutoCleanPlaceToSht==true  && InArmSuck.ArmUpSideNoIC()==false)))
                return 1;
            else
                return 0;
        }
        else
        {
            if(ptrInSHT->ArmUpSideAllTypeIC(NULL_IC, 0, 1) &&                   //左上無料
               ((bAutoCleanPlaceToSht==false && InArmSuck.ArmDownSideNoIC()) ||
                (bAutoCleanPlaceToSht==true  && InArmSuck.ArmDownSideNoIC()==false)))
                return 1;
            else
                return 0;
        }
    }
}
//==============================================================================
void SetInArmHasDropToShuttle_1x1_1(int SHT, int Row, int Col)
{
//    int iSuckX=0, iSuckY=0;

    if(SHT==0)
    {
        bDropAtSht1NeedCheckVac[0][0]=true;
    }
    else
    {
        bDropAtSht2NeedCheckVac[0][0]=true;
    }
}
//==============================================================================