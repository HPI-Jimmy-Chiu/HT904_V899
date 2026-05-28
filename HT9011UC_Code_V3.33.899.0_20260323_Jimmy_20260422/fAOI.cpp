//------------------------------------------------------------------------------

#include "MachineDefine.h"
#pragma hdrstop

#include "fAOI.h"
#include "main.h"
#include "cprod.h"
#include "MyKitSuck.h"
#include "cmydef.h"
#include "aoutarm.h"
#include "aoutarm9045.h"
#include "MyMotor.h"
#include "note.h"
#include "common.h"
//#include "INPUT.h"
#include "myswitch.h"
#include "mysensor.h"
#include "cOffSet.h"
#include "mymessbox.h"
#include "csystem.h"
#include "atester.h"
#include "rs232.h"
#include "myQwertyKeyBoard.h"
//#include "aScannerAOI.h"
#include "uTrayEditForm.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma link "SPComm"
#pragma resource "*.dfm"
TFrmAOI *FrmAOI;

int iTopViewTask=1;
int iTopViewInspectionTask=1;
int iPADViewTask=1;
int iPADViewInspectionTask=1;
int iBGAViewTask=1;
int iBGAViewInspectionTask=1;

int iScanAOITask=1;
int iScannerAOIInspectionTask=1;

int iAOITask=1;
const int eAOI_Top=1;
const int eAOI_PAD=2;
const int eAOI_BGA=3;
const int eAOI_SCANAOI=4;
bool bActiveSuck[MAX_ARM_Row][MAX_ARM_Col];

bool bVitroxBGAViewMap[MAX_ARM_Row][MAX_ARM_Col];                                 //Alick 20160713
bool bVitroxBGAViewUse[MAX_ARM_Row][MAX_ARM_Col];                                 //Alick 20160713
bool bBGAViewBySiteRes[MAX_ARM_Row][MAX_ARM_Col];                                 //Alick 20160713
bool bBGAViewBySiteResOld[MAX_ARM_Row][MAX_ARM_Col];                              //Alick 20160713
int  iBGAViewBySiteTotal[MAX_ARM_Row][MAX_ARM_Col];                               //Alick 20160713
bool bBGAViewByArmRes[2];                                                         //Alick 20160714
bool bBGAViewByArmResOld[2];                                                      //Alick 20160714
int  iBGAViewByArmTotal[2];                                                       //Alick 20160714
bool bBGAViewByArmDouChk[2]={false};                                              //Alick 20160715

bool bVitroxPADViewMap[MAX_ARM_Row][MAX_ARM_Col];                                 //Alick 20160713
bool bVitroxPADViewUse[MAX_ARM_Row][MAX_ARM_Col];                                 //Alick 20160713
bool bPADViewBySiteRes[MAX_ARM_Row][MAX_ARM_Col];                                 //Alick 20160713
bool bPADViewBySiteResOld[MAX_ARM_Row][MAX_ARM_Col];                              //Alick 20160713
int  iPADViewBySiteTotal[MAX_ARM_Row][MAX_ARM_Col];                               //Alick 20160713
bool bPADViewByArmRes[2];                                                         //Alick 20160714
bool bPADViewByArmResOld[2];                                                      //Alick 20160714
int  iPADViewByArmTotal[2];                                                       //Alick 20160714
bool bPADByArmDouChk[2]={false};                                                  //Alick 20160715

//bool bPickSH1Flag=false;                                                          //Alick 20160714    //Ifor 20191226 移至全域
//bool bPickSH2Flag=false;                                                          //Alick 20160714    //Ifor 20191226 移至全域
bool bBGABySiteFailCont=false;                                                    //Alick 20160714
bool bBGAByArmFailCont=false;                                                     //Alick 20160714
bool bPADBySiteFailCont=false;                                                    //Alick 20160714
bool bPADByArmFailCont=false;                                                     //Alick 20160714

TQPF_Timer hStartDelay;
TQPF_Timer hTimeOut;
bool bAOI_Fail_Unit=false;
int iAOI_Fail_Count=0;
int iAOIResult[2];
bool bTriggerAOI=false;
int iScanAOIIntervalCounter=9999;
bool bScanAOIBySiteFailCont=false;
bool bScanAOIByArmFailCont=false;
int  iScanAOIBySiteTotal[MAX_ARM_Row][MAX_ARM_Col];
bool bScanAOIByArmDouChk[2]={false};
int  iScanAOIByArmTotal[2];
TQPF_Timer hLGA_DelayTime;  //Ifor 20211022 add: AOI LGA Mode
int iScanLGAStep=0;         //Ifor 20211022 add: AOI LGA Mode
//Richard 20220817:Add Scanner_AOI add check Big Size IC Function
//==>
bool bplus =true;                        //判斷加or減
int irowX=0;
int icolumnY=0;
//<==
//Richard 20220817:Add Scanner_AOI add check Big Size IC Function

//Ifor 20200902 add: TFAMD Top AOI
//==>
int iTopScanAOITask=1;
int iTopScannerAOIInspectionTask=1;
int iTopScanAOIIntervalCounter=9999;
bool bTopScanAOIBySiteFailCont=false;
bool bTopScanAOIByArmFailCont=false;
int  iTopScanAOIBySiteTotal[MAX_ARM_Row][MAX_ARM_Col];
bool bTopScanAOIByArmDouChk[2]={false};
int  iTopScanAOIByArmTotal[2];
//<==
//Ifor 20200902 add: TFAMD Top AOI
int iLGAPitchXCnt,iLGAPitchYCnt,iLGAPitchX,iLGAPitchY;
//------------------------------------------------------------------------------
__fastcall TFrmAOI::TFrmAOI(TComponent* Owner)
    : TForm(Owner)
{
    mtDutCol_Vitrox->SetColorMap(0, clBtnFace);
    mtDutCol_Vitrox->SetColorMap(1, clGreen);
    mtDutRow_Vitrox->SetColorMap(0, clBtnFace);
    mtDutRow_Vitrox->SetColorMap(1, clGreen);
    mtDutOnOff_Vitrox->SetColorMap(0, clLime);
    mtDutOnOff_Vitrox->SetColorMap(1, clAqua);
    mtDutOnOff_Vitrox->SetColorMap(2, clSilver);
    mtDutCol_Vitrox1->SetColorMap(0, clBtnFace);
    mtDutCol_Vitrox1->SetColorMap(1, clGreen);
    mtDutRow_Vitrox1->SetColorMap(0, clBtnFace);
    mtDutRow_Vitrox1->SetColorMap(1, clGreen);
    mtDutOnOff_Vitrox1->SetColorMap(0, clLime);
    mtDutOnOff_Vitrox1->SetColorMap(1, clAqua);
    mtDutOnOff_Vitrox1->SetColorMap(2, clSilver);
    iQuotient=0;            //KenHsieh 20220830 : 修改add:AOI Tray Edit
    iRemainder=0;           //KenHsieh 20220830 : 修改add:AOI Tray Edit
}
//------------------------------------------------------------------------------
int DoViewBinFunction()
{
    int iResult=0;

    if(FrmAOI->ComboBox1->Text=="Auto1")
        iResult=0;
    else if(FrmAOI->ComboBox1->Text=="Auto2")
        iResult=1;
    else if(FrmAOI->ComboBox1->Text=="Auto3")
        iResult=2;
    else if(FrmAOI->ComboBox1->Text=="Fix1")
        iResult=3;
    else if(FrmAOI->ComboBox1->Text=="Fix3")
        iResult=5;

    return iResult;
}
//------------------------------------------------------------------------------
bool DoMoveXY_TopViewSafePos()
{
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXVariable[X_PITCH_COUNT];
    int iXPos=0, iYPos=0;
    int iYVariable  =0;
    int iMovePitchX =8000;
    int iMovePitchY =TestIF.iARM_Y_PITCH;
    int iOffsetPos  =-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(XPitchIsStand())                                                         //X Pitch is 4000mm
        iMovePitchX=TestIF.dSiteXPitch*3;
    else
        iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos=Prod.iTopViewSafePos_X;
    iYPos=Prod.iTopViewSafePos_Y;

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
        return true;
    return false;
}
//------------------------------------------------------------------------------
void SetActiveSuck()
{
    bool bflag=false;

    ZeroMemory(bActiveSuck ,sizeof(bActiveSuck));

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if((OutArmSuck.Item[i][j]!=NULL_IC && OutArmSuck.Item[i][j]!=HAS_NULL_IC)
                && OutArmSuck.iAOIStation[i][j]!=eAOI_Top)
            {
                bActiveSuck[i][j]=true;
                bflag=true;
                break;
            }
        }
        if(bflag)
            break;
    }
}
//------------------------------------------------------------------------------
bool DoMoveXY_TopView()
{
    int iRow=0,  iCol=0;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXVariable[X_PITCH_COUNT];
    int iXPos=0, iYPos=0;
    int iYVariable  =TestIF.dSiteXPitch*3;
    int iMovePitchX =8000;
    int iMovePitchY =TestIF.iARM_Y_PITCH;
    int iOffsetPos  =-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(bActiveSuck[i][j])
            {
                iRow=i;
                iCol=j;
            }
        }
    }

    if(XPitchIsStand())                                                         //X Pitch is 4000mm
        iMovePitchX=TestIF.dSiteXPitch*3;
    else
        iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos=Prod.iTopView_X+(iMovePitchX/3)*2-(iMovePitchX/3)*iCol;
    iYPos=Prod.iTopView_Y+iMovePitchY*iRow;

    if(IsCheckOutArmDestroyActiveFinish(iXPos, iYPos)==false)
        return false;

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
        return true;
    return false;
}
//------------------------------------------------------------------------------
bool DoMoveZ_TopView(int zPos)
{
    bool flag[2][4]={{true, true, true, true}, {true, true, true, true}};

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(bActiveSuck[i][j])
            {
                flag[i][j]=MOT[OutArmZIndex[i][j]].MotorMove(zPos);
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
bool DoPlace_TopView()
{
    bool bflag[MAX_ARM_Row][MAX_ARM_Col]={false};
    bool bHasDuplicateErr=false;
    bool bHasErr=false;
    AnsiString ErrPart=" ";

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(bActiveSuck[i][j])
            {
                bflag[i][j]=OutArmSuck.Suck[i][j].Destroy();
                if(bflag[i][j])
                {
                    AOIKit.MoveSuckData(OutArmSuck, i, j);
                    OutArmSuck.SetItemData(i, j, NULL_IC);

                    bOutArmSuckActive[i][j]=false;
                    bOutArmCheckDestroyACT[i][j]=true;                          //jou 981130 確認device確實destroy完成
                }

                if(OutArmSuck.Suck[i][j].Error)
                {
                    ErrPart+=InArmSuck.Suck[i][j].sName;
                    bHasErr=true;
                }
            }
            else
            {
                bflag[i][j]=true;
            }
        }
    }
    if(bHasErr)
    {
        ShowErrorMessage("JAM0203", K_RETRY, MOutArmX, bHasDuplicateErr, ErrPart);
    }
    for(int i=0; i<MAX_ARM_Row; i++)
        for(int j=0; j<MAX_ARM_Col; j++)
            if(bflag[i][j]==false)
                return false;
    return true;
}
//------------------------------------------------------------------------------
bool DoPick_TopView(bool &SuckError)
{
    bool bflag[MAX_ARM_Row][MAX_ARM_Col]={false};
    bool bHasErr=false;

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(bActiveSuck[i][j])
            {
                bflag[i][j]=OutArmSuck.Suck[i][j].Suck();
                if(bflag[i][j])
                {
                    OutArmSuck.MoveSuckData(AOIKit, i, j);
                }

                if(OutArmSuck.Suck[i][j].Error)
                {
                    bHasErr=true;
                }
            }
            else
            {
                bflag[i][j]=true;
            }
        }
    }
    if(bHasErr)
    {
        SuckError=true;
    }

    for(int i=0; i<MAX_ARM_Row; i++)
        for(int j=0; j<MAX_ARM_Col; j++)
            if(bflag[i][j]==false)
                return false;
    return true;
}
//------------------------------------------------------------------------------
bool DoTopViewMoveKit(bool Down)
{
    int iPos;

    if(Down)
    {
        iPos=Prod.iTopViewKit_Z;
    }
    else
    {
        iPos=Prod.iTopViewKit_Zup;
    }

    if(MOT[MAOIKit].MotorMove(iPos))
        return true;
    return false;
}
//------------------------------------------------------------------------------
void InitTopViewInspection()
{
    iTopViewInspectionTask=1;
}
//------------------------------------------------------------------------------
bool DoTopViewFunction_Inspection()
{
    bool bResult=false;
    int &Task=iTopViewInspectionTask;
    int ret;
    int iBin;

    switch(Task)
    {
        case 1:
            bAOI_Fail_Unit=false;
            Task=1000;
            break;
        case 1000:
            if(DoTopViewMoveKit(true))
            {
                hStartDelay.SetMSAndOn(tAOISetup.tTopView.iStartDelayTime);
                Task=2000;
            }
            break;
        case 2000:
            if(hStartDelay.Off())
            {
                Task=3000;
            }
            break;
        case 3000:
            hTimeOut.SetMSAndOn(tAOISetup.tTopView.iTimeOut);
            SW[SwStartTest1].On();
            Task=3500;
        case 3500:
            if(Sen[SnEOF1].IsOff())
            {
                Task=4000;
            }
            else if(hTimeOut.Off())
            {
                Task=4500;
            }
            break;
        case 4000:
            if(Sen[SnEOF1].IsOn())
            {
                SW[SwStartTest1].Off();

                if(Sen[SnAOIResult1].IsOff())
                {
                    bAOI_Fail_Unit=true;
                    iAOI_Fail_Count++;
                }

                if(iAOI_Fail_Count>=tAOISetup.iAlarmCount)
                {
                    Task=4600;
                }
                else
                {
                    Task=5000;
                }
            }
            else if(hTimeOut.Off())
            {
                Task=4500;
            }
            break;
        case 4500:
            SW[SwStartTest1].Off();
            ret=ShowErrorMessage("WAR16300", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                Task=3000;
            }
            else
            {
                bAOI_Fail_Unit=true;
                Task=5000;
            }
            break;
        case 4600:
            if(DoTopViewMoveKit(false))
            {
                Task=4610;
            }
            break;
        case 4610:
            iAOI_Fail_Count=0;
            ret=ShowErrorMessage("WAR16303", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                Task=1000;
            }
            else
            {
                Task=5000;
            }
            break;
        case 5000:
            if(DoTopViewMoveKit(false))
            {
                Task=6000;
            }
            break;
        case 6000:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(bActiveSuck[i][j])
                    {
                        OutArmSuck.iAOIStation[i][j]=eAOI_Top;
                        if(bAOI_Fail_Unit)
                        {
                            iBin=tAOISetup.tTopView.iFailBin;
                            CheckBin(iBin, 1); //kevin 20160826 add shuttle 2
                            AOIKit.iWhichAuto[i][j]=iBin;
                        }
                    }
                }
            }
            Task=7000;
            break;
        case 7000:
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitDoTopViewFunction()
{
    iTopViewTask=1;
}
//------------------------------------------------------------------------------
bool DoTopViewFunction()
{
    int &Task=iTopViewTask;
    bool bResult=false;
    bool bflag=false;
    bool bError=false;
    int ret;

    switch(Task)
    {
        case 1:
            SetActiveSuck();
            Task=1000;
            break;
        case 1000:
            if(DoMoveXY_TopView())
            {
                Task=1100;
            }
            break;
        case 1100:
            if(DoMoveZ_TopView(Prod.iTopView_Place))
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                (Sen[SnRKManualStep].IsOn() || fOffSet->UseAutoOffsetFunction("Top View")))    //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoTopViewFunction 1100");
                    Task=1200;
                }
                else
                {
                    Task=1500;
                }
            }
            break;
        case 1200:
            if(MoveOutArmToAutoSafe())
            {
                Task=1000;
            }
            break;
        case 1500:
            if(DoPlace_TopView())
            {
                Task=1300;
            }
            break;
        case 1300:
            if(MoveOutArmToAutoSafe())
            {
                Task=1400;
            }
            break;
        case 1400:
            if(DoMoveXY_TopViewSafePos())
            {
                Task=2000;
            }
            break;
        case 2000:
            InitTopViewInspection();
            Task=2100;
            break;
        case 2100:
            if(DoTopViewFunction_Inspection())
            {
                Task=3000;
            }
            break;
        case 3000:
            if(MoveOutArmToAutoSafe())
            {
                Task=3100;
            }
            break;
        case 3100:
            if(DoMoveXY_TopView())
            {
                Task=3200;
            }
            break;
        case 3200:
            if(DoMoveZ_TopView(Prod.iTopView_Pick))
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                (Sen[SnRKManualStep].IsOn() || fOffSet->UseAutoOffsetFunction("Top View")))    //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoTopViewFunction 3200");
                    Task=3250;
                }
                else
                {
                    Task=3300;
                }
            }
            break;
        case 3250:
            if(MoveOutArmToAutoSafe())
            {
                Task=3100;
            }
            break;
        case 3300:
            if(DoPick_TopView(bError))
            {
                Task=3400;
            }
            else if(bError)
            {
                Task=3310;
            }
            break;
        case 3310:
            if(MoveOutArmToAutoSafe())
            {
                Task=3320;
            }
            break;
        case 3320:
            ret=ShowErrorMessage("JAM0201", K_RETRY|K_SKIP, MOutArmX, false, NULL);

            if(ret==K_SKIP)
            {
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(OutArmSuck.Suck[i][j].Error)
                        {
                            OutArmSuck.SetItemData(i, j, NULL_IC);
                            AOIKit.SetItemData(i, j, NULL_IC);
                            Task=3400;
                        }
                    }
                }
            }
            else if(ret==K_RETRY)
            {
                Task=3200;
            }
            break;
        case 3400:
            if(MoveOutArmToAutoSafe())
            {
                Task=4000;
            }
            break;
        case 4000:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if((OutArmSuck.Item[i][j]!=NULL_IC) &&
                       (OutArmSuck.iAOIStation[i][j]!=eAOI_Top))
                    {
                        bflag=true;
                        break;
                    }
                }
            }

            if(bflag)
            {
                Task=1000;
            }
            else
            {
                Task=5000;
            }
            break;
        case 5000:
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
bool DoMoveXY_PADView(int &iRow, int &iCol)
{
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    int iMovePitchX=TestIF.dSiteXPitch*3;
    int iMovePitchY=TestIF.iARM_Y_PITCH;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    iZPos[iRow][iCol] =Prod.OutArm_Pad_Place[iRow][iCol];
    bZFlag[iRow][iCol]=true;

    if(XPitchIsStand())                                                         //X Pitch is 4000mm
        iMovePitchX=TestIF.dSiteXPitch/2*3;
    else
        iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos=Prod.iPADView_X+(iMovePitchX/3)*2-(iMovePitchX/3)*iCol;
    iYPos=Prod.iPADView_Y+iMovePitchY*iRow;

    if(IsCheckOutArmDestroyActiveFinish(iXPos, iYPos)==false)
        return false;

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
        return true;
    return false;
}
//------------------------------------------------------------------------------
void InitPADViewInspection()
{
    iPADViewInspectionTask=1;
}
//------------------------------------------------------------------------------
void DoContinuousFailBySitePADView(bool Res, int iRow, int iCol)                //Alick 20160712 add By Site 連續Fail  PADView
{
    if(Res==true && bVitroxPADViewUse[iRow][iCol])
    {
        iPADViewBySiteTotal[iRow][iCol]++;
    }
    else
    {
        iPADViewBySiteTotal[iRow][iCol]=0;
    }

    if(iPADViewBySiteTotal[iRow][iCol]>=tAOISetup.iContAlarmCountBySite)
        bPADBySiteFailCont=true;
}
//------------------------------------------------------------------------------
void DoContinuousFailByArmPADView(bool Res, int ArmSide)
{
    if(Res==true)
    {
        iPADViewByArmTotal[ArmSide]++;
    }
    else
    {
        iPADViewByArmTotal[ArmSide]=0;
    }

    if(iPADViewByArmTotal[ArmSide]>=tAOISetup.iContAlarmCountByArm)
    {
        bPADByArmFailCont=true;
        bPADByArmDouChk[ArmSide]=true;
    }
}
//------------------------------------------------------------------------------
void DoContinuousFailBySiteBGAView(bool Res, int iRow, int iCol)                //Alick 20160712 add By Site 連續Fail  BGAView
{
    if(Res==true && bVitroxBGAViewUse[iRow][iCol])
    {
        iBGAViewBySiteTotal[iRow][iCol]++;
    }
    else
    {
        iBGAViewBySiteTotal[iRow][iCol]=0;
    }

    if(iBGAViewBySiteTotal[iRow][iCol]>=tAOISetup.iContAlarmCountBySite)
        bBGABySiteFailCont=true;
}
//------------------------------------------------------------------------------
void DoContinuousFailByArmBGAView(bool Res, int ArmSide)
{
    if(Res==true)
    {
        iBGAViewByArmTotal[ArmSide]++;
    }
    else
    {
        iBGAViewByArmTotal[ArmSide]=0;
    }

    if(iBGAViewByArmTotal[ArmSide]>=tAOISetup.iContAlarmCountByArm)
    {
        bBGAByArmFailCont=true;
        bBGAViewByArmDouChk[ArmSide]=true;
    }
}
//------------------------------------------------------------------------------
bool DoPADViewFunction_Inspection(int &iRow, int &iCol)
{
    bool bResult=false;
    int &Task=iPADViewInspectionTask;
    int ret;
    int iBin;
    int iGetBin=0;
    switch(Task)
    {
        case 1:
            hStartDelay.SetMSAndOn(tAOISetup.tPADView.iStartDelayTime);
            bAOI_Fail_Unit=false;
            Task=1000;
            break;
        case 1000:
            if( hStartDelay.Off())
            {
                Task=2000;
            }
            break;
        case 2000:
            hTimeOut.SetMSAndOn(tAOISetup.tPADView.iTimeOut);
            SW[SwStartTest2].On();
            Task=2500;
        case 2500:
            if(Sen[SnEOF2].IsOff())
            {
                Task=3000;
            }
            else if(hTimeOut.Off())
            {
                Task=3500;
            }
            break;
        case 3000:
            if(Sen[SnEOF2].IsOn())
            {
                SW[SwStartTest2].Off();

                if(Sen[SnAOIResult2].IsOff())
                {
                    bAOI_Fail_Unit=true;
                    iAOI_Fail_Count++;
                }

                if(iAOI_Fail_Count>=tAOISetup.iAlarmCount)
                {
                    Task=3600;
                }
                else
                {
                    Task=4000;
                }

                if(fMain->cb1->Checked==true)
                {
                    if(rand()%2 ==1)
                    {
                        bAOI_Fail_Unit=false;
                        Task = 4000;
                    }
                    else
                    {
                        bAOI_Fail_Unit=true;
                        Task =4000;
                    }
                }
            }
            else if(hTimeOut.Off())
            {
                Task=3500;
            }
            break;
        case 3500:
            SW[SwStartTest2].Off();
            if(tAOISetup.tAutoSkipPadPkg.bEnabled)
                ret=K_SKIP;
            else
                ret=ShowErrorMessage("WAR16301", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                Task=3700;
            }
            else
            {
                bAOI_Fail_Unit=true;
                Task=4000;
            }
            break;
        case 3600:
            iAOI_Fail_Count=0;
            if(tAOISetup.tAutoSkipPadPkg.bEnabled)
                ret=K_SKIP;
            else
                ret=ShowErrorMessage("WAR16311", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                Task=3700;
            }
            else
            {
                Task=4000;
            }
            break;
        case 3700:
            if(MoveOutArmToAutoSafe())
            {
                Task=3800;
            }
            break;
        case 3800:
            if(DoMoveXY_PADView(iRow, iCol))
            {
                Task=2000;
            }
            break;
        case 4000:
            if((OutArmSuck.Item[iRow][iCol]!=NULL_IC &&
                OutArmSuck.Item[iRow][iCol]!=HAS_NULL_IC) &&
               OutArmSuck.iAOIStation[iRow][iCol]!=eAOI_PAD)
            {
                if(tAOISetup.tAOINoSort.bEnabled==false)                        //wei 20161207
                {
                    if(bAOI_Fail_Unit)
                    {
                        if(tAOISetup.tTesterFailBin.bEnabled)
                        {
                            iGetBin=OutArmSuck.Item[iRow][iCol]-TEST_PASS;
                            if(Prod.bConsFail[iGetBin]==true)
                            {
                                iBin=tAOISetup.tBGAPADView.iTestFailFailBin;
                            }
                            else
                            {
                                iBin=tAOISetup.tBGAPADView.iTestPassFailBin;
                            }
                        }
                        else
                        {
                            iBin=tAOISetup.tPADView.iFailBin;
                        }
                        CheckBin(iBin, 1);
                        OutArmSuck.iWhichAuto[iRow][iCol]=iBin;
                    }
                }
                OutArmSuck.iAOIStation[iRow][iCol]=eAOI_PAD;
            }
            Task=5000;
            break;
        case 5000:
            if(tAOISetup.bEnabledContinueAlarm==true)                                                    //Alick 20160715
            {
                if(tAOISetup.bEnabledBySitePADView==true)
                {
                    DoContinuousFailBySitePADView(bAOI_Fail_Unit, iRow, iCol);
                }

                if(tAOISetup.bEnabledByArmPADView==true)
                {
                    if(bPickSH1Flag==true && bPickSH2Flag==false && bPADByArmDouChk[0]==false)
                        DoContinuousFailByArmPADView(bAOI_Fail_Unit, 0);
                    else if (bPickSH1Flag==false && bPickSH2Flag==true && bPADByArmDouChk[1]==false)
                        DoContinuousFailByArmPADView(bAOI_Fail_Unit, 1);
                }
            }
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitDoPADViewFunction()
{
    iPADViewTask=1;
}
//------------------------------------------------------------------------------
bool DoPADViewFunction()
{
    static int iSuckRow=0, iSuckCol=0;
    int &Task=iPADViewTask;
    int ret;
    int icount=0;
    bool bResult=false;
    bool bflag=false;
    AnsiString ErrPart="";

    switch(Task)
    {
        case 1:
            bRunAOI=false;
            Task=100;
            break;
        case 100:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_PAD &&
                       bVitroxPADViewUse[i][j] &&
                       ((tAOISetup.tFailRunFunction.bEnabled &&
                         bAOIPassFail[0][i][j]==true) ||
                         tAOISetup.tFailRunFunction.bEnabled==false) &&
                       bRunAOI==false)
                    {
                        iSuckRow=i;
                        iSuckCol=j;
                        bRunAOI=true;
                        Task=1000;
                        break;
                    }
                }
            }
            if(bRunAOI==false)
            {
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(bAOIPassFail[0][i][j]==false)
                            icount++;
                        if(icount>=TestSocket.iShtCnt)
                            Task=5000;
                    }
                }
            }
            break;
        case 1000:
            if(DoMoveXY_PADView(iSuckRow, iSuckCol))
            {
                if((AccessLevel>=1 ||
                    CosFunction.bOPCanPressStepAndTStart) &&                    //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                   (Sen[SnRKManualStep].IsOn() ||
                    fOffSet->UseAutoOffsetFunction("PAD View")))                //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoPADViewFunction 1000");
                    Task=1100;
                }
                else
                {
                    InitPADViewInspection();
                    Task=2000;
                }
            }
            break;
        case 1100:
            if(MoveOutArmToAutoSafe())
            {
                bRunAOI=false;
                Task=1000;
            }
            break;
        case 2000:
            if(DoPADViewFunction_Inspection(iSuckRow, iSuckCol))
            {
                Task=3000;
            }
            break;
        case 3000:
            if(MoveOutArmToAutoSafe())
            {
                Task=4000;
            }
            break;
        case 4000:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_PAD &&
                       bVitroxPADViewUse[i][j])
                    {
                        bflag=true;
                        break;
                    }
                }
            }

            if(bflag)
            {
                bRunAOI=false;
                Task=100;
            }
            else
            {
                if(tAOISetup.bEnabledContinueAlarm==true)
                {
                    if(tAOISetup.bEnabledBySitePADView==true && bPADBySiteFailCont==true)                       //Alick 20160718 修改做完FUNCTION判斷連續FAIL結果
                        Task=4100;
                    else if(tAOISetup.bEnabledByArmPADView==true && bPADByArmFailCont==true)
                        Task=4200;
                    else
                        Task=5000;
                }
                else
                {
                    Task=5000;
                }
            }
            break;
        case 4100:                                                              //Alick 20160718 add PAD Continuous Fail by Site
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(iPADViewBySiteTotal[i][j]>=tAOISetup.iContAlarmCountBySite)
                    {
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                    }
                }
            }

            ret=ShowErrorMessage("WAR16308", K_SKIP, MMSystem, false, ErrPart);
            if(ret==K_SKIP)
            {
                bPADBySiteFailCont=false;
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        iPADViewBySiteTotal[i][j]=0;
                    }
                }
            }

            if(tAOISetup.bEnabledByArmPADView==true && bPADByArmFailCont==true)
                Task=4200;
            else
                Task=5000;
            break;
        case 4200:                                                              //Alick 20160718 add PAD Continuous Fail by Arm
            if(bPADByArmDouChk[0])
                ret=ShowErrorMessage("WAR16309", K_SKIP, MMSystem);
            else
                ret=ShowErrorMessage("WAR163010", K_SKIP, MMSystem);

            if(ret==K_SKIP)
            {
                bPADByArmFailCont=false;
                if(bPADByArmDouChk[0])
                {
                    iPADViewByArmTotal[0]=0;
                    bPADByArmDouChk[0]=false;
                }
                else
                {
                    iPADViewByArmTotal[1]=0;
                    bPADByArmDouChk[1]=false;
                }
            }
            Task=5000;
            break;
        case 5000:
            bResult=true;
            break;
        default:
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool DoMoveXY_BGAView(int &iRow, int &iCol)
{
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    int iMovePitchX=TestIF.dSiteXPitch*3;
    int iMovePitchY=TestIF.iARM_Y_PITCH;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    iZPos[iRow][iCol] =Prod.OutArm_BGA_Place[iRow][iCol];
    bZFlag[iRow][iCol]=true;

    if(XPitchIsStand())                                                         //X Pitch is 4000mm
        iMovePitchX=TestIF.dSiteXPitch/2*3;
    else
        iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos=Prod.iBGAView_X+(iMovePitchX/3)*2-(iMovePitchX/3)*iCol;
    iYPos=Prod.iBGAView_Y+TestIF.iARM_Y_PITCH*iRow;

    if(IsCheckOutArmDestroyActiveFinish(iXPos, iYPos)==false)
        return false;

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
        return true;
    return false;
}
//------------------------------------------------------------------------------
void InitBGAViewInspection()
{
    iBGAViewInspectionTask=1;
}
//------------------------------------------------------------------------------
bool DoBGAViewFunction_Inspection(int &x, int &y)
{
    bool bResult=false;
//    bResult = true;

    int &Task=iBGAViewInspectionTask;
    int ret;
    int iBin;
    int iGetBin=0;
    switch(Task)
    {
        case 1:
            hStartDelay.SetMSAndOn(tAOISetup.tBGAView.iStartDelayTime);
            bAOI_Fail_Unit=false;
            Task=1000;
            break;
        case 1000:
            if(hStartDelay.Off())
            {
                Task=2000;
            }
            break;
        case 2000:
            hTimeOut.SetMSAndOn(tAOISetup.tBGAView.iTimeOut);
            SW[SwStartTest3].On();
            Task=2500;
        case 2500:
            if(Sen[SnEOF3].IsOff())
            {
                Task=3000;
            }
            else if(hTimeOut.Off())
            {
                Task=3500;
            }
            #ifdef SOFT_SIMULTE
            Task=3000;
            #endif
            break;
        case 3000:
            #ifndef SOFT_SIMULTE
            if(Sen[SnEOF3].IsOn())
            #endif
            {
                SW[SwStartTest3].Off();

                if(Sen[SnAOIResult3].IsOff())
                {
                    bAOI_Fail_Unit=true;
                    iAOI_Fail_Count++;
                }

                if(iAOI_Fail_Count>=tAOISetup.iAlarmCount)
                {
                    Task=3600;
                }
                else
                {
                    Task=4000;
                }

                if(fMain->cb1->Checked==true)
                {
                    if(rand()%2 ==1)
                    {
                        bAOI_Fail_Unit=false;
                        Task = 4000;
                    }
                    else
                    {
                        bAOI_Fail_Unit = true;
                        Task =4000;
                    }
                }
            }
            #ifndef SOFT_SIMULTE
            else if(hTimeOut.Off())
            {
                Task=3500;
            }
            #endif
            break;
        case 3500:
            SW[SwStartTest3].Off();
            if(tAOISetup.tAutoSkipBGA.bEnabled)
                ret=K_SKIP;
            else
                ret=ShowErrorMessage("WAR16302", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                Task=3700;
            }
            else
            {
                bAOI_Fail_Unit=true;
                Task=4000;
            }
            break;
        case 3600:
            iAOI_Fail_Count=0;
            if(tAOISetup.tAutoSkipBGA.bEnabled)
                ret=K_SKIP;
            else
                ret=ShowErrorMessage("WAR16312", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                Task=3700;
            }
            else
            {
                Task=4000;
            }
            break;
        case 3700:
            if(MoveOutArmToAutoSafe())
            {
                Task=3800;
            }
            break;
        case 3800:
            if(DoMoveXY_BGAView(x, y))
            {
                Task=2000;
            }
            break;
        case 4000:
            if((OutArmSuck.Item[x][y]!=NULL_IC &&
                OutArmSuck.Item[x][y]!=HAS_NULL_IC) &&
                OutArmSuck.iAOIStation[x][y]!=eAOI_BGA)
            {
                if(tAOISetup.tAOINoSort.bEnabled==false)                        //wei 20161207
                {
                    if(bAOI_Fail_Unit)
                    {
                        if(tAOISetup.tTesterFailBin.bEnabled)
                        {
                            iGetBin=OutArmSuck.Item[x][y]-TEST_PASS;
                            if(Prod.bConsFail[iGetBin]==true)
                            {
                                iBin=tAOISetup.tBGAPADView.iTestFailFailBin;
                            }
                            else
                            {
                                iBin=tAOISetup.tBGAPADView.iTestPassFailBin;
                            }
                        }
                        else
                        {
                            iBin=tAOISetup.tBGAView.iFailBin;

                        }
                        CheckBin(iBin, 1);
                        OutArmSuck.iWhichAuto[x][y]=iBin;
                        bAOIPassFail[0][x][y]=false;            //wei 20160616
                    }
                    else
                    {
                        bAOIPassFail[0][x][y]=true;             //wei 20160616
                    }
                }
                OutArmSuck.iAOIStation[x][y]=eAOI_BGA;
            }
            Task=5000;
            break;
        case 5000:
            if(tAOISetup.bEnabledContinueAlarm==true)                                                    //Alick 20160715
            {
                if(tAOISetup.bEnabledBySiteBGAView==true)                 // bAOI_Fail_Unit==true &&
                {
                    DoContinuousFailBySiteBGAView(bAOI_Fail_Unit, x, y);
                }

                if(tAOISetup.bEnabledByArmBGAView==true)
                {
                    if(bPickSH1Flag==true && bPickSH2Flag==false && bBGAViewByArmDouChk[0]==false)
                        DoContinuousFailByArmBGAView(bAOI_Fail_Unit, 0);
                    else if (bPickSH1Flag==false && bPickSH2Flag==true && bBGAViewByArmDouChk[1]==false)
                        DoContinuousFailByArmBGAView(bAOI_Fail_Unit, 1);
                }
            }
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitBGAViewFunction()
{
    iBGAViewTask=1;
}
//------------------------------------------------------------------------------
bool DoBGAViewFunction()
{
    int &Task=iBGAViewTask;
    bool bResult=false;
    bool bflag=false;
    int i, j;
    int ret;
    AnsiString ErrPart="";
    static int x=0, y=0;

    switch(Task)
    {
        case 1:
            bRunAOI=false;
            Task=100;
            break;
        case 100:
            for(i=0; i<MAX_ARM_Row; i++)
            {
                for(j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC && OutArmSuck.iAOIStation[i][j]!=eAOI_BGA && bRunAOI==false && bVitroxBGAViewUse[i][j])
                    {
                        x=i;
                        y=j;
                        bRunAOI=true;
                        Task=1000;
                        break;
                    }
                }
            }
            break;
        case 1000:
            if(DoMoveXY_BGAView(x, y))
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                (Sen[SnRKManualStep].IsOn() || fOffSet->UseAutoOffsetFunction("BGA View")))    //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoBGAViewFunction 1000");
                    Task=1100;
                }
                else
                {
                    InitBGAViewInspection();
                    Task=2000;
                }
            }
            break;
        case 1100:
            if(MoveOutArmToAutoSafe())
            {
                bRunAOI=false;
                Task=1000;
            }
            break;
        case 2000:
            if(DoBGAViewFunction_Inspection(x, y))
            {
                Task=3000;
            }
            break;
        case 3000:
            if(MoveOutArmToAutoSafe())
            {
                Task=4000;
            }
            break;
        case 4000:
            for(i=0; i<MAX_ARM_Row; i++)
            {
                for(j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC && OutArmSuck.iAOIStation[i][j]!=eAOI_BGA && bVitroxBGAViewUse[i][j])
                    {
                        bflag=true;
                        break;
                    }
                }
            }

            if(bflag)
            {
                bRunAOI=false;
                Task=100;
            }
            else
            {
                if(tAOISetup.bEnabledContinueAlarm==true)
                {
                    if(tAOISetup.bEnabledBySiteBGAView==true && bBGABySiteFailCont==true)                       //Alick 20160718 修改做完FUNCTION判斷連續FAIL結果
                        Task=4100;
                    else if(tAOISetup.bEnabledByArmBGAView==true && bBGAByArmFailCont==true)
                        Task=4200;
                    else
                        Task=5000;
                }
                else
                    Task=5000;
            }
            break;
        case 4100:                                                              //Alick 20160718 add BGA Continuous Fail by Site
            for(i=0; i<MAX_ARM_Row; i++)
            {
                for(j=0; j<MAX_ARM_Col; j++)
                {
                    if(iBGAViewBySiteTotal[i][j]>=tAOISetup.iContAlarmCountBySite)
                    {
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                    }
                }
            }
            ret=ShowErrorMessage("WAR16305", K_SKIP, MMSystem, false, ErrPart);
            if(ret==K_SKIP)
            {
                bBGABySiteFailCont=false;
                for(i=0; i<MAX_ARM_Row; i++)
                {
                    for(j=0; j<MAX_ARM_Col; j++)
                    {
                        iBGAViewBySiteTotal[i][j]=0;
                    }
                }
            }

            if(tAOISetup.bEnabledByArmBGAView==true && bBGAByArmFailCont==true)
                Task=4200;
            else
                Task=5000;
            break;
        case 4200:                                                              //Alick 20160718 add BGA Continuous Fail by Arm
            if(bBGAViewByArmDouChk[0])
                ret=ShowErrorMessage("WAR16306", K_SKIP, MMSystem);
            else
                ret=ShowErrorMessage("WAR16307", K_SKIP, MMSystem);

            if(ret==K_SKIP)
            {
                bBGAByArmFailCont=false;
                if(bBGAViewByArmDouChk[0])
                {
                    iBGAViewByArmTotal[0]=0;
                    bBGAViewByArmDouChk[0]=false;
                }
                else
                {
                    iBGAViewByArmTotal[1]=0;
                    bBGAViewByArmDouChk[1]=false;
                }
            }
            Task=5000;
            break;
        case 5000:
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
bool DoMoveXY_ScannerAOI(int &iRow, int &iCol)
{
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXVariable[X_PITCH_COUNT];
    int iXPos=0, iYPos=0;
    int iYVariable  =0;
    int iMovePitchX =8000;
    int iMovePitchY =TestIF.iARM_Y_PITCH;
    int iOffsetPos  =-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    AnsiString sRecord="";
    static int iCountX=0;
    static int iCountY=0;
    int iCntX=0, iCntY=0, iPitchX=0, iPitchY=0;                                 //KenHsieh 20230202 : Fix AOI LGA Mode Direction

    iZPos[iRow][iCol] =Prod.OutArm_ScannerAOI_Place[iRow][iCol];
    bZFlag[iRow][iCol]=true;

    if(TestIF_File.iTestMode==SingleSite)
    {
        iMovePitchX=8000;
    }
    else
    {
        if(XPitchIsStand())                                                     //X Pitch is 4000mm
            iMovePitchX=TestIF.dSiteXPitch/2*3;
        else
            iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos=Prod.iScannerAOI_X+(iMovePitchX/3)*2-(iMovePitchX/3)*iCol;            //移到第一顆
    iYPos=Prod.iScannerAOI_Y+iMovePitchY*iRow;

    if(ScannerAOIIF.bLGAModeDirection==true)                                    //KenHsieh 20230202 : Fix AOI LGA Mode Direction
    {
        iCntX   =iLGAPitchXCnt;
        iCntY   =iLGAPitchYCnt;
        iPitchX =iLGAPitchX;
        iPitchY =iLGAPitchY;
    }
    else
    {
        iCntY   =iLGAPitchXCnt;
        iCntX   =iLGAPitchYCnt;
        iPitchY =iLGAPitchX;
        iPitchX =iLGAPitchY;
    }

    if(ScannerAOIIF.bScanAOIUseLGAMode==true)
    {
        if(iCntX==0 && iCntY==0)
        {

        }
        else if(iCntX==0 && iCntY!=0)
        {
            iXPos=iXPos+((DeviceForm_File.YDimension*100/(iCntY+1)/2)*((iCntY+1)*2/2-1)-(iPitchY*iScanLGAStep));
        }
        else if(iCntY==0 && iCntX!=0)
        {
            iXPos=iXPos-(DeviceForm_File.XDimension*100/(iCntX+1)/2)*((iCntX+1)*2/2-1)+(iPitchX*iScanLGAStep);
        }
        else
        {                                                                       //Richard 20220817:Add Scanner_AOI add check Big Size IC Function
            iXPos=iXPos-(DeviceForm_File.XDimension*100/(iCntX+1)/2)*((iCntX+1)*2/2-1);   //第一點拍攝位置在pin1對角位置  iX-(IC分割後每個分割之一半(ICx) * 要走幾個ICx) 第一點位置
            iYPos=iYPos+(DeviceForm_File.YDimension*100/(iCntY+1)/2)*((iCntY+1)*2/2-1);

            if(iScanLGAStep==0)
            {
                iCountX=0;
                iCountY=0;
            }

            iXPos=iXPos+iCountX*iPitchX;

            if(iCountX%2==0)
            {
                iYPos=iYPos-(iCountY)*iPitchY;
            }
            else
            {
                iYPos=iYPos-iCntY*iPitchY+(iCountY)*iPitchY;
            }
        }
    }
    sRecord.sprintf("iX:%d,iY:%d", iXPos, iYPos);

    if(IsCheckOutArmDestroyActiveFinish(iXPos, iYPos)==false)
        return false;

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
    {
        iCountY++;
        if(iCountY==iCntY+1)
        {
            iCountY=0;
            iCountX++;
        }
        FrmAOI->ShowCommData( "[Position]", sRecord);
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------
void InitScannerAOIInspection()
{
    iScannerAOIInspectionTask=1;
    iScanLGAStep=0;                                                             //Ifor 20211022 add: AOI LGA Mode
    bplus   =true;                                                              //判斷加or減
    irowX   =0;                                                                 //Richard 20220817:Add Scanner_AOI add check Big Size IC Function
    icolumnY=0;
}
//------------------------------------------------------------------------------
bool DoScanAOIFunction_Inspection(int &iRow, int &iCol)
{
    bool bResult=false;

    int &Task=iScannerAOIInspectionTask;
    int ret;
    int iBin;
    switch(Task)
    {
        case 1:
            hStartDelay.SetMSAndOn(ScannerAOIIF.iStartDelayTime);
            bAOI_Fail_Unit=false;
            Task=1000;
            break;
        case 1000:
            if(hStartDelay.Off())
            {
                Task=2000;
            }
            break;
        case 2000:                                                              //Richard 20220817:Add Scanner_AOI add check Big Size IC Function
            if(iScanLGAStep==0)
            {
                FrmAOI->TriggerAOISystem(true, true);                           //第一點位觸發改為"11BDAOI"
            }
            else if(iScanLGAStep<=((iLGAPitchXCnt+1)*(iLGAPitchYCnt+1))-1)
            {
                FrmAOI->TriggerAOISystem(true, false);                           //其他點位觸發改為"10BDAOI"
            }
            if(ScannerAOIIF.bScanAOIUseLGAMode==true && iScanLGAStep<((iLGAPitchXCnt+1)*(iLGAPitchYCnt+1))-1) //更改拍攝次數
            {
                Task=2100;
                hLGA_DelayTime.SetSecAndOn(ScannerAOIIF.fLGAWaitTime);
            }
            else
            {
                hTimeOut.SetMSAndOn(ScannerAOIIF.iTimeOut);
                Task=2500;
            }
            break;
        case 2100:
            if(hLGA_DelayTime.Off())
            {
                iScanLGAStep++;                                                 //Richard 20220817:Add Scanner_AOI add check Big Size IC Function 更改拍攝次數
                Task=3700;
            }
            break;
        case 2500:
            if(bTriggerAOI==false)                                              //收到 AOI 回覆
            {
                Task=3000;
            }
            else if(hTimeOut.Off())
            {
                Task=3500;
                FrmAOI->ShowCommData( "[Decode]" , " TimeOut");
            }
            break;
        case 3000:
            if(bTriggerAOI==false)                                              //收到 AOI 回覆
            {
                if(iAOIResult[0]!=1)
                {
                    bAOI_Fail_Unit=true;
                    iAOI_Fail_Count++;
                }
                Task=4000;
            }
            break;
        case 3500:
            ret=ShowErrorMessage("WAR0883", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                bplus   =true;                                                  //判斷加or減
                irowX   =0;                                                     //Richard 20220817:Add Scanner_AOI add check Big Size IC Function
                icolumnY=0;
                Task    =3700;
                iScanLGAStep=0;
            }
            else
            {
                bAOI_Fail_Unit=true;
                Task=4000;
            }
            break;
        case 3700:
            if(MoveOutArmToAutoSafe())
            {
                Task=3800;
            }
            break;
        case 3800:
            if(DoMoveXY_ScannerAOI(iRow, iCol))
            {
                Task=2000;
            }
            break;
        case 4000:
            if((OutArmSuck.Item[iRow][iCol]!=NULL_IC &&
                OutArmSuck.Item[iRow][iCol]!=HAS_NULL_IC) &&
                OutArmSuck.iAOIStation[iRow][iCol]!=eAOI_SCANAOI)
            {
                if(bAOI_Fail_Unit &&
                   tAOISetup.bEnabledScanAOIUnUseFailBin==false)                //Ifor 20200825 add:TF-AMD AOI Error 不丟至Fail Bin
                {                                                               //0: Auto1  1: Auto2    2: Auto3
                                                                                //3: Fix1   4: Fix2     5: Fix3     6: Fix4     7: Fix5     8: Fix6
                    iBin=GetAOIFailBin(iRow, iCol);                             //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
                    OutArmSuck.iWhichAuto[iRow][iCol]=iBin;
                    bAOIPassFail[0][iRow][iCol]=false;                          //wei 20160616
                }
                else
                {
                    bAOIPassFail[0][iRow][iCol]=true;                           //wei 20160616
                }
                OutArmSuck.iAOIStation[iRow][iCol]=eAOI_SCANAOI;
            }
            else if(OutArmSuck.Item[iRow][iCol]==HAS_NULL_IC)
            {
                OutArmSuck.iAOIStation[iRow][iCol]=eAOI_SCANAOI;
            }
            Task=5000;
            break;
        case 5000:
            if(tAOISetup.bEnabledScanAOIBySiteAlarm==true)                      // bAOI_Fail_Unit==true &&
            {
                FrmAOI->DoContinuousFailBySiteScanAOI(bAOI_Fail_Unit, iRow, iCol);
            }

            if(tAOISetup.bEnabledScanAOIByArmAlarm==true)
            {
                if(bPickSH1Flag==true && bPickSH2Flag==false && bScanAOIByArmDouChk[0]==false)
                    FrmAOI->DoContinuousFailByArmScanAOI(bAOI_Fail_Unit, 0);
                else if (bPickSH1Flag==false && bPickSH2Flag==true && bScanAOIByArmDouChk[1]==false)
                    FrmAOI->DoContinuousFailByArmScanAOI(bAOI_Fail_Unit, 1);
            }
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitTopScannerAOIInspection()
{
    iTopScannerAOIInspectionTask=1;
}
//------------------------------------------------------------------------------
bool DoTopScanAOIFunction_Inspection(int &iRow, int &iCol)
{
    bool bResult=false;

    int &Task=iTopScannerAOIInspectionTask;
    int ret;
    int iBin;
    switch(Task)
    {
        case 1:
            hStartDelay.SetMSAndOn(ScannerAOIIF.iTopStartDelayTime);
            bAOI_Fail_Unit=false;
            Task=1000;
            break;
        case 1000:
            if(hStartDelay.Off())
            {
                Task=2000;
            }
            break;
        case 2000:
            hTimeOut.SetMSAndOn(ScannerAOIIF.iTopTimeOut);
            FrmAOI->TriggerAOISystem(true, false, 1);
            Task=2500;
        case 2500:
            if(bTriggerAOI==false)                                              //收到 AOI 回覆
            {
                Task=3000;
            }
            else if(hTimeOut.Off())
            {
                Task=3500;
            }
            break;
        case 3000:
            if(bTriggerAOI==false)                                              //收到 AOI 回覆
            {
                if(iAOIResult[0]!=1)
                {
                    bAOI_Fail_Unit=true;
                }
                Task=4000;
            }
            break;
        case 3500:
            ret=ShowErrorMessage("WAR0887", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                Task=2000;
            }
            else
            {
                bAOI_Fail_Unit=true;
                Task=4000;
            }
            break;
        case 4000:
            if((OutArmSuck.Item[iRow][iCol]!=NULL_IC &&
                OutArmSuck.Item[iRow][iCol]!=HAS_NULL_IC) &&
                OutArmSuck.iAOIStation[iRow][iCol]!=eAOI_Top)
            {
                if(bAOI_Fail_Unit &&                                            //Ifor 20200825 add:TF-AMD AOI Error 不丟至Fail Bin
                   tAOISetup.bEnabledTopScanAOIUnUseFailBin==false)             //0: Auto1  1: Auto2    2: Auto3
                {                                                               //3: Fix1   4: Fix2     5: Fix3     6: Fix4     7: Fix5     8: Fix6
                    iBin=ScannerAOIIF.TopScannerIfError+4;                      //Fix 2 = 0+4
                    if(ScannerAOIIF.TopScannerIfError==2)
                    {
                        if(TrayForm.bTrayUpDownSet[eFix2]==true)
                        {
                            iBin=7;                                             //Fix5 = 2 + 4 + 1 = 7
                        }
                        else
                        {
                            iBin=4;                                             //Fix2 = 2 + 4 - 2 = 4
                        }
                    }
                    else if(ScannerAOIIF.TopScannerIfError==3)
                    {
                        if(TrayForm.bTrayUpDownSet[eFix3]==true)
                        {
                            iBin=8;                                             //Fix6 = 3 + 4 + 1 = 8
                        }
                        else
                        {
                            iBin=5;                                             //Fix3 = 3 + 4- 2 = 5
                        }
                    }
                    OutArmSuck.iWhichAuto[iRow][iCol]=iBin;
                    bAOIPassFail[0][iRow][iCol]=false;                          //wei 20160616
                }
                else
                {
                    bAOIPassFail[0][iRow][iCol]=true;                           //wei 20160616
                }
                OutArmSuck.iAOIStation[iRow][iCol]=eAOI_Top;
            }
            else if(OutArmSuck.Item[iRow][iCol]==HAS_NULL_IC)
            {
                OutArmSuck.iAOIStation[iRow][iCol]=eAOI_Top;
            }
            Task=5000;
            break;
        case 5000:
            if(tAOISetup.bEnabledScanAOIBySiteAlarm==true)                      // bAOI_Fail_Unit==true &&
            {
                FrmAOI->DoContinuousFailBySiteScanAOI(bAOI_Fail_Unit, iRow, iCol, 1);
            }

            if(tAOISetup.bEnabledScanAOIByArmAlarm==true)
            {
                if(bPickSH1Flag==true && bPickSH2Flag==false && bScanAOIByArmDouChk[0]==false)
                    FrmAOI->DoContinuousFailByArmScanAOI(bAOI_Fail_Unit, 0);
                else if (bPickSH1Flag==false && bPickSH2Flag==true && bScanAOIByArmDouChk[1]==false)
                    FrmAOI->DoContinuousFailByArmScanAOI(bAOI_Fail_Unit, 1);
            }
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitialScanAOITask()
{
    iScanAOITask=1;
}
//------------------------------------------------------------------------------
bool DoScanAOIFunction()                                                        //Ifor 20191210 : add Scanner AOI
{
    int &Task=iScanAOITask;
    bool bResult=false;
    bool bflag=false;
    int ret;
    AnsiString ErrPart="";
    static int iRow=0, iCol=0;
    static int iScanSite=-1;
    int iCheckSite=0;
    static bool bScanStep=false;

    switch(Task)
    {
        case 1:
            bRunAOI=false;
            Task=100;
            if(ScannerAOIIF.iIntervalCounter==0)                                //JerryYang 20210224 : 修正沒有執行AOI的問題
            {
                iScanSite=-1;
            }
            break;
        case 100:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    iCheckSite=(i*4)+j;
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_SCANAOI &&
                       bRunAOI==false)
                    {
                        iScanAOIIntervalCounter++;
                        if(TestIF_File.iTestMode==SingleSite &&
                           iScanAOIIntervalCounter>=ScannerAOIIF.iIntervalCounter+1)
                        {
                            iScanAOIIntervalCounter=0;
                            iScanSite=iCheckSite;
                            iRow=i;
                            iCol=j;
                            bRunAOI=true;
                            break;
                        }
                        else if(TestIF_File.iTestMode==DualSite &&
                                ScannerAOIIF.iIntervalCounter==1 &&
                                TestIF_File.iShuttleMode==0)                    //Ifor 20200218 50%抽檢
                        {
                            if(iScanAOIIntervalCounter>=9999)                   //Ifor 20200218 add 為了讓第一顆IC先測試AOI初始值9999
                            {
                                iScanAOIIntervalCounter=1;
                            }
                            if(iScanAOIIntervalCounter==1 ||
                               iScanAOIIntervalCounter==3 ||
                               iScanAOIIntervalCounter==6 ||
                               iScanAOIIntervalCounter==8 )                     //Ifor 20200218 50%抽檢
                            {
                                if(iScanAOIIntervalCounter==8)
                                {
                                    iScanAOIIntervalCounter=0;
                                }
                                iScanSite=iCheckSite;
                                iRow=i;
                                iCol=j;
                                bRunAOI=true;
                                break;
                            }
                            else
                            {
                                OutArmSuck.iAOIStation[i][j]=eAOI_SCANAOI;
                            }
                        }
                        else if(iScanAOIIntervalCounter>=ScannerAOIIF.iIntervalCounter+1 && iScanSite!=iCheckSite)
                        {
                            iScanAOIIntervalCounter=0;
                            iScanSite=iCheckSite;
                            iRow=i;
                            iCol=j;
                            bRunAOI=true;
                            break;
                        }
                        else
                        {
                            OutArmSuck.iAOIStation[i][j]=eAOI_SCANAOI;
                        }
                    }
                    else if(LastSet.bUseTestSocket[0][i][j]==false ||
                            LastSet.bUseTestSocket[1][i][j]==false)             //Ifor 20210923 fix:關Site 計數異常導致AOI檢測條件不成立
                    {
                        if(iScanSite==iCheckSite-1 && bScanStep==false)
                        {
                            bScanStep=true;
                            iScanSite++;
                        }
                    }
                }
            }
            if(bRunAOI==true)
            {
                Task=1000;
                iScanLGAStep=0;                                                 //Ifor 20211022 add: AOI LGA Mode
            }
            else
            {
                return true;
            }
            break;
        case 1000:
            if(DoMoveXY_ScannerAOI(iRow, iCol))                                 //first移到IC中心
            {
                InitScannerAOIInspection();
                bScanStep=false;
                Task=2000;
                if(LastSet.iRealDummy!=REALLY)                                  //Ifor 20200825 add:Scan AOI Dummy 不檢測
                {
                    Task=3000;
                    bAOIPassFail[0][iRow][iCol]=true;
                    OutArmSuck.iAOIStation[iRow][iCol]=eAOI_SCANAOI;
                }
            }
            break;
        case 2000:
            if(DoScanAOIFunction_Inspection(iRow, iCol))
            {
                Task=3000;
            }
            break;
        case 3000:
            if(MoveOutArmToAutoSafe())
            {
                Task=4000;
            }
            break;
        case 4000:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_SCANAOI)
                    {
                        bflag=true;
                        break;
                    }
                }
            }
            if(bflag)
            {
                bRunAOI=false;
                Task=100;
            }
            else
            {
                if(tAOISetup.bEnabledScanAOIBySiteAlarm==true && bScanAOIBySiteFailCont==true)
                {
                    Task=4100;
                }
                else if(tAOISetup.bEnabledScanAOIByArmAlarm==true && bScanAOIByArmFailCont==true)
                {
                    Task=4200;
                }
                else
                {
                    Task=5000;
                }
            }
            break;
        case 4100:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(iScanAOIBySiteTotal[i][j]>=tAOISetup.iScanAOIAlarmCountBySite)
                    {
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                    }
                }
            }

            ret=ShowErrorMessage("WAR0880", K_RETRY|K_SKIP, MMSystem, false, ErrPart);  //Ifor 20200909 add:AOI Fail Retry
            if(ret==K_SKIP)
            {
                bScanAOIBySiteFailCont=false;
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        iScanAOIBySiteTotal[i][j]=0;
                    }
                }

                if(tAOISetup.bEnabledScanAOIByArmAlarm==true && bScanAOIByArmFailCont==true)
                    Task=4200;
                else
                    Task=5000;
            }
            else if(ret==K_RETRY)
            {
                Task=6000;
            }
            break;
        case 4200:
            if(bScanAOIByArmDouChk[0])
                ret=ShowErrorMessage("WAR0881", K_RETRY|K_SKIP, MMSystem);      //Ifor 20200909 add:AOI Fail Retry
            else
                ret=ShowErrorMessage("WAR0882", K_RETRY|K_SKIP, MMSystem);      //Ifor 20200909 add:AOI Fail Retry

            if(ret==K_SKIP)
            {
                bScanAOIByArmFailCont=false;
                if(bScanAOIByArmDouChk[0])
                {
                    iScanAOIByArmTotal[0]=0;
                    bScanAOIByArmDouChk[0]=false;
                }
                else
                {
                    iScanAOIByArmTotal[1]=0;
                    bScanAOIByArmDouChk[1]=false;
                }
                Task=5000;
            }
            else if(ret==K_RETRY)
            {
                Task=6000;
            }

            break;
        case 5000:
            bResult=true;
            break;
        case 6000:
            if(MoveOutArmToAutoSafe())
            {
                bRunAOI=true;
                OutArmSuck.iAOIStation[iRow][iCol]=0;
                iScanLGAStep=0;                                                 //Ifor 20211022 add: AOI LGA Mode
                Task=1000;
            }
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitialTopScanAOITask()
{
    iTopScanAOITask=1;
}
//------------------------------------------------------------------------------
bool DoTopScanAOIFunction()                                                     //Ifor 20191210 : add Scanner AOI
{
    int &Task=iTopScanAOITask;
    bool bResult=false;
    bool bflag=false;
    int ret;
    AnsiString ErrPart="";
    static int iRow=0, iCol=0;
    static int iScanSite=-1;
    int iCheckSite=0;
    bool bError=false;

    switch(Task)
    {
        case 1:
            bRunAOI=false;
            Task=100;
            break;
        case 100:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_Top &&
                       bRunAOI==false)
                    {
                        iTopScanAOIIntervalCounter++;
                        iCheckSite=(i*4)+j;
                        if(TestIF_File.iTestMode==SingleSite && iTopScanAOIIntervalCounter>=ScannerAOIIF.iTopIntervalCounter+1)
                        {
                            iTopScanAOIIntervalCounter=0;
                            iScanSite=iCheckSite;
                            iRow=i;
                            iCol=j;
                            bRunAOI=true;
                            break;
                        }
                        else if(TestIF_File.iTestMode==DualSite && ScannerAOIIF.iTopIntervalCounter==1 && TestIF_File.iShuttleMode==0)   //Ifor 20200218 50%抽檢
                        {
                            if(iTopScanAOIIntervalCounter>=9999)                //Ifor 20200218 add 為了讓第一顆IC先測試AOI初始值9999
                            {
                                iTopScanAOIIntervalCounter=1;
                            }
                            if(iTopScanAOIIntervalCounter == 1 ||
                               iTopScanAOIIntervalCounter == 3 ||
                               iTopScanAOIIntervalCounter == 6 ||
                               iTopScanAOIIntervalCounter == 8 )                //Ifor 20200218 50%抽檢
                            {
                                if(iTopScanAOIIntervalCounter==8)
                                {
                                    iTopScanAOIIntervalCounter=0;
                                }
                                iScanSite=iCheckSite;
                                iRow=i;
                                iCol=j;
                                bRunAOI=true;
                                break;
                            }
                            else
                            {
                                OutArmSuck.iAOIStation[i][j]=eAOI_Top;
                            }
                        }
                        else if(iTopScanAOIIntervalCounter>=ScannerAOIIF.iTopIntervalCounter+1 && iScanSite!=iCheckSite)
                        {
                            iTopScanAOIIntervalCounter=0;
                            iScanSite=iCheckSite;
                            iRow=i;
                            iCol=j;
                            bRunAOI=true;
                            break;
                        }
                        else
                        {
                            OutArmSuck.iAOIStation[i][j]=eAOI_Top;
                        }
                    }
                }
            }
            if(bRunAOI==true)
            {
                SetActiveSuck();
                Task=1000;
            }
            else
            {
                return true;
            }
            break;
        case 1000:
            if(DoMoveXY_TopView())
            {
                Task=1100;
            }
            break;
        case 1100:
            if(DoMoveZ_TopView(Prod.iTopView_Place))
            {
                if((AccessLevel>=1 ||
                    CosFunction.bOPCanPressStepAndTStart) &&                    //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                   (Sen[SnRKManualStep].IsOn() ||
                    fOffSet->UseAutoOffsetFunction("Top View")))                //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoTopScanAOIFunction 1100");
                    Task=1200;
                }
                else
                {
                    Task=1500;
                }
            }
            break;
        case 1200:
            if(MoveOutArmToAutoSafe())
            {
                Task=1000;
            }
            break;
        case 1500:
            if(DoPlace_TopView())
            {
                Task=1300;
            }
            break;
        case 1300:
            if(MoveOutArmToAutoSafe())
            {
                Task=1400;
            }
            break;
        case 1400:
            if(DoMoveXY_TopViewSafePos())
            {
                InitScannerAOIInspection();
                Task=2000;
                if(LastSet.iRealDummy!=REALLY)                                  //Ifor 20200825 add:Scan AOI Dummy 不檢測
                {
                    Task=3000;
                    bAOIPassFail[0][iRow][iCol]=true;
                    OutArmSuck.iAOIStation[iRow][iCol]=eAOI_Top;
                }
            }
            break;
        case 2000:
            if(DoTopScanAOIFunction_Inspection(iRow, iCol))
            {
                Task=3000;
            }
            break;
        case 3000:
            if(MoveOutArmToAutoSafe())
            {
                Task=3100;
            }
            break;
        case 3100:
            if(DoMoveXY_TopView())
            {
                Task=3200;
            }
            break;
        case 3200:
            if(DoMoveZ_TopView(Prod.iTopView_Pick))
            {
                if((AccessLevel>=1 ||
                    CosFunction.bOPCanPressStepAndTStart) &&                    //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                   (Sen[SnRKManualStep].IsOn() ||
                    fOffSet->UseAutoOffsetFunction("Top View")))                //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoTopScanAOIFunction 3200");
                    Task=3250;
                }
                else
                {
                    Task=3300;
                }
            }
            break;
        case 3250:
            if(MoveOutArmToAutoSafe())
            {
                Task=3100;
            }
            break;
        case 3300:
            if(DoPick_TopView(bError))
            {
                Task=3400;
            }
            else if(bError)
            {
                Task=3310;
            }
            break;
        case 3310:
            if(MoveOutArmToAutoSafe())
            {
                Task=3320;
            }
            break;
        case 3320:
            ret=ShowErrorMessage("JAM0201", K_RETRY|K_SKIP, MOutArmX, false, NULL);

            if(ret==K_SKIP)
            {
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(OutArmSuck.Suck[i][j].Error)
                        {
                            OutArmSuck.SetItemData(i, j, NULL_IC);
                            AOIKit.SetItemData(i, j, NULL_IC);
                            Task=3400;
                        }
                    }
                }
            }
            else if(ret==K_RETRY)
            {
                Task=3200;
            }
            break;
        case 3400:
            if(MoveOutArmToAutoSafe())
            {
                Task=4000;
            }
            break;
        case 4000:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_Top)
                    {
                        bflag=true;
                        break;
                    }
                }
            }
            if(bflag)
            {
                bRunAOI=false;
                Task=100;
            }
            else
            {
                if(tAOISetup.bEnabledTopScanAOIBySiteAlarm==true && bTopScanAOIBySiteFailCont==true)
                {
                    Task=4100;
                }
                else if(tAOISetup.bEnabledTopScanAOIByArmAlarm==true && bTopScanAOIByArmFailCont==true)
                {
                    Task=4200;
                }
                else
                {
                    Task=5000;
                }
            }
            break;
        case 4100:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(iTopScanAOIBySiteTotal[i][j]>=tAOISetup.iTopScanAOIAlarmCountBySite)
                    {
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                    }
                }
            }

            ret=ShowErrorMessage("WAR0880", K_RETRY|K_SKIP, MMSystem, false, ErrPart);
            if(ret==K_SKIP)
            {
                bScanAOIBySiteFailCont=false;
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        iTopScanAOIBySiteTotal[i][j]=0;
                    }
                }
                if(tAOISetup.bEnabledTopScanAOIByArmAlarm==true && bTopScanAOIByArmFailCont==true)
                    Task=4200;
                else
                    Task=5000;
            }
            else
            {
                Task=6000;
            }
            break;
        case 4200:
            if(bTopScanAOIByArmDouChk[0])
                ret=ShowErrorMessage("WAR0885", K_RETRY|K_SKIP, MMSystem);
            else
                ret=ShowErrorMessage("WAR0886", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_SKIP)
            {
                bTopScanAOIByArmFailCont=false;
                if(bTopScanAOIByArmDouChk[0])
                {
                    iTopScanAOIByArmTotal[0]=0;
                    bTopScanAOIByArmDouChk[0]=false;
                }
                else
                {
                    iTopScanAOIByArmTotal[1]=0;
                    bTopScanAOIByArmDouChk[1]=false;
                }
                Task=5000;
            }
            else
            {
                Task=6000;
            }
            break;
        case 5000:
            bResult=true;
            break;
        case 6000:
            if(MoveOutArmToAutoSafe())
            {
                bRunAOI=true;
                OutArmSuck.iAOIStation[iRow][iCol]=0;
                Task=6010;
            }
            break;
        case 6010:

            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitAOIFunction()
{
    iAOITask=1;
    bAOI_Fail_Unit=false;

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            OutArmSuck.iAOIStation[i][j]=0;
        }
    }
}
//------------------------------------------------------------------------------
bool DoAOIFunction()
{
    int &Task=iAOITask;
    bool bResult=false;

    switch(Task)
    {
        case 1:
            if(tAOISetup.tBGAView.bEnabled)
            {
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        bAOIPassFail[0][i][j]=false;
                    }
                }
                Task=1000;
            }
            else if(tAOISetup.tPADView.bEnabled)
            {
                Task=2000;
            }
            else if(USE_Scanner_AOI_Inspection==true && ScannerAOIIF.iEnableScannerMode!=0)
            {
                Task=6000;
            }
            else if(USE_Top_Scanner_AOI_Inspection==true && ScannerAOIIF.iEnableTopScannerMode!=0)  //Ifor 20200902 add: TFAMD Top AOI
            {
                Task=6500;
            }
            else
            {
                Task=4000;
            }
            break;
        case 1000:
            InitBGAViewFunction();
            Task=1100;
            break;
        case 1100:
            if(DoBGAViewFunction())
            {
                if(tAOISetup.tPADView.bEnabled)
                {
                    Task=2000;
                }
                else
                {
                    Task=4000;
                }
            }
            break;
        case 2000:
            InitDoPADViewFunction();
            Task=2100;
            break;
        case 2100:
            if(DoPADViewFunction())
            {
                Task=4000;
            }
            break;
        case 3000:
            InitDoTopViewFunction();
            Task=3100;
            break;
        case 3100:
            if(DoTopViewFunction())
            {
                Task=4000;
            }
            break;
        case 4000:
            bResult=true;
            bPickSH1Flag=false;
            bPickSH2Flag=false;
            break;
        case 6000:
            InitialScanAOITask();
            Task=6100;
            break;
        case 6100:
            if(DoScanAOIFunction())
            {
                if(USE_Top_Scanner_AOI_Inspection==true && ScannerAOIIF.iEnableTopScannerMode!=0)  //Ifor 20200902 add: TFAMD Top AOI
                    Task=6500;
                else
                    Task=4000;
            }
            break;
        case 6500:
            InitialTopScanAOITask();
            Task=6600;
            break;
        case 6600:
            if(DoTopScanAOIFunction())
            {
                Task=4000;
            }
            break;
        default:
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
int GetAOIFailBin(int iRow, int iCol)                                                             //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
{
    //0: Auto1  1: Auto2    2: Auto3
    //3: Fix1   4: Fix2     5: Fix3     6: Fix4     7: Fix5     8: Fix6

    int iWitchScannerIfError;
    int iBin;
    if(ScannerAOIIF.iAOIFailBinType==0)                                         //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    {
        //舊規則
        iBin=ScannerAOIIF.ScannerIfError+4;                                     //Fix 2 = 0+4
        iWitchScannerIfError=ScannerAOIIF.ScannerIfError;
    }
    else //新規則                                                               //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    {
        //判斷outarm上的IC在測試後是pass/fail
        if(Prod.bIsPassBin[OutArmSuck.iBinData[iRow][iCol]]==true)
        {
            iBin=ScannerAOIIF.ScannerIfErrorAndTestPass+4;
            iWitchScannerIfError=ScannerAOIIF.ScannerIfErrorAndTestPass;
        }
        else
        {
            iBin=ScannerAOIIF.ScannerIfError+4;
            iWitchScannerIfError=ScannerAOIIF.ScannerIfError;
        }
    }

    if(iWitchScannerIfError==2)
    {
        if(TrayForm.bTrayUpDownSet[eFix2]==true)
        {
            iBin=7;                                             //Fix5 = 2 + 4 + 1 = 7
        }
        else
        {
            iBin=4;                                             //Fix2 = 2 + 4 - 2 = 4
        }
    }
    else if(iWitchScannerIfError==3)
    {
        if(TrayForm.bTrayUpDownSet[eFix3]==true)
        {
            iBin=8;                                             //Fix6 = 3 + 4 + 1 = 8
        }
        else
        {
            iBin=5;                                             //Fix3 = 3 + 4- 2 = 5
        }
    }
    return iBin;
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString S="";

    if(CheckFailBin()==false)
    {
        ShowMyMessage("AOI Fail Bin設定與Bin Sel Pass Bin別相同");
    }

    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\AOI.Data", DataPath, S);

    WriteIniData(szDir, "SETTING", "EnabledTopView", cbEnabledTopView->Checked);
    WriteIniData(szDir, "SETTING", "FailBinTopView", edt_FailTopView->Text);
    WriteIniData(szDir, "SETTING", "TimeOutTopView", edt_TimeoutTopView->Text);
    WriteIniData(szDir, "SETTING", "StartDelayTimeTopView", edt_SDelayTopView->Text);

    WriteIniData(szDir, "SETTING", "EnabledPADView", cbEnabledPADView->Checked);
    WriteIniData(szDir, "SETTING", "FailBinPADView", edt_FailPADView->Text);
    WriteIniData(szDir, "SETTING", "TimeOutPADView", edt_TimeoutPADView->Text);
    WriteIniData(szDir, "SETTING", "StartDelayTimePADView", edt_SDelayPADView->Text);

    WriteIniData(szDir, "SETTING", "EnabledBGAView", cbEnabledBGAView->Checked);
    WriteIniData(szDir, "SETTING", "FailBinBGAView", edt_FailBGAView->Text);
    WriteIniData(szDir, "SETTING", "TimeOutBGAView", edt_TimeoutBGAView->Text);
    WriteIniData(szDir, "SETTING", "StartDelayTimeBGAView", edt_SDelayBGAView->Text);

    WriteIniData(szDir, "SETTING", "AlarmCount", edt_AlarmCount->Text);

    WriteIniData(szDir, "SETTING", "EnabledailRunFunction", cbBGAFailRun->Checked);
    WriteIniData(szDir, "SETTING", "EnabledAutoSkipBGA",    cbAutoSkipBGA->Checked);
    WriteIniData(szDir, "SETTING", "EnabledAutoSkipPadPkg", cbAutoSkipPadPkg->Checked);

    WriteIniData(szDir, "SETTING", "EnabledAOINoSort",      cbAOINoSort->Checked);   //wei 20161207

    WriteIniData(szDir, "SETTING", "TestPassFailBinBGAPADView", edt_PassFailBGAPADView->Text);    //wei  20160706
    WriteIniData(szDir, "SETTING", "TestFailFailBinBGAPADView", edt_FailFailBGAPADView->Text);    //wei  20160706

    WriteIniData(szDir, "SETTING", "EnabledTesterFailBin",   cbTesterFailBin->Checked);     //wei  20160706
    WriteIniData(szDir, "SETTING", "EnabledContinueAlarm",   cb_VitContiuneAlarm->Checked); //Alick 20160712
    WriteIniData(szDir, "SETTING", "EnabledBySiteBGAView",   cb_AlarmBySite->Checked);      //Alick 20160712
    WriteIniData(szDir, "SETTING", "EnabledByArmBGAView",    cb_AlarmByArm->Checked);       //Alick 20160712
    WriteIniData(szDir, "SETTING", "ContAlarmCountBySite",   edt_ConAlarmConut->Text);      //Alick 20160712
    WriteIniData(szDir, "SETTING", "ContAlarmCountByArm",    edt_ConAlarmConut1->Text);     //Alick 20160718
    WriteIniData(szDir, "SETTING", "EnabledBySitePADView",   cb_AlarmBySite1->Checked);     //Alick 20160712
    WriteIniData(szDir, "SETTING", "EnabledByArmPADView",    cb_AlarmByArm1->Checked);      //Alick 20160712

    //Ifor 20191225 : add Scan AOI Alarm
    //==>
    WriteIniData(szDir, "SETTING", "bEnabledScanAOIBySiteAlarm",    cb_ScanAOIAlarmBySite->Checked);
    WriteIniData(szDir, "SETTING", "bEnabledScanAOIByArmAlarm",     cb_ScanAOIAlarmByArm->Checked);
    WriteIniData(szDir, "SETTING", "iScanAOIAlarmCountBySite",      edt_ScanAOIAlarmCountBySite->Text);
    WriteIniData(szDir, "SETTING", "iScanAOIAlarmCountByArm",       edt_ScanAOIAlarmCountByArm->Text);

    WriteIniData(szDir, "SETTING", "bEnabledScanAOIUnUseFailBin",   cb_ScanAOIUnUseFailBin->Checked);   //Ifor 20200825 add:TF-AMD AOI Error 不丟至Fail Bin
    //<==
    //Ifor 20191225 : add Scan AOI Alarm

    for(int i=0; i<MAX_ARM_Row; i++)                                                              //Alick 20160713
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            S.sprintf("Dut %s", IndexSuckName[i][j]);
            WriteIniData(szDir, "DutOnOff_BGAView", S, bVitroxBGAViewUse[i][j]);
            bVitroxBGAViewMap[i][j]=bVitroxBGAViewUse[i][j];
        }
    }

    for(int i=0; i<MAX_ARM_Row; i++)                                                              //Alick 20160714
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            S.sprintf("Dut %s", IndexSuckName[i][j]);
            WriteIniData(szDir, "DutOnOff_PADView", S, bVitroxPADViewUse[i][j]);
            bVitroxPADViewMap[i][j]=bVitroxPADViewUse[i][j];
        }
    }

    //Ifor 20190724 add AMD AOI
    //==>
    WriteIniData(szDir, "SETTING", "iEnableScannerMode",        rgScannerMode->ItemIndex);
    WriteIniData(szDir, "SETTING", "ScannerReadTimeout",        edtScannerReadTimeout->Text);

    WriteIniData(szDir, "SETTING", "iIntervalCounter",          edtAOIIntervalCounter->Text);
    WriteIniData(szDir, "SETTING", "iRetryCounter",             edtAOIRetryCounter->Text);
    WriteIniData(szDir, "SETTING", "iAOIFailBinType",           rgAOIFailBinType->ItemIndex);       //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    WriteIniData(szDir, "SETTING", "iScanAOIFialBin",           cbAOIFial->ItemIndex);
    WriteIniData(szDir, "SETTING", "iAOIFialAndTestPass",       cbAOIFialAndTestPass->ItemIndex);   //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項

    WriteIniData(szDir, "SETTING", "bBDTotalFunction",          cbBDByTotal->Checked);
    WriteIniData(szDir, "SETTING", "bBDTotalContiFunction",     cbBDByTotalConti->Checked);
    WriteIniData(szDir, "SETTING", "bBDSiteFunction",           cbBDBySite->Checked);
    WriteIniData(szDir, "SETTING", "bBDSiteContiFunction",      cbBDBySiteConti->Checked);
    WriteIniData(szDir, "SETTING", "bDBAlramAutoResetCount",    cbDBAlarmAutoReset->Checked);

    WriteIniData(szDir, "SETTING", "iBDTotalCounter",           edtAOIBDCounterTotal->Text);
    WriteIniData(szDir, "SETTING", "iBDTotalContiCounter",      edtAOIBDCounterTotalConti->Text);
    WriteIniData(szDir, "SETTING", "iBDSiteCounter",            edtAOIBDCounterSite->Text);
    WriteIniData(szDir, "SETTING", "iBDSiteContiCounter",       edtAOIBDCounterSiteConti->Text);

    WriteIniData(szDir, "SETTING", "iBDTotalCounterLast",       pnlAOIBDTotalLast->Caption);
    WriteIniData(szDir, "SETTING", "iBDTotalContiCounterLast",  pnlAOIBDTotalContiLast->Caption);
    WriteIniData(szDir, "SETTING", "iBDSite1CounterLast",       pnlAOIBDSite1Last->Caption);
    WriteIniData(szDir, "SETTING", "iBDSite2CounterLast",       pnlAOIBDSite2Last->Caption);
    WriteIniData(szDir, "SETTING", "iBDSite1ContiCounterLast",  pnlAOIBDSite1ContiLast->Caption);
    WriteIniData(szDir, "SETTING", "iBDSite2ContiCounterLast",  pnlAOIBDSite2ContiLast->Caption);

    WriteIniData(szDir, "SETTING", "StartDelayTimeScanAOIView", edt_SDelayScanAOI->Text);
    WriteIniData(szDir, "SETTING", "TimeOutScanAOIView", edt_TimeoutScanAOI->Text);

    WriteIniData(szDir, "RS232",   "Device",                    cbDevice->Text);
    WriteIniData(szDir, "RS232",   "Baud Rate",                 cbBaudRate->Text);
    WriteIniData(szDir, "RS232",   "Byte Size",                 cbByteSize->Text);
    WriteIniData(szDir, "RS232",   "Stop Bit",                  cbStopBit->Text);
    WriteIniData(szDir, "RS232",   "Parity",                    cbParity->Text);

    WriteIniData(szDir, "SETTING", "ScannerICGain",             edtScannerICGain->Text);        //Ifor 20200525 add:依據Tray Y Pitch Gain 決定不使用範圍

    WriteIniData(szDir, "SETTING", "bEnabledScanAOIUseLGAMode", cb_ScanAOIUseLGAMode->Checked); //Ifor 20211022 add: AOI LGA Mode
    WriteIniData(szDir, "SETTING", "ScannerLGAWaitTime",        edtScannerLGAWaitTime->Text);   //Ifor 20211022 add: AOI LGA Mode

    WriteIniData(szDir, "SETTING", "bLGAModeDirection", cb_LGAModeDirection->Checked);          //Ifor 20230202 add: AOI LGA Mode Direction
    //<==
    //Ifor 20190724 add AMD AOI

    //Ifor 20211027 add:AOI Tray Edit
    //==>
    AnsiString AsTrayData="";

    for(int k=0; k<iQuotient; k++)                                              //KenHsieh 20220830 : 修改add:AOI Tray Edit
    {
        AsTrayData="";
        if(iRemainder==0 || k!=iQuotient-1)
        {
            for(int i=0; i<10; i++)
            {
                for(int j=0; j<_MAX_ROW_ITEM; j++)
                {
                    AsTrayData=AsTrayData+MOT[MMScanAOI].Tray.Data[i+(k*10)][j]+",";
                }
            }
        }
        else
        {
            for(int i=0; i<iRemainder; i++)
            {
                for(int j=0; j<_MAX_ROW_ITEM; j++)
                {
                    AsTrayData=AsTrayData+MOT[MMScanAOI].Tray.Data[i+(k*10)][j]+",";
                }
            }
        }

        S.sprintf("TrayDisable_%d", k+1);
        WriteIniData(szDir, "AOITRAY", S, AsTrayData);                          //Ifor 20211022 add: AOI LGA Mode
    }
    //<==
    //Ifor 20211027 add:AOI Tray Edit

    //Ifor 20200902 add: TFAMD Top AOI
    //==>
    WriteIniData(szDir, "SETTING", "iEnableTopScannerMode",     rgTopScannerMode->ItemIndex);
    WriteIniData(szDir, "SETTING", "iTopIntervalCounter",       edtTopAOIIntervalCounter->Text);
    WriteIniData(szDir, "SETTING", "iTopRetryCounter",          edtTopAOIRetryCounter->Text);
    WriteIniData(szDir, "SETTING", "iTopScanAOIFialBin",        cbTopAOIFial->ItemIndex);

    WriteIniData(szDir, "SETTING", "bEnabledTopScanAOIBySiteAlarm", cb_TopScanAOIAlarmBySite->Checked);
    WriteIniData(szDir, "SETTING", "iTopScanAOIAlarmCountBySite",   edt_TopScanAOIAlarmCountBySite->Text);
    WriteIniData(szDir, "SETTING", "bEnabledTopScanAOIByArmAlarm",  cb_TopScanAOIAlarmByArm->Checked);
    WriteIniData(szDir, "SETTING", "iTopScanAOIAlarmCountByArm",    edt_TopScanAOIAlarmCountByArm->Text);
    WriteIniData(szDir, "SETTING", "StartDelayTimeScanAOIView",     edt_SDelayTopScanAOI->Text);
    WriteIniData(szDir, "SETTING", "TimeOutTopScanAOIView",         edt_TimeoutTopScanAOI->Text);
    WriteIniData(szDir, "SETTING", "TopScannerReadTimeout",         edtTopScannerReadTimeout->Text);
    WriteIniData(szDir, "SETTING", "bEnabledTopScanAOIUnUseFailBin",cb_TopScanAOIUnUseFailBin->Checked);

    WriteIniData(szDir, "RS232",   "Top Device",                    cbTopDevice->Text);
    WriteIniData(szDir, "RS232",   "Top Baud Rate",                 cbTopBaudRate->Text);
    WriteIniData(szDir, "RS232",   "Top Byte Size",                 cbTopByteSize->Text);
    WriteIniData(szDir, "RS232",   "Top Stop Bit",                  cbTopStopBit->Text);
    WriteIniData(szDir, "RS232",   "Top Parity",                    cbTopParity->Text);
    //<==
    //Ifor 20200902 add: TFAMD Top AOI
    fAOI_ReadFile();
    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data

    if(USE_Scanner_AOI_Inspection==true)
    {
        if(ScannerAOIIF.iEnableScannerMode==0)
        {
            FrmAOI->spbStopCom->Click();
        }
        else
        {
            FrmAOI->spbStartCom->Click();
        }
    }

    if(USE_Top_Scanner_AOI_Inspection==true)                                    //Ifor 20200902 add: TFAMD Top AOI
    {
        if(ScannerAOIIF.iEnableTopScannerMode==0)
        {
            FrmAOI->spbStopCom->Click();
        }
        else
        {
            FrmAOI->spbStartCom->Click();
        }
    }
}
//------------------------------------------------------------------------------
void TFrmAOI::fAOI_ReadFile()
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";

    AnsiString tmpS="";
    szDir.sprintf("%s%s\\AOI.Data", DataPath, S);

    tAOISetup.tTopView.bEnabled         =ReadIniData(szDir, "SETTING", "EnabledTopView", false);
    tAOISetup.tTopView.iFailBin         =ReadIniData(szDir, "SETTING", "FailBinTopView", 15);
    tAOISetup.tTopView.iTimeOut         =ReadIniData(szDir, "SETTING", "TimeOutTopView", 3);
    tAOISetup.tTopView.iStartDelayTime  =ReadIniData(szDir, "SETTING", "StartDelayTimeTopView", 10);

    tAOISetup.tPADView.bEnabled         =ReadIniData(szDir, "SETTING", "EnabledPADView", false);
    tAOISetup.tPADView.iFailBin         =ReadIniData(szDir, "SETTING", "FailBinPADView", 15);
    tAOISetup.tPADView.iTimeOut         =ReadIniData(szDir, "SETTING", "TimeOutPADView", 3);
    tAOISetup.tPADView.iStartDelayTime  =ReadIniData(szDir, "SETTING", "StartDelayTimePADView", 10);

    tAOISetup.tBGAView.bEnabled         =ReadIniData(szDir, "SETTING", "EnabledBGAView", false);
    tAOISetup.tBGAView.iFailBin         =ReadIniData(szDir, "SETTING", "FailBinBGAView", 15);
    tAOISetup.tBGAView.iTimeOut         =ReadIniData(szDir, "SETTING", "TimeOutBGAView", 3);
    tAOISetup.tBGAView.iStartDelayTime  =ReadIniData(szDir, "SETTING", "StartDelayTimeBGAView", 10);

    tAOISetup.tBGAPADView.iTestPassFailBin =ReadIniData(szDir, "SETTING", "TestPassFailBinBGAPADView", 15);         //wei  20160706
    tAOISetup.tBGAPADView.iTestFailFailBin =ReadIniData(szDir, "SETTING", "TestFailFailBinBGAPADView", 15);         //wei  20160706
    tAOISetup.tTesterFailBin.bEnabled   =ReadIniData(szDir, "SETTING", "EnabledTesterFailBin",   false);            //wei  20160706

    tAOISetup.iAlarmCount               =ReadIniData(szDir, "SETTING", "AlarmCount", 1);
    tAOISetup.bEnabledContinueAlarm     =ReadIniData(szDir, "SETTING", "EnabledContinueAlarm", false);              //Alick 20160712 是否使用連續Fail
    tAOISetup.bEnabledBySiteBGAView     =ReadIniData(szDir, "SETTING", "EnabledBySiteBGAView", false);              //Alick 20160712 BGA Fail by Site
    tAOISetup.bEnabledByArmBGAView      =ReadIniData(szDir, "SETTING", "EnabledByArmBGAView",  false);              //Alick 20160712 BGA Fail by Arm
    tAOISetup.iContAlarmCountBySite     =ReadIniData(szDir, "SETTING", "ContAlarmCountBySite", 3);                  //Alick 20160712 Fail by Site Count
    tAOISetup.iContAlarmCountByArm      =ReadIniData(szDir, "SETTING", "ContAlarmCountByArm",  3);                  //Alick 20160718 Fail by Arm Count
    tAOISetup.bEnabledBySitePADView     =ReadIniData(szDir, "SETTING", "EnabledBySitePADView", false);              //Alick 20160712 PAD Fail by Site
    tAOISetup.bEnabledByArmPADView      =ReadIniData(szDir, "SETTING", "EnabledByArmPADView",  false);              //Alick 20160712 PAD Fail bu Arm

    //Ifor 20191225 : add Scan AOI Alarm
    tAOISetup.bEnabledScanAOIBySiteAlarm    =ReadIniData(szDir, "SETTING", "bEnabledScanAOIBySiteAlarm", false);
    tAOISetup.bEnabledScanAOIByArmAlarm     =ReadIniData(szDir, "SETTING", "bEnabledScanAOIByArmAlarm",  false);
    tAOISetup.iScanAOIAlarmCountBySite      =ReadIniData(szDir, "SETTING", "iScanAOIAlarmCountBySite", 3);
    tAOISetup.iScanAOIAlarmCountByArm       =ReadIniData(szDir, "SETTING", "iScanAOIAlarmCountByArm",  3);

    tAOISetup.bEnabledScanAOIUnUseFailBin   =ReadIniData(szDir, "SETTING", "bEnabledScanAOIUnUseFailBin", false);   //Ifor 20200825 add:TF-AMD AOI Error 不丟至Fail Bin
    //Ifor 20191225 : add Scan AOI Alarm

    for(int i=0; i<MAX_ARM_Row; i++)                                                                                //Alick 20160713
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            S.sprintf("Dut %s", IndexSuckName[i][j]);
            bVitroxBGAViewUse[i][j]=ReadIniData(szDir, "DutOnOff_BGAView", S, bVitroxBGAViewMap[i][j]);
            bVitroxBGAViewMap[i][j]=bVitroxBGAViewUse[i][j];
        }
    }

    for(int i=0; i<MAX_ARM_Row; i++)                                                                            //Alick 20160714
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            S.sprintf("Dut %s", IndexSuckName[i][j]);
            bVitroxPADViewUse[i][j]=ReadIniData(szDir, "DutOnOff_PADView", S, bVitroxPADViewMap[i][j]);
            bVitroxPADViewMap[i][j]=bVitroxPADViewUse[i][j];
        }
    }

    if(USE_AOI_Inspection)
        tAOISetup.bEnabledAOI = tAOISetup.tTopView.bEnabled || tAOISetup.tPADView.bEnabled || tAOISetup.tBGAView.bEnabled;
    else
        tAOISetup.bEnabledAOI = false;

    tAOISetup.tFailRunFunction.bEnabled = ReadIniData(szDir, "SETTING", "EnabledailRunFunction", false);
    tAOISetup.tAutoSkipBGA.bEnabled     = ReadIniData(szDir, "SETTING", "EnabledAutoSkipBGA",    false);
    tAOISetup.tAutoSkipPadPkg.bEnabled  = ReadIniData(szDir, "SETTING", "EnabledAutoSkipPadPkg", false);

    tAOISetup.tAOINoSort.bEnabled       = ReadIniData(szDir, "SETTING", "EnabledAOINoSort", false);   //wei 20161207

    ScannerAOIIF.iEnableScannerMode      =ReadIniData(szDir, "SETTING", "iEnableScannerMode", 0);                               //Ifor 20190724 add AMD AOI
    ScannerAOIIF.fScannerReadTimeout     =ReadIniData(szDir, "SETTING", "ScannerReadTimeout", 100);
    ScannerAOIIF.iIntervalCounter        =ReadIniData(szDir, "SETTING", "iIntervalCounter", 0);
    ScannerAOIIF.iRetryCounter           =ReadIniData(szDir, "SETTING", "iRetryCounter", 0);
    ScannerAOIIF.iAOIFailBinType         =ReadIniData(szDir, "SETTING", "iAOIFailBinType", 0);                              //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    ScannerAOIIF.ScannerIfError          =ReadIniData(szDir, "SETTING", "iScanAOIFialBin", 0);
    ScannerAOIIF.ScannerIfErrorAndTestPass=ReadIniData(szDir,"SETTING", "iAOIFialAndTestPass", 0);                          //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    ScannerAOIIF.iBallDamageType         =ReadIniData(szDir, "SETTING", "iBallDamageType", 0);
    ScannerAOIIF.bBDTotalFunction        =ReadIniData(szDir, "SETTING", "bBDTotalFunction", false);
    ScannerAOIIF.bBDTotalContiFunction   =ReadIniData(szDir, "SETTING", "bBDTotalContiFunction", false);
    ScannerAOIIF.bBDSiteFunction         =ReadIniData(szDir, "SETTING", "bBDSiteFunction", false);
    ScannerAOIIF.bBDSiteContiFunction    =ReadIniData(szDir, "SETTING", "bBDSiteContiFunction", false);
    ScannerAOIIF.bDBAlramAutoResetCount  =ReadIniData(szDir, "SETTING", "bDBAlramAutoResetCount", false);

    ScannerAOIIF.iBDTotalCounter         =ReadIniData(szDir, "SETTING", "iBDTotalCounter", 0);
    ScannerAOIIF.iBDTotalContiCounter    =ReadIniData(szDir, "SETTING", "iBDTotalContiCounter", 0);
    ScannerAOIIF.iBDSiteCounter          =ReadIniData(szDir, "SETTING", "iBDSiteCounter", 0);
    ScannerAOIIF.iBDSiteContiCounter     =ReadIniData(szDir, "SETTING", "iBDSiteContiCounter", 0);
    ScannerAOIIF.iBDTotalCounterLast     =ReadIniData(szDir, "SETTING", "iBDTotalCounterLast", 0);
    ScannerAOIIF.iBDTotalContiCounterLast=ReadIniData(szDir, "SETTING", "iBDTotalContiCounterLast", 0);
    ScannerAOIIF.iBDSite1CounterLast     =ReadIniData(szDir, "SETTING", "iBDSite1CounterLast", 0);
    ScannerAOIIF.iBDSite2CounterLast     =ReadIniData(szDir, "SETTING", "iBDSite2CounterLast", 0);
    ScannerAOIIF.iBDSite1ContiCounterLast=ReadIniData(szDir, "SETTING", "iBDSite1ContiCounterLast", 0);
    ScannerAOIIF.iBDSite2ContiCounterLast=ReadIniData(szDir, "SETTING", "iBDSite2ContiCounterLast", 0);

    ScannerAOIIF.iStartDelayTime        =ReadIniData(szDir, "SETTING", "StartDelayTimeScanAOIView", 10);
    ScannerAOIIF.iTimeOut               =ReadIniData(szDir, "SETTING", "TimeOutScanAOIView", 3);

    ScannerAOIIF.sSCANNER_ComPort       =ReadIniData(szDir, "RS232", "Device", AnsiString(""));
    ScannerAOIIF.sSCANNER_BaudRate      =ReadIniData(szDir, "RS232", "Baud Rate", AnsiString("57600"));
    ScannerAOIIF.sSCANNER_ByteSize      =ReadIniData(szDir, "RS232", "Byte Size", AnsiString("8"));
    ScannerAOIIF.sSCANNER_StopBit       =ReadIniData(szDir, "RS232", "Stop Bit", AnsiString("1"));
    ScannerAOIIF.sSCANNER_Parity        =ReadIniData(szDir, "RS232", "Parity", AnsiString("None"));

    ScannerAOIIF.fScannerICGain         =CheckRange(ReadIniData(szDir, "SETTING", "ScannerICGain", 1.0),1.5 ,0.5);          //Ifor 20200525 add:依據Tray Y Pitch Gain 決定不使用範圍
    edtScannerICGain->Text              =FormatFloat("0.00", ScannerAOIIF.fScannerICGain);

    ScannerAOIIF.bScanAOIUseLGAMode     =ReadIniData(szDir, "SETTING", "bEnabledScanAOIUseLGAMode", false);                 //Ifor 20211022 add: AOI LGA Mode
    ScannerAOIIF.fLGAWaitTime           =CheckRange(ReadIniData(szDir, "SETTING", "ScannerLGAWaitTime", 2.0),20.0 ,1.0);    //Ifor 20211022 add: AOI LGA Mode
    ScannerAOIIF.bLGAModeDirection      =ReadIniData(szDir, "SETTING", "bLGAModeDirection", true);                          //Ifor 20230202 add: AOI LGA Mode Direction

    ScannerAOIIF.iEnableTopScannerMode      =ReadIniData(szDir, "SETTING", "iEnableTopScannerMode", 0);                     //Ifor 20200902 add: TFAMD Top AOI
    ScannerAOIIF.fTopScannerReadTimeout     =ReadIniData(szDir, "SETTING", "TopScannerReadTimeout", 100);
    ScannerAOIIF.iTopIntervalCounter        =ReadIniData(szDir, "SETTING", "iTopIntervalCounter", 0);
    ScannerAOIIF.iTopRetryCounter           =ReadIniData(szDir, "SETTING", "iTopRetryCounter", 0);
    ScannerAOIIF.TopScannerIfError          =ReadIniData(szDir, "SETTING", "iTopScanAOIFialBin", 0);

    ScannerAOIIF.iTopStartDelayTime        =ReadIniData(szDir, "SETTING", "StartDelayTimeTopScanAOIView", 10);
    ScannerAOIIF.iTopTimeOut               =ReadIniData(szDir, "SETTING", "TimeOutScanTopAOIView", 3);

    ScannerAOIIF.sTopSCANNER_ComPort       =ReadIniData(szDir, "RS232", "Top Device", AnsiString(""));
    ScannerAOIIF.sTopSCANNER_BaudRate      =ReadIniData(szDir, "RS232", "Top Baud Rate", AnsiString("57600"));
    ScannerAOIIF.sTopSCANNER_ByteSize      =ReadIniData(szDir, "RS232", "Top Byte Size", AnsiString("8"));
    ScannerAOIIF.sTopSCANNER_StopBit       =ReadIniData(szDir, "RS232", "Top Stop Bit", AnsiString("1"));
    ScannerAOIIF.sTopSCANNER_Parity        =ReadIniData(szDir, "RS232", "Top Parity", AnsiString("None"));

    ScannerAOIIF.fScannerICGain         =CheckRange(ReadIniData(szDir, "SETTING", "ScannerICGain", 1.0), 1.5 ,0.5);   //Ifor 20200525 add:依據Tray Y Pitch Gain 決定不使用範圍
    edtScannerICGain->Text              =FormatFloat("0.00", ScannerAOIIF.fScannerICGain);

    AnsiString AsTrayData="";
    AnsiString asBuffer="";
    bool bHasFile=true;
    if(iQuotient==0)                                                            //KenHsieh 20220830 : 修改add:AOI Tray Edit
    {
        iQuotient=_MAX_COL_ITEM/10;
        iRemainder=_MAX_COL_ITEM%10;
        if(iRemainder!=0)
            iQuotient+=1;
    }

    for(int k=0; k<iQuotient; k++)                                              //KenHsieh 20220830 : 修改add:AOI Tray Edit
    {
        S.sprintf("TrayDisable_%d", k+1);
        AsTrayData                      =ReadIniData(szDir, "AOITRAY", S, AnsiString("NA"));
        asBuffer=asBuffer+AsTrayData;
        if(AsTrayData=="NA")
            bHasFile=false;
    }
    TStringList *sList;
    sList=new TStringList();
    sList->CommaText=asBuffer;
    if(asBuffer.Length()<_MAX_COL_ITEM*_MAX_ROW_ITEM*2)                         //KenHsieh 20220830 : 修改add:AOI Tray Edit
        bHasFile=false;

    if(bHasFile==true)
    {
        for(int i=0; i<_MAX_COL_ITEM; i++)
        {
            for(int j=0; j<_MAX_ROW_ITEM; j++)
            {
                MOT[MMScanAOI].Tray.Data[i][j]=atoi(sList->Strings[i*_MAX_ROW_ITEM+j].c_str());   //KenHsieh 20220830 : 修改add:AOI Tray Edit
            }
        }
    }
    sList->Clear();
    delete sList;
    fAOI_DoIniDataToForm();                                                     //Ifor 20200813 fix:移至檔案讀取後
    UpDataTrayData();
}
//------------------------------------------------------------------------------
void TFrmAOI::fAOI_DoIniDataToForm()
{
    cbEnabledTopView->Checked = tAOISetup.tTopView.bEnabled;
    edt_FailTopView->Text     = tAOISetup.tTopView.iFailBin;
    edt_TimeoutTopView->Text  = tAOISetup.tTopView.iTimeOut;
    edt_SDelayTopView->Text   = tAOISetup.tTopView.iStartDelayTime;

    cbEnabledPADView->Checked = tAOISetup.tPADView.bEnabled;
    edt_FailPADView->Text     = tAOISetup.tPADView.iFailBin;
    edt_TimeoutPADView->Text  = tAOISetup.tPADView.iTimeOut;
    edt_SDelayPADView->Text   = tAOISetup.tPADView.iStartDelayTime;

    cbEnabledBGAView->Checked = tAOISetup.tBGAView.bEnabled;
    edt_FailBGAView->Text     = tAOISetup.tBGAView.iFailBin;
    edt_TimeoutBGAView->Text  = tAOISetup.tBGAView.iTimeOut;
    edt_SDelayBGAView->Text   = tAOISetup.tBGAView.iStartDelayTime;

    edt_AlarmCount->Text         = tAOISetup.iAlarmCount;
    cbBGAFailRun->Checked        = tAOISetup.tFailRunFunction.bEnabled;   //wei 20160617 Vitrox

    cbAutoSkipBGA->Checked       = tAOISetup.tAutoSkipBGA.bEnabled;       //wei 20160617 Vitrox
    cbAutoSkipPadPkg->Checked    = tAOISetup.tAutoSkipPadPkg.bEnabled;    //wei 20160617 Vitrox
    cbAOINoSort->Checked         = tAOISetup.tAOINoSort.bEnabled;         //wei 20161207

    edt_PassFailBGAPADView->Text = tAOISetup.tBGAPADView.iTestPassFailBin;   //wei  20160706
    edt_FailFailBGAPADView->Text = tAOISetup.tBGAPADView.iTestFailFailBin;   //wei  20160706
    cbTesterFailBin->Checked     = tAOISetup.tTesterFailBin.bEnabled;     //wei  20160706
    cb_VitContiuneAlarm->Checked = tAOISetup.bEnabledContinueAlarm;       //Alick 20160712
    edt_ConAlarmConut->Text      = tAOISetup.iContAlarmCountBySite;       //Alick 20160712
    edt_ConAlarmConut1->Text     = tAOISetup.iContAlarmCountByArm;        //Alick 20160718
    cb_AlarmBySite->Checked      = tAOISetup.bEnabledBySiteBGAView;       //Alick 20160712
    cb_AlarmByArm->Checked       = tAOISetup.bEnabledByArmBGAView;        //Alick 20160712
    cb_AlarmBySite1->Checked     = tAOISetup.bEnabledBySitePADView;       //Alick 20160712
    cb_AlarmByArm1->Checked      = tAOISetup.bEnabledByArmPADView;        //Alick 20160712

    //Ifor 20191225 : add Scan AOI Alarm
    //==>
    cb_ScanAOIAlarmBySite->Checked      = tAOISetup.bEnabledScanAOIBySiteAlarm;
    cb_ScanAOIAlarmByArm->Checked       = tAOISetup.bEnabledScanAOIByArmAlarm;
    edt_ScanAOIAlarmCountBySite->Text   = tAOISetup.iScanAOIAlarmCountBySite;
    edt_ScanAOIAlarmCountByArm->Text    = tAOISetup.iScanAOIAlarmCountByArm;
    cb_ScanAOIUnUseFailBin->Checked     = tAOISetup.bEnabledScanAOIUnUseFailBin;    //Ifor 20200825 add:TF-AMD AOI Error 不丟至Fail Bin
    //<==
    //Ifor 20191225 : add Scan AOI Alarm

    //Ifor 20190724 add AMD AOI
    //==>
    rgScannerMode->ItemIndex        = ScannerAOIIF.iEnableScannerMode;
    edtScannerReadTimeout->Text     = ((int)(ScannerAOIIF.fScannerReadTimeout*100))/100;

    edtAOIIntervalCounter->Text     = ScannerAOIIF.iIntervalCounter;
    edtAOIRetryCounter->Text        = ScannerAOIIF.iRetryCounter;

    rgBallDamageType->ItemIndex     = ScannerAOIIF.iBallDamageType;

    cbBDByTotal->Checked            = ScannerAOIIF.bBDTotalFunction;
    cbBDByTotalConti->Checked       = ScannerAOIIF.bBDTotalContiFunction;
    cbBDBySite->Checked             = ScannerAOIIF.bBDSiteFunction;
    cbBDBySiteConti->Checked        = ScannerAOIIF.bBDSiteContiFunction;
    cbDBAlarmAutoReset->Checked     = ScannerAOIIF.bDBAlramAutoResetCount;

    edtAOIBDCounterTotal->Text      = ScannerAOIIF.iBDTotalCounter;
    edtAOIBDCounterTotalConti->Text = ScannerAOIIF.iBDTotalContiCounter;
    edtAOIBDCounterSite->Text       = ScannerAOIIF.iBDSiteCounter;
    edtAOIBDCounterSiteConti->Text  = ScannerAOIIF.iBDSiteContiCounter;

    pnlAOIBDTotalLast->Caption      = ScannerAOIIF.iBDTotalCounterLast;
    pnlAOIBDTotalContiLast->Caption = ScannerAOIIF.iBDTotalContiCounterLast;
    pnlAOIBDSite1Last->Caption      = ScannerAOIIF.iBDSite1CounterLast;
    pnlAOIBDSite2Last->Caption      = ScannerAOIIF.iBDSite2CounterLast;
    pnlAOIBDSite1ContiLast->Caption = ScannerAOIIF.iBDSite1ContiCounterLast;
    pnlAOIBDSite2ContiLast->Caption = ScannerAOIIF.iBDSite2ContiCounterLast;

    edt_SDelayScanAOI->Text         = ScannerAOIIF.iStartDelayTime;
    edt_TimeoutScanAOI->Text        = ScannerAOIIF.iTimeOut;

    cbDevice->Text                  = ScannerAOIIF.sSCANNER_ComPort;
    cbBaudRate->Text                = ScannerAOIIF.sSCANNER_BaudRate;

    edtScannerICGain->Text          =FormatFloat("0.00", ScannerAOIIF.fScannerICGain);  //Ifor 20200525 add:依據Tray Y Pitch Gain 決定不使用範圍

    if(ScannerAOIIF.sSCANNER_ByteSize == "5")
        cbByteSize->ItemIndex = 0;
    else if(ScannerAOIIF.sSCANNER_ByteSize == "6")
        cbByteSize->ItemIndex = 1;
    else if(ScannerAOIIF.sSCANNER_ByteSize == "7")
        cbByteSize->ItemIndex = 2;
    else if(ScannerAOIIF.sSCANNER_ByteSize == "8")
        cbByteSize->ItemIndex = 3;

    if(ScannerAOIIF.sSCANNER_StopBit == "1" )
        cbStopBit->ItemIndex = 0;
    else if(ScannerAOIIF.sSCANNER_StopBit == "1.5" )
        cbStopBit->ItemIndex = 1;
    else if(ScannerAOIIF.sSCANNER_StopBit == "2" )
        cbStopBit->ItemIndex = 2;

    if(ScannerAOIIF.sSCANNER_Parity == "None" )
        cbParity->ItemIndex = 0;
    else if(ScannerAOIIF.sSCANNER_Parity == "Odd" )
        cbParity->ItemIndex = 1;
    else if(ScannerAOIIF.sSCANNER_Parity == "Even" )
        cbParity->ItemIndex = 2;

    rgAOIFailBinType->ItemIndex     =ScannerAOIIF.iAOIFailBinType;                  //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    UpdateAOIFailBinTypetoForm();                                                   //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    cbAOIFial->ItemIndex            =ScannerAOIIF.ScannerIfError;
    cbAOIFialAndTestPass->ItemIndex =ScannerAOIIF.ScannerIfErrorAndTestPass;        //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    //<==
    //Ifor 20190724 add AMD AOI
    cb_ScanAOIUseLGAMode->Checked   = ScannerAOIIF.bScanAOIUseLGAMode;              //Ifor 20211022 add: AOI LGA Mode
    cb_LGAModeDirection->Checked    = ScannerAOIIF.bLGAModeDirection;               //Ifor 20230202 add: AOI LGA Mode Direction
    edtScannerLGAWaitTime->Text     = ((int)(ScannerAOIIF.fLGAWaitTime*100))/100;   //Ifor 20211022 add: AOI LGA Mode
    //Ifor 20200902 add: TFAMD Top AOI
    //==>
    rgTopScannerMode->ItemIndex         = ScannerAOIIF.iEnableTopScannerMode;
    edtTopAOIIntervalCounter->Text      = ScannerAOIIF.iTopIntervalCounter;
    edtTopAOIRetryCounter->Text         = ScannerAOIIF.iTopRetryCounter;
    cbTopAOIFial->ItemIndex             = ScannerAOIIF.TopScannerIfError;
    cb_TopScanAOIAlarmBySite->Checked   = tAOISetup.bEnabledTopScanAOIBySiteAlarm;
    edt_TopScanAOIAlarmCountBySite->Text= tAOISetup.iTopScanAOIAlarmCountBySite;
    cb_TopScanAOIAlarmByArm->Checked    = tAOISetup.bEnabledTopScanAOIByArmAlarm;
    edt_TopScanAOIAlarmCountByArm->Text = tAOISetup.iTopScanAOIAlarmCountByArm;
    edt_SDelayTopScanAOI->Text          = ScannerAOIIF.iTopStartDelayTime;
    edt_TimeoutTopScanAOI->Text         = ScannerAOIIF.iTopTimeOut;
    edtTopScannerReadTimeout->Text      = ((int)(ScannerAOIIF.fTopScannerReadTimeout*100))/100;
    cb_TopScanAOIUnUseFailBin->Checked  = tAOISetup.bEnabledTopScanAOIUnUseFailBin;

    cbTopDevice->Text                  = ScannerAOIIF.sTopSCANNER_ComPort;
    cbTopBaudRate->Text                = ScannerAOIIF.sTopSCANNER_BaudRate;

    if(ScannerAOIIF.sTopSCANNER_ByteSize == "5")
        cbTopByteSize->ItemIndex = 0;
    else if(ScannerAOIIF.sTopSCANNER_ByteSize == "6")
        cbTopByteSize->ItemIndex = 1;
    else if(ScannerAOIIF.sTopSCANNER_ByteSize == "7")
        cbTopByteSize->ItemIndex = 2;
    else if(ScannerAOIIF.sTopSCANNER_ByteSize == "8")
        cbTopByteSize->ItemIndex = 3;

    if(ScannerAOIIF.sTopSCANNER_StopBit == "1" )
        cbTopStopBit->ItemIndex = 0;
    else if(ScannerAOIIF.sTopSCANNER_StopBit == "1.5" )
        cbTopStopBit->ItemIndex = 1;
    else if(ScannerAOIIF.sTopSCANNER_StopBit == "2" )
        cbTopStopBit->ItemIndex = 2;

    if(ScannerAOIIF.sTopSCANNER_Parity == "None" )
        cbTopParity->ItemIndex = 0;
    else if(ScannerAOIIF.sTopSCANNER_Parity == "Odd" )
        cbTopParity->ItemIndex = 1;
    else if(ScannerAOIIF.sTopSCANNER_Parity == "Even" )
        cbTopParity->ItemIndex = 2;
    //<==
    //Ifor 20200902 add: TFAMD Top AOI
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::FormShow(TObject *Sender)
{
    fAOI_ReadFile();
    if(USE_Scanner_AOI_Inspection==true)
    {
        PageControl1->ActivePageIndex=2;
        TabSheet1->TabVisible=false;
        TabSheet2->TabVisible=false;
        if(USE_Top_Scanner_AOI_Inspection==false)                               //Ifor 20200902 add: TFAMD Top AOI
        {
            TabSheet5->TabVisible=false;
            TabSheet6->TabVisible=false;
        }
    }
    else if(USE_Top_Scanner_AOI_Inspection==true)                               //Ifor 20200902 add: TFAMD Top AOI
    {
        PageControl1->ActivePageIndex=4;
        TabSheet1->TabVisible=false;
        TabSheet2->TabVisible=false;
        TabSheet3->TabVisible=false;
        TabSheet4->TabVisible=false;
    }
    else
    {
        PageControl1->ActivePageIndex=0;
        DrawSitePanelVitrox1();
        DrawSitePanelVitrox2();
        TabSheet3->TabVisible=false;
        TabSheet4->TabVisible=false;
        TabSheet5->TabVisible=false;                                            //Ifor 20200902 add: TFAMD Top AOI
        TabSheet6->TabVisible=false;                                            //Ifor 20200902 add: TFAMD Top AOI
        if(cbTesterFailBin->Checked)
        {
            Label11->Visible=false;
            edt_FailBGAView->Visible=false;
            Label6->Visible=false;
            edt_FailPADView->Visible=false;
            Panel3->Visible=true;
        }
        else
        {
            Label11->Visible=true;
            edt_FailBGAView->Visible=true;
            Label6->Visible=true;
            edt_FailPADView->Visible=true;
            Panel3->Visible=false;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::sbtExitClick(TObject *Sender)
{
    sbtExit->Down =false;
    rbSetFocus->SetFocus();                                                     //Ifor 20200818 add:避免按下離開Scanner Mode無法更新問題
    Close();
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edt_FailTopViewClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1, 15);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edtAOIRetryCounterClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0, 15);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edt_SDelayTopViewClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0, 5000);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edt_AlarmCountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1, 10000);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edtScannerReadTimeoutClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, 1, 120);
}
//------------------------------------------------------------------------------
bool TFrmAOI::CheckFailBin()
{
    int iBin[5];

    iBin[1]=atoi(edt_FailPADView->Text.c_str());
    iBin[2]=atoi(edt_FailBGAView->Text.c_str());

    iBin[3]=atoi(edt_PassFailBGAPADView->Text.c_str());
    iBin[4]=atoi(edt_FailFailBGAPADView->Text.c_str());

    if(cbEnabledBGAView->Checked || cbEnabledPADView->Checked || cbEnabledTopView->Checked)
    {
        for(int i=1; i<sizeof(iBin)/sizeof(int); i++)
        {
            if(Prod.bIsPassBin[iBin[i]])
            {
                return false;
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::DrawSitePanelVitrox1()                                  //Alick 20160712 add
{
    mtDutOnOff_Vitrox->XItem=TestSocket.iShtCol;
    mtDutOnOff_Vitrox->YItem=TestSocket.iShtRow;
    mtDutCol_Vitrox->XItem=TestSocket.iShtCol;
    mtDutRow_Vitrox->YItem=TestSocket.iShtRow;

    for(int i=0; i<TestSocket.iShtCol; i++)
    {
        mtDutCol_Vitrox->SetCellNumber(i, 0, AnsiString(char('a'+i)).c_str());
        mtDutCol_Vitrox->SetCellColorIndex(i, 0, 0);
    }

    for(int j=0; j<TestSocket.iShtRow; j++)
    {
        mtDutRow_Vitrox->SetCellNumber(0, j, AnsiString(char('A'+j)).c_str());
        mtDutRow_Vitrox->SetCellColorIndex(0, j, 0);
    }

    int z=1;
    for(int i=0; i<TestSocket.iShtCol; i++)
    {
        for(int j=0; j<TestSocket.iShtRow; j++)
        {
            mtDutOnOff_Vitrox->SetCellNumber(i, j, z);
            z+=1;
            bVitroxBGAViewUse[i][j]=bVitroxBGAViewMap[i][j];
            if(bVitroxBGAViewUse[j][i])
                mtDutOnOff_Vitrox->SetCellColorIndex(i, j, 0);
            else
                mtDutOnOff_Vitrox->SetCellColorIndex(i, j, 2);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::DrawSitePanelVitrox2()                                  //Alick 20160714 add
{
    mtDutOnOff_Vitrox1->XItem=TestSocket.iShtCol;
    mtDutOnOff_Vitrox1->YItem=TestSocket.iShtRow;
    mtDutCol_Vitrox1->XItem=TestSocket.iShtCol;
    mtDutRow_Vitrox1->YItem=TestSocket.iShtRow;

    for(int i=0; i<TestSocket.iShtCol; i++)
    {
        mtDutCol_Vitrox1->SetCellNumber(i, 0, AnsiString(char('a'+i)).c_str());
        mtDutCol_Vitrox1->SetCellColorIndex(i, 0, 0);
    }

    for(int j=0; j<TestSocket.iShtRow; j++)
    {
        mtDutRow_Vitrox1->SetCellNumber(0, j, AnsiString(char('A'+j)).c_str());
        mtDutRow_Vitrox1->SetCellColorIndex(0, j, 0);
    }

    int z=1;
    for(int i=0; i<TestSocket.iShtCol; i++)
    {
        for(int j=0; j<TestSocket.iShtRow; j++)
        {
            mtDutOnOff_Vitrox1->SetCellNumber(i, j, z);
            z+=1;
            bVitroxPADViewUse[i][j]=bVitroxPADViewMap[i][j];
            if(bVitroxPADViewUse[j][i])
                mtDutOnOff_Vitrox1->SetCellColorIndex(i, j, 0);
            else
                mtDutOnOff_Vitrox1->SetCellColorIndex(i, j, 2);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::mtDutOnOff_Vitrox1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    if(mtDutOnOff_Vitrox1->ConvertIndexCells(X,Y)==-1)
        return;
     bVitroxPADViewUse[Y][X]=!bVitroxPADViewUse[Y][X];
     if(bVitroxPADViewUse[Y][X])
        mtDutOnOff_Vitrox1->SetCellColorIndex(X, Y, 0);
     else
        mtDutOnOff_Vitrox1->SetCellColorIndex(X, Y, 2);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::mtDutOnOff_VitroxMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    if(mtDutOnOff_Vitrox->ConvertIndexCells(X,Y)==-1)
        return;
     bVitroxBGAViewUse[Y][X]=!bVitroxBGAViewUse[Y][X];
     if(bVitroxBGAViewUse[Y][X])
        mtDutOnOff_Vitrox->SetCellColorIndex(X, Y, 0);
     else
        mtDutOnOff_Vitrox->SetCellColorIndex(X, Y, 2);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::cbTesterFailBinMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(cbTesterFailBin->Checked)
    {
        Label11->Visible=false;
        edt_FailBGAView->Visible=false;
        Label6->Visible=false;
        edt_FailPADView->Visible=false;
        Panel3->Visible=true;
    }
    else
    {
        Label11->Visible=true;
        edt_FailBGAView->Visible=true;
        Label6->Visible=true;
        edt_FailPADView->Visible=true;
        Panel3->Visible=false;
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::FormDestroy(TObject *Sender)
{
    try
    {
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TFrmAOI::FormDestroy");
    }
    LogSoftwareOffTime("TFrmAOI, FormDestroy");                                 //Steven 20210526 : 紀錄軟體執行時間
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::spbStopComClick(TObject *Sender)
{
    TSpeedButton* pBtn=(TSpeedButton*)Sender;
    if(pBtn->Tag==1)
        TopAOIComm->StopComm();
    else
        AOIComm->StopComm();
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::spbStartComClick(TObject *Sender)
{
    TSpeedButton* pBtn=(TSpeedButton*)Sender;
    if(pBtn->Tag==1)
        TopAOIComm->StopComm();
    else
        AOIComm->StopComm();
    RS232Init();
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::RS232Init()
{
    #ifndef SOFT_SIMULTE
    bool flag;
    AnsiString Str;
    flag=GetCOMPortStatus(ScannerAOIIF.sSCANNER_ComPort);
    if(flag==false)
    {
        Str.sprintf("Scanner AOI : %s port status error!", ScannerAOIIF.sSCANNER_ComPort);
        ShowMyMessage(Str);
    }
    else
    {
        AOIComm->CommName="\\\\.\\"+ScannerAOIIF.sSCANNER_ComPort;
        try
        {
            AOIComm->BaudRate=atoi(ScannerAOIIF.sSCANNER_BaudRate.c_str());

            if(ScannerAOIIF.sSCANNER_ByteSize == "5")
                AOIComm->ByteSize = _5;
            else if(ScannerAOIIF.sSCANNER_ByteSize == "6")
                AOIComm->ByteSize = _6;
            else if(ScannerAOIIF.sSCANNER_ByteSize == "7")
                AOIComm->ByteSize = _7;
            else if(ScannerAOIIF.sSCANNER_ByteSize == "8")
                AOIComm->ByteSize = _8;

            if(ScannerAOIIF.sSCANNER_StopBit == "1" )
                AOIComm->StopBits = _1;
            else if(ScannerAOIIF.sSCANNER_StopBit == "1.5" )
                AOIComm->StopBits = _1_5;
            else if(ScannerAOIIF.sSCANNER_StopBit == "2" )
                AOIComm->StopBits = _2;

            if(ScannerAOIIF.sSCANNER_Parity == "None" )
                AOIComm->Parity = None;
            else if(ScannerAOIIF.sSCANNER_Parity == "Odd" )
                AOIComm->Parity = Odd;
            else if(ScannerAOIIF.sSCANNER_Parity == "Even" )
                AOIComm->Parity = Even;

            AOIComm->StopComm();
            AOIComm->StartComm();                                               //僅能啟動一次
        }
        catch(...)
        {

        }
    }
    #endif
}
//------------------------------------------------------------------------------
void TFrmAOI::ShowCommData(AnsiString sType, AnsiString sData, int iNum)        //Ifor 20200908 add:可使用多台Scan AOI 裝置
{
    AnsiString sLog     =sType+sData;
    AnsiString sHexLog  =sType+"";

    if(iNum==1)
    {
        if(TopMemoLogString->Lines->Count>300)
        {
            TopMemoLogString->Clear();
        }

        TopMemoLogString->Lines->Append(sLog);

        if(TopMemoLogHex->Lines->Count>300)
        {
            TopMemoLogHex->Clear();
        }
    }
    else
    {
        if(MemoLogString->Lines->Count>300)
        {
            MemoLogString->Clear();
        }

        MemoLogString->Lines->Append(sLog);

        if(MemoLogHex->Lines->Count>300)
        {
            MemoLogHex->Clear();
        }
    }

    for(int i=0; i<sData.Length(); i++)
    {
        Byte dData=(Byte)sData[i+1];                                            // 2013.05.31 , Joye , AMD-SG UBS
        sHexLog+=IntToHex((int)dData, 2)+" ";                                   // 2013.05.31 , Joye , AMD-SG UBS
    }
    if(iNum==1)
        TopMemoLogHex->Lines->Append(sHexLog);
    else
        MemoLogHex->Lines->Append(sHexLog);
}
//------------------------------------------------------------------------------
void TFrmAOI::SendCommand(AnsiString sData, int iNum)                           //Ifor 20200908 add:可使用多台Scan AOI 裝置
{
    int iSize=sData.Length();
    if(iSize<=0)
    {
        ShowCommData("[Send]", sData, iNum);                                    //Ifor 20200908 add:可使用多台Scan AOI 裝置
        ShowCommData("[Send Data Fail]", " Size = 0", iNum);                    //Ifor 20200908 add:可使用多台Scan AOI 裝置
        return;
    }

    char* Buff;
    Buff=new char[iSize+1];

    for(int i=0; i<iSize; i++)
    {
        Buff[i]=sData[i+1];
    }

    Buff[iSize]='\0';

    if(iNum==0)
        AOIComm->WriteCommData(Buff, strlen(Buff));
    else
        TopAOIComm->WriteCommData(Buff, strlen(Buff));

    ShowCommData("[Send]", sData, iNum);                                        //Ifor 20200908 add:可使用多台Scan AOI 裝置
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::sbSendClick(TObject *Sender)
{
    TSpeedButton* pBtn=(TSpeedButton*)Sender;
    int iNum=pBtn->Tag;

    SendCommand(edtManual->Text, iNum);                                         //Ifor 20200908 add:可使用多台Scan AOI 裝置
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::sbClearClick(TObject *Sender)
{
    TSpeedButton* pBtn=(TSpeedButton*)Sender;
    if(pBtn->Tag==0)
    {
        MemoLogString->Clear();
        MemoLogHex->Clear();
    }
    else
    {
        TopMemoLogString->Clear();
        TopMemoLogHex->Clear();
    }
}
//------------------------------------------------------------------------------
void TFrmAOI::GetScannerResult(AnsiString sData)
{
    AnsiString sResult;

    int iPos1=sData.Pos("site");

    if(iPos1==0)
    {
        iAOIResult[0]   =-1;
    }
    else
    {
        sResult         =sData.SubString(iPos1+6 , 1);
        iAOIResult[0]   =atoi(sResult.c_str());
    }

    if(iAOIResult[0]==-1)
    {
        FrmAOI->ShowCommData("[Decode] Fail ", "Abnormal data");
    }
    else if(iAOIResult[0]!=1)
    {
        FrmAOI->ShowCommData("[Decode] Fail ", sResult);
    }
    else
    {
        FrmAOI->ShowCommData("[Decode] Pass ", sResult);
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::AOICommReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    char *data;
    data=(char*)Buffer;
    AnsiString sRead;
    AnsiString sReadChk;
    int iDataCount=0;
    int j=strlen(data);
    AnsiString LogMsg="";

    sRead="";
    for(int i=0; i<j; i++)
    {
        sRead+=data[i];
    }
    ShowCommData("[Read]", sRead);

    iDataCount= BufferLength-2;

    if(j<1+iDataCount+1)
    {
        ShowCommData("[Error]", " Read Length Error!!");
        return;
    }

    sReadChk=sRead.SubString(2,iDataCount);
    try                                                                         //Richard 20220817:Add Scanner_AOI add check Big Size IC Function
    {
        TStringList *sList=new TStringList();
        sList->CommaText=sReadChk;
        if(sList->Strings[0] == "GetGrabPos")
        {
            if(sList->Count==5)
            {
                iLGAPitchXCnt=atoi(sList->Strings[1].c_str());
                iLGAPitchYCnt=atoi(sList->Strings[2].c_str());
                iLGAPitchX   =atoi(sList->Strings[3].c_str());
                iLGAPitchY   =atoi(sList->Strings[4].c_str());
            }
        }
        delete sList;
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }

    char *chk=sReadChk.c_str();
    Byte k=0;
    for(int i=0; i<iDataCount; i++)
    {
        k+=Byte(chk[i]);
    }
    k=k%0x100;
    sReadChk.sprintf("%c",k);
    char *chk1=sReadChk.c_str();

    if(data[j-1]!=chk1[0])
    {
        ShowCommData("[Error]", " Checksum Error!!");
        return;
    }
    bTriggerAOI=false;
    GetScannerResult(sRead);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::sbBDTotalResetClick(TObject *Sender)
{
    TSpeedButton* pBtn=(TSpeedButton*)Sender;

    if(pBtn->Tag==1)
    {
        pnlAOIBDTotalLast->Caption              ="0";
        ScannerAOIIF.iBDTotalCounterLast        =0;
    }
    else if(pBtn->Tag==2)
    {
        pnlAOIBDTotalContiLast->Caption         ="0";
        ScannerAOIIF.iBDTotalContiCounterLast   =0;
    }
    else if(pBtn->Tag==3)
    {
        pnlAOIBDSite1Last->Caption              ="0";
        pnlAOIBDSite2Last->Caption              ="0";

        ScannerAOIIF.iBDSite1CounterLast        =0;
        ScannerAOIIF.iBDSite2CounterLast        =0;
    }
    else if(pBtn->Tag==4)
    {
        pnlAOIBDSite1ContiLast->Caption         ="0";
        pnlAOIBDSite2ContiLast->Caption         ="0";

        ScannerAOIIF.iBDSite1ContiCounterLast   =0;
        ScannerAOIIF.iBDSite2ContiCounterLast   =0;
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::TriggerAOISystem(bool bSite1, bool bSite2, int iNum)   //Ifor 20200908 add:可使用多台Scan AOI 裝置
{
    Byte k=0;
    Byte Blength=0x07;
    AnsiString asR="";
    AnsiString asChk="";

    asChk.sprintf("%d%dBDAOI", bSite1, bSite2);

    unsigned char *chk=asChk.c_str();
    int j=asChk.Length();
    for(int i=0; i<j; i++)
    {
        k+=Byte(chk[i]);
    }
    k=k%0x100;
    asR.sprintf("%c%s%c", Blength, asChk, k);
    bTriggerAOI=true;
    SendCommand(asR, iNum);                                                     //Ifor 20200908 add:可使用多台Scan AOI 裝置
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::sbAOISendClick(TObject *Sender)
{
    TSpeedButton* pBtn = (TSpeedButton*)Sender;
    int iNum=pBtn->Tag;

    if(pBtn->Tag==1)
    {
        if(rgTopAOISiteMode->ItemIndex==0)
        {
            FrmAOI->TriggerAOISystem(true, false, iNum);
        }
        else if(rgTopAOISiteMode->ItemIndex==1)
        {
            FrmAOI->TriggerAOISystem(false, true, iNum);
        }
        else if(rgTopAOISiteMode->ItemIndex==2)
        {
            FrmAOI->TriggerAOISystem(true, true, iNum);
        }
    }
    else
    {
        if(rgAOISiteMode->ItemIndex==0)
        {
            FrmAOI->TriggerAOISystem(true, false, iNum);
        }
        else if(rgAOISiteMode->ItemIndex==1)
        {
            FrmAOI->TriggerAOISystem(false, true, iNum);
        }
        else if(rgAOISiteMode->ItemIndex==2)
        {
            FrmAOI->TriggerAOISystem(true, true, iNum);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::CheckBallDamageCounter()
{
    if(bAOIBallDamageCounterFailAlarm==true)                                    // 2012.12.17 , Joye , AMD HT-7046M
        return;

    if(ScannerAOIIF.bBDTotalFunction==true)
    {
        if(ScannerAOIIF.iBDTotalCounterLast>=ScannerAOIIF.iBDTotalCounter)
        {                                                                       // Ball Damage Fail Alarm
            ShowErrorMessage("WAR0870", K_SKIP, MMSystem, false);               //Ifor 20191205 : add Ball AOI
            bAOIBallDamageCounterFailAlarm=true;
        }
    }
    else
    {
        ScannerAOIIF.iBDTotalCounterLast=0;
    }

    if(ScannerAOIIF.bBDTotalContiFunction==true)
    {
        if(ScannerAOIIF.iBDTotalContiCounterLast>=ScannerAOIIF.iBDTotalContiCounter)
        {                                                                       // Ball Damage Fail Alarm
            ShowErrorMessage("WAR0873", K_SKIP, MMSystem, false);               //Ifor 20191205 : add Ball AOI
            bAOIBallDamageCounterFailAlarm=true;
        }
    }
    else
    {
        ScannerAOIIF.iBDTotalContiCounterLast=0;
    }

    if(ScannerAOIIF.bBDSiteFunction==true)
    {
        if(ScannerAOIIF.iBDSite1CounterLast>=ScannerAOIIF.iBDSiteCounter)
        {                                                                       // Ball Damage Fail Alarm
            ShowErrorMessage("WAR0871", K_SKIP, MMSystem, false);               //Ifor 20191205 : add Ball AOI
            bAOIBallDamageCounterFailAlarm=true;
        }

        if(ScannerAOIIF.iBDSite2CounterLast>=ScannerAOIIF.iBDSiteCounter)
        {                                                                       // Ball Damage Fail Alarm
            ShowErrorMessage("WAR0872", K_SKIP, MMSystem, false);               //Ifor 20191205 : add Ball AOI
            bAOIBallDamageCounterFailAlarm=true;
        }
    }
    else
    {
        ScannerAOIIF.iBDSite1CounterLast=0;
        ScannerAOIIF.iBDSite2CounterLast=0;
    }

    if(ScannerAOIIF.bBDSiteContiFunction==true)
    {
        if(ScannerAOIIF.iBDSite1ContiCounterLast>=ScannerAOIIF.iBDSiteContiCounter)
        {                                                                       // Ball Damage Fail Alarm
            ShowErrorMessage("WAR0874", K_SKIP, MMSystem, false);               //Ifor 20191205 : add Ball AOI
            bAOIBallDamageCounterFailAlarm=true;
        }

        if(ScannerAOIIF.iBDSite2ContiCounterLast>=ScannerAOIIF.iBDSiteContiCounter)
        {                                                                       // Ball Damage Fail Alarm
            ShowErrorMessage("WAR0875", K_SKIP, MMSystem, false);               //Ifor 20191205 : add Ball AOI
            bAOIBallDamageCounterFailAlarm=true;
        }
    }
    else
    {
        ScannerAOIIF.iBDSite1ContiCounterLast=0;
        ScannerAOIIF.iBDSite2ContiCounterLast=0;
    }

    if(bAOIBallDamageCounterFailAlarm==true)
    {
        iOneCycle=1;
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::DoContinuousFailBySiteScanAOI(bool Res, int iRow, int iCol, int iNum)
{
    if(iNum==0)
    {
        if(Res==true)
        {
            iScanAOIBySiteTotal[iRow][iCol]++;
        }
        else
        {
            iScanAOIBySiteTotal[iRow][iCol]=0;
        }

        if(iScanAOIBySiteTotal[iRow][iCol]>=tAOISetup.iScanAOIAlarmCountBySite)
        {
            bScanAOIBySiteFailCont=true;
        }
    }
    else
    {
        if(Res==true)
        {
            iTopScanAOIBySiteTotal[iRow][iCol]++;
        }
        else
        {
            iTopScanAOIBySiteTotal[iRow][iCol]=0;
        }

        if(iTopScanAOIBySiteTotal[iRow][iCol]>=tAOISetup.iTopScanAOIAlarmCountBySite)
        {
            bTopScanAOIBySiteFailCont=true;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::DoContinuousFailByArmScanAOI(bool Res, int ArmSide, int iNum)
{
    if(iNum==0)
    {
        if(Res==true)
        {
            iScanAOIByArmTotal[ArmSide]++;
        }
        else
        {
            iScanAOIByArmTotal[ArmSide]=0;
        }

        if(iScanAOIByArmTotal[ArmSide]>=tAOISetup.iScanAOIAlarmCountByArm)
        {
            bScanAOIByArmFailCont=true;
            bScanAOIByArmDouChk[ArmSide]=true;
        }
    }
    else
    {
        if(Res==true)
        {
            iTopScanAOIByArmTotal[ArmSide]++;
        }
        else
        {
            iTopScanAOIByArmTotal[ArmSide]=0;
        }

        if(iTopScanAOIByArmTotal[ArmSide]>=tAOISetup.iTopScanAOIAlarmCountByArm)
        {
            bTopScanAOIByArmFailCont=true;
            bTopScanAOIByArmDouChk[ArmSide]=true;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edtScannerICGainMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 1.50, 0.50);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edtScannerLGAWaitTimeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 20.0, 1.0);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::mtAOIBufferMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(HasICUnderMachine()==false)
    {
        EditTray(MMScanAOI);
        UpDataTrayData();
    }
    else
    {
        ShowErrorMessage("MES1646", 0, MMSystem, false, "AOI Tray Edit");       //Must finish [Clean out]!!
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::UpDataTrayData()
{
    if(InitialOK==false)
        return;

    mtAOIBuffer->XItem=MOT[MManualTray2].Tray.XItem;
    mtAOIBuffer->YItem=MOT[MManualTray2].Tray.YItem;
    MOT[MMScanAOI].Tray.XItem=MOT[MManualTray2].Tray.XItem;
    MOT[MMScanAOI].Tray.YItem=MOT[MManualTray2].Tray.YItem;
    for(int i=0; i<MOT[MMScanAOI].Tray.XItem; i++)
    {
        for(int j=0; j<MOT[MMScanAOI].Tray.YItem; j++)
        {
            mtAOIBuffer->SetCellColorIndex(i, j, MOT[MMScanAOI].Tray.Data[i][j]);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::rgAOIFailBinTypeClick(TObject *Sender)                 //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
{
    UpdateAOIFailBinTypetoForm();
}
//---------------------------------------------------------------------------
void __fastcall TFrmAOI::UpdateAOIFailBinTypetoForm()                           //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
{
    if(rgAOIFailBinType->ItemIndex==0)
    {
        lblAOIBinSel1->Caption          ="AOI Fail :";
        lblAOIBinSel2->Visible          =false;
        cbAOIFialAndTestPass->Visible   =false;
    }
    else
    {
        lblAOIBinSel1->Caption          ="AOI Fail + Test Fail :";
        lblAOIBinSel2->Visible          =true;
        cbAOIFialAndTestPass->Visible   =true;
    }
}
//---------------------------------------------------------------------------
