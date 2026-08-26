#include "MachineDefine.h"
#pragma hdrstop

#include "LaserSensorShuttle.h"
#include "LaserSensor.h"

//---------------------------------------------------------------------------
#include "MyMotor.h"
#include "note.h"
#include "cprod.h"
#include "cmydef.h"
#include "MyKitSuck.h"
#include "ainarm2.h"
#include "cprod.h"
#include "ainarm9045.h"
#include "aoutarm9045.h"
#include "cinitial.h"
#include "mymessbox.h"
#include "mycylin.h"
extern int GetRowCol(int &iRow, int &iCol);                                 //Eastsun 20260525 laser 整合
extern void GetShuttleSize(int &XItem, int &YItem, int &PitchX, int &PitchY);   //Eastsun 20260525 laser 整合

#pragma package(smart_init)

int iShtLaserInitTask=1;
int iShtLaserCheckTask=1;
int iUseInArmShtLaserCheckTask=1;
int iUseOutArmShtLaserCheckTask=1;
TQPF_Timer LaserShtDelay;
TQPF_Timer LaserShtTimeOut;
TQPF_Timer LaserOutShtTimeOut;
double dCurrentValue[2][MAX_Index_Row][MAX_Index_Col];
double dShtDelayTime=0.5;  //等待馬達整定的時間
double dShtTimeOutTime=3.0;//讀取多久就是逾時
double dShtRetryGap=3.0;   //大於多少需要重新擷取一次
int iShuttleRetryCount; //重試的次數
int iShuttleMaxRetry=2; //最大重試次數
extern int  GetRowCol(int &iRow, int &iCol);
//---------------------------------------------------------------------------
void SetShtTrayColor(int iSht, int iX, int iY, eLaserStatus Status, bool Golden=false, int iInOutSht=0);
void SetShtTrayNumber(int iSht, int iX, double dNum1, double dNum2, bool Golden=false, int iInOutSht=0);
void SetShtTrayColor(int iSht, int iX, int iY, eLaserStatus Status, bool Golden, int iInOutSht)
{
    int myX=SThreadPara.iCHKStep-iX-1;

    if(Golden)
    {
        if(iInOutSht==0)
        {
            if(iSht==0)
            {
                fLaserSensor->mtShuttle1_Golden->SetCellColorIndex(myX, iY, Status);
            }
            else
            {
                fLaserSensor->mtShuttle2_Golden->SetCellColorIndex(myX, iY, Status);
            }
        }
        else
        {
            if(iSht==0)
            {
                fLaserSensor->mtOutShuttle1_Golden->SetCellColorIndex(myX, iY, Status);
            }
            else
            {
                fLaserSensor->mtOutShuttle2_Golden->SetCellColorIndex(myX, iY, Status);
            }
        }
    }
    else
    {
        if(iInOutSht==0)
        {
            if(iSht==0)
            {
                fLaserSensor->mtShuttle1->SetCellColorIndex(myX, iY, Status);
            }
            else
            {
                fLaserSensor->mtShuttle2->SetCellColorIndex(myX, iY, Status);
            }
        }
        else
        {
            if(iSht==0)
            {
                fLaserSensor->mtOutShuttle1->SetCellColorIndex(myX, iY, Status);
            }
            else
            {
                fLaserSensor->mtOutShuttle2->SetCellColorIndex(myX, iY, Status);
            }
        }
    }
}
//---------------------------------------------------------------------------
void SetShtGoldenTrayColor(int iSht, int iX, int iY, eLaserStatus Status)
{
    SetShtTrayColor(iSht, iX, iY, Status, true);
}
//---------------------------------------------------------------------------
void SetShtTrayNumber(int iSht, int iX, double dNum1, double dNum2, bool Golden, int iInOutSht)
{
    int myX=SThreadPara.iCHKStep-iX-1;
    if(Golden)
    {
        if(USE_LASER_DISTANCE==2)
        {
            if(iInOutSht==0)
            {
                if(iSht==0)
                {
                    if(dNum1!=0.0 && dNum2==0.0)
                        fLaserSensor->mtShuttle1_Golden->SetCellNumber(myX, 0, dNum1);
                    else if(dNum1==0.0 && dNum2!=0.0)
                        fLaserSensor->mtShuttle1_Golden->SetCellNumber(myX, 1, dNum2);
                }
                else
                {
                    if(dNum1!=0.0 && dNum2==0.0)
                        fLaserSensor->mtShuttle2_Golden->SetCellNumber(myX, 0, dNum1);
                    else if(dNum1==0.0 && dNum2!=0.0)
                        fLaserSensor->mtShuttle2_Golden->SetCellNumber(myX, 1, dNum2);
                }
            }
            else
            {
                if(iSht==0)
                {
                    if(dNum1!=0.0 && dNum2==0.0)
                        fLaserSensor->mtOutShuttle1_Golden->SetCellNumber(myX, 0, dNum1);
                    else if(dNum1==0.0 && dNum2!=0.0)
                        fLaserSensor->mtOutShuttle1_Golden->SetCellNumber(myX, 1, dNum2);
                }
                else
                {
                    if(dNum1!=0.0 && dNum2==0.0)
                        fLaserSensor->mtOutShuttle2_Golden->SetCellNumber(myX, 0, dNum1);
                    else if(dNum1==0.0 && dNum2!=0.0)
                        fLaserSensor->mtOutShuttle2_Golden->SetCellNumber(myX, 1, dNum2);
                }
            }
        }
        else
        {
        if(iSht==0)
        {
            fLaserSensor->mtShuttle1_Golden->SetCellNumber(myX, 0, dNum1);
            fLaserSensor->mtShuttle1_Golden->SetCellNumber(myX, 1, dNum2);
        }
        else
        {
            if(SThreadPara.iScanSensor==2)
            {
                fLaserSensor->mtShuttle2_Golden->SetCellNumber(myX, 0, dNum1);
                fLaserSensor->mtShuttle2_Golden->SetCellNumber(myX, 1, dNum2);
            }
            else
            {
                fLaserSensor->mtShuttle2_Golden->SetCellNumber(myX, 1, dNum1);
                fLaserSensor->mtShuttle2_Golden->SetCellNumber(myX, 0, dNum2);
            }
        }
    }
    }
    else
    {
        if(USE_LASER_DISTANCE==2)
        {
            if(iInOutSht==0)
            {
                if(iSht==0)
                {
                    if(dNum1!=0.0 && dNum2==0.0)
                        fLaserSensor->mtShuttle1->SetCellNumber(myX, 0, dNum1);
                    else if(dNum1==0.0 && dNum2!=0.0)
                        fLaserSensor->mtShuttle1->SetCellNumber(myX, 1, dNum2);
                }
                else
                {
                    if(dNum1!=0.0 && dNum2==0.0)
                        fLaserSensor->mtShuttle2->SetCellNumber(myX, 0, dNum1);
                    else if(dNum1==0.0 && dNum2!=0.0)
                        fLaserSensor->mtShuttle2->SetCellNumber(myX, 1, dNum2);
                }
            }
            else
            {
                if(iSht==0)
                {
                    if(dNum1!=0.0 && dNum2==0.0)
                        fLaserSensor->mtOutShuttle1->SetCellNumber(myX, 0, dNum1);
                    else if(dNum1==0.0 && dNum2!=0.0)
                        fLaserSensor->mtOutShuttle1->SetCellNumber(myX, 1, dNum2);
                }
                else
                {
                    if(dNum1!=0.0 && dNum2==0.0)
                        fLaserSensor->mtOutShuttle2->SetCellNumber(myX, 0, dNum1);
                    else if(dNum1==0.0 && dNum2!=0.0)
                        fLaserSensor->mtOutShuttle2->SetCellNumber(myX, 1, dNum2);
                }
            }
        }
        else
        {
            if(iSht==0)
            {
                fLaserSensor->mtShuttle1->SetCellNumber(myX, 0, dNum1);
                fLaserSensor->mtShuttle1->SetCellNumber(myX, 1, dNum2);
            }
            else
            {
                if(SThreadPara.iScanSensor==2)
                {
                    fLaserSensor->mtShuttle2->SetCellNumber(myX, 0, dNum1);
                    fLaserSensor->mtShuttle2->SetCellNumber(myX, 1, dNum2);
                }
                else
                {
                    fLaserSensor->mtShuttle2->SetCellNumber(myX, 1, dNum1);
                    fLaserSensor->mtShuttle2->SetCellNumber(myX, 0, dNum2);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void SetShtGoldenTrayNumber(int iSht, int iX, double dNum1, double dNum2)
{
    SetShtTrayNumber(iSht, iX, dNum1, dNum2, true);
}
//---------------------------------------------------------------------------
bool ShtLaserInit(int iSht, bool bReset)        //將讀頭歸零並取得每個位置的數值
{
    static int Step=0;
    static int Zero=0;

    AnsiString Str, Str1;
    bool flag=false;
    bool flag1=false;
    int &Task=iShtLaserInitTask;

    if(bReset)
    {
        for(int i=0; i<fLaserSensor->mtShuttle1_Golden->XItem; i++)
        {
            for(int j=0; j<fLaserSensor->mtShuttle1_Golden->YItem; j++)
            {
                SetShtGoldenTrayColor(iSht, i, j, lsNeedCheck);
            }
        }

        iShuttleRetryCount=0;
        Zero=0;
        Task=1;
        Step=0;
        return flag;
    }

    switch(Task)
    {
        case 1:         //先將數值歸零
            flag1=MOT[MInShuttle1+iSht].MotorMove(SThreadPara.iLaserCheckPos[iSht][0]);

            if(flag1)
            {
                LaserShtDelay.SetSecAndOn(dShtDelayTime);
                Task=100;
            }
            break;
        case 100:
            if(LaserShtDelay.Off())
            {
                fLaserSensor->iLaser1Task=1;
                fLaserSensor->iLaser2Task=1;
                if(iSht==0)
                {
                    fLaserSensor->btSetToZeroSh1->Click();
                }
                else
                {
                    fLaserSensor->btSetToZeroSh2->Click();
                }

                LaserShtTimeOut.SetSecAndOn(dShtTimeOutTime);
                Task=200;
            }
            break;
        case 200:       //確認數值歸零
            if((iSht==0 && fLaserSensor->bSetToZeroSh1==false) ||
               (iSht==1 && fLaserSensor->bSetToZeroSh2==false))
            {
                Zero++;     //多做兩次
                if(Zero<2)
                {
                    LaserShtDelay.SetSecAndOn(0.1);
                    Task=100;
                }
                else
                {
                    iShuttleRetryCount=0;
                    Task=300;
                }
            }
            else if(LaserShtTimeOut.Off())
            {
                ShowErrorMessage("WAR0413", K_RETRY, MInShuttle1);  //Input Shuttle Laser Read Error, Please Check COM Port Connection!!
                Task=1;
            }
            break;
        case 300:       //偵測第(Step)個位置的數值
            SetShtGoldenTrayColor(iSht, Step, 0, lsChecking);
            SetShtGoldenTrayColor(iSht, Step, 1, lsChecking);

            flag1=MOT[MInShuttle1+iSht].MotorMove(SThreadPara.iLaserCheckPos[iSht][Step]);
            if(flag1)
            {
                LaserShtDelay.SetSecAndOn(dShtDelayTime);
                Task=400;
            }
            break;
        case 400:
            if(LaserShtDelay.Off())
            {
                if(iSht==0)
                {
                    if(SThreadPara.iScanSensor==1)
                        fLaserSensor->btGetValueSht1_1Row->Click();
                    else
                        fLaserSensor->btGetValueSht1_2Row->Click();
                }
                else //if(iSht==1)
                {
                    if(SThreadPara.iScanSensor==1)
                        fLaserSensor->btGetValueSht2_1Row->Click();
                    else
                        fLaserSensor->btGetValueSht2_2Row->Click();
                }
                LaserShtTimeOut.SetSecAndOn(dShtTimeOutTime);
                Task=500;
            }
            break;
        case 500:       //確認是否全部讀取完畢
            flag1=false;
            if(iSht==0 && fLaserSensor->bSh1_2Row==false && fLaserSensor->bSh1_1Row==false)
            {
                TestIF_File.dLaserValue[iSht][1][Step]=atof(fLaserSensor->palLaserValue_InShuttle1_2->Caption.c_str());
                if(SThreadPara.iScanSensor==2)
                {
                    TestIF_File.dLaserValue[iSht][0][Step]=atof(fLaserSensor->palLaserValue_InShuttle1_1->Caption.c_str());
                }

                SetShtGoldenTrayNumber(iSht, Step, TestIF_File.dLaserValue[iSht][1][Step], TestIF_File.dLaserValue[iSht][0][Step]);

                if(abs(TestIF_File.dLaserValue[iSht][1][Step])>=dShtRetryGap ||
                  (abs(TestIF_File.dLaserValue[iSht][0][Step])>=dShtRetryGap && SThreadPara.iScanSensor==2))
                {
                    iShuttleRetryCount++;
                    SetShtGoldenTrayColor(iSht, Step, 0, lsFail);
                    SetShtGoldenTrayColor(iSht, Step, 1, lsFail);

                    if(iShuttleRetryCount>=iShuttleMaxRetry)
                    {
                        ShowErrorMessage("WAR0414", K_RETRY, MInShuttle1);  //Input Shuttle Laser Read Error, Please Check COM Port Connection!!
                        iShuttleRetryCount=0;
                    }

                    Task=1;
                    break;
                }

                flag1=true;
            }
            else if(iSht==1 && fLaserSensor->bSh2_2Row==false && fLaserSensor->bSh2_1Row==false)
            {
                TestIF_File.dLaserValue[iSht][0][Step]=atof(fLaserSensor->palLaserValue_InShuttle2_1->Caption.c_str());
                if(SThreadPara.iScanSensor==2)
                {
                    TestIF_File.dLaserValue[iSht][1][Step]=atof(fLaserSensor->palLaserValue_InShuttle2_1->Caption.c_str());
                }

                SetShtGoldenTrayNumber(iSht, Step, TestIF_File.dLaserValue[iSht][1][Step], TestIF_File.dLaserValue[iSht][0][Step]);

                if(abs(TestIF_File.dLaserValue[iSht][0][Step])>=dShtRetryGap ||
                  (abs(TestIF_File.dLaserValue[iSht][1][Step])>=dShtRetryGap && SThreadPara.iScanSensor==2))
                {
                    iShuttleRetryCount++;
                    SetShtGoldenTrayColor(iSht, Step, 0, lsFail);
                    SetShtGoldenTrayColor(iSht, Step, 1, lsFail);

                    if(iShuttleRetryCount>=iShuttleMaxRetry)
                    {
                        ShowErrorMessage("WAR0415", K_RETRY, MInShuttle2);  //Input Shuttle Laser Read Error, Please Check COM Port Connection!!
                        iShuttleRetryCount=0;
                    }

                    Task=1;
                    break;
                }

                flag1=true;
            }
            else if(LaserShtTimeOut.Off())
            {
                ShowErrorMessage("WAR0413", K_RETRY, MInShuttle1);  //Input Shuttle Laser Read Error, Please Check COM Port Connection!!
                Task=300;
                break;
            }

            if(flag1)
            {
                SetShtGoldenTrayColor(iSht, Step, 0, lsNormal);
                SetShtGoldenTrayColor(iSht, Step, 1, lsNormal);

                Step++;
                iShuttleRetryCount=0;
                if(Step<SThreadPara.iCHKStep)
                {
                    Task=300;
                }
                else
                {
                    Task=600;
                }
            }
            break;
        case 600:
            Str="InitValue, ";
            for(int i=0; i<SThreadPara.iCHKStep; i++)
            {
                Str1.sprintf("%0.3f", TestIF_File.dLaserValue[iSht][0][i]);
                Str+=Str1+", ";
                Str1.sprintf("%0.3f", TestIF_File.dLaserValue[iSht][1][i]);
                Str+=Str1+", ";
            }

            if(iSht==0)
            {
                fLaserSensor->WriteToMemo(fLaserSensor->MemoSh1, Str);
            }
            else
            {
                fLaserSensor->WriteToMemo(fLaserSensor->MemoSh2, Str);
            }

            fLaserSensor->SaveShuttleLaserValue(iSht);
            flag=true;
    }

    return flag;
}
//---------------------------------------------------------------------------
bool CheckShtFloating(int iSht, bool bReset)     //確認每個位置的數值
{
    static int Step=0;
    static bool bHasDuplicateErr=false;
    static AnsiString ErrPart;
    AnsiString Str, Str1;

    bool flag=false;
    bool bHasErr=false;
    bool flag1=false;
    double dTemp;

    int &Task=iShtLaserCheckTask;

    if(bReset)
    {
        for(int i=0; i<fLaserSensor->mtShuttle1->XItem; i++)
        {
            for(int j=0; j<fLaserSensor->mtShuttle1->YItem; j++)
            {
                SetShtTrayColor(iSht, i, j, lsNeedCheck);
            }
        }
        iShuttleRetryCount=0;
        bHasDuplicateErr=false;
        Task=1;
        Step=0;
        return flag;
    }

    switch(Task)
    {
        case 1:
            flag1=MOT[MInShuttle1+iSht].MotorMove(SThreadPara.iLaserCheckPos[iSht][Step]);
            SetShtTrayColor(iSht, Step, 0, lsChecking);
            SetShtTrayColor(iSht, Step, 1, lsChecking);
            if(flag1)
            {
                LaserShtDelay.SetSecAndOn(dShtDelayTime);
                Task=50;
            }
            break;
        case 50:
            if(LaserShtDelay.Off())
            {
                if(iSht==0)
                {
                    if(SThreadPara.iScanSensor==1)
                        fLaserSensor->btGetValueSht1_1Row->Click();
                    else
                        fLaserSensor->btGetValueSht1_2Row->Click();
                }
                else //if(iSht==1)
                {
                    if(SThreadPara.iScanSensor==1)
                        fLaserSensor->btGetValueSht2_1Row->Click();
                    else
                        fLaserSensor->btGetValueSht2_2Row->Click();
                }
                Task=100;

                LaserShtTimeOut.SetSecAndOn(dShtTimeOutTime);
            }
            break;
        case 100:
            flag1=false;
            if(iSht==0 && fLaserSensor->bSh1_2Row==false && fLaserSensor->bSh1_1Row==false)
            {
                dCurrentValue[iSht][1][Step]=atof(fLaserSensor->palLaserValue_InShuttle1_2->Caption.c_str());
                if(SThreadPara.iScanSensor==2)
                    dCurrentValue[iSht][0][Step]=atof(fLaserSensor->palLaserValue_InShuttle1_1->Caption.c_str());
                else
                    dCurrentValue[iSht][0][Step]=0.0;

                SetShtTrayNumber(iSht, Step, dCurrentValue[iSht][1][Step], dCurrentValue[iSht][0][Step]);

                if(abs(dCurrentValue[iSht][1][Step])>=dShtRetryGap ||
                  (abs(dCurrentValue[iSht][0][Step])>=dShtRetryGap && SThreadPara.iScanSensor==2))
                {
                    SetShtTrayColor(iSht, Step, 0, lsFail);
                    SetShtTrayColor(iSht, Step, 1, lsFail);

                    iShuttleRetryCount++;
                    if(iShuttleRetryCount>=iShuttleMaxRetry)
                    {
                        ShowErrorMessage("WAR0414", K_RETRY, MInShuttle1);  //Input Shuttle Laser Read Error, Please Check COM Port Connection!!
                        iShuttleRetryCount=0;
                    }

                    Task=1;
                    break;
                }

                flag1=true;
            }
            else if(iSht==1 && fLaserSensor->bSh2_2Row==false && fLaserSensor->bSh2_1Row==false)
            {
                dCurrentValue[iSht][0][Step]=atof(fLaserSensor->palLaserValue_InShuttle2_1->Caption.c_str());
                if(SThreadPara.iScanSensor==2)
                {
                    dCurrentValue[iSht][1][Step]=atof(fLaserSensor->palLaserValue_InShuttle2_2->Caption.c_str());
                }
                else
                {
                    dCurrentValue[iSht][1][Step]=0.0;
                }

                SetShtTrayNumber(iSht, Step, dCurrentValue[iSht][1][Step], dCurrentValue[iSht][0][Step]);

                if(abs(dCurrentValue[iSht][0][Step])>=dShtRetryGap ||
                  (abs(dCurrentValue[iSht][1][Step])>=dShtRetryGap && SThreadPara.iScanSensor==2))
                {
                    SetShtTrayColor(iSht, Step, 0, lsFail);
                    SetShtTrayColor(iSht, Step, 1, lsFail);

                    iShuttleRetryCount++;
                    if(iShuttleRetryCount>=iShuttleMaxRetry)
                    {
                        ShowErrorMessage("WAR0415", K_RETRY, MInShuttle2);  //Input Shuttle Laser Read Error, Please Check COM Port Connection!!
                        iShuttleRetryCount=0;
                    }

                    Task=1;
                    break;
                }

                flag1=true;
            }
            else if(LaserShtTimeOut.Off())
            {
                ShowErrorMessage("WAR0413", K_RETRY, MInShuttle1);  //Input Shuttle Laser Read Error, Please Check COM Port Connection!!
                Task=1;
                break;
            }

            if(flag1)
            {
                SetShtTrayColor(iSht, Step, 0, lsNormal);
                SetShtTrayColor(iSht, Step, 1, lsNormal);

                Step++;
                iShuttleRetryCount=0;
                if(Step<SThreadPara.iCHKStep)
                {
                    Task=1;
                }
                else
                {
                    Task=200;
                }
            }
            break;
        case 200:
            ErrPart="";
            if(LastSet.iRealDummy!=DUMMY)
            {
                for(int i=0; i<SThreadPara.iCHKStep; i++)
                {
                    if(iSht==0)
                    {
                        dTemp=dCurrentValue[iSht][1][i]-TestIF_File.dLaserValue[iSht][1][i];
                        if(dTemp<-(TestIF_File.dIcThickness+TestIF_File.dLaserThresholdSht))    //疊料或置偏
                        {
                            if((FLCarryKit.Item[0][SThreadPara.iCHKStep-i-1]!=NULL_IC && FLCarryKit.Item[0][SThreadPara.iCHKStep-i-1]!=HAS_NULL_IC))
                            {
                                SetShtTrayColor(iSht, i, 1, lsFail);
                                bHasErr=true;

                                ErrPart+=IndexSuckName[0][SThreadPara.iCHKStep-i-1];
                            }
                        }

                        if(SThreadPara.iScanSensor==2)
                        {
                            dTemp=dCurrentValue[iSht][0][i]-TestIF_File.dLaserValue[iSht][0][i];
                            if(dTemp<-(TestIF_File.dIcThickness+TestIF_File.dLaserThresholdSht))    //疊料或置偏
                            {
                                if((FLCarryKit.Item[1][SThreadPara.iCHKStep-i-1]!=NULL_IC && FLCarryKit.Item[1][SThreadPara.iCHKStep-i-1]!=HAS_NULL_IC))
                                {
                                    SetShtTrayColor(iSht, i, 0, lsFail);
                                    bHasErr=true;
                                    ErrPart+=IndexSuckName[1][SThreadPara.iCHKStep-i-1];
                                }
                            }
                        }
                    }
                    else
                    {
                        if(SThreadPara.iScanSensor==2)
                        {
                            dTemp=dCurrentValue[iSht][0][i]-TestIF_File.dLaserValue[iSht][0][i];
                            if(dTemp<-(TestIF_File.dIcThickness+TestIF_File.dLaserThresholdSht))    //疊料
                            {
                                if((BLCarryKit.Item[1][SThreadPara.iCHKStep-i-1]!=NULL_IC && BLCarryKit.Item[1][SThreadPara.iCHKStep-i-1]!=HAS_NULL_IC))
                                {
                                    SetShtTrayColor(iSht, i, 1, lsFail);
                                    ErrPart+=IndexSuckName[1][SThreadPara.iCHKStep-i-1];
                                    bHasErr=true;
                                }
                            }

                            dTemp=dCurrentValue[iSht][1][i]-TestIF_File.dLaserValue[iSht][1][i];
                            if(dTemp<-(TestIF_File.dIcThickness+TestIF_File.dLaserThresholdSht))    //疊料
                            {
                                if((BLCarryKit.Item[0][SThreadPara.iCHKStep-i-1]!=NULL_IC && BLCarryKit.Item[0][SThreadPara.iCHKStep-i-1]!=HAS_NULL_IC))
                                {
                                    SetShtTrayColor(iSht, i, 0, lsFail);
                                    bHasErr=true;
                                    ErrPart+=IndexSuckName[0][SThreadPara.iCHKStep-i-1];
                                }
                            }
                        }
                        else
                        {
                            dTemp=dCurrentValue[iSht][0][i]-TestIF_File.dLaserValue[iSht][0][i];
                            if(dTemp<-(TestIF_File.dIcThickness+TestIF_File.dLaserThresholdSht))    //疊料
                            {
                                if((BLCarryKit.Item[0][SThreadPara.iCHKStep-i-1]!=NULL_IC && BLCarryKit.Item[0][SThreadPara.iCHKStep-i-1]!=HAS_NULL_IC))
                                {
                                    SetShtTrayColor(iSht, i, 0, lsFail);
                                    ErrPart+=IndexSuckName[0][SThreadPara.iCHKStep-i-1];
                                    bHasErr=true;
                                }
                            }
                        }
                    }
                }
            }

            Str="RunValue, ";
            for(int i=0; i<SThreadPara.iCHKStep; i++)
            {
                Str1.sprintf("%0.3f", dCurrentValue[iSht][0][i]);
                Str+=Str1+", ";
                Str1.sprintf("%0.3f", dCurrentValue[iSht][1][i]);
                Str+=Str1+", ";
            }

            if(iSht==0)
            {
                fLaserSensor->MemoSh1->Lines->Add(Str);
            }
            else
            {
                fLaserSensor->MemoSh2->Lines->Add(Str);
            }

            if(bHasErr)
            {
                Task=300;
            }
            else
            {
                for(int i=0; i<fLaserSensor->mtShuttle1->XItem; i++)
                {
                    for(int j=0; j<fLaserSensor->mtShuttle1->YItem; j++)
                    {
                        SetShtTrayColor(iSht, i, j, lsNormal);
                    }
                }
                bHasDuplicateErr=false;
                flag=true;
            }
            break;
        case 300:
            flag1=MOT[MInShuttle1+iSht].MotorMove(Prod.InSHT[iSht].iLeft);
            if(flag1)
            {
                Task=400;
            }
            break;
        case 400:
            Task=1;
            Step=0;

            if(iSht==0)
                ShowErrorMessage("JAM0411", K_RETRY, MInShuttle1, bHasDuplicateErr, ErrPart);   //Input Shuttle 1 device floating error.
            else
                ShowErrorMessage("JAM0412", K_RETRY, MInShuttle2, bHasDuplicateErr, ErrPart);   //Input Shuttle 2 device floating error.

            bHasDuplicateErr=true;
            break;
    }
    return flag;
}
//---------------------------------------------------------------------------
//==> Eastsun 20260525 laser 整合 : 從 a-side atester.cpp 移植 GetRowCol (c-side 無此全域函式)
int GetRowCol(int &iRow, int &iCol)
{
//                    ========================================================
//                    第5碼        Site 分佈樣式
//                    ========================================================
//                    asSiteMap    0         1 x 1
//                                 1         1 x 2
//                                 2         1 x 4
//                                 3         2 x 2
//                                 4         2 x 4
//                                 5         2 x 6
//                                 6         2 x 8
//                    ========================================================
    int iSiteMapRTC=-1;

    if(TestIF_File.iTestMode==DualSite) //1x2
    {
        iRow=1;
        iCol=2;
        iSiteMapRTC=1;
    }
    else if(TestIF_File.iTestMode==SingleSite) //1x1
    {
        iRow=1;
        iCol=1;
        iSiteMapRTC=0;
    }
    else if(TestIF_File.iTestMode==TriSite1X3)    //Frank 20160329 add for 1x3_4
    {
        iRow=1;
        iCol=3;
        iSiteMapRTC=1;
    }
    else if(TestIF_File.iTestMode==QualSite1X4 || TestIF_File.iTestMode==_8Site1X4)  //ChungHung 20150528 add for 海思 _8Site1x4 //1x4
    {
        iRow=1;
        iCol=4;
        iSiteMapRTC=2;
    }
    else if(TestIF_File.iTestMode==QualSite2X2 || TestIF_File.iTestMode==QualSite2X2N) //2x2    //Frank 20200520 2X2NN Mod
    {
        iRow=2;
        iCol=2;
        iSiteMapRTC=3;
    }
    else if(TestIF_File.iTestMode==DualSite2x1) //2x1
    {
        iRow=2;
        iCol=1;
    }
    else if(TestIF_File.iTestMode==_6Site2X3 || //ChungHung 20140115 add for 2x3_6
            TestIF_File.iTestMode==_6Site2X3N)  //Steven 20220425 : 2X3NN Mode
    {
        iRow=2;
        iCol=3;
        iSiteMapRTC=7;
    }
    else if(TestIF_File.iTestMode==_8Site2X4) //2x4
    {
        iRow=2;
        iCol=4;
        iSiteMapRTC=4;
    }
    else if(TestIF_File.iTestMode==_16Site4X4) //Sam 20190226 : 16Site4X4
    {
        iRow=4;
        iCol=4;
    }
    else if(TestIF_File.iTestMode==_10Site2X5) //2x5  //wei 20190614 10 site
    {
        iRow=2;
        iCol=5;
        iSiteMapRTC=5;
    }
    else if(TestIF_File.iTestMode==_12Site2X6) //2x6
    {
        iRow=2;
        iCol=6;         //Steven 20130702 : _12Site2X6
        iSiteMapRTC=5;
    }
    else if(TestIF_File.iTestMode==_16Site2X8) //2x8  //Eliot 2009_12_25
    {
        iRow=2;
        iCol=8;
        iSiteMapRTC=6;
    }
    else if(TestIF_File.iTestMode==_32Site4X8M || TestIF_File.iTestMode==_32Site4X8N) //4x8   //ChungHung 20130627 alter TestIF--->TestIF_File 修正無法跑32Site
    {
        iRow=4;
        iCol=8;
    }
    return iSiteMapRTC;
}
//<== Eastsun 20260525 laser 整合
//-----
//==> Eastsun 20260525 laser 整合 : KYEC LaserSensorShuttle 用 InArm/OutArm 移動檢查
//---------------------------------------------------------------------------
bool MoveInArmXYToSHLaserPos(int iShuttle, int iCol, int iRow)
{
    int iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch;
    int  iXpos=0, iYpos=0, iXPitch=0, iVari=0, iYVari=0;

    iXPitch=6000;  //使用iXpitchMaxX3會在 HT-9132LS遇到問題
    iVari=GetInArmPitch_9045(iXPitch);

    if(iShuttle==0)
    {
        iXpos=Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
        iYpos=Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
    }
    else
    {
        iXpos=Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
        iYpos=Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
    }

    iXpos+=iInArmShtXCenterPos;
    iYpos+=iInArmShtYCenterPos;

    iXpos-=860;      //與基準軸的相對距離
    iYpos-=5500;     //與基準軸的相對距離
    iYpos+=1750;
    GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);

    iXpos+=(((iShuttleXItem-1)/2.0)*iShuttleXPitch)-iCol*iShuttleXPitch;

    if(iShuttleYItem==1)
        iYpos+=0;
    else if(iShuttleYItem==2)
        iYpos+=(iShuttleYPitch/2.0)-(iRow*iShuttleYPitch);

    iXpos+=TestIF_File.iDeviceXOffset;
    iYpos+=TestIF_File.iDeviceYOffset;

    //Eastsun 20260525 laser 整合 : 新架構 8 參數陣列簽名 (Vari[X_PITCH_COUNT], ZDownSel[Row][Col], iZPos[Row][Col])
    int  VariArr[X_PITCH_COUNT];
    bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col];
    int  iZPosArr[MAX_ARM_Row][MAX_ARM_Col];
    for(int k=0; k<X_PITCH_COUNT; k++) VariArr[k]=iVari;
    memset(ZDownSel, 0, sizeof(ZDownSel));
    memset(iZPosArr, 0, sizeof(iZPosArr));
    if(InArmContinuousMove_9045(iXpos, iYpos, VariArr, iYVari, ZDownSel, iZPosArr, ZAxisDown))
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool MoveOutArmXYToSHLaserPos(int iShuttle, int iCol, int iRow)
{
    int iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch;
    int  iXpos=0, iYpos=0, iXPitch=0, iVari=0, iYVari=0;

    iXPitch=6000;
    iVari=GetOutArmPitch_9045(iXPitch);

    if(iShuttle==0)
    {
        iXpos=Prod.XOutArm_Shuttle1_Pick[iInArmYBase][iInArmXBase];
        iYpos=Prod.YOutArm_Shuttle1_Pick[iInArmYBase][iInArmXBase];
    }
    else
    {
        iXpos=Prod.XOutArm_Shuttle2_Pick[iInArmYBase][iInArmXBase];
        iYpos=Prod.YOutArm_Shuttle2_Pick[iInArmYBase][iInArmXBase];
    }

    iXpos+=iInArmShtXCenterPos;
    iYpos+=iInArmShtYCenterPos;

    iXpos+=5850;
    iYpos-=5500;
    iYpos+=1750;
    GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);

    iXpos+=(((iShuttleXItem-1)/2.0)*iShuttleXPitch)-iCol*iShuttleXPitch;

    if(iShuttleYItem==1)
        iYpos+=0;
    else if(iShuttleYItem==2)
        iYpos+=(iShuttleYPitch/2.0)-(iRow*iShuttleYPitch);

    iXpos+=TestIF_File.iOutDeviceXOffset;
    iYpos+=TestIF_File.iOutDeviceYOffset;

    //Eastsun 20260525 laser 整合 : 新架構 8 參數陣列簽名
    int  VariArr[X_PITCH_COUNT];
    bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col];
    int  iZPosArr[MAX_ARM_Row][MAX_ARM_Col];
    for(int k=0; k<X_PITCH_COUNT; k++) VariArr[k]=iVari;
    memset(ZDownSel, 0, sizeof(ZDownSel));
    memset(iZPosArr, 0, sizeof(iZPosArr));
    if(OutArmContinuousMove_9045(iXpos, iYpos, VariArr, iYVari, ZDownSel, iZPosArr, ZAxisDown))
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
//==> Eastsun 20260525 laser 整合 UseInArmCheckShtFloating 完整照搬 a-side L789-L1085
bool UseInArmCheckShtFloating(int iSht, bool bReset, bool bSetGold)             //確認每個位置的數值
{
    static int Step=0;
    static int iRow=0, iCol=0;
    static int iNowRow=0, iNowCol=0;
    static bool bHasDuplicateErr=false;
    static AnsiString ErrPart;
    AnsiString Str, Str1;

    bool flag=false;
//    bool bHasErr=false;
    bool flag1=false;
    double dTemp;
    double dValue[2];
    int ret;
    int myX;

    int &Task=iUseInArmShtLaserCheckTask;

    if(bReset)
    {
        GetRowCol(iRow, iCol);

        for(int i=0; i<fLaserSensor->mtShuttle1->XItem; i++)
        {
            for(int j=0; j<fLaserSensor->mtShuttle1->YItem; j++)
            {
                if(bSetGold)
                    SetShtTrayColor(iSht, i, j, lsNeedCheck, true);
                else
                    SetShtTrayColor(iSht, i, j, lsNeedCheck);
            }
        }
        iShuttleRetryCount=0;
        bHasDuplicateErr=false;
        iNowRow=0;
        iNowCol=0;
        Task=1;
        Step=0;
        return flag;
    }

    switch(Task)
    {
        case 1:
            if(bSetGold)
                ShowMyMessage("請放置IC至Shuttle上");

            Task=30;
            break;
        case 30:
            flag1=MoveInArmXYToSHLaserPos(iSht, iNowCol, iNowRow);
            if(bSetGold)
                SetShtTrayColor(iSht, iNowCol, iNowRow, lsChecking, true);
            else
                SetShtTrayColor(iSht, iNowCol, iNowRow, lsChecking);
            if(flag1)
            {
                LaserShtDelay.SetSecAndOn(dShtDelayTime);
                Task=50;
            }
            break;
        case 50:
            if(LaserShtDelay.Off())
            {
                //測距
                Task=100;
                asLaserValue[0]="";
                fLaserSensor->btGetShValue->Click();
                LaserShtTimeOut.SetSecAndOn(1);
            }
            break;
        case 100:
            if(LaserShtTimeOut.Off())
                flag1=true;

            if(asLaserValue[0]!="")
            {
                Str=asLaserValue[0].SubString(11, asLaserValue[0].Length()-14);
                dTemp=atof(Str.c_str());
                myX=SThreadPara.iCHKStep-iNowCol-1;
                if(bSetGold)
                {
                    if(iSht==0)
                    {
                        if(iNowRow==0)
                        {
                            TestIF_File.dLaserValue[iSht][0][myX]=dTemp;
                            SetShtTrayNumber(iSht, iNowCol, dTemp, 0.0, true);
                        }
                        else
                        {
                            TestIF_File.dLaserValue[iSht][1][myX]=dTemp;
                            SetShtTrayNumber(iSht, iNowCol, 0.0, dTemp, true);
                        }
                    }
                    else if(iSht==1)
                    {
                        if(iNowRow==0)
                        {
                            TestIF_File.dLaserValue[iSht][0][myX]=dTemp;
                            SetShtTrayNumber(iSht, iNowCol, dTemp, 0.0, true);
                        }
                        else
                        {
                            TestIF_File.dLaserValue[iSht][1][myX]=dTemp;
                            SetShtTrayNumber(iSht, iNowCol, 0.0, dTemp, true);
                        }
                    }
                }
                else
                {
                    if(iSht==0)
                    {
                        if(iNowRow==0)
                            SetShtTrayNumber(iSht, iNowCol, dTemp, 0.0);
                        else
                            SetShtTrayNumber(iSht, iNowCol, 0.0, dTemp);
                    }
                    else if(iSht==1)
                    {
                        if(iNowRow==0)
                            SetShtTrayNumber(iSht, iNowCol, dTemp, 0.0);
                        else
                            SetShtTrayNumber(iSht, iNowCol, 0.0, dTemp);
                    }
                }
            }

            if(flag1)
            {
                if(bSetGold)
                    SetShtTrayColor(iSht, iNowCol, iNowRow, lsNormal, true);
                else
                    SetShtTrayColor(iSht, iNowCol, iNowRow, lsNormal);

                iNowCol++;
                if(iNowCol<iCol)
                {
                }
                else
                {
                    if(iNowRow<iRow)
                    {
                        iNowCol=0;
                        iNowRow++;
                    }
                }

                if(iNowRow==iRow)
                {
                    if(bSetGold)
                        Task=300;
                    else
                        Task=200;
                }
                else
                {
                    Task=30;
                }
            }
            break;
        case 200:
            //比較
            flag1=false;
            ErrPart="";
            for(int i=0; i<fLaserSensor->mtShuttle1->XItem; i++)
            {
                for(int j=0; j<fLaserSensor->mtShuttle1->YItem; j++)
                {
                    if(iSht==0)
                    {
                        dValue[0]=atof(fLaserSensor->mtShuttle1->GetCellText(i, j).c_str());
                        dValue[1]=atof(fLaserSensor->mtShuttle1_Golden->GetCellText(i, j).c_str());
                    }
                    else
                    {
                        dValue[0]=atof(fLaserSensor->mtShuttle2->GetCellText(i, j).c_str());
                        dValue[1]=atof(fLaserSensor->mtShuttle2_Golden->GetCellText(i, j).c_str());
                    }

                    if(iSht==0)
                        ptrInSHT=&FLCarryKit;
                    else if(iSht==1)
                        ptrInSHT=&BLCarryKit;

                    if(ptrInSHT->Item[j][i]!=NULL_IC && ptrInSHT->Item[j][i]!=HAS_NULL_IC)
                    {
                        if((abs((dValue[0]-dValue[1])*100))>=TestIF_File.dInLaserThresholdSht*100)
                        {
                            SetShtTrayColor(iSht, SThreadPara.iCHKStep-i-1, j, lsFail);
//                            bHasErr=true;
                            ErrPart+=IndexSuckName[j][i];
                            flag1=true;
    //                        Str.sprintf("Row %d Col %d 異常", j, i);
    //                        ShowMyMessage(Str);
                        }
                    }
                }
            }

            if(flag1)
            {
                if(iSht==0)
                    ret=ShowErrorMessage("JAM0411", K_RETRY|K_SKIP, MInShuttle1, bHasDuplicateErr, ErrPart);   //Input Shuttle 1 device floating error.
                else
                    ret=ShowErrorMessage("JAM0412", K_RETRY|K_SKIP, MInShuttle2, bHasDuplicateErr, ErrPart);   //Input Shuttle 2 device floating error.

                if(ret==K_SKIP)
                {
                    flag=true;
                }
                else
                {
                    Task=250;
                }
            }
            else
            {
                flag=true;
            }
            break;
        case 250:
            GetRowCol(iRow, iCol);

            for(int i=0; i<fLaserSensor->mtShuttle1->XItem; i++)
            {
                for(int j=0; j<fLaserSensor->mtShuttle1->YItem; j++)
                {
                    if(bSetGold)
                        SetShtTrayColor(iSht, i, j, lsNeedCheck, true);
                    else
                        SetShtTrayColor(iSht, i, j, lsNeedCheck);
                }
            }
            iShuttleRetryCount=0;
            bHasDuplicateErr=false;
            iNowRow=0;
            iNowCol=0;
            Task=1;
            Step=0;
            break;
        case 300:
            ret=ShowMyMessageBox_YES_NO("Sure To Setting Laser Value", "確定要儲存測距數值？");
            if(ret==2)
            {
                Task=400;
            }
            else
            {
                fLaserSensor->SaveShuttleLaserValue(iSht);
                ShowMyMessage("請取出Shuttle上IC");
                flag=true;
            }
            break;
        case 400:
            for(int i=0; i<fLaserSensor->mtShuttle1->XItem; i++)
            {
                for(int j=0; j<fLaserSensor->mtShuttle1->YItem; j++)
                {
                    SetShtTrayColor(iSht, i, j, lsNormal, true);
                    if(iSht==0)
                    {
                        if(j==0)
                        {
                            TestIF_File.dLaserValue[iSht][0][iNowCol]=0.0;
                            SetShtTrayNumber(iSht, i, dTemp, 0.0, true);
                        }
                        else
                        {
                            TestIF_File.dLaserValue[iSht][1][iNowCol]=0.0;
                            SetShtTrayNumber(iSht, i, 0.0, dTemp, true);
                        }
                    }
                    else if(iSht==1)
                    {
                        if(j==0)
                        {
                            TestIF_File.dLaserValue[iSht][0][iNowCol]=0.0;
                            SetShtTrayNumber(iSht, i, dTemp, 0.0, true);
                        }
                        else
                        {
                            TestIF_File.dLaserValue[iSht][1][iNowCol]=0.0;
                            SetShtTrayNumber(iSht, i, 0.0, dTemp, true);
                        }
                    }
                }
            }
            ShowMyMessage("請取出Shuttle上IC");
            flag=true;
            break;
    }
    return flag;
}
//---------------------------------------------------------------------------
//<== Eastsun 20260525 laser 整合
//---------------------------------------------------------------------------
bool UseOutArmCheckShtFloating(int iSht, bool bReset, bool bSetGold) //Eastsun 20260525 laser 整合 完整照搬 a-side (iInOutSht=1)
{
    static int Step=0;
    static int iRow=0, iCol=0;
    static int iNowRow=0, iNowCol=0;
    static bool bHasDuplicateErr=false;
    static AnsiString ErrPart;
    AnsiString Str, Str1;

    bool flag=false;
//    bool bHasErr=false;
    bool flag1=false;
    double dTemp;
    double dValue[2];
    int ret;
    int myX;

    int &Task=iUseOutArmShtLaserCheckTask;

    if(bReset)
    {
        GetRowCol(iRow, iCol);

        for(int i=0; i<fLaserSensor->mtOutShuttle1->XItem; i++)
        {
            for(int j=0; j<fLaserSensor->mtOutShuttle1->YItem; j++)
            {
                if(bSetGold)
                    SetShtTrayColor(iSht, i, j, lsNeedCheck, true, 1);
                else
                    SetShtTrayColor(iSht, i, j, lsNeedCheck, false, 1);
            }
        }
        iShuttleRetryCount=0;
        bHasDuplicateErr=false;
        iNowRow=0;
        iNowCol=0;
        Task=1;
        Step=0;
        return flag;
    }

    switch(Task)
    {
        case 1:
            if(bSetGold)
                ShowMyMessage("請放置IC到Shuttle上");

            Task=30;
            break;
        case 30:
            flag1=MoveOutArmXYToSHLaserPos(iSht, iNowCol, iNowRow);
            if(bSetGold)
                SetShtTrayColor(iSht, iNowCol, iNowRow, lsChecking, true, 1);
            else
                SetShtTrayColor(iSht, iNowCol, iNowRow, lsChecking, false, 1);
            if(flag1)
            {
                LaserShtDelay.SetSecAndOn(dShtDelayTime);
                Task=50;
            }
            break;
        case 50:
            if(LaserShtDelay.Off())
            {
                //測距
                Task=100;
                asLaserValue[1]="";
                fLaserSensor->btGetOutShValue->Click();
                LaserOutShtTimeOut.SetSecAndOn(1);
            }
            break;
        case 100:
            if(LaserOutShtTimeOut.Off())
                flag1=true;

            if(asLaserValue[1]!="")
            {
                Str=asLaserValue[1].SubString(11, asLaserValue[1].Length()-14);
                dTemp=atof(Str.c_str());
                myX=SThreadPara.iCHKStep-iNowCol-1;
                if(bSetGold)
                {
                    if(iSht==0)
                    {
                        if(iNowRow==0)
                        {
                            TestIF_File.dLaserValueOutSh[iSht][0][myX]=dTemp;
                            SetShtTrayNumber(iSht, iNowCol, dTemp, 0.0, true, 1);
                        }
                        else
                        {
                            TestIF_File.dLaserValueOutSh[iSht][1][myX]=dTemp;
                            SetShtTrayNumber(iSht, iNowCol, 0.0, dTemp, true, 1);
                        }
                    }
                    else if(iSht==1)
                    {
                        if(iNowRow==0)
                        {
                            TestIF_File.dLaserValueOutSh[iSht][0][myX]=dTemp;
                            SetShtTrayNumber(iSht, iNowCol, dTemp, 0.0, true, 1);
                        }
                        else
                        {
                            TestIF_File.dLaserValueOutSh[iSht][1][myX]=dTemp;
                            SetShtTrayNumber(iSht, iNowCol, 0.0, dTemp, true, 1);
                        }
                    }
                }
                else
                {
                    if(iSht==0)
                    {
                        if(iNowRow==0)
                            SetShtTrayNumber(iSht, iNowCol, dTemp, 0.0, false, 1);
                        else
                            SetShtTrayNumber(iSht, iNowCol, 0.0, dTemp, false, 1);
                    }
                    else if(iSht==1)
                    {
                        if(iNowRow==0)
                            SetShtTrayNumber(iSht, iNowCol, dTemp, 0.0, false, 1);
                        else
                            SetShtTrayNumber(iSht, iNowCol, 0.0, dTemp, false, 1);
                    }
                }
                flag1=true;
            }

            if(flag1)
            {
                if(bSetGold)
                    SetShtTrayColor(iSht, iNowCol, iNowRow, lsNormal, true, 1);
                else
                    SetShtTrayColor(iSht, iNowCol, iNowRow, lsNormal, false, 1);

                iNowCol++;
                if(iNowCol<iCol)
                {
                }
                else
                {
                    if(iNowRow<iRow)
                    {
                        iNowCol=0;
                        iNowRow++;
                    }
                }

                if(iNowRow==iRow)
                {
                    if(bSetGold)
                        Task=300;
                    else
                        Task=200;
                }
                else
                {
                    Task=30;
                }
            }
            break;
        case 200:
            //比對
            flag1=false;
            ErrPart="";
            for(int i=0; i<fLaserSensor->mtOutShuttle1->XItem; i++)
            {
                for(int j=0; j<fLaserSensor->mtOutShuttle1->YItem; j++)
                {
                    if(iSht==0)
                    {
                        dValue[0]=atof(fLaserSensor->mtOutShuttle1->GetCellText(i, j).c_str());
                        dValue[1]=atof(fLaserSensor->mtOutShuttle1_Golden->GetCellText(i, j).c_str());
                    }
                    else
                    {
                        dValue[0]=atof(fLaserSensor->mtOutShuttle2->GetCellText(i, j).c_str());
                        dValue[1]=atof(fLaserSensor->mtOutShuttle2_Golden->GetCellText(i, j).c_str());
                    }

                    if(iSht==0)
                        ptrOutSHT=&FRCarryKit;
                    else if(iSht==1)
                        ptrOutSHT=&BRCarryKit;

                    if(ptrOutSHT->Item[j][i]!=NULL_IC && ptrOutSHT->Item[j][i]!=HAS_NULL_IC)
                    {
                        if((abs((dValue[0]-dValue[1])*100))>=TestIF_File.dOutLaserThresholdSht*100)
                        {
                            SetShtTrayColor(iSht, SThreadPara.iCHKStep-i-1, j, lsFail, false, 1);
//                            bHasErr=true;
                            ErrPart+=IndexSuckName[j][i];
                            flag1=true;
                        }
                    }
                }
            }

            if(flag1)
            {
                if(iSht==0)
                    ret=ShowErrorMessage("JAM0501", K_RETRY|K_SKIP, MOutShuttle1, bHasDuplicateErr, ErrPart);   //Out Shuttle 1 device floating error.
                else
                    ret=ShowErrorMessage("JAM0501", K_RETRY|K_SKIP, MOutShuttle2, bHasDuplicateErr, ErrPart);   //TODO Eastsun: a-side 用 JAM0502 但 c-side note.cpp 未註冊, 暫共用 JAM0501

                if(ret==K_SKIP)
                {
                    flag=true;
                }
                else
                {
                    Task=250;
                }
            }
            else
            {
                flag=true;
            }
            break;
        case 250:
            GetRowCol(iRow, iCol);

            for(int i=0; i<fLaserSensor->mtOutShuttle1->XItem; i++)
            {
                for(int j=0; j<fLaserSensor->mtOutShuttle1->YItem; j++)
                {
                    if(bSetGold)
                        SetShtTrayColor(iSht, i, j, lsNeedCheck, true, 1);
                    else
                        SetShtTrayColor(iSht, i, j, lsNeedCheck, false, 1);
                }
            }
            iShuttleRetryCount=0;
            bHasDuplicateErr=false;
            iNowRow=0;
            iNowCol=0;
            Task=1;
            Step=0;
            break;
        case 300:
            ret=ShowMyMessageBox_YES_NO("Sure To Setting Laser Value", "確定要儲存此距數值？");
            if(ret==2)
            {
                Task=400;
            }
            else
            {
                fLaserSensor->SaveShuttleLaserValue(iSht, 1);
                ShowMyMessage("請取出Out Shuttle上IC");
                flag=true;
            }
            break;
        case 400:
            for(int i=0; i<fLaserSensor->mtOutShuttle1->XItem; i++)
            {
                for(int j=0; j<fLaserSensor->mtOutShuttle1->YItem; j++)
                {
                    SetShtTrayColor(iSht, i, j, lsNormal, true, 1);
                    if(iSht==0)
                    {
                        if(j==0)
                        {
                            TestIF_File.dLaserValueOutSh[iSht][0][iNowCol]=0.0;
                            SetShtTrayNumber(iSht, i, dTemp, 0.0, true, 1);
                        }
                        else
                        {
                            TestIF_File.dLaserValueOutSh[iSht][1][iNowCol]=0.0;
                            SetShtTrayNumber(iSht, i, 0.0, dTemp, true, 1);
                        }
                    }
                    else if(iSht==1)
                    {
                        if(j==0)
                        {
                            TestIF_File.dLaserValueOutSh[iSht][0][iNowCol]=0.0;
                            SetShtTrayNumber(iSht, i, dTemp, 0.0, true, 1);
                        }
                        else
                        {
                            TestIF_File.dLaserValueOutSh[iSht][1][iNowCol]=0.0;
                            SetShtTrayNumber(iSht, i, 0.0, dTemp, true, 1);
                        }
                    }
                }
            }
            ShowMyMessage("請取出Shuttle上IC");
            flag=true;
            break;
    }
    return flag;
}
//---------------------------------------------------------------------------
