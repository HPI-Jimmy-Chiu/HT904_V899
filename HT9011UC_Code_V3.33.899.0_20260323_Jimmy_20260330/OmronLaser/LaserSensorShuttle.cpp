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

#pragma package(smart_init)

int iShtLaserInitTask=1;
int iShtLaserCheckTask=1;
TQPF_Timer LaserShtDelay;
TQPF_Timer LaserShtTimeOut;
double dCurrentValue[2][MAX_Index_Row][MAX_Index_Col];
double dShtDelayTime=0.5;  //等待馬達整定的時間
double dShtTimeOutTime=3.0;//讀取多久就是逾時
double dShtRetryGap=3.0;   //大於多少需要重新擷取一次
int iShuttleRetryCount; //重試的次數
int iShuttleMaxRetry=2; //最大重試次數


//---------------------------------------------------------------------------
void SetShtTrayColor(int iSht, int iX, int iY, eLaserStatus Status, bool Golden=false);
void SetShtTrayNumber(int iSht, int iX, double dNum1, double dNum2, bool Goldn=false);
void SetShtTrayColor(int iSht, int iX, int iY, eLaserStatus Status, bool Golden)
{
    int myX=SThreadPara.iCHKStep-iX-1;

    if(Golden)
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
            fLaserSensor->mtShuttle1->SetCellColorIndex(myX, iY, Status);
        }
        else
        {
            fLaserSensor->mtShuttle2->SetCellColorIndex(myX, iY, Status);
        }
    }
}
//---------------------------------------------------------------------------
void SetShtGoldenTrayColor(int iSht, int iX, int iY, eLaserStatus Status)
{
    SetShtTrayColor(iSht, iX, iY, Status, true);
}
//---------------------------------------------------------------------------
void SetShtTrayNumber(int iSht, int iX, double dNum1, double dNum2, bool Golden)
{
    int myX=SThreadPara.iCHKStep-iX-1;
    if(Golden)
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
