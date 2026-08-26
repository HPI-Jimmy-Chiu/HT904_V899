//---------------------------------------------------------------------------
#include <vcl.h>
#include "LastSet.h"
#include "cVisionComm.h"
#ifndef CAlignmentBH
#define CAlignmentBH
//---------------------------------------------------------------------------
//Alignment Bottom CCD Class
class TCAlignmentB
{
    private:
        bool bMoveAllZ;
        int iBottomCCDOK;
        int iTopCCDOK;
        int iBasePickerOK;
        int iPickerPitch;
        int iTeachMarkOK;
        double dLUx;
        double dLUy;
        double dRDx;
        double dRDy;
        int iPicker;
        bool IsCalAlignPitchOK;
        int iMark;
        bool bShutMoveOK1;
        bool bShutMoveOK2;
        int ix;
        int iy;
        int iReturn;
    public:
        __fastcall TCAlignmentB(AnsiString asInputType);
        __fastcall ~ TCAlignmentB();

        AnsiString asType;  //IN ,OUT
        int iMode;

        void SetXPitchTeach(int iBasePickerOnBottomCCDX,int iBasePickerOnBottomCCDY);

        int iX40PitchTeachX[8];
        int iX40PitchTeachY[8];
        int iX120PitchTeachX[8];
        int iX120PitchTeachY[8];

        int iX40PitchTeachAccX[8];
        int iX40PitchTeachAccY[8];
        int iX120PitchTeachAccX[8];
        int iX120PitchTeachAccY[8];

        int iX40PitchTeachOfsX[8];
        int iX40PitchTeachOfsY[8];
        int iX120PitchTeachOfsX[8];
        int iX120PitchTeachOfsY[8];

        int iBasePickerOnBottomAccX;
        int iBasePickerOnBottomAccY;
        int iBasePickerOnBottomOfsX;
        int iBasePickerOnBottomOfsY;

        int iTop2DummyAccX;
        int iTop2DummyAccY;
        int iTop2DummyOfsX;
        int iTop2DummyOfsY;

        int iTop2BasePickerAccX;
        int iTop2BasePickerAccY;
        int iTop2BasePickerOfsX;
        int iTop2BasePickerOfsY;

        int iMarkAccX[20];
        int iMarkAccY[20];
        int iMarkOfsX[20];
        int iMarkOfsY[20];

        void CalAlignPitch(int X40Pitch,int X120Pitch);
        int iX40PitchCount[8];
        int iX120PitchCount[8];
        int iX40PitchCountAvg;
        int iX120PitchCountAvg;

        //Flow
        HTimer tInCCDMoveDelay;
        TQPF_Timer CycleTimeTimer;
        int _MArmX;
        int _MArmY;
        int _MArmPitchY;
        int _MArmPitchX2;
        int _IO;
        bool bShuttleIsBusy[2];  //bShuttleIsBusy[0]=true 表示 Shuttle 被  InArm 控制使用。
                                 //bShuttleIsBusy[1]=true 表示 Shuttle 被 OutArm 控制使用。
        void InitialShuttleStatus();

        int DoAlign(bool bReset=false);         //In Alignment 流程
        int iAlignTask;

        int DoBottomCCDCalibration(bool bReset=false);      //計算 BottomCCD PixelSize
        int iBottomCCDCalibrationTask;

        int DoTopCCDCalibration(bool bReset=false);         //計算 TopCCD PixelSize
        int iTopCCDCalibrationTask;

        int DoBasePickerCalibration(bool bReset=false);       //計算 TopCCD To BasePicker
        int iBasePickerCalibrationTask;

        int DoPickerPitchCalibration(bool bReset=false);      //計算 Pitch X40 & X120
        int iPickerPitchCalibrationTask;

        int DoTeachMarkCalibration(bool bReset=false);      //計算 Teach Mark Offset
        int iTeachMarkCalibrationTask;

        int MoveArmXYToAccuracyPos(int iCCD,int iMatchType,bool bCenter,int iTeachx,int iTeachy,int iXpitch,bool *ZDownSel,int *ZPos,bool ZNeedDown);
        void InitMoveArmXYToAccuracyPosTask();
        int iMoveArmXYToAccuracyPosTask;
        int iArmMoveXYZ;

        bool bEnableVisionSimulate;

        TVisionComm *vcAlign[2];

        void WriteAlignFlowLog(AnsiString Message);
};
#endif
