#include "MachineDefine.h"
#pragma hdrstop

#include "cInArmPlacement.h"
#include "cprod.h"
#include "cmydef.h"
#include "mysensor.h"
#include "HTimer.h"
#include "ainarm2.h"
#include "MyMotor.h"
#include "mycylin.h"
#include "MyKitSuck.h"
#include "note.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
const int iArmDownDelayTime=0.1;
//---------------------------------------------------------------------------
cInArmPlacement::cInArmPlacement()
{
    InitialDatas();
}
//---------------------------------------------------------------------------
cInArmPlacement::~cInArmPlacement()
{
    InitialDatas();
}
//---------------------------------------------------------------------------
void cInArmPlacement::InitialDatas()
{
}
//---------------------------------------------------------------------------
void cInArmPlacement::InitialShuttleDatas()
{
    iSuckToRecycleFromShuttleTask=0;
    iSuckFromShuttleToPlaceTask=0;
    iSuckFromShuttleTask=0;
    iPlaceSuckToPlacementTask=0;
    asErrorInArmPlacement=AnsiString("");
    iShuttleLine=0;
    iWhichShuttle=0;
    iSuckToRecycleFromLoaderTask=0;
    iSuckFromLoaderToPlaceTask=0;
}
//---------------------------------------------------------------------------
bool cInArmPlacement::DoSuckToRecycleFromShuttle(bool bIsFirst, AnsiString &asError)
{
    int &iTask=iSuckToRecycleFromShuttleTask;
    static int iErrorNo=0;
    if(bIsFirst)
    {
        asError=AnsiString("");
        iTask=1;
        return true;
    }
    else
    {
        switch(iTask)
        {
            case 1://Initial
                iTask=1000;
                break;
            case 1000://check shuttle has error IC
                iErrorNo=DoCheckShuttleSensor();
                if(iErrorNo>=0)
                {
                    iTask=1100;
                }
                else
                {
                    iTask=9900;
                }
                break;
            case 1100://pick
                DoSuckFromShuttleToPlace(true, iWhichShuttle, iErrorNo, asErrorInArmPlacement);
                iTask=1200;
//                break;
            case 1200://pick
                if(DoSuckFromShuttleToPlace(false, iWhichShuttle, iErrorNo, asErrorInArmPlacement))
                {
                    iTask=1000;
                }
                break;
            case 9900://finish
                return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
int cInArmPlacement::DoCheckShuttleSensor()
{
    int Pos=-1;
    if(iShuttleLine<=0)
    {
        return -1;
    }
    else
    {
        for(int i=0; i<iShuttleLine; i++)
        {
            Pos=SThreadPara.iInShSenIndex[iWhichShuttle][i];
            if(Sen[Pos].Enable && Sen[Pos].IsOn())
            {
                return i;
            }
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
bool cInArmPlacement::DoSuckFromShuttleToPlace(bool bIsFirst, int iSht, int iCol, AnsiString &asError)
{
    int &iTask=iSuckFromShuttleToPlaceTask;
    if(bIsFirst)
    {
        asError=AnsiString("");
        iTask=1;
        return true;
    }
    else
    {
        switch(iTask)
        {
            case 1:                                                             //Initial
                iTask=1000;
                break;
            case 1000:                                                          //pick shuttle row1
                DoSuckFromShuttle(true, iSht, ShRow1, iCol, asError);
                iTask=1100;
//                break;
            case 1100:
                if(DoSuckFromShuttle(false, iSht, ShRow1, iCol, asError))
                {
                    iTask=1200;
                }
                break;
            case 1200:                                                          //move to placement
                DoPlaceSuckToPlacement(true, asError);
                iTask=1300;
//                break;
            case 1300:                                                          //move to placement
                if(DoPlaceSuckToPlacement(false, asError))
                {
                    iTask=2000;
                }
                break;
            case 2000:                                                          //pick shuttle row2
                DoSuckFromShuttle(true, iSht, ShRow2, iCol, asError);
                iTask=2100;
//                break;
            case 2100:                                                          //pick shuttle row2
                if(DoSuckFromShuttle(false, iSht, ShRow2, iCol, asError))
                {
                    iTask=2200;
                }
                break;
            case 2200:                                                          //pick shuttle row2
                DoPlaceSuckToPlacement(true, asError);
                iTask=2300;
//                break;
            case 2300:                                                          //move to placement
                if(DoPlaceSuckToPlacement(false, asError))
                {
                    iTask=9900;
                }
                break;
            case 9900:                                                          //finish
                return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool cInArmPlacement::DoSuckFromShuttle(bool bIsFirst, int iSht, int iRow, int iCol, AnsiString &asError)
{
    int &iTask=iSuckFromShuttleTask;
    int iPosX=0, iPosY=0;
    if(bIsFirst)
    {
        asError=AnsiString("");
        iTask=1;
        return true;
    }
    else
    {
        switch(iTask)
        {
            case 1:                                                             //Initial
                iTask=100;
                break;
            case 100:                                                           //move to error ic and suck
                GetShuttlePos(iSht, iRow, iCol, iPosX, iPosY);
                if(MoveInArmXY(iPosX, iPosY))
                {
                    DoInArmPlaceDown(true);
                    DoInArmPlaceSuck(true);
                    timeArmDownDelay.SetSecAndOn(iArmDownDelayTime);
                    iTask=200;
                }
                break;
            case 200:                                                           //wait and check suck
                if(timeArmDownDelay.Off())
                {
                    DoInArmPlaceDown(false);
                    timeArmDownDelay.SetSecAndOn(iArmDownDelayTime);
                    iTask=300;
                }
                break;
            case 300:                                                           //wait and check suck
                if(timeArmDownDelay.Off())
                {
                    if(CheckInArmPlaceSuckHasIC()==false)
                    {
                        asError.sprintf("Auto Pickup Error IC Failed,Shuttle:%d, Row:%d, Col:%d", iSht, iRow, iCol);
                        int ret=ShowErrorMessage("JAM0126", K_SKIP|K_RETRY, MInArmX, false, asError);
                        if(ret==K_RETRY)
                        {
                            asError="";
                            iTask=100;
                        }
                        else                                                    //K_SKIP
                        {
                            iTask=9900;
                        }
                    }
                    else
                    {
                        iTask=9900;
                    }
                }
                break;
            case 9900:                                                          //finish
                return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
void cInArmPlacement::GetShuttlePos(int iSht, int iRow, int iCol, int &iPosX, int &iPosY)
{
    iPosX=iInArmShtXCenterPos;                                                  //offset by shuttle center
    iPosY=iInArmShtYCenterPos;
    if(iSht==0)                                                                 //move to shuttle basic point
    {
        iPosX+=Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];            //Steven 20141110 : [0][2] --> [iInArmYBase][iInArmXBase]
        iPosY+=Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
    }
    else
    {
        iPosX+=Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
        iPosY+=Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
    }
    //move to basic hole/suck
    //<==
    iPosX+=TestIF.dSiteXPitch/2;
    if(USE_IN_Y_IS_AUTO_PITCH==true)                                                  //ChungHung 20140304 add for AutoYPitch  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        iPosY-=TestIF.dSiteYPitch/2;
    else
        iPosY+=TestIF.dSiteYPitch/2;
    //<==
    //move to basic hole/suck
    //move to Placement Suck
    //<==
    iPosX+=Prod.iInPlacementOffsetByBasicX;
    iPosY+=Prod.iInPlacementOffsetByBasicY;
    //<==
    //move to Placement Suck
    //move to Destination hole 2x4
    //<==
    iPosX=(iPosX-(iInArmXBase*TestIF.dSiteXPitch))+iCol*TestIF.dSiteXPitch;
    iPosY=(iPosY-(iInArmYBase*TestIF.dSiteYPitch))+iRow*TestIF.dSiteYPitch;
    //<==
    //move to Destination hole 2x4
    TransferInShuttleRatio(iSht, &iPosX, &iPosY, iInArmYBase, iInArmXBase);     //Steven 20110324
}
//---------------------------------------------------------------------------
void cInArmPlacement::GetPlacementPos(int &iPosX, int &iPosY)
{
    iPosX=Prod.iInPlacementX;
    iPosY=Prod.iInPlacementY;
}
//---------------------------------------------------------------------------
bool cInArmPlacement::MoveInArmXY(int iPosX, int iPosY)
{
    int iXPos=iPosX, iYPos=iPosY;
    int iXVariable[X_PITCH_COUNT]={0, 0, 0, 0};
    int iYVariable  =0;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(InArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false, false))
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void cInArmPlacement::DoInArmPlaceDown(bool bDown)
{
    if(bDown)
    {
        if(Cylinder[C_TrayX_UpDown].OnStatus()==false)
        {
            Cylinder[C_TrayX_UpDown].On();
        }
    }
    else
    {
        if(Cylinder[C_TrayX_UpDown].OnStatus()==true)
        {
            Cylinder[C_TrayX_UpDown].Off();
        }
    }
}
//---------------------------------------------------------------------------
void cInArmPlacement::DoInArmPlaceSuck(bool bSuck)
{
    if(bSuck)
    {
        InArmPlaceSuck.Suck[0][0].On();
    }
    else
    {
        InArmPlaceSuck.Suck[0][0].Normal();
    }
}
//---------------------------------------------------------------------------
bool cInArmPlacement::CheckInArmPlaceSuckHasIC()
{
    bool bflag=false;
    #ifdef SOFT_SIMULTE
    bflag=true;
    #else
    bflag=InArmPlaceSuck.Suck[0][0].GetStatus();

    #endif
    return bflag;
}
//---------------------------------------------------------------------------
bool cInArmPlacement::InArmPlacementEnable()
{
    return (USE_InPlacement==eartInstall && IniConfig.bE69_PickupErrorPlacement);
}
//---------------------------------------------------------------------------
bool cInArmPlacement::DoPlaceSuckToPlacement(bool bIsFirst, AnsiString &asError)
{
    int &iTask=iPlaceSuckToPlacementTask;
    int iPosX=0, iPosY=0;
    if(bIsFirst)
    {
        asError=AnsiString("");
        iTask=1;
        return true;
    }
    else
    {
        switch(iTask)
        {
            case 1:                                                             //Initial
                iTask=100;
                break;
            case 100:                                                           //move to error ic and suck
                GetPlacementPos(iPosX, iPosY);
                if(MoveInArmXY(iPosX, iPosY))
                {
                    DoInArmPlaceDown(true);
                    timeArmDownDelay.SetSecAndOn(iArmDownDelayTime);
                    iTask=200;
                }
                break;
            case 200:                                                           //wait and check suck
                if(timeArmDownDelay.Off())
                {
                    DoInArmPlaceSuck(false);
                    DoInArmPlaceDown(false);
                    timeArmDownDelay.SetSecAndOn(iArmDownDelayTime);
                    iTask=300;
                }
                break;
            case 300:                                                           //wait and check suck
                if(timeArmDownDelay.Off())
                {
                    iTask=9900;
                }
                break;
            case 9900:                                                          //finish
                return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool cInArmPlacement::DoCheckLoaderHasErrorIC(int &iRow, int &iCol)
{
    for(int j=0; j<MOT[MMTrayY].Tray.YItem; j++)
        for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
            if(MOT[MMTrayY].Tray.Data[i][j]>0)
            {
                if(MOT[MMTrayY].Tray.Data[i][j]==HAS_SKIP_IC)
                {
                    iCol=i;
                    iRow=j;
                    return true;
                }
            }
    return false;
}
//---------------------------------------------------------------------------
bool cInArmPlacement::DoSuckToRecycleFromLoader(bool bIsFirst, AnsiString &asError)
{
    int &iTask=iSuckToRecycleFromLoaderTask;
    static int iCol=0, iRow=0;
    if(bIsFirst)
    {
        asError=AnsiString("");
        iTask=1;
        return true;
    }
    else
    {
        switch(iTask)
        {
            case 1://Initial
                iTask=1000;
                break;
            case 1000://check loader has error IC
                if(DoCheckLoaderHasErrorIC(iRow, iCol))
                {
                    iTask=1100;
                }
                else
                {
                    iTask=9900;
                }
                break;
            case 1100://pick
                DoSuckFromLoaderToPlace(true, iRow, iCol, asErrorInArmPlacement);
                iTask=1200;
//                break;
            case 1200://pick
                if(DoSuckFromLoaderToPlace(false, iRow, iCol, asErrorInArmPlacement))
                {
                    iTask=1000;
                }
                break;
            case 9900://finish
                return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool cInArmPlacement::DoSuckFromLoaderToPlace(bool bIsFirst, int iRow, int iCol, AnsiString &asError)
{
    int &iTask=iSuckFromLoaderToPlaceTask;
    if(bIsFirst)
    {
        asError=AnsiString("");
        iTask=1;
        return true;
    }
    else
    {
        switch(iTask)
        {
            case 1://Initial
                iTask=1000;
                break;
            case 1000://pick shuttle row1
                DoSuckFromLoader(true, iRow, iCol, asError);
                iTask=1100;
//                break;
            case 1100:
                if(DoSuckFromLoader(false, iRow, iCol, asError))
                {
                    iTask=1200;
                }
                break;
            case 1200://move to placement
                DoPlaceSuckToPlacement(true, asError);
                iTask=1300;
//                break;
            case 1300://move to placement
                if(DoPlaceSuckToPlacement(false, asError))
                {
                    iTask=9900;
                }
                break;
            case 9900://finish
                return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool cInArmPlacement::DoSuckFromLoader(bool bIsFirst, int iRow, int iCol, AnsiString &asError)
{
    int &iTask=iSuckFromLoaderTask;
    int iPosX=0,iPosY=0;
    if(bIsFirst)
    {
        asError=AnsiString("");
        iTask=1;
        return true;
    }
    else
    {
        switch(iTask)
        {
            case 1://Initial
                iTask=100;
                break;
            case 100://move to error ic and suck
                GetLoaderPos(iRow, iCol, iPosX, iPosY);
                if(MoveInArmXY(iPosX, iPosY))
                {
                    DoInArmPlaceDown(true);
                    DoInArmPlaceSuck(true);
                    timeArmDownDelay.SetSecAndOn(iArmDownDelayTime);
                    iTask=200;
                }
                break;
            case 200://wait and check suck
                if(timeArmDownDelay.Off())
                {
                    DoInArmPlaceDown(false);
                    timeArmDownDelay.SetSecAndOn(iArmDownDelayTime);
                    iTask=300;
                }
                break;
            case 300://wait and check suck
                if(timeArmDownDelay.Off())
                {
                    if(CheckInArmPlaceSuckHasIC()==false)
                    {
                        asError.sprintf("Auto Pickup Error IC Failed,Loader, Row:%d, Col:%d, InarmX=%d,InarmY=%d", iCol, iRow, iPosX, iPosY);
                        int ret=ShowErrorMessage("JAM0126", K_SKIP|K_RETRY, MInArmX, false, asError);
                        if(ret==K_RETRY)
                        {
                            asError="";
                            iTask=100;
                        }
                        else//K_SKIP
                        {
                            iTask=9900;
                        }
                    }
                    else
                    {
                        iTask=9900;
                    }
                }
                break;
            case 9900://finish
                MOT[MMTrayY].SetTraySingleData(iCol, iRow, NULL_IC);
                return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
void cInArmPlacement::GetLoaderPos(int iRow, int iCol, int &iPosX, int &iPosY)
{
    iPosX=Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+(Prod.LoadForm.iXPitch*iCol);
    iPosY=Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]+(Prod.LoadForm.iYPitch*iRow);
    //move to Placement Suck
    //<==
    iPosX+=Prod.iInPlacementOffsetByBasicX;
    iPosY+=Prod.iInPlacementOffsetByBasicY;
    //<==
    //move to Placement Suck
    TransferLoaderRatio(&iPosX, &iPosY);                                        //Steven 20141029 : Loader的軟體齒輪比
}
//---------------------------------------------------------------------------
