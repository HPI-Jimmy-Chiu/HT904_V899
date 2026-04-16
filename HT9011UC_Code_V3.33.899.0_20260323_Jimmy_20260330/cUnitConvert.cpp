#include "MachineDefine.h"
#pragma hdrstop

#include "cUnitConvert.h"
#include "cprod.h"
#include "cmydef.h"
#include "uShowMessage.h"
#include "ATC_Handler_Side.h"
#include "MyKitSuck.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
int iUnitMultiply100(double Data)
{
    int iReturn;
    AnsiString asString;
    asString=FormatFloat("0.00", Data*100.00);
    iReturn=atoi(asString.c_str());
    return iReturn;
}
//---------------------------------------------------------------------------
int iUnitMultiply1000(double Data)
{
    return Data*1000.0;
}
//---------------------------------------------------------------------------
void DoTestIFConvert()
{
    memcpy(&TestIF.iTestMode, &TestIF_File.iTestMode, sizeof(TestIF_File));
    TestIF.dSiteXPitch = iUnitMultiply100(TestIF_File.dSiteXPitch);
    TestIF.dSiteYPitch = iUnitMultiply100(TestIF_File.dSiteYPitch);
    TestIF.dSiteYOffset= iUnitMultiply100(TestIF_File.dSiteYOffset);            //Steven 20140512 : For HT-9047
    TestIF.dShiftXPitch= iUnitMultiply100(TestIF_File.dShiftXPitch);            //wei 20160226 TSMC X Shift
    TestIF.dSiteXCenterPitch=iUnitMultiply100(TestIF_File.dSiteXCenterPitch);   //Steven 20170706 (wei) : 2x4中間的Pitch不同 for SCC
    TestIF.dMulti2DXPitch = iUnitMultiply100(TestIF_File.dMulti2DXPitch);       //Steven 20240612 : for 對角2D

    //ChungHung 20130426 add
    if(TestIF.bAutoClean_UseTray && IniConfig.bE43AutoCleanUseHotplate==false)  //Steven 20160630 : fixed for AutoCleanUseHotplate
    {
        TestIF.dAutoClean_XPitch=TestIF_File.dAutoClean_XPitch_Tray;
        TestIF.dAutoClean_YPitch=TestIF_File.dAutoClean_YPitch_Tray;
        TestIF.dAutoClean_XStart=TestIF_File.dAutoClean_XStart_Tray;
        TestIF.dAutoClean_YStart=TestIF_File.dAutoClean_YStart_Tray;
        TestIF.iAutoClean_XDivision=TestIF_File.iAutoClean_XDivision_Tray;
        TestIF.iAutoClean_YDivision=TestIF_File.iAutoClean_YDivision_Tray;
    }
    else
    {
        TestIF.dAutoClean_XPitch=TestIF_File.dAutoClean_XPitch_Kit;             //ChungHung 20130708 TestIF--->TestIF_File
        TestIF.dAutoClean_YPitch=TestIF_File.dAutoClean_YPitch_Kit;
        TestIF.dAutoClean_XStart=TestIF_File.dAutoClean_XStart_Kit;
        TestIF.dAutoClean_YStart=TestIF_File.dAutoClean_YStart_Kit;
        TestIF.iAutoClean_XDivision=TestIF_File.iAutoClean_XDivision_Kit;
        TestIF.iAutoClean_YDivision=TestIF_File.iAutoClean_YDivision_Kit;
    }
}
//---------------------------------------------------------------------------
void DoDeviceConvert()
{
    memcpy(&DeviceForm.IndexArmPick[0], &DeviceForm_File.IndexArmPick[0], sizeof(DeviceForm_File));
    for(int i=0; i<2; i++)
    {
        DeviceForm.IndexArmPick[i]  =iUnitMultiply100(DeviceForm_File.IndexArmPick[i]);
        DeviceForm.IndexPlace[i]    =iUnitMultiply100(DeviceForm_File.IndexPlace[i]);
        DeviceForm.IndexDrop[i]     =iUnitMultiply100(DeviceForm_File.IndexDrop[i]);
        DeviceForm.IndexContact[i]  =iUnitMultiply100(DeviceForm_File.IndexContact[i]);
        DeviceForm.IndexUp[i]       =iUnitMultiply100(DeviceForm_File.IndexUp[i]);
    }
    DeviceForm.XDimension       =iUnitMultiply100(DeviceForm_File.XDimension);
    DeviceForm.YDimension       =iUnitMultiply100(DeviceForm_File.YDimension);
}
//---------------------------------------------------------------------------
void DoHotPlateConvert()
{
    memcpy(&HotPlateForm.XPitch,&HotPlateForm_File.XPitch,sizeof(TRAY_TYPE_PARA));
    HotPlateForm.XPitch = iUnitMultiply100(HotPlateForm_File.XPitch);
    HotPlateForm.YPitch = iUnitMultiply100(HotPlateForm_File.YPitch);
    HotPlateForm.XStart = iUnitMultiply100(HotPlateForm_File.XStart);
    HotPlateForm.YStart = iUnitMultiply100(HotPlateForm_File.YStart);
}
//---------------------------------------------------------------------------
void DoArmOffsetConvert()
{
    //In Arm
    for(int i=0; i<InOfsTotal; i++)                                             //Steven 20140512
    {
        InArmOffSet[i]->SetOneByOne(InArmOffSet_File[i]->GetOneByOne());
        InArmOffSet[i]->SetX(iUnitMultiply100(InArmOffSet_File[i]->GetX()));
        InArmOffSet[i]->SetY(iUnitMultiply100(InArmOffSet_File[i]->GetY()));
        InArmOffSet[i]->SetPickUp(iUnitMultiply100(InArmOffSet_File[i]->GetPickUp()));
        InArmOffSet[i]->SetPlace(iUnitMultiply100(InArmOffSet_File[i]->GetPlace()));
        InArmOffSet[i]->SetVariable(iUnitMultiply100(InArmOffSet_File[i]->GetVariable()));
        InArmOffSet[i]->SetVariableY(iUnitMultiply100(InArmOffSet_File[i]->GetVariableY()));    //Steven 20140512 : XY變距
        InArmOffSet[i]->SetVariable2(iUnitMultiply100(InArmOffSet_File[i]->GetVariable2()));    //Steven 20140512 : XY變距
        InArmOffSet[i]->SetVariable3(iUnitMultiply100(InArmOffSet_File[i]->GetVariable3()));
        InArmOffSet[i]->SetVariable4(iUnitMultiply100(InArmOffSet_File[i]->GetVariable4()));

        for(int j=0; j<InArmSuck.iMotRow; j++)
        {
            for(int k=0; k<InArmSuck.iMotCol; k++)
            {
                InArmOffSet[i]->SingleOffSet->dPosOffSetX[j][k]     = iUnitMultiply100(InArmOffSet_File[i]->SingleOffSet->dPosOffSetX[j][k]);
                InArmOffSet[i]->SingleOffSet->dPosOffSetY[j][k]     = iUnitMultiply100(InArmOffSet_File[i]->SingleOffSet->dPosOffSetY[j][k]);
                InArmOffSet[i]->SingleOffSet->dPickUpOffSet[j][k]   = iUnitMultiply100(InArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[j][k]);
                InArmOffSet[i]->SingleOffSet->dPlaceOffSet[j][k]    = iUnitMultiply100(InArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[j][k]);
            }
        }
    }

    //Out Arm
    for(int i=0; i<OutOfsTotal; i++)                                            //Steven 20140512
    {
        OutArmOffSet[i]->SetOneByOne    (OutArmOffSet_File[i]->GetOneByOne());
        OutArmOffSet[i]->SetX           (iUnitMultiply100(OutArmOffSet_File[i]->GetX()));
        OutArmOffSet[i]->SetY           (iUnitMultiply100(OutArmOffSet_File[i]->GetY()));
        OutArmOffSet[i]->SetPickUp      (iUnitMultiply100(OutArmOffSet_File[i]->GetPickUp()));
        OutArmOffSet[i]->SetPlace       (iUnitMultiply100(OutArmOffSet_File[i]->GetPlace()));
        OutArmOffSet[i]->SetVariable    (iUnitMultiply100(OutArmOffSet_File[i]->GetVariable()));
        OutArmOffSet[i]->SetVariableY   (iUnitMultiply100(OutArmOffSet_File[i]->GetVariableY()));    //Steven 20140512 : XY變距
        OutArmOffSet[i]->SetVariable2   (iUnitMultiply100(OutArmOffSet_File[i]->GetVariable2()));    //Steven 20140512 : XY變距
        OutArmOffSet[i]->SetVariable3   (iUnitMultiply100(OutArmOffSet_File[i]->GetVariable3()));
        OutArmOffSet[i]->SetVariable4   (iUnitMultiply100(OutArmOffSet_File[i]->GetVariable4()));

        for(int j=0; j<InArmSuck.iMotRow; j++)
        {
            for(int k=0; k<InArmSuck.iMotCol; k++)
            {
                OutArmOffSet[i]->SingleOffSet->dPosOffSetX[j][k]    = iUnitMultiply100(OutArmOffSet_File[i]->SingleOffSet->dPosOffSetX[j][k]);
                OutArmOffSet[i]->SingleOffSet->dPosOffSetY[j][k]    = iUnitMultiply100(OutArmOffSet_File[i]->SingleOffSet->dPosOffSetY[j][k]);
                OutArmOffSet[i]->SingleOffSet->dPickUpOffSet[j][k]  = iUnitMultiply100(OutArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[j][k]);
                OutArmOffSet[i]->SingleOffSet->dPlaceOffSet[j][k]   = iUnitMultiply100(OutArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[j][k]);
            }
        }
    }

    //Tray Arm
    for(int i=0; i<MAX_TRACK; i++)
    {
        Offset.iTrayArmX[i]     = iUnitMultiply100(Offset_File.iTrayArmX[i]);
        Offset.iTrayArmX_ART[i] = iUnitMultiply100(Offset_File.iTrayArmX_ART[i]);       //kevin 20170831 (Steven) add ART offset
    }

    //Test
    for(int i=0; i<2; i++)
    {
        Offset.iIndexArmPickUp[i]   = iUnitMultiply100(Offset_File.iIndexArmPickUp[i]);
        Offset.iIndexArmPlace[i]    = iUnitMultiply100(Offset_File.iIndexArmPlace[i]);
        Offset.iIndexArmContact[i]  = iUnitMultiply100(Offset_File.iIndexArmContact[i]);
        Offset.iSHHalft[i]          = iUnitMultiply100(Offset_File.iSHHalft[i]);
        Offset.iSHRightPod[i]       = iUnitMultiply100(Offset_File.iSHRightPod[i]);
        Offset.iSHLeftPod[i]        = iUnitMultiply100(Offset_File.iSHLeftPod[i]);
        Offset.iSHLeft2D[i]         = iUnitMultiply100(Offset_File.iSHLeft2D[i]);       //Steven 20151218 : Offset for 2d reader
    }
    Offset.iPreciserOpen       = iUnitMultiply100(Offset_File.iPreciserOpen);           //JerryYang 20180820 add
    Offset.iPreciserClose      = iUnitMultiply100(Offset_File.iPreciserClose);

    for(int i=0; i<MAX_TRACK; i++)                                                      //JerryYang 20200805 fix tray z motor offset
    {
        Offset.dTrayZseparate[i]=iUnitMultiply100(Offset_File.dTrayZseparate[i]);
    }
}
//---------------------------------------------------------------------------
void DoArmSpeedConvert()
{
    int i;
    for(i=0; i<SpeedPartTotal; i++)    //Steven 20140428 : For Rotate 4 --> SpeedPartTotal
    {
        memcpy(&ArmSpeed[i].iBodySP, &ArmSpeed_File[i].iBodySP, sizeof(ARM_CONDITION));
        ArmSpeed[i].dRetryDown = iUnitMultiply100(ArmSpeed_File[i].dRetryDown);
    }
    memcpy(&SHSpeed.iSH1Sp, &SHSpeed_File.iSH1Sp, sizeof(SHUTTLE_SPEED));

    memcpy(&MGSpeed.iCYSp, &MGSpeed_File.iCYSp, sizeof(MAGAZINE_SPEED));  //JerryYang 20220909 : add magazine

    fShowMessage->ShowSpeed(IniConfig.bG05ShowSpeedMessage);
}
//---------------------------------------------------------------------------
void DoDefFormConvert()
{
    for(int i=0; i<4; i++)
    {
        memcpy(&UserDefForm[i].XPitch, &UserDefForm_File[i].XPitch, sizeof(TRAY_TYPE_PARA));
        UserDefForm[i].XPitch       = iUnitMultiply100(UserDefForm_File[i].XPitch);
        UserDefForm[i].YPitch       = iUnitMultiply100(UserDefForm_File[i].YPitch);
        UserDefForm[i].XStart       = iUnitMultiply100(UserDefForm_File[i].XStart);
        UserDefForm[i].YStart       = iUnitMultiply100(UserDefForm_File[i].YStart);
        UserDefForm[i].ZDepth       = iUnitMultiply100(UserDefForm_File[i].ZDepth);

        if(IniConfig.bC03UseCatchTray)        //kevin 20220302 夾TRAY 模組 限定 TRAY 寬度避免 被 IN OUT ARM 撞機
        {
            if(UserDefForm_File[i].iPickUp<60 || UserDefForm_File[i].iPickUp>73)
                UserDefForm_File[i].iPickUp=65;
        }
        else
        {
            if(UserDefForm_File[i].iPickUp<30 || UserDefForm_File[i].iPickUp>110)
                UserDefForm_File[i].iPickUp=65;
        }
        UserDefForm[i].iPickUp          = iUnitMultiply100(UserDefForm_File[i].iPickUp);    //Steven 20220523 : Unmark
        UserDefForm[i].BlockXStart      = iUnitMultiply100(UserDefForm_File[i].BlockXStart);
        UserDefForm[i].BlockYStart      = iUnitMultiply100(UserDefForm_File[i].BlockYStart);
        UserDefForm[i].BlockPitchX      = iUnitMultiply100(UserDefForm_File[i].BlockPitchX);
        UserDefForm[i].BlockPitchY      = iUnitMultiply100(UserDefForm_File[i].BlockPitchY);
        UserDefForm[i].BlockTraySize    = iUnitMultiply100(UserDefForm_File[i].BlockTraySize);
    }

    if(USE_LdUldCassetteMode==1)                                                //RogerYang 20260203 : Add for HT9046CR
    {
        UserDefForm[0].dCassetteZPitch  = iUnitMultiply100(UserDefForm_File[0].dCassetteZPitch);
        UserDefForm[0].dCassetteZStart  = iUnitMultiply100(UserDefForm_File[0].dCassetteZStart);
    }
}
//---------------------------------------------------------------------------
void DoStructUnitConvert()
{
    DoTestIFConvert();
    DoDeviceConvert();
    DoHotPlateConvert();
    DoArmOffsetConvert();
    DoArmSpeedConvert();
    DoDefFormConvert();

    if(Temperature.bATCActiveCooling==true && LastSet.iTemperature==Tempture_Ambient &&
      (ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33 ||
       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35 ||
       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_61) &&
       Temperature.bActiveHeatGun && Temperature.dATC_HotGunTime!=0)
    {
        ArmSpeed[OutArm].dWaitOnSH=ArmSpeed[OutArm].dWaitOnSH+Temperature.dATC_HotGunTime;
    }
}
//---------------------------------------------------------------------------
