//---------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MyTempture.h"
#pragma link "SPComm"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SPComm"
#pragma resource "*.dfm"

TDataModule2 *DataModule2;
//---------------------------------------------------------------------------
__fastcall TDataModule2::TDataModule2(TComponent* Owner)
    : TDataModule(Owner)
{
}
//------------------------------------------------------------------------------
// 解構子
//------------------------------------------------------------------------------
TMyTempCtrl::~TMyTempCtrl()
{
    Timer1->Enabled=false;
    for(int i=0; i<MAX_CONTROL_UNIT; i++)
    {
        if(TempMainPal[i]!=NULL)
            delete TempMainPal[i];

        if(TempMonitor[i]!=NULL)
            delete TempMonitor[i];

        if(Series[i]!=NULL)
            delete Series[i];

        if(TempOffset[i]!=NULL)
            delete TempOffset[i];
    }

    delete Timer1;
}
//------------------------------------------------------------------------------
// 建構子
//------------------------------------------------------------------------------
TMyTempCtrl::TMyTempCtrl()
{
    int i, j;

    for(i=0; i<MAX_CONTROL_UNIT; i++)
    {
        bHasUnitArray[i]=false;
        bSetTemp[i]=false;
        fReadSetTemp[i]=0;
        bReadSetTemp[i]=false;
        iMaxPeriodCount[i]=0;
        bAutoTuningDevice[i]=false;
        bUseBias[i]=false;
        bFirstRecord[i]=true;
        for(j=0; j<MAX_HISTROY__RECORD_COUNT; j++)
            fHistroyTemp[i][j]=0;
        Alias[i]="";
        bMonitorSelect[i]=false;
        TempMainPal[i]=new TMyTempMainPanel(NULL);
        TempMonitor[i]=new TMyTempMonitor(NULL);
        TempOffset[i] =new TMyTempOffset(NULL);     //溫控器的Offset
        Series[i]     =new TLineSeries(NULL);
    }
    bStartSetTemp=false;
    bHasUnit=false;
    Com2ReceiveOK=false;
    ComPort=-1;
    ComParity=None;
    bAutoTuning=false;
    bStopAutoTuning=false;
    Timer1=new TTimer(NULL);
    Timer1->OnTimer=Timer1Timer;
    Timer1->Interval=30;
    Timer1->Enabled=true;
    iTotalInstalledUnit=-1;

    bStartReadPIDData=false;                             //2011.03.10 Q_Q V208D Temp PID Add
    bStartWritePIDData=false;                            //2011.03.10 Q_Q V208D Temp PID Add
    bAmbientMode=true;
    bShowChart=false;
    bStartMonitor=false;
}
//------------------------------------------------------------------------------
void TMyTempCtrl::RecordTempture(int Index, double Temp)
{
    int i;
    static int iSaveMin[MAX_CONTROL_UNIT]={0};
    static bool bFirstRecord[MAX_CONTROL_UNIT]={false};

    //fTemp[Index]=Temp;
    fTemp[Index] = Temp - fOffset[Index];   // 2010.07.19 , Joye

    if(bFirstRecord[Index]==false)
    {
        for(i=0; i<MAX_HISTROY__RECORD_COUNT; i++)
        {
            fHistroyTemp[Index][i]=Temp;
            asHistroyTempTime[Index][i].sprintf("%2d:%2d", SystemHour, SystemMin);
        }
        iSaveMin[Index]=SystemMin;
        bFirstRecord[Index]=true;
    }
    else
    {
        if(iSaveMin[Index]!=SystemMin)    //每分鐘一次
        {
            for(i=MAX_HISTROY__RECORD_COUNT-1; i>0; i--)
            {
                fHistroyTemp[Index][i]=fHistroyTemp[Index][i-1];
                asHistroyTempTime[Index][i]=asHistroyTempTime[Index][i-1];
            }
            asHistroyTempTime[Index][0].sprintf("%2d:%2d", SystemHour, SystemMin);
            fHistroyTemp[Index][0]=Temp;
            iSaveMin[Index]=SystemMin;
        }
    }

    if(bShowChart)
    {
        for(i=0; i<MAX_HISTROY__RECORD_COUNT; i--)
        {
            Series[Index]->AddY(fHistroyTemp[Index][i], asHistroyTempTime[Index][i], Series[Index]->AreaColor);
        }
    }
}
//------------------------------------------------------------------------------
void TMyTempCtrl::GetTemptureHistroy(int Index, int *RecordCount, double *HistroyTemp)
{
    int i;
    (*RecordCount)=MAX_HISTROY__RECORD_COUNT;
    for(i=0; i<MAX_HISTROY__RECORD_COUNT; i++)
        HistroyTemp[i]=fHistroyTemp[Index][i];
}
//------------------------------------------------------------------------------
// virtual function,不得刪除
//------------------------------------------------------------------------------
/*void TMyTempCtrl::ReadData(int Addr, int Command)
{
}*/
//------------------------------------------------------------------------------
// virtual function,不得刪除
// 對address=Index溫控器送出欲設定之 Registor
//------------------------------------------------------------------------------
void TMyTempCtrl::WriteData(int Addr, int Command, short Value)
{
}
//------------------------------------------------------------------------------
// virtual function,不得刪除
// 對溫控器群組送出欲設定之溫度值
//------------------------------------------------------------------------------
bool TMyTempCtrl::DoStartSetTemp()
{
    return false;
}
//------------------------------------------------------------------------------
// virtual function,不得刪除
// 開始讀取address=Index溫控器預定加熱之溫度值
//------------------------------------------------------------------------------
bool TMyTempCtrl::ReadTargetTemp(int Addr)
{
    return false;
}
//------------------------------------------------------------------------------
// virtual function,不得刪除
// 掃描所有溫控器目前溫度值 並儲存於 fTemp[] 中
//------------------------------------------------------------------------------
bool TMyTempCtrl::DoReadCurrentTemp()
{
    return false;
}
//------------------------------------------------------------------------------
// virtual function,不得刪除
// 對溫控器群組送出 Auto Turning 動作要求
//------------------------------------------------------------------------------
bool TMyTempCtrl::DoAutoTuning()
{
    return false;
}

//2011.03.10 Q_Q V208D Temp PID Add
bool TMyTempCtrl::DoReadPIDData()
{
    return false;
}
//2011.03.10 Q_Q V208D Temp PID Add
bool TMyTempCtrl::DoWritePIDData()
{
    return false;
}

//------------------------------------------------------------------------------
// 溫控器處理中心
//------------------------------------------------------------------------------
void __fastcall TMyTempCtrl::Timer1Timer(TObject *Sender)
{
    //jou 2010-05-19 start : 未initital完成,不能執行
    if(InitialOK==false)
        return;
    //jou 2010-05-19 end

    static int Task=1;
    ///static int Addr=0;
    ///static int CommRetry[MAX_CONTROL_UNIT];
    int pos, P;
    AnsiString S, S1, S2;
    int attr, pH, pL, i;
    double p, Read, Temp;
    ///static double OldTemp[MAX_CONTROL_UNIT];
    ///static int ReceiveErr=-1;
    static bool bEMG;
    bEMG=false;
    ///static bool ReStartComflag=false;
    AnsiString CN;

    switch(Task)
    {
        case 1:
            if(ComPort==-1)
                return;
            Comm2=DataModule2->Tempture;
            Comm2->OnReceiveData=Comm2ReceiveData;
            Comm2->Parity=ComParity;
            if(GetCOMPortStatus(ComPort))
            {
                try
                {
                    CN="\\\\.\\COM";
                    CN+=AnsiString(ComPort);
                    Comm2->CommName=CN;
                    Comm2->StartComm();
                }
                catch(...)
                {
                    ShowMyMessage("Error open com port");
                }
                Task=100;
            }
            break;
        case 100:
            if(bHasUnit==false)
                break;
            if(bStartSetTemp==true)
            {
                iStartSetTempTask=1;
                Task=1000;
            }
            else if(bAutoTuning==true)
            {
                iStartAutoTuningTask=1;
                Task=1100;
            }
            else if(bStartReadPIDData==true)                                    //2011.03.10 Q_Q V208D Temp PID Add
            {
                iStartReadPIDDataTask=1;
                Task=1120;
            }
            else if(bStartWritePIDData==true)                                   //2011.03.10 Q_Q V208D Temp PID Add
            {
                iStartWritePIDDataTask=1;
                Task=1130;
            }
            else
            {
                iReadCurrentTempTask=1;
                Task=1200;
                //Task=100;
            }
            break;
        case 1000:
            if(DoStartSetTemp()==true)
            {
                bStartSetTemp=false;
                Task=100;
            }
            break;
        case 1100:
            if(DoAutoTuning()==true || bStopAutoTuning)
            {
                bAutoTuning=false;
                Task=1110;
            }
            break;
        case 1110:
            //if(CheckAllControlHasReady())
            //{
            //    bStopAutoTuning=false;
            //    Task=100;
            //    break;
            //}

            if(bStopAutoTuning==true)
            {
                bStopAutoTuning=false;
                Task=100;
            }
            break;
        case 1120:

            if(DoReadPIDData()==true)                                           //2011.03.10 Q_Q V208D Temp PID Add
            {
                bStartReadPIDData=false;
                Task=100;
            }
            break;

       case 1130:

            if(DoWritePIDData()==true)                                          //2011.03.10 Q_Q V208D Temp PID Add
            {
                bStartWritePIDData=false;
                Task=100;
            }
            break;

        case 1200:
            if(DoReadCurrentTemp()==true)
                Task=100;
            break;
    }
}
//------------------------------------------------------------------------------
// 溫控器COM PORT 接收資料function
//------------------------------------------------------------------------------
void __fastcall TMyTempCtrl::Comm2ReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    char *P;
    P=(char *)Buffer;
    int i;

    Receivelen=BufferLength;
    if(BufferLength<1020)
    {
        for(i=0; i<BufferLength; i++)
            Com2ReceiveByte[i]=P[i];                 // for RTU mode
    }

    if(BufferLength >250)
        BufferLength=250;

    strncpy(Com2Buffer, (char*)Buffer,BufferLength);  // for ASCII mode use
    Com2Buffer[BufferLength]='\x0';
    Com2ReceiveOK=true;
}
//------------------------------------------------------------------------------
// 設定溫控器群組是利用那一個COM PORT
//------------------------------------------------------------------------------
void TMyTempCtrl::SetComPort(int port)
{
    ComPort=port;
}
//------------------------------------------------------------------------------
// 設定溫控器群組是利用那一個Parity
//------------------------------------------------------------------------------
void TMyTempCtrl::SetComParity(TParity Parity)
{
    ComParity=Parity;
}
//------------------------------------------------------------------------------
// 設定address=Index 的溫控器是有存在的
//------------------------------------------------------------------------------
void TMyTempCtrl::InstalledUnit(int Index)
{
    bHasUnitArray[Index]=true;
    bHasUnit=true;
    if(iTotalInstalledUnit<Index)
        iTotalInstalledUnit=Index;
};
//------------------------------------------------------------------------------
void TMyTempCtrl::OpenUnit(int Index)
{
    bHasUnitArray[Index]=true;
    bHasUnit=true;
};
//------------------------------------------------------------------------------
void TMyTempCtrl::CloseUnit(int Index)
{
    bHasUnitArray[Index]=false;
//    bHasUnit=false;
};
//------------------------------------------------------------------------------
int TMyTempCtrl::GetTotalInstalledUnit()
{
    return iTotalInstalledUnit+1;
};
//------------------------------------------------------------------------------
bool TMyTempCtrl::UnitHasInstall(int Index)
{
    return bHasUnitArray[Index];
};
//------------------------------------------------------------------------------
// 將address=Index溫控器設定預加熱之溫度f
//------------------------------------------------------------------------------
void TMyTempCtrl::WriteTargetTemp(int Index, double f)
{
    bStartSetTemp=true;
    bSetTemp[Index]=true;
    fSetTemp[Index]=f;
    bUseBias[Index]=false;
};
//------------------------------------------------------------------------------
// 讀取address=Index 的溫控器目前溫度值
//------------------------------------------------------------------------------
double TMyTempCtrl::ReadCurrentTemp(int Index)
{
    double f=0.0;
    double fTargetMax, fTargetMin, fTarget;
    double fMeasureMax, fMeasureMin;
    double fReadMax(999), fReadMin(-999);
    bool bHasF=false;
    int i;

    if(bUseBias[Index]==true)
    {
        f=fTemp[Index];
    }
    else
    {
        //30  60  90
        //25  53  87

        if(iMaxPeriodCount[Index]<2)
        {
            f=fTemp[Index];
        }
        else
        {
            //  find f
            //  fTargetMax -fTargetMin     fTargetMax-fTemp[Index]
            //  -----------------------  = -------------------
            //  fMeasureMax-fMeasureMin    fMeasureMax-f

            //                                                fMeasureMax-fMeasureMin
            //  fMeasureMax-f   =(fTargetMax-fTemp[Index]) * ------------------------
            //                                                fTargetMax -fTargetMin

            //                                              fMeasureMax-fMeasureMin
            //  f=fMeasureMax-(fTargetMax-fTemp[Index]) *  ------------------------
            //                                              fTargetMax -fTargetMin

            ///bool bOldCalculate = false;
            //if(fTemp[Index]<fTargetPointPeriod[Index][0])
            if(fTemp[Index] < fTargetPointPeriod[Index][0] + (fTargetPointPeriod[Index][0]-fMeasureValuePeriod[Index][0]))      // 2011.01.13 , Joye
            {

                fMeasureMin = fMeasureValuePeriod[Index][0];
                fMeasureMax = fMeasureValuePeriod[Index][1];
                fTargetMin  = fTargetPointPeriod[Index][0];
                fTargetMax  = fTargetPointPeriod[Index][1];

                fReadMin    = fTargetMin+(fTargetMin-fMeasureMin);      // 2011.01.13 , Joye
                fReadMax    = fTargetMax+(fTargetMax-fMeasureMax);      // 2011.01.13 , Joye

            }
            //else if(fTemp[Index]>fTargetPointPeriod[Index][iMaxPeriodCount[Index]-1])
            else if(fTemp[Index] > fTargetPointPeriod[Index][iMaxPeriodCount[Index]-1] + (fTargetPointPeriod[Index][iMaxPeriodCount[Index]-1]-fMeasureValuePeriod[Index][iMaxPeriodCount[Index]-1]))      // 2011.01.13 , Joye
            {
                fMeasureMin = fMeasureValuePeriod[Index][iMaxPeriodCount[Index]-2];
                fMeasureMax = fMeasureValuePeriod[Index][iMaxPeriodCount[Index]-1];
                fTargetMin  = fTargetPointPeriod[Index][iMaxPeriodCount[Index]-2];
                fTargetMax  = fTargetPointPeriod[Index][iMaxPeriodCount[Index]-1];

                fReadMin    = fTargetMin+(fTargetMin-fMeasureMin);      // 2011.01.13 , Joye
                fReadMax    = fTargetMax+(fTargetMax-fMeasureMax);      // 2011.01.13 , Joye

            }
            else
            {
                for(i=1; i<iMaxPeriodCount[Index]; i++)
                {
                    fMeasureMin = fMeasureValuePeriod[Index][i-1];
                    fMeasureMax = fMeasureValuePeriod[Index][i];
                    fTargetMin  = fTargetPointPeriod[Index][i-1];
                    fTargetMax  = fTargetPointPeriod[Index][i];

                    // 2011.01.13 , Joye , V2.36A {
                    fReadMin    = fTargetMin+(fTargetMin-fMeasureMin);
                    fReadMax    = fTargetMax+(fTargetMax-fMeasureMax);

                    /*if(fTemp[Index]==fTargetMin)
                    {
                        return fMeasureMin;
                    }
                    if(fTemp[Index]==fTargetMax)
                    {
                        return fMeasureMax;
                    }
                    if(fTemp[Index]>=fTargetMin && fTemp[Index]<=fTargetMax)
                    {
                        break;
                    }*/
                    if(fTemp[Index] >= fReadMin && fTemp[Index] <= fReadMax )
                    {
                        f=fTemp[Index];
                        bHasF=true;
                    }
                    //
                }
            }

            if(bHasF==false)
            {
                //if(fMeasureMax==fMeasureMin)
                if(fMeasureMax <= fMeasureMin || fTargetMax <= fTargetMin || fReadMax <= fReadMin )    // 2011.01.13 , Joye , V2.36A
                {
                    //return fTargetMin;
                    f=999;
                }
                else if(bAmbientMode==true || bMonitorSelect[Index] == false)    // 2011.01.13 , Joye
                {
                    f=fTemp[Index];
                }
                else
                {
                    //                                              fMeasureMax-fMeasureMin
                    //  f=fMeasureMax-(fTargetMax-fTemp[Index]) *  ------------------------
                    //                                              fTargetMax -fTargetMin

                    //f=fMeasureMax-(fTargetMax-fTemp[Index]) *(fMeasureMax-fMeasureMin)/(fTargetMax -fTargetMin);
                    f=fTargetMax-(fTargetMax-fTargetMin)*(fReadMax-fTemp[Index])/(fReadMax-fReadMin);           // 2011.01.13 , Joye
                }
            }
        }
    }
    ShowTempValue(Index, f);
    return f;
}
//------------------------------------------------------------------------------
// 將溫控器溫度區間重置
//------------------------------------------------------------------------------
void TMyTempCtrl::ClearPointPeriod(int Index)
{
    iMaxPeriodCount[Index]=0;
}
//------------------------------------------------------------------------------
// 加入一組溫控器溫度區間
//------------------------------------------------------------------------------
bool TMyTempCtrl::AddPointPeriod(int Index, double fTarget, double fMeasue)
{
    if(iMaxPeriodCount[Index]>=20)
        return false;
    fTargetPointPeriod[Index][iMaxPeriodCount[Index]]=fTarget;
    fMeasureValuePeriod[Index][iMaxPeriodCount[Index]]=fMeasue;
    iMaxPeriodCount[Index]++;
    return true;
}
//------------------------------------------------------------------------------
// 將address=Index溫控器設定被轉換後之預加熱之溫度f
//------------------------------------------------------------------------------
bool TMyTempCtrl::WriteTargetConvertTemp(int Index, double f , double fNowOffset)
{
    int i;
    double fTargetMax, fTargetMin, fTarget;
    double fMeasureMax, fMeasureMin;
    if(iMaxPeriodCount[Index]<2)
        return false;

    //  fTargetMax -fTargetMin     fTargetMax-fTarget
    //  -----------------------  = -------------------
    //  fMeasureMax-fMeasureMin    fMeasureMax-f

    //                                         fTargetMax -fTargetMin
    //  fTarget=fTargetMax - (fMeasureMax-f) * -----------------------
    //                                         fMeasureMax-fMeasureMin

    ///bool bOldCalculate = false;
    //if(f<fMeasureValuePeriod[Index][0])
    if(f < fTargetPointPeriod[Index][0])      // 2011.01.13 , Joye , V2.36A
    {

        fMeasureMin = fMeasureValuePeriod[Index][0];
        fMeasureMax = fMeasureValuePeriod[Index][1];
        fTargetMin  = fTargetPointPeriod[Index][0];
        fTargetMax  = fTargetPointPeriod[Index][1];

    }
    //else if(f>fMeasureValuePeriod[Index][iMaxPeriodCount[Index]-1])
    else if(f > fTargetPointPeriod[Index][iMaxPeriodCount[Index]-1])      // 2011.01.13 , Joye , V2.36A
    {
        fMeasureMin = fMeasureValuePeriod[Index][iMaxPeriodCount[Index]-2];
        fMeasureMax = fMeasureValuePeriod[Index][iMaxPeriodCount[Index]-1];
        fTargetMin  = fTargetPointPeriod[Index][iMaxPeriodCount[Index]-2];
        fTargetMax  = fTargetPointPeriod[Index][iMaxPeriodCount[Index]-1];
    }
    else
    {
        for(i=1; i<iMaxPeriodCount[Index]; i++)
        {
            fMeasureMin = fMeasureValuePeriod[Index][i-1];
            fMeasureMax = fMeasureValuePeriod[Index][i];
            fTargetMin  = fTargetPointPeriod[Index][i-1];
            fTargetMax  = fTargetPointPeriod[Index][i];

            // 2011.01.13 , Joye , V2.36A {
            /*if(f==fMeasureMin)
            {
                fSetTemp[Index]=fTargetMin;
                bStartSetTemp=true;
                bSetTemp[Index]=true;
                bUseBias[Index]=false;
                return true;
            }
            if(f==fMeasureMax)
            {
                fSetTemp[Index]=fTargetMax;
                bStartSetTemp=true;
                bSetTemp[Index]=true;
                bUseBias[Index]=false;
                return true;
            }*/
            // }

            //if(f>=fMeasureMin && f<=fMeasureMax)
            if(f >= fTargetMin && f <= fTargetMax )    // 2011.01.13 , Joye , V2.36A
            {
                break;
            }
        }
    }
    //if(fMeasureMax==fMeasureMin)
    if(fMeasureMax <= fMeasureMin || fTargetMax <= fTargetMin )    // 2011.01.13 , Joye , V2.36A
    {
        return false;
    }


    //20100515  Yi start
    /*if(LastSet.iTemperature)
        fTarget=fTargetMax-(fMeasureMax-f)*(fTargetMax -fTargetMin)/(fMeasureMax-fMeasureMin);
    else
        fTarget=f;
    */
    //20100515  Yi  end

    //if(LastSet.iTemperature ==false)
    //if(LastSet.iTemperature == false || bMonitorSelect[Index] == false)   // 2010.12.22 , Joye ,
    if(bMonitorSelect[Index] == false || f == 0)   // 2011.09.08 , Joye ,
    {
        fTarget = 0;
        fNowOffset = 0;      // 2011.01.13 , Joye
    }
    else
    {
        //fTarget=fTargetMax-(fMeasureMax-f)*(fTargetMax -fTargetMin)/(fMeasureMax-fMeasureMin);
        //fTarget=fTargetMax-(fMeasureMax-f)*(fTargetMax -fTargetMin)/(fMeasureMax-fMeasureMin) + fNowOffset;    // 2010.07.19 , Joye
        fTarget = f + (fTargetMin-fMeasureMin) + ((fTargetMax-fMeasureMax)-(fTargetMin-fMeasureMin))*(1-(fTargetMax-f)/(fTargetMax-fTargetMin)) + fNowOffset;    // 2011.01.13 , Joye

    }


    fOffset[Index] = fNowOffset;
    fSetTemp[Index]= fTarget;
    fBias[Index]=0;
    bStartSetTemp=true;
    bSetTemp[Index]=true;
    bUseBias[Index]=false;
    return true;
}
//------------------------------------------------------------------------------
// 將address=Index溫控器設定被轉換後之預加熱之溫度f 並且使用 偏差值
//------------------------------------------------------------------------------
bool TMyTempCtrl::WriteTargetTempUseBias(int Index, double f)
{
    int i;
    double fTargetMax, fTargetMin, fTarget;
    double fMeasureMax, fMeasureMin;
    if(iMaxPeriodCount[Index]<2)
        return false;

    //  fTargetMax -fTargetMin     fTargetMax-fTarget
    //  -----------------------  = -------------------
    //  fMeasureMax-fMeasureMin    fMeasureMax-f

    //                                         fTargetMax -fTargetMin
    //  fTarget=fTargetMax - (fMeasureMax-f) * -----------------------
    //                                         fMeasureMax-fMeasureMin

    if(f<fMeasureValuePeriod[Index][0])
    {
        fMeasureMin=fMeasureValuePeriod[Index][0];
        fMeasureMax=fMeasureValuePeriod[Index][1];
        fTargetMin=fTargetPointPeriod[Index][0];
        fTargetMax=fTargetPointPeriod[Index][1];
    }
    else if(f>fMeasureValuePeriod[Index][iMaxPeriodCount[Index]-1])
    {
        fMeasureMin=fMeasureValuePeriod[Index][iMaxPeriodCount[Index]-2];
        fMeasureMax=fMeasureValuePeriod[Index][iMaxPeriodCount[Index]-1];
        fTargetMin=fTargetPointPeriod[Index][iMaxPeriodCount[Index]-2];
        fTargetMax=fTargetPointPeriod[Index][iMaxPeriodCount[Index]-1];
    }
    else
    {
        for(i=1; i<iMaxPeriodCount[Index]; i++)
        {
            fMeasureMin=fMeasureValuePeriod[Index][i-1];
            fMeasureMax=fMeasureValuePeriod[Index][i];
            fTargetMin=fTargetPointPeriod[Index][i-1];
            fTargetMax=fTargetPointPeriod[Index][i];
            if(f==fMeasureMin)
            {
               fBias[Index]=f-fTargetMin;
               fSetTemp[Index]=f;
               bStartSetTemp=true;
               bSetTemp[Index]=true;
               bUseBias[Index]=true;
               return true;
            }
            if(f==fMeasureMax)
            {
               fBias[Index]=f-fTargetMax;
               fSetTemp[Index]=f;
               bStartSetTemp=true;
               bSetTemp[Index]=true;
               bUseBias[Index]=true;
               return true;
            }
            if(f>=fMeasureMin && f<=fMeasureMax)
            {
                break;
            }
        }
    }
    if(fMeasureMax==fMeasureMin)
    {
        return false;
    }

    fTarget=fTargetMax-(fMeasureMax-f)*(fTargetMax-fTargetMin)/(fMeasureMax-fMeasureMin);
    fBias[Index]=f-fTarget;
    fSetTemp[Index]=f;
    bStartSetTemp=true;
    bSetTemp[Index]=true;
    bUseBias[Index]=true;
    return true;
};
//------------------------------------------------------------------------------
// 將address=Index溫控器設定溫度f,作Auto Tuning
//------------------------------------------------------------------------------
void TMyTempCtrl::AutoTuning(int Index)
{
    bStopAutoTuning=false;
    bAutoTuning=true;
    bAutoTuningDevice[Index]=true;
}
//------------------------------------------------------------------------------
void TMyTempCtrl::StopAutoTuning()
{
    bStopAutoTuning=true;
}
//------------------------------------------------------------------------------
double TMyTempCtrl::ReadPIDData(int Index,int PID)                              //2011.03.10 Q_Q V208D Temp PID Add
{
    return fReadSetPID[Index][PID];
}
//------------------------------------------------------------------------------
void TMyTempCtrl::WriteSetPID(int Index,int P,int I,int D)                      //2011.03.10 Q_Q V208D Temp PID Add
{
    bStartWritePIDData=true;
    bSetPID[Index]=true;
    iSetPID[Index][0]=P;iSetPID[Index][1]=I;iSetPID[Index][2]=D;
}
//------------------------------------------------------------------------------
void TMyTempCtrl::StartReadPIDData()                                            //2011.03.10 Q_Q V208D Temp PID Add
{
    bStartReadPIDData=true;
}
//------------------------------------------------------------------------------
void TMyTempCtrl::StartWritePID()                                               //2011.03.10 Q_Q V208D Temp PID Add
{
    bStartWritePIDData=true;
}
//------------------------------------------------------------------------------
void TMyTempCtrl::ShowTempValue(int index, double value)
{
    //jou 2010-05-19 start : 未initital完成,不能執行
    if(InitialOK==false)
        return;
    //jou 2010-05-19 end
    TempMainPal[index]->SetValue(value, bMonitorSelect[index]);

    if(bStartMonitor)
    {
        if(bMonitorSelect[index])
            TempMonitor[index]->SetValue(value);
    }
}
//---------------------------------------------------------------------------
bool TMyTempCtrl::GetCOMPortStatus(int i)
{
    HANDLE h=INVALID_HANDLE_VALUE;
    AnsiString CN="\\\\.\\COM";
    CN+=AnsiString(i);
    h=::CreateFile(CN.c_str(),
        GENERIC_READ|GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);
    CloseHandle(h);
    if(h==INVALID_HANDLE_VALUE)
        return false;
    return true;
}
//---------------------------------------------------------------------------
void TMyTempCtrl::GetTimeInfo()
{
    TDateTime dtPresent;
    dtPresent=Now();
    DecodeDate(dtPresent, SystemYear, SystemMonth, SystemDate);
    DecodeTime(dtPresent, SystemHour, SystemMin, SystemSec, SystemMSec);
}
//---------------------------------------------------------------------------
int TMyTempCtrl::HexStrToInt(char *str)
{
    int i,len,sum=0,Scale=1,j;
    char c;
    if(strlen(str)==0)  return 0;
    CutSpaceAtHead(str);
    CutSpaceAtTail(str);
    strcpy( str,strupr(str));
    len=strlen(str);
    sum=0;
    for(i=(len-1); i>=0;i--)
    {
        c=str[i];
        if((c>='0' && c<='9'))
            j=(int)(c-'0');
        else if((c>='A' && c<='F'))
            j=10+(int)(c-'A');
        else
            break;
        sum+=j*Scale;
        Scale*=16;
    }
    return sum;
}
//---------------------------------------------------------------------------
void TMyTempCtrl::CutSpaceAtTail(char *S)
{
    int len;
    len=strlen(S);
    while(len)
    {
        if(S[len]==' ')
            S[len]='\0';
        else if(S[len]!='\0')
        {
            return;
        }
        len--;
    }
}
//---------------------------------------------------------------------------
void TMyTempCtrl::CutSpaceAtHead(char *S)
{
    char str2[256]={""};
    int i=0,pos=0;
    while(1)
    {
        if(S[i]!=' ')
            break;
        i++;
    }
    if(S[i]==0)
        return;
    while(1)
    {
        str2[pos]=S[i];
        if(S[i]==0)
            break;
        pos++;
        i++;
    }
    strcpy(S,str2);
}

