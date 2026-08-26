//==============================================================================
// 9045  2x4 吸嘴組
// 支援  2x4  ( 每次 4 隻 都用)
//==============================================================================
#include "MachineDefine.h"
#pragma hdrstop

#include "ainarm9045.h"

#include "aArmHeader.h"

#include "cprod.h"
#include "mymotor.h"
#include "mykitsuck.h"
#include "mycylin.h"
#include "mysensor.h"
#include "csetup.h"
#include "note.h"
#include "main.h"
#include "csystem.h"
#include "uhome.h"
#include "mymessbox.h"
#include "cinitial.h"
#include "asendic.h"
#include "cmydef.h"
#include "cTrayMapping.h"
#include "cSetUp.h"
#include "aRotateKIT.h"
#include "BarCode.h"
#include "fRotate.h"
#include "cSortCT.h"
#include "cObserver.h"
#include "SCK_ART.h"
#include "uLotInfo.h"
#include "fVATMesFileSys.h"                                                     //jou 20200409 : VTest Mes system
#include "cSocket.h"
#include "acarry.h"
#include "cShowBinSelect.h"
#include "aRotateKIT_In.h"
#include "MyCCLinkSensor.h"                                                 //KenHsieh 20230829 : add Check In Shuttle Sensor By pass
#include "LtcSensor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//==============================================================================
//int iInArmXSafe=6000;
const int XDir=0;
const int YDir=1;
bool bInArmCheckDestroyACT[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false},
                                                      {false, false, false, false}};

bool bPickFormHotplatePartOK=false;                                             //ChungHung 2012012 add

bool bPickFormHotplateRetry=false;                                              //Ifor 20160616 add Hot plate Pickup error Retry

bool bInArmTryPickFromHotPlateFinish=true;                                      //ChungHung 20120206 Hotplate check

bool bInArmToPickHotPlatePos=false;                                             //ChungHung 20110323
int iTempSuckX=-1;
int iTempSuckY=-1;

//----------------------------------------------------------------------------
bool ArmYCanSuck2IC()
{
    if(DeviceForm.YDimension<=0)                                                //Sam 20250203 : 有讀取到參數後才能判斷
        return true;

    if(USE_IN_Y_IS_AUTO_PITCH==true)                                            //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(DeviceForm.YDimension>IN_OUT_ARM_Y_PITCH_MAX)
            return false;
    }
    else
    {
        if(DeviceForm.YDimension>=TestIF.iARM_Y_PITCH)
            return false;
    }
    return true;
}
//----------------------------------------------------------------------------
bool ArmXCanSuck2IC_9045S()
{
    if(DeviceForm.XDimension>=iXpitchMaxX2-250)                                 //超過7750就要減少吸嘴數量
        return false;

    return true;
}
//----------------------------------------------------------------------------
bool ArmXCanSuck4IC_9045()
{
    if(DeviceForm.XDimension>=iXpitchMax-250)                                   //超過3750就要減少吸嘴數量
        return false;

    if(TestIF_File.dSiteXPitch>iXpitchMaxX1_MM &&                               //X-Pitch=60
       (TestIF_File.dSiteXPitch/2.0)<=(DeviceForm_File.XDimension+2))           //IC 28.575 mm時, 要減少吸嘴數量
        return false;

    if((TestIF_File.iTestMode==QualSite1X4 ||                                   // 1x4
        TestIF_File.iTestMode==_8Site2X4N) &&                                   //Wei 20231211 : 2X4NN Mode
       LastSet.iTemperature==Tempture_Hot &&
       HotPlateForm_File.XPitch>40)                                             //Steven 20240308 : 1x4加熱模式, X-Pitch>40, 只能跑2吸嘴
    {
        return false;
    }

    if((TestIF_File.iTestMode==TriSite1X3  ||                                   //1x3 //Frank 20160329 add for 1x3_4
        TestIF_File.iTestMode==_6Site2X3N) &&                                   //Steven 20220425 : 2X3NN Mode
       TestIF_File.dSiteXPitch>iXpitchMinX3_MM)                                 //Steven 20240427 : fixed for 1x3
    {
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------
bool ArmXCanSuck4IC_1032()
{
    if(DeviceForm.XDimension>=iXpitchMaxX2-250)                                 //超過3750就要減少吸嘴數量
        return false;

    if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM &&                               //X-Pitch=60
       (TestIF_File.dSiteXPitch/2.0)<=(DeviceForm_File.XDimension+2))           //IC 28.575 mm時, 要減少吸嘴數量
        return false;

    return true;
}
//==============================================================================
// 取得判斷入料變 pitch 距離
//==============================================================================
int GetInArmPitch_9045(int w)
{
    double m,r;
    m=ChangeToFloatNonPcnt((double)(((double)(Prod.iInArmX120Pitch-Prod.iInArmX40Pitch))), (double)(iPitch_Max_minus_Min));
    r=(double)Prod.iInArmX40Pitch+m*((double)w-double(iXpitchMinX3));
    return (int)r;
}
//==============================================================================
int GetInArmPitchX_9045(int w, int iX, int iOffsetPos)                          //Steven 20230323 : For HT1032
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
int GetInArmPitch2_9045(int w)
{
    double m, r;
    m=ChangeToFloatNonPcnt((double)(((double)(Prod.iInArmX120Pitch2-Prod.iInArmX40Pitch2))), (double)(iPitch_Max_minus_Min));                                   //Steven 20131002 : XY變距        //Isaac 20171204 (Steven) : Xpitch40->50mm, 新增150mm的Teaching點位，內差法公式計算
    r=(double)Prod.iInArmX40Pitch2+m*((double)w-double(iXpitchMinX3));
    return (int)r;
}
//==============================================================================
int GetInArmPitchY_9045(int w, int iOffsetPos)                                  //ChungHung 20131231 alter AutoYPitch
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
int GetInArmXToShuttleOffset_9045(int iSht, int iOffsetPos)
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
int GetInArmYToShuttleOffset_9045(int iSht, int iOffsetPos)
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
int GetInArmToShuttleOffset_9045(int iSht, int iModeRow, int iModeCol, bool bPitch)
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
int GetInArmToLoaderOffset_9045(int iSelRow)
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
void SetShuttleToHasNullIC_9045(int iSht, int iKit)
{
    if(iKit==0)
    {
        for(int i=0; i<InArmSuck.iShtRow; i++)
        {
            for(int j=0; j<InArmSuck.iShtKitStep; j++)
            {
                if(iSht==0)
                {
                    if(FLCarryKit.Item[i][j]==NULL_IC)
                        FLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                }
                else
                {
                    if(BLCarryKit.Item[i][j]==NULL_IC)
                        BLCarryKit.SetItemData(i, j, HAS_NULL_IC);
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
                {
                    if(FLCarryKit.Item[i][j]==NULL_IC)
                        FLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                }
                else
                {
                    if(BLCarryKit.Item[i][j]==NULL_IC)
                        BLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                }
            }
        }
    }
}
//==============================================================================
bool SetClosedShtKitToHasNullIC_9045(int iSht, int iKit, AnsiString Func, AnsiString Task)                              //Steven 20231115 : fixed for auto site off
{
    int iAnotherSht=1-iSht;
    bool bFreeSht=false, bNeedAdjust2Time=false;
    if(iKit==0)
    {
        SetShuttleToHasNullIC_9045(iSht, 0);                                    //0是只塞一半

        if(IniConfig.bUseAutoSiteMapping &&
           LastSet.iRunStartMode==rsmAutoSiteMap &&
           IniConfig.bI21ASMRunTimeCHeck==false)                                //Steven 20140729 : AutoSiteMapping, 邊生產邊做)
        {
            SetShuttleToHasNullIC_9045(iSht);
            if(InArmSideAllCloseWithKit(iAnotherSht, 0)==true &&                //如果Arm 2的Site全部被關掉
               InArmSideAllCloseWithKit(iAnotherSht, 1)==true)                  //Steven 20241108 : fixed for auto site map and close arm.
            {
                SetShuttleToHasNullIC_9045(iAnotherSht);
            }
            bNeedAdjust2Time=true;
            bFreeSht=true;
        }
        else
        {
            if(InArmSideAllCloseWithKit(iSht, 1)==true)                         //jou 2013-08-14 修正關右邊八個site時,in arm 與 shuttle 撞機
            {
                SetShuttleToHasNullIC_9045(iSht);
                bNeedAdjust2Time=true;

                if(InArmSideAllCloseWithKit(iAnotherSht, 0)==true &&            //如果Arm 2的Site全部被關掉
                   InArmSideAllCloseWithKit(iAnotherSht, 1)==true)
                {
                    SetShuttleToHasNullIC_9045(iAnotherSht);
                }
                bFreeSht=true;
            }
            else
            {
                bFreeSht=false;
            }
        }
    }
    else
    {
        SetShuttleToHasNullIC_9045(iSht);
        if(IniConfig.bUseAutoSiteMapping &&
           LastSet.iRunStartMode==rsmAutoSiteMap &&
           IniConfig.bI21ASMRunTimeCHeck==false)                                //Steven 20140729 : AutoSiteMapping, 邊生產邊做
        {
            if(InArmSideAllCloseWithKit(iAnotherSht, 0)==true &&                //如果Arm 2的Site全部被關掉
               InArmSideAllCloseWithKit(iAnotherSht, 1)==true)                  //Steven 20241108 : fixed for auto site map and close arm.
            {
                SetShuttleToHasNullIC_9045(iAnotherSht);
            }
        }
        else
        {
            if(InArmSideAllCloseWithKit(iAnotherSht, 0)==true &&                //如果Arm 2的Site全部被關掉
               InArmSideAllCloseWithKit(iAnotherSht, 1)==true)
            {
                SetShuttleToHasNullIC_9045(iAnotherSht);
            }
        }
        bFreeSht=true;
    }
    SetShuttlefCanMoveL(iSht, bFreeSht, Func, Task);

    if(iInArmType==e9045_1x1_1      ||
       iInArmType==e9045_1x2_2_13   ||
       iInArmType==e9045_1x2_2_14   ||
       iInArmType==e9045_1x2_4_Hot  ||
       iInArmType==e9045_1x3_4      ||
       iInArmType==e9045_1x4_1_Ac   ||
       iInArmType==e9045_1x4_4      ||
       iInArmType==e9045_1x4_8_Hot  ||                                          //Steven 20240119 : fixed for 2x4_16
       iInArmType==e9045_1x4_4_Back ||
       iInArmType==e9045_2x2_8_Hot  ||
       iInArmType==e9045_2x1_2_13   ||
       iInArmType==e9045_2x2_4_12   ||
       iInArmType==e9045_2x2_4_13   ||
       iInArmType==e9045_2x2_4_14   ||
       iInArmType==e9045_2x3_6      ||                                          //Steven 20200219 : 2x3mode, Layout: 120x70mm
       iInArmType==e9045_2x4_8      ||
       iCloseSiteModeFor2x8>e2x8OneByOne ||
       iCloseSiteModeFor2x6>e2x6OneByOne)                                       //Steven 20240417 : 12site關中間4site
    {
        bNeedAdjust2Time=false;
    }

    return bNeedAdjust2Time;
}
//==============================================================================
void SetShuttleStatus_9045(int SHT, int iShtRow, int iShtCol, int iSuckRow, int iSuckCol)
{
    AnsiString Str;

    if(InArmSuck.Item[iSuckRow][iSuckCol]!=HAS_NULL_IC)                         //Steven 20170109 : 沒IC的地方不檢查吹氣
    {
        bInArmCheckDestroyACT[iSuckRow][iSuckCol]=true;                         //jou 981130 確認device確實destroy完成
        LastSet.lShuttleCount++;                                                //Steven 20170113 : For count place to shuttle
    }

    if(SHT==0)
    {
        if(FLCarryKit.Item[iShtRow][iShtCol]==NULL_IC)
        {
            InArmSuck.PordRec[iSuckRow][iSuckCol].AddArmSiteRecord(1, InArmSuck.iWhichSite[iSuckRow][iSuckCol]);        //Steven 20240508 : fixed for iWhichSite flag
            InspectInArmPosition(MInShuttle1, iSuckRow, iSuckCol, iShtRow, iShtCol, true);                              //Jimmychiu 20240408 : debug for inarm position
            FLCarryKit.MoveSuckDataDiff(InArmSuck, iSuckRow, iSuckCol, iShtRow, iShtCol);
            FLCarryKit.iWhichSite[iShtRow][iShtCol]=FLCarryKit.iWhichSite[iShtRow][iShtCol]-1;                          //Steven 20240320 : 變更iWhcihSite填入位置
            FLCarryKit.iWhichIndex[iShtRow][iShtCol]=0;                         //Steven 20240320 : 變更iWhichIndex填入位置
        }
        else if(InArmSuck.Item[iSuckRow][iSuckCol]==HAS_NULL_IC)
        {
            InArmSuck.SetItemData(iSuckRow, iSuckCol, NULL_IC);
        }
        else
        {
            Str.sprintf("FLCarryKit.Item[%d][%d]=%d, InArmSuck.Item[%d][%d]=%d", iShtRow, iShtCol, FLCarryKit.Item[iShtRow][iShtCol], iSuckRow, iSuckCol, InArmSuck.Item[iSuckRow][iSuckCol]);  //Steven 20220620 : add log message
            ShowMyMessage("Program Error in SetShuttleStatus", Str);
        }
        FLCarryKit.HotCount=InArmSuck.HotCount;                                 //JerryYang 20230204 : add hot plate放置順序資料
    }
    else
    {
        if(BLCarryKit.Item[iShtRow][iShtCol]==NULL_IC)
        {
            InArmSuck.PordRec[iSuckRow][iSuckCol].AddArmSiteRecord(2, InArmSuck.iWhichSite[iSuckRow][iSuckCol]);        //Steven 20240508 : fixed for iWhichSite flag
            InspectInArmPosition(MInShuttle2, iSuckRow, iSuckCol, iShtRow, iShtCol, true);                              //Jimmychiu 20240408 : debug for inarm position
            BLCarryKit.MoveSuckDataDiff(InArmSuck, iSuckRow, iSuckCol, iShtRow, iShtCol);
            BLCarryKit.iWhichSite[iShtRow][iShtCol]=BLCarryKit.iWhichSite[iShtRow][iShtCol]-1;                          //Steven 20240320 : 變更iWhcihSite填入位置
            BLCarryKit.iWhichIndex[iShtRow][iShtCol]=1;                         //Steven 20240320 : 變更iWhichIndex填入位置   //Steven 20241219 : 0 --> 1
        }
        else if(InArmSuck.Item[iSuckRow][iSuckCol]==HAS_NULL_IC)
        {
            InArmSuck.SetItemData(iSuckRow, iSuckCol, NULL_IC);
        }
        else
        {
            Str.sprintf("BLCarryKit.Item[%d][%d]=%d, InArmSuck.Item[%d][%d]=%d", iShtRow, iShtCol, BLCarryKit.Item[iShtRow][iShtCol], iSuckRow, iSuckCol, InArmSuck.Item[iSuckRow][iSuckCol]);  //Steven 20220620 : add log message
            ShowMyMessage("Program Error in SetShuttleStatus", Str);
        }
        BLCarryKit.HotCount=InArmSuck.HotCount;                                 //JerryYang 20230204 : add hot plate放置順序資料
    }
}
//==============================================================================
void GetInArmZShtDownPos_9045(int iSht, bool bPlace, bool IncludeZ)             //Steven 20230323 : For HT1032
{
    bool bNeedDown=false;
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            if(bPlace)
            {
                if(bRunAutoClean==true)                                         //Steven 20241102 : fixed for Auto Clean offset
                {
                    if(iSht==0)
                        iZPosToSht[i][j]=Prod.ZInArm_Shuttle1_Place[i][j]+TestIF_File.iPadThickness+TestIF_File.iAutoClean_Shuttle1PlaceOffset;
                    else
                        iZPosToSht[i][j]=Prod.ZInArm_Shuttle2_Place[i][j]+TestIF_File.iPadThickness+TestIF_File.iAutoClean_Shuttle2PlaceOffset;
                }
                else
                {
                    if(iSht==0)
                        iZPosToSht[i][j]=Prod.ZInArm_Shuttle1_Place[i][j];
                    else
                        iZPosToSht[i][j]=Prod.ZInArm_Shuttle2_Place[i][j];
                }

                bNeedDown=(IncludeZ==true &&
                           InArmSuck.Item[i][j]!=NULL_IC &&
                           InArmSuck.Item[i][j]!=HAS_NULL_IC &&
                           InArmSuck.Suck[i][j].GetNeedDestroyStatus());

                bZFlgToSht[i][j]=bNeedDown;
            }
            else                                                                //kevin 20160924
            {
                if(bRunAutoClean==true)                                         //Steven 20241102 : fixed for Auto Clean offset
                {
                    if(iSht==0)
                        iZPosToSht[i][j]=Prod.ZInArm_Shuttle1_Place[i][j]+TestIF_File.iPadThickness+TestIF_File.iAutoClean_Shuttle1PickOffset;
                    else
                        iZPosToSht[i][j]=Prod.ZInArm_Shuttle2_Place[i][j]+TestIF_File.iPadThickness+TestIF_File.iAutoClean_Shuttle2PickOffset;
                }
                else
                {
                    if(iSht==0)
                        iZPosToSht[i][j]=Prod.ZInArm_Shuttle1_Place[i][j];
                    else
                        iZPosToSht[i][j]=Prod.ZInArm_Shuttle2_Place[i][j];
                }

                bNeedDown=(IncludeZ==true &&
                           InArmSuck.Item[i][j]==NULL_IC &&
                           (InArmSuck.iNeedSuck[i][j]!=NULL_IC ||               //Steven 20240306 : 改用iNeedSuck
                            InArmSuck.iNeedSuck[i][j]!=HAS_NULL_CLEAN_IC));

                bZFlgToSht[i][j]=bNeedDown;
            }
        }
    }
}
//---------------------------------------------------------------------------
extern void InArmZNeedDown_1x1_1(int iSht, bool bPlace);
extern void InArmZNeedDown_1x2_2(int iSht, bool bPlace);
extern void InArmZNeedDown_1x2_2_14(int iSht, bool bPlace);
extern void InArmZNeedDown_1x2_4_Hot(int iSht, bool bPlace);
extern void InArmZNeedDown_1x3_4(int iSht, bool bPlace);
extern void InArmZNeedDown_1x3_2_14(int iSht, int iKit, bool bPlace);
extern void InArmZNeedDown_1x4_2(int iSht, int iKit, bool bPlace);
extern void InArmZNeedDown_1x4_4(int iSht, bool bPlace);
extern void InArmZNeedDown_1x4_4_Back(int iSht, bool bPlace);
extern void InArmZNeedDown_1x4_4_13(int iSht, bool bPlace);
extern void InArmZNeedDown_1x4_8_Hot(int iSht, bool bPlace);
extern void InArmZNeedDown_2x1_2(int iSht, bool bPlace);
extern void InArmZNeedDown_2x2_4_12(int iSht, bool bPlace);
extern void InArmZNeedDown_2x2_4_13(int iSht, bool bPlace);
extern void InArmZNeedDown_2x2_4_14(int iSht, bool bPlace);
extern void InArmZNeedDown_2x2_8_Hot(int iSht, bool bPlace);
extern void InArmZNeedDown_2x3_6(int iSht, bool bPlace);
extern void InArmZNeedDown_2x3_6_14(int iSht, int iKit, bool bPlace);
extern void InArmZNeedDown_2x4_4_14(int iSht, int iKit, bool bPlace);
extern void InArmZNeedDown_2x4_4_13(int iSht, int iKit, bool bPlace);
extern void InArmZNeedDown_2x4_8(int iSht, bool bPlace);
extern void InArmZNeedDown_2x5_8(int iSht, int iKit, bool bPlace);
extern void InArmZNeedDown_2x6_8(int iSht, int iKit, bool bPlace);
extern void InArmZNeedDown_2x8_8(int iSht, int iKit, bool bPlace);
void InArmZNeedDown_9045(int iSht, int iKit, bool bPlace)                       //Steven 20240512 : 整合InArmZNeedDown
{
    if(USE_PICKER_COUNT==ep1Picker)
    {
        InArmZNeedDown_All_1Pick(iSht, bPlace);
        return;
    }

    switch(TestIF.iTestMode)
    {
        case SingleSite:                                                        //2013-09-13    Dell    for TSMC Single site
            InArmZNeedDown_1x1_1(iSht, bPlace);
            break;
        case DualSite:                                                          //1x2
        case QualSite2X2N:                                                      //Steven 20201014 : for 2x2 nn mode auto clean
            if(iInArmType==e9045_1x2_2_14)
                InArmZNeedDown_1x2_2_14(iSht, bPlace);
            else if(iInArmType==e9045_1x2_4_Hot)
                InArmZNeedDown_1x2_4_Hot(iSht, bPlace);
            else
                InArmZNeedDown_1x2_2(iSht, bPlace);
            break;
        case TriSite1X3:                                                        //Frank 20160323 1x3 AutoClean Add
        case _6Site2X3N:                                                        //Steven 20220425 : 2X3NN Mode
            if(iInArmType==e9045_1x3_2_14)                                      //Steven 20220425 : 1x3_14
            {
                InArmZNeedDown_1x3_2_14(iSht, iKit, bPlace);
            }
            else
            {
                InArmZNeedDown_1x3_4(iSht, bPlace);
            }
            break;
        case QualSite1X4:                                                       //1x4
        case _8Site1X4:                                                         //ChungHung 20150528 add for 海思 _8Site1x4
        case _8Site2X4N:                                                        //Wei 20231211 : 2X4NN Mode
            if(iInArmType==e9045_1x4_2_14)                                      //Steven 20240221 : Add for 1x4_2 Auto Clean
            {
                InArmZNeedDown_1x4_2(iSht, iKit, bPlace);
            }
            else if(iInArmType==e9045_1x4_1_Ac)                                 //Steven 20200720 : 1x4只開site Ac
            {
                InArmZNeedDown_1x1_1(iSht, bPlace);
            }
            else if(iInArmType==e9045_1x4_4_13)                                 //Sam 20180208 (Steven) : HT-9045S AutoClean 1X4 Add
            {
                InArmZNeedDown_1x4_4_13(iSht, bPlace);
            }
            else if(iInArmType==e9045_1x4_8_Hot)
            {
                InArmZNeedDown_1x4_8_Hot(iSht, bPlace);
            }
            else
            {
                InArmZNeedDown_1x4_4(iSht, bPlace);
            }
            break;
        case QualSite2X2:                                                       //2x2
            if(iInArmType==e9045_2x2_4_14)                                      //Steven 20150330 : X-Pitch > 80mm
                InArmZNeedDown_2x2_4_14(iSht, bPlace);
            else if(iInArmType==e9045_2x2_4_12)                                 //Steven 20160929 : X-Pitch < 26.66mm
                InArmZNeedDown_2x2_4_12(iSht, bPlace);
            else if(iInArmType==e9045_2x2_8_Hot)
                InArmZNeedDown_2x2_8_Hot(iSht, bPlace);
            else
                InArmZNeedDown_2x2_4_13(iSht, bPlace);
            break;
        case _6Site2X3:
            if(iInArmType==e9045_2x3_6_14)                                      //Steven 20200219 : 2x3mode, Layout: 120x70mm
            {
                InArmZNeedDown_2x3_6_14(iSht, iKit, bPlace);
            }
            else
            {
                InArmZNeedDown_2x3_6(iSht, bPlace);
            }
            break;
        case _16Site4X4:                                                        //Sam 20190226 : 16Site4X4
        case _8Site2X4:                                                         //2x4
            if(USE_PICKER_COUNT==0)                                             //Steven 20161229 : for HT-9045S
                InArmZNeedDown_2x4_4_13(iSht, iKit, bPlace);
            else if(iInArmType==e9045_2x4_4_14)                                 //JerryYang 20160413 : X-Pitch > 40mm
                InArmZNeedDown_2x4_4_14(iSht, iKit, bPlace);
            else
                InArmZNeedDown_2x4_8(iSht, bPlace);
            break;
        case _10Site2X5:                                                        //2X5 //wei 20190614 10 site
            InArmZNeedDown_2x5_8(iSht, iKit, bPlace);
            break;
        case _12Site2X6:                                                        //2X6 //ChungHung 20140224 add
            InArmZNeedDown_2x6_8(iSht, iKit, bPlace);
            break;
        case _16Site2X8:                                                        //2x8
        case _32Site4X8N:
            InArmZNeedDown_2x8_8(iSht, iKit, bPlace);
            break;
        default:
            ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "InArmZNeedDown_CleanKit");
    }
}
//------------------------------------------------------------------------------
extern bool MoveInArm2XYToShuttle_9045_1x1_1(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_1x2_2_13(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_1x2_2_14(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_1x2_4_Hot(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_1x3_4(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_1x3_2_14(int iSht, int iKit, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_1x4_2(int iSht, int iKit, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_1x4_4(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_1x4_4_Back(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_1x4_4_13(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_1x4_8_Hot(int iSht, bool IncludeZ, bool bPlace);                                 //Sam 20250203 : 修正 1X4 8 吸嘴模式
extern bool MoveInArm2XYToShuttle_9045_2x1_2(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_2x2_4_12(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_2x2_4_13(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_2x2_4_14(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_2x2_8_Hot(int iSht, bool IncludeZ, bool bPlace);                                 //Sam 20250212 : 修正 2X2 8 吸嘴模式
extern bool MoveInArm2XYToShuttle_9045_2x3_6_14(int iSht, int iKit, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_2x3_6(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_2x4_4_13(int iSht, int iKit, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_2x4_4_14(int iSht, int iKit, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_2x4_8(int iSht, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_2x5_8(int iSht, int iKit, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_2x6_8(int iSht, int iKit, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_2x8_8(int iSht, int iKit, bool IncludeZ, bool bPlace);
extern bool MoveInArm2XYToShuttle_9045_All_1Pick(int iSht, bool IncludeZ, bool bPlace);
//------------------------------------------------------------------------------
bool MoveInArmXYToShuttle_9045(int iSht, int iKit, bool IncludeZ, bool bPlace)  //Steven 20240512 : 整合MoveInArmXYToShuttle
{
    bool bResult;
    if(USE_PICKER_COUNT==ep1Picker)
    {
        bResult=MoveInArm2XYToShuttle_9045_All_1Pick(iSht, ZAxisNotDown, bPlace);
        return bResult;
    }

    switch(TestIF.iTestMode)
    {
        case SingleSite:
            bResult=MoveInArm2XYToShuttle_9045_1x1_1(iSht, ZAxisNotDown, bPlace);
            break;
        case DualSite:                                                          //1x2
        case QualSite2X2N:                                                      //Steven 20201014 : for 2x2 nn mode auto clean
            if(iInArmType==e9045_1x2_4_Hot && bRunAutoClean==false)             //Sam 20250212 : 修正 2X2 8 吸嘴模式
                bResult=MoveInArm2XYToShuttle_9045_1x2_4_Hot(iSht, ZAxisNotDown, bPlace);
            else if(iInArmType==e9045_1x2_2_14)                                 //Steven 20240508 : fixed for auto clean
                bResult=MoveInArm2XYToShuttle_9045_1x2_2_14(iSht, ZAxisNotDown, bPlace);
            else
                bResult=MoveInArm2XYToShuttle_9045_1x2_2_13(iSht, ZAxisNotDown, bPlace);
            break;
        case TriSite1X3:                                                        //Frank 20160323 1x3 AutoClean Add
        case _6Site2X3N:                                                        //Steven 20220425 : 2X3NN Mode
            if(iInArmType==e9045_1x3_2_14)                                      //Steven 20220425 : 1x3_14
                bResult=MoveInArm2XYToShuttle_9045_1x3_2_14(iSht, iKit, ZAxisNotDown, bPlace);
            else
                bResult=MoveInArm2XYToShuttle_9045_1x3_4(iSht, ZAxisNotDown, bPlace);
            break;
        case QualSite1X4:                                                       //1x4
        case _8Site1X4:                                                         //ChungHung 20150528 add for 海思 _8Site1x4
        case _8Site2X4N:                                                        //Wei 20231211 : 2X4NN Mode
            if(iInArmType==e9045_1x4_1_Ac)                                      //Steven 20200720 : 1x4只開site Ac
                bResult=MoveInArm2XYToShuttle_9045_1x1_1(iSht, ZAxisNotDown, bPlace);
            else if(iInArmType==e9045_1x4_4_13)                                 //Steven 20170810 (Jou) : Add for HT-9045S
                bResult=MoveInArm2XYToShuttle_9045_1x4_4_13(iSht, ZAxisNotDown, bPlace);
            else if(iInArmType==e9045_1x4_2_14)
                bResult=MoveInArm2XYToShuttle_9045_1x4_2(iSht, iKit, ZAxisNotDown, bPlace);
            else if(iInArmType==e9045_1x4_8_Hot && bRunAutoClean==false)        //Sam 20250212 : 修正 1X4 8 吸嘴模式
                bResult=MoveInArm2XYToShuttle_9045_1x4_8_Hot(iSht, ZAxisNotDown, bPlace);
            else
                bResult=MoveInArm2XYToShuttle_9045_1x4_4(iSht, ZAxisNotDown, bPlace);
            break;
        case QualSite2X2:                                                       //2x2
            if(iInArmType==e9045_2x2_4_14)                                      //Steven 20240508 : fixed for auto clean
                bResult=MoveInArm2XYToShuttle_9045_2x2_4_14(iSht, ZAxisNotDown, bPlace);
            else if(iInArmType==e9045_2x2_8_Hot && bRunAutoClean==false)        //Sam 20250212 : 修正 2X2 8 吸嘴模式
                bResult=MoveInArm2XYToShuttle_9045_2x2_8_Hot(iSht, ZAxisNotDown, bPlace);
            else
                bResult=MoveInArm2XYToShuttle_9045_2x2_4_13(iSht, ZAxisNotDown, bPlace);
            break;
        case _6Site2X3:
            if(iInArmType==e9045_2x3_6_14)                                      //Steven 20200219 : 2x3mode, Layout: 120x70mm
                bResult=MoveInArm2XYToShuttle_9045_2x3_6_14(iSht, iKit, ZAxisNotDown, bPlace);
            else                                                                //ChungHung 20140115 add for 2x3 ~ need test
                bResult=MoveInArm2XYToShuttle_9045_2x3_6(iSht, ZAxisNotDown, bPlace);
            break;
        case _8Site2X4:                                                         //2x4
        case _16Site4X4:                                                        //Sam 20190226 : 16Site4X4
            if(iInArmType==e9045_2x4_4_13)                                      //Steven 20240508 : fixed for auto clean
                bResult=MoveInArm2XYToShuttle_9045_2x4_4_13(iSht, iKit, ZAxisNotDown, bPlace);
            else if(iInArmType==e9045_2x4_4_14)
                bResult=MoveInArm2XYToShuttle_9045_2x4_4_14(iSht, iKit, ZAxisNotDown, bPlace);
            else
                bResult=MoveInArm2XYToShuttle_9045_2x4_8(iSht, ZAxisNotDown, bPlace);
            break;
        case _10Site2X5:
            bResult=MoveInArm2XYToShuttle_9045_2x5_8(iSht, iKit, ZAxisNotDown, bPlace);
            break;
        case _12Site2X6:                                                        //2013-11-05    Dell    Modify _12Site2X6 autoclean
            bResult=MoveInArm2XYToShuttle_9045_2x6_8(iSht, iKit, ZAxisNotDown, bPlace);
            break;
        case _16Site2X8:                                                        //2x8
        case _32Site4X8N:                                                       //Steven 20140512: For HT-9047 Auto Clean 待驗證
//            if(iInArmType==e9045_2x8_16)
//                bResult=MoveInArm2XYToShuttle_9045_2x8_16(iSht, ZAxisNotDown, bPlace);
//            else
                bResult=MoveInArm2XYToShuttle_9045_2x8_8(iSht, iKit, ZAxisNotDown, bPlace);
            break;
        default:
            ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "MoveInArmXYToShuttle");
    }
    return bResult;
}
//------------------------------------------------------------------------------
int iMoveToShuttle=0;
//------------------------------------------------------------------------------
int MoveInArmXYToWaitTrayArm(int iSht, int iKit, bool IncludeZ, bool bPlace)
{
    int iResult=0;

    if(iMoveToShuttle==0)                                                       //Steven 20241115 : for in arm 去sht 1放料hang up
    {
        if(MOT[MMTrayY].Tray.HasIC()==false &&
           MOT[MMTrayY].fHasTray &&
           TRAY_ARM_MODE!=eUnderCoveyor)
        {
            iMoveToShuttle=1;
        }
        else
        {
            iMoveToShuttle=2;
        }
    }

    if(iMoveToShuttle==1)
    {
        if(MoveInArm2XYToWait())
        {
            iMoveToShuttle=0;
            iResult=(iSht==0)?1000:2000;
        }
    }
    else
    {
        if(MoveInArmXYToShuttle_9045(iSht, iKit, IncludeZ, bPlace))
        {
            iMoveToShuttle=0;
            iResult=(iSht==0)?1100:2100;
        }
    }

    return iResult;
}
//------------------------------------------------------------------------------
bool MoveInArmZToShuttlePlace_9045(int iSht)
{
    GetInArmZShtDownPos_9045(iSht, true, true);
    bool bResult=InArmZMoveDown(bZFlgToSht, iZPosToSht, false);
    return bResult;
}
//==============================================================================
//Index         : Shuttle 1 or 2
//alarmflag     : 是否要直接Alarm
//Line          : 有幾顆Sensor要偵測
//bDuplicateErr : 是否有重複發生
//bSensorOn     : 是否有錯誤
//==============================================================================
int CheckShuttleSensorStatus(int Index, bool alarmflag, int Line, bool *bDuplicateErr, bool *bSensorOn, bool bAutoclean)                                        //Steven 20110409 : 整合Alarm Code  //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
{
    int j, Pos;
    AnsiString ErrPart="";                                                      //Steven 20110408 : 合併Alarm
    bool bHasDuplicateErr=false;                                                //Steven 20110408 : 合併Alarm
    bool bHasErr=false;                                                         //Steven 20110408 : 合併Alarm

    for(int i=0; i<Line; i++)
    {
        if(bDuplicateErr[i]==true)                                              //檢查是否為重覆的Alarm
            bHasDuplicateErr=true;

        Pos=SThreadPara.iInShSenIndex[Index][i];
        if(Sen[Pos].Enable==false)
        {
            bSensorOn[i]=false;                                                 //計為無錯誤或已經排除
        }
        else
        {
            if(Sen[Pos].IsOn()==true)                                           //檢查Sensor是否被遮斷
            {
                bHasErr=true;                                                   //表示有錯誤
                bSensorOn[i]=true;                                              //計為已經有發生過錯誤

                if(i==7)                                                        //9046的第8顆要另外處理
                {
                    ErrPart+=ShuttleSensorName[7];                              //紀錄錯誤的位置
                }
                else if(i==8)
                {
                    ErrPart+=" X1";                                             //紀錄錯誤的位置
                }
                else if(i==9)                                                   //2013-07-16    Dell    Shuttle cross sensor
                {
                    ErrPart+=" X2";                                             //紀錄錯誤的位置
                }
                else
                {
                    if(Pos>=SnInPutSHT1S8)
                    {
                        if(Index==0)
                            j=Pos-SnInPutSHT1S8+7;                              //Steven 20200812 : 8 --> 7
                        else
                            j=Pos-SnInPutSHT2S8+7;                              //Steven 20200812 : 8 --> 7
                    }
                    else
                    {
                        if(Index==0)
                            j=Pos-SnInPutSHT1S1;
                        else
                            j=Pos-SnInPutSHT2S1;
                    }

                    if(Index==0)
                        ErrPart+=ShuttleSensorName[j];                          //紀錄錯誤的位置    //jou 2014-08-08 記憶體破壞 187-33  [Pos-SnInPutSHT1S1] -> i
                    else
                        ErrPart+=ShuttleSensorName[j];                          //紀錄錯誤的位置    //jou 2014-08-08 記憶體破壞 187-33 [Pos-SnInPutSHT2S1] -> i

                    iInShuttleJam[Index][j]=2;                                  //kevin 20220819 shuttle Jam  紀錄位置後續，要做放到垃圾桶動作   //KEVIN 20220818 j=1 - 8
                }
            }
            else
            {
                bSensorOn[i]=false;                                             //計為無錯誤或已經排除
                if(i==7 || i==8 || i==9)                                        //9046的第8顆要另外處理
                {
                }
                else
                {
                    if(Pos>=SnInPutSHT1S8)
                    {
                        if(Index==0)
                            j=Pos-SnInPutSHT1S8+7;                              //Steven 20200812 : 8 --> 7
                        else
                            j=Pos-SnInPutSHT2S8+7;                              //Steven 20200812 : 8 --> 7
                    }
                    else
                    {
                        if(Index==0)
                            j=Pos-SnInPutSHT1S1;
                        else
                            j=Pos-SnInPutSHT2S1;
                    }
                    iInShuttleJam[Index][j]=0;                                  //kevin 20220819 shuttle Jam  紀錄位置後續，要做放到垃圾桶動作   //KEVIN 20220818 j=1 - 8
                }
            }
        }
    }

    if(bHasErr)
    {
        if(alarmflag)
        {
            if(!MoveInArm2XYToWait())                                           //先將In Arm讓開
                return 3;                                                       //Steven && jou 20091008

            if(bAutoclean)                                                      //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            {
                if(Index==0)
                    ShowErrorMessage("JAM0416", K_RETRY, MInShuttle1, bHasDuplicateErr, ErrPart);                       //Input Shuttle 1 device floating error.                }
                else
                    ShowErrorMessage("JAM0417", K_RETRY, MInShuttle2, bHasDuplicateErr, ErrPart);                       //Input Shuttle 2 device floating error.                }
            }
            else
            {
                if(Index==0)
                    ShowErrorMessage("JAM0401", K_RETRY, MInShuttle1, bHasDuplicateErr, ErrPart);                       //Input Shuttle 1 device floating error.
                else
                    ShowErrorMessage("JAM0404", K_RETRY, MInShuttle2, bHasDuplicateErr, ErrPart);                       //Input Shuttle 2 device floating error.
                iCheckShuttleSensor=1;
            }

            for(int i=0; i<Line; i++)                                           //顯示錯誤後,才進行flag的轉換
            {
                if(bSensorOn[i]==true)
                {
                    bDuplicateErr[i]=true;
                }
                else
                {
                    bDuplicateErr[i]=false;
                }
            }
            return 1;
        }
        else
        {
            return 2;                                                           //等待抖抖抖
        }
    }
    else
    {
        for(int i=0; i<Line; i++)                                               //沒有錯誤要清空紀錄
        {
            bDuplicateErr[i]=false;
        }
    }
    return 0;
}
//==============================================================================
// check suttle ic position
// return 0 : 正確放入飛梭中
// return 1 : 不正確放入飛梭中
// return 2 : 不正確放入飛梭中且時間延遲時間已到達
// return 3 : 等待InArm移動到安全位置
//==============================================================================
int CheckShuttleSensor_9045_2x5(int Index, bool alarmflag, bool bAutoclean)
{
    static bool bDuplicateErr[2][8]={false, false};                             //2013-07-16    Dell    Shuttle cross sensor

    if(LastSet.iRealDummy!=REALLY)
        return 0;

    int ret;
    int SenNum=(CROSS_SENSOR_INSTALL)?7:5;
    bool bSensorOn[2][8]={false};

    ret=CheckShuttleSensorStatus(Index, alarmflag, SenNum, bDuplicateErr[Index], bSensorOn[Index], bAutoclean);         //Steven 20130702 : _12Site2X6
    return ret;
}
//------------------------------------------------------------------------------
int CheckShuttleSensor_9045_2x6(int Index, bool alarmflag, bool bAutoclean)
{
    static bool bDuplicateErr[2][8]={false, false};                             //2013-07-16    Dell    Shuttle cross sensor

    if(LastSet.iRealDummy!=REALLY)
        return 0;

    int ret;
    int SenNum=(CROSS_SENSOR_INSTALL)?8:6;
    bool bSensorOn[2][8]={false};

    ret=CheckShuttleSensorStatus(Index, alarmflag, SenNum, bDuplicateErr[Index], bSensorOn[Index], bAutoclean);         //Steven 20130702 : _12Site2X6  //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
    return ret;
}
//------------------------------------------------------------------------------
int CheckShuttleSensor_9045_2x8(int Index, bool alarmflag, bool bAutoclean)     //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
{
    static bool bDuplicateErr[2][10]={false, false};                            //2013-07-16    Dell    Shuttle cross sensor

    if(LastSet.iRealDummy!=REALLY)
        return 0;

    int ret;
    int SenNum=(CROSS_SENSOR_INSTALL)?10:8;
    bool bSensorOn[2][10]={false};

    ret=CheckShuttleSensorStatus(Index, alarmflag, SenNum, bDuplicateErr[Index], bSensorOn[Index], bAutoclean);         //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
    return ret;
}
//------------------------------------------------------------------------------
int CheckShuttleSensor_9045_1x4(int Index, bool alarmflag, bool bAutoclean)
{
    static bool bDuplicateErr[2][6]={false, false};                             //2013-07-16    Dell    Shuttle cross sensor

    if(LastSet.iRealDummy!=REALLY)
        return 0;

    int ret;
    int SenNum=(CROSS_SENSOR_INSTALL)?6:4;
    bool bSensorOn[2][6]={false};

    ret=CheckShuttleSensorStatus(Index, alarmflag, SenNum, bDuplicateErr[Index], bSensorOn[Index], bAutoclean);         //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
    return ret;
}
//------------------------------------------------------------------------------
int CheckShuttleSensor_9045_2x3(int Index, bool alarmflag, bool bAutoclean)     //ChungHung 20140115 add for 2x3_6
{
    static bool bDuplicateErr[2][5]={false, false};                             //2013-07-16    Dell    Shuttle cross sensor

    if(LastSet.iRealDummy!=REALLY)
        return 0;

    int ret;
    int SenNum=(CROSS_SENSOR_INSTALL)?5:3;
    bool bSensorOn[2][5]={false};

    ret=CheckShuttleSensorStatus(Index, alarmflag, SenNum, bDuplicateErr[Index], bSensorOn[Index], bAutoclean);         //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
    return ret;
}
//------------------------------------------------------------------------------
int CheckShuttleSensor_9045_2x4(int Index, bool alarmflag, bool bAutoclean)
{
    return CheckShuttleSensor_9045_1x4(Index, alarmflag, bAutoclean);
}
//------------------------------------------------------------------------------
int CheckShuttleSensor_9045_1x2(int Index, bool alarmflag, bool bAutoclean)
{
    static bool bDuplicateErr[2][4]={false, false};                             //2013-07-16    Dell    Shuttle cross sensor

    if(LastSet.iRealDummy!=REALLY)
        return 0;

    int ret;
    int SenNum=(CROSS_SENSOR_INSTALL)?4:2;
    bool bSensorOn[2][4]={false};

    ret=CheckShuttleSensorStatus(Index, alarmflag, SenNum, bDuplicateErr[Index], bSensorOn[Index], bAutoclean);         //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
    return ret;
}
//------------------------------------------------------------------------------
int CheckShuttleSensor_9045_2x2(int Index, bool alarmflag, bool bAutoclean)
{
    return CheckShuttleSensor_9045_1x2(Index, alarmflag, bAutoclean);
}
//------------------------------------------------------------------------------
int CheckShuttleSensor_9045_1x1(int Index, bool alarmflag, bool bAutoclean)
{
    if(iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
        return CheckShuttleSensor_9045_1x4(Index, alarmflag, bAutoclean);
    else
        return CheckShuttleSensor_9045_1x2(Index, alarmflag, bAutoclean);
}
//------------------------------------------------------------------------------
int CheckShuttleSensor_9045_2x1(int Index, bool alarmflag, bool bAutoclean)
{
    return CheckShuttleSensor_9045_1x2(Index, alarmflag, bAutoclean);
}
//-----------------------------------------------------------------------------
//ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
bCheckInArmDestroyActiveFinish[MAX_ARM_Row][MAX_ARM_Col]={{true, true, true, true}, {true, true, true, true}};
//ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
//-----------------------------------------------------------------------------
void CheckInArmDestroyActiveFinishInitial()                                     //kevin 20170915
{
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            bCheckInArmDestroyActiveFinish[i][j]=true;                          //kevin 20170915 (wei) device 回吸檢測尚未做完
        }
    }
}
//-----------------------------------------------------------------------------
bool IsCheckInArmDestroyActiveFinish()
{
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
           if(bCheckInArmDestroyActiveFinish[i][j]==false)
              return false;
        }
    }
    return true;
}
//-----------------------------------------------------------------------------
//jou 981130 start : 確認device確實destroy完成
bool bCheckInArmDestroyActive[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
TQPF_Timer CheckInArmDestroyActiveDelay[MAX_ARM_Row][MAX_ARM_Col];
void CheckInArmDestroyActive()
{
    static int iTask[MAX_ARM_Row][MAX_ARM_Col]={{1, 1, 1, 1}, {1, 1, 1, 1}};    //Sam 20240704 : 修正回吸偵測異常
    static bool bDuplicate[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};      //Steven 20190701 : 加上重複Alarm判斷

    int iMotZ;
    int iPos[MAX_ARM_Row][MAX_ARM_Col];
    bool bFlag[MAX_ARM_Row][MAX_ARM_Col];

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(bInArmCheckDestroyACT[i][j]==false)
            {
                iTask[i][j]=1;
                bDuplicate[i][j]=false;
                continue;
            }

            switch(iTask[i][j])
            {
                case 1:
                    bCheckInArmDestroyActiveFinish[i][j]=false;                                                         //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
                    iMotZ=(InOutArmPickerUseMotor==eptUseMotCyn)?MInArmZA:InArmSuck.Suck[i][j].iMotNo;
                    iPos[i][j]=MOT[iMotZ].ReadPos();
                    if(ArmSpeed_File[InArm].bHeightCheck)                                                               //wei 20160825 回吸偵測
                    {
                        if(iPos[i][j]>Tech.iInArmShuttlePlaceZ+ArmSpeed_File[InArm].iHeightCheck)
                        {
                            iTask[i][j]=200;
                            InArmSuck.Suck[i][j].OnSuck();                                                              //kevin 20170915 (wei) 提早動作
                            CheckInArmDestroyActiveDelay[i][j].SetSecAndOn(ArmSpeed[InArm].dDestroyCheckTime);          //2013-08-01    Dell    modify  公司統一回吸功能
                        }
                    }
                    else
                    {
                        if(iPos[i][j]>-500)
                        {
                            InArmSuck.Suck[i][j].OnSuck();                                                              //kevin 20170915 (wei) 提早動作
                            CheckInArmDestroyActiveDelay[i][j].SetSecAndOn(ArmSpeed[InArm].dDestroyCheckTime);          //2013-08-01    Dell    modify  公司統一回吸功能
                            iTask[i][j]=200;
                        }
                    }
                    break;
                case 200:
                    InArmSuck.Suck[i][j].OnSuck();                                                                      //kevin 20181102 add
                    CheckInArmDestroyActiveDelay[i][j].SetSecAndOn(ArmSpeed[InArm].dDestroyCheckTime);                  //kevin 20181112 add //2013-08-01    Dell    modify  公司統一回吸功能
                    if(ArmSpeed[InArm].bSuckOnDown==false ||                                                            //2013-08-01    Dell    modify  公司統一回吸功能
                       ArmSpeed[InArm].bDestroyPauseCheck)                                                              //Steven 20120507 : 針對小IC才做停止偵測的動作
                    {
                        bCheckInArmDestroyActive[i][j]=true;                                                            //Steven 20120507 : 要偵測完才可以動，會影響UPH
                    }

                    iTask[i][j]=300;
                case 300:
                    if(CheckInArmDestroyActiveDelay[i][j].Off())
                    {
                        bFlag[i][j]=InArmSuck.Suck[i][j].GetStatus();
                        if(bFlag[i][j])
                        {
                            int iXpos=MOT[MInArmX].ReadPos();
                            int iYpos=MOT[MInArmY].ReadPos();
                            ShowErrorMessage("WAR0158", 0, MInArmX, bDuplicate[i][j], InArmSuck.Suck[i][j].sName);      //破壞錯誤, 請將IC取下
                            InArmSuck.PordRec[i][j].AddErrorRecord("WAR0158", true, iXpos, iYpos);                      //Steven 20160114 (jou) : Add Production Error Log
                            bDuplicate[i][j]=true;

                            bCheckInArmDestroyActive[i][j]=true;
                            iTask[i][j]=200;                                                                            //Steven 20120507 : 有出錯就繼續吸吸看，看IC有沒有被拿掉。
                        }
                        else
                        {
                            bCheckInArmDestroyActiveFinish[i][j]=true;                                                  //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成

                            InArmSuck.Suck[i][j].Normal();
                            bInArmCheckDestroyACT[i][j]=false;
                            bCheckInArmDestroyActive[i][j]=false;
                            bDuplicate[i][j]=false;                                                                     //Steven 20190701 : 加上重複Alarm判斷
                            iTask[i][j]=1;                                                                              //ChungHung 20111229 add 防止跳出後bInArmCheckDestroyACT馬上被設為true 會照成有一次無效
                        }
                    }
                    break;
                default :
                    break;
            }
        }
    }
}
//-----------------------------------------------------------------------------
int CheckPickerMode(int iPickCount, bool bSupport14Mode, bool bSupport13Mode)   //12=Aa+Ab, 13=Aa+Ac, 14=Aa+Ad, 1234=Aa+Ab+Ac+Ad
{
    int Ld=TrayForm.Loader.iTrayType;
    int iResult=1234;
    InArmSuck.iXStep=1;
    if(USE_16PICKER_TYPE==1)                                                    //Ztex 2023.12.06 Add HT-1032
    {
        if(iPickCount==4)
        {
            if(ArmXCanSuck4IC_1032()==false)
            {
                if(bSupport13Mode==false ||
                   (bSupport14Mode && TestIF_File.dSiteXPitch>iXpitchMaxX4_MM))
                {
                    iResult=14;
                    if(TestIF_File.dSiteXPitch>=iXpitchMinX6_MM &&
                       TestIF_File.dSiteXPitch<=iXpitchMaxX6_MM)
                        InArmSuck.iXStep=1;
                    else
                        InArmSuck.iXStep=2;
                }
                else if(bSupport13Mode)
                {
                    iResult=13;
                    if(TestIF_File.dSiteXPitch>=iXpitchMinX4_MM &&
                       TestIF_File.dSiteXPitch<=iXpitchMaxX4_MM)
                        InArmSuck.iXStep=1;
                    else
                        InArmSuck.iXStep=2;
                }
                else
                {
                    InArmSuck.iXStep=4;
                }
            }
            else
            {
                if(TestIF_File.dSiteXPitch<=iXpitchMaxX2_MM &&                  //一次放
                   TestIF_File.dSiteXPitch>=iXpitchMinX2_MM)
                {
                    InArmSuck.iXStep=1;
                }
                else if(TestIF_File.iInArmToShtReleaseMode==1)                  //Steven 20250630 : 力旺要照順序擺
                {
                    InArmSuck.iXStep=4;
                }
                else if(TestIF_File.dSiteXPitch>iXpitchMaxX4_MM)                //四吸嘴跑13吸嘴
                {
                    InArmSuck.iXStep=4;
                }
                else
                {
                    if(UserDefForm[Ld].XPitch/100.0<=TestIF_File.dSiteXPitch/2)
                        InArmSuck.iXStep=2;
                    else
                        InArmSuck.iXStep=4;
                }
            }
        }
    }
    else
    {
        if(iPickCount==4)
        {
            if(ArmXCanSuck4IC_9045()==false)
            {
                if((TestIF_File.iTestMode==TriSite1X3 ||
                    TestIF_File.iTestMode==_6Site2X3N) &&                       //JerryYang 20251124 : fix 1x3
                   TestIF.iSiteMap[0][1]==0 &&
                   TestIF.iSiteMap[1][1]==0 &&
                   TestIF_File.dSiteXPitch>=120)
                {
                    iResult=14;
                    InArmSuck.iXStep=3;
                }
                else
                {
                    if(bSupport13Mode==false ||
                       (bSupport14Mode && TestIF_File.dSiteXPitch>iXpitchMaxX2_MM))
                    {
                        iResult=14;
                        if(TestIF_File.dSiteXPitch>=iXpitchMinX3_MM &&
                           TestIF_File.dSiteXPitch<=iXpitchMaxX3_MM)
                            InArmSuck.iXStep=1;
                        else
                            InArmSuck.iXStep=2;
                    }
                    else if(bSupport13Mode)
                    {
                        iResult=13;
                        if(TestIF_File.dSiteXPitch>=iXpitchMinX2_MM &&
                           TestIF_File.dSiteXPitch<=iXpitchMaxX2_MM)
                            InArmSuck.iXStep=1;
                        else
                            InArmSuck.iXStep=2;
                    }
                    else
                    {
                        InArmSuck.iXStep=4;                                     //理論上沒辦法跑
                    }
                }
            }
            else
            {
                if(TestIF_File.dSiteXPitch<=iXpitchMaxX1_MM &&                  //一次放
                   TestIF_File.dSiteXPitch>=iXpitchMinX1_MM)
                {
                    InArmSuck.iXStep=1;
                }
                else if(TestIF_File.iInArmToShtReleaseMode==1)                  //Steven 20250630 : 力旺要照順序擺
                {
                    InArmSuck.iXStep=4;
                }
                else if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM)
                {
                    InArmSuck.iXStep=4;
                }
                else
                {
                    if(iInArmType==e9045_2x8_8)                                 //JerryYang 20231003
                    {
                        InArmSuck.iXStep=4;
                    }
                    else if(iInArmType==e9045_2x6_8 &&                          //JerryYang 20231003
                            TestIF_File.dSiteXPitch>iXpitchMaxX1_MM)            //Sam 20250203 : 修正 2X6 Hangup
                    {
                        InArmSuck.iXStep=4;
                    }
                    else
                    {
                        if((TestIF_File.dSiteXPitch==iXpitchMaxX2_MM &&         //Steven 20240308 : 修正1x4 X-Pitch 80mm
                            InArmSuck.iShtCol==4) ||
                           UserDefForm[Ld].XPitch/100.0<=TestIF_File.dSiteXPitch/2)
                            InArmSuck.iXStep=2;
                        else
                            InArmSuck.iXStep=4;
                    }
                }
            }
        }
    }
    OutArmSuck.iXStep=InArmSuck.iXStep;
    return iResult;
}
//-----------------------------------------------------------------------------
//資料還沒轉換前就要先判斷可能會使用的mode, 所以全部的判斷都得使用 _File 的參數
//-----------------------------------------------------------------------------
void CheckInArmYStep()                                                          //Steven 20240205 : Y-Pitch可否一次放
{
    if(TestIF_File.iTestMode==SingleSite   ||
       TestIF_File.iTestMode==DualSite     ||                                   // 1x2
       TestIF_File.iTestMode==QualSite2X2N ||                                   //Frank 20200520 2X2NN Mode
       TestIF_File.iTestMode==TriSite1X3   ||                                   //1x3 //Frank 20160329 add for 1x3_4
       TestIF_File.iTestMode==_6Site2X3N   ||                                   //Steven 20220425 : 2X3NN Mode
       TestIF_File.iTestMode==QualSite1X4  ||                                   // 1x4
       TestIF_File.iTestMode==_8Site2X4N   ||                                   //Wei 20231211 : 2X4NN Mode
       TestIF_File.iTestMode==DualSite2x1)
    {
        InArmSuck.iYStep=1;
    }
    else
    {
        if(USE_IN_Y_IS_AUTO_PITCH==true)                                        //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {
            if(TestIF_File.dSiteYPitch>=IN_OUT_ARM_Y_PITCH_MIN/100.0 &&
               TestIF_File.dSiteYPitch<=IN_OUT_ARM_Y_PITCH_MAX/100.0)
                InArmSuck.iYStep=1;
            else
                InArmSuck.iYStep=2;
        }
        else
        {
            int iYPitch=GetVariableYInShuttleData();
            if((TestIF_File.dSiteYPitch*100)==iYPitch)                          //Steven 20240306 : 修正Y-Pitch異常問題
                InArmSuck.iYStep=1;
            else
                InArmSuck.iYStep=2;
        }

        if(USE_OUT_Y_IS_AUTO_PITCH==true)                                       //Rogeryang 20260108 : 9011UC in/outarm模組不同需拆開
        {
            if(TestIF_File.dSiteYPitch>=IN_OUT_ARM_Y_PITCH_MIN/100.0 &&
               TestIF_File.dSiteYPitch<=IN_OUT_ARM_Y_PITCH_MAX/100.0)
                OutArmSuck.iYStep=1;
            else
                OutArmSuck.iYStep=2;
        }
        else
        {
            int iYPitch=GetVariableYOutShuttleData();
            if((TestIF_File.dSiteYPitch*100)==iYPitch)
                OutArmSuck.iYStep=1;
            else
                OutArmSuck.iYStep=2;
        }
    }

    if(USE_OUT_SORT_ARM!=eartUninstall)                                         //RogerYang 20250521 add for 9046AU
    {
        if(TestIF_File.iTestMode==SingleSite   ||
           TestIF_File.iTestMode==TriSite1X3   ||
           TestIF_File.iTestMode==QualSite1X4  ||
           TestIF_File.iTestMode==DualSite2x1)
        {
            OutArm2Suck.iYStep=1;
        }
        else
        {
            OutArm2Suck.iYStep=2;
        }
    }
}
//------------------------------------------------------------------------------
void SetArmRowCount()                                                           //Steven 20250401 : 整合in arm參數設定
{
    OutArmSuck.iXStep=InArmSuck.iXStep;
//    OutArmSuck.iYStep=InArmSuck.iYStep;                                       //Rogeryang 20260108 : 9011UC in/outarm模組不同需拆開
    OutArmSuck.iModeX=InArmSuck.iModeX;
    FLCarryKit.SetPickerCount(InArmSuck.iShtRow, InArmSuck.iShtCol);
    BLCarryKit.SetPickerCount(InArmSuck.iShtRow, InArmSuck.iShtCol);
    FRCarryKit.SetPickerCount(InArmSuck.iShtRow, InArmSuck.iShtCol);
    BRCarryKit.SetPickerCount(InArmSuck.iShtRow, InArmSuck.iShtCol);
    FTestSuck.SetPickerCount(InArmSuck.iShtRow, InArmSuck.iShtCol);             //Steven 20241002 : TestSuck補上 Row / Col
    BTestSuck.SetPickerCount(InArmSuck.iShtRow, InArmSuck.iShtCol);

    OutSht3Kit.SetPickerCount(InArmSuck.iShtRow, InArmSuck.iShtCol);            //RogerYang 20250514 Add for 9046AU

    if(iInArmType==e9045_1x4_1_Ac)                                              //Steven 20250820 : fix for 1x4_AC
    {
        TestSocket.SetPickerCount(1, 4);
    }
    else if(IsNNMode()==None_NN)
    {
        TestSocket.SetPickerCount(InArmSuck.iShtRow, InArmSuck.iShtCol);
    }
    else
    {
        TestSocket.SetPickerCount(InArmSuck.iShtRow+InArmSuck.iShtRow, InArmSuck.iShtCol);
    }
}
//------------------------------------------------------------------------------
void SetInOutArmParameter()                                                     //Steven 20250401 : 整合in arm參數設定
{
    if(InArmSuck.HasIC()==false && OutArmSuck.HasIC()==false)                   //Steven 20230921 : 修正LED例外異常
    {
        CopyKitSuck(&InArmSuckBackup,  &InArmSuck);                             //Frank HT1032 add
        CopyKitSuck(&OutArmSuckBackup, &OutArmSuck);

        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                CopyInitSuck(&InArmSuckBackup , &InArmSuck , i, j, i, j);
                CopyInitSuck(&OutArmSuckBackup, &OutArmSuck, i, j, i, j);
            }
        }

        if(USE_PICKER_COUNT==ep1Picker && iInArmType==e9045_2x2_4_13)
        {
            SetInOutArmParameter_All_1Pick();
        }
        else if(iInArmType==e9045_1x1_1)
        {
            SetInOutArmParameter_1x1_1();
        }
        else if(iInArmType==e9045_1x2_2_14 ||
                iInArmType==e9045_1x2_2_13)
        {
            SetInOutArmParameter_1x2_2();
        }
        else if(iInArmType==e9045_1x2_4_Hot)
        {
            SetInOutArmParameter_1x2_4_Hot();
        }
        else if(iInArmType==e9045_1x3_2_14)
        {
            SetInOutArmParameter_1x3_2_14();
        }
        else if(iInArmType==e9045_1x3_4)
        {
            SetInOutArmParameter_1x3_4();
        }
        else if(iInArmType==e9045_1x4_4_13)
        {
            SetInOutArmParameter_1x4_4_13();
        }
        else if(iInArmType==e9045_1x4_2_14)
        {
            SetInOutArmParameter_1x4_2();
        }
        else if(iInArmType==e9045_1x4_4_Back ||
                iInArmType==e9045_1x4_4 ||
                iInArmType==e9045_1x4_1_Ac)
        {
            SetInOutArmParameter_1x4_4();
        }
        else if(iInArmType==e9045_1x4_8_Hot)
        {
            SetInOutArmParameter_1x4_8_Hot();
        }
        else if(iInArmType==e9045_2x1_2_13)
        {
            SetInOutArmParameter_2x1_2();
        }
        else if(iInArmType==e9045_2x2_4_14 ||
                iInArmType==e9045_2x2_4_12 ||
                iInArmType==e9045_2x2_4_13)
        {
            SetInOutArmParameter_2x2_4();
        }
        else if(iInArmType==e9045_2x2_8_Hot)
        {
            SetInOutArmParameter_2x2_8_Hot();
        }
        else if(iInArmType==e9045_2x3_6_14)
        {
            SetInOutArmParameter_2x3_6_14();
        }
        else if(iInArmType==e9045_2x3_6)
        {
            SetInOutArmParameter_2x3_6();
        }
        else if(iInArmType==e9045_2x4_4_13 ||
                iInArmType==e9045_2x4_4_14)
        {
            SetInOutArmParameter_2x4_4();
        }
        else if(iInArmType==e9045_2x4_8)
        {
            SetInOutArmParameter_2x4_8();
        }
        else if(iInArmType==e9045_2x5_8)
        {
            SetInOutArmParameter_2x5_8();
        }
        else if(iInArmType==e9045_2x6_8)
        {
            SetInOutArmParameter_2x6_8();
        }
        else if(iInArmType==e9045_2x8_8)
        {
            SetInOutArmParameter_2x8_8();
        }
    //    else if(iInArmType==e9045_2x8_16)
    //    {
    //        SetInOutArmParameter_2x8_16();
    //    }
        else if(iInArmType==e9045_2x8_32)
        {
    //    if(USE_PICKER_COUNT==ep16Picker)
    //        SetInOutArmParameter_2x8_16();
    //    else
            SetInOutArmParameter_2x8_8();
        }
        SetArmRowCount();
    }
}
//-----------------------------------------------------------------------------
void DoInArm_9045_Type()                                                        //Steven 20201014 : 將DoInArm_9045_Type獨立,避免DoInArm_9045還沒執行就被使用
{
    double LoaderXPitch=UserDefForm_File[TrayForm.Loader.iTrayType].XPitch;
//    int Ld=TrayForm.Loader.iTrayType;
    int iModeX;
    InArmSuck.iXStep=1;
    OutArm2Suck.iXStep=1;                                                       //RogerYang 20250521 add for 9046AU
    CheckInArmYStep();                                                          //Steven 20240205 : Y-Pitch可否一次放

//    if(USE_PICKER_COUNT==ep1Picker)                                           //JerryYang 20250830 : todo
//    {
//        iInArmType=e9045_1x1_1;
//    }
//    else
    if(TestIF_File.iTestMode==SingleSite)
    {
        iInArmType=e9045_1x1_1;
    }
    else if(TestIF_File.iTestMode==DualSite ||                                  // 1x2
            TestIF_File.iTestMode==QualSite2X2N)                                //Frank 20200520 2X2NN Mode
    {
        if(USE_PICKER_COUNT==0)                                                 //Steven 20161117 : for HT-9045S
        {
            if(TestIF_File.iUseSuckMode==4)                                     //use 4 pick unit
            {
                if(ArmXCanSuck2IC_9045S()==false ||                             //先確認可不可以跑4吸嘴
                   ArmYCanSuck2IC()==false)
                {
                    TestIF_File.iUseSuckMode=2;
                }
            }

            if(TestIF_File.iUseSuckMode==4)                                     //use 4 pick unit
            {
                iInArmType=e9045_1x2_4_Hot;
                if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM)
                    InArmSuck.iXStep=2;
            }
            else
            {
                iInArmType=e9045_1x2_2_13;
                if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM)
                    InArmSuck.iXStep=2;
            }
        }
        else if(USE_16PICKER_TYPE==1)                                           //Ztex 2023.12.06 Add HT-1032
        {
        }
        else
        {
            if(TestIF_File.iUseSuckMode==4)                                     //use 4 pick unit
            {
                if(TestIF_File.dSiteXPitch<iXpitchMaxX3_MM &&                   //先確認可不可以跑4吸嘴
                   (ArmXCanSuck4IC_9045()==false &&
                    ArmYCanSuck2IC()==false))
                {
                    TestIF_File.iUseSuckMode=2;
                }
                else if(TestIF_File.iTestMode==QualSite2X2N)
                {
                    TestIF_File.iUseSuckMode=2;
                }
            }

            if(TestIF_File.iUseSuckMode==4 &&
               TestIF_File.iTestMode!=QualSite2X2N)                             //Ifor 20260123 add:
            {
                iInArmType=e9045_1x2_4_Hot;
                if(ArmXCanSuck4IC_9045()==false)
                {
                    i1x2_4UseACEGPicker=0;                                      //使用Aa, Ad, Ba, Bd
                    if(TestIF_File.dSiteXPitch>iXpitchMaxX3_MM)
                        InArmSuck.iXStep=2;
                }
                else
                {
                    i1x2_4UseACEGPicker=1;                                      //Steven 20230530 : 1x2_4改用Row A
                    if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM)
                        InArmSuck.iXStep=2;
                }
            }
            else
            {
//                if(TestIF_File.dSiteXPitch>iXpitchMaxX3_MM)                   //Steven 20250701 : Mark for 1x2
//                {
//                    iInArmType=e9045_1x2_2_14;
//                    InArmSuck.iXStep=2;
//                }
//                else if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM)
//                {
//                    iInArmType=e9045_1x2_2_14;
//                }
//                else
//                {
//                    if(LastSet.iTemperature==Tempture_Hot)                      //Steven 20250716 : 修正HP item4 1x2/2x2 NN mode
//                    {
//                        if((Prod.HotPlateForm[0].iXItem==4  && Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/2)) ||        //HP X-Pitch >40mm
//                           (Prod.HotPlateForm[0].iXItem==6  && Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/3+1)) ||      //HP X-Pitch >28mm
//                           (Prod.HotPlateForm[0].iXItem==8  && Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/4)) ||        //HP X-Pitch >10mm
//                           (Prod.HotPlateForm[0].iXItem==10 && Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/5)) ||        //HP X-Pitch >8mm
//                           (Prod.HotPlateForm[0].iXItem==12 && Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/6)) ||        //HP X-Pitch >7mm
//                           (Prod.HotPlateForm[0].iXItem==16 && Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/6)))          //HP X-Pitch >7mm
//                        {
//                            iInArmType=e9045_1x2_2_14;
//                        }
//                        else
//                        {
//                            iInArmType=e9045_1x2_2_13;
//                        }
//                    }
//                    else
                    {
                        if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM)
                        {
                            iInArmType=e9045_1x2_2_14;
                            if(TestIF_File.dSiteXPitch>iXpitchMaxX3_MM)         //Steven 202507023 : Fixed for 1x2
                            {
                                InArmSuck.iXStep=2;
                            }
                            else
                            {
                                InArmSuck.iXStep=1;
                            }
                        }
                        else if(LastSet.iTemperature==Tempture_Hot &&
                                Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/3+1) &&                                      //RogerYang 20250827 修正HP Pitch>26.66且iXItem=6使用14吸嘴
                                Prod.HotPlateForm[0].iXItem==6)
                        {
                            iInArmType=e9045_1x2_2_14;
                        }
                        else
                        {
                            iInArmType=e9045_1x2_2_13;
                        }
                    }
//                }
            }
        }
    }
    else if(TestIF_File.iTestMode==TriSite1X3 ||                                //1x3 //Frank 20160329 add for 1x3_4
            TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
    {
        iInArmType=e9045_1x3_4;
//        if(USE_PICKER_COUNT==0)                                                 //Steven 20161117 : for HT-9045S
//        {
//            iInArmType=e9045_1x3_2_13;
//            if(TestIF_File.dSiteXPitch<=iXpitchMaxX2_MM)
//                InArmSuck.iXStep=1;
//            else
//                InArmSuck.iXStep=2;
//        }
//        else
        {
            iModeX=CheckPickerMode(4, true, false);                             //Steven 20240427 : fixed for 1x3
            if(iModeX==14)
                iInArmType=e9045_1x3_2_14;
        }
    }
    else if(TestIF_File.iTestMode==QualSite1X4 ||                               // 1x4
            TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
    {
        iInArmType=e9045_1x4_4;

        if(LastSet.iTemperature==Tempture_Hot &&                                //Steven 20240308 : 修正HP item4 X-Pitch 45mm
           Prod.HotPlateForm[0].iXPitch>4000)
        {
            TestIF_File.iUseSuckMode=2;
        }

        if(TestIF_File.iTestMode==QualSite1X4 &&
           CosFunction.b1x4OnlyAaUse1x1Mode &&                                  //Steven 20200720 : 1x4只開site Ac, 跑1x1模式
           TestIF_File.iSiteMap[0][0]<=0 &&
           TestIF_File.iSiteMap[0][1]<=0 &&
           TestIF_File.iSiteMap[0][3]<=0)
        {
            TestIF_File.iUseSuckMode=4;
            iInArmType=e9045_1x4_1_Ac;
        }
        else if(USE_PICKER_COUNT==0)                                            //Steven 20161117 : for HT-9045S
        {
            TestIF_File.iUseSuckMode=4;
            iInArmType=e9045_1x4_4_13;

            if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM)
                InArmSuck.iXStep=2;
        }
        else
        {
            if(TestIF_File.iUseSuckMode==8)
            {
                if(CheckPickerMode(4, true, false)!=1234 ||
                   ArmYCanSuck2IC()==false ||
                   LoaderXPitch>=iXpitchMaxX1_MM ||                             //Steven 20200327 : Tray form大於40的,不能跑8吸嘴)  //Steven 20240221 : > --> >=
                   TestIF_File.bInArmUseBackRowSuck==true)                      //jou 20161122 (Steven) In & Out arm use back row suck
                {
                    TestIF_File.iUseSuckMode=4;
                }
            }

            if(TestIF_File.iUseSuckMode==8)                                     //jou 980525 1x4 select 4 or 8 pick unit
            {
                iInArmType=e9045_1x4_8_Hot;
                if(USE_16PICKER_TYPE==1)                                        //Ztex 2023.12.06 Add HT-1032
                {
                    if(TestIF_File.dSiteXPitch>iXpitchMaxX4_MM)
                        InArmSuck.iXStep=4;
                    else if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM)
                        InArmSuck.iXStep=2;
                }
                else
                {
                    if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM)
                        InArmSuck.iXStep=4;
                    else if(TestIF_File.dSiteXPitch>iXpitchMaxX1_MM)            //Sam 20250203 : 再改回 > //Steven 20240221 : > --> >=
                        InArmSuck.iXStep=2;
                }
            }
            else
            {
                iModeX=CheckPickerMode(4, true, false);                         //JerryYang 20231003 1x4 14吸嘴
                if(iModeX==13)
                {
                    iInArmType=e9045_1x4_4_13;
                }
                else if(iModeX==14)
                {
                    iInArmType=e9045_1x4_2_14;
                }
                else
                {
                    if(TestIF_File.bInArmUseBackRowSuck==true)                  //jou 20161122 (Steven) In & Out arm use back row suck
                        iInArmType=e9045_1x4_4_Back;
                    else
                        iInArmType=e9045_1x4_4;
                }
            }
        }
    }
    else if(TestIF_File.iTestMode==DualSite2x1)
    {
        iInArmType=e9045_2x1_2_13;
    }
    else if(TestIF_File.iTestMode==QualSite2X2)                                 //2x2
    {
        InArmSuck.iXStep=1;
        if(USE_PICKER_COUNT==ep1Picker)                                         //JerryYang 20250830 : todo
        {
            iInArmType=e9045_2x2_4_13;
        }
        else if(USE_PICKER_COUNT==0)                                            //Steven 20161117 : for HT-9045S
        {
            TestIF_File.iUseSuckMode=4;
            iInArmType=e9045_2x2_4_13;
            if(TestIF_File.dSiteXPitch>iXpitchMaxX3_MM)
                InArmSuck.iXStep=2;
        }
        else if(USE_16PICKER_TYPE==1)                                           //Ztex 2023.12.06 Add HT-1032
        {
        }
        else
        {
            if(TestIF_File.iUseSuckMode==8)
            {
                if(LoaderXPitch>iXpitchMaxX1_MM ||                              //Steven 20200327 : Tray form大於40的,不能跑8吸嘴
                   ArmXCanSuck4IC_9045()==false)
                {
                    TestIF_File.iUseSuckMode=4;
                }
            }

            if(TestIF_File.iUseSuckMode==8)
            {
                iInArmType=e9045_2x2_8_Hot;
                if(TestIF_File.dSiteXPitch>iXpitchMaxX4_MM &&
                   TestIF_File.dSiteXPitch>iXpitchMinX4_MM)
                {
                    InArmSuck.iXStep=2;
                }
            }
            else
            {
//                if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM)
//                {
//                    iInArmType=e9045_2x2_4_14;
//                    if(TestIF_File.dSiteXPitch>iXpitchMaxX3_MM ||
//                       TestIF_File.dSiteXPitch<iXpitchMinX3_MM)
//                        InArmSuck.iXStep=2;
//                }
//                else if(TestIF_File.dSiteXPitch<iXpitchMinX2_MM)
//                {
//                    iInArmType=e9045_2x2_4_12;
//                    if(TestIF_File.dSiteXPitch>iXpitchMaxX1_MM ||
//                       TestIF_File.dSiteXPitch<iXpitchMinX1_MM)
//                        InArmSuck.iXStep=2;
//                }
//                else
//                {
//                    if(LastSet.iTemperature==Tempture_Hot)                      //Steven 20240308 : 修正HP item4 X-Pitch 45mm
//                    {
//                        if((Prod.HotPlateForm[0].iXItem==4  && Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/2)) ||
//                           (Prod.HotPlateForm[0].iXItem==6  && Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/3+1)) ||
//                           (Prod.HotPlateForm[0].iXItem==8  && Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/4)) ||
//                           (Prod.HotPlateForm[0].iXItem==10 && Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/5)) ||
//                           (Prod.HotPlateForm[0].iXItem==12 && Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/6)) ||
//                           (Prod.HotPlateForm[0].iXItem==16 && Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/6)))
//                        {
//                            iInArmType=e9045_2x2_4_14;
//                            if(TestIF_File.dSiteXPitch>iXpitchMaxX3_MM ||
//                               TestIF_File.dSiteXPitch<iXpitchMinX3_MM)
//                                InArmSuck.iXStep=2;
//                        }
//                        else
//                        {
//                            iInArmType=e9045_2x2_4_13;
//                            if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM ||
//                               TestIF_File.dSiteXPitch<iXpitchMinX2_MM)
//                                InArmSuck.iXStep=2;
//                        }
//                    }

                    if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM)
                    {
                        iInArmType=e9045_2x2_4_14;
                        if(TestIF_File.dSiteXPitch>iXpitchMaxX3_MM)             //Steven 202507023 : Fixed for 1x2
                        {
                            InArmSuck.iXStep=2;
                        }
                        else
                        {
                            InArmSuck.iXStep=1;
                        }
                    }
                    else if(LastSet.iTemperature==Tempture_Hot &&
                            Prod.HotPlateForm[0].iXPitch>(iXpitchMaxX2/3+1) &&  //RogerYang 20250827 修正HP Pitch>26.66且iXItem=6使用14吸嘴
                            Prod.HotPlateForm[0].iXItem==6)
                    {
                        iInArmType=e9045_2x2_4_14;
                    }
                    else if(LastSet.iTemperature==Tempture_Hot &&               //AI(ht9045-inarm-flow) 20260512 (RogerYang) : HP 12x20 XP*6>80mm
                            Prod.HotPlateForm[0].iXItem==12 &&
                            Prod.HotPlateForm[0].iXPitch*6>iXpitchMaxX2)
                    {
                        iInArmType=e9045_2x2_4_14;
                    }
                    else
                    {
                        iInArmType=e9045_2x2_4_13;
                        if(TestIF_File.dSiteXPitch>iXpitchMaxX2_MM ||
                           TestIF_File.dSiteXPitch<iXpitchMinX2_MM)
                            InArmSuck.iXStep=2;
                    }
//                }
            }
        }
    }
    else if(TestIF_File.iTestMode==_6Site2X3)                                   //ChungHung 20140115 add for 2x3_6
    {
        iInArmType=e9045_2x3_6;
        if(LastSet.iTemperature==Tempture_Hot &&                                //Steven 20241023 : 修正HP item4 X-Pitch 45mm
           Prod.HotPlateForm[0].iXPitch>4000)
        {
            iModeX=CheckPickerMode(4, true, false);
            if(iModeX==14)
            {
                iInArmType=e9045_2x3_6_14;
            }
            else
            {
            }
        }
        else if(USE_PICKER_COUNT==0)                                            //Steven 20161117 : for HT-9045S
        {
//            iInArmType=e9045_2x3_6_13;
        }
        else
        {
            iModeX=CheckPickerMode(4, true, false);
            if(iModeX==14)
            {
                iInArmType=e9045_2x3_6_14;
            }
            else
            {
            }
        }
    }
    else if(TestIF_File.iTestMode==_8Site2X4 ||                                 // 2x4
            TestIF_File.iTestMode==_16Site4X4)                                  //Sam 20190226 : 16Site4X4
    {
        iInArmType=e9045_2x4_8;

        if(LastSet.iTemperature==Tempture_Hot &&                                //Steven 20241023 : 修正HP item4 X-Pitch 45mm
           Prod.HotPlateForm[0].iXPitch>4000)
        {
            iModeX=CheckPickerMode(4, true, false);
            if(iModeX==14)
                iInArmType=e9045_2x4_4_14;
        }
        else if(USE_PICKER_COUNT==0)                                            //Steven 20161117 : for HT-9045S
        {
            iInArmType=e9045_2x4_4_13;
            if(TestIF_File.dSiteXPitch<=iXpitchMaxX2_MM)
                InArmSuck.iXStep=1;
            else
                InArmSuck.iXStep=2;
        }
        else
        {
            iModeX=CheckPickerMode(4, true, false);
            if(iModeX==14)
                iInArmType=e9045_2x4_4_14;
        }
        OutArm2Suck.iXStep=2;                                                   //RogerYang 20250710 Add for 9046AU
    }
    else if(TestIF_File.iTestMode==_10Site2X5)                                  // 2x5  //Steven 20211220 : for 2x5
    {
        iInArmType=e9045_2x5_8;
        iModeX=CheckPickerMode(4, false, false);
    }
    else if(TestIF_File.iTestMode==_12Site2X6)
    {
        iInArmType=e9045_2x6_8;
        iModeX=CheckPickerMode(4, false, false);
    }
    else if(TestIF_File.iTestMode==_16Site2X8)                                  // 2x8
    {
        iInArmType=e9045_2x8_8;
        iModeX=CheckPickerMode(4, false, false);
        OutArm2Suck.iXStep=4;                                                   //RogerYang 20250710 Add for 9046AU
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {
        iInArmType=e9045_2x8_32;
        iModeX=CheckPickerMode(4, false, false);
    }

    if(InArmSuck.iXStep==1)
        InArmSuck.iModeX=0;
    else if(InArmSuck.iXStep==2)
        InArmSuck.iModeX=1;
    else                                                                        //if(InArmSuck.iXStep==4)
        InArmSuck.iModeX=2;
//    else
//        InArmSuck.iModeX=3;

    fSetup->CheckSTMMode();                                                     //Steven 20221006 : 確認STM模式
    SetInOutArmParameter();
}
//-----------------------------------------------------------------------------
//jou 2010-01-23 for contact do auto-high use
//CheckItem=0 : 兩邊都檢查
//CheckItem=1 : 檢查Shuttle 1
//CheckItem=2 : 檢查Shuttle 2
//-----------------------------------------------------------------------------
bool CheckShuttleSensor_9045(int CheckItem)                                     //Steven 20150415 : 加上CheckItem,可以判別哪邊不用做檢查
{
    int ret1=1, ret2=1;
    switch(TestIF.iTestMode)
    {
        case SingleSite:
            if(CheckItem!=2)
                ret1=CheckShuttleSensor_9045_1x1(0, true);
            else
                ret1=0;

            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                TestIF_File.bArm1PickPlaceArm2Test==true) ||                    //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
               CheckItem==1)
                ret2=0;
            else
                ret2=CheckShuttleSensor_9045_1x1(1, true);
            break;
        case DualSite:                                                          //1x2
        case QualSite2X2N:                                                      //Frank 20200520 2X2NN Mode
            if(CheckItem!=2)
                ret1=CheckShuttleSensor_9045_1x2(0, true);
            else
                ret1=0;

            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                TestIF_File.bArm1PickPlaceArm2Test==true) ||                    //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
               CheckItem==1)
                ret2=0;
            else
                ret2=CheckShuttleSensor_9045_1x2(1, true);
            break;
        case DualSite2x1:
            if(CheckItem!=2)
                ret1=CheckShuttleSensor_9045_2x1(0, true);
            else
                ret1=0;

            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                TestIF_File.bArm1PickPlaceArm2Test==true) ||                    //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
               CheckItem==1)
                ret2=0;
            else
                ret2=CheckShuttleSensor_9045_2x1(1, true);
            break;
        case QualSite2X2:                                                       //2x2
            if(CheckItem!=2)
                ret1=CheckShuttleSensor_9045_2x2(0, true);
            else
                ret1=0;

            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                TestIF_File.bArm1PickPlaceArm2Test==true) ||                    //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
               CheckItem==1)
                ret2=0;
            else
                ret2=CheckShuttleSensor_9045_2x2(1,true);
            break;
        case QualSite1X4:                                                       //1x4
        case _8Site1X4:                                                         //ChungHung 20150528 add for 海思 _8Site1x4
        case _8Site2X4N:                                                        //Wei 20231211 : 2X4NN Mode
            if(CheckItem!=2)
                ret1=CheckShuttleSensor_9045_1x4(0, true);
            else
                ret1=0;

            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                TestIF_File.bArm1PickPlaceArm2Test==true) ||                    //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
               CheckItem==1)
                ret2=0;
            else
                ret2=CheckShuttleSensor_9045_1x4(1,true);
            break;
        case TriSite1X3:                                                        //Frank 20160329 add for 1x3_4
        case _6Site2X3:                                                         //ChungHung 20140115 add for 2x3_6
        case _6Site2X3N:                                                        //Steven 20220425 : 2X3NN Mode
            if(CheckItem!=2)
                ret1=CheckShuttleSensor_9045_2x3(0, true);
            else
                ret1=0;

            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                TestIF_File.bArm1PickPlaceArm2Test==true) ||                    //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
               CheckItem==1)
                ret2=0;
            else
                ret2=CheckShuttleSensor_9045_2x3(1, true);
            break;
        case _16Site4X4:                                                        //Sam 20190226 : 16Site4X4
        case _8Site2X4:                                                         //2x4
            if(CheckItem!=2)
                ret1=CheckShuttleSensor_9045_2x4(0, true);
            else
                ret1=0;

            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                TestIF_File.bArm1PickPlaceArm2Test==true) ||                    //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
               CheckItem==1)
                ret2=0;
            else
                ret2=CheckShuttleSensor_9045_2x4(1, true);
            break;
        case _10Site2X5:                                                        //2x5 //wei 20190614 10 site
            if(CheckItem!=2)
                ret1=CheckShuttleSensor_9045_2x5(0, true);
            else
                ret1=0;

            if(TestIF_File.bArm1PickPlaceArm2Test || CheckItem==1)
                ret2=0;
            else
                ret2=CheckShuttleSensor_9045_2x5(1, true);
            break;
        case _12Site2X6:                                                        //2x6
            if(CheckItem!=2)
                ret1=CheckShuttleSensor_9045_2x6(0, true);
            else
                ret1=0;

            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                TestIF_File.bArm1PickPlaceArm2Test==true) ||                    //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
               CheckItem==1)
                ret2=0;
            else
                ret2=CheckShuttleSensor_9045_2x6(1, true);
            break;
        case _16Site2X8:                                                        //2x8
            if(CheckItem!=2)
                ret1=CheckShuttleSensor_9045_2x8(0, true);
            else
                ret1=0;

            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                TestIF_File.bArm1PickPlaceArm2Test==true) ||                    //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
               CheckItem==1)
                ret2=0;
            else
                ret2=CheckShuttleSensor_9045_2x8(1, true);
            break;
        case _32Site4X8N:                                                       //4x8
            if(CheckItem!=2)
                ret1=CheckShuttleSensor_9045_2x8(0, true);
            else
                ret1=0;

            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                TestIF_File.bArm1PickPlaceArm2Test==true) ||                    //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
               CheckItem==1)
                ret2=0;
            else
                ret2=CheckShuttleSensor_9045_2x8(1, true);
            break;
    }

    if(ret1==0 && ret2==0)
    {
        return true;                                                            //OK
    }
    return false;
}
//==============================================================================
void DoInArmSuckPreOn(int iXPos, int iYPos)
{
    int iPosX=0, iPosY=0;
    int iPreRange=50;

    if(LastSet.iRealDummy==REALLY &&
       ArmSpeed[InArm].bSuckOnDown &&
       Sen[SnRKManualTStart].IsOn()==false)
    {
        bPickFromLoader=true;                                                   //Steven 20171226 (Wei) : 確認Loader吸取完成
        iPosX=MOT[MInArmX].ReadPos();
        iPosY=MOT[MInArmY].ReadPos();
        if(iPosX>=iXPos-iPreRange && iPosX<=iXPos+iPreRange &&
           iPosY>=iYPos-iPreRange && iPosY<=iYPos+iPreRange)
        {
            for(int i=0; i<MAX_ARM_Row; i++)
                for(int j=0; j<MAX_ARM_Col; j++)
                    if(InArmSuckUse[i][j] && InArmSuck.Item[i][j]==NULL_IC)
                        InArmSuck.Suck[i][j].On();
        }
    }
}
//==============================================================================
int GetVariableYInShuttleData()                                                 //Steven 20230519 : 改成泛用型計算Y-Pitch
{
    if(USE_IN_Y_IS_AUTO_PITCH==true)                                            //Steven for HT1032  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(InArmSuck.iShtRow==1 ||                                              //Steven 20240311 : iPickRow --> iShtRow
           iInArmType==e9045_1x4_8_Hot ||
           iInArmType==e9045_1x2_4_Hot ||
           iInArmType==e9045_2x2_8_Hot)                                         //JerryYang 20231003
        {
            if(MachineTypeChoice==Type_HT9046_LS &&
               SubMachineType==Type_HT9016C)                                    //Jimmychiu 20230207 Add TempPosShift for HT9016C
            {
                if(Prod.LoadForm.iYPitch>=IN_OUT_ARM_Y_PITCH_MIN &&
                   Prod.LoadForm.iYPitch<=IN_OUT_ARM_Y_PITCH_MAX)               //Min Y pitch 1500 in HT9016c
                {
                    return Prod.LoadForm.iYPitch;
                }
                else
                {
                    return IN_OUT_ARM_Y_PITCH_MIN;
                }
            }
            else
            {
                return 6000;
            }
        }

        if(TestIF.dSiteYPitch>=IN_OUT_ARM_Y_PITCH_MAX)
            return IN_OUT_ARM_Y_PITCH_MAX;
        else if(TestIF.dSiteYPitch<=IN_OUT_ARM_Y_PITCH_MIN)
            return IN_OUT_ARM_Y_PITCH_MIN;

        return TestIF.dSiteYPitch;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635 &&
            TestIF_File.iARM_Y_PITCH==6350)                                     //Steven 20240306 : 修正Y-Pitch異常問題
    {
        return 6350;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual360 &&
            TestIF_File.iARM_Y_PITCH==3600)                                     //Steven 20240306 : 修正Y-Pitch異常問題
    {
        return 3600;
    }
    return 6000;
}
//==============================================================================
void InArmSuckReset()
{
    for(int i=0; i<MAX_ARM_Row; i++)                                            //wei 20150318 suck完就等於true
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            bSuckEnd[i][j]=false;
        }
    }
}
//==============================================================================
bool CheckPlaceToShuttle()
{
    bool bResult=false;
    int  iCenter;

    if(CosFunction.bUseInitialDelayAsSoakTime &&
       Temperature.bUseInitialDelayAsSoakTime)
    {
        if(bFirstInput)
        {
            for(int i=0; i<MAX_ARM_Row; i++)
                for(int j=0; j<MAX_ARM_Col; j++)
                    if(InArmSuck.Item[i][j]==HAS_IC)
                        InArmSuck.SetItemData(i, j, HAS_HOT_IC);

            if(TestIF.iTestMode==_16Site2X8 ||
               TestIF.iTestMode==_12Site2X6 ||
               TestIF.iTestMode==_32Site4X8N)
            {
                if(TestIF.iTestMode==_12Site2X6)
                    iCenter=3;
                else
                    iCenter=4;

                if((TestIF_File.iShuttleMode==0 ||
                   (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0)) &&
                   (FLCarryKit.LeftSideNoIC(iCenter)==true ||
                    FLCarryKit.RightSideNoIC(iCenter)==true))
                {
                    InArmSuck.iWhichShtPickFor32=0;                             //Ifor 20260317 : fix should be 0 for FLCarryKit(Shuttle1)
                    InArmSuck.iWhichKitPickFor32=InArmSuck.iWhichKit;
                    InitInArmPlaceToShuttleTask();
                    bPlaceShuttle=true;
                    bResult=true;
                }

                if(bResult==false &&
                   (BLCarryKit.LeftSideNoIC(iCenter)==true ||
                    BLCarryKit.RightSideNoIC(iCenter)==true))
                {
                    InArmSuck.iWhichShtPickFor32=1;                             //Ifor 20260317 : fix should be 1 for BLCarryKit(Shuttle2)
                    InArmSuck.iWhichKitPickFor32=InArmSuck.iWhichKit;
                    InitInArmPlaceToShuttleTask();
                    bPlaceShuttle=true;
                    bResult=true;
                }
            }
            else
            {
                if((TestIF_File.iShuttleMode==0 ||
                   (TestIF_File.iShuttleMode==1 &&
                    TestIF_File.iShuttle_Sel==0)) &&
                   FLCarryKit.UseSiteNoIC())
                {
                    InArmSuck.iWhichShtPickFor32=0;                             //Ifor 20260317 : fix should be 0 for FLCarryKit(Shuttle1)
                    InArmSuck.iWhichKitPickFor32=InArmSuck.iWhichKit;
                    InitInArmPlaceToShuttleTask();
                    bPlaceShuttle=true;
                    bResult=true;
                }

                if(bResult==false && BLCarryKit.UseSiteNoIC())
                {
                    InArmSuck.iWhichShtPickFor32=1;                             //Ifor 20260317 : fix should be 1 for BLCarryKit(Shuttle2)
                    InArmSuck.iWhichKitPickFor32=InArmSuck.iWhichKit;
                    InitInArmPlaceToShuttleTask();
                    bPlaceShuttle=true;
                    bResult=true;
                }
            }
        }
    }
    return bResult;
}
//==============================================================================
void AutoSkipHasIClog(AnsiString ZName, int iX, int iY)                         //kevin 20170904 (wei) 記錄每一盤 AutoSkip 後有吸到IC 紀錄XY 位置
{
    AnsiString sBuffer="";
    if(bASkStart ||
       bAutoSkiplog)                                                            //kevin 20170224 (wei) Load Auto Skip 開始有吸取到空格
    {                                                                           //kevin 20170904 (wei) 記錄SKIP TRAY X Y 位置
        if(bASkStart)
        {
            bAutoSkipHasIC=true;
            sBuffer.sprintf("(X:%d,Y:%d),", iX+1, iY+1);                        //kevin 20170224 (wei) LOAD AUTO SKIP HAS IC data
            sAskStartDetect+=sBuffer;
        }
        sAutoTrayendabnormalBuffer.sprintf("(X:%d,Y:%d),", iX+1, iY+1);         //kevin 20170901
        sAutoTrayendabnormalPos+=sAutoTrayendabnormalBuffer;                    //kevin 20170823 auto tray end 中途有料 記錄一盤ＸＹ位置
        sAutoTrayendabnormalBuffer="";
        bAutoTrayEndHasIC=true;                                                 //kevin 20170904 add auto tray end 有吸到IC
        iAutoTrayendhasIC++;                                                    //kevin 20170314 (Steven) 吸到IC數量計數
    }
}
//------------------------------------------------------------------------------
void SetAutoSkipCount(int Add)                                                  //Steven 20150217 : 顯示Auto Skip的數量, 1=++, 0=清空計數
{
    AnsiString sData="";                                                        //kevin 20170211
    if(Add==0)
    {
        if(iAutoSkipCT!=0)                                                      //kevin 20170301
        {
            AutoTrayendEndTime[0]=MyTickCount();
            AutoTrayendPassTime[0]=(AutoTrayendEndTime[0]-AutoTrayendStartTime[0])/1000;
            sData.sprintf("Auto Tray End Finished:%s,IC=%d,Empty=%d ,%d", Now().FormatString("yyyymmdd-hh:nn"), iAutoTrayendhasIC, iAutoTrayEndTotal, AutoTrayendPassTime[0]);
            if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
               ArmSpeed[InArm].bAutoSKIP==true)                                 //kevin 20210811  有使用 AutoSkip 再記錄
                RecordProcess(sData);
            iAse_LoadSkipTime=0;
        }

        if(iAse_LoadTrayEndTotalTime==2)
        {
            iAse_LoadTrayEndTotalTime=0;
            AutoTrayendEndTime[1]=MyTickCount();
            AutoTrayendPassTime[1]=(AutoTrayendEndTime[1]-AutoTrayendStartTime[1])/1000;
            sData.sprintf("Auto Tray End Finished Total time:%s,IC=%d,Empty=%d ,%d", Now().FormatString("yyyymmdd-hh:nn"), iAutoTrayendhasIC, iAutoTrayEndTotal, AutoTrayendPassTime[1]);
            if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
               ArmSpeed[InArm].bAutoSKIP==true)                                 //kevin 20210811  有使用 AutoSkip 再記錄
                RecordProcess(sData);
            iAutoTrayEndTotal=0;
            iAutoTrayendhasIC=0;
        }
        iAutoSkipCT=0;                                                          //jou 2013-09-23 add Auto SKIP CT
        iInArmWaitPosition=0;                                                   //Ifor 20210209 add:旗標未清除導致Tray Arm 不做動
    }
    else
    {
        if(iAse_LoadSkipTime==0|| iAse_LoadTrayEndTotalTime==0)                 //kevin 20170320 (Steven) add auto skip total time
        {
            AutoTrayendStartTime[0]=MyTickCount();
            if(iAse_LoadTrayEndTotalTime==0)
            {
                AutoTrayendStartTime[1]=MyTickCount();
                iAse_LoadTrayEndTotalTime=1;

                sData.sprintf(" Auto Tray End Start: %s",DateTimeToStr(Now()));
                if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
                   ArmSpeed[InArm].bAutoSKIP==true)                             //kevin 20210811  有使用 AutoSkip 再記錄
                   RecordProcess(sData);
            }
            sData.sprintf(" Auto Tray End has IC Start: %s",DateTimeToStr(Now()));
            if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
               ArmSpeed[InArm].bAutoSKIP==true)                                 //kevin 20210811  有使用 AutoSkip 再記錄
                RecordProcess(sData);

            iAse_LoadSkipTime=1;
        }
        iAutoSkipCT++;                                                          //jou 2013-09-23 add Auto SKIP CT
        iAutoTrayEndTotal++;
    }

    if(ArmSpeed[InArm].bAutoSKIP==true)
    {
        if(fMain->labAutoSkip->Visible==false)
            fMain->labAutoSkip->Visible=true;
        fMain->labAutoSkip->Caption="Auto Skip : "+AnsiString(iAutoSkipCT);
    }
    else
    {
        if(fMain->labAutoSkip->Visible==true)
            fMain->labAutoSkip->Visible=false;
    }
}
//==============================================================================
void ShowAutoSkipError()                                                        //kevin 20170904 (wei) 記錄每一盤 AutoSkip 後有吸到IC 紀錄XY 位置 發ALARM
{
    if(CUSTOMER_CODE!=CC_ASE_KaohSiung)
        return;

    if(bAutoSkiplog &&sAutoTrayendabnormalPos!="")                              //kevin 20170904 add
    {
        bAutoSkiplog =false;
        sAutoTrayendabnormalBuffer.sprintf("Auto Tray abnormal :%s", sAutoTrayendabnormalPos);                          //kevin 20170824 回應 ase close Site
        SendDataToASE(sAutoTrayendabnormalBuffer);                              //kevin 20170824 回應 ase close Site
        sAutoTrayendabnormalBuffer="";                                          //kevin 20170901
        sAutoTrayendabnormalPos="";
    }

    if(bAutoSkipHasIC)                                                          //kevin 20170224 (wei) add AutoSkip has IC
    {
        bASkStart=false;
        bAutoSkipHasIC=false;
        ShowErrorMessage("MES0102", 0, MInArmX, false, sAskStartDetect);
        sAskStartDetect="";
        SetAutoSkipCount(0);                                                    //kevin 20170320 (wei) add Fixed for Auto Skip
    }
}
//==============================================================================
void PickErrorData(int iCol, int iRow)                                          //kevin 20170904 (wei) 記錄每一盤沒吸到IC  SKIP 沒吸到IC位置
{
    //if(bAutoSkipStartXYlog)
    {
        sAutoTrayendStartPosBuffer.sprintf("(X:%d,Y:%d)", iCol+1, iRow+1);      //kevin 20170823 auto tray end 記錄一盤ＸＹ位置
        sAutoTrayendStartPos+=sAutoTrayendStartPosBuffer;                       //kevin 20170823 auto tray end 記錄一盤ＸＹ位置
        bAutoSkipStartXYlog=false;
    }
    sAutoTrayendEndPosBuffer.sprintf("(X:%d,Y%d)", iCol+1, iRow+1);             //kevin 20170823 auto tray end 記錄一盤ＸＹ位置
    sAutoTrayendEndPos+=sAutoTrayendEndPosBuffer;                               //kevin 20170823 auto tray end 記錄一盤ＸＹ位置

    return;
}
//==============================================================================
void TrayPickupErrorData()                                                      //kevin 20171213 (Steven) 整盤沒有吸到IC位置
{
    if(sAutoTrayendEndPos!="")
    {
        sAutoTrayendEndPosBuffer.sprintf("Auto Tray Skip End:%s", sAutoTrayendEndPos);                                  //kevin 20170824 回應 ase close Site
        SendDataToASE(sAutoTrayendEndPosBuffer);                                //kevin 20170824 回應 ase close Site
        sAutoTrayendEndPos="";
        sAutoTrayendEndPosBuffer="";
    }
}
//===============================================================================
bool AutoTrayEndMusic(int iSkeyTime, int iHandMode)                             //kevin 20170904 (wei) Auto Tray end 每盤吸到空位置就發ALARM 及音樂。
{
    if(bNeedMusicFinishLighAndAlarmOn==false)                                   //kevin 20170816 每盤進來啟動音樂
    {
        bNeedMusicAndAlarmOn=true;                                              //kevin 20170816 add 開始起動
    }

    if(iSkeyTime>=iHandMode)
        return true;

    return false;
}
//==============================================================================
bool bCheckYPitchRunHomeSen(int iXYHome)                                        //kevin 20180822 (Steven) : y pitch 遮住home
{
    bool bflag1=false, bflag2=false;
    if(USE_IN_Y_IS_AUTO_PITCH==true)                                            //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(iXYHome==0)
        {
            bflag1=MOT[MInArmPitchY].MotorMove(0);
            MOT[MInArmPitchY].ScanMotorStatus();
            bflag2=MOT[MInArmPitchY].Led[iHomeLed];
        }
        else
        {
            bflag1=MOT[MOutArmPitchY].MotorMove(0);
            MOT[MOutArmPitchY].ScanMotorStatus();
            bflag2=MOT[MOutArmPitchY].Led[iHomeLed];
        }

        if(bflag1 && bflag2)
            return true;
    }
    return false;
}
//==============================================================================
bool bCheckYPitchHome(int iXYHome)                                              //kevin 20180822 (Steven) : y pitch 歸home 動作
{
    bool bflag1=false;
    if(USE_IN_Y_IS_AUTO_PITCH==true)                                            //ChungHung 20140304 add for AutoYPitch  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(iXYHome==0)
        {
            bflag1=ProcessSingleMotorHome(MInArmPitchY);
        }
        else
        {
            bflag1=ProcessSingleMotorHome(MOutArmPitchY);
        }

        if(bflag1)
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void AddInArmPickerCount(int iSuckRow, int iSuckCol)                            //JerryYang 20220331 : add 吸真空次數計數
{
    if(IniConfig.bO20InOutArmPickerLifeTimeCount==true)
    {
        TestIF_File.InArmPickerLifeCnt[iSuckRow][iSuckCol]++;
    }
}
//------------------------------------------------------------------------------
void AddLoadingCount(int iSuckRow, int iSuckCol, int iTrayRow, int iTrayCol)    //Steven 20161123 : 將Loader吸取的數量計算統一成Function
{
    int iSiteRow=0, iSiteCol=0;
    int iMotXpos=0, iMotYpos=0;                                                 //Sam 20250211 : Add Load Pos
    bSCKARTLoadCntAlarm=false;
    iSCKARTLoadingStatus=fSCKART->CheckLoadingCount();                          //Steven 20161223 (wei) : For SCK ART loading count
    iLoadPickX=iSuckRow;
    iLoadPickY=iSuckCol;

    bAutoRetryFlag=false;                                                       //JerryYang 20220901 : ASE-CL pick up error時吸取邏輯要改找吸最左上角的

    fAutoTeach->CheckLoaderCountNeedOneCycle();                                 //JimmyChiu 20211020 : Auto alignment mode

    bSuckEnd[iSuckRow][iSuckCol]=true;
    InArmSuckUse[iSuckRow][iSuckCol]=false;
    bPickLoaderDuplicateErr[iSuckRow][iSuckCol]=false;

    if((bCanRunSCKART==true ||
        IniConfig.bA65_BundleIDList) &&                                         //JerryYang 20250220 : fix AUTO IN OUT
       LastSet.iRunStartMode!=rsmContinuRetest  &&                              //Steven 20170513 (jou) : For SCK ART can do manual RT
       LastSet.iRunStartMode!=rsmCInitialRetest &&
       iSCKARTLoadingStatus>=1                  &&                              //數量到了要放下來,然後 Alarm
       TestIF_File.bSCKART_LotDeviceCheck       )
    {
        if(TestIF_File.bRENESAS_EnableFTCT==false &&                            //RogerYang 20250923 : 瑞薩FT-CT 不要Alarm 數量到了繼續把這一盤做完
            fSCKART->DoChkInputCntAlarm(true)==true)                            //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定
        {
            bSCKARTLoadCntAlarm=true;
            return;
        }
    }
    else if(IniConfig.bP57LoaderAutoCleanOutByInputCT &&                        //Sam 20250605 : Loader Count AutoCleanOut
            iSCKARTLoadingStatus>=1)
    {
        bSCKARTLoadCntAlarm=true;
        return;
    }
    AutoSkipHasIClog(InArmSuck.Suck[iSuckRow][iSuckCol].sName, iTrayCol, iTrayRow);                                     //Steven 20211228 : 換位置 //kevin 20170904 (wei) add 記錄每一盤 SKIP 沒吸到IC位置
    RestoreLoadeIC(iTrayRow);                                                   //Steven 20211228 : 換位置 //JerryYang 20200422 Auto skip次數到達後, 自動再去最後一排吸吸看
    AddInArmPickerCount(iSuckRow, iSuckCol);                                    //JerryYang 20220331 : add 吸真空次數計數
    CheckPickerLifeNeedOneCycle();                                              //AI(ht9045-config) 20260521 (RogerYang) : SCC吸嘴壽命報警OneCycle優化

    bTrayMapDataError[iSuckRow][iSuckCol]=false;                                //wei 20170302 Tray map data error
    iTrayMapDataX[iSuckRow][iSuckCol]=0;
    iTrayMapDataY[iSuckRow][iSuckCol]=0;
    if(EnableTraymapCheckFunction(0))                                           //wei 20170302 (Steven) Tray map data error
    {
        if(MyDeCodeASCII(fTrayMapping->iTrayMappingDate[iTrayRow][iTrayCol])!=1)
        {
            bTrayMapDataCheckError=true;
            bTrayMapDataError[iSuckRow][iSuckCol]=true;
            iTrayMapDataX[iSuckRow][iSuckCol]=iTrayCol;
            iTrayMapDataY[iSuckRow][iSuckCol]=iTrayRow;
            iMapErrorCount++;
            EventReport(SECS_EVENT.MapNoArmHasIC);                              //wei 20170609
        }
    }

    if(IniConfig.bI27_ManualSortMode && bRunManualSortMode==true)               //Steven 20150915 : For TSMC 手動整盤功能
        InArmSuck.CopyFromTray(iSuckRow, iSuckCol, HAS_IC, MOT[MMTrayY], iTrayRow, iTrayCol, NULL_IC, MOT[MMTrayY].Tray.iTarget[iTrayCol][iTrayRow]);
    else
        InArmSuck.CopyFromTray(iSuckRow, iSuckCol, HAS_IC, MOT[MMTrayY], iTrayRow, iTrayCol, NULL_IC);

    InspectInArmPosition(MMTrayY, iSuckRow, iSuckCol, iTrayRow, iTrayCol, false);

    if(IsNNMode()==NN_2Row)
    {
        if(InArmSuck.iWhichSht==0)
            iSiteRow=iSuckRow+2;
        else
            iSiteRow=iSuckRow;
    }
    else if(IsNNMode()==NN_1Row)
    {
        if(InArmSuck.iWhichSht==0)
            iSiteRow=iSuckRow+1;
        else
            iSiteRow=iSuckRow;
    }
    else
    {
        iSiteRow=iSuckRow;
    }

    int iPickKit=(InArmSuck.iWhichKit==1)?4:0;
    if(iCloseSiteModeFor2x8>e2x8OneByOne ||
       iCloseSiteModeFor2x6>e2x6OneByOne)
    {
        iPickKit=0;
    }

    if((IniConfig.bI37_EnableFIFOMode &&
        IniConfig.bI37_EnableFIFOSiteOrder &&
        LastSet.iRunStartMode==rsmFIFOMode) ||                                  //Steven 20170302 (wei) : FIFO MODE
       ((fContact->fShow && iContactMode==CONTACT_DEVICE_MAP_CHECK) ||          //Steven 20220510 : For QTI SD Device Map Function
         fContact->IsRun2DCheck()==true))                                       //JerryYang 20250220 : 2DID硬體順序檢查功能
    {
        //這兩個模式不能用
    }
    else                                                                        //Steven 20220527 : for JCET Auto Site Map
    {
        InArmSuck.iWhichSite[iSuckRow][iSuckCol]=Prod.iSiteMap[InArmSuck.iWhichSht][iSuckRow][iSuckCol+iPickKit];       //Steven 20220526 : 針對Auto Site Map加熱模式做紀錄   //Steven 20240508 : fixed for iWhichSite flag
    }

    if(USE_ROTATE_KIT==1 &&
       (iRotate_Type==e1MotRotate1Dut ||                                        //Steven 20250801 : for rotator
        iRotate_Type==e2MotRotate2Dut) &&
       tRotate.ActiveRotate)
    {
        InArmSuck.iNeedRotAng[iSuckRow][iSuckCol]=Prod.RotateDutDate[0][iSuckRow][iSuckCol];                            //預計旋轉的角度   //jou 20231122 : Rotate Use RT mode
    }
    else
    {
        InArmSuck.iNeedRotAng[iSuckRow][iSuckCol]=Prod.RotateDutDate[0][iSiteRow][iSiteCol];                            //預計旋轉的角度   //jou 20231122 : Rotate Use RT mode
    }
    InArmSuck.iCurrRotAng[iSuckRow][iSuckCol]=FrmRotate->iFromTrayAngle;        //目前產品的角度

    iMotXpos=MOT[MInArmX].ReadPos();                                            //Sam 20250211 : Add Load Pos
    iMotYpos=MOT[MInArmY].ReadPos();

    if(fContact->IsRun2DCheck()==true)                                          //JerryYang 20250220 : 2DID硬體順序檢查功能
    {
        InArmSuck.PordRec[iSuckRow][iSuckCol].asBuffer->Strings[eLoadX]   =AnsiString(iTrayCol);                        //JerryYang 20250220 : 2DID硬體順序檢查功能
        InArmSuck.PordRec[iSuckRow][iSuckCol].asBuffer->Strings[eLoadY]   =AnsiString(iTrayRow);
        return;
    }

    if(bCanRunSCKART==true || CosFunction.bART_SECSGEM_93K)                     //JerryYang 20220923 : add SPIL             //Steven 20161214 (wei) : For SCK ART
    {
        InArmSuck.PordRec[iSuckRow][iSuckCol].AddLoadRecord(ASE_InTrayNum, iTrayRow, iTrayCol, iMotXpos, iMotYpos, fSCKART->sLotID+" "+fSCKART->sInfo_CustLotID, fSCKART->sLotStartTime);
        LastSet.iSCKARTInputCT++;
    }
    else if(fLotInfo->tsLotID->TabVisible==true &&
            fLotInfo->edtSysLotID->Text!="")                                    //Steven 20200617 : Log加上Lot ID
    {
        InArmSuck.PordRec[iSuckRow][iSuckCol].AddLoadRecord(ASE_InTrayNum, iTrayRow, iTrayCol, iMotXpos, iMotYpos, fLotInfo->edtSysLotID->Text, ASET_StartTimeNAME);
    }
    else
    {
        InArmSuck.PordRec[iSuckRow][iSuckCol].AddLoadRecord(ASE_InTrayNum, iTrayRow, iTrayCol, iMotXpos, iMotYpos, ASET_ScheduleNAME, ASET_StartTimeNAME);      //Steven 20160114 : Add Production Log -- Load
    }
    InArmSuck.PordRec[iSuckRow][iSuckCol].AddInArmLoaderPickRecord(iSuckRow, iSuckCol);                                 //Sam 20200716 : Add record

    iRecordTrayPickPosY[iSuckRow][iSuckCol]=iTrayRow;
    iRecordTrayPickPosX[iSuckRow][iSuckCol]=iTrayCol;

    if(USE_TRAY_MAPPING==etmInstall)
        fTrayMapping->sgSuckmap->Cells[iTrayCol+1][iTrayRow+1]=1;               //wei 20170119 (Steven) Tray Mapping紀錄是否有吸取IC

    if(IniConfig.bVTESTFunction==true && IniConfig.bCheckFile==true)
    {
        if(LastSet.SendCT[0]==0)
        {
            if(fMesSystem->bFormShowJustInitial==false)                         //marvin 20200424 (Kirin) Added always record report by time.
            {
#ifdef SOFT_SIMULTE
                fMesSystem->bFormShowJustInitial=true;
                fMesSystem->DoInitailRecordReportByTime();
#else
                if(LastSet.iTester==ON_LINE)
                {
                    fMesSystem->bFormShowJustInitial=true;
                    fMesSystem->DoInitailRecordReportByTime();
                }
#endif
            }
        }
    }

    LotSummary.iLoadTotal++;
    iUPH_LoaderCount++;
    LastSet.SendCT[0]++;
    LastSet.SendCT[1]++;                                                        //jou 2010-08-13 計數jam rate,改為Tray Feed為一單位
    LastSet.iDaySendCT++;                                                       //jou 20210108 : 上海偉測要求新增每日jam rate統計
    iRecordJamRateByTime_LoaderCount++;                                         // 2015.11.11 , Joye , Add Jam Rate Record
    LastSet.iLoaderCount++;                                                     //Sam 20210107 : Summary Report fuction
    iOneTrayPickCount[0]++;                                                     //kevin 20210623  loader 目前TRAY吸取數量
    if(IniConfig.bSIGURDFunction)                                               //KaiChen 20200618 ：矽格，增加Jam統計頁面
        fObserver->StatisticalLoaderCount();

    if(bRTLoaderTrayCount)
    {
        LastSet.iLoaderTrayCount_ART++;
        bRTLoaderTrayCount=false;                                               //wei 20160112 ART R1不算空盤
    }

    if((USE_AUTO_RETEST==eartInstall &&                                         //kevin 20150615
        (bAutoReTest_ART || IniConfig.bA10_AutoReTest)) ||
        CosFunction.bUseARTSortCount)                                           //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
    {
        LastSet.SendCT_ART[0]++;
        LastSet.SendCT_ART[1]++;
    }

    if(TestIF.bContinuousLoader==true)
    {
        LastSet.SendCT[2]++;                                                    //jou 2010-11-17 Piggy-Back Functions
        if((USE_AUTO_RETEST==eartInstall &&                                     //kevin 20150615
           (bAutoReTest_ART || IniConfig.bA10_AutoReTest)) ||
           CosFunction.bUseARTSortCount)                                        //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
        {
            LastSet.SendCT_ART[2]++;
        }

        if(CUSTOMER_CODE==CC_PTI)
        {
            fLotInfo->RefreshOtherTool();
        }
    }

    if(CosFunction.bQAModeUseUnloadCnt)                                         //JerryYang 20221004 : Maxim版本QA mode
    {
    }
    else
    {
        iQAModeLoaderCT++;                                                      //jou 2012-05-03 增加QA mode使用獨立的Loader Count
    }
    LastSet.iIndexInputOutPut[3]++;                                             //jou 2015-08-21 add InArm Pick count
    fSortCT->ShowLoadingIC();

    if((USE_AUTO_RETEST==eartInstall &&                                         //kevin 20150615
        (bAutoReTest_ART || IniConfig.bA10_AutoReTest)) ||
       CosFunction.bUseARTSortCount)                                            //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
    {
        fSortCT->ShowLoadingIC_ART();
    }

    iContinueAutoSkipAutoTrayEnd=0;                                             //ChungHung 20141002 add for KYEC AutoRetest
    bLoaderHasSuck=true;

    if(CosFunction.bShowHPICCount)                                              //Steven 20221228 : 計算加熱盤IC數量
    {
        fSortCT->pnlLoad->Caption=MOT[MMTrayY].Tray.HowManyIC();
    }
}
//==============================================================================
bool DoTrayEndProcess_9045()                                                    //Steven 20190703 : 整合Tray End動作
{
    bool bHasSkip=false;                                                        //Sam 20200922 : Pick up Error Production_Log 也要記錄
    SetAutoSkipCount(0);                                                        //Steven 20150217 : 顯示Auto Skip的數量, 1=++, 0=清空計數

    if(CUSTOMER_CODE==CC_ASE_Korea)
    {
    }
    else
    {
        bLoaderHasSkip=true;                                                    //Steven 20101019
        iManualRemoveTrayCnt=2;                                                 //Sam 20190731 : 當 Load 發生 Pickup Error Skip 時，當前這盤除了要手動移除，下一盤也要手動移除。
    }
    bAutoRetryFlag=false;                                                       //JerryYang 20220901 : ASE-CL pick up error時吸取邏輯要改找吸最左上角的
    Cylinder[C_TrayY_Fixer].Off();
    Cylinder[C_LoaderEdgePush].Off();
    Cylinder[C_LoaderUpPress].Off();                                            //JerryYang 20181120 (Steven) : (Steven) : 獨立控制loader壓tray
    bHasSkip=false;                                                             //Sam 20200922 : Pick up Error Production_Log 也要記錄
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(InArmSuck.Suck[i][j].Error && bHasSkip==false)                   //Sam 20200922 : Pick up Error Production_Log 也要記錄
            {
                bHasSkip=true;                                                  //一次Skip一顆
//                if(CosFunction.bLoadPickUpErrorMES0101Record)
//                {
////                    AddLoadingCount(i, j, iYPosition+iLoadPitchStepY*i, iXPosition[j]);
//                    InArmSuck.PordRec[i][j].AddErrorRecord("MES0101");
//                }
            }
            InArmSuckUse[i][j]=false;
            bPickLoaderDuplicateErr[i][j]=false;
            InArmSuck.Suck[i][j].Error=false;                                   //jou 2012-01-11 Tray end會造成Vaccum OFF error JAM0127
        }
    }

    MOT[MMTrayY].InitNewTray(NULL_IC, false, __FUNC__);
    if(CosFunction.bShowHPICCount)                                              //Steven 20221228 : 計算加熱盤IC數量
    {
        fSortCT->pnlLoad->Caption=MOT[MMTrayY].Tray.HowManyIC();
    }
    InitTrayEndFunction();
    iPickFromLoadStageTask=1;
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        sAutoTrayendStartPosBuffer.sprintf("Tray End Pos:%s", sAutoTrayendStartPos);                                    //kevin 20171026 (wei) 回應 ase close Site
        RecordProcess(sAutoTrayendStartPosBuffer);                              //kevin 20171026 (wei) inarm pick error for tray X Y pos log
        sAutoTrayendStartPos="";
        sAutoTrayendStartPosBuffer="";
    }

    if(iInArmType!=e9045_1x2_4_Hot &&                                           //Steven 20161103 : 1x2_4_Hot強制補滿
       iInArmType!=e9045_1x4_8_Hot &&                                           //Steven 20161103 : 1x4_8_Hot強制補滿
       iInArmType!=e9045_2x2_8_Hot &&                                           //Steven 20161103 : 2x2_8_Hot強制補滿
       IniConfig.bP17InArmFullPickFromLoader==false && InArmSuck.HasIC())       //Steven 20101221 :讓每盤都是滿的
    {
        return true;
    }
    else if(IniConfig.bI37_EnableFIFOMode && LastSet.iRunStartMode==rsmFIFOMode && InArmSuck.HasIC())                   //Steven 20160303 : FIFO Mode
    {
        return true;
    }
    return false;
}
//==============================================================================
bool DoTraySkipProcess_9045()                                                   //Steven 20190703 : 整合Load Tray Skip動作
{
    bool bHasSkip;
    bool bClearLoaderTray=false;
    bool bFTCTSkipFlag=false;                                                   //RogerYang 20251002 : RogerYang 瑞薩FT-CT

    if(EnableTraymapCheckFunction(1))                                           //wei 20170302 (Steven) Tray map data error
    {
        for(int k=0; k<MAX_ARM_Row; k++)
        {
            for(int l=0; l<MAX_ARM_Col; l++)
            {
                if(InArmSuck.Suck[k][l].Error &&
                   MyDeCodeASCII(fTrayMapping->iTrayMappingDate[iYPosition+iLoadPitchStepY*k][iXPosition[l]])!=1)
                {
                    if(TestIF_File.bEnableTrayMapDoubleCheck)
                    {
                        if(MyDeCodeASCII(fTrayMapping->iTrayMappingDateCheck[iYPosition+iLoadPitchStepY*k][iXPosition[l]])!=1)
                            MOT[MMTrayY].SetTraySingleData(iXPosition[l], iYPosition+iLoadPitchStepY*k, NULL_IC);
                    }
                    else
                    {
                        MOT[MMTrayY].SetTraySingleData(iXPosition[l], iYPosition+iLoadPitchStepY*k, NULL_IC);
                    }
                }
            }
        }
        InArmSuck.ResetAll();                                                   //Steven 20160323 : 避免未開啟真空
        bTrayMapDataCheckErrorSkip=false;
        return false;
    }
    else
    {
        bLoaderHasSkip=true;                                                    //Steven 20101019
        iManualRemoveTrayCnt=2;                                                 //Sam 20190731 : 當 Load 發生 Pickup Error Skip 時，當前這盤除了要手動移除，下一盤也要手動移除。
        bHasSkip=false;

        if(TestIF_File.bRENESAS_EnableFTCT==true &&                             //RogerYang 20251112 : ART數量到，如果inarm上有IC直接搬走
            fSCKART->bFirstFullSkip==false &&                                   //但是數量超過，在下一次取料要變回原本吸料方式
            LastSet.iSCKARTInputCT>=fSCKART->iInputCount)
        {
            fSCKART->bFirstFullSkip=true;
            bFTCTSkipFlag=true;
        }

        for(int k=0; k<MAX_ARM_Row; k++)
        {
            for(int l=0; l<MAX_ARM_Col; l++)
            {
                if(InArmSuck.Suck[k][l].Error && bHasSkip==false)
                {
                    if(IniConfig.bRecordSkipPosition==false &&
                       USE_IN_Y_IS_AUTO_PITCH==false)                           //JerryYang 20251218 : IN/OUT ARM支援不同模組
                    {
                        bHasSkip=true;                                          //一次Skip一顆
                    }

                    if(IniConfig.bI37_EnableFIFOMode &&                         //Frank 20180810 : add FIFO SKIP Action
                       IniConfig.bI37_EnableFIFOSiteOrder &&                    //Ifor 20181009 (Steven) : KYEC add FIFO Function
                       LastSet.iRunStartMode==rsmFIFOMode)
                    {
                        InArmSuck.SetItemData(k, l, HAS_NULL_IC);
                    }

                    SetAutoSkipCount(1);                                        //Steven 20150217 : 顯示Auto Skip的數量, 1=++, 0=清空計數

                    if(bFTCTSkipFlag==true)                                     //RogerYang 20251112 : ART數量到，如果inarm上有IC直接搬走
                    {
                        InArmSuck.SetItemData(k, l, HAS_NULL_IC);
                    }

                    InArmSuckUse[k][l]=false;
                    bPickLoaderDuplicateErr[k][l]=false;
                    InArmSuck.Suck[k][l].Error=false;                           //jou 2012-01-11 Tray end會造成Vaccum OFF error JAM0127
                    MOT[MMTrayY].SetTraySingleData(iXPosition[l], iYPosition+iLoadPitchStepY*k, NULL_IC);

                    if(IniConfig.bRecordSkipPosition)                           //jou 2013-05-30 Record Skip position
                    {
                        MOT[MMTrayY].SetTrayBufferSingleData(iXPosition[l], iYPosition+iLoadPitchStepY*k, HAS_SKIP_IC);

                        if(ArmSpeed[InArm].bAutoSKIP==true)                     //jou 2013-09-23 add Auto SKIP CT
                        {
                            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                 //吸取到空格後再吸到IC需發ALARM
                            {
                                bAutoSkiplog=true;                              //kevin 20170904 (Steven) 記錄SKIP TRAY X Y 位置
                                bASkStart=AutoTrayEndMusic(iAutoSkipCT, 4);
                            }

                            if(iAutoSkipCT>=ArmSpeed[InArm].iAutoSkipCT)
                            {
                                bAutoSkipCntOver=true;                          //JerryYang 20210420 : 矽品蘇州要求auto skip數量小於設定值要由工程師排除
                                SetAutoSkipCount(0);                            //Steven 20150217 : 顯示Auto Skip的數量, 1=++, 0=清空計數
                                bClearLoaderTray=true;                          //Steven 20150130 : 修正Auto Skip功能
                            }
                        }
                    }
                }
            }
        }

        DoRecordSkipPosition_9045(bClearLoaderTray);                            //Steven 20190703 : 整合Load Tray Skip動作
        return true;
    }
}
//==============================================================================
bool DoRecordSkipPosition_9045(bool bClearLoaderTray)                           //Steven 20190703 : 整合Load Tray Skip動作
{
    int iAutotrayEndYStart=0;
    int iAutotrayEndYEnd=MOT[MMTrayY].Tray.YItem;                               //kevin 20170429 add search load tray
    int iTrayDir=TrayForm.Loader.Direction;
    if(IniConfig.bRecordSkipPosition && bClearLoaderTray)                       //Steven 20150130 : 修正Auto Skip功能
    {
        if(CUSTOMER_CODE==CC_ASE_KaohSiung ||                                   //kevin 20170529 (wei) add autoskip finish search load tray
           IniConfig.bE62TryPickLastRow)                                        //JerryYang 20200422 Auto skip次數到達後, 自動再去最後一排吸吸看
        {
            if(iTrayDir==0 || iTrayDir==1)
            {
                iAutotrayEndYEnd=MOT[MMTrayY].Tray.YItem-1;                     //20170429 每一 盤最後一排要再搜尋是否有IC
                if(MOT[MMTrayY].Tray.Data[0][iAutotrayEndYEnd]==NULL_IC &&
                   MOT[MMTrayY].Tray.Data[MOT[MMTrayY].Tray.XItem-1][iAutotrayEndYEnd]==NULL_IC)                        //判斷是否已搜尋最後一排 就不再搜最後一排
                {
                    iAutotrayEndYStart=0;
                    iAutotrayEndYEnd=MOT[MMTrayY].Tray.YItem;
                }
                else
                {
                    bTryPickLastRowIng=true;
                    for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
                    {
                        for(int j=0; j<MOT[MMTrayY].Tray.YItem; j++)
                        {
                            if(MOT[MMTrayY].Tray.Data[i][j]==HAS_IC)
                            {
                                if(MOT[MMTrayY].Tray.BufferData[i][j]==NULL_IC)
                                {
                                    MOT[MMTrayY].SetTrayBufferSingleData(i, j, HAS_IC);
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                iAutotrayEndYStart=1;
                if(MOT[MMTrayY].Tray.Data[0][0]==NULL_IC &&
                   MOT[MMTrayY].Tray.Data[MOT[MMTrayY].Tray.XItem-1][0]==NULL_IC)                                       //判斷是否已搜尋最後一排 就不再搜最後一排
                {
                    iAutotrayEndYStart=0;
                    iAutotrayEndYEnd=MOT[MMTrayY].Tray.YItem;
                }
                else
                {
                    bTryPickLastRowIng=true;
                    for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
                    {
                        for(int j=0; j<MOT[MMTrayY].Tray.YItem; j++)
                        {
                            if(MOT[MMTrayY].Tray.Data[i][j]==HAS_IC)
                            {
                                if(MOT[MMTrayY].Tray.BufferData[i][j]==NULL_IC)
                                {
                                    MOT[MMTrayY].SetTrayBufferSingleData(i, j, HAS_IC);
                                }
                            }
                        }
                    }
                }
            }
        }

        for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
            for(int j=iAutotrayEndYStart; j<iAutotrayEndYEnd; j++)
                MOT[MMTrayY].SetTraySingleData(i, j, NULL_IC);
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        if(bManualTrayend)                                                      //kevin 20171026 (wei) 手縱 tray end
        {
            bManualTrayend=false;
            sAutoTrayendStartPosBuffer.sprintf("Tray End Pos:%s", sAutoTrayendStartPos);                                //kevin 20171026 (wei) 回應 ase close Site
            RecordProcess(sAutoTrayendStartPosBuffer);                          //kevin 20171026 (wei) inarm pick error for tray X Y pos log
        }
        else
        {
            sAutoTrayendStartPosBuffer.sprintf("Auto Tray Skip Pos:%s", sAutoTrayendStartPos);                          //kevin 20171026 (wei) 回應 ase close Site
            RecordProcess(sAutoTrayendStartPosBuffer);                          //kevin 20171026 (wei) inarm pick error for tray X Y pos log
        }
        sAutoTrayendStartPos="";
        sAutoTrayendStartPosBuffer="";
    }

    if(CosFunction.bShowHPICCount)                                              //Steven 20221228 : 計算加熱盤IC數量
    {
        fSortCT->pnlLoad->Caption=MOT[MMTrayY].Tray.HowManyIC();
    }
    return false;
}
//==============================================================================
// 整合In Arm相關功能
//==============================================================================
bool bBottom2DID;
bool bPrecisor;
bool bInRotator;
bool bDieClean;
int iInArmAdditionalFunctionTask;
//==============================================================================
void InitDoInArmAdditionalFunction()                                            //Steven 20210609 : 整合Precisor, Bottom CCD, Die Clean, Rotator
{
    iInArmAdditionalFunctionTask=1;
}
//==============================================================================
bool CheekNeedToDoInArmAdditionalFunction()
{
    bool bResult=false;
    bool flag;
    if(InArmSuck.HasType(HAS_HOT_IC) ||
       InArmSuck.HasType(HAS_TRY_SUCK_IC))
    {
       if(InArmSuck.HasType(HAS_HOT_IC) &&
          TestIF_File.UseRotateForHT7000HPKit)                                  //Sam 20210416 : 新增特殊模式 For Rotate Function HT7000 HP Kit
        {
            CheckNeedRotateWithoutHotIC();                                      //Steven 20160809 : 改成Function, 加熱IC不跑Rotate
            bInRotator=true;
            bResult=true;
        }
        else
        {
            bInRotator=false;
        }
        PreSetInArmAdditionalFlag();
        bBottom2DID=false;
        bPrecisor=false;
        bDieClean=false;
        return bResult;
    }

    if(TestIF_File.UseRotateForHT7000HPKit==false &&
       USE_ROTATE_KIT==1 &&
       tRotate.ActiveRotate &&
       TrayForm.iRotateKIT_InputType!=0 &&
       iInRotateFinish==1)                                                      //kevin 20130524 IC已放在ROTATE 此時機台歸零後，會再去LOAD TRAY吸IC
    {
        bInRotator=true;
        bBottom2DID=false;
        bPrecisor=false;
        bDieClean=false;
        bResult=true;
    }
    else if(InArmSuck.HasIC())
    {
        if(bDoPreciser==false && TestIF_File.bEnableUsePreciser==true)          //Frank 20180410 (Steven) : InArm Preciser Station
        {
            if(InArmSuck.bAlreadyPreciser==false)
            {
                bPrecisor=true;
                bResult=true;
            }
        }

        if(USE_DIE_CLEAN==1 && ArmSpeed_File[InArm].iEnableDieClean==1)         //wei 20170418
        {
            if(InArmSuck.bAlreadyDieClean==false)
            {
                bDieClean=true;
                bResult=true;
            }
        }

        if(BOTTOM_2DID && TestIF_File.bEnableBottom2D==true)                    //Steven 20190308 : Bottom 2D
        {
            if(InArmSuck.bAlready2DID==false)
            {
                bBottom2DID=true;
                bResult=true;
            }
        }

        if(bInArmHasHotIC)                                                      //Sam 20211012 : Debug 用
        {
            if(CheckNeedRotateWithoutHotIC())
            {
                fMain->DebugOneCycleHotPlate("CheekNeedToDoInArmAdditionalFunction");
            }
        }

        flag=CheckNeedRotateWithoutHotIC();                                     //Steven 20160809 : 改成Function, 加熱IC不跑Rotate
        if(flag)
        {
            if(InArmSuck.bAlreadyRotate==false)
            {
                bInRotator=true;
                bResult=true;
            }
        }
    }

    if(CosFunction.bART_RT_NoRotate &&
       tRotate.bART_RT_NoRotate && bCanRunSCKART)                               //Sam 20240809 : ART RT No Rotate
    {
        if((LastSet.iRunStartMode==rsmInitial_ART ||
            LastSet.iRunStartMode==rsmContinuStart_ART ||
            LastSet.iRunStartMode==rsmContinuRetest_ART) &&
           fSCKART->iFTRTCount!=0)
        {
            bInRotator=false;
        }
    }

    if(bResult)
        InitDoInArmAdditionalFunction();
    return bResult;
}
//==============================================================================
bool DoInArmAdditionalFunction()                                                //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
{
    bool bResult=false;
    bool flag;

    int &Task=iInArmAdditionalFunctionTask;

    switch(Task)
    {
        case 1:
            CheekNeedToDoInArmAdditionalFunction();
            Task=100;
        case 100:
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
            else if(InArmSuck.bAlreadyRotate==false && bInRotator)
            {
                Task=40000;
            }
            else
            {
                bResult=true;
            }
            break;
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
            {
                flag=fBarCode->DoBottom2DID_8CCD_Scan();
            }
            else
            {
                flag=fBarCode->DoBottom2DIDScan();
            }

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
    }
    return bResult;
}
//==============================================================================
void PreSetInArmAdditionalFlag()                                                //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
{
    if(InArmSuck.HasType(HAS_HOT_IC) || InArmSuck.HasType(HAS_TRY_SUCK_IC))
    {
        InArmSuck.bAlreadyPreciser=true;
        InArmSuck.bAlreadyDieClean=true;
        InArmSuck.bAlready2DID=true;
        if(InArmSuck.HasType(HAS_HOT_IC) &&
           TestIF_File.UseRotateForHT7000HPKit)                                 //Sam 20210416 : 新增特殊模式 For Rotate Function HT7000 HP Kit
        {
            InArmSuck.bAlreadyRotate    =!(USE_ROTATE_KIT==1 && tRotate.ActiveRotate==true);
        }
        else
        {
            InArmSuck.bAlreadyRotate=true;
        }
    }
    else if(USE_ROTATE_KIT==1 && tRotate.ActiveRotate==true && MOT[MInRotateKit].HasRealIC())
    {
        InArmSuck.bAlreadyPreciser=true;
        InArmSuck.bAlreadyDieClean=true;
        InArmSuck.bAlready2DID=true;
        InArmSuck.bAlreadyRotate=false;
    }
    else
    {
        InArmSuck.bAlreadyRotate    =!(USE_ROTATE_KIT==1 && tRotate.ActiveRotate==true);
        InArmSuck.bAlreadyPreciser  =!(TestIF_File.bEnableUsePreciser==true);
        InArmSuck.bAlreadyDieClean  =!(USE_DIE_CLEAN==1 && ArmSpeed_File[InArm].iEnableDieClean==true);
        InArmSuck.bAlready2DID      =!(BOTTOM_2DID!=ebctUninstall && TestIF_File.bEnableBottom2D==true);
    }
}
//------------------------------------------------------------------------------
int iInArmInArmCheckShtFloatTask;
TQPF_Timer hPutOKDelay1;
void InitDoInArmCheckShtFloatTask()
{
    iInArmInArmCheckShtFloatTask=1;
}
//------------------------------------------------------------------------------
bool DoInArmCheckShuttleFloating(int iSht, bool bPlaceOtherShuttle, bool bNeedCheck)
{
    static bool bShakeFlag[2]={false, false}, bVibration[2]={false, false};

    int &Task=iInArmInArmCheckShtFloatTask;
    int ret=0, iret=0, iLtcMoveAct=0, iCheckSHPos=0;
    bool bResult=false, flag=false, bLtcAlarm=true;
    AnsiString ErrorPickup=AnsiString("");
    IN_ARM_CHECK_SHUTTLE:

    switch(Task)
    {
        case 1:
            if(MoveInArmZToPlateSafe(Task))
            {
                if(bNeedCheck==true)
                {
                    Task=1300;
                }
                else if(iSht==0 &&
                   (InSHT1InLF()==false ||                                      //Steven 20250314 : add protection to avoid picker hit shuttle
                    FLCarryKit.HasRealIC()==false))
                {
                    Task=9000;
                }
                else if(iSht==1 &&
                        (InSHT2InLF()==false ||                                 //Steven 20250314 : add protection to avoid picker hit shuttle
                         BLCarryKit.HasRealIC()==false))
                {
                    Task=9000;
                }
                else if(IniConfig.bF14_1KnockShuttleFirst)
                {
                    DoKnockShuttleFirst(iSht, true);                            //jou 2015-12-09 SCS 要求 Shuttle 每次入料前 敲擊
                    Task=1260;
                }
                else
                {
                    Task=1300;
                }
                goto IN_ARM_CHECK_SHUTTLE;
            }
            break;
        case 1260:
            if(DoKnockShuttleFirst(iSht))                                       //jou 2015-12-09 SCS 要求 Shuttle 每次入料前 敲擊
            {
                Task=1300;
            }

            if(Task!=1300)
                break;
        case 1300:                                                              //kevin 20141206 start
            if(iInArmType==e9045_1x4_8_Hot)                                     //Steven 20220522 : fixed for 8吸嘴模式
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(bPlaceOtherShuttle==false)
                        InArmSuck.SetItemData(1, j, NULL_IC);
                    else
                        InArmSuck.SetItemData(0, j, NULL_IC);
                }
            }
            else if(iInArmType==e9045_1x2_4_Hot)
            {
                if(i1x2_4UseACEGPicker==0)                                      //Steven 20230530 : 1x2_4改用Row A
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(bPlaceOtherShuttle)
                            InArmSuck.SetItemData(1, j, NULL_IC);
                        else
                            InArmSuck.SetItemData(0, j, NULL_IC);
                    }
                }
                else
                {
                    for(int j=0; j<2; j++)
                    {
                        if(bPlaceOtherShuttle)
                            InArmSuck.SetItemData(0, j*2+1, NULL_IC);
                        else
                            InArmSuck.SetItemData(0, j*2, NULL_IC);
                    }
                }
            }
            else if(iInArmType==e9045_2x2_8_Hot)
            {
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col/2; j++)
                    {
                        if(bPlaceOtherShuttle)
                            InArmSuck.SetItemData(i, j*2+1, NULL_IC);
                        else
                            InArmSuck.SetItemData(i, j*2, NULL_IC);
                    }
                }
            }
            else
            {
                InArmSuck.SetAllToNullIC();
            }
            hPutOKDelay1.SetSecAndOn(ArmSpeed[InArm].dWaitOnSH);
            Task=1301;
        case 1301:                                                              //kevin 20131011 在下黏貨誤判
            if(hPutOKDelay1.Off())
            {
                flag=CheckInArmDestroyICFail();                                 //Steven 20111223 : 檢查破壞錯誤
                if(flag==false)
                {
                    RecordProcess("VOFTask=1301");
                    return false;
                }

                if(In_Shuttle_Auto_Latch==eInSHAutoLtc)                         //KenHsieh 20250722 : InSht sensor 改為2頭，並用Latch 判別疊料以及飛料
                {
                    InitAutoChkInSHLatchTask();
                    bInSHLtcFin[iSht]=false;                                    //KenHsieh 20251106 : fix close site don't do In Sht Latch
                    Task=3000;
                    break;
                }
                else
                {
                    Task=1410;
                }
            }
            else
            {
                break;
            }
        case 1410:
            if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
               IniConfig.bF23ShuttleVibration)                                  //kevin 20210415 IN Arm Vibrate shuttle
                SW[SwShuttleVibration1+iSht].Off();

            bShakeFlag[iSht]=false;
            bVibration[iSht]=false;                                             //JerryYang 20180711 (wei) Shuttle震動後還是置偏要接著搖
            if(IniConfig.bF01ShakeShuttleWhenJam==false &&
               IniConfig.bF23ShuttleVibration==false)                           //JerryYang 20171205 (Steven) shuttle震動馬達功能
                bShakeFlag[iSht]=true;

            if(CosFunction.bShakeShuttleEveryTime &&                            //Steven 20200616 : ATK要求每次放料都要搖搖蝦頭
               (TestIF_File.iShakeShuttleWhenPlaceIC==1 ||
                TestIF_File.iShakeShuttleWhenPlaceIC==3))                       //Steven 20220427 : 每次放料都要抖抖馬達
            {
                if(TestIF_File.iShakeShuttleWhenPlaceIC==3)
                    bVibration[iSht]=true;
                DoShakeShuttle(iSht, true);
                Task=1420;
                break;
            }
            else if(CosFunction.bShakeShuttleEveryTime &&
                    TestIF_File.iShakeShuttleWhenPlaceIC==2)                    //Steven 20220427 : 每次放料都要抖抖馬達
            {
                DoVibrateShuttle(iSht, true);
                Task=1440;
                break;
            }
            else
            {
                for(int i=0; i<9; i++)
                    iInShuttleJam[iSht][i]=0;                                   //kevin 20220819 shuttle Jam  紀錄位置後續，要做放到垃圾桶動作
                Task=1411;
            }
        case 1411:
            if(TestIF.iTestMode==SingleSite || TestIF.iTestMode==DualSite2x1)
                ret=CheckShuttleSensor_9045_1x1(iSht, bShakeFlag[iSht]);
            else if(TestIF.iTestMode==DualSite || TestIF.iTestMode==QualSite2X2N)
                ret=CheckShuttleSensor_9045_1x2(iSht, bShakeFlag[iSht]);
            else if(TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==_8Site2X4N)                                      //Wei 20231211 : 2X4NN Mode
                ret=CheckShuttleSensor_9045_1x4(iSht, bShakeFlag[iSht]);
            else if(TestIF.iTestMode==QualSite2X2)
                ret=CheckShuttleSensor_9045_2x2(iSht, bShakeFlag[iSht]);
            else if(TestIF.iTestMode==TriSite1X3 || TestIF.iTestMode==_6Site2X3 || TestIF.iTestMode==_6Site2X3N)
                ret=CheckShuttleSensor_9045_2x3(iSht, bShakeFlag[iSht]);
            else if(TestIF.iTestMode==_10Site2X5)
                ret=CheckShuttleSensor_9045_2x5(iSht, bShakeFlag[iSht]);
            else if(TestIF.iTestMode==_12Site2X6)
                ret=CheckShuttleSensor_9045_2x6(iSht, bShakeFlag[iSht]);
            else if(TestIF.iTestMode==_16Site2X8 || TestIF.iTestMode==_32Site4X8N)
                ret=CheckShuttleSensor_9045_2x8(iSht, bShakeFlag[iSht]);
            else
                ret=CheckShuttleSensor_9045_2x4(iSht, bShakeFlag[iSht]);

            if(ret==1)                                                                                                  //Steven 20160411 : 不然不會搖
            {
                bShakeFlag[iSht]=false;
                bVibration[iSht]=false;                                                                                 //JerryYang 20180711 (wei) Shuttle震動後還是置偏要接著搖
                iShuttleLog=0;                                                                                          //JerryYang 20181121 (Steven) : add log,記錄in shuttle device是被震動馬達or搖搖功能導正
            }
            else if(ret==3)
            {
                break;
            }
            else if(ret==2 ||
                    (IniConfig.bF29AlwaysVibrateOnShuttle &&
                     bShakeFlag[iSht]==false &&
                     bVibration[iSht]==false &&
                     IsFLCarrKitAllHasIC()))                                                                            //Sam 20210602 : 每次都要強制震動
            {
                if(IniConfig.bF23ShuttleVibration &&
                   ((IniConfig.bF01ShakeShuttleWhenJam==true && bVibration[iSht]==false) ||
                     IniConfig.bF01ShakeShuttleWhenJam==false))                                                         //JerryYang 20180711 (wei) Shuttle震動後還是置偏要接著搖
                {
                    if(IniConfig.bF01ShakeShuttleWhenJam==true)
                    {
                        bShakeFlag[iSht]=false;
                    }
                    else
                    {
                        bShakeFlag[iSht]=true;
                    }
                    bVibration[iSht]=true;
                    Task=1440;
                    DoVibrateShuttle(iSht, true);                                                                       //JerryYang 20190123 shuttle震動馬達
                }
                else if(bShuttleKnock==false)                                                                           //Jou 2013-03-08 修改敲敲功能
                {
                    if(IniConfig.bF01ShakeShuttleWhenJam==false &&
                       IniConfig.bF23ShuttleVibration==false)                                                           //kevin 20190731 不使用搖搖
                    {
                        bShakeFlag[iSht]=true;
                        break;
                    }
                    Task=1420;
                    DoShakeShuttle(iSht, true);                                                                         //Steven 20120801 : 修改抖抖功能 (true為初始化)
                }
                else
                {
                    bShakeFlag[iSht]=true;
                    DoKnockShuttle(iSht, true);                                                                         //jou 2013-07-17 Knock Shuttle(true為初始化)
                    Task=1430;
                }
                break;
            }
            else
            {
                Task=9000;                                                                                              //JimmyChiu 20220908 add Pickup Error Placement
            }
            break;
        case 1420:
            if(iSht==0 && InSHT1InLF()==false)                                  //Steven 20250314 : add protection to avoid picker hit shuttle
            {
                break;
            }
            else if(iSht==1 && InSHT2InLF()==false)
            {
                break;
            }
            else
            {
                Task=1401;
            }
        case 1401:
            if(DoShakeShuttle(iSht))                                            //Steven 20120801 : 修改抖抖功能
            {
                if(CosFunction.bShakeShuttleEveryTime &&                        //Steven 20220427 : 每次放料都要抖抖馬達
                   TestIF_File.iShakeShuttleWhenPlaceIC==3 &&
                   bVibration[iSht]==true)
                {
                    DoVibrateShuttle(iSht, true);
                    Task=1430;
                }
                else if(fMain->cInplace->InArmPlacementEnable())                //JimmyChiu 20220908 add Pickup Error Placement
                {
                    Task=2000;
                }
                else
                {
                    Task=1411;
                }
            }
            break;
        case 1430:
            if(DoKnockShuttle(iSht))                                            //Jou 2013-03-08 修改敲敲功能
            {
                Task=1411;
            }
            break;
        case 1440:
            if(DoVibrateShuttle(iSht))                                          //JerryYang 20190123 shuttle震動馬達
            {
                Task=1411;
            }
            break;
        case 2000:
            if(fMain->cInplace->InArmPlacementEnable())                         //JimmyChiu 20220908 add Pickup Error Placement
            {
                fMain->cInplace->DoSuckToRecycleFromShuttle(true,ErrorPickup);
            }
            Task=2100;
        case 2100:
            if(fMain->cInplace->InArmPlacementEnable())
            {
                if(fMain->cInplace->DoSuckToRecycleFromShuttle(false,ErrorPickup))
                {
                    fMain->cInplace->bRunNo9=false;                             //KenHsieh 20240131 : add No9作動畫面
                    bInSHLtcFin[iSht]=true;                                     //KenHsieh 20251106 : fix close site don't do In Sht Latch
                    Task=9000;
                }
                break;
            }
            else
            {
                fMain->cInplace->bRunNo9=false;                                 //KenHsieh 20240131 : add No9作動畫面
            }

            if(In_Shuttle_Auto_Latch==eInSHAutoLtc)                             //back to check shuttle sensor //KenHsieh 20251105 : Auto In shuttle latch combine No9 func.
            {
                InitAutoChkInSHLatchTask();
                Task=3000;
            }
            else
            {
                Task=1411;
            }
            break;
        //JimmyChiu 20220908 add Pickup Error Placement
        //<==
        case 3000:                                                              //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
            if(((iSht==0 && FLCarryKit.UseSiteFullIC()) ||
               (iSht==1 && BLCarryKit.UseSiteFullIC())) ||                      //Nomal 放滿data
               ((iOneCycle==1 || iCleanOut==1) &&                               //OneCycle or CleanOut
               ((iSht==0 &&
                 FLCarryKit.HasIC() &&
                 FLCarryKit.UseSiteFullIC()==false) ||
                (iSht==1 &&
                 BLCarryKit.HasIC() &&
                 BLCarryKit.UseSiteFullIC()==false)) &&                         //OneCycle or CleanOut下有IC
                ((MOT[MMTrayY].fHasTray==false &&
                 MOT[MMPlate1].HasIC()==false &&
                 MOT[MMPlate2].HasIC()==false &&                                //OneCycle or CleanOut下 Loader & HP 無料
                (iCleanOut!=1 ||
                (iCleanOut==1 && MOT[MMTrayY_Car].fHasTray==false))) ||         //Cleanout 下要判斷Car
                (iOneCycle==1 && IniConfig.bP17InArmFullPickFromLoader==false))))   //Onecycle + Close P17  //KenHsieh 20251014 : fix onecycle or cleanout 時，跑Latch問題
            {
                if(((BAR_CODE_INSTALL==ebctInShtIntel ||
                     BAR_CODE_INSTALL==ebctUseCCDMode ||
                     BAR_CODE_INSTALL==ebctEtherNetCCD ||
                     BAR_CODE_INSTALL==ebcUseOCR) &&
                     TestIF_File.bEnableBarCode   &&
                    ((iSht==0 && FLCarryKit.HasIC())  ||
                     (iSht==1 && BLCarryKit.HasIC())) &&                        //Ifor 20190129 : add Cognex EtherNet 通訊   //Ifor 20210407 add: 自製OCR
                    (BOTTOM_2DID==0 || TestIF_File.bEnableBottom2D==false)) ||  //KenHsieh 20251008 : Fix Auto InSht Latch for 2D hangup, need to move left
                     DeviceForm.bShuttleWaitingOutSiteChamber)                  //KenHsieh 20260320 : 此功能開啟，Shuttle 必須回到左邊，避免Hanhgup
                {
                    iLtcMoveAct=1;
                }

                if(fMain->cInplace->InArmPlacementEnable())                     //KenHsieh 20251105 : Auto In shuttle latch combine No9 func.
                {
                    bLtcAlarm=false;
                }

                iret=CheckInShuttleSensor_Latch(iSht, iLtcMoveAct, bLtcAlarm);  //KenHsieh 20251105 : Auto In shuttle latch combine No9 func.
                if(iret==1)
                {
                    bInSHLtcFin[iSht]=true;                                     //KenHsieh 20251106 : fix close site don't do In Sht Latch

                    if(iSht==0)                                                 //KenHsieh 20251118 : 修改Out Sht 會重新GetLtc 導致Lose IC 異常
                    {
                        iCheckSHPos=MOT[MInShuttle1].ReadPos();
                        if(iLtcMoveAct==1)                                      //KenHsieh 20251121 : 修正跑barcode 未清除ltc 導致誤判out lose IC
                        {
                            fLtcSensor->ClearLtcSensor(0);
                            fLtcSensor->SetLtcSensor(0);
                        }
                    }
                    else
                    {
                        iCheckSHPos=MOT[MInShuttle2].ReadPos();
                        if(iLtcMoveAct==1)                                      //KenHsieh 20251121 : 修正跑barcode 未清除ltc 導致誤判out lose IC
                        {
                            fLtcSensor->ClearLtcSensor(1);
                            fLtcSensor->SetLtcSensor(1);
                        }
                    }

                    if(iCheckSHPos!=Prod.InSHT[iSht].iLeft)
                        bNeedGetSHRightLtc[iSht]=false;
                    else
                        bNeedGetSHRightLtc[iSht]=true;
                    Task=9000;
                }
                else if(iret==2)
                {
                    InitAutoChkInSHLatchTask();                                 //有異常皆會跑到左邊，固可Init後重跑一次
                    Task=3000;
                }
            }
            else
            {
                Task=9000;
            }
            break;
        case 9000:                                                              //JimmyChiu 20220908 add Pickup Error Placement
            RecordShtSuperflous(iSht);                                          //JerryYang 20181121 (Steven) : add log,記錄in shuttle device是被震動馬達or搖搖功能導正
            IndexZCanMove[0]=true;
            IndexZCanMove[1]=true;
            bShuttleShake=false;
            bShuttleKnock=false;                                                //Jou 2013-03-08 修改敲敲功能
            bVibration[iSht]=false;                                             //JerryYang 20180711 (wei) Shuttle震動後還是置偏要接著搖
            fMain->cInplace->bNo9Action=false;                                  //KenHsieh 20251105 : Auto In shuttle latch combine No9 func.
            bResult=true;
            Task=1;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
extern bool IsTestZ1NotSafeShuttle1CanNotMove(int &iRetryCT);                   //KenHsieh 20251112 : 修改Index 判斷方式，避免Hangup與無謂的等待
extern bool IsTestZ2NotSafeShuttle2CanNotMove(int &iRetryCT);
int iAutoChkInSHLatchTask;
int iLtcErrRetryCt[2];                                                          //KenHsieh 20260402 : add Auto retry 1 times
void InitAutoChkInSHLatchTask()
{
    iAutoChkInSHLatchTask=1;
    iLtcErrRetryCt[0]=0;                                                        //KenHsieh 20260402 : add Auto retry 1 times
    iLtcErrRetryCt[1]=0;
}

//==============================================================================
bool InitialInArmNeedSuck(int iShuttle, bool bPlace)                            //Steven 20220927 : 重新整理In / Out Arm Need Suck
{
    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(bPlace==true)
            {
                InArmSuck.Suck[i][j].SetNeedDestroy(false);
            }
            else
            {
                InArmSuck.Suck[i][j].SetNeedSuck(false);
                InArmSuck.iNeedSuck[i][j]=false;
            }
        }
    }

    if(bPlace==false)
    {
        if(iShuttle==0)
            ptrInSHTBackup.CopyFrom(FLCarryKit);
        else
            ptrInSHTBackup.CopyFrom(BLCarryKit);
    }

    if(iShuttle==0)
    {
        if(InSHT1InLF()==false)
            return false;
    }
    else if(iShuttle==1)
    {
        if(InSHT2InLF()==false)
            return false;
    }
    return true;
}
//------------------------------------------------------------------------------
int CheckInShuttleSensor_Latch(int iShuttle, int iMoveAct, bool bAlarm)         //0:NotFin 1:Finish 2:Error  //KenHsieh 20251104 : add 檢測後動作 & alarm 變數  //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
{
    if(MOTION_CARD_TYPE!=MotionCard_Contec ||
       LastSet.iRealDummy!=REALLY)
        return 1;

    int &Task=iAutoChkInSHLatchTask;
    int iResult=0;
    bool bInSHLtcZFlag[2]={false, false};
    bool bflag=false, bCheckData=true, bDuplicateErr=false;
    AnsiString asStr1="";
    static int iSHAutoLtcErr=0, iRetryCT=0;
    static AnsiString ErrPart_InSHLtc="";
    static bool bfCanMoveLChange[2]={false, false}, bDupErrFlag[2][10]={{false}};

    switch(Task)
    {
        case 1:
            if(iShuttle==0)
            {
                if(IsTestZ1NotSafeShuttle1CanNotMove(iRetryCT))                 //KenHsieh 20251112 : 修改Index 判斷方式，避免Hangup與無謂的等待
                    break;
                iRetryCT=0;
                fLtcSensor->ClearLtcSensor(0);                                  //KenHsieh 20260402 : SH1增加Clear，避免檢測失敗
                fLtcSensor->SetLtcSensor(0);
                bInSh1DoLtc=true;
                Task=1000;
            }

            if(iShuttle==1)
            {
                if(IsTestZ2NotSafeShuttle2CanNotMove(iRetryCT))                 //KenHsieh 20251112 : 修改Index 判斷方式，避免Hangup與無謂的等待
                    break;
                iRetryCT=0;
                fLtcSensor->SetLtcSensor(1);
                bInSh2DoLtc=true;
                Task=2000;
            }
            bfCanMoveLChange[iShuttle]=false;
            break;
        case 1000:
            bInSHLtcZFlag[0]=MOT[MInSh1LtcSenZ1].MotorMove(Prod.iInSH1SenICDetectZ1+Prod.iInSH1SenICAddPos);
            bInSHLtcZFlag[1]=MOT[MInSh1LtcSenZ2].MotorMove(Prod.iInSH1SenICDetectZ2+Prod.iInSH1SenICAddPos);

            if(bInSHLtcZFlag[0]==true && bInSHLtcZFlag[1]==true)
                Task=1100;
            break;
        case 1100:
            if(IsTestZ1NotSafeShuttle1CanNotMove(iRetryCT)==false)              //KenHsieh 20251112 : 修改Index 判斷方式，避免Hangup與無謂的等待
            {
                iRetryCT=0;
                if(MOT[MInShuttle1].fCanMoveL==false)
                {
                    SetShuttlefCanMoveL(iShuttle, true, __FUNC__, "1100");
                    bfCanMoveLChange[iShuttle]=true;
                }
                Task=1200;
            }
            break;
        case 1200:
            if(IsTestZ1NotSafeShuttle1CanNotMove(iRetryCT))                     //KenHsieh 20251112 : 修改Index 判斷方式，避免Hangup與無謂的等待
                break;

            iRetryCT=0;
            bflag=MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iRight-200);         //KenHsieh 20260120 : Add Shuttle move 保護

            if(bflag)
            {
                if(bfCanMoveLChange[iShuttle]==true)
                {
                    SetShuttlefCanMoveL(iShuttle, false, __FUNC__, "1200");
                    bfCanMoveLChange[iShuttle]=false;
                }
                Task=1250;
            }
            break;
        case 1250:
            iSHAutoLtcErr=0;
            fLtcSensor->GetLtcSensor(0);
            ErrPart_InSHLtc="";
            #ifndef SOFT_SIMULTE
            if(fMain->cInplace->InArmPlacementEnable() &&
               fMain->cInplace->bNo9Action)                                     //KenHsieh 20251105 : Auto In shuttle latch combine No9 func.
            {
                bCheckData=false;
            }

            if(LastSet.iRealDummy!=REALLY)                                      //KenHsieh 20260506 : 模式有IC才需要判斷
                iSHAutoLtcErr=0;
            else
                iSHAutoLtcErr=DoCheckShuttle1ICByLTC_AutoLatch(ErrPart_InSHLtc, bCheckData);
            if(iSHAutoLtcErr!=0)
            {
                if(MOT[MInShuttle1].fCanMoveL==false)
                {
                    SetShuttlefCanMoveL(iShuttle, true, __FUNC__, "1250");
                    bfCanMoveLChange[iShuttle]=true;
                }
                Task=1260;
            }
            else
            #endif
            {
                iLtcErrRetryCt[0]=0;                                            //KenHsieh 20260402 : add Auto retry 1 times
                if(iMoveAct==1)                                                 //KenHsieh 20251104 : add 檢測後動作 & alarm 變數
                {
                    if(MOT[MInShuttle1].fCanMoveL==false)
                    {
                        SetShuttlefCanMoveL(iShuttle, true, __FUNC__, "1250_iMoveAct=1");
                        bfCanMoveLChange[iShuttle]=true;
                    }
                    Task=1260;
                }
                else
                {
                    Task=9900;
                }
            }
            break;
        case 1260:
            bflag=MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft);       //KenHsieh 20260120 : Add Shuttle move 保護
            if(bflag)
            {
                if(bfCanMoveLChange[iShuttle]==true)
                {
                    SetShuttlefCanMoveL(iShuttle, false, __FUNC__, "1260");
                    bfCanMoveLChange[iShuttle]=false;
                }

                if(iSHAutoLtcErr!=0)                                            //KenHsieh 20251008 : Fix Auto InSht Latch for 2D hangup, need to move left
                {
                    if(bAlarm)                                                  //KenHsieh 20251104 : add 檢測後動作 & alarm 變數
                    {
                        Task=1265;
                    }
                    else
                    {
                        InitDoInShZHome();
                        Task=1270;
                    }
                }
                else
                {
                    Task=9900;
                }
            }
            break;
        case 1265:
            for(int i=0; i<FLCarryKit.iShtCol; i++)                             //KenHsieh 20251128 : InShtLtc 新增異常重複判斷  //KenHsieh 20260518 : SThreadPara.iXItem → FLCarryKit.iShtCol
            {
                if(bInSHLtcErrNo[iShuttle][i] &&
                   bDupErrFlag[iShuttle][i])
                    bDuplicateErr=true;

                if(bInSHLtcErrNo[iShuttle][i])
                    bDupErrFlag[iShuttle][i]=true;
                else
                    bDupErrFlag[iShuttle][i]=false;
            }

            iLtcErrRetryCt[0]++;                                                //KenHsieh 20260402 : add Auto retry 1 times
            if(iLtcErrRetryCt[0]>1)
            {
                if(iSHAutoLtcErr==1)
                    ShowErrorMessage("JAM0401", K_RETRY, MInShuttle1, bDuplicateErr, ErrPart_InSHLtc);
                else
                    ShowErrorMessage("JAM0403", K_RETRY, MInShuttle1, bDuplicateErr, ErrPart_InSHLtc);
            }
            else
            {
                asStr1.sprintf("Auto retry for In shuttle1 latch error : %s", ErrPart_InSHLtc);
                RecordProcess(asStr1);
            }

            InitDoInShZHome();
            Task=1270;
            break;
        case 1270:
            if(DoInShZHome(iShuttle))
            {
                if(bAlarm)                                                      //KenHsieh 20251104 : add 檢測後動作 & alarm 變數
                {
                    iResult=0;
                    Task=1;
                }
                else
                {
                    fLtcSensor->SetLtcSensor(0);                                //JerryYang 20230406 : 重置shuttle1的latch
                    fLtcSensor->ClearLtcSensor(0);                              //Sam 20221101 : Latch 清除都要確認是否清清乾淨
                    Task=9901;
                }
            }
            break;
        case 2000:
            bInSHLtcZFlag[0]=MOT[MInSh2LtcSenZ1].MotorMove(Prod.iInSH2SenICDetectZ1+Prod.iInSH2SenICAddPos);
            bInSHLtcZFlag[1]=MOT[MInSh2LtcSenZ2].MotorMove(Prod.iInSH2SenICDetectZ2+Prod.iInSH2SenICAddPos);

            if(bInSHLtcZFlag[0]==true && bInSHLtcZFlag[1]==true)
                Task=2100;
            break;
        case 2100:
            if(IsTestZ2NotSafeShuttle2CanNotMove(iRetryCT)==false)              //KenHsieh 20251112 : 修改Index 判斷方式，避免Hangup與無謂的等待
            {
                iRetryCT=0;
                if(MOT[MInShuttle2].fCanMoveL==false)
                {
                    SetShuttlefCanMoveL(iShuttle, true, __FUNC__, "2100");
                    bfCanMoveLChange[iShuttle]=true;
                }
                Task=2200;
            }
            break;
        case 2200:
            if(IsTestZ2NotSafeShuttle2CanNotMove(iRetryCT))                     //KenHsieh 20251112 : 修改Index 判斷方式，避免Hangup與無謂的等待
                break;

            iRetryCT=0;
            bflag=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight-200);  //KenHsieh 20260120 : Add Shuttle move 保護
            if(bflag)
            {
                if(bfCanMoveLChange[iShuttle]==true)
                {
                    SetShuttlefCanMoveL(iShuttle, false, __FUNC__, "2200");
                    bfCanMoveLChange[iShuttle]=false;
                }
                Task=2250;
            }
            break;
        case 2250:
            iSHAutoLtcErr=0;
            fLtcSensor->GetLtcSensor(1);
            ErrPart_InSHLtc="";
            #ifndef SOFT_SIMULTE
            if(fMain->cInplace->InArmPlacementEnable() &&
               fMain->cInplace->bNo9Action)                                     //KenHsieh 20251105 : Auto In shuttle latch combine No9 func.
            {
                bCheckData=false;
            }

            if(LastSet.iRealDummy!=REALLY)                                      //KenHsieh 20260506 : 模式有IC才需要判斷
                iSHAutoLtcErr=0;
            else
                iSHAutoLtcErr=DoCheckShuttle2ICByLTC_AutoLatch(ErrPart_InSHLtc, bCheckData);
            if(iSHAutoLtcErr!=0)
            {
                if(MOT[MInShuttle2].fCanMoveL==false)
                {
                    SetShuttlefCanMoveL(iShuttle, true, __FUNC__, "2250");
                    bfCanMoveLChange[iShuttle]=true;
                }
                Task=2260;
            }
            else
            #endif
            {
                iLtcErrRetryCt[1]=0;                                            //KenHsieh 20260402 : add Auto retry 1 times
                if(iMoveAct==1)
                {
                    if(MOT[MInShuttle2].fCanMoveL==false)
                    {
                        SetShuttlefCanMoveL(iShuttle, true, __FUNC__, "2250_iMoveAct=1");
                        bfCanMoveLChange[iShuttle]=true;
                    }
                    Task=2260;
                }
                else
                {
                    Task=9900;
                }
            }
            break;
        case 2260:
            bflag=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft);       //KenHsieh 20260120 : Add Shuttle move 保護
            if(bflag)
            {
                if(bfCanMoveLChange[iShuttle]==true)
                {
                    SetShuttlefCanMoveL(iShuttle, false, __FUNC__, "2260");
                    bfCanMoveLChange[iShuttle]=false;
                }

                if(iSHAutoLtcErr!=0)                                            //KenHsieh 20251008 : Fix Auto InSht Latch for 2D hangup, need to move left
                {
                    if(bAlarm)                                                  //KenHsieh 20251104 : add 檢測後動作 & alarm 變數
                    {
                        Task=2265;
                    }
                    else
                    {
                        InitDoInShZHome();
                        Task=2270;
                    }
                }
                else
                {
                    Task=9900;
                }
            }
            break;
        case 2265:
            for(int i=0; i<BLCarryKit.iShtCol; i++)                             //KenHsieh 20251128 : InShtLtc 新增異常重複判斷  //KenHsieh 20260518 : SThreadPara.iXItem → BLCarryKit.iShtCol
            {
                if(bInSHLtcErrNo[iShuttle][i] &&
                   bDupErrFlag[iShuttle][i])
                    bDuplicateErr=true;

                if(bInSHLtcErrNo[iShuttle][i])
                    bDupErrFlag[iShuttle][i]=true;
                else
                    bDupErrFlag[iShuttle][i]=false;
            }

            iLtcErrRetryCt[1]++;                                                //KenHsieh 20260402 : add Auto retry 1 times
            if(iLtcErrRetryCt[1]>1)
            {
                if(iSHAutoLtcErr==1)
                    ShowErrorMessage("JAM0404", K_RETRY, MInShuttle2, bDuplicateErr, ErrPart_InSHLtc);
                else
                    ShowErrorMessage("JAM0406", K_RETRY, MInShuttle2, bDuplicateErr, ErrPart_InSHLtc);
            }
            else
            {
                asStr1.sprintf("Auto retry for In shuttle2 latch error : %s", ErrPart_InSHLtc);
                RecordProcess(asStr1);
            }

            InitDoInShZHome();
            Task=2270;
            break;
        case 2270:
            if(DoInShZHome(iShuttle))
            {
                if(bAlarm)                                                      //KenHsieh 20251104 : add 檢測後動作 & alarm 變數
                {
                    iResult=0;
                    Task=1;
                }
                else
                {
                    fLtcSensor->ClearLtcSensor(1);                              //Sam 20221101 : Latch 清除都要確認是否清清乾淨
                    fLtcSensor->SetLtcSensor(1);                                //JerryYang 20230406 : 重置shuttle1的latch
                    Task=9901;
                }
            }
            break;
        case 9900:
            bInSh1DoLtc=false;
            bInSh2DoLtc=false;
            bfCanMoveLChange[iShuttle]=false;
            for(int i=0; i<(iShuttle==0 ? FLCarryKit.iShtCol : BLCarryKit.iShtCol); i++)     //KenHsieh 20251128 : InShtLtc 新增異常重複判斷  //KenHsieh 20260518 : SThreadPara.iXItem → kit-aware iShtCol
                bDupErrFlag[iShuttle][i]=false;

            iResult=1;
            break;
        case 9901:
            bInSh1DoLtc=false;
            bInSh2DoLtc=false;
            bfCanMoveLChange[iShuttle]=false;
            iResult=2;
            break;
    }

    return iResult;
}
//------------------------------------------------------------------------
void DoInArmLoadPickUP_9045(AnsiString ErrPart,AnsiString ErrTray)
{
    AnsiString sbuffer="";
    sbuffer.sprintf("LoadTrayPickupError%s:%s", ErrPart, ErrTray);
    MyDBIProcess("Message", sbuffer);                                           //kevin 20220521 add Tray X Y  Pos
}
//------------------------------------------------------------------------------
void sLoadPickupClean()                                                         //kevin 20220723 initial data
{
    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            sLoadPickupErrorTrayPos[i][j]="";                                   //kevin 20220521 add Tray X Y  Pos
        }
    }
}
//==============================================================================
bool InArmLeftSideNoIC(int iRow)
{
    if(iRow==2)
    {
        if(InArmSuck.Item[0][0]==NULL_IC && InArmSuck.Item[0][1]==NULL_IC &&
           InArmSuck.Item[1][0]==NULL_IC && InArmSuck.Item[1][1]==NULL_IC)
            return true;
        else
            return false;
    }
    else
    {
        if(InArmSuck.Item[0][0]==NULL_IC && InArmSuck.Item[0][1]==NULL_IC)
            return true;
        else
            return false;
    }
}
//==============================================================================
bool InArmLeftSideHasIC(int iRow)
{
    if(iRow==2)
    {
        if(InArmSuck.Item[0][0] && InArmSuck.Item[0][1] &&
           InArmSuck.Item[1][0] && InArmSuck.Item[1][1])
            return true;
        else
            return false;
    }
    else
    {
        if(InArmSuck.Item[0][0] && InArmSuck.Item[0][1])
            return true;
        else
            return false;
    }
}
//==============================================================================
bool InArmSideAllClose(int iSht)
{
    if(iSht==0 && TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)   //jou 2024-09-24 : 修正single site 關arm hang up
        return true;
    else if(iSht==1 && TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0)
        return true;

    for(int i=0; i<InArmSuck.iShtRow; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(Prod.fInArmSuck4x8[iSht][i][j]==true)
            {
                return false;
            }
        }
    }
    return true;
}
//==============================================================================
bool InArmSideAllCloseWithKit(int iSht, int iKit)
{
    if(iSht==0 && TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)   //jou 2024-09-24 : 修正single site 關arm hang up
        return true;
    else if(iSht==1 && TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0)
        return true;

    int iKit32=(iKit==0)?0:4;

    for(int i=0; i<InArmSuck.iShtRow; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(Prod.fInArmSuck4x8[iSht][i][j+iKit32]==true)
            {
                return false;
            }
        }
    }
    return true;
}
//==============================================================================
void SetShuttleToNullICWhenCleanOut()                                           //Steven 20240326 : 修正Clean時, 都沒IC造成的Hang Up
{
    if(FLCarryKit.HasRealIC()==false && FTestSuck.HasRealIC()==false &&
       BLCarryKit.HasRealIC()==false && BTestSuck.HasRealIC()==false &&
       InArmSuck.HasRealIC()==false)
    {
        FLCarryKit.SetAllToNullIC();
        FTestSuck.SetAllToNullIC();
        BLCarryKit.SetAllToNullIC();
        BTestSuck.SetAllToNullIC();
        if(TestSocket.HasRealIC()==false)
            TestSocket.SetAllToNullIC();                                        //Steven 20250206 : fixed for hang up
    }
}
//==============================================================================
void SetShuttleToHasNullICWhenCleanOut()                                        //Steven 20230309 : 修正Clean時, 都沒IC造成的Hang Up
{
    if(IsInArmCleanOutFinish(iOneCycle))                                        //入料區都沒料了
    {
        if(FLCarryKit.HasRealIC()==true && InSHT1InLF())                        //Shuttle 1或Arm 1有IC
        {
            MOT[MInShuttle1].fCanMoveL=true;
            FLCarryKit.SetNullIcToHasNullIc();
        }

        if(BLCarryKit.HasRealIC()==true && InSHT2InLF())
        {
            MOT[MInShuttle2].fCanMoveL=true;
            BLCarryKit.SetNullIcToHasNullIc();
        }

        if(bUseTwoArm32Site==true)
        {
            if((BLCarryKit.UseSiteHasIC()==true ||                              //Steven 20230320 : fixed for NN mode hang up
                (BTestSuck.HasRealIC() && BTestSuck.AlreadyTest()==false)) &&   //有未測的等於在等Shuttle 2
               InSHT1InLF() &&
               (FTestSuck.UseSiteNoIC() ||
                FTestSuck.AlreadyTest()==true))                                 //有已測表示還沒吸取未測的
            {
                MOT[MInShuttle1].fCanMoveL=true;
                FLCarryKit.SetNullIcToHasNullIc();
            }

            if((FLCarryKit.UseSiteHasIC()==true ||
                (FTestSuck.HasRealIC() && FTestSuck.AlreadyTest()==false)) &&   //有未測的等於在等Shuttle 2
               InSHT2InLF() &&
               (BTestSuck.UseSiteNoIC() ||
                BTestSuck.AlreadyTest()==true))                                 //有已測表示還沒吸取未測的
            {
                MOT[MInShuttle2].fCanMoveL=true;
                BLCarryKit.SetNullIcToHasNullIc();
            }
        }
    }
}
//==============================================================================
bool InArmNeedCheckOffset(bool bPlace, int iPos)                                //Steven 20230531 : 簡化判斷式
{
    bool bResult=false;
    if(bPlace)
    {
        if(iPos==0)
        {
            if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&      //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                (Sen[SnRKManualTStart].IsOn() ||
                fOffSet->UseAutoOffsetFunction("Input Shuttle1") ||             //jou 2013-08-29 Use Auto Offset Funtion
                fOffSet->UseInArmSetupTeach(InOfsInSh1)))                       //JerryYang 20180921 Setup Teach功能
                bResult=true;
        }
        else
        {
            if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&      //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                (Sen[SnRKManualTStart].IsOn() ||
                fOffSet->UseAutoOffsetFunction("Input Shuttle2") ||             //jou 2013-08-29 Use Auto Offset Funtion
                fOffSet->UseInArmSetupTeach(InOfsInSh2)))                       //JerryYang 20180921 Setup Teach功能
                bResult=true;
        }
    }
    else
    {
        if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&          //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
           (Sen[SnRKManualTStart].IsOn() ||
           fOffSet->UseAutoOffsetFunction("Loader") ||                          //jou 2013-08-29 Use Auto Offset Funtion
           fOffSet->UseInArmSetupTeach(InOfsLoader)))                           //JerryYang 20180921 Setup Teach功能
            bResult=true;
    }

    if(bResult)
    {
        bEnterOffset=false;
        fMain->Pause("InArmNeedCheckOffset");
    }
    return bResult;
}
//==============================================================================
bool bCheckInShuttlePosAndNOIC()                                                //KenHsieh 20230614 : Inarm pick IC from tray需待SH到位且為可取放料狀態
{
    if(IniConfig.bE72_InarmPickICNeedWaitSH==false)
        return false;

    if(InArmSuck.iWhichSht==0)
    {
        if(InSHT1InLF()==false || FLCarryKit.UseSiteHasIC())
        {
            return true;
        }
    }
    else
    {
        if(InSHT2InLF()==false || BLCarryKit.UseSiteHasIC())
        {
            return true;
        }
    }

    return false;
}
//------------------------------------------------------------------------------
extern void DoInArm_9045_1x1_1();
extern void DoInArm_9045_1x2_2();
extern void DoInArm_9045_1x2_2_14();
extern void DoInArm_9045_1x2_4_Hot();
extern void DoInArm_9045_1x3_4();
extern void DoInArm_9045_1x3_2_14();
extern void DoInArm_9045_1x4_2();
extern void DoInArm_9045_1x4_4();
extern void DoInArm_9045_1x4_4_Back();
extern void DoInArm_9045S_1x4_4();
extern void DoInArm_9045_1x4_8_Hot();
extern void DoInArm_9045_2x1_2();
extern void DoInArm_9045_2x2_4();
extern void DoInArm_9045_2x2_4_12();
extern void DoInArm_9045_2x2_4_14();
extern void DoInArm_9045_2x2_8_Hot();
extern void DoInArm_9045_2x3_6();
extern void DoInArm_9045_2x3_6_14();
extern void DoInArm_9045_2x4_4();
extern void DoInArm_9045_2x4_4_13();
extern void DoInArm_9045_2x4_8();
extern void DoInArm_9045_2x5_8();
extern void DoInArm_9045_2x6_8();
extern void DoInArm_9045_2x8_8();
extern void DoInArm_9045_All_1Pick();
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
        MOT[MTestZ1].Gali_Command("ST", __FUNC__);
        return;
    }

    if(IniConfig.bAlarmNeedServoOff==true)                                      //JerryYang 20180514 新增保護避免servo off後推動in arm, 再重新start會發生點位錯誤
    {
        if(fNote->bMyServoOffInArm==true)
        {
            return;
        }
    }

    if(ASET_StartTimeNAME=="")                                                  //kevin 20150212 高雄日月光IC履歷記錄 先給檔案初始時間    //Steven 20160531 : modify
    {
        ASET_StartTimeNAME=Now().FormatString("yyyymmdd");                      //kevin 20161228 記錄天數為檔名
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

    if(bEject)                                                                  //JerryYang 20251020 : 渠梁半清機功能
    {
        return;
    }

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

/*    if((fHeaterOK==false ||
        fHeaterStableOK==false) &&                                              //JerryYang 20210122 : ASE-CL新增待溫功能
       (LastSet.iTemperature==Tempture_AmbientHot))
    {
        bHangTimePause=true;                                                    //kevin 20141017 add Steven 20090827 : Hang Up dectecto
        return;                                                                 //溫度過低
    }    */

    if(bInArmLaserActionflag[0]==true ||                                        //==> Eastsun 20260525 laser 整合
       bInArmLaserActionflag[1]==true)
    {
        return;
    }

    if(USE_PICKER_COUNT==ep1Picker)
    {
        DoInArm_9045_All_1Pick();
    }
    else if(iInArmType==e9045_1x1_1)
    {
        DoInArm_9045_1x1_1();
    }
    else if(iInArmType==e9045_1x2_2_13)
    {
        DoInArm_9045_1x2_2();
    }
    else if(iInArmType==e9045_1x2_2_14)
    {
        DoInArm_9045_1x2_2_14();
    }
    else if(iInArmType==e9045_1x2_4_Hot)
    {
        DoInArm_9045_1x2_4_Hot();
    }
    else if(iInArmType==e9045_1x3_2_14)
    {
        DoInArm_9045_1x3_2_14();
    }
    else if(iInArmType==e9045_1x3_4)
    {
        DoInArm_9045_1x3_4();
    }
    else if(iInArmType==e9045_1x4_1_Ac)
    {
        DoInArm_9045_1x1_1();
    }
    else if(iInArmType==e9045_1x4_4_13)
    {
        DoInArm_9045S_1x4_4();
    }
    else if(iInArmType==e9045_1x4_2_14)
    {
        DoInArm_9045_1x4_2();
    }
    else if(iInArmType==e9045_1x4_4_Back)
    {
        DoInArm_9045_1x4_4_Back();
    }
    else if(iInArmType==e9045_1x4_4)
    {
        DoInArm_9045_1x4_4();
    }
    else if(iInArmType==e9045_1x4_8_Hot)
    {
        DoInArm_9045_1x4_8_Hot();
    }
    else if(iInArmType==e9045_2x1_2_13)
    {
        DoInArm_9045_2x1_2();
    }
    else if(iInArmType==e9045_2x2_4_12)
    {
        DoInArm_9045_2x2_4_12();
    }
    else if(iInArmType==e9045_2x2_4_13)
    {
        DoInArm_9045_2x2_4();
    }
    else if(iInArmType==e9045_2x2_4_14)
    {
        DoInArm_9045_2x2_4_14();
    }
    else if(iInArmType==e9045_2x2_8_Hot)
    {
        DoInArm_9045_2x2_8_Hot();
    }
    else if(iInArmType==e9045_2x3_6_14)
    {
        DoInArm_9045_2x3_6_14();
    }
    else if(iInArmType==e9045_2x3_6)
    {
        DoInArm_9045_2x3_6();
    }
    else if(iInArmType==e9045_2x4_4_13)
    {
        DoInArm_9045_2x4_4_13();
    }
    else if(iInArmType==e9045_2x4_4_14)
    {
        DoInArm_9045_2x4_4();
    }
    else if(iInArmType==e9045_2x4_8)
    {
        DoInArm_9045_2x4_8();
    }
    else if(iInArmType==e9045_2x5_8)
    {
        DoInArm_9045_2x5_8();
    }
    else if(iInArmType==e9045_2x6_8)
    {
        DoInArm_9045_2x6_8();
    }
    else if(iInArmType==e9045_2x8_8)
    {
        DoInArm_9045_2x8_8();
    }
    else if(iInArmType==e9045_2x8_32)
    {
        DoInArm_9045_2x8_8();
    }
    else
    {
        Str.sprintf("iInArmType=%d", iInArmType);                               //Steven 20220620 : add log message
        ShowMyMessage("Program Error in DoInArm_9045()", Str);
    }
}
//-----------------------------------------------------------------------------
extern void DoInArm_9045_1x1_1_SuckerMap();
extern void DoInArm_9045_1x2_2_SuckerMap();
extern void DoInArm_9045_1x2_2_14_SuckerMap();
extern void DoInArm_9045_1x2_4_Hot_SuckerMap();
extern void DoInArm_9045_1x3_2_14_SuckerMap();
extern void DoInArm_9045_1x3_4_SuckerMap();
extern void DoInArm_9045_1x3_2_14_SuckerMap();
extern void DoInArm_9045_1x4_2_SuckerMap();
extern void DoInArm_9045_1x4_4_SuckerMap();
extern void DoInArm_9045_1x4_4_Back_SuckerMap();
extern void DoInArm_9045S_1x4_4_SuckerMap();
extern void DoInArm_9045_1x4_8_Hot_SuckerMap();
extern void DoInArm_9045_2x1_2_SuckerMap();
extern void DoInArm_9045_2x2_4_SuckerMap();
extern void DoInArm_9045_2x2_4_12_SuckerMap();
extern void DoInArm_9045_2x2_4_14_SuckerMap();
extern void DoInArm_9045_2x2_8_Hot_SuckerMap();
extern void DoInArm_9045_2x3_6_SuckerMap();
extern void DoInArm_9045_2x3_6_14_SuckerMap();
extern void DoInArm_9045_2x4_4_SuckerMap();
extern void DoInArm_9045_2x4_4_13_SuckerMap();
extern void DoInArm_9045_2x4_8_SuckerMap();
extern void DoInArm_9045_2x5_8_SuckerMap();
extern void DoInArm_9045_2x6_8_SuckerMap();
extern void DoInArm_9045_2x8_8_SuckerMap();
extern void DoInArm_9045_All_1Pick_SuckerMap();
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
            Prod.iSiteMap[0][i][j]=-1;                                          //AI(general) 20260408 (RogerYang) : fix copy-paste bug, was [0][0][j] missing row index i
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

    if(iInArmType==e9045_1x1_1 ||
       iInArmType==e9045_1x4_1_Ac)
    {
        DoInArm_9045_1x1_1_SuckerMap();
    }
    else if(iInArmType==e9045_1x2_2_13)
    {
        i1x2_4UseACEGPicker=0;
        DoInArm_9045_1x2_2_SuckerMap();
    }
    else if(iInArmType==e9045_1x2_2_14)
    {
        i1x2_4UseACEGPicker=0;
        DoInArm_9045_1x2_2_14_SuckerMap();
    }
    else if(iInArmType==e9045_1x2_4_Hot)
    {
        i1x2_4UseACEGPicker=0;
        DoInArm_9045_1x2_4_Hot_SuckerMap();
    }
    else if(iInArmType==e9045_1x3_4)
    {
        DoInArm_9045_1x3_4_SuckerMap();
    }
    else if(iInArmType==e9045_1x3_2_14)
    {
        DoInArm_9045_1x3_2_14_SuckerMap();
    }
    else if(iInArmType==e9045_1x4_4_13)
    {
        DoInArm_9045S_1x4_4_SuckerMap();
    }
    else if(iInArmType==e9045_1x4_2_14)
    {
        DoInArm_9045_1x4_2_SuckerMap();
    }
    else if(iInArmType==e9045_1x4_4_Back)
    {
        DoInArm_9045_1x4_4_Back_SuckerMap();
    }
    else if(iInArmType==e9045_1x4_4)
    {
        DoInArm_9045_1x4_4_SuckerMap();
    }
    else if(iInArmType==e9045_1x4_8_Hot)
    {
        DoInArm_9045_1x4_8_Hot_SuckerMap();
    }
    else if(iInArmType==e9045_2x1_2_13)
    {
        DoInArm_9045_2x1_2_SuckerMap();
    }
    else if(iInArmType==e9045_2x2_4_12)
    {
        DoInArm_9045_2x2_4_12_SuckerMap();
    }
    else if(iInArmType==e9045_2x2_4_13)
    {
        DoInArm_9045_2x2_4_SuckerMap();
    }
    else if(iInArmType==e9045_2x2_4_14)
    {
        DoInArm_9045_2x2_4_14_SuckerMap();
    }
    else if(iInArmType==e9045_2x2_8_Hot)
    {
        DoInArm_9045_2x2_8_Hot_SuckerMap();
    }
    else if(iInArmType==e9045_2x3_6_14)
    {
        DoInArm_9045_2x3_6_14_SuckerMap();
    }
    else if(iInArmType==e9045_2x3_6)
    {
        DoInArm_9045_2x3_6_SuckerMap();
    }
    else if(iInArmType==e9045_2x4_4_13)
    {
        DoInArm_9045_2x4_4_13_SuckerMap();
    }
    else if(iInArmType==e9045_2x4_4_14)
    {
        DoInArm_9045_2x4_4_SuckerMap();
    }
    else if(iInArmType==e9045_2x4_8)
    {
        DoInArm_9045_2x4_8_SuckerMap();
    }
    else if(iInArmType==e9045_2x5_8)
    {
        DoInArm_9045_2x5_8_SuckerMap();
    }
    else if(iInArmType==e9045_2x6_8)
    {
        DoInArm_9045_2x6_8_SuckerMap();
    }
    else if(iInArmType==e9045_2x8_8)
    {
        DoInArm_9045_2x8_8_SuckerMap();
    }
    else if(iInArmType==e9045_2x8_32)
    {
        DoInArm_9045_2x8_8_SuckerMap();
    }
    else
    {
        Str.sprintf("iInArmType=%d", iInArmType);                               //Steven 20220620 : add log message
        ShowMyMessage("Program Error in DoInArm_9045_SuckerMap()", Str);
    }

    DoInArm_SuckerMapForCloseArm();

    if(IniConfig.bUseAutoSiteMapping            &&                              //JerryYang 20160419 add auto site mapping
       Prod.bInitialAutoSiteMap==false          &&
       LastSet.iRunStartMode==rsmAutoSiteMap    &&
       bSiteMappingCHKOK==false                 &&
       bSiteMapHotplateReady==true              &&                              //Ifor 20170920 (Steven) : add Auto Site Mapping Hotplate 擺放旗標
       bRunAutoClean==false)                                                    //jou 20200618 : 修正 Auto Clean 跟 Auto site mapping 衝突
    {
        DoInArmAutoSiteMapping();                                               //Steven 20240427 : 重新整合auto site map功能
    }
    else if(fContact->IsRun2DCheck())                                           //JerryYang 20250220 : 2DID硬體順序檢查功能
    {
        GetInArm2DIDMapping();
    }
}
//==============================================================================
bool TrayYDirForArmYPitch()
{
    int iLoadYPitch=Prod.LoadForm.iYPitch;

    if(bInArmPickErrFromLoader)                                                 //Steven 20170828 (wei) : 確認是否有在Loader吸料異常
        return false;

    if(USE_IN_Y_IS_AUTO_PITCH==true)                                            //Steven for HT1032   //JerryYang 20251218 : IN/OUT ARM支援不同模組  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        iCloseSiteState=CloseSiteState();                                       //Steven 20210420 : 修正12site關site跑2x4 mode, HP=6x11 hang up
        if(CosFunction.bYPitchNotUseSearchLastMode &&
           ArmSpeed_File[InArm].bYPitchUseSearchLastMode==0)
        {
            return false;                                                       //kevin 20211130  Y PITCH 不使用一排一排 吸
        }
        else if(iCloseSiteState==0 &&
                AutoCalculateInArmYClosePitch(false, true)!=0)
        {
            return true;
        }
    }
    else if(iLoadYPitch==TestIF.iARM_Y_PITCH)
    {
        return true;
    }
    else if(LoadTrayCanUse8Suck()==1 && TestIF.bSearchLastMode)                 //kevin 20171120 (wei) 判斷LOAD TRAY Y PITCH是否是 ARM Y PITCH可以一次8吸嘴吸IC
    {
        return true;
    }
    return false;
}
//==============================================================================
int GetLoaderYPitchStep()                                                       //Steven 20141112
{
    int iYPitch=LoadForm->YPitch;
    int iYPitchStep=0;

    if(TrayYDirForArmYPitch()==false)
    {
        iYPitchStep=0;
    }
    else
    {
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //ChungHung 20140304 add for AutoYPitch
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            iYPitchStep=(iYPitch==0)?0:(AutoCalculateInArmYClosePitch()/iYPitch);                                       //RogerYang 20250618 防止除以零     //ChungHung 20131231 alter AutoYPitch
        }
        else
        {
            if(USE_IN_OUT_ARM_Y_PITCH==iXPitch60)
            {
                iYPitch=iYPitch/10;
                iYPitch=iYPitch*10;
            }

            iYPitchStep=(iYPitch==0)?0:(TestIF.iARM_Y_PITCH/iYPitch);           //RogerYang 20250618 防止除以零
        }
    }
    return iYPitchStep;
}
//------------------------------------------------------------------------------
void InArmAddBlockPitch(int &iXPos, int &iYPos, int indexRow, int indexCol, int iXY)                                    //Frank 20160928 add Subtray Function
{                                                                               //2014-03-04    Dell    for SPIL WLP Add Tray Block
    if(IniConfig.bUseTrayBlockMode &&
       IniConfig.bP06_LoaderUseCarrierTray==true)                               //Ifor 20181012 (Steven) : add Block Pitch Calculation
    {
        if(iXY==1)
        {
            iXPos=iXPos+((ChangeToFloatNonPcnt((double)(indexCol), (double)(LoadForm->XDivision)))*LoadForm->BlockPitchX);
            iXPos=iXPos-((ChangeToFloatNonPcnt((double)(indexCol), (double)(LoadForm->XDivision)))*(LoadForm->XPitch*LoadForm->XDivision));
        }
        else if(iXY==2)
        {
            iYPos=iYPos-((ChangeToFloatNonPcnt((double)(indexRow), (double)(LoadForm->YDivision)))*LoadForm->BlockPitchY);
            iYPos=iYPos+((ChangeToFloatNonPcnt((double)(indexRow), (double)(LoadForm->YDivision)))*(LoadForm->YPitch*LoadForm->YDivision));
        }
    }
}
//==============================================================================
int AutoCalculateInArmYClosePitch(bool bStart, bool bCheckIsZero)               //ChungHung 20131231 alter AutoYPitch
{
    int Ld=TrayForm.Loader.iTrayType;
    int TrayYPitch=UserDefForm[Ld].YPitch;
    int iMin=2500;                                                              //KevinYang 20210126 : 3500 --> 2500

    if(bStart==false)                                                           //Steven 20190314 : for Y-Pitch
    {
        return InArmClose_PitchY;
    }
    else
    {
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //ChungHung 20140304 add for AutoYPitch
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            if(TestIF_File.iTestMode==SingleSite)                               //kevin 20210104
            {
                InArmClose_PitchY=6000;
                return InArmClose_PitchY;                                       //Steven 20240201 : TestIF.iARM_Y_PITCH --> InArmClose_PitchY
            }

            if(IN_OUT_ARM_Y_PITCH_MIN>iMin)
                iMin=IN_OUT_ARM_Y_PITCH_MIN;

            if(TrayYPitch>=iMin && TrayYPitch<=IN_OUT_ARM_Y_PITCH_MAX)
            {
                InArmClose_PitchY=TrayYPitch;
                return InArmClose_PitchY;
            }

            if(TrayYPitch<iMin)
            {
                int i=0;
                do
                {
                    i++;
                    if(TrayYPitch*i>=iMin)                                      //18800 @ 16x39 (MIN:2500)
                    {                                                           //19600 @ 16x39 (MIN:3100)
                        InArmClose_PitchY=TrayYPitch*i;
                        break;
                    }
                }while(TrayYPitch*i<=IN_OUT_ARM_Y_PITCH_MAX);

                if(InArmClose_PitchY<=IN_OUT_ARM_Y_PITCH_MAX)
                {
                    return InArmClose_PitchY;
                }
                else
                {
                    InArmClose_PitchY=TestIF.iARM_Y_PITCH;
                }
            }
            else
            {
                InArmClose_PitchY=TestIF.iARM_Y_PITCH;
            }
        }
        else
        {
            InArmClose_PitchY=TestIF.iARM_Y_PITCH;
        }

        return InArmClose_PitchY;                                               //Steven 20240201 : TestIF.iARM_Y_PITCH --> InArmClose_PitchY
    }
}
//==============================================================================
double dInArmXPitch_1Step=0.0;                                                  //1吸嘴之間的X-Pitch
double dInArmXPitch_MovePitch=0.0;                                              //張到最開的X-Pitch
int    iInArmXStep=1;                                                           //In Arm在Tray X一次要跳幾格
int    iInArmYStep=1;                                                           //In Arm在Tray Y一次要跳幾格
//==============================================================================
int AutoCalculateInArmXClosePitch()
{
    int iStep;
    int Ld=TrayForm.Loader.iTrayType;
    InArmClose_PitchX=0;
    iTotalXPitchStep    =(USE_16PICKER_TYPE==1)?7:3;
    iMaxXPitch4Pick     =(USE_16PICKER_TYPE==1)?iXpitchMaxX2:iXpitchMax;
    iMinXPitch4Pick     =(USE_16PICKER_TYPE==1)?iXpitchMinX2:iXpitchMin;
    iMaxXPitch2Pick_13  =(USE_16PICKER_TYPE==1)?iXpitchMaxX4:iXpitchMaxX2;
    iMinXPitch2Pick_13  =(USE_16PICKER_TYPE==1)?iXpitchMinX4:iXpitchMinX2;
    iMaxXPitch2Pick_14  =(USE_16PICKER_TYPE==1)?iXpitchMaxX6:iXpitchMaxX3;
    iMinXPitch2Pick_14  =(USE_16PICKER_TYPE==1)?iXpitchMinX6:iXpitchMinX3;

    if(USE_16PICKER_TYPE==1 &&                                                  //Ztex 2023.12.06 Add HT-1032
       InArmSuck.iPickCol==8)
    {
        for(int i=1; i<UserDefForm[Ld].XDivision; i++)
        {
            iInArmXStep=i;
            InArmClose_PitchX=UserDefForm[Ld].XPitch*iInArmXStep;
            if(InArmClose_PitchX>=iXpitchMin && InArmClose_PitchX<=iXpitchMax)
            {
                dInArmXPitch_1Step      =InArmClose_PitchX;
                dInArmXPitch_MovePitch  =InArmClose_PitchX*7;
                return InArmClose_PitchX;
            }
        }
    }
    else if(bUseAxxGPicker() ||                                                 //Steven 20240515 : 往上移動
            iCloseSiteModeFor1x4==e1x4CloseAbAc)                                //Steven 20241111 : for 1x4 close 2 site
    {
        if((UserDefForm[Ld].XDivision%2)==0)
        {
            iInArmXStep=(UserDefForm[Ld].XDivision/2);
            InArmClose_PitchX=UserDefForm[Ld].XPitch*iInArmXStep;
            if(InArmClose_PitchX>=iMinXPitch2Pick_14 && InArmClose_PitchX<=iMaxXPitch2Pick_14)
            {
                dInArmXPitch_1Step      =(USE_16PICKER_TYPE==1)?(double(InArmClose_PitchX)/6.0):(double(InArmClose_PitchX)/3.0);
                dInArmXPitch_MovePitch  =(USE_16PICKER_TYPE==1)?(dInArmXPitch_1Step*7.0):(dInArmXPitch_1Step*3.0);
                return InArmClose_PitchX;
            }
        }
        else
        {
            iStep=UserDefForm[Ld].XDivision/2+1;
            for(int i=iStep; i>=1; i--)                                         //Steven 20241231 : > --> >=
            {
                iInArmXStep=i;
                InArmClose_PitchX=UserDefForm[Ld].XPitch*iInArmXStep;
                if(InArmClose_PitchX>=iMinXPitch2Pick_14 && InArmClose_PitchX<=iMaxXPitch2Pick_14)
                {
                    dInArmXPitch_1Step      =(USE_16PICKER_TYPE==1)?(double(InArmClose_PitchX)/6.0):(double(InArmClose_PitchX)/3.0);
                    dInArmXPitch_MovePitch  =(USE_16PICKER_TYPE==1)?(dInArmXPitch_1Step*7.0):(dInArmXPitch_1Step*3.0);
                    return InArmClose_PitchX;
                }
            }
        }
    }
    else if(bUseAxExPicker())                                                   //Steven 20240515 : 往上移動
    {
        if((UserDefForm[Ld].XDivision%2)==0)
        {
            iInArmXStep=(UserDefForm[Ld].XDivision/2);
            InArmClose_PitchX=UserDefForm[Ld].XPitch*iInArmXStep;
            if(InArmClose_PitchX>=iMinXPitch2Pick_13 && InArmClose_PitchX<=iMaxXPitch2Pick_13)
            {
                dInArmXPitch_1Step      =(USE_16PICKER_TYPE==1)?(double(InArmClose_PitchX)/4.0):(double(InArmClose_PitchX)/2.0);
                dInArmXPitch_MovePitch  =(USE_16PICKER_TYPE==1)?(dInArmXPitch_1Step*7.0):(dInArmXPitch_1Step*3.0);
                return InArmClose_PitchX;
            }
        }
        else
        {
            iStep=UserDefForm[Ld].XDivision/2+1;
            for(int i=iStep; i>=1; i--)                                         //Steven 20241231 : > --> >=
            {
                iInArmXStep=i;
                InArmClose_PitchX=UserDefForm[Ld].XPitch*iInArmXStep;
                if(InArmClose_PitchX>=iMinXPitch2Pick_13 && InArmClose_PitchX<=iMaxXPitch2Pick_13)
                {
                    dInArmXPitch_1Step      =(USE_16PICKER_TYPE==1)?(double(InArmClose_PitchX)/4.0):(double(InArmClose_PitchX)/2.0);                            //Steven 20240807 : 2.0 --> 3.0  //Steven 20240902 : 2.0才對!!
                    dInArmXPitch_MovePitch  =(USE_16PICKER_TYPE==1)?(dInArmXPitch_1Step*7.0):(dInArmXPitch_1Step*3.0);
                    return InArmClose_PitchX;
                }
            }
        }
    }
    else if(InArmSuck.iPickCol==4 ||                                            //Steven 20240515 : 吸嘴數少的要先判斷
            InArmSuck.iPickCol==1)
    {
        for(int i=1; i<UserDefForm[Ld].XDivision; i++)
        {
            iInArmXStep=i;
            InArmClose_PitchX=UserDefForm[Ld].XPitch*i;
            if(InArmClose_PitchX>=iMinXPitch4Pick && InArmClose_PitchX<=iMaxXPitch4Pick)
            {
                dInArmXPitch_1Step      =(USE_16PICKER_TYPE==1)?(double(InArmClose_PitchX)/2.0):InArmClose_PitchX;
                dInArmXPitch_MovePitch  =(USE_16PICKER_TYPE==1)?(dInArmXPitch_1Step*7.0):(dInArmXPitch_1Step*3.0);
                return InArmClose_PitchX;
            }
        }
    }
    else
//    else if((TestIF.iTestMode==_8Site2X4  ||
//             TestIF.iTestMode==_10Site2X5 ||                                  //wei 20190614 10 site
//             TestIF.iTestMode==_12Site2X6 ||                                  //Steven 20141122 : 補上12, 16, 32 Site的X-Pitch模式
//             TestIF.iTestMode==_16Site4X4 ||                                  //kevin 20210916 add
//             TestIF.iTestMode==_16Site2X8 ||
//             TestIF.iTestMode==_32Site4X8N ||
//             TestIF.iTestMode==_32Site4X8M ||
//             TestIF.iTestMode==QualSite1X4) &&                                //JerryYang 20230322 : fix 1x4_14吸嘴
//             ArmCanSuck4IC(0)==false)                                         // 2x4 big ic
    {
        for(int i=1; i<UserDefForm[Ld].XDivision; i++)
        {
            iInArmXStep=i;
            InArmClose_PitchX=UserDefForm[Ld].XPitch*i;
            if(InArmClose_PitchX>=iMinXPitch4Pick && InArmClose_PitchX<=iMaxXPitch4Pick)
            {
                dInArmXPitch_1Step      =(USE_16PICKER_TYPE==1)?(double(InArmClose_PitchX)/2.0):InArmClose_PitchX;
                dInArmXPitch_MovePitch  =(USE_16PICKER_TYPE==1)?(dInArmXPitch_1Step*7.0):(dInArmXPitch_1Step*3.0);
                return InArmClose_PitchX;
            }
        }
    }

    iInArmXStep=1;
    InArmClose_PitchX       =(USE_16PICKER_TYPE==1)?(double(iMaxXPitch4Pick)/2.0):iMaxXPitch4Pick;
    dInArmXPitch_1Step      =(USE_16PICKER_TYPE==1)?(double(InArmClose_PitchX)/2.0):InArmClose_PitchX;
    dInArmXPitch_MovePitch  =(USE_16PICKER_TYPE==1)?(dInArmXPitch_1Step*7.0):(dInArmXPitch_1Step*3.0);
    return -1;
}
//------------------------------------------------------------------------------
void GetInArmToLoaderPosition(int iSelRow, int &iXPos, int &iYPos, int iRow, int iCol)
{
    iYPosition=iRow;

//    if(InArmSuck.iPickCol==1)
//    {
//        iXPosition[0]=iCol;
//        iXPosition[1]=iCol+1;                                                   //Steven 20240920 : Fixed for 1x1 mode
//        iXPosition[2]=iCol+2;                                                   //Steven 20240920 : Loader吸料計算的資料錯誤;
//        iXPosition[3]=iCol+3;
//    }
//    else
    if(bUseAxxGPicker()||
      iCloseSiteModeFor1x4==e1x4CloseAbAc)                                      //Steven 20241111 : for 1x4 close 2 site
    {
        iXPosition[0]=iCol;
        iXPosition[3]=iCol+iInArmXStep;
    }
    else if(bUseAxExPicker())
    {
        iXPosition[0]=iCol;
        iXPosition[2]=iCol+iInArmXStep;
    }
    else if(InArmSuck.iPickCol==4 ||
            InArmSuck.iPickCol==1)                                              //Steven 20240920 : Fixed for 1x1 mode
    {
        iXPosition[0]=iCol;
        iXPosition[1]=iCol+1*iInArmXStep;
        iXPosition[2]=iCol+2*iInArmXStep;
        iXPosition[3]=iCol+3*iInArmXStep;
    }
    else                                                                        //if(InArmSuck.iPickCol==8)
    {
        iXPosition[0]=iCol+0*iInArmXStep;
        iXPosition[1]=iCol+1*iInArmXStep;
        iXPosition[2]=iCol+2*iInArmXStep;
        iXPosition[3]=iCol+3*iInArmXStep;
        iXPosition[4]=iCol+4*iInArmXStep;
        iXPosition[5]=iCol+5*iInArmXStep;
        iXPosition[6]=iCol+6*iInArmXStep;
        iXPosition[7]=iCol+7*iInArmXStep;
    }

    if(IniConfig.bE46_LoaderUse2Offset && iSelRow==1 &&                         //Steven 20140827 : Loader可以分別調前後排的Offset
       TestIF.bEnableAutoAlignment==false)                                      //KenHsieh 20220923 : add AOA功能開啟不補償
    {                                                                           //基準移動到吸取的位置    //第一支移動到基準位置
        iXPos=Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+iCol*Prod.LoadForm.iXPitch+iInArmXBase*dInArmXPitch_1Step-InArmOffSet[InOfsLoader]->GetX()+InArmOffSet[InOfsLoaderB]->GetX();
        iYPos=Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]-iRow*Prod.LoadForm.iYPitch-InArmOffSet[InOfsLoader]->GetY()+InArmOffSet[InOfsLoaderB]->GetY();
    }
    else
    {
        if(USE_PICKER_COUNT==ep1Picker)
            iXPos=Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+iCol*Prod.LoadForm.iXPitch;
        else
            iXPos=Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+iCol*Prod.LoadForm.iXPitch+iInArmXBase*dInArmXPitch_1Step;
        iYPos=Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]-iRow*Prod.LoadForm.iYPitch;
    }
}
//------------------------------------------------------------------------------
int GetInArmToLoaderPosition_Single(int iSelRow, int &iXPos, int &iYPos, int iRow, int iCol)
{
    int iRealUseSuck=-1, iCurrSuck=-1, iTrayXPitch;
    bool bHasMatch=false;
    iYPosition=iRow;

    for(int i=0; i<4; i++)
    {
        if(InArmSuckUse[iSelRow][i]==true)
        {
            iCurrSuck=i;
            for(int j=(i+1); j<4; j++)
            {
                InArmSuckUse[iSelRow][j]=false;
                InArmSuckUse[1-iSelRow][j]=false;
            }
            break;
        }
    }

    if(iCurrSuck<0 ||
       iCurrSuck>4)
    {
        return dInArmXPitch_MovePitch;                                          //Steven 20240701 : 修正Fix模式下, X Pitch錯誤
    }

    iRealUseSuck=InArmSuck.Suck[iSelRow][iCurrSuck].iMyCol;

    if(USE_PICKER_COUNT==ep1Picker)
        iXPos=double(Prod.XInArm_Tray_Pick[iSelRow][iCurrSuck])+double(iCol*Prod.LoadForm.iXPitch);
    else                                                                        //基準移動到吸取的位置                    //第一支移動到基準位置 - 要使用的吸嘴移動到第一支的位置
        iXPos=double(Prod.XInArm_Tray_Pick[iSelRow][iCurrSuck])+double(iCol*Prod.LoadForm.iXPitch)+double(iInArmXBase-iRealUseSuck)*dInArmXPitch_1Step;
    if(IniConfig.bUseTrayBlockMode)                                             //Frank 20160928 add Subtray Function
        InArmAddBlockPitch(iXPos, iYPos, iRow, iCol, 1);

    if(iXPos>(MOT[MInArmX].Motor->PSoftLimitP-100))
    {
        if(InArmSuck.iPickCol==1)
        {
            bHasMatch=true;
            dInArmXPitch_1Step    =iXpitchMin;
            dInArmXPitch_MovePitch=iXpitchMinX7;
        }
        else if(bUseAxxGPicker() ||
                iCloseSiteModeFor1x4==e1x4CloseAbAc)                            //Steven 20241111 : for 1x4 close 2 site)
        {
            for(int i=0; i<Prod.LoadForm.iXItem; i++)
            {
                iTrayXPitch=DeviceForm.XDimension*i+200;
                if(USE_16PICKER_TYPE==1)                                        //Ztex 2023.12.06 Add HT-1032
                {
                    if(bHasMatch==false &&
                       iTrayXPitch>=iXpitchMinX6 &&
                       iTrayXPitch<=iXpitchMaxX6)
                    {
                        bHasMatch=true;
                        dInArmXPitch_1Step      =double(iTrayXPitch)/6.0;
                        dInArmXPitch_MovePitch  =dInArmXPitch_1Step*7.0;
                    }
                }
                else
                {
                    if(bHasMatch==false &&
                       iTrayXPitch>=iXpitchMinX3 &&
                       iTrayXPitch<=iXpitchMaxX3)
                    {
                        bHasMatch=true;
                        dInArmXPitch_1Step      =double(iTrayXPitch)/3.0;
                        dInArmXPitch_MovePitch  =iTrayXPitch;                   //dInArmXPitch_1Step*3.0;
                    }
                }
            }
        }
        else if(bUseAxExPicker())
        {
            for(int i=0; i<Prod.LoadForm.iXItem; i++)
            {
                iTrayXPitch=DeviceForm.XDimension*i+200;
                if(USE_16PICKER_TYPE==1)                                        //Ztex 2023.12.06 Add HT-1032
                {
                    if(bHasMatch==false &&
                       iTrayXPitch>=iXpitchMinX4 &&
                       iTrayXPitch<=iXpitchMaxX4)
                    {
                        bHasMatch=true;
                        dInArmXPitch_1Step      =double(iTrayXPitch)/4.0;
                        dInArmXPitch_MovePitch  =dInArmXPitch_1Step*7.0;
                    }
                }
                else
                {
                    if(bHasMatch==false &&
                       iTrayXPitch>=iXpitchMinX2 &&
                       iTrayXPitch<=iXpitchMaxX2)
                    {
                        bHasMatch=true;
                        dInArmXPitch_1Step      =double(iTrayXPitch)/2.0;
                        dInArmXPitch_MovePitch  =dInArmXPitch_1Step*3.0;
                    }
                }
            }
        }
        else if(InArmSuck.iPickCol==4)
        {
            for(int i=0; i<Prod.LoadForm.iXItem; i++)
            {
//                iTrayXPitch=Prod.LoadForm.iXPitch*i;
                iTrayXPitch=DeviceForm.XDimension*i+200;                        //Steven 20250122 : 修正A吸嘴到最右邊下不去
                if(USE_16PICKER_TYPE==1)                                        //Ztex 2023.12.06 Add HT-1032
                {
                    if(bHasMatch==false &&
                       iTrayXPitch>=iXpitchMinX2 &&
                       iTrayXPitch<=iXpitchMaxX2)
                    {
                        bHasMatch=true;
                        dInArmXPitch_1Step      =double(iTrayXPitch)/2.0;
                        dInArmXPitch_MovePitch  =dInArmXPitch_1Step*7.0;
                    }
                }
                else
                {
                    if(bHasMatch==false &&
                       iTrayXPitch>=iXpitchMin &&
                       iTrayXPitch<=iXpitchMax)
                    {
                        bHasMatch=true;
                        dInArmXPitch_1Step      =iTrayXPitch;
                        dInArmXPitch_MovePitch  =iTrayXPitch*3.0;
                        break;
                    }
                }
            }
        }
        else if(InArmSuck.iPickCol==8)
        {
            bHasMatch=true;
            if(Prod.LoadForm.iXPitch*7.0<=iXpitchMinX7)
            {
                dInArmXPitch_1Step      =iXpitchMin;
                dInArmXPitch_MovePitch  =iXpitchMinX7;
            }
            else if(Prod.LoadForm.iXPitch*7.0<=iXpitchMaxX7 &&
                    Prod.LoadForm.iXPitch*7.0>=iXpitchMinX7)                    //Steven 20140520 : 大IC使用Fix時,吸嘴B吸Loader最右側可能會Close Pitch導致飛料
            {
                dInArmXPitch_1Step      =Prod.LoadForm.iXPitch;
                dInArmXPitch_MovePitch  =Prod.LoadForm.iXPitch*7.0;
            }
            else
            {
                AutoCalculateInArmXClosePitch();
            }
        }
    }
    else
    {
        bHasMatch=true;
    }

    if(bHasMatch==false)
    {
        if(USE_16PICKER_TYPE==1)                                                //Ztex 2023.12.06 Add HT-1032
        {
            dInArmXPitch_1Step      =iXpitchMax;
            dInArmXPitch_MovePitch  =iXpitchMaxX7;
        }
        else
        {
            dInArmXPitch_1Step      =iXpitchMax;
            dInArmXPitch_MovePitch  =iXpitchMaxX3;
        }
    }

    if(USE_PICKER_COUNT==ep1Picker)
        iXPos=double(Prod.XInArm_Tray_Pick[iSelRow][iCurrSuck])+double(iCol*Prod.LoadForm.iXPitch);
    else                                                                        //基準移動到吸取的位置            //第一支移動到基準位置 - 要使用的吸嘴移動到第一支的位置
        iXPos=double(Prod.XInArm_Tray_Pick[iSelRow][iCurrSuck])+double(iCol*Prod.LoadForm.iXPitch)+double(iInArmXBase-iRealUseSuck)*dInArmXPitch_1Step;
    iYPos=Prod.YInArm_Tray_Pick[iSelRow][iCurrSuck]-iRow*Prod.LoadForm.iYPitch;
    if(IniConfig.bUseTrayBlockMode)                                             //Frank 20160928 add Subtray Function
        InArmAddBlockPitch(iXPos, iYPos, iRow, iCol, 2);

    iXPosition[iCurrSuck]=iCol;
    iYPosition=iRow;

    return dInArmXPitch_MovePitch;
}
//------------------------------------------------------------------------------
char DateTime[256];
//------------------------------------------------------------------------------
void LotRecordUPH(AnsiString UPH_StartTime, AnsiString UPH_EndTime, AnsiString UPH_PauseTime, int iUPH)                 //wei 20151221 KYEC 儲存UPH紀錄
{
    AnsiString filename;
    AnsiString t;
    filename.sprintf("%s\\%s_UPH.csv", as9045UPH.c_str(), fLotInfo->edtSysLotID->Text.c_str());
    if(!FileExists(filename))
    {
        t.sprintf("%s, %s, %s, %s\n", "Start Time", "End Time", "Pause Time", "UPH");
        WriteDataToFile(filename, t);                                           //Steven 20160604 : add protect of fopen
    }
    t.sprintf("%s, %s, %s, %d\n", UPH_StartTime, UPH_EndTime, UPH_PauseTime, iUPH);
    WriteDataToFile(filename, t);
}
//---------------------------------------------------------------------------
void RecordUPH(int UPH)
{
    AnsiString filename="";
    AnsiString t="";
    filename.sprintf("%s\\%04d_%02d%02d%02d_UPH.csv", as9045UPH.c_str(), SystemYear, SystemMonth, SystemDate, SystemHour);
    MyForceDirectories(as9045UPH);

    if(fMain->bCloseExcelfinishflag)
    {
        t.sprintf("%s, %d\n", DateTime, UPH);
        WriteDataToFile(filename, t);
    }
}
//------------------------------------------------------------------------------
void RecordLotUPH_For_FOREHOPE_NINGBO(AnsiString UPH_StartTime, AnsiString UPH_EndTime, AnsiString UPH_PauseTime, int iUPH,int iTrayCount,int iOpenSiteNum)     //wei 20151221 KYEC 儲存UPH紀錄
{
//    AnsiString filename=AnsiString().sprintf("%s\\%s_UPH.csv", as9045UPH.c_str(), fLotInfo->edtSysLotID->Text.c_str());                              //Jimmychiu 20250902 : 客戶要求save by day
    Word year, month, day;
    DecodeDate(Now(), year, month, day);
    AnsiString sSaveFolder=AnsiString().sprintf("%s\\%d\\%d\\%d", as9045UPH.c_str(), year, month, day);                 //Jimmychiu 20250902 : 客戶要求save by day
    FileInfo().EnsureDirectoriesExist(sSaveFolder);
    AnsiString filename=AnsiString().sprintf("%02d%02d%02d_UPH.csv", year, month, day);
    filename=FileInfo().PathCombin(sSaveFolder, filename);
    AnsiString t="";

    if(!FileExists(filename))
    {
        t.sprintf("%s, %s, %s, %s, %s, %s", "Start Time", "End Time", "Pause Time", "UPH", "Tray Count", "Site Count");
        WriteDataToFile(filename, t);                                           //Steven 20160604 : add protect of fopen
    }
    t.sprintf("%s, %s, %s, %d, %d, %d", UPH_StartTime, UPH_EndTime, UPH_PauseTime, iUPH,iTrayCount,iOpenSiteNum);
    WriteDataToFile(filename, t);
}
//------------------------------------------------------------------------------
// 計算最近10次的UPH與平均數
//------------------------------------------------------------------------------
void __fastcall CalculateUPH(bool bReset)
{
    int iCount=0, iTotalUPH;
    double fConsumeSecond,fTimerMultiple;
    TDateTime tTempTime, tConsumeSecond;
    Word Hour, Min, Sec, MSec;

    tTempTime=Now();
    if(bReset || bOneTimes)
    {
        bOneTimes=false;
        RunInfo.iUPH=0;
        tUPH_StartTime=tTempTime;
        tUPH_PauseTime=0;
        iUPH_LoaderCount=0;

        fShowBinSelect->UPH_StringGrid->Cells[0][0]="Start Time";
        fShowBinSelect->UPH_StringGrid->Cells[1][0]="End Time";
        fShowBinSelect->UPH_StringGrid->Cells[2][0]="Pause Time";
        fShowBinSelect->UPH_StringGrid->Cells[3][0]="UPH";
        for(int i=1; i<=12; i++)
        {
            fShowBinSelect->UPH_StringGrid->Cells[0][i]="";
            fShowBinSelect->UPH_StringGrid->Cells[1][i]="";
            fShowBinSelect->UPH_StringGrid->Cells[2][i]="";
            fShowBinSelect->UPH_StringGrid->Cells[3][i]="";
        }

        if(IniConfig.bVTESTFunction==true)                                      //RogerYang 20250224 偉測需求 新增三列信息,耗時,數量,site
        {
            fShowBinSelect->UPH_StringGrid->Cells[4][0]="Elaps. Time";
            fShowBinSelect->UPH_StringGrid->Cells[5][0]="Total Units";
            fShowBinSelect->UPH_StringGrid->Cells[6][0]="Site";
            for(int i=1; i<=12; i++)
            {
                fShowBinSelect->UPH_StringGrid->Cells[4][i]="";
                fShowBinSelect->UPH_StringGrid->Cells[5][i]="";
                fShowBinSelect->UPH_StringGrid->Cells[6][i]="";
            }
        }
    }
    else
    {
        if(tTempTime>tUPH_StartTime)
        {
            for(int i=10; i>=1; i--)
            {
                fShowBinSelect->UPH_StringGrid->Cells[0][i]=fShowBinSelect->UPH_StringGrid->Cells[0][i-1];
                fShowBinSelect->UPH_StringGrid->Cells[1][i]=fShowBinSelect->UPH_StringGrid->Cells[1][i-1];
                fShowBinSelect->UPH_StringGrid->Cells[2][i]=fShowBinSelect->UPH_StringGrid->Cells[2][i-1];
                fShowBinSelect->UPH_StringGrid->Cells[3][i]=fShowBinSelect->UPH_StringGrid->Cells[3][i-1];

                if(IniConfig.bVTESTFunction==true)                              //RogerYang 20250224 偉測需求 新增三列信息,耗時,數量,site
                {
                    fShowBinSelect->UPH_StringGrid->Cells[4][i]=fShowBinSelect->UPH_StringGrid->Cells[4][i-1];
                    fShowBinSelect->UPH_StringGrid->Cells[5][i]=fShowBinSelect->UPH_StringGrid->Cells[5][i-1];
                    fShowBinSelect->UPH_StringGrid->Cells[6][i]=fShowBinSelect->UPH_StringGrid->Cells[6][i-1];
                }
            }

            tUPH_EndTime=tTempTime;
            tConsumeSecond=(tUPH_EndTime-tUPH_StartTime);
            if(IniConfig.bVTESTFunction==true)                                  //RogerYang 20250224 偉測需求 新增三列信息,耗時,數量,site (Elaps. Time 不包含 Pause Time)
            {
                fShowBinSelect->UPH_StringGrid->Cells[4][1]=tConsumeSecond.FormatString("hh:nn:ss");                    //Elaps. Time
                fShowBinSelect->UPH_StringGrid->Cells[5][1]=iUPH_LoaderCount;   //Total Units
                fShowBinSelect->UPH_StringGrid->Cells[6][1]=TestSocket.iShtRow*TestSocket.iShtCol;                      //Site數
            }
            fShowBinSelect->UPH_StringGrid->Cells[0][1]=tUPH_StartTime.FormatString("hh:nn:ss");                        //Steven 20090714
            fShowBinSelect->UPH_StringGrid->Cells[1][1]=tUPH_EndTime.FormatString("hh:nn:ss");                          //Steven 20090714
            fShowBinSelect->UPH_StringGrid->Cells[2][1]=tUPH_PauseTime.FormatString("hh:nn:ss");                        //Steven 20090714
            tUPH_StartTime=tUPH_EndTime;
            tConsumeSecond=tConsumeSecond-tUPH_PauseTime;
            tUPH_PauseTime=0;
            fConsumeSecond=0;
            DecodeTime(tConsumeSecond, Hour, Min, Sec, MSec);
            fConsumeSecond+=Hour*3600.0;
            fConsumeSecond+=Min*60.0;
            fConsumeSecond+=Sec;
            fConsumeSecond+=MSec/(double)1000.0;
            if(fConsumeSecond>0)                                                //Steven 20110418
                fTimerMultiple=3600/fConsumeSecond;
            else
                fTimerMultiple=0;

            sprintf(DateTime, "%02d:%02d:%02d",  SystemHour, SystemMin, SystemSec);                                     //Frank 20150515
            RunInfo.iUPH=fTimerMultiple*iUPH_LoaderCount;
            MyDBIUPH(RunInfo.iUPH);                                             //Steven 20190906 : Add UPH in EventLog
            int iLoaderCount=iUPH_LoaderCount;
            iUPH_LoaderCount=0;
            fShowBinSelect->UPH_StringGrid->Cells[3][1]=RunInfo.iUPH;           //Steven 20090714
            if(IniConfig.bP11RecordUPH)                                         //Frank 20150515
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE)
                {
                    LotRecordUPH(fShowBinSelect->UPH_StringGrid->Cells[0][1], fShowBinSelect->UPH_StringGrid->Cells[1][1], fShowBinSelect->UPH_StringGrid->Cells[2][1], RunInfo.iUPH);  //wei 20151221 KYEC 儲存UPH紀錄
                }
                else if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)                      //Jimmychiu 20250304 : P11新增CC_FOREHOPE_NINGBO
                {
                    AnsiString sStartTime=fShowBinSelect->UPH_StringGrid->Cells[0][1];
                    AnsiString sEndTime=fShowBinSelect->UPH_StringGrid->Cells[1][1];
                    AnsiString sPauseTime=fShowBinSelect->UPH_StringGrid->Cells[2][1];
                    int iTrayCount=iLoaderCount;
                    int iSiteCount=GetSiteCount(false);
                    RecordLotUPH_For_FOREHOPE_NINGBO(sStartTime, sEndTime, sPauseTime, RunInfo.iUPH, iTrayCount, iSiteCount);
                }
                else
                {
                    fMain->bCloseExcelflag=true;
                    fMain->bCloseExcelfinishflag=false;
                }
            }
            iTotalUPH=0;
            iCount=0;

            for(int i=0; i<20; i+=2)                                            //Steven 20140621 : For SECS GEM
            {
                fShowBinSelect->tsUPH->Strings[i  ]=fShowBinSelect->UPH_StringGrid->Cells[0][i/2+1];
                fShowBinSelect->tsUPH->Strings[i+1]=fShowBinSelect->UPH_StringGrid->Cells[3][i/2+1];
            }

            if(IniConfig.bEnable_SECS_GEM==true)                                //Steven 20140528 : Secs Gem
                EventReport(SECS_EVENT.UPHRecordEnd);                           //54 UPH Record End

            for(int i=1; i<=10; i++)
            {
#ifdef DEBUG_TRY_CATCH
                try
                {
#endif
                    if(fShowBinSelect->UPH_StringGrid->Cells[3][i]!="")
                    {
                        iTotalUPH+=atoi(AnsiString(fShowBinSelect->UPH_StringGrid->Cells[3][i]).c_str());
                        iCount++;
                    }
#ifdef DEBUG_TRY_CATCH
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "CalculateUPH");
                }
#endif
            }
            fShowBinSelect->UPH_StringGrid->Cells[2][12]="Avg UPH :";
            RunInfo.iAvgUPH=AnsiString((iCount==0)?0:iTotalUPH/iCount);
            fShowBinSelect->UPH_StringGrid->Cells[3][12]=RunInfo.iAvgUPH;
        }
        else
        {
            RunInfo.iUPH=0;
            tUPH_StartTime=tTempTime;
            fShowBinSelect->UPH_StringGrid->Cells[0][0]="Start Time";
            fShowBinSelect->UPH_StringGrid->Cells[1][0]="End Time";
            fShowBinSelect->UPH_StringGrid->Cells[2][0]="Pause Time";
            fShowBinSelect->UPH_StringGrid->Cells[3][0]="UPH";
            for(int i=1; i<=12; i++)
            {
                fShowBinSelect->UPH_StringGrid->Cells[0][i]="";
                fShowBinSelect->UPH_StringGrid->Cells[1][i]="";
                fShowBinSelect->UPH_StringGrid->Cells[2][i]="";
                fShowBinSelect->UPH_StringGrid->Cells[3][i]="";
            }

            if(IniConfig.bVTESTFunction==true)                                  //RogerYang 20250224 偉測需求 新增三列信息,耗時,數量,site
            {
                fShowBinSelect->UPH_StringGrid->Cells[4][0]="Elaps. Time";
                fShowBinSelect->UPH_StringGrid->Cells[5][0]="Total Units";
                fShowBinSelect->UPH_StringGrid->Cells[6][0]="Site";
                for(int i=1; i<=12; i++)
                {
                    fShowBinSelect->UPH_StringGrid->Cells[4][i]="";
                    fShowBinSelect->UPH_StringGrid->Cells[5][i]="";
                    fShowBinSelect->UPH_StringGrid->Cells[6][i]="";
                }
            }
        }
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20180417 只秀一個UPH 高雄
    {
        if(IniConfig.bG10ShowImmediateUPH==false)
        {
            fMain->StatusBar1->Panels->Items[7]->Text="";
            fMain->StatusBar1->Panels->Items[2]->Text="UPH = "+AnsiString(fShowBinSelect->UPH_StringGrid->Cells[3][1]);
        }
    }
    else
    {
        fMain->StatusBar1->Panels->Items[2]->Text="UPH = "+AnsiString(fShowBinSelect->UPH_StringGrid->Cells[3][1]);
    }
}
//==============================================================================
bool MoveInArmXYToLoader_9045(bool &bCanPick2ICAtOnceTime, int iSelRow, int &iRow, int &iCol, bool RealMove, bool ZNeedDown)
{
    AutoCalculateInArmXClosePitch();                                            //Steven 20240509 : 移到外面, 避免Fix mode沒執行
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable  =0;
    int iMovePitchX =dInArmXPitch_MovePitch;
    int iMovePitchY =AutoCalculateInArmYClosePitch()+IniConfig.iInArm60mmOffset;
    int Ld          =TrayForm.Loader.iTrayType;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(iSelRow==2)                                                              //ChungHung 20131231 alter AutoYPitch
    {
        iSelRow=0;
        if(bCanPick2ICAtOnceTime==false)                                        //Steven 20141029 : XY-Pitch with Fix Pitch mode
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
                InArmSuckUse[1][j]=false;
        }
    }

    if(bCanPick2ICAtOnceTime==true)
    {
        GetInArmToLoaderPosition(iSelRow, iXPos, iYPos, iRow, iCol);
    }
    else                                                                        //one by one suck
    {
        iMovePitchX=GetInArmToLoaderPosition_Single(iSelRow, iXPos, iYPos, iRow, iCol);                                 //Steven 20240701 : 修正Fix模式下, X Pitch錯誤
    }

    if(USE_IN_Y_IS_AUTO_PITCH==true)                                            //ChungHung 20140304 add for AutoYPitch  //JerryYang 20251218 : IN/OUT ARM支援不同模組  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(iInArmOrder==0)
        {
            iYPos=iYPos-iMovePitchY;
            if(iYPos<(MOT[MInArmY].Motor->PSoftLimitN+10))                      //JerryYang 20180221 (Steven) : fix y-pitch 最小25mm會有行程的問題, y-pitch要縮到最小
            {
                iYPos=iYPos+iMovePitchY;
                if((UserDefForm[Ld].YPitch-500)<IN_OUT_ARM_Y_PITCH_MIN)         //Steven 20230727 : 當TrayPitch比最小還小的時候,才能縮到Tray Pitch
                {
                    iMovePitchY=IN_OUT_ARM_Y_PITCH_MIN;
                }
                else
                {
                    if(UserDefForm[Ld].YPitch>6000)                             //Stevenhong 20260325 : TESNA 的IC 與Pitch 比較大
                    {
                        iMovePitchY=DeviceForm.YDimension/2+1000;
                    }
                else
                    {
                    iMovePitchY=UserDefForm[Ld].YPitch-500;
                    }
                }

                iYPos=iYPos-iMovePitchY;
            }
        }
    }
    else
    {
        iYPos=iYPos+iMovePitchY*iInArmOrder;
    }

    for(int i=0; i<InArmSuck.iMaxRow; i++)                                      //Eastsun 20251231 : 從下面移上來的
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            bZFlag[i][j]=(ZNeedDown)?InArmSuckUse[i][j]:false;                  //kevin 20210305 add Z motor 不往下
            iZPos[i][j] =(bZFlag[i][j])?Prod.ZInArm_Tray_Pick[i][j]:ZSafePos;
        }
    }

    if(bCanPick2ICAtOnceTime==false)                                            //Eastsun 20251231 : 單支吸嘴偏移功能
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)                                  //Eastsun 20251231 : 從下面移上來的
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(bZFlag[i][j])
                {
                    iXPos+=InArmOffSet[InOfsLoader]->GetArmX(i, j);             //Eastsun 20251231 : 本來依照架構須加在GetInArmToLoaderPosition_Single 但是沒辦法判斷吸嘴編號
                    iYPos+=InArmOffSet[InOfsLoader]->GetArmY(i, j);
                }
            }
        }
    }

    if(RealMove==false)
    {
        if(iXPos>=MOT[MInArmX].Motor->PSoftLimitP || iXPos<=MOT[MInArmX].Motor->PSoftLimitN)
            return false;
        if(iYPos>=MOT[MInArmY].Motor->PSoftLimitP || iYPos<=MOT[MInArmY].Motor->PSoftLimitN)
            return false;
        return true;
    }

//==>                                                                       //Eastsun 20251231 : 偏移需求往上移
//    for(int i=0; i<InArmSuck.iMaxRow; i++)
//    {
//        for(int j=0; j<InArmSuck.iMaxCol; j++)
//        {
//            bZFlag[i][j]=(ZNeedDown)?InArmSuckUse[i][j]:false;                  //kevin 20210305 add Z motor 不往下
//            iZPos[i][j] =(bZFlag[i][j])?Prod.ZInArm_Tray_Pick[i][j]:ZSafePos;
//        }
//    }

    iOffsetPos=GetInArmToLoaderOffset_9045(iSelRow);
    iYVariable=GetInArmPitchY_9045(iMovePitchY, iOffsetPos);
    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetInArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    TransferLoaderRatio(&iXPos, &iYPos);                                        //Steven 20141029 : Loader的軟體齒輪比
    DoInArmSuckPreOn(iXPos, iYPos);

    if(InArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, ZNeedDown, true))
    {
        if(bRecordUPH && iCol<=0 && iRow==0)
        {
            bRecordUPH=false;
            CalculateUPH(false);
        }
        return true;
    }
    return false;
}
//==============================================================================
int Find_InArm_PickerMaxUseCountOnTime(int iRowCT, int &isStart, int &iRow, int &iCol, bool bSearchLast)
{
    int cx, iMax=0, ct=0;
    bool backupFlag[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
    bool bSourceFlag[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
    int ixStart, ixEnd, ixStep;
    int iyStart, iyEnd, iyStep;
    static int iRecY=0;
    int iTrayDir=TrayForm.Loader.Direction;

    if(IniConfig.bQAMode==true &&
       LastSet.iRunStartMode==rsmQAMode &&
       CosFunction.bQAmodeSupplyTrayDir==true)                                  //jou 2014-10-24 QA mode 增加支援吸Tray方向
    {
        iTrayDir=TestIF_File.iQATrayDirect;
    }

    if(CUSTOMER_CODE==CC_JCET                           &&                      //Ifor 20171108 (Steven) add JCET 要求 Loader 後排吸取不到至前排吸取
       bInArmPickErrFromLoader==true                    &&
       IniConfig.bE56LoaderRetryAtSamePosition==false   &&
       USE_IN_Y_IS_AUTO_PITCH==false                          )                 //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        bSearchLast=false;
    }

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            bSourceFlag[i][j]=bInArmToLoaderUsage[i][j];
        }
    }

    AutoCalculateInArmXClosePitch();

    iInArmYStep=ChangeToFloatNonPcnt((double)AutoCalculateInArmYClosePitch(), LoadForm->YPitch);

    bool bFrontHasIC=false;

    if(iTrayDir==0)
    {
        if(InArmSuck.iPickRow==2)
        {
            if(bSearchLast && iCloseSiteState==0)
            {
                if(MOT[MMTrayY].FullIC())
                {
                    iyStart=0;
                    iRecY=0;
                }
                else
                {
                    if(iRowCT==1)
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                        {
                            if(InArmSuckUse[0][j] && InArmSuck.Item[0][j]!=NULL_IC)
                                bFrontHasIC=true;
                        }

                        if(bFrontHasIC || CosFunction.bLoaderAutoRetry)         //JerryYang 20220901 : ASE-CL要求只有左上角的device吸不到要跳ALARM, 其他的位置吸不到要自動RETRY    //kevin 20181222 add吸嘴第一排有IC 第二排沒有IC 需回到第一排開始吸
                            iyStart=iRecY;                                      //避免未滿盤往下一直吸
                        else
                            iyStart=iRecY+iInArmYStep;                          //ChungHung 20120505 9045WS

                        if(iyStart>=LoadForm->YDivision)                        //KEVIN 20181222 ?? LOAD TRAY 8 PICK UP ERROR
                        {
                            iyStart=0;
                            iRecY=0;
                        }
                    }
                    else
                    {
                        iyStart=0;
                    }
                }
            }
            else
            {
                iyStart=0;
            }
            ixStart=0;
            ixEnd=LoadForm->XDivision;
            ixStep=1;
            iyEnd=LoadForm->YDivision;
            iyStep=1;
        }
        else
        {
            ixStart=0;
            ixEnd=LoadForm->XDivision;
            ixStep=1;
            iyStart=0;
            iyEnd=LoadForm->YDivision;
            iyStep=1;
        }
    }
    else if(iTrayDir==1)
    {
        ixStart=MOT[MMTrayY].Tray.XItem-1;
        ixEnd=-1;
        ixStep=-1;

        iyStart=0;
        iyEnd=MOT[MMTrayY].Tray.YItem;
        iyStep=1;
    }
    else if(iTrayDir==2)
    {
        ixStart=0;
        ixEnd=MOT[MMTrayY].Tray.XItem;
        ixStep=1;

        iyStart=MOT[MMTrayY].Tray.YItem-1;
        iyEnd=-1;
        iyStep=-1;
    }
    else
    {
        ixStart=MOT[MMTrayY].Tray.XItem-1;
        ixEnd=-1;
        ixStep=-1;

        iyStart=MOT[MMTrayY].Tray.YItem-1;
        iyEnd=-1;
        iyStep=-1;
    }

    for(int y=iyStart; y!=iyEnd; y+=iyStep)
    {
        if(iMax!=0)                                                             //如果Tray X排都吸不到IC，Y排跳下一格
            break;
        for(int x=ixStart; x!=ixEnd; x+=ixStep)
        {
            if(MOT[MMTrayY].Tray.Data[x][y]==HAS_IC)
            {
                if(bUseAxExPicker() ||
                   bUseAxxGPicker() ||
                   iCloseSiteModeFor1x4==e1x4CloseAbAc)                         //Steven 20241111 : for 1x4 close 2 site
                {
                    for(int j=0; j<2; j++)
                    {
                        int j2=j*GetJStep();
                        if(iRowCT==2)                                           //ChungHung 20131231 alter AutoYPitch
                        {
                            for(int i=0; i<iRowCT; i++)
                            {
                                bInArmToLoaderUsage[i][j2]=bSourceFlag[i][j2];  //Steven 20110810 : 右下往左上的吸取問題
                                if(InArmSuck.Item[i][j2]!=NULL_IC)
                                    bInArmToLoaderUsage[i][j2]=false;
                            }
                        }
                        else
                        {
                            bInArmToLoaderUsage[iRowCT][j2]=bSourceFlag[iRowCT][j2];
                            if(InArmSuck.Item[iRowCT][j2]!=NULL_IC)
                                bInArmToLoaderUsage[iRowCT][j2]=false;          //Frank 20160420 j*2
                        }
                    }
                }
                else
                {
                    if(iRowCT==2)                                               //ChungHung 20131231 alter AutoYPitch
                    {
                        for(int i=0; i<iRowCT; i++)
                        {
                            for(int j=0; j<MAX_ARM_Col; j++)
                            {
                                bInArmToLoaderUsage[i][j]=bSourceFlag[i][j];
                                if(InArmSuck.Item[i][j]!=NULL_IC)               //判斷吸嘴上是否有IC，有IC時 flag設成false
                                    bInArmToLoaderUsage[i][j]=false;
                            }
                        }
                    }
                    else
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                        {
                            bInArmToLoaderUsage[iRowCT][j]=bSourceFlag[iRowCT][j];
                            if(InArmSuck.Item[iRowCT][j]!=NULL_IC)              //判斷吸嘴上是否有IC，有IC時 flag設成false
                                bInArmToLoaderUsage[iRowCT][j]=false;
                        }
                    }
                }

                if(iRowCT==2)                                                   //ChungHung 20131231 alter AutoYPitch
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(bInArmToLoaderUsage[0][j])
                        {
                            isStart=j;                                          //確認從第幾隻吸嘴開始
                            break;
                        }
                    }
                }
                else
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(bInArmToLoaderUsage[iRowCT][j])
                        {
                            isStart=j;                                          //確認從第幾隻吸嘴開始
                            break;
                        }
                    }
                }

                ct=0;

                if(bUseAxExPicker() ||
                   bUseAxxGPicker() ||
                   iCloseSiteModeFor1x4==e1x4CloseAbAc)                         //Steven 20241111 : for 1x4 close 2 site
                {
                    for(int j=0; j<2; j++)
                    {
                        int j2=j*GetJStep();
                        cx=x+(j-isStart)*iInArmXStep;
                        if(iRowCT==2)                                           //ChungHung 20131231 alter AutoYPitch
                        {
                            if(cx<0)                                            //Steven 20160525 : 避免小於0
                            {
                                bInArmToLoaderUsage[0][j2]=false;
                                bInArmToLoaderUsage[1][j2]=false;
                            }
                            else
                            {
                                if(cx>=MOT[MMTrayY].Tray.XItem)                 //判斷是否大於Tray X Item
                                {
                                    bInArmToLoaderUsage[0][j2]=false;
                                    bInArmToLoaderUsage[1][j2]=false;
                                }

                                if(MOT[MMTrayY].Tray.Data[cx][y]==NULL_IC)      //判斷Tray上是否有IC
                                {
                                    bInArmToLoaderUsage[0][j2]=false;
                                }

                                if(MOT[MMTrayY].Tray.Data[cx][y+iInArmYStep]==NULL_IC)                                  //判斷Tray上是否有IC
                                {
                                    bInArmToLoaderUsage[1][j2]=false;
                                }
                            }

                            if(bInArmToLoaderUsage[0][j2])
                            {
                                ct++;                                           //記數能一次吸幾顆IC
                            }

                            if(bInArmToLoaderUsage[1][j2])
                            {
                                ct++;                                           //記數能一次吸幾顆IC
                            }
                        }
                        else
                        {
                            if(cx<0)                                            //Steven 20160525 : 避免小於0
                            {
                                bInArmToLoaderUsage[iRowCT][j2]=false;
                            }
                            else
                            {
                                if(cx>=MOT[MMTrayY].Tray.XItem)                 //判斷是否大於Tray X Item
                                    bInArmToLoaderUsage[iRowCT][j2]=false;
                                if(MOT[MMTrayY].Tray.Data[cx][y]==NULL_IC)      //判斷Tray上是否有IC
                                    bInArmToLoaderUsage[iRowCT][j2]=false;
                            }

                            if(bInArmToLoaderUsage[iRowCT][j2])
                                ct++;                                           //記數能一次吸幾顆IC
                        }
                    }
                }
                else
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(iRowCT==2)                                           //ChungHung 20131231 alter AutoYPitch
                        {
                            cx=x+(j-isStart)*iInArmXStep;
                            if(cx<0)                                            //Steven 20160525 : 避免小於0
                            {
                                bInArmToLoaderUsage[0][j]=false;
                                bInArmToLoaderUsage[1][j]=false;
                            }
                            else
                            {
                                if(cx>=MOT[MMTrayY].Tray.XItem)                 //判斷是否大於Tray X Item
                                {
                                    bInArmToLoaderUsage[0][j]=false;
                                    bInArmToLoaderUsage[1][j]=false;
                                }

                                if(MOT[MMTrayY].Tray.Data[cx][y]==NULL_IC)      //判斷Tray上是否有IC
                                {
                                    bInArmToLoaderUsage[0][j]=false;
                                }

                                if(MOT[MMTrayY].Tray.Data[cx][y+iInArmYStep]==NULL_IC)                                  //判斷Tray上是否有IC
                                {
                                    bInArmToLoaderUsage[1][j]=false;
                                }
                            }

                            if(bInArmToLoaderUsage[0][j])
                            {
                                ct++;                                           //記數能一次吸幾顆IC
                            }

                            if(bInArmToLoaderUsage[1][j])
                            {
                                ct++;                                           //記數能一次吸幾顆IC
                            }
                        }
                        else
                        {
                            cx=x+(j-isStart)*iInArmXStep;
                            if(cx<0)                                            //Steven 20160525 : 避免小於0
                            {
                                bInArmToLoaderUsage[iRowCT][j]=false;
                            }
                            else
                            {
                                if(cx>=MOT[MMTrayY].Tray.XItem)                 //判斷是否大於Tray X Item
                                    bInArmToLoaderUsage[iRowCT][j]=false;
                                if(MOT[MMTrayY].Tray.Data[cx][y]==NULL_IC)      //判斷Tray上是否有IC
                                    bInArmToLoaderUsage[iRowCT][j]=false;
                            }

                            if(bInArmToLoaderUsage[iRowCT][j])
                                ct++;                                           //記數能一次吸幾顆IC
                        }
                    }
                }

                if(CosFunction.bLoaderAutoRetry &&
                   TrayForm.Loader.Direction==0 &&
                   bAutoRetryFlag==true && ct>0)                                //JerryYang 20220901 : ASE-CL要求只有左上角的device吸不到要跳ALARM, 其他的位置吸不到要自動RETRY
                {
                    iMax=ct;
                    iCol=x;
                    iRow=y;
                    if(iRowCT==2)                                               //ChungHung 20131231 alter AutoYPitch
                    {
                        for(int i=0; i<iRowCT; i++)
                            for(int j=0; j<MAX_ARM_Col; j++)
                                backupFlag[i][j]=bInArmToLoaderUsage[i][j];
                    }
                    else
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                            backupFlag[iRowCT][j]=bInArmToLoaderUsage[iRowCT][j];
                    }
                    x=ixEnd;                                                    //跳出for迴圈
                    break;
                }

                if(ct>iMax)                                                     //確認最多顆位置
                {
                    iMax=ct;
                    iCol=x;
                    iRow=y;
                    if(iRowCT==2)                                               //ChungHung 20131231 alter AutoYPitch
                    {
                        for(int i=0; i<iRowCT; i++)
                            for(int j=0; j<MAX_ARM_Col; j++)
                                backupFlag[i][j]=bInArmToLoaderUsage[i][j];
                    }
                    else
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                            backupFlag[iRowCT][j]=bInArmToLoaderUsage[iRowCT][j];
                    }
                }
            }
        }
    }

    for(int i=0; i<MAX_ARM_Row; i++)
        for(int j=0; j<MAX_ARM_Col; j++)
            bInArmToLoaderUsage[i][j]=backupFlag[i][j];

    if(bSearchLast && iCloseSiteState==0)
    {
        if(InArmSuck.iPickRow==2)
        {
            if(iRowCT==0)
            {
                iRecY=iRow;
            }
        }
    }

    return iMax;
}
//==============================================================================
bool bMyFlag[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
int iMyCol, iMyRow;
bool Find_InArm_Single_FIFO_SiteOrder()                                         //Steven 20170302 (wei) : FIFO MODE
{
    int iSiteNo=0;
    int iTrayDir;
    bool bHasEmptyPicker=false;
    int iSht  =InArmSuck.iWhichSht;
    int iKit32=(InArmSuck.iWhichKit==1)?4:0;

    if(IniConfig.bI37_LockLoaderDirection)
    {
        iTrayDir=IniConfig.iI37_LockLoaderDirection;
    }
    else
    {
        iTrayDir=TrayForm.Loader.Direction;
    }

    ZeroMemory(bMyFlag, sizeof(bMyFlag));
    int iInArmRow=0, iInArmCol=0;                                               //Jimmychiu 20220901 加入初始值
    iMyCol=0;
    iMyRow=0;

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(InArmSuck.Item[i][j]!=NULL_IC)
            {
                bInArmToLoaderUsage[i][j]=false;
            }

            if(bHasEmptyPicker==false       &&
               bInArmToLoaderUsage[i][j]==true  &&
               InArmSuck.Item[i][j]==NULL_IC)
            {
                bHasEmptyPicker=true;
                iSiteNo=Prod.iSiteMap[iSht][i][j+iKit32];
                iInArmRow=i;
                iInArmCol=j;
            }
        }
    }

    if(iSiteNo!=0)
    {
        // ----   左至右,上至下
        //  /
        // --->
        if(iTrayDir==0)
        {
            for(int j=0; j<MOT[MMTrayY].Tray.YItem; j++)
            {
                for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
                {
                    if(iSiteNo==MOT[MMTrayY].Tray.iWhichSite[i][j] &&
                       MOT[MMTrayY].Tray.Data[i][j]==HAS_IC)
                    {
                        iMyCol=i;
                        iMyRow=j;
                        return true;
                    }
                }
            }
        }
        // ----   右至左,上至下
        //  \\
        // <---
        else if(iTrayDir==1)
        {
            for(int j=0; j<MOT[MMTrayY].Tray.YItem; j++)
            {
                for(int i=MOT[MMTrayY].Tray.XItem-1; i>=0; i--)
                {
                    if(iSiteNo==MOT[MMTrayY].Tray.iWhichSite[i][j] &&
                       MOT[MMTrayY].Tray.Data[i][j]==HAS_IC)
                    {
                        iMyCol=i;
                        iMyRow=j;
                        return true;
                    }
                }
            }
        }
        // --->   左至右,下至上
        //  \\
        // ----
        else if(iTrayDir==2)
        {
            for(int j=MOT[MMTrayY].Tray.YItem-1; j>=0; j--)
            {
                for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
                {
                    if(iSiteNo==MOT[MMTrayY].Tray.iWhichSite[i][j] &&
                       MOT[MMTrayY].Tray.Data[i][j]==HAS_IC)
                    {
                        iMyCol=i;
                        iMyRow=j;
                        return true;
                    }
                }
            }
        }
        // <---   右至左,下至上
        //   /
        // ----
        else if(iTrayDir==3)
        {
            for(int j=MOT[MMTrayY].Tray.YItem-1; j>=0; j--)
            {
                for(int i=MOT[MMTrayY].Tray.XItem-1; i>=0; i--)
                {
                    if(iSiteNo==MOT[MMTrayY].Tray.iWhichSite[i][j] &&
                       MOT[MMTrayY].Tray.Data[i][j]==HAS_IC)
                    {
                        iMyCol=i;
                        iMyRow=j;
                        return true;
                    }
                }
            }
        }
        // |   | 上至下, 左至右
        // | / |
        // |   V
        else if(iTrayDir==4)
        {
            for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
            {
                for(int j=0; j<MOT[MMTrayY].Tray.YItem; j++)
                {
                    if(iSiteNo==MOT[MMTrayY].Tray.iWhichSite[i][j] &&
                       MOT[MMTrayY].Tray.Data[i][j]==HAS_IC)
                    {
                        iMyCol=i;
                        iMyRow=j;
                        return true;
                    }
                }
            }
        }
        // |   ^ 下至上, 左至右
        // | \\|
        // |   |
        else if(iTrayDir==5)
        {
            for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
            {
                for(int j=MOT[MMTrayY].Tray.YItem-1; j>=0; j--)
                {
                    if(iSiteNo==MOT[MMTrayY].Tray.iWhichSite[i][j] &&
                       MOT[MMTrayY].Tray.Data[i][j]==HAS_IC)
                    {
                        iMyCol=i;
                        iMyRow=j;
                        return true;
                    }
                }
            }
        }
        // |   | 上至下, 右至左
        // | \\|
        // V   |
        else if(iTrayDir==6)
        {
            for(int i=MOT[MMTrayY].Tray.XItem-1; i>=0; i--)
            {
                for(int j=0; j<MOT[MMTrayY].Tray.YItem; j++)
                {
                    if(iSiteNo==MOT[MMTrayY].Tray.iWhichSite[i][j] &&
                       MOT[MMTrayY].Tray.Data[i][j]==HAS_IC)
                    {
                        iMyCol=i;
                        iMyRow=j;
                        return true;
                    }
                }
            }
        }
        // ^   | 下至上, 右至左
        // | / |
        // |   |
        else if(iTrayDir==7)
        {
            for(int i=MOT[MMTrayY].Tray.XItem-1; i>=0; i--)
            {
                for(int j=MOT[MMTrayY].Tray.YItem-1; j>=0; j--)
                {
                    if(iSiteNo==MOT[MMTrayY].Tray.iWhichSite[i][j] &&
                       MOT[MMTrayY].Tray.Data[i][j]==HAS_IC)
                    {
                        iMyCol=i;
                        iMyRow=j;
                        return true;
                    }
                }
            }
        }

        bHasEmptyPicker=false;
        InArmSuck.SetItemData(iInArmRow, iInArmCol, HAS_NULL_IC);
        bInArmToLoaderUsage[iInArmRow][iInArmCol]=false;

        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(bHasEmptyPicker==false       &&
                   bInArmToLoaderUsage[i][j]==true  &&
                   InArmSuck.Item[i][j]==NULL_IC)
                {
                    bHasEmptyPicker=true;
                    iSiteNo=Prod.iSiteMap[iSht][i][j+iKit32];
                }
            }
        }

        if(iSiteNo!=0)
        {
            if(Find_InArm_Single_FIFO_SiteOrder()==true)
            {
                return true;
            }
        }
    }
    else
    {
        //不知道會不會發生
    }
    return false;
}
//==============================================================================
bool Find_InArm_Single(int iUseSuck, int &iRow, int &iCol)
{
    int XArea=0, YArea=0;
    int iTrayDir=TrayForm.Loader.Direction;

    if((IniConfig.bI37_EnableFIFOMode &&
        IniConfig.bI37_EnableFIFOSiteOrder &&
        LastSet.iRunStartMode==rsmFIFOMode) ||                                  //Steven 20170302 (wei) : FIFO MODE
       (fContact->fShow && iContactMode==CONTACT_DEVICE_MAP_CHECK))             //Steven 20220510 : For QTI SD Device Map Function
    {
        if(Find_InArm_Single_FIFO_SiteOrder()==true)
        {
            iCol=iMyCol;
            iRow=iMyRow;
            return true;
        }
        else
        {
            return false;
        }
    }

    if(IniConfig.bQAMode==true &&
       LastSet.iRunStartMode==rsmQAMode &&
       CosFunction.bQAmodeSupplyTrayDir==true)                                  //jou 2014-10-24 QA mode 增加支援吸Tray方向
        iTrayDir=TestIF_File.iQATrayDirect;

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(InArmSuck.Item[i][j]!=NULL_IC)
            {
                bInArmToLoaderUsage[i][j]=false;
            }
        }
    }

    if(iTrayDir==0)
    {
        XArea=MOT[MMTrayY].Tray.XItem;
        YArea=MOT[MMTrayY].Tray.YItem;

        if(IniConfig.bUseTrayBlockMode &&
           IniConfig.bP06_LoaderUseCarrierTray==true)                           //Frank 20160928 add Subtray Function
        {
            int BX, BY, BlockTotal;
            bool bBlockEmpty=true;
            if((LoadForm->BlockXItem>0) || (LoadForm->BlockYItem>0))
            {
                BX = ChangeToFloatNonPcnt((double)(MOT[MMTrayY].Tray.XItem), (double)(LoadForm->BlockXItem));
                BY = ChangeToFloatNonPcnt((double)(MOT[MMTrayY].Tray.YItem), (double)(LoadForm->BlockYItem));
                BlockTotal = LoadForm->BlockXItem*LoadForm->BlockYItem;
                iInArmBlockIndex =0;
                for(int k=0; k<BlockTotal; k++)
                {
                    XArea=BX+(iInArmBlockIndex%LoadForm->BlockXItem*BX);
                    YArea=BY+(ChangeToFloatNonPcnt((double)(iInArmBlockIndex), (double)(LoadForm->BlockXItem))*BY);

                    for(int y=0; y<YArea; y++)
                    {
                        for(int x=0; x<XArea; x++)
                        {
                            if(MOT[MMTrayY].Tray.Data[x][y]==HAS_IC)
                            {
                                bBlockEmpty=false;
                            }
                        }
                    }

                    if(bBlockEmpty)
                    {
                        iInArmBlockIndex=k+1;
                        XArea=BX+(iInArmBlockIndex%LoadForm->BlockXItem*BX);    //Frank 20160928 add Subtray Function
                        YArea=BY+(ChangeToFloatNonPcnt((double)(iInArmBlockIndex), (double)(LoadForm->BlockXItem))*BY);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else                                                                //Dell 20140409 : Fix WLP
            {
                YArea=MOT[MMTrayY].Tray.YItem;
                XArea=MOT[MMTrayY].Tray.XItem;
            }

            for(int y=0; y<YArea; y++)
            {
                for(int x=0; x<XArea; x++)
                {
                    if(MOT[MMTrayY].Tray.Data[x][y]==HAS_IC)
                    {
                        iCol=x;
                        iRow=y;
                        return true;
                    }
                }
            }
        }
        else
        {
            for(int y=0; y<MOT[MMTrayY].Tray.YItem; y++)
            {
                for(int x=0; x<MOT[MMTrayY].Tray.XItem; x++)
                {
                    if(MOT[MMTrayY].Tray.Data[x][y]==HAS_IC)
                    {
                        iCol=x;
                        iRow=y;
                        return true;
                    }
                }
            }
        }
    }
    else if(iTrayDir==1)
    {
        for(int y=0; y<MOT[MMTrayY].Tray.YItem; y++)
        {
            for(int x=(MOT[MMTrayY].Tray.XItem-1); x>=0; x--)
            {
                if(MOT[MMTrayY].Tray.Data[x][y]==HAS_IC)
                {
                    iCol=x;
                    iRow=y;
                    return true;
                }
            }
        }
    }
    else if(iTrayDir==2)
    {
        for(int y=(MOT[MMTrayY].Tray.YItem-1); y>=0; y--)
        {
            for(int x=0; x<MOT[MMTrayY].Tray.XItem; x++)
            {
                if(MOT[MMTrayY].Tray.Data[x][y]==HAS_IC)
                {
                    iCol=x;
                    iRow=y;
                    return true;
                }
            }
        }
    }
    else if(iTrayDir==3)
    {
        for(int y=(MOT[MMTrayY].Tray.YItem-1); y>=0; y--)
        {
            for(int x=(MOT[MMTrayY].Tray.XItem-1); x>=0; x--)
            {
                if(MOT[MMTrayY].Tray.Data[x][y]==HAS_IC)
                {
                    iCol=x;
                    iRow=y;
                    return true;
                }
            }
        }
    }
    else if(iTrayDir==4)
    {
        for(int x=0; x<MOT[MMTrayY].Tray.XItem; x++)
        {
            for(int y=0; y<MOT[MMTrayY].Tray.YItem; y++)
            {
                if(MOT[MMTrayY].Tray.Data[x][y]==HAS_IC)
                {
                    iCol=x;
                    iRow=y;
                    return true;
                }
            }
        }
    }
    else if(iTrayDir==5)
    {
        for(int x=0; x<MOT[MMTrayY].Tray.XItem; x++)
        {
            for(int y=(MOT[MMTrayY].Tray.YItem-1); y>=0; y--)
            {
                if(MOT[MMTrayY].Tray.Data[x][y]==HAS_IC)
                {
                    iCol=x;
                    iRow=y;
                    return true;
                }
            }
        }
    }
    else if(iTrayDir==6)
    {
        for(int x=(MOT[MMTrayY].Tray.XItem-1); x>=0; x--)
        {
            for(int y=0; y<MOT[MMTrayY].Tray.YItem; y++)
            {
                if(MOT[MMTrayY].Tray.Data[x][y]==HAS_IC)
                {
                    iCol=x;
                    iRow=y;
                    return true;
                }
            }
        }
    }
    else
    {
        for(int x=(MOT[MMTrayY].Tray.XItem-1); x>=0; x--)
        {
            for(int y=(MOT[MMTrayY].Tray.YItem-1); y>=0; y--)
            {
                if(MOT[MMTrayY].Tray.Data[x][y]==HAS_IC)
                {
                    iCol=x;
                    iRow=y;
                    return true;
                }
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void AdjustInArmClosePitchCondition(bool &bCanPick2ICAtOnceTime)
{
    bCanPick2ICAtOnceTime=false;

    fSCKART->CheckInArmNeedVariModeFIX();                                       //Sam 20211118 : 整合 ART InOut Arm 一顆一顆放
    int iResult=AutoCalculateInArmXClosePitch();                                //Steven 20240509 : 移到外面, 避免Fix mode沒執行
    if(ArmSpeed[InArm].bVariModeFIX==false)
    {
        if(iResult==-1)
        {
            bCanPick2ICAtOnceTime=false;
        }
        else
        {
            bCanPick2ICAtOnceTime=true;
        }
    }
    else
    {
        bCanPick2ICAtOnceTime=false;
    }

    if((fContact->fShow && iContactMode==CONTACT_DEVICE_MAP_CHECK) ||           //Steven 20220510 : For QTI SD Device Map Function
       fContact->IsRun2DCheck()==true)                                          //JerryYang 20250220 : 2DID硬體順序檢查功能
    {
        bCanPick2ICAtOnceTime=false;
    }

    if(IniConfig.bI37_EnableFIFOMode && IniConfig.bI37_EnableFIFOSiteOrder && LastSet.iRunStartMode==rsmFIFOMode)       //Steven 20170302 (wei) : FIFO MODE
        bCanPick2ICAtOnceTime=false;

    if(TrayForm.Loader.Direction>=4)
        bCanPick2ICAtOnceTime=false;

    if((IniConfig.bUseTrayBlockMode &&
        IniConfig.bP06_LoaderUseCarrierTray==true) &&                           //Ifor 20161129 add P06 判斷避免動作異常
       (TestIF.iTestMode==_8Site2X4  ||
        TestIF.iTestMode==_16Site4X4 ||                                         //Sam 20190226 : 16Site4X4
        iInArmType==e9045_1x4_1_Ac   ||                                         //Steven 20200720 : 1x4只開site Ac
        TestIF.iTestMode==SingleSite ||
        TestIF.iTestMode==DualSite))                                            //Steven 20140716 Add
    {
        if((LoadForm->BlockNumberX>0) || (LoadForm->BlockNumberY>0))
        {
            bCanPick2ICAtOnceTime=false;
        }
    }

    if(IniConfig.bUseTrayBlockMode &&
       IniConfig.bP06_LoaderUseCarrierTray==true)                               //Frank 20160928 add Subtray Function
    {
        if((LoadForm->BlockXItem>0) || (LoadForm->BlockYItem>0))
        {
            bCanPick2ICAtOnceTime=false;
        }
    }

    if(USE_PICKER_COUNT==ep1Picker)
    {
        bCanPick2ICAtOnceTime=false;
    }
}
// =============================================================================
bool ArmRow1FinishForLoader()
{
    int iSht  =InArmSuck.iWhichSht;
    int iKit32=(InArmSuck.iWhichKit==0)?0:4;

    if(iInArmType==e9045_1x4_8_Hot ||                                           //Steven 20240119 : fixed for 2x4_16
       iInArmType==e9045_1x2_4_Hot ||
       iInArmType==e9045_2x2_8_Hot)
    {
//        iSht32=0;
        iKit32=0;
    }

    for(int j=0; j<4; j++)
    {
        int j2=j+iKit32;
        if(Prod.fInArmSuck4x8[iSht][0][j2]==true)
        {
            if(InArmSuck.Item[0][j]==NULL_IC)
                return false;
        }
    }
    return true;
}
// =============================================================================
bool ArmFinishForLoader()                                                       //check Arm finish and  can go shuttle
{
    int iSht  =InArmSuck.iWhichSht;
    int iKit32=(InArmSuck.iWhichKit==0)?0:4;

    if(iInArmType==e9045_1x4_8_Hot ||                                           //Steven 20240119 : fixed for 2x4_16
       iInArmType==e9045_1x2_4_Hot ||
       iInArmType==e9045_2x2_8_Hot)
    {
        iSht  =0;
        iKit32=0;
    }

    if(iCleanOut &&
       MOT[MMTrayY].HasIC()==false &&
       MOT[MMTrayY_Car].fHasTray==false)
        return true;

    if(LastSet.iRunStartMode==rsmFIFOMode &&
       IniConfig.bI37_EnableFIFOMode &&
       IniConfig.bI37_EnableFIFOSiteOrder)                                      //Jimmychiu 20250715 : 每盤IC吸完後編號重新計數
    {
        if(MOT[MMTrayY].HasIC()==false)
        {
            return true;
        }
    }

    if(USE_PICKER_COUNT==ep1Picker)
    {
        if(InArmSuck.Item[0][0]==NULL_IC)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            int j2=j+iKit32;
            if(Prod.fInArmSuck4x8[iSht][i][j2]==true)
            {
                if(InArmSuck.Item[i][j]==NULL_IC)
                {
                    return false;
                }
            }
        }
    }
    return true;
}
//=============================================================================
bool SearchLoadTrayUpDown_9045(bool &bCanPick2ICAtOnceTime, int &iRow, int &iCol, int &NowRow, bool bNeedDown)
{
    AdjustInArmClosePitchCondition(bCanPick2ICAtOnceTime);

    bool CanMove=false, bNeedPick;
    bool flag_Temp[MAX_ARM_Row][MAX_ARM_Col], bflag=false;                      //Ztex 2024.03.02 Add HT-1032AT Right Side Can't Suck IC Issue
    int isStart=0, _iRow=0, _iCol=0;
    int ArmMaxSuck=4;
    int iMinXStep=(USE_16PICKER_TYPE==1)?-7:-3;                                 //Steven 20230727 : 修正最小X位置 //Ztex 2023.12.06 Add HT-1032
    int iRowCT=0;
    int iSht  =InArmSuck.iWhichSht;
    int iKit32=(InArmSuck.iWhichKit==1)?4:0;
    int iCollimit=0;
    ZeroMemory(bInArmToLoaderUsage, sizeof(bInArmToLoaderUsage));
    ZeroMemory(flag_Temp, sizeof(flag_Temp));                                   //Ztex 2024.03.02 Add HT-1032AT Right Side Can't Suck IC Issue
    //----------------------------------------
    //Set Arm Suck Status
    //----------------------------------------
    DoInArm_9045_SuckerMap();
    if(fContact->IsRun2DCheck())                                                //JerryYang 20260409 : fix 2DID mapping
    {
        iKit32=(InArmSuck.iWhichKit==1)?4:0;
    }

    if(iInArmType==e9045_1x4_8_Hot ||
       iInArmType==e9045_1x2_4_Hot ||
       iInArmType==e9045_2x2_8_Hot)
    {
        iSht  =0;
        iKit32=0;
    }

    if(iInArmType==e9045_1x1_1     ||                                           //Sam 20250902 : 修正一次吸料問題
       iInArmType==e9045_1x4_1_Ac  ||
       iInArmType==e9045_1x2_2_13  ||
       iInArmType==e9045_1x2_2_14  ||
       iInArmType==e9045_1x2_4_Hot ||
       iInArmType==e9045_1x4_2_14  ||
       iInArmType==e9045_1x4_4_13  ||
       iInArmType==e9045_2x1_2_13  ||
       iInArmType==e9045_2x2_2_14  ||
       iInArmType==e9045_2x2_4_12  ||
       iInArmType==e9045_2x2_4_13  ||
       iInArmType==e9045_2x2_4_14  ||
       iInArmType==e9045_2x3_6_14  ||
       iInArmType==e9045_2x4_4_13  ||
       iInArmType==e9045_2x4_4_14)
    {
        iCollimit=0;
    }
    else
    {
        iCollimit=iMinXStep*iInArmXStep;
    }

    while(CanMove==false)
    {
        if(ArmSpeed[InArm].bVariModeFIX==false &&
           InArmSuck.iPickRow==2  &&
           TrayYDirForArmYPitch() &&
           ArmRow1FinishForLoader()==false)                                     //是否一次能吸8顆
        {
            iRowCT=2;
            iInArmOrder=0;                                                      //吸嘴組，第一排
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    bInArmToLoaderUsage[i][j]=Prod.fInArmSuck4x8[iSht][i][j+iKit32];
                    flag_Temp[i][j] =Prod.fInArmSuck4x8[iSht][i][j+iKit32];     //Ztex 2024.03.02 Add HT-1032AT Right Side Can't Suck IC Issue
                }
            }
        }
        else
        {
            if(InArmSuck.iPickRow==1 ||
               ArmRow1FinishForLoader()==false)
            {
                iInArmOrder=0;                                                  //吸嘴組，第一排
                iRowCT=0;
            }
            else
            {
                iInArmOrder=1;                                                  //吸嘴組，第二排
                iRowCT=1;
            }

            if(USE_PICKER_COUNT==ep1Picker)                                     //JerryYang 20250902 : add
            {
                bInArmToLoaderUsage[0][0]=true;
                flag_Temp[0][0]=true;
            }
            else
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    bInArmToLoaderUsage[iRowCT][j]=Prod.fInArmSuck4x8[iSht][iRowCT][j+iKit32];
                    flag_Temp[iRowCT][j]=Prod.fInArmSuck4x8[iSht][iRowCT][j+iKit32];                                    //Ztex 2024.03.02 Add HT-1032AT Right Side Can't Suck IC Issue
                }
            }
        }

        NowRow=iRowCT;
        //----------------------------------------------------------
        //check inArm Close pitch is legal
        if(bCanPick2ICAtOnceTime)
        {
            if(AutoCalculateInArmXClosePitch()==-1)
                bCanPick2ICAtOnceTime=false;
        }
        //----------------------------------------------------------
        //Set Arm Suck whith tray
        //----------------------------------------------------------
        if(bCanPick2ICAtOnceTime==true)
        {
            Find_InArm_PickerMaxUseCountOnTime(iRowCT, isStart, _iRow, _iCol, TestIF.bSearchLastMode);
            for(int i=0; i<InArmSuck.iMaxRow; i++)                              //Ztex 2024.03.02 Add HT-1032AT Right Side Can't Suck IC Issue
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    bflag|=bInArmToLoaderUsage[i][j];
                }
            }

            if(bflag==false)
            {
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                        bInArmToLoaderUsage[i][j]=flag_Temp[i][j];

                bool bsingle=Find_InArm_Single(ArmMaxSuck, _iRow, _iCol);
                isStart=0;
                bCanPick2ICAtOnceTime=false;
                if(bsingle==false)
                {
                    return false;
                }
            }
        }
        else
        {
            bool bsingle=Find_InArm_Single(ArmMaxSuck, _iRow, _iCol);
            isStart=0;
            if(bsingle==false)
            {
                return false;
            }
        }
        //-----------------------------------------------------------

        if(IniConfig.bI37_EnableFIFOMode &&
           IniConfig.bI37_EnableFIFOSiteOrder &&
           LastSet.iRunStartMode==rsmFIFOMode)                                  //Steven 20170302 (wei) : FIFO MODE
        {
            bNeedPick=false;
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    InArmSuckUse[i][j]=bInArmToLoaderUsage[i][j];
                    if(bInArmToLoaderUsage[i][j]==true)
                        bNeedPick=true;
                }
            }

            if(bNeedPick==false)
                return false;
        }
        else
        {
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    InArmSuckUse[i][j]=bInArmToLoaderUsage[i][j];
                }
            }
        }

        //iX===>Tray has IC in x direct isStart==>need suck unit  iInArmXStep===>suck two ic pitch
        iCol=_iCol-(isStart*iInArmXStep);
        iRow=_iRow;

        if(IniConfig.bUseTrayBlockMode &&
           IniConfig.bP06_LoaderUseCarrierTray==true)                           //Ifor 20181011 (Steven) : add P06 功能馬達極限位置
        {
            if((iCol<(iMinXStep*iInArmXStep) ||
                iCol>=(LoadForm->XDivision*LoadForm->BlockXItem)) ||
               (iRow<0 ||
                iRow>=(LoadForm->YDivision*LoadForm->BlockYItem)))
            {
                CanMove=false;
            }
            else
            {
                CanMove=MoveInArmXYToLoader_9045(bCanPick2ICAtOnceTime, iRowCT, iRow, iCol, false, bNeedDown);
            }
        }
        else if((iCol<iCollimit ||                                              //Sam 20250902 : 修正一次吸料問題 //Sam 20250313 : 修正吸料順序錯誤問題
                 iCol>=LoadForm->XDivision) ||                                  //jou 2011-04-15 最左邊最多不能超過(-3*iInArmXStep)
                (iRow<0 ||
                 iRow>=LoadForm->YDivision))                                    //Hung 20110407 : 2x2吸取異常時會跳舞解除
        {
            CanMove=false;
        }
        else
        {
            CanMove=MoveInArmXYToLoader_9045(bCanPick2ICAtOnceTime, iRowCT, iRow, iCol, false, bNeedDown);
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
//------------------------------------------------------------------------------
bool SearchAndMoveInArmXYToLoad_9045()                                          //Steven 20240427 : 整合Loader動作
{
    int  iCol=0, iRow=0, iSelRow=0;
    bool bCanPick2ICAtOnceTime=false;
    bool bNeedDown=true;

    if(MOT[MMTrayY].fHasTray==false ||
       MOT[MMTrayY].HasIC()==false)
    {
        bNeedDown=false;
    }

    if(SearchLoadTrayUpDown_9045(bCanPick2ICAtOnceTime, iRow, iCol, iSelRow, bNeedDown)==false)
        return false;

    if(bLoadAutoTeachInarm)                                                     //kevin 20210305 add inarm Auto teach move X Y PITCH Z 軸不動
        return MoveInArmXYToLoader_9045(bCanPick2ICAtOnceTime, iSelRow, iRow, iCol, true, bNeedDown);
    else if(bLoadInarmAutoHigh)                                                 //kevin 20170929 (wei) load initial load 一盤 inarm 校正高度 Z軸先不要下去吸
        return MoveInArmXYToLoader_9045(bCanPick2ICAtOnceTime, iSelRow, iRow, iCol, false, bNeedDown);
    else
        return MoveInArmXYToLoader_9045(bCanPick2ICAtOnceTime, iSelRow, iRow, iCol, true, bNeedDown);
}
//------------------------------------------------------------------------------
bool MoveInArmZToLoaderPick(int Step)                                           //Steven 20160414 : 簡化In Arm程式碼
{
    bool flag[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
    bool bResult=true;
    int iSuckR=0, iSuckC=0;

    if(TestIF.iTestMode==SingleSite || iInArmType==e9045_1x4_1_Ac)              //Steven 20200720 : 1x4只開site Ac
    {
        if(Prod.bSingleUseOtherSuck ||
           Prod.bSingleInArmUseOtherSuck)                                       //wei 20220823 Single Site使用C吸嘴判斷
        {
            iSuckC=1;
        }
        bResult=MOT[InArmZIndex[iSuckR][iSuckC]].MotorMove2SpeedForPicker(Prod.ZInArm_Tray_Pick[iSuckR][iSuckC]+Step*ArmSpeed[InArm].dRetryDown, &ArmSpeed[InArm], true);  //JerryYang 20190729 二段速功能可選擇only at loader   //Steven 20140217 : 兩段速移動
    }
    else
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(InArmSuckUse[i][j])
                {
                    flag[i][j]=MOT[InArmZIndex[i][j]].MotorMove2SpeedForPicker(Prod.ZInArm_Tray_Pick[i][j]+Step*ArmSpeed[InArm].dRetryDown, &ArmSpeed[InArm], true);  //JerryYang 20190729 二段速功能可選擇only at loader  //Steven 20140217 : 兩段速移動
                }
                else
                {
                    flag[i][j]=true;
                }
            }
        }

        for(int i=0; i<InArmSuck.iMaxRow; i++)
            for(int j=0; j<InArmSuck.iMaxCol; j++)
                if(flag[i][j]==false)
                    return bResult=false;
    }

    if(bResult)                                                                 //Steven 20161223 (jou) : 簡化程式碼
    {
        Cylinder[C_TrayY_Fixer].On();
        Cylinder[C_LoaderEdgePush].On();
        Cylinder[C_LoaderUpPress].On();                                         //JerryYang 20181120 (Steven) : (Steven) : 獨立控制loader壓tray
        InArmSuck.ResetAll();                                                   //Steven 20160323 : 避免未開啟真空
        InArmSuckReset();                                                       //wei 20150318 suck動作重置
    }

    return bResult;
}
//------------------------------------------------------------------------------
int ProcessMES0101InArmPickLoaderError(bool bHasDuplicateErr, AnsiString ErrPart)
{                                                                               //Steven 20141121 : 整合InArm吸取Loader異常Alarm
    int ret=0;                                                                  //Jimmychiu 20220901 加入初始值
    int iX, iY;
    bool bASECL_PickupErrNeedAlm=false;
    AnsiString str="";
    if(IniConfig.bE56LoaderRetryAtSamePosition==false)                          //Steven 20170828 (wei) : Loader吸取異常時,要在同一個位置作Retry
        bInArmPickErrFromLoader=true;                                           //Steven 20170828 (wei) : 確認是否有在Loader吸料異常

    if(CosFunction.bLoaderAutoRetry)                                            //JerryYang 20220901 : ASE-CL要求只有左上角的device吸不到要跳ALARM, 其他的位置吸不到要自動RETRY
    {
        {
            for(int y=0; y<MOT[MMTrayY].Tray.YItem; y++)                        //tray盤最左上角的
            {
                for(int x=0; x<MOT[MMTrayY].Tray.XItem; x++)
                {
                    if(MOT[MMTrayY].Tray.Data[x][y]==HAS_IC)
                    {
                        iX=x;
                        iY=y;
                        y=MOT[MMTrayY].Tray.YItem;                              //找到最左上角的就跳出迴圈
                        break;
                    }
                }
            }

            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(InArmSuck.Suck[i][j].Error==true)
                    {
                        if(iX==iXPosition[j] && iY==iYPosition+iLoadPitchStepY*i)
                        {
                            bASECL_PickupErrNeedAlm=true;
                        }
                    }
                }
            }
        }
    }

    if(bCanRunSCKART==true          &&                                          //Steven 20170315 (wei) : For SCK ART Auto Skip
       TestIF_File.bSCKART_AutoSkip &&
       LastSet.iSCKARTInputCT==fSCKART->iInputCount)                            //Steven 20170720 (wei) : Add
    {
        if(iContinueAutoSkipAutoTrayEnd<TestIF_File.iSCKART_AutoSkipCount-1)    //Sam 20191209 : ART Fix issue
        {
            ret=K_SKIP;
            iContinueAutoSkipAutoTrayEnd++;
            str.sprintf("K_SKIP and iContinueAutoSkipAutoTrayEnd = %d", iContinueAutoSkipAutoTrayEnd);
            RecordProcess(str, "ProcessMES0101InArmPickLoaderError_1");         //Steven 20220608 : 紀錄MES0101的處理動作
        }
        else
        {
            ret=K_TRAY_END;
            iContinueAutoSkipAutoTrayEnd=0;
            for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)                        //Steven 20170706 (wei) : Add for ATK ART
            {
                for(int j=0; j<MOT[MMTrayY].Tray.YItem; j++)
                {
                    if(MOT[MMTrayY].Tray.Data[i][j]==HAS_IC)
                    {
                        MOT[MMTrayY].SetTrayBufferSingleData(i, j, HAS_IC);     //Steven 20170712 (Wei) : HAS_SKIP_IC --> HAS_IC
                    }
                }
            }
            str.sprintf("K_TRAY_END and iContinueAutoSkipAutoTrayEnd=0");
            RecordProcess(str, "ProcessMES0101InArmPickLoaderError_2");         //Steven 20220608 : 紀錄MES0101的處理動作
            fMain->Start("ProcessMES0101InArmPickLoaderError 1");
        }
    }
    else if(CUSTOMER_CODE!=CC_ASE_KaohSiung &&
            CosFunction.bUseSCKART==false   &&
            USE_AUTO_RETEST==eartInstall    &&
            (IniConfig.bA10_AutoReTest || bAutoReTest_ART) &&
            CosFunction.bContinueAutoSkipAutoTrayEnd &&
            LastSet.bLoaderTrayCount_ART)                                       //ChungHung 20141014 add for KYEC AutoRetest
    {                                                                           //kevin 20150707
        if(iContinueAutoSkipAutoTrayEnd<=(TrayForm.bEnableAMR?3:5))             //==> Eastsun 20260512 F7-T1 (a-side <=3 wrap)
        {
            ret=K_SKIP;
            iContinueAutoSkipAutoTrayEnd++;
        }
        else
        {
            ret=K_TRAY_END;
            fMain->Start("ProcessMES0101InArmPickLoaderError 2");
        }

        if(TrayForm.bEnableAMR)                                                 //==> Eastsun 20260512 : Phase4-F7T2a KYEC AMR bLoaderCocverRemove (a-side ainarm2.cpp L5156-L5157)
            bLoaderCocverRemove=true;

        if(CUSTOMER_CODE==CC_KYEC_LEE &&                                        //Frank 20160919 add
           !TrayForm.bEnableAMR)                                                //Eastsun 20260512 F7-T2b 整合 (a-side ainarm2.cpp L5160 add !bEnableAMR guard)
        {
            TrayForm.iManualRemoveLoader=2;
        }
    }
    else if(EnableTraymapCheckFunction(0) && EnableTraymapCheckFunction(1))     //wei 20170302 (Steven) Tray map data error
    {
        ret=K_SKIP;
        str.sprintf("K_SKIP by EnableTraymapCheckFunction");
        RecordProcess(str, "ProcessMES0101InArmPickLoaderError_3");             //Steven 20220608 : 紀錄MES0101的處理動作
    }
    else if(ArmSpeed[InArm].bAutoSKIP==true && IniConfig.bRecordSkipPosition && bFirstRecordLoaderData==true)
    {
        ret=K_SKIP;
        bAutoSkipFlag=true;                                                     //jou 20150320   auto skip 次數時間導致Hang up
        str.sprintf("K_SKIP by bAutoSkipFlag");
        RecordProcess(str, "ProcessMES0101InArmPickLoaderError_4");             //Steven 20220608 : 紀錄MES0101的處理動作
    }
    else if(CosFunction.bLoaderAutoRetry &&
            bASECL_PickupErrNeedAlm==false &&
            TrayForm.Loader.Direction==0)                                       //JerryYang 20220901 : ASE-CL要求只有左上角的device吸不到要跳ALARM, 其他的位置吸不到要自動RETRY
    {
        ret=K_RETRY;
        bAutoRetryFlag=true;                                                    //JerryYang 20220901 : ASE-CL pick up error時吸取邏輯要改找吸最左上角的
        MyDBIProcessNew("Motion", "WAR2211", "In arm pick up error at loader(Auto Retry)", ErrPart);                    //JerryYang 20200203 pick up error auto skip
    }
    else if(IniConfig.bP57LoaderAutoCleanOutByInputCT &&                        //Sam 20250605 : Loader Count AutoCleanOut
            LastSet.iP57_InputCT!=0)
    {
        if(iContinueAutoSkipAutoTrayEnd<5-1)                                    //Sam 20191209 : ART Fix issue
        {
            ret=K_SKIP;
            iContinueAutoSkipAutoTrayEnd++;
            str.sprintf("K_SKIP and iContinueAutoSkipAutoTrayEnd = %d", iContinueAutoSkipAutoTrayEnd);
            RecordProcess(str, "ProcessMES0101InArmPickLoaderError_5");
        }
        else
        {
            ret=K_TRAY_END;
            iContinueAutoSkipAutoTrayEnd=0;
            for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
            {
                for(int j=0; j<MOT[MMTrayY].Tray.YItem; j++)
                {
                    if(MOT[MMTrayY].Tray.Data[i][j]==HAS_IC)
                    {
                        MOT[MMTrayY].SetTrayBufferSingleData(i, j, HAS_IC);
                    }
                }
            }
            str.sprintf("K_TRAY_END and iContinueAutoSkipAutoTrayEnd=0");
            RecordProcess(str, "ProcessMES0101InArmPickLoaderError_6");
            fMain->Start("ProcessMES0101InArmPickLoaderError 3");
        }
    }
    else
    {
        if(CUSTOMER_CODE!=CC_ASE_KaohSiung &&
           CosFunction.bUseSCKART==false    &&                                  //Steven 20170712 (Wei) : Fixed for SCK_ART error
           USE_AUTO_RETEST==eartInstall &&
           (IniConfig.bA10_AutoReTest || bAutoReTest_ART) &&
           CosFunction.bContinueAutoSkipAutoTrayEnd &&
           LastSet.bLoaderTrayCount_ART)                                        //ChungHung 20141014 add for KYEC AutoRetest
        {
            ret=K_TRAY_END;
            iContinueAutoSkipAutoTrayEnd=0;
            str.sprintf("K_TRAY_END by iContinueAutoSkipAutoTrayEnd=0");
            RecordProcess(str, "ProcessMES0101InArmPickLoaderError_5");         //Steven 20220608 : 紀錄MES0101的處理動作
            fMain->Start("ProcessMES0101InArmPickLoaderError 4");
        }
        else if(IniConfig.bE86_InArmPickErrOnLoaderOnlyCanSKIP)                 //JerryYang 20250120 : add
        {
            ret=ShowErrorMessage("MES0101", K_TRAY_END|K_SKIP, MInArmX, bHasDuplicateErr, ErrPart);                     //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home start
        }
        else if(IniConfig.bInOutArmCanPushHome)
        {
            ret=ShowErrorMessage("MES0101", K_RETRY|K_TRAY_END|K_SKIP|K_HOME, MInArmX, bHasDuplicateErr, ErrPart);      //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home start
        }
        else if(IniConfig.bE49_LoaderOnlyRetryAndCleanOut)                      //Steven 20141105 : Loader吸取異常只能Retry與CleanOut
        {
            ret=ShowErrorMessage("MES0101", K_RETRY|K_CLEAN_OUT, MInArmX, bHasDuplicateErr, ErrPart);
        }
        else
        {
            if(CosFunction.bMES0101CanCleanOut && TrayForm.bAutoFeed==false)    //Steven 20120830 : Loader吸取異常可以按CleanOut
                ret=ShowErrorMessage("MES0101", K_RETRY|K_TRAY_END|K_SKIP|K_CLEAN_OUT, MInArmX, bHasDuplicateErr, ErrPart);
            else
                ret=ShowErrorMessage("MES0101", K_RETRY|K_TRAY_END|K_SKIP, MInArmX, bHasDuplicateErr, ErrPart);         //Steven 20100508
        }
        bFirstRecordLoaderData=true;                                            //Steven 20150429 : Fixed for Auto Skip

        if(ret==K_TRAY_END || ret==K_SKIP || ret==K_CLEAN_OUT ||
           (ret==K_RETRY && CUSTOMER_CODE==CC_PTI))                             //Sam 20231129 : 力成 Retry 也要搬到 Color
        {
            if(IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray==true ||      //Frank 20150626 : for矽格 Loader有Skip要到Empty的位置做檢查
               IniConfig.bP24SkipEventNeedRemoveColorTrayForIDT==true)          //Sam 20220530 : 當 Loader 發生 Skip/Edit 時，此盤做完後搬到 Empty 軌道後，會收盤起來並報警提示人員收盤(連兩盤)
            {
                if(CosFunction.bSpecialP24)                                     //Sam 20230320 : 整合 P24 北興版為獨立功能碼
                {
                    MOT[MMTrayY].Tray.iNeedManualRemoved=1;                     //to color alarm
                }
                else
                {
                    if(TrayForm.LoaderToEmptyColor[iRunStartMode]==0)
                         MOT[MMTrayY].Tray.iNeedManualRemoved=0;                //to empty alarm
                    else
                        MOT[MMTrayY].Tray.iNeedManualRemoved=1;                 //to color alarm
                }

                iManualRemoveTrayCnt=2;                                         //Sam 20220530 : 當 Loader 發生 Skip/Edit 時，此盤做完後搬到 Empty 軌道後，會收盤起來並報警提示人員收盤(連兩盤)
            }
            else if(IniConfig.bP39LoaderHasSkipPlaceToEmpty)                    //Sam 20230220 : 移除客戶碼                     //KaiChen 20201125 ： 矽格湖口，Loader 有 Skip 強制放Loader
            {
                MOT[MMTrayY].Tray.bMustToEmpty=true;                            //Sam 20230317 : 修正 P24 功能
            }
            else if(CosFunction.bOneByOneWhenPickErrAtLoader &&
                    IniConfig.bE78OneByOneWhenPickErrAtLoader &&
                    ret==K_SKIP)                                                //Jimmychiu 20250924 : Suck one by one when a pickup error occurs at the loader.
            {
                IniConfig.bBackUpInArmMode=ArmSpeed[InArm].bVariModeFIX;
                ArmSpeed_File[InArm].bVariModeFIX=true;
                bNeedOneByOnePickInArm=true;
            }
        }
    }
    bPickFromLoader=false;                                                      //Ifor 20180322 : add 避免發生異常讓位旗標為true造成Hang up
    return ret;
}
//------------------------------------------------------------------------------
int CheckLoaderHasTray(bool bAlarm, int iErrorCount, bool bTrayDuplicateErr)    //Steven 20161223 (jou) : 確認LoaderSensor統一成Function
{
    if(bAlarm==false && bCheckInShuttlePosAndNOIC())                            //KenHsieh 20230614 : Inarm pick IC from tray需待SH到位且為可取放料狀態
        return 3;

    int  iResult=0;                                                             //Steven 20170105 (Wei) : 修改為int
    bool bHasError=false;
    bool flag1=Sen[SnLoaderSureTray].IsOff();
    bool flag2=false;
    #ifndef SOFT_SIMULTE
    if(INSTALL_OCR_YMot==eocrYMotInstal)                                        //Frank 20250214 add
        flag2=Sen[SnLoaderFixCyPush].IsOn();
    else
        flag2=(Sen[SnLoaderFixCyPush].IsOn() ||
               Cylinder[C_TrayY_Fixer].OffSensor());                            //wei 20241011 新增後勾氣缸off sensor,避免氣缸沒作動
    #else
        flag2=false;
    #endif
    bool flag3=Sen[SnLoaderEdgePush].IsOn();

    if(flag1==true || flag2==true || flag3==true)
        bHasError=true;

    if(LastSet.iRealDummy!=DUMMY && bHasError)
    {
        if(bAlarm && iErrorCount>=3)
        {
            if(flag1)
                ShowErrorMessage("JAM0929", K_RETRY, MMTrayY, bTrayDuplicateErr, __FUNC__);
            else if(flag2)
                ShowErrorMessage("JAM0903", K_RETRY, MMTrayY, bTrayDuplicateErr, __FUNC__);
            else if(flag3)
                ShowErrorMessage("JAM0902", K_RETRY, MMTrayY, bTrayDuplicateErr, __FUNC__);
            else
                ShowErrorMessage("JAM0909", K_RETRY, MMTrayY, bTrayDuplicateErr, __FUNC__);
            iResult=2;
        }
        else
        {
            iResult=1;
        }
    }
    return iResult;
}
//------------------------------------------------------------------------------
//Steven 20161223 (wei) : For SCK ART loading count
//把多出來的IC放回去Loader,然後Alarm,結束後要再檢查一次
//------------------------------------------------------------------------------
int iProcessSCKARTLoadingCountTask;
bool ProcessSCKARTLoadingCount(bool bReset=false)
{
    AnsiString ErrPart;
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
            if(InArmSuck.Suck[iLoadPickX][iLoadPickY].Destroy())
            {
                InArmSuckUse[iLoadPickX][iLoadPickY]=true;
                Task=100;
            }
            break;
        case 100:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=200;
            }
            break;
        case 200:
            ErrPart=InArmSuck.Suck[iLoadPickX][iLoadPickY].sName;
            ShowErrorMessage("WAR0120", K_RETRY, MInArmX, false, ErrPart);
            bResult=true;
            bPickFromLoader=false;                                              //Steven 20171226 (Wei) : 確認Loader吸取完成
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool DoAutoSkipCheck()                                                          //kevin 20170822 auto tray end 有吸到IC 換TRAY 要ALARM
{                                                                               //Ifor 20180418 : add Auto Site Mapping 補料回Hotplate必須滿料避免Hang up
    if(bRunAutoSiteMapping==true)                                               //Ifor 20180518 : add 簡化Site Mapping 旗標
    {
        return false;
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung && ArmSpeed[InArm].bAutoSKIP==true)      //kevin 20170822 (wei) add
    {
        if(sAutoTrayendEndPos!="")
        {
            sAutoTrayendEndPosBuffer.sprintf("Auto Tray Skip End:%s",sAutoTrayendEndPos);                               //kevin 20170824 回應 ase close Site
            SendDataToASE(sAutoTrayendEndPosBuffer);                            //kevin 20170824 回應 ase close Site
            sAutoTrayendEndPos="";
            sAutoTrayendEndPosBuffer="";
        }

        if(bAutoTrayEndHasIC)                                                   //kevin 20170822 auto tray end 有吸到IC 換TRAY 要ALARM
            ShowErrorMessage("WAR09101", K_TRAY_END, MInArmX);

        if((LastSet.iTemperature==Tempture_AmbientHot ||
            LastSet.iTemperature==Tempture_Ambient) &&
           InArmSuck.HasIC())
        {
            return true;
        }
    }
//    else if(IniConfig.bP17InArmFullPickFromLoader==false && InArmSuck.HasIC())  //Steven 20101221 :讓每盤都是滿的
//    {
//        return true;
//    }
    else if(IniConfig.bI37_EnableFIFOMode &&                                    //Steven 20160303 : FIFO Mode
            LastSet.iRunStartMode==rsmFIFOMode &&
            InArmSuck.HasIC())
    {
        return true;
    }
    else if(iCleanOut==1 ||                                                     //Steven 20241218 : avoid the tray not goes down yet
            bCleanoutStart==true)                                               //Steven 20250113 : 準備clean out, 但是被one cycle插隊會hang up
    {
        return true;
    }

    return false;
}
//==============================================================================
bool ShuttleIsFull_9045(int iSht)                                               //Steven 20240322 : 檢查shuttle是否已擺滿
{
    if(bPlaceShuttle==false && bPickFromHotplate==false)
    {
        if(iSht==0)
        {
            if(FLCarryKit.UseSiteFullIC())                                      //Steven 20241119 : Fixed
            {
                return true;
            }
            else
            {
                for(int i=0; i<FLCarryKit.iShtRow; i++)
                {
                    for(int j=0; j<FLCarryKit.iShtCol; j++)
                    {
                        if(FLCarryKit.Item[i][j]==NULL_IC)
                            return false;
                    }
                }
            }
        }
        else
        {
            if(BLCarryKit.UseSiteFullIC())
            {
                return true;
            }
            else
            {
                for(int i=0; i<BLCarryKit.iShtRow; i++)
                {
                    for(int j=0; j<BLCarryKit.iShtCol; j++)
                    {
                        if(BLCarryKit.Item[i][j]==NULL_IC)
                            return false;
                    }
                }
            }
        }
    }
    return true;
}
//==============================================================================
int CheckOneCycleAction(int iTask)                                              //Steven 20240326 : 判斷one cycle的時候要不要繼續放料
{
    int Task=iTask;
    int iP=0, iR=0, iC=0, iSht=-1, iKit=-1;

    if(IniConfig.bP17InArmFullPickFromLoader &&                                 //Steven 20181018 : 修正常溫也要補滿Site
       LastSet.iRunStartMode!=rsmQAMode)                                        //Steven 20190410 : Fixed for QA mode hang up
    {
        if(LastSet.iTemperature==Tempture_Hot)
        {
            if(MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC())
            {
                if(PickFromHPList->GetHPFirstTeamPlate(iP, iR, iC, iSht, iKit))                                         //JerryYang 20250408 : fix Hot plate偶發one cycle hang up
                {
                    if(iSht==0 && InArmSideAllClose(0)==false)
                    {
                        if(InSHT1InLF())
                        {
                            if(iInArmType==e9045_1x3_2_14 ||
                               iInArmType==e9045_1x4_2_14 ||
                               iInArmType==e9045_2x3_6_14 ||
                               iInArmType==e9045_2x4_4_13 ||
                               iInArmType==e9045_2x4_4_14 ||
                               iInArmType==e9045_2x5_8 ||
                               iInArmType==e9045_2x6_8 ||
                               iInArmType==e9045_2x8_32 ||
                               iInArmType==e9045_2x8_8 ||
                               USE_PICKER_COUNT==ep1Picker)
                            {
                                if(FLCarryKit.UseSiteHasIC()==true &&           //如果蝦頭一有料而且
                                   ShuttleIsFull_9045(0)==false)                //    蝦頭一還沒擺滿
                                {
                                    Task=500;
                                }
                            }
                        }
                    }
                    else if(iSht==1)
                    {
                        if(InSHT2InLF())
                        {
                            if(InArmSideAllClose(0)==false)                     //如果Arm 1有開site, 要做完Arm 2的料
                            {
                                if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
                                   (bShuttle2MoveToRight ||                     //Steven 20260121 : dont set shuttle when index jam
                                    bShuttle2MoveToLeft  ||                     //Steven 20220620 : 避免index在Kit1吸取異常, In arm偷放料
                                    bShuttle2HasPickErr))                       //Steven 20230116 : 避免In arm 偷放料
                                {
                                }
                                else if(ShuttleIsFull_9045(1)==false ||         //如果蝦頭二還沒擺滿 或者
                                        BLCarryKit.UseSiteNoIC())               //如果蝦頭二沒料
                                {
                                    MOT[MInShuttle2].fCanMoveL=false;           //把蝦頭鎖定, 避免index position error
                                }
                                Task=500;
                            }
                            else
                            {
                                if(iInArmType==e9045_1x3_2_14 ||
                                   iInArmType==e9045_1x4_2_14 ||
                                   iInArmType==e9045_2x3_6_14 ||
                                   iInArmType==e9045_2x4_4_13 ||
                                   iInArmType==e9045_2x4_4_14 ||
                                   iInArmType==e9045_2x5_8 ||
                                   iInArmType==e9045_2x6_8 ||
                                   iInArmType==e9045_2x8_32 ||
                                   iInArmType==e9045_2x8_8)
                                {
                                    if(iKit==1)
                                    {
                                        Task=500;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                SetShuttleToHasNullICWhenCleanOut();                            //Steven 20230309 : 修正Clean時, 都沒IC造成的Hang Up
            }
        }
        else
        {
            if(MOT[MMTrayY].HasIC())
            {
                if(InArmSuck.iWhichSht==0)
                {
                    if(InArmSideAllClose(0)==true)
                    {
                        AdjustShuttlePlaceOrder(1);
                    }
                    else if(InSHT1InLF())
                    {
                        if(FLCarryKit.UseSiteHasIC()==true &&                   //如果蝦頭一有料而且
                           ShuttleIsFull_9045(0)==false)                        //    蝦頭一還沒擺滿
                        {
                            Task=100;
                        }
                    }
                }
                else
                {
                    if(InArmSideAllClose(1)==true)
                    {
                        AdjustShuttlePlaceOrder(0);
                    }
                    else if(InSHT2InLF())
                    {
                        if(InArmSideAllClose(0)==false)                         //如果Arm 1有開site, 要做完Arm 2的料
                        {
                            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
                               (bShuttle2MoveToRight ||                         //Steven 20260121 : dont set shuttle when index jam
                                bShuttle2MoveToLeft  ||                         //Steven 20220620 : 避免index在Kit1吸取異常, In arm偷放料
                                bShuttle2HasPickErr))                           //Steven 20230116 : 避免In arm 偷放料
                            {
                            }
                            else if(ShuttleIsFull_9045(1)==false ||             //如果蝦頭二還沒擺滿 或者
                                    BLCarryKit.UseSiteNoIC())                   //如果蝦頭二沒料
                            {
                                MOT[MInShuttle2].fCanMoveL=false;               //把蝦頭鎖定, 避免index position error
                            }
                            Task=100;
                        }
                        else
                        {
                            if(BLCarryKit.UseSiteHasIC()==true &&               //如果蝦頭二有料而且
                               ShuttleIsFull_9045(1)==false)                    //    蝦頭二還沒擺滿
                            {
                                Task=100;
                            }
                        }
                    }
                }
            }
            else
            {
                SetShuttleToHasNullICWhenCleanOut();                            //Steven 20230309 : 修正Clean時, 都沒IC造成的Hang Up
            }
        }

        if(Task!=500 && Task!=100)
            SetShuttleToNullICWhenCleanOut();                                   //Steven 20240326 : 修正Clean時, 都沒IC造成的Hang Up
    }

    if(Task==iTask &&
       bPickFromLoader==true ||
       bPickFromHotplate==true ||
       InArmSuck.HasIC()==true)
    {
        Task=50;
    }
    return Task;
}
//------------------------------------------------------------------------------
bool bNeedOneCycle()                                                            //Steven 20131029 : 解決Index Position Error
{
    if(IniConfig.bQAMode==true && LastSet.iRunStartMode==rsmQAMode)             //ChungHung 20141023 fix bNeedOneCycle make QA mode Error running
    {
        if(iQAModeLoaderCT>=Prod.iQAModeCount && iOneCycle==1)
            return true;
    }

    if(iOneCycle==1)
    {
        int iRet=CheckOneCycleAction(0);
        if(iRet!=0)
        {
            return false;
        }
        else if(LastSet.iTemperature!=Tempture_Hot &&                           //JerryYang 20251020 : fix 32 site one cycle hang up
                (TestIF.iTestMode==QualSite2X2N || TestIF.iTestMode==_16Site4X4 ||
                 TestIF.iTestMode==_32Site4X8N  || TestIF.iTestMode==_6Site2X3N) &&                                     //Steven 20220425 : 2X3NN Mode
                ((FLCarryKit.HasRealIC() || (FTestSuck.HasIC() && FTestSuck.AlreadyTest()==false)) &&                   //Shuttle 1或Arm 1有IC
                (BLCarryKit.NoIC() && BTestSuck.NoIC())))                       //Shuttle 2或Arm 2沒IC
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    return false;
}
//==============================================================================
TQPF_Timer DoArmPickFromLoadStage_Delay;                                        //Steven 20160219 : 重新整理HTimer命名
bool DoInArmPickFromLoadStage_9045()
{
    static bool bHasLog=false;
    if(MOT[MTrayX].ReadEncoderPos()<Prod.iXTraySafty)                           //JimmyChiu 20250722 : 確保In arm移動到loader時，Tray arm已離開
    {
        MoveInArm2XYToWait();
//        if(bHasLog==false)
//        {
//            AnsiString sErr=AnsiString().sprintf("Tray arm not Safe pos. %s , Trayarm encoder=%d",__FUNC__,MOT[MTrayX].ReadEncoderPos());
//            RecordProcess(sErr);
//            bHasLog=true;
//        }
        return false;
    }

    static int iRetryCT=0, iBuf=0;
    static bool bTrayDuplicateErr=false;

    IN_ARM_LOADER:

    QueueTaskList[13].CheckTaskChange();                                        //Steven 20200821 : 使用Goto也要記錄Task變化
    int ret;
    int iFlag;
    int &Task=iPickFromLoadStageTask;
    int iYpos=0;
    int iSht=InArmSuck.iWhichSht;
    int iKit=InArmSuck.iWhichKit;

    bool flag1, bTrayEndCleanOut, bPickFallDown;
    bool bHasDuplicateErr=false, bLoadpickerrorMoveWait=false;                  //kevin 20220708            //Steven 20100104 : 整合AlarmCodde
    bool bflag=false;
    AnsiString ErrPart="", ErrTray="", sbuffer="";                              //kevin 20220521 tray pos   //Steven 20100104 : 整合AlarmCodde
    bHasLog=false;

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

            if(MoveInArmZToPlateSafeAndCheckLoaderTray(Task))                   //Ifor 20221227 add:新增Loader 取料後判斷是否吸到Loader Tray
            {
                if(bNeedOneCycle())                                             //Steven 20131029 : 解決Index Position Error
                {
                    if(MOT[MMTrayY].Tray.HasIC()==false &&                      //JerryYang 20250718 : fix 取TRAY時input arm沒有讓位
                       MOT[MMTrayY].fHasTray &&
                       (TrayForm.iManualRemoveLoader!=0 &&                      //Steven 20130819 : 加入開門檢查的功能  (==true --> !=0)
                        bLoaderHasSkip==true) ||                                //jou 2010-12-20 修正需要手動拿Loader Tray時,In Arm Y移至shuttle 2上,方便OP拿Tray
                        bNeedManualRemoveTray==true)                            //Ifor 20200331 : add Edit Loader Tray Need Manual Remove Tray
                    {
                        MoveInArm2XYToShuttle2Wait();
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

                if(DoPickLoaderOK(iSht, iKit))
                {
                    return true;
                }

                MyInArmAtShuttleTimer.LatchCycleTime(true);                     //Steven 20151201

                if(tRotate.ActiveRotate==1 && iRotate_Type==eInOutArm1Motor)    //add One sucker with rotate
                {
                    MoveInRotateToDegreeAtSameTime(0, true);
                    Task=9;
                }
                else
                {
                    Task=10;
                }

                goto IN_ARM_LOADER;
            }
            break;
        case 5:                                                                 //kevin 20171103 (wei) Inarm Auto High //kevin 20170929 test4 不讓z軸往下
            if(AutoTeachLoadTrayZ(false, InArm, iInArmZTeachTask))
            {
                bLoadInarmAutoHigh=false;
                Task=1;
            }
            break;
        case 9:
            if(MoveInRotateToDegreeAtSameTime(0))                               //add One sucker with rotate
            {
                Task=10;
            }
            break;
        case 10:
            if(MOT[MMTrayY].HasIC())
            {
                iFlag=CheckLoaderHasTray(false, 0, false);                      //Steven 20161223 (jou) : 確認LoaderSensor統一成Function
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
                    Task=1;
                    return true;
                }

                if(bNeedPickupErrorICToRecycleBin())                            //JimmyChiu 20220908 add Pickup Error Placement
                {
                    Task=10;
                    break;
                }
                Task=12;
            }
            else
            {
                if(bNeedOneCycle())                                             //Steven 20131029 : 解決Index Position Error
                {
                    Task=1;
                    return true;
                }

                if((TrayForm.iManualRemoveLoader!=0 &&                          //Steven 20130819 : 加入開門檢查的功能  (==true --> !=0)
                    bLoaderHasSkip==true) ||                                    //jou 2010-12-20 修正需要手動拿Loader Tray時,In Arm Y移至shuttle 2上,方便OP拿Tray
                    bNeedManualRemoveTray==true)                                //Ifor 20200331 : add Edit Loader Tray Need Manual Remove Tray
                {
                    if(MoveInArm2XYToShuttle2Wait())
                    {
                        Task=15;
                    }
                }
                else
                {
                    if(MoveInArm2XYToWait())
                    {
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

            if(ArmFinishForLoader())
            {
                Task=2100;
            }
            else if(MOT[MMTrayY].HasIC())
            {
                if(SearchAndMoveInArmXYToLoad_9045())
                {
                    if(IsCheckInArmDestroyActiveFinish())                       //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
                    {
                        Task=200;
                    }
                }
            }
            else
            {
                Task=10;
            }
            break;
        case 15:
            if(MOT[MMTrayY_Car].fHasTray ||
               MOT[MMTrayY].fHasTray ||
               MOT[MMTrayZ].fHasTray)
            {
                Task=10;
                return false;
            }
            else
            {
                if(DoAutoSkipCheck())                                           //Steven 20241218 :  avoid the tray not goes down yet
                {
                    if(InArmSuck.HasRealIC())                                   //Sam 20250307 : 真的上面有 IC 再來補滿
                        SetInArmUseSuckToHasNullIC(iSht, iKit);                 //Steven 20241226
                    else
                        InArmSuck.ClearAll();

                    Task=1;
                    return true;
                }
            }
            break;
        case 20:
            if(MoveInArm2XYToWait())
                Task=30;
            break;
        case 30:
            if(INSTALL_OCR_YMot==eocrYMotInstal)                                //Frank 20250214 add
            {
                Cylinder[C_TrayY_Fixer].On();
                DoArmPickFromLoadStage_Delay.SetSecAndOn(Ld_UldDelayTime.LD_FixTrayDely);
                Task=39;
            }
            else
            {
                if(Cylinder[C_TrayY_Fixer].Push())
                    Task=40;
            }
            break;
        case 39:
            if(DoArmPickFromLoadStage_Delay.Off())
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

                if(InArmSuck.iPickRow==1)
                {
                    iLoadPitchStepY=0;
                }
                else
                {
                    if(USE_IN_Y_IS_AUTO_PITCH==true &&                          //ChungHung 20131231 alter AutoYPitch  //JerryYang 20251218 : IN/OUT ARM支援不同模組
                       ArmRow1FinishForLoader()==true)
                    {
                        iLoadPitchStepY=0;
                    }
                    else
                    {
                        iLoadPitchStepY=GetLoaderYPitchStep();
                    }
                }

                sLoadPickupClean();                                             //kevin 20220723 initial data
                Task=1000;
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
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(InArmSuckUse[i][j] && InArmSuck.Item[i][j]==NULL_IC)
                    {
                        if(iXPosition[j]<0)                                                                             //Steven 20240920 : Loader吸料計算的資料錯誤;
                        {
                            ShowErrorMessage("WAR0149", 0, MInArmX, 0, IndexSuckName[i][j]);
                            continue;
                        }

                        if(ArmSpeed_File[InArm].bUseHPVacuum)                                                           //Steven 20180125 (Jou) : 加熱盤的真空等待時間
                            InArmSuck.Suck[i][j].OnDelayTime=ArmSpeed[InArm].dVacuumTI*100;

                        iYpos=iYPosition+iLoadPitchStepY*i;
                        if(bSuckEnd[i][j]==false && InArmSuck.Suck[i][j].Suck())                                        //wei 20150318 suck 判斷是否動作
                        {
                            AddLoadingCount(i, j, iYpos, iXPosition[j]);                                                //Steven 20161123 (jou) : 將Loader吸取的數量計算統一成Function
                        }
                        else if(InArmSuck.Suck[i][j].Error==false)
                        {
                            flag1=false;
                        }
                        else if(InArmSuck.Suck[i][j].Error==true)
                        {
                            bSuckEnd[i][j]=true;
                            sLoadPickupErrorTrayPos[i][j].sprintf("(%d,%d);", iXPosition[j]+1, iYpos+1);                //kevin 20220521 Load pick error record X, Y 座標
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

            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
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
//                            if(bPickUpHomeFinish==false)                      //Steven 20240417 : mark
//                            {
//                                bPickUpHomeFinish=true;
//                                Task=1010;
//                                SetInArmHome();
//                            }
                        }
                        return false;
                    }
                }
            }

            iRetryCT=0;
            if((bCanRunSCKART==true ||                                                                                  //Steven 20161223 (wei) : For SCK ART loading count
                IniConfig.bA65_BundleIDList) &&
               iSCKARTLoadingStatus>=1 &&                                                                               //數量到了要放下來,然後 Alarm
               TestIF_File.bSCKART_LotDeviceCheck &&
               bSCKARTLoadCntAlarm==true)
            {
                ProcessSCKARTLoadingCount(true);
                Task=4000;
            }
            else if(IniConfig.bP57LoaderAutoCleanOutByInputCT &&                                                        //Sam 20250605 : Loader Count AutoCleanOut
                    iSCKARTLoadingStatus>=1 &&
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
            if(MoveInArmZToPlateSafeAndCheckLoaderTray(Task))                   //Ifor 20221227 add:新增Loader 取料後判斷是否吸到Loader Tray
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
            if(MoveInArmZToPlateSafeAndCheckLoaderTray(Task))                   //Ifor 20221227 add:新增Loader 取料後判斷是否吸到Loader Tray
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
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
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
                if(CosFunction.bOpenDoorCheckLoaderAfterTrayEnd)                                                        //Jimmychiu 20250826 : Open Door Check Loader After TrayEnd
                {
                    bNeedManualCheckEmptyTray=true;
                    ShowErrorMessage("MES0923", K_RETRY, MMTrayY, false, "DoCatchTray");
                }
                bTrayEndCleanOut=true;
                return DoTrayEndProcess_9045();                                                                         //Steven 20190703 : 整合Tray End動作
            }
            else if(ret==K_CLEAN_OUT)                                                                                   // maybe is SKIP ,so can not include under line
            {
                fMain->CleanOut("DoInArmPickFromLoadStage");
                Task=1;
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

            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(InArmSuckUse[i][j])
                    {
                        Task=10;
                        break;
                    }
                }
            }

            iRetryCT=0;
            Task=2000;
            break;
        case 1200:
            if(MoveInArmZToPlateSafeAndCheckLoaderTray(Task))                   //Ifor 20221227 add:新增Loader 取料後判斷是否吸到Loader Tray
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
            if(MoveInArmZToPlateSafeAndCheckLoaderTray(Task))                   //Ifor 20221227 add:新增Loader 取料後判斷是否吸到Loader Tray                                     //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home
            {
                Task=10;
            }
            break;
        case 2000:
            if(MoveInArmZToPlateSafeAndCheckLoaderTray(Task))                   //Ifor 20221227 add:新增Loader 取料後判斷是否吸到Loader Tray
            {
                bPickFallDown=CheckInArmSuckFromLoaderICFallDown();             //Steven 20110516
                if(bPickFallDown)
                {
                    if(CosFunction.bAutoSkipNoDropError &&
                       IniConfig.bE60PickLoaderDropAutoSkip &&
                       (bDropRetry==true &&
                        ArmSpeed[InArm].bAutoSKIP==true &&
                        IniConfig.bRecordSkipPosition &&
                        bFirstRecordLoaderData==true))
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
            }
            break;
        case 2100:
            if(ArmFinishForLoader())
            {
                SetInArmUseSuckToHasNullIC(iSht, iKit);
                fObserver->AddTimeData(1, MyInArmAtShuttleTimer.LatchCycleTime()/1000.0);                               //JerryYang 20151209
                Task=1;
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
                            Task=1;
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
                            Task=1;
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
            if(MoveInArmZToPlateSafeAndCheckLoaderTray(Task))                   //Ifor 20221227 add:新增Loader 取料後判斷是否吸到Loader Tray
            {
                Task=1;

                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
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
//------------------------------------------------------------------------------
bool bUseAxExPicker()
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
bool bUseAxxGPicker()
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
bool bUseACEGPicker()
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
int GetJStep()
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
int GetShuttleCol(int iSuckRow, int iSuckCol, bool bOutArm)
{
    int iShtCol=0;

    if(USE_PICKER_COUNT==ep1Picker)
    {
        return iSuckCol;
    }

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
    else if(iInArmType==e9045_2x2_4_12)
    {
        iShtCol=XPHSuckToSht_2x2_12[iSuckCol];
    }
    else if(iInArmType==e9045_1x2_2_13 ||
            iInArmType==e9045_2x1_2_13 ||
            iInArmType==e9045_2x2_4_13)
    {
        iShtCol=XPHSuckToSht_2x2_13[InArmSuck.iModeX][iSuckCol];
    }
    else if(iInArmType==e9045_1x2_2_14 ||
            iInArmType==e9045_2x2_2_14 ||
            iInArmType==e9045_2x2_4_14)
    {
        iShtCol=XPHSuckToSht_2x2_14[InArmSuck.iModeX][iSuckCol];
    }
    else if(iInArmType==e9045_2x2_8_Hot)
    {
        if(bRunAutoClean)
            iShtCol=XPHSuckToSht_2x2_13[0][iSuckCol];
        else
            iShtCol=XPHSuckToSht_2x2_8[iSuckCol];
    }
    else if(iInArmType==e9045_1x3_2_14 ||
            iInArmType==e9045_2x3_6_14)
    {
        if(bOutArm)                                                             //Steven 20240427 : fixed for 1x3
            iShtCol=XPHSuckToSht_2x3_14_OutArm[InArmSuck.iModeX][iSuckCol];
        else
            iShtCol=XPHSuckToSht_2x3_14[InArmSuck.iModeX][iSuckCol];
    }
    else if(iInArmType==e9045_1x3_4 ||
            iInArmType==e9045_2x3_6)
    {
        if(bOutArm)                                                             //Steven 20240427 : fixed for 1x3
            iShtCol=XPHSuckToSht_2x3_6_OutArm[InArmSuck.iModeX][iSuckCol];
        else
            iShtCol=XPHSuckToSht_2x3_6[InArmSuck.iModeX][iSuckCol];
    }
    else if(iInArmType==e9045_1x4_4_13)
    {
        iShtCol=XPHSuckToSht_1x4_13[iSuckRow][iSuckCol];
    }
    else if(iInArmType==e9045_1x4_2_14)
    {
        iShtCol=XPHSuckToSht_2x4_14[InArmSuck.iModeX][iSuckCol];
    }
    else if(iInArmType==e9045_1x4_4 ||
            iInArmType==e9045_1x4_4_Back ||
            iInArmType==e9045_1x4_8_Hot)
    {
        iShtCol=XPHSuckToSht_2x4_8[InArmSuck.iModeX][iSuckCol];
    }
    else if(iInArmType==e9045_2x4_4_13)
    {
        iShtCol=XPHSuckToSht_2x4_13[InArmSuck.iModeX][iSuckCol];
    }
    else if(iInArmType==e9045_2x4_4_14)
    {
        iShtCol=XPHSuckToSht_2x4_14[InArmSuck.iModeX][iSuckCol];
    }
    else if(iInArmType==e9045_2x4_8)
    {
        iShtCol=XPHSuckToSht_2x4_8[InArmSuck.iModeX][iSuckCol];
    }
    else if(iInArmType==e9045_2x5_8)
    {
        iShtCol=XPHSuckToSht_2x5_8[InArmSuck.iModeX][iSuckCol];
    }
    else if(iInArmType==e9045_2x6_8)
    {
        iShtCol=XPHSuckToSht_2x6_6[InArmSuck.iModeX][iSuckCol];
    }
    else if(iInArmType==e9045_2x8_8 ||
            iInArmType==e9045_2x8_32)
    {
        if(bOutArm)                                                             //JerryYang 20250711 : fix for 2x8特殊關SITE
            iShtCol=XPHSuckToSht_2x8_8_OutArm[InArmSuck.iModeX][iSuckRow][iSuckCol];
        else
            iShtCol=XPHSuckToSht_2x8_8[InArmSuck.iModeX][iSuckRow][iSuckCol];
    }

    return iShtCol;
}
//------------------------------------------------------------------------------
bool DoPickLoaderOK(int iSht, int iKit)
{
    bool bflag=false;
    if(IniConfig.bI28_OnOffSiteOnTheFly &&
       LastSet.iTemperature==Tempture_Hot &&
       InArmSideAllCloseWithKit(iSht, iKit))
    {
        SetInArmUseSuckToHasNullIC(iSht, iKit);
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
int GetInArmPitchXMM_9045(int iMotPulse)
{
    double m=0.0, r=0.0;
    m=ChangeToFloatNonPcnt((double)(((double)(Prod.iInArmX120Pitch-Prod.iInArmX40Pitch))), (double)(iPitch_Max_minus_Min));
    r=(double)ChangeToFloatNonPcnt((double)((iMotPulse-Prod.iInArmX40Pitch)), (double)(m));
    r+=double(iXpitchMinX3);
    return (int)r;
}
//==============================================================================
int GetInArmPitchYMM_9045(int iMotPulse)
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
void GetInArmCellPos(int iRow, int iCol, int &Ypos, int &Xpos)
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
void GetInArmToShtCellPos(int iRow, int iCol, int &Ypos, int &Xpos)
{
    int iMovePitchX=GetInArmPitchXMM_9045(MOT[MInArmPitch].ReadPos());
//    int iMovePitchY=GetInArmPitchYMM_9045(MOT[MInArmPitchY].ReadPos());
    iMovePitchX/=3;
    Xpos=MOT[MInArmX].ReadPos()+(iMovePitchX*(iCol-iInArmXBase));
    Ypos=MOT[MInArmY].ReadPos();
}
//==============================================================================
int GetInOffsetFromWhichTarget(int iWhichTarget)
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
void InspectInArmPosition(int iTarget, int iSuckRow, int iSuckCol, int iTargetRow, int iTargetCol, bool bPlace)
{
    if(IniConfig.bE74_InspectArmPosition==false)                                //Jimmychiu 20240408 : debug for inarm position
        return;

    bool bX, bY;
    AnsiString sError, sPos="";
    AnsiString sPlace=(bPlace)?("Place to"):("Pick from");
    int XEncoder=0, YEncoder=0, HardwarePosX=0, HardwarePosY=0;

    if(InArmSuck.Item[iSuckRow][iSuckCol]!=HAS_NULL_IC &&
       InArmSuck.Item[iSuckRow][iSuckCol]!=HAS_NULL_CLEAN_IC)
    {
//        if(iTarget==MInShuttle1 ||
//           iTarget==MInShuttle2)
//            GetInArmToShtCellPos(iSuckRow, iSuckCol, YEncoder, XEncoder);
//        else
            GetInArmCellPos(iSuckRow, iSuckCol, YEncoder, XEncoder);

        if(iTarget==MMTrayY)
        {
            HardwarePosX=Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+Prod.LoadForm.iXPitch*iTargetCol;
            HardwarePosY=Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]-Prod.LoadForm.iYPitch*iTargetRow;
        }
        else if(iTarget==MMPlate1)
        {
            HardwarePosX=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+Prod.HotPlateForm[0].iXPitch*iTargetCol;
            HardwarePosY=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-Prod.HotPlateForm[0].iYPitch*iTargetRow;
        }
        else if(iTarget==MMPlate2)
        {
            HardwarePosX=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+Prod.HotPlateForm[0].iXPitch*iTargetCol;
            HardwarePosY=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-Prod.HotPlateForm[0].iYPitch*iTargetRow;
        }
        else if(iTarget==MInShuttle1 ||
                iTarget==MInShuttle2)
        {
            GetShtRowColPos(iTarget,iTargetCol,iTargetRow,HardwarePosX,HardwarePosY);
        }
        else if(iTarget==MMAutoCleanKit)
        {
            HardwarePosX=Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]+TestIF_File.dAutoClean_XPitch*iTargetCol;
            HardwarePosY=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-TestIF_File.dAutoClean_YPitch*iTargetRow;
        }
        else if(iTarget==MInRotateKit)                                          //待確認
        {
            HardwarePosX=Prod.iInArmRotateToHotPlateX;
            HardwarePosY=Prod.iInArmRotateToHotPlateY;
        }
        else if(iTarget==MPreciser)                                             //待確認
        {
            HardwarePosX=Prod.iInArmPreciserX;
            HardwarePosY=Prod.iInArmPreciserY;
        }

        if(USE_PICKER_COUNT==ep1Picker)
            HardwarePosX=HardwarePosX;
        else
            HardwarePosX=HardwarePosX+(((InArmOffSet[GetInOffsetFromWhichTarget(iTarget)]->GetVariable())/3)*(iSuckCol-iInArmXBase));                           //Jimmy 20240826 : fixed for E74
        HardwarePosY=HardwarePosY-(InArmOffSet[GetInOffsetFromWhichTarget(iTarget)]->GetVariableY()*(iSuckRow-iInArmYBase));

        int iLimit=InputLimit.iOffsetXYHigh*100;
        bX=MOT[MInArmX].CheckArmPosArrival(HardwarePosX, XEncoder, iLimit);     //Steven 20240719 : 放寬檢查的範圍
        bY=MOT[MInArmY].CheckArmPosArrival(HardwarePosY, YEncoder, iLimit);
        if(bX==false || bY==false)
        {
            if(iTarget==MMTrayY)
            {
                sPos="Loader";
            }
            else if(iTarget==MMPlate1)
            {
                sPos="Hot Plate 1";
            }
            else if(iTarget==MMPlate2)
            {
                sPos="Hot Plate 2";
            }
            else if(iTarget==MInShuttle1)
            {
                sPos="Shuttle 1";
            }
            else if(iTarget==MInShuttle2)
            {
                sPos="Shuttle 2";
            }
            else if(iTarget==MMAutoCleanKit)
            {
                sPos="Clean Kit";
            }
            else if(iTarget==MInRotateKit)
            {
                sPos="In Rotator";
            }
            else if(iTarget==MPreciser)
            {
                sPos="Precisor";
            }
            sError.sprintf("InArm Suck[%d, %d] Pos(Y=%d, X=%d), \r%s %s [%d, %d] Pos(Y=%d, X=%d)",
                           iSuckRow, iSuckCol, YEncoder, XEncoder, sPlace, sPos, iTargetRow, iTargetCol, HardwarePosY, HardwarePosX);

            if(bUseTwoArm32Site==false)
                ShowMyMessage(sError);
        }
    }
}
//------------------------------------------------------------------------------
extern bool DoInArmPlaceToShuttle_9045_1x1_1();
extern bool DoInArmPlaceToShuttle_9045_1x2_2_13();
extern bool DoInArmPlaceToShuttle_9045_1x2_2_14();
extern bool DoInArmPlaceToShuttle_9045_1x2_4_Hot();
extern bool DoInArmPlaceToShuttle_9045_1x3_2_14();
extern bool DoInArmPlaceToShuttle_9045_1x3_4();
extern bool DoInArmPlaceToShuttle_9045_1x4_2_14();
extern bool DoInArmPlaceToShuttle_9045_1x4_4_13();
extern bool DoInArmPlaceToShuttle_9045_1x4_4();
extern bool DoInArmPlaceToShuttle_9045_1x4_4_Back();
extern bool DoInArmPlaceToShuttle_9045_1x4_8_Hot();
extern bool DoInArmPlaceToShuttle_9045_2x1_2();
extern bool DoInArmPlaceToShuttle_9045_2x2_4_12();
extern bool DoInArmPlaceToShuttle_9045_2x2_4_13();
extern bool DoInArmPlaceToShuttle_9045_2x2_4_14();
extern bool DoInArmPlaceToShuttle_9045_2x2_8_Hot();
extern bool DoInArmPlaceToShuttle_9045_2x3_6();
extern bool DoInArmPlaceToShuttle_9045_2x3_6_14();
extern bool DoInArmPlaceToShuttle_9045_2x4_4_13();
extern bool DoInArmPlaceToShuttle_9045_2x4_4_14();
extern bool DoInArmPlaceToShuttle_9045_2x4_8();
extern bool DoInArmPlaceToShuttle_9045_2x5_8();
extern bool DoInArmPlaceToShuttle_9045_2x6_8();
extern bool DoInArmPlaceToShuttle_9045_2x8_8();
extern bool DoInArmPlaceToShuttle_9045_All_1Pick();
bool DoInArmPlaceToShuttle_9045()
{
    bool bResult=false;
    AnsiString Str;
    bPlaceToHotplate=false;                                                     //RogerYang 20251220 : add for catchtray hangup
    if(USE_PICKER_COUNT==ep1Picker)
    {
        bResult=DoInArmPlaceToShuttle_9045_All_1Pick();
    }
    else if(iInArmType==e9045_1x1_1 ||
       iInArmType==e9045_1x4_1_Ac)
    {
        bResult=DoInArmPlaceToShuttle_9045_1x1_1();
    }
    else if(iInArmType==e9045_1x2_2_13)
    {
        bResult=DoInArmPlaceToShuttle_9045_1x2_2_13();
    }
    else if(iInArmType==e9045_1x2_2_14)
    {
        bResult=DoInArmPlaceToShuttle_9045_1x2_2_14();
    }
    else if(iInArmType==e9045_1x2_4_Hot)
    {
        bResult=DoInArmPlaceToShuttle_9045_1x2_4_Hot();
    }
    else if(iInArmType==e9045_1x3_2_14)
    {
        bResult=DoInArmPlaceToShuttle_9045_1x3_2_14();
    }
    else if(iInArmType==e9045_1x3_4)
    {
        bResult=DoInArmPlaceToShuttle_9045_1x3_4();
    }
    else if(iInArmType==e9045_1x4_4_13)
    {
        bResult=DoInArmPlaceToShuttle_9045_1x4_4_13();
    }
    else if(iInArmType==e9045_1x4_2_14)
    {
        bResult=DoInArmPlaceToShuttle_9045_1x4_2_14();
    }
    else if(iInArmType==e9045_1x4_4_Back)
    {
        bResult=DoInArmPlaceToShuttle_9045_1x4_4_Back();
    }
    else if(iInArmType==e9045_1x4_4)
    {
        bResult=DoInArmPlaceToShuttle_9045_1x4_4();
    }
    else if(iInArmType==e9045_1x4_8_Hot)
    {
        bResult=DoInArmPlaceToShuttle_9045_1x4_8_Hot();
    }
    else if(iInArmType==e9045_2x1_2_13)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x1_2();
    }
    else if(iInArmType==e9045_2x2_4_12)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x2_4_12();
    }
    else if(iInArmType==e9045_2x2_4_13)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x2_4_13();
    }
    else if(iInArmType==e9045_2x2_4_14)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x2_4_14();
    }
    else if(iInArmType==e9045_2x2_8_Hot)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x2_8_Hot();
    }
    else if(iInArmType==e9045_2x3_6_14)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x3_6_14();
    }
    else if(iInArmType==e9045_2x3_6)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x3_6();
    }
    else if(iInArmType==e9045_2x4_4_13)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x4_4_13();
    }
    else if(iInArmType==e9045_2x4_4_14)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x4_4_14();
    }
    else if(iInArmType==e9045_2x4_8)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x4_8();
    }
    else if(iInArmType==e9045_2x5_8)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x5_8();
    }
    else if(iInArmType==e9045_2x6_8)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x6_8();
    }
    else if(iInArmType==e9045_2x8_8)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x8_8();
    }
    else if(iInArmType==e9045_2x8_32)
    {
        bResult=DoInArmPlaceToShuttle_9045_2x8_8();
    }
    else
    {
        Str.sprintf("iInArmType=%d", iInArmType);                               //Steven 20220620 : add log message
        ShowMyMessage("Program Error in DoInArmPlaceToShuttle_9045()", Str);
    }
    return bResult;
}
//-----------------------------------------------------------------------------
int iDoInArmAutoCalSuckZTask=0;
TQPF_Timer tAutoCalSuckDelayInarm;
bool DoInArmAutoCalSuckZ(bool bIsFirst)
{
    bool bResult=false;
    int &Task=iDoInArmAutoCalSuckZTask;
    static int iInArmZVacuumbufferPos[MAX_ARM_Row][MAX_ARM_Col]={0};            //Z軸吸取高度
    int iMot[MAX_ARM_Row][MAX_ARM_Col]={
        {MInArmZA, MInArmZC, MInArmZE, MInArmZG},
        {MInArmZB, MInArmZD, MInArmZF, MInArmZH}
    };
    TEdit *edZ[MAX_ARM_Row][MAX_ARM_Col]={
        {fProductionInfo->setEditZ1A, fProductionInfo->setEditZ1C, fProductionInfo->setEditZ1E, fProductionInfo->setEditZ1G},
        {fProductionInfo->setEditZ1B, fProductionInfo->setEditZ1D, fProductionInfo->setEditZ1F, fProductionInfo->setEditZ1H}
    };
    TMyKitSuck& ArmSuck=InArmSuck;
    bool bflag=false;
    static int iStep=0;                                                         //因在橫桿校正高度，一次做一列
    static bool bSuckActive[MAX_ARM_Row][MAX_ARM_Col]={false};
    if(bIsFirst)
    {
        Task=1;
        iStep=0;
        ZeroMemory(bSuckActive , sizeof(bSuckActive));
        return true;
    }
    switch(Task)
    {
        case 1:                                                                 //Initial
            for(int irow=0; irow<MAX_ARM_Row; irow++)
            {
                for(int icol=0; icol<MAX_ARM_Col; icol++)
                {
                    if(irow==iStep)
                    {
                        bSuckActive[irow][icol]=true;
                        iInArmZVacuumbufferPos[irow][icol]=iInArmSearchStartZ;
                    }
                    else
                    {
                        bSuckActive[irow][icol]=false;
                    }
                    ArmSuck.Suck[irow][icol].Off();
                }
            }
            Task=100;
//            break;
        case 100:                                                                                                       //Suck Z to safe area and move to teaget area
            if(MoveInArmXYAndSuckUp(InArmAutoCalSuckZPoint.X, InArmAutoCalSuckZPoint.Y,iStep))                          //iType 0:A row 2:B row
            {
                Task=500;
            }
            break;
        case 500:                                                               //A row down to flate surface and Get Height
            bflag=true;
            for(int irow=0; irow<MAX_ARM_Row; irow++)
            {
                for(int icol=0; icol<MAX_ARM_Col; icol++)
                {
                    if(bSuckActive[irow][icol]==true)
                    {
                        if(ArmSuck.Suck[irow][icol].Enable &&
                           ArmSuck.Suck[irow][icol].SenUsing!="")
                        {
                            ArmSuck.Suck[irow][icol].On();
                        }

                        if(MOT[iMot[irow][icol]].MotorMove(iInArmZVacuumbufferPos[irow][icol])==false)
                        {
                            bflag=false;
                        }
                    }
                }
            }

            if(bflag)
            {
                tAutoCalSuckDelayInarm.SetSecAndOn(0.2);
                Task=600;
            }
            break;
        case 600:                                                               //check suck status
            if(tAutoCalSuckDelayInarm.Off()==false)
                break;
            bflag=true;
            for(int irow=0; irow<MAX_ARM_Row; irow++)
            {
                for(int icol=0; icol<MAX_ARM_Col; icol++)
                {
                    if(bSuckActive[irow][icol]==true &&
                       ArmSuck.Suck[irow][icol].GetStatus()==false)
                    {
                        iInArmZVacuumbufferPos[irow][icol]-=10;
                        bflag=false;
                        #ifdef SOFT_SIMULTE
                        if(iInArmZVacuumbufferPos[irow][icol]>=-2000+(irow*icol+icol))
                        {
                            bflag=true;
                        }
                        #endif
                    }
                }
            }

            if(bflag)
            {
                Task=1000;
            }
            else
            {
                Task=650;
            }
            break;
        case 650:                                                               //check motor step loss
            for(int irow=0; irow<MAX_ARM_Row; irow++)
            {
                for(int icol=0; icol<MAX_ARM_Col; icol++)
                {
                    if(iInArmZVacuumbufferPos[irow][icol]<-3000)
                    {
                        Task=8000;
                        SetInArmHome();
                        ShowMyMessage("Inarm suck motor step loss : "+ArmSuck.Suck[irow][icol].sName);
                        break;
                    }
                }
            }
            Task=500;
            break;
        case 1000:
            iStep++;
            if(iStep<MAX_ARM_Row)
            {
                Task=1;
            }
            else
            {
                Task=2000;
            }
            break;
        case 2000:  //record pos
            for(int irow=0; irow<MAX_ARM_Row; irow++)
            {
                for(int icol=0; icol<MAX_ARM_Col; icol++)
                {
                    if(irow==iInArmYBase && icol==iInArmXBase)
                    {
                        iInArmZHeightDiff[irow][icol]=0;
                    }
                    else
                    {
                        iInArmZHeightDiff[irow][icol]=iInArmZVacuumbufferPos[irow][icol]-iInArmZVacuumbufferPos[iInArmYBase][iInArmXBase]-Tech.iInArmZHeightSub[irow][icol];  //1051-1000-50
                    }
                    edZ[irow][icol]->Text=IntToStr(iInArmZHeightDiff[irow][icol]);
                }
            }
            fProductionInfo->SaveAutoCalSuckZ();
            SetTechDataToProd();
            Task=8000;
            break;
        case 8000:                                                              //z to 0
            bflag=true;
            for(int irow=0; irow<MAX_ARM_Row; irow++)
            {
                for(int icol=0; icol<MAX_ARM_Col; icol++)
                {
                    if(MOT[iMot[irow][icol]].MotorMove(0)==false)
                    {
                        bflag=false;
                    }
                }
            }

            if(bflag)
            {
                Task=9999;
            }
            break;
        case 9999:                                                              //finish
            bResult=true;
            break;
        default:
            bResult=true;
            break;
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool MoveInArmXYAndSuckUp(int iPosX, int iPosY, int iType)                      //iType 0:A row 2:B row
{
    if(MoveInArmZToPlateSafe(0)==false)
    {
        return false;
    }
    int iXPos=0,iYPos=0,iMovePitchX=0,iMovePitchY=0;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    int iActSuckR=iType;

    ZeroMemory(iXVariable , sizeof(iXVariable));

    iMovePitchX=iXpitchMinX3;
    iMovePitchY=TestIF.iARM_Y_PITCH;

    iXVariable[0]=GetInArmPitch_9045(iMovePitchX);
    iXVariable[1]=GetInArmPitch2_9045(iMovePitchX);
    iYVariable=GetInArmPitchY_9045(iMovePitchY);
    if(IN_OUT_ARM_Y_PITCH_MIN==0)
    {
        iMovePitchY=TestIF.iARM_Y_PITCH;
    }
    else
    {
        iMovePitchY=iYVariable;
    }

    iXPos=iPosX;
    iYPos=iPosY+(iActSuckR-iOutArmYBase)*iMovePitchY;

    if(InArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
double FindCentorPointIndex(int iItemCount)
{
    int iPitchCount=iItemCount-1;
    double dret=(double)iPitchCount/2;
    return dret;
}
//---------------------------------------------------------------------------
int GetShtStartPos(int iCentorABSPostion, double dCentorIndex, int iGap)
{
    int iret=iCentorABSPostion-(int)(dCentorIndex*iGap);
    return iret;
}
//---------------------------------------------------------------------------
int GetShtStartPos(int iItemCount,
                   int iCentorABSPostion,
                   int iGap)
{
    double dCentorIndex=FindCentorPointIndex(iItemCount);
    int iStart=GetShtStartPos(iCentorABSPostion, dCentorIndex, iGap);
    return iStart;
}
//---------------------------------------------------------------------------
void GetShtRowColStartPos(int iTarget, int &iColStart, int &iRowStart)
{
    int iRow=TestSocket.iShtRow;
    int iCol=TestSocket.iShtCol;
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
void GetShtRowColPos(int iTarget, int iColIndex, int iRowindex, int &HardwarePosX, int &HardwarePosY)
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
//---------------------------------------------------------------------------
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
            if(MoveInArm2XYToDecayTeach())
            {
                RecordProcess("In Arm Ion Fan Give Way");
                Task=5;
            }
            break;
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
//------------------------------------------------------------------------------
