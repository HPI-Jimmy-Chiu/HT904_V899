#include "MachineDefine.h"
#pragma hdrstop

#include "LaserSensorInArm.h"
#include "LaserSensor.h"

//---------------------------------------------------------------------------
#include "MyMotor.h"
#include "note.h"
#include "cprod.h"
#include "cmydef.h"
#include "MyKitSuck.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

int iInArmLaserInitTask=1;
int iInArmLaserCheckTask=1;
TQPF_Timer LaserDelay;
TQPF_Timer LaserTimeOut;
double dInArmCurrentValue[2][50][50];
double dDelayTime=0.5;  //等待馬達整定的時間
double dTimeOutTime=3.0;//讀取多久就是逾時
double dRetryGap=3.0;   //大於多少需要重新擷取一次
int iInArmRetryCount;   //重試的次數
int iInArmAlarmRetryCount;   //重試的次數
int iInArmMaxRetry=2;   //最大重試次數

int iPlate=0;
int iX=0;
int iY=0;
vector< TInLaserCheck* > LaserCheckPos;
//---------------------------------------------------------------------------
extern bool MoveInArmZToPlateSafe(int iTask);
//---------------------------------------------------------------------------
bool SortLaserCheckPos(const TInLaserCheck *lhs, const TInLaserCheck *rhs)  //對Vector排序
{
    if(lhs->iPlate==rhs->iPlate && lhs->iY==rhs->iY)    //最後對X排序
        return lhs->iX<rhs->iX;

    if(lhs->iPlate==rhs->iPlate)                        //再對Y排序
        return lhs->iY<rhs->iY;

    return lhs->iPlate<rhs->iPlate;                     //先對HotPlate排序
}
//---------------------------------------------------------------------------
void SetTrayColor(int iPlate, int iX, int iY, eLaserStatus Status, bool Golden=false);

void SetTrayColor(int iPlate, int iX, int iY, eLaserStatus Status, bool Golden)
{
    if(Golden)
    {
        if(iPlate==0)
        {
            fLaserSensor->mtPlate2_Golden->SetCellColorIndex(iX, iY, Status);
        }
        else
        {
            fLaserSensor->mtPlate1_Golden->SetCellColorIndex(iX, iY, Status);
        }
    }
    else
    {
        if(iPlate==0)
        {
            fLaserSensor->mtPlate2->SetCellColorIndex(iX, iY, Status);
        }
        else
        {
            fLaserSensor->mtPlate1->SetCellColorIndex(iX, iY, Status);
        }
    }
}
//---------------------------------------------------------------------------
void SetGoldenTrayColor(int iPlate, int iX, int iY, eLaserStatus Status)
{
    SetTrayColor(iPlate, iX, iY, Status, true);
}
//---------------------------------------------------------------------------
bool InArmLaserInit(bool bReset)        //將讀頭歸零並取得每個位置的數值
{
    static int Zero=0;

    AnsiString Str, Str1;
    double dTemp;
    bool flag=false;
    bool flag1=false;
    int iXpos, iYpos;

    static bool bMoveX=false, bMoveY=false;

    int &Task=iInArmLaserInitTask;

    if(bReset)
    {
        iInArmRetryCount=0;
        Zero=0;
        Task=1;

        if(HotPlateForm.iPlateSelect==1)    //Hotplate 1
            iPlate=1;   //從Plate 2開始
        else
            iPlate=0;   //從Plate 2開始

        iX=0;
        iY=0;
        bMoveX=false;
        bMoveY=false;

        for(int i=0; i<fLaserSensor->mtPlate2_Golden->XItem; i++)   //把需要偵測的部分先改為水藍色
        {
            for(int j=0; j<fLaserSensor->mtPlate2_Golden->YItem; j++)
            {
                SetGoldenTrayColor(0, i, j, lsNeedCheck);
                SetGoldenTrayColor(1, i, j, lsNeedCheck);
            }
        }

        return flag;
    }

    switch(Task)
    {
        case 1:         //先將數值歸零
            if(MoveInArmZToPlateSafe(Task))
            {
                bMoveX=false;
                bMoveY=false;
                Task=2;
            }
            break;
        case 2:
            if(iPlate==0)
            {
                iXpos=Prod.iHP2LaserX+HotPlateForm.XPitch*iX;
                iYpos=Prod.iHP2LaserY-HotPlateForm.YPitch*iY;
            }
            else
            {
                iXpos=Prod.iHP1LaserX+HotPlateForm.XPitch*iX;
                iYpos=Prod.iHP1LaserY-HotPlateForm.YPitch*iY;
            }
            if(bMoveX==false)
                bMoveX=MOT[MInArmX].MotorMove(iXpos);
            if(bMoveY==false)
                bMoveY=MOT[MInArmY].MotorMove(iYpos);

            if(bMoveX && bMoveY)
            {
                LaserDelay.SetSecAndOn(dDelayTime);
                Task=100;
            }
            break;
        case 100:
            if(LaserDelay.Off())
            {
                fLaserSensor->iLaserInArmTask=1;
                fLaserSensor->btSetToZeroInArm->Click();
                LaserTimeOut.SetSecAndOn(dTimeOutTime);
                Task=200;
            }
            break;
        case 200:       //確認數值歸零
            if(fLaserSensor->bSetToZeroInArm==false)
            {
                Zero++;     //多做兩次
                if(Zero<2)
                {
                    LaserDelay.SetSecAndOn(0.1);
                    Task=100;
                }
                else
                {
                    iInArmRetryCount=0;
                    iPlate=0;
                    iX=0;
                    iY=0;
                    Task=300;
                }
            }
            else if(LaserTimeOut.Off())
            {
                ShowErrorMessage("WAR0164", K_RETRY, MInArmX);  //Input Arm Laser Read Error, Please Check COM Port Connection!!
                Task=1;
            }
            break;
        case 300:       //偵測第(Step)個位置的數值
            if(MoveInArmZToPlateSafe(Task))
            {
                bMoveX=false;
                bMoveY=false;
                Task=350;
            }
            break;
        case 350:
            SetGoldenTrayColor(iPlate, iX, iY, lsChecking);
            if(iPlate==0)
            {
                iXpos=Prod.iHP2LaserX+HotPlateForm.XPitch*iX;
                iYpos=Prod.iHP2LaserY-HotPlateForm.YPitch*iY;
            }
            else
            {
                iXpos=Prod.iHP1LaserX+HotPlateForm.XPitch*iX;
                iYpos=Prod.iHP1LaserY-HotPlateForm.YPitch*iY;
            }

            if(bMoveX==false)
                bMoveX=MOT[MInArmX].MotorMove(iXpos);
            if(bMoveY==false)
                bMoveY=MOT[MInArmY].MotorMove(iYpos);

            if(bMoveX && bMoveY)
            {
                bMoveX=false;
                bMoveY=false;
                LaserDelay.SetSecAndOn(dDelayTime);
                Task=400;
            }
            break;
        case 400:
            if(LaserDelay.Off())
            {
                fLaserSensor->btGetValueInArm->Click();
                LaserTimeOut.SetSecAndOn(dTimeOutTime);
                Task=500;
            }
            break;
        case 500:       //確認是否全部讀取完畢
            flag1=false;
            if(fLaserSensor->bInArm==false)
            {
                dTemp=atof(fLaserSensor->palLaserValue_InArm->Caption.c_str());

                if(dTemp>=dRetryGap)
                {
                    SetGoldenTrayColor(iPlate, iX, iY, lsFail);

                    iInArmRetryCount++;
                    if(iInArmRetryCount>=iInArmMaxRetry)
                    {
                        if(iPlate==0)
                            ShowErrorMessage("WAR0166", K_RETRY, MInArmX);  //Hot Plate 2 Laser Read Error, Please Check Sensor Position!!
                        else
                            ShowErrorMessage("WAR0165", K_RETRY, MInArmX);  //Hot Plate 1 Laser Read Error, Please Check Sensor Position!!
                        iInArmRetryCount=0;
                    }

                    Task=1;
                    break;
                }

                TestIF_File.dLaserValueIn[iPlate][iX][iY]=dTemp;
                Str.sprintf("%0.2f", dTemp);            //Steven 20140416 : 改為小數點兩位

                SetGoldenTrayColor(iPlate, iX, iY, lsNormal);
                if(iPlate==0)
                {
                    fLaserSensor->mtPlate2_Golden->SetCellNumber(iX, iY, Str.c_str());  //Steven 20140416 : 改為小數點兩位
                }
                else
                {
                    fLaserSensor->mtPlate1_Golden->SetCellNumber(iX, iY, Str.c_str());  //Steven 20140416 : 改為小數點兩位
                }

                flag1=true;
            }
            else if(LaserTimeOut.Off())
            {
                ShowErrorMessage("WAR0164", K_RETRY, MInArmX);  //Input Arm Laser Read Error, Please Check COM Port Connection!!
                Task=300;
                break;
            }

            if(flag1)
            {
                iX++;
                if(iX<HotPlateForm.XDivision)
                {
                    Task=300;
                }
                else
                {
                    iX=0;
                    iY++;
                    if(iY<HotPlateForm.YDivision)
                    {
                        Task=300;
                    }
                    else
                    {
                        iPlate++;
                        if(HotPlateForm.iPlateSelect==2) //Hotplate 2
                        {
                            iPlate++;
                        }

                        iX=0;
                        iY=0;
                        if(iPlate<2)
                        {
                            Task=300;
                        }
                        else
                        {
                            Task=600;
                        }
                    }
                }
            }
            break;
        case 600:
            fLaserSensor->SaveInArmLaserValue();
            flag=true;
    }

    return flag;
}
//---------------------------------------------------------------------------
bool CheckInArmFloating(bool bReset)     //確認每個位置的數值
{
    static bool bHasDuplicateErr=false;
    static bool bMoveX=false, bMoveY=false;
    static AnsiString ErrPart;
    AnsiString Str, Str1;
    int iXpos, iYpos;

    bool flag=false, flag1=false;
    double dTemp;

    int &Task=iInArmLaserCheckTask;

    if(bReset)
    {
        iInArmAlarmRetryCount=0;
        iInArmRetryCount=0;
        bHasDuplicateErr=false;
        Task=1;
        bMoveX=false;
        bMoveY=false;
        return flag;
    }

    if(LaserCheckPos.size()==0)
    {
        flag=true;
        return flag;
    }

    switch(Task)
    {
        case 1:
            if(MoveInArmZToPlateSafe(Task))
            {
                sort(LaserCheckPos.begin(), LaserCheckPos.end(), SortLaserCheckPos);    //先排序

                for(unsigned int i=0; i<LaserCheckPos.size(); i++)   //把需要偵測的部分先改為水藍色
                {
                    SetTrayColor(LaserCheckPos[i]->iPlate, LaserCheckPos[i]->iX, LaserCheckPos[i]->iY, lsNeedCheck);
                }

                bMoveX=false;
                bMoveY=false;
                Task=2;
            }
            break;
        case 2:
            SetTrayColor(LaserCheckPos[0]->iPlate, LaserCheckPos[0]->iX, LaserCheckPos[0]->iY, lsChecking);
            if(LaserCheckPos[0]->iPlate==0)     //檢查中的設為黃色
            {
                iXpos=Prod.iHP2LaserX+HotPlateForm.XPitch*LaserCheckPos[0]->iX;
                iYpos=Prod.iHP2LaserY-HotPlateForm.YPitch*LaserCheckPos[0]->iY;
            }
            else
            {
                iXpos=Prod.iHP1LaserX+HotPlateForm.XPitch*LaserCheckPos[0]->iX;
                iYpos=Prod.iHP1LaserY-HotPlateForm.YPitch*LaserCheckPos[0]->iY;
            }
            if(bMoveX==false)
                bMoveX=MOT[MInArmX].MotorMove(iXpos);
            if(bMoveY==false)
                bMoveY=MOT[MInArmY].MotorMove(iYpos);

            if(bMoveX && bMoveY)
            {
                bMoveX=false;
                bMoveY=false;
                LaserDelay.SetSecAndOn(dDelayTime);
                Task=50;
            }
            break;
        case 50:
            if(LaserDelay.Off())
            {
                fLaserSensor->btGetValueInArm->Click();
                Task=100;

                LaserTimeOut.SetSecAndOn(dTimeOutTime);
            }
            break;
        case 100:
            flag1=false;
            if(fLaserSensor->bInArm==false)
            {
                dTemp=atof(fLaserSensor->palLaserValue_InArm->Caption.c_str())-TestIF_File.dLaserValueIn[LaserCheckPos[0]->iPlate][LaserCheckPos[0]->iX][LaserCheckPos[0]->iY];

                if(abs(dTemp)>=dRetryGap)
                {
                    iInArmRetryCount++;

                    SetTrayColor(LaserCheckPos[0]->iPlate, LaserCheckPos[0]->iX, LaserCheckPos[0]->iY, lsFail);

                    if(iInArmRetryCount>=iInArmMaxRetry)
                    {
                        ShowErrorMessage("JAM0160", K_RETRY, MInArmX);  //Input Shuttle Laser Read Error, Please Check COM Port Connection!!
                        iInArmRetryCount=0;
                    }

                    Task=1;
                    break;
                }

                Str.sprintf("%0.2f", dTemp);
                if(LaserCheckPos[0]->iPlate==0)
                {
                    fLaserSensor->mtPlate2->SetCellNumber(LaserCheckPos[0]->iX, LaserCheckPos[0]->iY, Str.c_str());
                }
                else
                {
                    fLaserSensor->mtPlate1->SetCellNumber(LaserCheckPos[0]->iX, LaserCheckPos[0]->iY, Str.c_str());
                }

                flag1=true;
            }
            else if(LaserTimeOut.Off())
            {
                ShowErrorMessage("WAR0164", K_RETRY, MInArmX);  //Input Shuttle Laser Read Error, Please Check COM Port Connection!!
                Task=1;
                break;
            }

            if(flag1)
            {
                Task=200;
            }
            break;
        case 200:
            dTemp=atof(fLaserSensor->palLaserValue_InArm->Caption.c_str())-TestIF_File.dLaserValueIn[LaserCheckPos[0]->iPlate][LaserCheckPos[0]->iX][LaserCheckPos[0]->iY];

            if(MOT[MMPlate1+LaserCheckPos[0]->iPlate].Tray.Data[LaserCheckPos[0]->iX][LaserCheckPos[0]->iY]==HAS_IC ||
               MOT[MMPlate1+LaserCheckPos[0]->iPlate].Tray.Data[LaserCheckPos[0]->iX][LaserCheckPos[0]->iY]==HAS_HOT_IC)
            {
                if(dTemp<-(TestIF_File.dIcThickness+TestIF_File.dLaserThresholdHP))     //置偏
                {
                    iInArmAlarmRetryCount++;
                    SetTrayColor(LaserCheckPos[0]->iPlate, LaserCheckPos[0]->iX, LaserCheckPos[0]->iY, lsFail); //有問題的設為紅色

                    if(iInArmAlarmRetryCount>=iInArmMaxRetry)
                    {
                        if(LaserCheckPos[0]->iPlate==0)
                        {
                            ShowErrorMessage("JAM0161", K_RETRY, MMPlate2, bHasDuplicateErr);   //Hot Plate 2 device height error by Laser!!
                        }
                        else
                        {
                            ShowErrorMessage("JAM0160", K_RETRY, MMPlate1, bHasDuplicateErr);   //Hot Plate 2 device height error by Laser!!
                        }
                        iInArmAlarmRetryCount=0;
                        bHasDuplicateErr=true;
                    }
                }
                else if(dTemp>-(TestIF_File.dIcThickness-TestIF_File.dLaserThresholdHP))    //IC遺失
                {
                    iInArmAlarmRetryCount++;
                    SetTrayColor(LaserCheckPos[0]->iPlate, LaserCheckPos[0]->iX, LaserCheckPos[0]->iY, lsFail); //有問題的設為紅色

                    if(iInArmAlarmRetryCount>=iInArmMaxRetry)
                    {
                        if(LaserCheckPos[0]->iPlate==0)
                        {
                            fLaserSensor->mtPlate2->SetCellColorIndex(LaserCheckPos[0]->iX, LaserCheckPos[0]->iY, lsFail);
                            ShowErrorMessage("JAM0163", K_RETRY, MMPlate2, bHasDuplicateErr);   //Hot Plate 2 device lose error by Laser!!
                        }
                        else
                        {
                            fLaserSensor->mtPlate1->SetCellColorIndex(LaserCheckPos[0]->iX, LaserCheckPos[0]->iY, lsFail);
                            ShowErrorMessage("JAM0162", K_RETRY, MMPlate1, bHasDuplicateErr);   //Hot Plate 1 device lose error by Laser!!
                        }
                        iInArmAlarmRetryCount=0;
                        bHasDuplicateErr=true;
                    }
                }
            }

            if((LastSet.iRealDummy!=DUMMY && iInArmAlarmRetryCount!=0) ||
               (LastSet.iRealDummy==DUMMY && iInArmAlarmRetryCount!=0 && iInArmAlarmRetryCount<=iInArmMaxRetry))
            {
                Task=1;
                break;
            }

            SetTrayColor(LaserCheckPos[0]->iPlate, LaserCheckPos[0]->iX, LaserCheckPos[0]->iY, lsNormal); //檢查完成設定回白色
            LaserCheckPos.erase(LaserCheckPos.begin());
            if(LaserCheckPos.size()!=0)
            {
                iInArmAlarmRetryCount=0;
                bHasDuplicateErr=false;
                Task=1;
            }
            else
            {
                Task=300;
            }
            break;
        case 300:
            Task=1;
            iInArmAlarmRetryCount=0;
            bHasDuplicateErr=false;
            flag=true;
            break;
    }
    return flag;
}

