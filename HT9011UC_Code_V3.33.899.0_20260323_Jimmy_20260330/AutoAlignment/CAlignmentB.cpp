#include "MachineDefine.h"
//---------------------------------------------------------------------------


#pragma hdrstop

#include "CAlignmentB.h"

#include "ainarm9045.h"
#include "aoutarm9045.h"
#include "MyKitSuck.h"
#include "cmydef.h"
#include "mymotor.h"
#include "ainarm2.h"
#include "aoutarm.h"
#include "common.h"
#include "mymessbox.h"
#include "cAutoAlignment.h"
#include "main.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


enum eITeachMarkItem
{
    eInShut1            =0,         //SHT1 取1點
    eInShut2            =1,
    eHP2_1              =2,         //HP2 取3點
    eHP2_2              =3,
    eHP2_3              =4,
    eHP1_1              =5,         //HP1 取3點
    eHP1_2              =6,
    eHP1_3              =7,
    eLoad_1             =8,         //Load 取2點
    eLoad_2             =9,
    eITeachMarkTotal   //10
};
enum eOTeachMarkItem
{
    eAuto1_1            =0,         //Auto1 取 2 點
    eAuto1_2            =1,
    eAuto2_1            =2,         //Auto2 取 3 點
    eAuto2_2            =3,
    eAuto2_3            =4,
    eAuto3_1            =5,         //Auto3 取 3 點
    eAuto3_2            =6,
    eAuto3_3            =7,
    eFix1_3             =8,
    eFix2_1             =9,         //Fix 取2點
    eOutShut1           =10,
    eOutShut2           =11,
    eOTeachMarkTotal   //12
};
__fastcall TCAlignmentB::TCAlignmentB(AnsiString asInputType)
{
    asType=asInputType;
    if(asType=="IN")
    {
        _MArmX=MInArmX;
        _MArmY=MInArmY;
        _MArmPitchY=MInArmPitchY;
        _MArmPitchX2=MInArmPitchX2;
        _IO=0;
        vcAlign[0]=new TVisionComm("IN_TOP");
        vcAlign[1]=new TVisionComm("IN_BOTTOM");
    }
    else
    {
        _MArmX=MOutArmX;
        _MArmY=MOutArmY;
        _MArmPitchY=MOutArmPitchY;
        _MArmPitchX2=MInArmPitchX2;
         _IO=1;
        vcAlign[0]=new TVisionComm("OUT_TOP");
        vcAlign[1]=new TVisionComm("OUT_BOTTOM");
    }

}
//---------------------------------------------------------------------------
__fastcall TCAlignmentB::~TCAlignmentB()
{
    delete vcAlign[0];
    delete vcAlign[1];
}
//---------------------------------------------------------------------------
int TCAlignmentB::DoAlign(bool bReset)
{
    int iReturn=0;
    AnsiString s="";

    int &Task=iAlignTask;

    iReturn=0;
    if(bReset==true)
    {
        Task=1;
        bMoveAllZ=false;
        InitialShuttleStatus();
        return iReturn;
    }

    switch(Task)
    {
        case 1:
            //CycleTimeTimer(true);
            WriteAlignFlowLog("Alignment Start");
            InitialShuttleStatus();
            bMoveAllZ=false;
            Task=100;
            break;
        case 100:
            if(asType=="IN")
            {
                //bMoveAllZ=MoveInArmZToPlateSafe();
            }
            else
            {
                bMoveAllZ=MoveOutArmZToPlateSafe();
            }
            if(bMoveAllZ==true)
            {
                s.sprintf("Alignment Mode：%d",iMode);
                WriteAlignFlowLog(s);
                if(iMode==1)//僅跑 Teach Mark Flow
                {
                    WriteAlignFlowLog("Teach Mark Calibration Start");
                    DoTeachMarkCalibration(true);
                    Task=600;
                }
                else
                {
                    WriteAlignFlowLog("Bottom CCD Calibration Start");
                    DoBottomCCDCalibration(true);
                    Task=200;
                }
            }
            break;
        case 200:
            iBottomCCDOK=DoBottomCCDCalibration();
            if(iBottomCCDOK==1)
            {
                WriteAlignFlowLog("Top CCD Calibration Start");
                DoTopCCDCalibration(true);
                Task=300;
            }
            else if(iBottomCCDOK==0)
            {

            }
            else
            {
                ShowMyMessage("Bottom CCD error on dummy IC position!!");
                DoBottomCCDCalibration(true);
            }
            break;
        case 300:
            iTopCCDOK=DoTopCCDCalibration();
            if(iTopCCDOK==1)
            {
                WriteAlignFlowLog("Base Picker Calibration Start");
                DoBasePickerCalibration(true);
                Task=400;
            }
            else if(iTopCCDOK==0)
            {

            }
            else
            {
                ShowMyMessage("Top CCD error on dummy IC position!!");
                DoTopCCDCalibration(true);
            }
            break;
        case 400:
            iBasePickerOK=DoBasePickerCalibration();
            if(iBasePickerOK==1)
            {
                WriteAlignFlowLog("Picker Pitch Calibration Start");
                DoPickerPitchCalibration(true);
                Task=500;
            }
            else if(iBasePickerOK==0)
            {

            }
            else
            {
                ShowMyMessage("Top CCD to Base Picker calibration error!!");
                DoBasePickerCalibration(true);
            }
            break;
        case 500:
            iPickerPitch=DoPickerPitchCalibration();
            if(iPickerPitch==1)
            {
                WriteAlignFlowLog("Teach Mark Calibration Start");
                DoTeachMarkCalibration(true);
                Task=600;
            }
            else if(iPickerPitch==0)
            {

            }
            else
            {
                ShowMyMessage("Bottom CCD to Picker Pitch calibration error!!");
                DoPickerPitchCalibration(true);
            }
            break;
        case 600:
            iTeachMarkOK=DoTeachMarkCalibration();
            if(iTeachMarkOK==1)
            {
                WriteAlignFlowLog("Alignment Done");
                Task=700;
            }
            else if(iTeachMarkOK==0)
            {

            }
            else
            {
                ShowMyMessage("Top CCD to Teach Mark calibration error!!");
                DoTeachMarkCalibration(true);
            }
            break;
        case 700:
            iReturn=1;
            break;
    }

    return iReturn;
}
//---------------------------------------------------------------------------
int TCAlignmentB::DoBottomCCDCalibration(bool bReset)
{
    int iReturn=0;

    int iTempX,iTempY,iTempX1,iTempY1;
    int iZptr[8]={0, 0, 0, 0, 0, 0, 0, 0};
    bool bZptr[8]={false, false, false, false, false, false, false, false};
    int &Task=iBottomCCDCalibrationTask;
    iReturn=0;
    if(bReset==true)
    {
        Task=1;
        return iReturn;
    }
    switch(Task)
    {
        case 1:
            InitMoveArmXYToAccuracyPosTask();
            Task=100;
            break;
        case 100:
            bZptr[ePickerE]=true;
            iZptr[ePickerE]=TestIF_File.dOutBottomPixelSizeX;

            if(_IO==0)
            {
               iTempX=Prod.iInTop2DummyX;
               iTempY=Prod.iInTop2DummyY;
            }
            else
            {
               iTempX=Prod.iOutTop2DummyX;
               iTempY=Prod.iOutTop2DummyY;
            }

            iArmMoveXYZ=MoveArmXYToAccuracyPos(1,1,true,iTempX,iTempY,4000,&bZptr[0],&iZptr[0],true);



            if(_IO==0)
            {
                iTempX1=Prod.iInBasePickerOnBottomX;
                iTempY1=Prod.iInBasePickerOnBottomY;
            }
            else
            {
                iTempX1=Prod.iOutBasePickerOnBottomX;
                iTempY1=Prod.iOutBasePickerOnBottomY;
            }


            if(iArmMoveXYZ==1)
            {
                iBasePickerOnBottomAccX=MOT[_MArmX].ReadPos();
                iBasePickerOnBottomAccY=MOT[_MArmY].ReadPos();
                iBasePickerOnBottomOfsX=iBasePickerOnBottomAccX-iTempX1;
                iBasePickerOnBottomOfsY=iBasePickerOnBottomAccY-iTempY1;
                InitMoveArmXYToAccuracyPosTask();
                Task=200;
            }
            else if(iArmMoveXYZ==0)
            {

            }
            else
            {
                ShowMyMessage("InBottom CCD error on basePicker position!!");
                InitMoveArmXYToAccuracyPosTask();
            }
            break;
        case 200:
            bZptr[ePickerE]=true;
            if(_IO==0)
            {
                iZptr[ePickerE]=Prod.iInBasePickerOnBottomZ;
            }
            else
            {
                iZptr[ePickerE]=Prod.iOutBasePickerOnBottomZ;
            }
            iTempX=iBasePickerOnBottomAccX+200;
            iTempY=iBasePickerOnBottomAccY-200;
            iArmMoveXYZ=MoveArmXYToAccuracyPos(1,1,false,iTempX,iTempY,4000,&bZptr[0],&iZptr[0],true);

            if(iArmMoveXYZ==1)
            {

                dLUx=vcAlign[1]->dCenterOffsetPixelX;
                dLUy=vcAlign[1]->dCenterOffsetPixelY;
                if(bEnableVisionSimulate)
                {
                    dLUx=200;
                    dLUy=-200;
                }
                InitMoveArmXYToAccuracyPosTask();
                Task=300;
            }
            break;
        case 300:
            bZptr[ePickerE]=true;
            if(_IO=0)
            {
                iZptr[ePickerE]=Prod.iInBasePickerOnBottomZ;
            }
            else
            {
                iZptr[ePickerE]=Prod.iOutBasePickerOnBottomZ;
            }
            iTempX=iBasePickerOnBottomAccX-200;
            iTempY=iBasePickerOnBottomAccY+200;
            iArmMoveXYZ=MoveArmXYToAccuracyPos(0,1,false,iTempX,iTempY,4000,&bZptr[0],&iZptr[0],true);
            if(iArmMoveXYZ==1)
            {
                dRDx=vcAlign[0]->dCenterOffsetPixelX;
                dRDy=vcAlign[0]->dCenterOffsetPixelY;
                if(bEnableVisionSimulate)
                {
                    dRDx=-200;
                    dRDy=200;
                }
                Task=400;
            }
            break;
        case 400:
            if(_IO==0)
            {
                TestIF_File.dInBottomPixelSizeX=ChangeToFloatNonPcnt((double)(400.0), (double)(abs(dLUx-dRDx)));
                TestIF_File.dInBottomPixelSizeY=ChangeToFloatNonPcnt((double)(400.0), (double)(abs(dLUy-dRDy)));
                WriteAlignFlowLog("In Bottom CCD Calibration Done");
            }
            else
            {
                TestIF_File.dOutBottomPixelSizeX=ChangeToFloatNonPcnt((double)(400.0), (double)(abs(dLUx-dRDx)));
                TestIF_File.dOutBottomPixelSizeY=ChangeToFloatNonPcnt((double)(400.0), (double)(abs(dLUy-dRDy)));
                WriteAlignFlowLog("Out Bottom CCD Calibration Done");
            }


            Task=500;
            break;
        case 500:
            iReturn=1;
            break;

    }

    return iReturn;
}
//---------------------------------------------------------------------------
int TCAlignmentB::DoTopCCDCalibration(bool bReset)
{
    int iReturn=0;

    int iTempX,iTempY;
    int iZptr[8]={0, 0, 0, 0, 0, 0, 0, 0};
    bool bZptr[8]={false, false, false, false, false, false, false, false};
    int &Task=iTopCCDCalibrationTask;
    iReturn=0;
    if(bReset==true)
    {
        Task=1;
        return iReturn;
    }
    switch(Task)
    {
        case 1:
            InitMoveArmXYToAccuracyPosTask();
            Task=100;
            break;
        case 100:

            if(_IO==0)
            {
                iTempX=Prod.iInTop2DummyX;
                iTempY=Prod.iInTop2DummyY;
            }
            else
            {
                iTempX=Prod.iOutTop2DummyX;
                iTempY=Prod.iOutTop2DummyY;
            }



            iArmMoveXYZ=MoveArmXYToAccuracyPos(0,3,true,iTempX,iTempY,4000,&bZptr[0],&iZptr[0],false);
            if(iArmMoveXYZ==1)
            {
                iTop2DummyAccX=MOT[_MArmX].ReadPos();
                iTop2DummyAccY=MOT[_MArmY].ReadPos();
                iTop2DummyOfsX=iTop2DummyAccX-iTempX;
                iTop2DummyOfsY=iTop2DummyAccY-iTempY;
                InitMoveArmXYToAccuracyPosTask();
                Task=200;
            }
            else if(iArmMoveXYZ==0)
            {

            }
            else
            {
                ShowMyMessage("Top CCD error on Dummy IC position!!");
                InitMoveArmXYToAccuracyPosTask();
            }
            break;
        case 200:
            iTempX=iTop2DummyAccX+200;
            iTempY=iTop2DummyAccY-200;
            iArmMoveXYZ=MoveArmXYToAccuracyPos(0,3,false,iTempX,iTempY,4000,&bZptr[0],&iZptr[0],false);
            if(iArmMoveXYZ==1)
            {
                dLUx=vcAlign[0]->dCenterOffsetPixelX;
                dLUy=vcAlign[0]->dCenterOffsetPixelY;
                if(bEnableVisionSimulate)
                {
                    dLUx=200;
                    dLUy=-200;
                }
                InitMoveArmXYToAccuracyPosTask();
                Task=300;
            }
            break;
        case 300:
            iTempX=iTop2DummyAccX-200;
            iTempY=iTop2DummyAccY+200;

            iArmMoveXYZ=MoveArmXYToAccuracyPos(0,3,false,iTempX,iTempY,4000,&bZptr[0],&iZptr[0],false);
            if(iArmMoveXYZ==1)
            {
                dRDx=vcAlign[_IO]->dCenterOffsetPixelX;
                dRDy=vcAlign[_IO]->dCenterOffsetPixelY;
                if(bEnableVisionSimulate)
                {
                    dRDx=-200;
                    dRDy=200;
                }
                Task=400;
            }
            break;
        case 400:
            if(_IO=0)
            {
                TestIF_File.dInTopPixelSizeX=ChangeToFloatNonPcnt((double)(400.0), (double)(abs(dLUx-dRDx)));
                TestIF_File.dInTopPixelSizeY=ChangeToFloatNonPcnt((double)(400.0), (double)(abs(dLUy-dRDy)));
                WriteAlignFlowLog("In Top CCD Calibration Done");
            }
            else
            {
                TestIF_File.dOutTopPixelSizeX=ChangeToFloatNonPcnt((double)(400.0), (double)(abs(dLUx-dRDx)));
                TestIF_File.dOutTopPixelSizeY=ChangeToFloatNonPcnt((double)(400.0), (double)(abs(dLUy-dRDy)));
                WriteAlignFlowLog("Out Top CCD Calibration Done");
            }
            Task=500;

            break;
        case 500:
            iReturn=1;
            break;

    }

    return iReturn;
}
//---------------------------------------------------------------------------
int TCAlignmentB::DoBasePickerCalibration(bool bReset)
{
    int iReturn=0;

    int iVari,iYVari=0,iVari2=0;
    int iTempX,iTempY;
    int iZptr[8]={0, 0, 0, 0, 0, 0, 0, 0};
    bool bZptr[8]={false, false, false, false, false, false, false, false};
    bool bMoveXYZ;
    int &Task=iBasePickerCalibrationTask;
    iReturn=0;
    if(bReset==true)
    {
        Task=1;
        return iReturn;
    }
    switch(Task)
    {
        case 1:
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
            {
                iYVari=MOT[_MArmPitchY].ReadPos();
                iVari2=MOT[_MArmPitchX2].ReadPos();
            }

            bZptr[ePickerE]=true;
            iTempX=iTop2DummyAccX;
            iTempY=iTop2DummyAccY;
            if(_IO==0)
            {
                iZptr[ePickerE]=Prod.iInDummyZ;
            }
            else
            {
                iZptr[ePickerE]=Prod.iOutDummyZ;
            }
            if(asType=="IN")
            {
                iVari=GetInArmPitch_9045(4000);
                bMoveXYZ=InArmContinuousMove(iTempX, iTempY, iVari, &bZptr[0], &iZptr[0], true, iYVari, iVari2);
            }
            else
            {
                iVari=GetOutArmPitch_9045(4000);
                bMoveXYZ=OutArmContinuousMove(iTempX, iTempY, iVari, &bZptr[0], &iZptr[0], true, iYVari, iVari2);
            }


            if(bMoveXYZ==true)
            {
                tInCCDMoveDelay.SetMSAndOn(200);
                Task=100;
            }
            break;
        case 100:
            if(tInCCDMoveDelay.Off())
            {
                if(asType=="IN")
                {
                    if(InArmSuck.Suck[0][2].Suck())
                    {
                        InitMoveArmXYToAccuracyPosTask();
                        Task=200;
                    }
                    else if(InArmSuck.Suck[0][2].Error==true)
                    {
                        ShowMyMessage("In BasePicker Vacuum Dummy IC error!!");
                        Task=1;
                    }
                }
                else
                {
                    if(OutArmSuck.Suck[0][2].Suck())
                    {
                        InitMoveArmXYToAccuracyPosTask();
                        Task=200;
                    }
                    else if(OutArmSuck.Suck[0][2].Error==true)
                    {
                        ShowMyMessage("Out BasePicker Vacuum Dummy IC error!!");
                        Task=1;
                    }
                }
            }
            break;
        case 200:
            bZptr[ePickerE]=true;
            if(_IO==0)
            {
                iZptr[ePickerE]=Prod.iInBasePickerOnBottomZ+20;//Dummy IC 厚度。
            }
            else
            {
                iZptr[ePickerE]=Prod.iOutBasePickerOnBottomZ+20;//Dummy IC 厚度。
            }
            iTempX=iBasePickerOnBottomAccX;
            iTempY=iBasePickerOnBottomAccY;
            iArmMoveXYZ=MoveArmXYToAccuracyPos(1,1,true,iTempX,iTempY,4000,&bZptr[0],&iZptr[0],true);

            if(iArmMoveXYZ==1)
            {
                iTop2BasePickerOfsX=MOT[_MArmX].ReadPos()-iTop2BasePickerAccX;
                iTop2BasePickerOfsY=MOT[_MArmY].ReadPos()-iTop2BasePickerAccY;
                iTop2BasePickerAccX=iTempX+iTop2BasePickerOfsX;
                iTop2BasePickerAccY=iTempY+iTop2BasePickerOfsY;
                Task=300;
            }
            else if(iArmMoveXYZ==0)
            {

            }
            else
            {
                ShowMyMessage("Bottom CCD error on Dummy IC position!!");
                InitMoveArmXYToAccuracyPosTask();
            }
            break;
        case 300:
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
            {
                iYVari=MOT[_MArmPitchY].ReadPos();
                iVari2=MOT[_MArmPitchX2].ReadPos();
            }
            bZptr[ePickerE]=true;
            if(_IO==0)
            {
                iZptr[ePickerE]=Prod.iInDummyZ;
                iTempX=iTop2DummyAccX+Prod.iInTop2BasePickerX;
                iTempY=iTop2DummyAccY+Prod.iInTop2BasePickerY;

            }
            else
            {
                iZptr[ePickerE]=Prod.iOutDummyZ;
                iTempX=iTop2DummyAccX+Prod.iOutTop2BasePickerX;
                iTempY=iTop2DummyAccY+Prod.iOutTop2BasePickerY;
            }



            if(asType=="IN")
            {
                iVari=GetInArmPitch_9045(4000);
                bMoveXYZ=InArmContinuousMove(iTempX, iTempY, iVari, &bZptr[0], &iZptr[0], true, iYVari, iVari2);
            }
            else
            {
                iVari=GetOutArmPitch_9045(4000);
                bMoveXYZ=OutArmContinuousMove(iTempX, iTempY, iVari, &bZptr[0], &iZptr[0], true, iYVari, iVari2);
            }

            if(bMoveXYZ==true)
            {
                tInCCDMoveDelay.SetMSAndOn(200);
                Task=400;
            }
            break;
        case 400:
            if(tInCCDMoveDelay.Off())
            {
                if(asType=="IN")
                {
                    if(InArmSuck.Suck[0][2].Destroy())
                    {
                        WriteAlignFlowLog("Base Picker Calibration Done");
                        Task=500;
                    }
                    else if(InArmSuck.Suck[0][2].Error==true)
                    {
                        WriteAlignFlowLog("Base Picker Calibration Warnning, Dummy may lose.... ");
                        Task=500;
                    }
                }
                else
                {
                    if(OutArmSuck.Suck[0][2].Destroy())
                    {
                        WriteAlignFlowLog("Base Picker Calibration Done");
                        Task=500;
                    }
                    else if(OutArmSuck.Suck[0][2].Error==true)
                    {
                        WriteAlignFlowLog("Base Picker Calibration Warnning, Dummy may lose.... ");
                        Task=500;
                    }
                }
            }
            break;
        case 500:
            iReturn=1;
            break;
    }

    return iReturn;
}
//---------------------------------------------------------------------------
int TCAlignmentB::DoPickerPitchCalibration(bool bReset)
{
   int iReturn=0;

    int iTempX,iTempY;
    int iZptr[8]={0, 0, 0, 0, 0, 0, 0, 0};
    bool bZptr[8]={false, false, false, false, false, false, false, false};
    int &Task=iPickerPitchCalibrationTask;
    iReturn=0;
    if(bReset==true)
    {
        IsCalAlignPitchOK=false;
        iPicker=0;
        Task=1;
        return iReturn;
    }
    switch(Task)
    {
        case 1:     //計算 XPitch X40 X120 時吸嘴位置
            SetXPitchTeach(iBasePickerOnBottomAccX,iBasePickerOnBottomAccY);
            if(asType=="IN")
            {
                Prod.iInArmX40Pitch =Tech.iInArmX40Pitch;
                Prod.iInArmX120Pitch=Tech.iInArmX120Pitch;
            }
            else
            {
                Prod.iOutArmX40Pitch =Tech.iOutArmX40Pitch;
                Prod.iOutArmX120Pitch=Tech.iOutArmX120Pitch;
            }

            iPicker=0;
            InitMoveArmXYToAccuracyPosTask();
            Task=100;
            break;
        case 100:  //找尋 XPitch X40 時吸嘴實際位置
            bZptr[iPicker]=true;
            if(_IO==0)
            {
                iZptr[iPicker]=Prod.iInBasePickerOnBottomZ;
            }
            else
            {
                iZptr[iPicker]=Prod.iOutBasePickerOnBottomZ;
            }
            iTempX=iX40PitchTeachX[iPicker];
            iTempY=iX40PitchTeachY[iPicker];

            iArmMoveXYZ=MoveArmXYToAccuracyPos(1,1,true,iTempX,iTempY,4000,&bZptr[0],&iZptr[0],true);
            if(iArmMoveXYZ==1)
            {
                iX40PitchTeachAccX[iPicker]=MOT[_MArmX].ReadPos();
                iX40PitchTeachAccY[iPicker]=MOT[_MArmY].ReadPos();
                iX40PitchTeachOfsX[iPicker]=MOT[_MArmX].ReadPos() - iX40PitchTeachX[iPicker];
                iX40PitchTeachOfsY[iPicker]=MOT[_MArmY].ReadPos() - iX40PitchTeachY[iPicker];
                iPicker++;
                if(iPicker<ePickerTotal)
                {
                    InitMoveArmXYToAccuracyPosTask();
                    Task=100;
                }
                else
                {
                    iPicker=0;
                    InitMoveArmXYToAccuracyPosTask();
                    Task=200;
                }
//                fMain->Pause();
            }
            else if(iArmMoveXYZ==0)
            {

            }
            else
            {
                ShowMyMessage("Bottom CCD error on X40 Suck!!");
                InitMoveArmXYToAccuracyPosTask();
            }
            break;
        case 200:      //找尋 XPitch X120 時吸嘴實際位置
            bZptr[iPicker]=true;
            if(_IO==0)
            {
                iZptr[iPicker]=Prod.iInBasePickerOnBottomZ;
            }
            else
            {
                iZptr[iPicker]=Prod.iOutBasePickerOnBottomZ;
            }

            iTempX=iX120PitchTeachX[iPicker];
            iTempY=iX120PitchTeachY[iPicker];

            iArmMoveXYZ=MoveArmXYToAccuracyPos(1,1,true,iTempX,iTempY,12000,&bZptr[0],&iZptr[0],true);

            if(iArmMoveXYZ==1)
            {
                iX120PitchTeachAccX[iPicker]=MOT[_MArmX].ReadPos();
                iX120PitchTeachAccY[iPicker]=MOT[_MArmY].ReadPos();
                iX120PitchTeachOfsX[iPicker]=MOT[_MArmX].ReadPos() - iX120PitchTeachX[iPicker];
                iX120PitchTeachOfsY[iPicker]=MOT[_MArmY].ReadPos() - iX120PitchTeachY[iPicker];
                iPicker++;
                if(iPicker<ePickerTotal)
                {
                    InitMoveArmXYToAccuracyPosTask();
                    Task=200;
                }
                else
                {
                    if(IsCalAlignPitchOK==false)
                    {
                        WriteAlignFlowLog("Picker Pitch Calibration Done1");
                        Task=300;
                    }
                    else
                    {
                        WriteAlignFlowLog("Picker Pitch Calibration Done2");
                        Task=400;
                    }
                }
            }
            else if(iArmMoveXYZ==0)
            {

            }
            else
            {
                ShowMyMessage("Bottom CCD error on X120Suck!!");
                InitMoveArmXYToAccuracyPosTask();
            }
            break;
        case 300:       //計算 X40 X120 平均 Count，再以平均在跑一次 X40 X120 吸嘴實際位置。
            if(asType=="IN")
            {
                CalAlignPitch(Prod.iInArmX40Pitch,Prod.iInArmX120Pitch);
                Prod.iInArmX40Pitch =iX40PitchCountAvg;
                Prod.iInArmX120Pitch=iX120PitchCountAvg;
            }
            else
            {
                CalAlignPitch(Prod.iOutArmX40Pitch,Prod.iOutArmX120Pitch);
                Prod.iOutArmX40Pitch =iX40PitchCountAvg;
                Prod.iOutArmX120Pitch=iX120PitchCountAvg;
            }
            IsCalAlignPitchOK=true;
            iPicker=0;
            InitMoveArmXYToAccuracyPosTask();
            Task=100;
            break;
        case 400:
            if(asType=="IN")
            {
                Prod.iInArmX40Pitch =Tech.iInArmX40Pitch;
                Prod.iInArmX120Pitch=Tech.iInArmX40Pitch;
            }
            else
            {
                Prod.iOutArmX40Pitch =Tech.iOutArmX40Pitch;
                Prod.iOutArmX120Pitch=Tech.iOutArmX40Pitch;
            }
            iReturn=1;
            break;
    }
    return iReturn;
}
//---------------------------------------------------------------------------
int TCAlignmentB::DoTeachMarkCalibration(bool bReset)
{
    int iReturn=0;
    int iTempX,iTempY;
    int iZptr[8]={0, 0, 0, 0, 0, 0, 0, 0};
    bool bZptr[8]={false, false, false, false, false, false, false, false};

    int &Task=iTeachMarkCalibrationTask;
    iReturn=0;
    if(bReset==true)
    {
        iMark=0;
        Task=1;
        return iReturn;
    }
    switch(Task)
    {
        case 1:
            iMark=0;
            bShutMoveOK1=false;
            bShutMoveOK2=false;
            InitMoveArmXYToAccuracyPosTask();
            Task=50;
            break;
        case 50:
            //if((asType=="IN" && bShuttleIsBusy[1]==false) || (asType=="OUT" && bShuttleIsBusy[0]==false))
            if(asType=="OUT" && bShuttleIsBusy[0]==false)
            {
                bShuttleIsBusy[_IO]=true;    //Shuttle 被使用著 Lock
                if(asType=="IN")
                {
                    if(bShutMoveOK1==false)
                    {
                        bShutMoveOK1=MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft);
                    }
                    if(bShutMoveOK2==false)
                    {
                        bShutMoveOK2=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft);
                    }
                }
                else
                {
                    if(bShutMoveOK1==false)
                    {
                        bShutMoveOK1=MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iRight);
                    }
                    if(bShutMoveOK2==false)
                    {
                        bShutMoveOK2=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight);
                    }
                }
            }

            if(bShutMoveOK1==true && bShutMoveOK2==true)
            {
                Task=100;
            }
            break;
        case 100:
            if(_IO==0)
            {
                iTempX=Prod.iInMarkX[iMark];
                iTempY=Prod.iInMarkY[iMark];
            }
            else
            {
                iTempX=Prod.iOutMarkX[iMark];
                iTempY=Prod.iOutMarkY[iMark];
            }
            iArmMoveXYZ=MoveArmXYToAccuracyPos(0,3,true,iTempX,iTempY,4000,&bZptr[0],&iZptr[0],false);
            if(iArmMoveXYZ==1)
            {
                iMarkAccX[iMark]=MOT[_MArmX].ReadPos();
                iMarkAccY[iMark]=MOT[_MArmY].ReadPos();
                iMarkOfsX[iMark]=MOT[_MArmX].ReadPos()-iTempX;
                iMarkOfsY[iMark]=MOT[_MArmY].ReadPos()-iTempY;
                iMark++;
                if((asType=="IN"  && iMark<eITeachMarkTotal) ||
                   (asType=="OUT" && iMark<eOTeachMarkTotal))
                {
                    if(iMark==2) //eHP_1 or eAuto1_1
                    {
                        bShuttleIsBusy[_IO]=false; //Shuttle 使用完後釋放掉
                    }

                    InitMoveArmXYToAccuracyPosTask();
                    Task=100;
                }
                else
                {
                    Task=200;
                }
            }
            else if(iArmMoveXYZ==0)
            {

            }
            else
            {
                ShowMyMessage("Top CCD error on Teach Mark!!");
                InitMoveArmXYToAccuracyPosTask();
            }
            break;
        case 200:
            Task=300;
            WriteAlignFlowLog("Teach Mark Calibration Done");
            break;
        case 300:
            iReturn=1;
            break;
    }

    return iReturn;
}
//---------------------------------------------------------------------------
void TCAlignmentB::InitMoveArmXYToAccuracyPosTask()
{
    iMoveArmXYToAccuracyPosTask=1;
    iArmMoveXYZ=0;
}
//---------------------------------------------------------------------------
int TCAlignmentB::MoveArmXYToAccuracyPos(int iCCD,int iMatchType,bool bCenter,int iTeachx,int iTeachy,int iXpitch,bool *ZDownSel,int *ZPos,bool ZNeedDown)
{
    int iVari,iYVari=0,iVari2=0;
    //int iZptr[8]={0, 0, 0, 0, 0, 0, 0, 0};
    double dx,dy;
    //bool bZptr[8]={false, false, false, false, false, false, false, false};
    bool bRet,bMoveXYZ;
    int itemp;
    AnsiString strSendCMD="";

    int &Task=iMoveArmXYToAccuracyPosTask;
    iReturn=0;

    if(iCCD<0 && iCCD>1) //iCCD=0 TopCCD ; iCCD=1 BottomCCD
    {
        iReturn=-10;     //表示輸入參數錯誤
        return iReturn;
    }

    switch(Task)
    {
        case 1:
            ix=iTeachx;
            iy=iTeachy;
            Task=100;
            break;
        case 100:       //1. 移動到第 Teach 點
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
            {
                iYVari=MOT[_MArmPitchY].ReadPos();
                iVari2=MOT[_MArmPitchX2].ReadPos();
            }

            if(asType=="IN")
            {
                iVari=GetInArmPitch_9045(iXpitch);
                bMoveXYZ=InArmContinuousMove(ix, iy, iVari, &ZDownSel[0], &ZPos[0], ZNeedDown, iYVari, iVari2);
            }
            else
            {
                iVari=GetOutArmPitch_9045(iXpitch);
                bMoveXYZ=OutArmContinuousMove(ix, iy, iVari, &ZDownSel[0], &ZPos[0], ZNeedDown, iYVari, iVari2);
            }

            if(bMoveXYZ==true)
            {
                tInCCDMoveDelay.SetMSAndOn(300);
                Task=200;
            }
            break;
        case 200:       //2. 到位整定後拍照
            if(tInCCDMoveDelay.Off())
            {

                if(asType=="IN")
                {
                    itemp=iCCD;
                }
                else
                {
                    itemp=iCCD+2;
                }
                vcAlign[iCCD]->iResult=0;
                strSendCMD.sprintf("E1,7,%d,%d",itemp,iMatchType);       //E1,7,0,0....  In Top 拍。
                vcAlign[iCCD]->SendCommand(" ", strSendCMD);
                Task=300;
            }
            break;
        case 300:       //3. 收到拍照完成
            if(bEnableVisionSimulate==true)
            {
                vcAlign[iCCD]->iResult=1;
                vcAlign[iCCD]->dCenterOffsetPixelX=0;
                vcAlign[iCCD]->dCenterOffsetPixelY=0;
            }
            if(vcAlign[iCCD]->iResult==1)
            {
                if(bCenter==false)
                {
                    iReturn=1;
                    Task=1000;
                    break;
                }

                if(_IO==0 && iCCD==0)       // In Top CCD
                {
                    dx=TestIF_File.dInTopPixelSizeX;
                    dy=TestIF_File.dInTopPixelSizeY;
                }
                else if(_IO==0 && iCCD==1)  // In Bottom CCD
                {
                    dx=TestIF_File.dInBottomPixelSizeX;
                    dy=TestIF_File.dInBottomPixelSizeY;
                }
                else if(_IO==1 && iCCD==0)  // Out Top CCD
                {
                    dx=TestIF_File.dOutTopPixelSizeX;
                    dy=TestIF_File.dOutTopPixelSizeY;
                }
                else if(_IO==1 && iCCD==1)  // Out Bottom CCD
                {
                    dx=TestIF_File.dOutBottomPixelSizeX;
                    dy=TestIF_File.dOutBottomPixelSizeY;
                }

                if(abs(vcAlign[iCCD]->dCenterOffsetPixelX*dx)>TestIF_File.iAccuracy || abs(vcAlign[iCCD]->dCenterOffsetPixelY*dy)>TestIF_File.iAccuracy)
                    bRet=true;
                else
                    bRet=false;

                if(bRet==true)          // 3. 檢查是否在影像中心，誤差內補償
                {
                    iReturn=0;
                    ix+=vcAlign[iCCD]->dCenterOffsetPixelX*dx;
                    iy+=vcAlign[iCCD]->dCenterOffsetPixelY*dy;
                    Task=100;
                }
                else
                {
                    iReturn=1;
                    Task=1000;
                }
            }
            else
            {
                 iReturn=vcAlign[iCCD]->iResult;
            }
            break;
        case 1000:

            break;
    }
    return iReturn;
}
//---------------------------------------------------------------------------
void TCAlignmentB::SetXPitchTeach(int iBasePickerOnBottomCCDX,int iBasePickerOnBottomCCDY)
{
    int iXPitchMax=12000;   //120mm
    int iXPitchMin=4000;    //40mm
    int iYPitch=6000;       //60mm
    //取得 BottomCCD 看 In Arm 吸嘴的 Teach 點，用來計算變距是否異常
    /*In Arm Picker Teach 順序(變距最小)
     A C E G
     B D F H
    */
    iX40PitchTeachX[ePickerA]=iBasePickerOnBottomCCDX+(iXPitchMin/3)*2;           //A
    iX40PitchTeachY[ePickerA]=iBasePickerOnBottomCCDY;
    iX40PitchTeachX[ePickerB]=iBasePickerOnBottomCCDX+(iXPitchMin/3)*2;           //B
    iX40PitchTeachY[ePickerB]=iBasePickerOnBottomCCDY+iYPitch;
    iX40PitchTeachX[ePickerC]=iBasePickerOnBottomCCDX+(iXPitchMin/3);             //C
    iX40PitchTeachY[ePickerC]=iBasePickerOnBottomCCDY;
    iX40PitchTeachX[ePickerD]=iBasePickerOnBottomCCDX+(iXPitchMin/3);             //D
    iX40PitchTeachY[ePickerD]=iBasePickerOnBottomCCDY+iYPitch;
    iX40PitchTeachX[ePickerE]=iBasePickerOnBottomCCDX;                            //E
    iX40PitchTeachY[ePickerE]=iBasePickerOnBottomCCDY;
    iX40PitchTeachX[ePickerF]=iBasePickerOnBottomCCDX;                            //F
    iX40PitchTeachY[ePickerF]=iBasePickerOnBottomCCDY+iYPitch;
    iX40PitchTeachX[ePickerG]=iBasePickerOnBottomCCDX-(iXPitchMin/3);             //G
    iX40PitchTeachY[ePickerG]=iBasePickerOnBottomCCDY;
    iX40PitchTeachX[ePickerH]=iBasePickerOnBottomCCDX-(iXPitchMin/3);             //H
    iX40PitchTeachY[ePickerH]=iBasePickerOnBottomCCDY+iYPitch;
    /*In Arm Picker Teach 順序(變距最大)
     A C E G
     B D F H
    */
    //8~10
    iX120PitchTeachX[ePickerA]=iBasePickerOnBottomCCDX+(iXPitchMax/3)*2;          //A
    iX120PitchTeachY[ePickerA]=iBasePickerOnBottomCCDY;
    iX120PitchTeachX[ePickerB]=iBasePickerOnBottomCCDX+(iXPitchMax/3)*2;          //B
    iX120PitchTeachY[ePickerB]=iBasePickerOnBottomCCDY+iYPitch;
    iX120PitchTeachX[ePickerC]=iBasePickerOnBottomCCDX+(iXPitchMax/3);            //C
    iX120PitchTeachY[ePickerC]=iBasePickerOnBottomCCDY;
    iX120PitchTeachX[ePickerD]=iBasePickerOnBottomCCDX+(iXPitchMax/3);            //D
    iX120PitchTeachY[ePickerD]=iBasePickerOnBottomCCDY+iYPitch;
    iX120PitchTeachX[ePickerE]=iBasePickerOnBottomCCDX;                           //E
    iX120PitchTeachY[ePickerE]=iBasePickerOnBottomCCDY;
    iX120PitchTeachX[ePickerF]=iBasePickerOnBottomCCDX;                           //F
    iX120PitchTeachY[ePickerF]=iBasePickerOnBottomCCDY+iYPitch;
    iX120PitchTeachX[ePickerG]=iBasePickerOnBottomCCDX-(iXPitchMax/3);            //G
    iX120PitchTeachY[ePickerG]=iBasePickerOnBottomCCDY;
    iX120PitchTeachX[ePickerH]=iBasePickerOnBottomCCDX-(iXPitchMax/3);            //H
    iX120PitchTeachY[ePickerH]=iBasePickerOnBottomCCDY+iYPitch;
}
//---------------------------------------------------------------------------
void TCAlignmentB::CalAlignPitch(int X40Pitch,int X120Pitch)
{
    //AnsiString s;
   // s.sprintf("CalculationInArmAlignmentPitch()");
    //WriteAlignLog(s);
    double dX120ToX40PitchCount;
    double dPitchScaleX;
    int i,iCnt;
    //Pitch Count Result
    //PickerA PickerC PickerE PickerG            iBasePickerEx
    //PickerB PickerD PickerF PickerH
    int iClosePickerAx=iX40PitchTeachAccX[ePickerA];
    int iClosePickerBx=iX40PitchTeachAccX[ePickerB];
    int iClosePickerCx=iX40PitchTeachAccX[ePickerC];
    int iClosePickerDx=iX40PitchTeachAccX[ePickerD];
    int iClosePickerEx=iX40PitchTeachAccX[ePickerE];
    //int iClosePickerFx=vX40PitchTeach[ePickerF].iAccX;
    int iClosePickerGx=iX40PitchTeachAccX[ePickerG];
    int iClosePickerHx=iX40PitchTeachAccX[ePickerH];

    int iOpenPickerAx=iX120PitchTeachAccX[ePickerA];
    int iOpenPickerBx=iX120PitchTeachAccX[ePickerB];
    int iOpenPickerCx=iX120PitchTeachAccX[ePickerC];
    int iOpenPickerDx=iX120PitchTeachAccX[ePickerD];
    int iOpenPickerEx=iX120PitchTeachAccX[ePickerE];
    //int iOpenPickerFx=vX120PitchTeach[ePickerF].iAccX;
    int iOpenPickerGx=iX120PitchTeachAccX[ePickerG];
    int iOpenPickerHx=iX120PitchTeachAccX[ePickerH];

    dX120ToX40PitchCount=X120Pitch-X40Pitch;
    for(int i=0; i<ePickerTotal; i++)
    {
        iX40PitchCount [i]=X40Pitch;
        iX120PitchCount[i]=X120Pitch;
    }
    dPitchScaleX=ChangeToFloatNonPcnt((double)((iOpenPickerAx-iClosePickerAx)), (double)(dX120ToX40PitchCount));
    iX40PitchCount [ePickerA] += ChangeToFloatNonPcnt((double)(((2* 4000/3)-(iClosePickerAx-iClosePickerEx ))), (double)(dPitchScaleX));
    iX120PitchCount[ePickerA] += ChangeToFloatNonPcnt((double)(((2*12000/3)-( iOpenPickerAx-iOpenPickerEx  ))), (double)(dPitchScaleX));

    dPitchScaleX=ChangeToFloatNonPcnt((double)((iOpenPickerBx-iClosePickerBx)), (double)(dX120ToX40PitchCount));
    iX40PitchCount [ePickerB] += ChangeToFloatNonPcnt((double)(((2* 4000/3)-(iClosePickerBx-iClosePickerEx ))), (double)(dPitchScaleX));
    iX120PitchCount[ePickerB] += ChangeToFloatNonPcnt((double)(((2*12000/3)-( iOpenPickerBx-iOpenPickerEx  ))), (double)(dPitchScaleX));

    dPitchScaleX=ChangeToFloatNonPcnt((double)((iOpenPickerCx-iClosePickerCx)), (double)(dX120ToX40PitchCount));
    iX40PitchCount [ePickerC] += ChangeToFloatNonPcnt((double)(((   4000/3)-(iClosePickerCx-iClosePickerEx ))), (double)(dPitchScaleX));
    iX120PitchCount[ePickerC] += ChangeToFloatNonPcnt((double)(((  12000/3)-( iOpenPickerCx-iOpenPickerEx  ))), (double)(dPitchScaleX));

    dPitchScaleX=ChangeToFloatNonPcnt((double)((iOpenPickerDx-iClosePickerDx)), (double)(dX120ToX40PitchCount));
    iX40PitchCount [ePickerD] += ChangeToFloatNonPcnt((double)(((   4000/3)-(iClosePickerDx-iClosePickerEx ))), (double)(dPitchScaleX));
    iX120PitchCount[ePickerD] += ChangeToFloatNonPcnt((double)(((  12000/3)-( iOpenPickerDx-iOpenPickerEx  ))), (double)(dPitchScaleX));

    dPitchScaleX=ChangeToFloatNonPcnt((double)((iClosePickerGx-iOpenPickerGx)), (double)(dX120ToX40PitchCount));
    iX40PitchCount [ePickerG] += ChangeToFloatNonPcnt((double)(((   4000/3)-(-iClosePickerGx+iClosePickerEx ))), (double)(dPitchScaleX));
    iX120PitchCount[ePickerG] += ChangeToFloatNonPcnt((double)(((  12000/3)-(- iOpenPickerGx+iOpenPickerEx  ))), (double)(dPitchScaleX));

    dPitchScaleX=ChangeToFloatNonPcnt((double)((iClosePickerHx-iOpenPickerHx)), (double)(dX120ToX40PitchCount));
    iX40PitchCount [ePickerH] += ChangeToFloatNonPcnt((double)(((   4000/3)-(-iClosePickerHx+iClosePickerEx ))), (double)(dPitchScaleX));
    iX120PitchCount[ePickerH] += ChangeToFloatNonPcnt((double)(((  12000/3)-(- iOpenPickerHx+iOpenPickerEx  ))), (double)(dPitchScaleX));

    iCnt=0;
    iX40PitchCountAvg=0;
    iX120PitchCountAvg=0;
    for(i=0; i<ePickerTotal; i++)
    {
        iX40PitchCountAvg  += iX40PitchCount[i];
        iX120PitchCountAvg += iX120PitchCount[i];
        iCnt++;
    }
    iX40PitchCountAvg = ChangeToFloatNonPcnt((double)(iX40PitchCountAvg), (double)(iCnt));
    iX120PitchCountAvg=ChangeToFloatNonPcnt((double)(iX120PitchCountAvg), (double)(iCnt));
}
//---------------------------------------------------------------------------
void TCAlignmentB::InitialShuttleStatus()
{
    bShuttleIsBusy[0]=false;
    bShuttleIsBusy[1]=false;
}
//---------------------------------------------------------------------------
void TCAlignmentB::WriteAlignFlowLog(AnsiString Message)
{
    AnsiString asAlignFlowLogPath="D:\\HT9045_Log\\AutoAlignFlow";
    AnsiString asLog,asFolderPath,asFilePath;

    asLog.sprintf("%02d:%02d:%02d:%03d %s %s",SystemHour,SystemMin,SystemSec,SystemMSec,asType,Message);
    if(fAutoAlignment->mmoCommLog!=NULL)
    {
        if(fAutoAlignment->mmoAlignFlowLog->Lines->Count>100)
            fAutoAlignment->mmoAlignFlowLog->Lines->Clear();
        fAutoAlignment->mmoAlignFlowLog->Lines->Add(Message);
    }


    asFolderPath.sprintf("%s\\%04d_%02d_%02d\\", asAlignFlowLogPath, SystemYear, SystemMonth, SystemDate);
    if(DirectoryExists(asFolderPath)==false)
        ForceDirectories(asFolderPath);

    asFilePath.sprintf("%s\\%04d_%02d_%02d_%02d.txt", asFolderPath, SystemYear, SystemMonth, SystemDate, SystemHour);
    WriteDataToFile(asFilePath.c_str() , asLog.c_str());
}
