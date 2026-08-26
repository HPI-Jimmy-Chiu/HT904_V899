//==============================================================================
//
//   YOKOGAWA UT100 系列
//
//==============================================================================
#include <vcl.h>
#include <stdio.h>
#include <Filectrl.hpp>
#pragma hdrstop

#define UT100_STX         2
#define UT100_ETX         3
#define UT100_CR         13
#include "MyTempture_UT100.h"
//#include "cpublic.h"
//#include "cmydef.h"
//------------------------------------------------------------------------------
// 對address=Index溫控器送出欲讀取之 Registor
//------------------------------------------------------------------------------
void TMyYokogawa::ReadData(int Addr, int Command)
{
     sprintf(SendBuffer, "%c%02d010WRDD%04d,01%c%c", UT100_STX, Addr+1, Command, UT100_ETX, UT100_CR); //Polling 現在溫度
     Comm2->WriteCommData(SendBuffer, strlen(SendBuffer)+1);
}
//------------------------------------------------------------------------------
// 對address=Index溫控器送出欲設定之 Registor
//------------------------------------------------------------------------------
void TMyYokogawa::WriteData(int Addr, int Command, short Value)
{
     char str[256]={"SEND   :"};
     AnsiString S;

     if(Value>=0)
     {
        sprintf(SendBuffer, "%c%02d010WWRD%04d,01,%02X%02X%c%c", UT100_STX, Addr+1, Command, Value/256, Value%256, UT100_ETX, UT100_CR);
     }
     else
     {
         sprintf(str, "%04X", Value);
         S=str;
         S=S.SubString(5, 4);
         sprintf(SendBuffer,"%c%02d010WWRD%04d,01,%04s%c%c", UT100_STX, Addr+1, Command, S.c_str(), UT100_ETX, UT100_CR);
     }
     //S=str;
     //sprintf(SendBuffer,"%c%02d010WWRD%04d,01,%04s%c%c",UT100_STX,Addr+1,Command,S.c_str(),UT100_ETX,CR);

     Comm2->WriteCommData(SendBuffer, strlen(SendBuffer)+1);
}
//------------------------------------------------------------------------------
// 掃描所有溫控器目前溫度值 並儲存於 fTemp[] 中
//------------------------------------------------------------------------------
bool TMyYokogawa::DoReadCurrentTemp()
{
    int &Task=iReadCurrentTempTask;
    int i, pos;//, pH, pL, P;
    AnsiString S, S1, S2;
    float p, Read, Temp;

    switch(Task)
    {
        case 1:
            if(bHasUnit==false)
                return true;
            Addr=0;
            Task=100;
            break;
        case 100:
            if(bHasUnitArray[Addr]==true)
            {
                CommRetry[Addr]=_MAX_RETRY;
                Task=200;
                break;
            }
            while(1)
            {
                Addr++;
                if(Addr>=MAX_CONTROL_UNIT)
                    return true;
                if(bHasUnitArray[Addr]==true)
                {
                    CommRetry[Addr]=_MAX_RETRY;
                    Task=200;
                    break;
                }
            };
            break;
        case 200:
            Com2ReceiveOK=false;
            ReadData(Addr,2);
            Com2Delay.Clear();
            Com2Delay.Set(10);
            Com2Delay.On();
            Task=300;
            break;
          case 300:
            if(Com2ReceiveOK)
            {
                S=Com2Buffer;
                pos=S.Pos("OK");
                if(pos!=6) // not correct format
                {
                    //if(CommRetry[Addr])
                    if(CommRetry[Addr] > 0 && CommRetry[Addr] <= _MAX_RETRY )     // 2010.08.31 , Joye
                    {
                        CommRetry[Addr]--;
                        Task=200;
                    }
                    else
                    {
                        CommRetry[Addr]=_MAX_RETRY;
                        RecordTempture(Addr,999);
                        Task=400;
                    }
                    break;
                }
                S=S.SubString(8, 4);    //8~11位元為溫度值
                Read=(HexStrToInt(S.c_str()))/10.0f;

                /*S=S.SubString(pos+2,4);
                S1=S.SubString(1,2);
                S2=S.SubString(3,2);
                pH=HexStrToInt(S1.c_str());
                pL=HexStrToInt(S2.c_str());
                P=pH*256+pL;
                Read=P/10.0; */
                if(Read==0)
                {
                    //if(CommRetry[Addr])
                    if(CommRetry[Addr] > 0 && CommRetry[Addr] <= _MAX_RETRY )     // 2010.08.31 , Joye
                    {
                        CommRetry[Addr]--;
                        Task=200;
                        break;
                    }
                }
                RecordTempture(Addr, Read);
                Task=400;
                break;
            }

            if(Com2Delay.Off())
            {
                //if(CommRetry[Addr])
                if(CommRetry[Addr] > 0 && CommRetry[Addr] <= _MAX_RETRY )     // 2010.08.31 , Joye
                {
                    CommRetry[Addr]--;
                    Task=200;
                }
                else
                {
                    CommRetry[Addr]=_MAX_RETRY;
                    RecordTempture(Addr, 999);
                    Task=400;
                    break;
                }
            }
            break;
        case 400:
            Addr++;
            Task=100;
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
// 對溫控器群組送出欲設定之溫度值
//------------------------------------------------------------------------------
bool TMyYokogawa::DoStartSetTemp()
{
    int i;
    int &Task=iStartSetTempTask;
    switch(Task)
    {
        case 1:
            Addr=0;
            Task=100;
            break;
        case 100:
            while(1)
            {
                if(Addr>=MAX_CONTROL_UNIT)
                    return true;
                if(bSetTemp[Addr]==true)
                {
                    WriteData(Addr, 114, fSetTemp[Addr]*10.0+0.5); //  +0.5 避免浮點運算造成資料不正確
                    iStartReadSetTempTask=1;
                    fReadSetTemp[Addr]=999;
                    Task=200;
                    Com2Delay.Set(2);
                    Com2Delay.On();
                    break;
                }
                Addr++;
            }
            break;
        case 200:
            if(Com2Delay.Off())
            {
                if(bUseBias[Addr] == true)        // 2011.07.18 , Joye , Add Use Bias Function
                    Task=300;
                else
                    Task=500;                       // 2011.07.18 , Joye , 跳過 117 的寫入
            }
            break;
        case 300:
            WriteData(Addr, 117, fBias[Addr]*10.0+0.5); //  +0.5 避免浮點運算造成資料不正確
            Com2Delay.Set(2);
            Com2Delay.On();
            Task=400;
            break;
        case 400:
            if(Com2Delay.Off())
                Task=500;
            break;
        case 500:
            if(ReadTargetTemp(Addr))
            {
                bSetTemp[Addr]=false;
                Task=100;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
// 開始讀取address=Index溫控器預定加熱之溫度值
//------------------------------------------------------------------------------
bool TMyYokogawa::ReadTargetTemp(int Addr)
{
    int &Task=iStartReadSetTempTask;

    int pos,P;
    AnsiString S,S1,S2;
    int attr,pH,pL,i;
    float p,Read,Temp;

    switch(Task)
    {
        case 1:
            if(bHasUnit==false)
                return false;
            Addr=0;
            Task=200;
            break;
        case 200:
            CommRetry[Addr]=_MAX_RETRY;
            Com2ReceiveOK=false;
            ReadData(Addr, 114);  // read SP1  ,預加溫之溫度,
            Com2Delay.Clear();
            Com2Delay.Set(10);
            Com2Delay.On();
            Task=300;
            break;
          case 300:
            if(Com2ReceiveOK)
            {
                S=Com2Buffer;
                S=S.SubString(2, 2);
                S=Com2Buffer;
                pos=S.Pos("OK");
                if(pos!=6) // not correct format
                {
                    Task=200;

                    //if(CommRetry[Addr])
                    if(CommRetry[Addr] > 0 && CommRetry[Addr] <= _MAX_RETRY )     // 2010.08.31 , Joye
                    {
                        CommRetry[Addr]--;
                    }
                    else
                    {
                        CommRetry[Addr]=_MAX_RETRY;
                        fReadSetTemp[Addr]=999;
                        Task=400;
                    }
                    break;
                }
                CommRetry[Addr]=_MAX_RETRY;
                S=S.SubString(pos+2, 4);
                S1=S.SubString(1, 2);
                S2=S.SubString(3, 2);
                pH=HexStrToInt(S1.c_str());
                pL=HexStrToInt(S2.c_str());
                P=pH*256+pL;
                Read=P/10.0;
                fReadSetTemp[Addr]=Read;
                Task=400;
                break;
            }

            if(Com2Delay.Off())
            {
                //if(CommRetry[Addr])
                if(CommRetry[Addr] > 0 && CommRetry[Addr] <= _MAX_RETRY )     // 2010.08.31 , Joye
                {
                    CommRetry[Addr]--;
                    Task=200;
                }
                else
                {
                    CommRetry[Addr]=_MAX_RETRY;
                    fReadSetTemp[Addr]=999;
                    Task=400;
                    break;
                }
            }
            break;
        case 400:
            return true;
    }
    return false;
}

//------------------------------------------------------------------------------
// 對溫控器群組送出 Auto Turning 動作要求
//------------------------------------------------------------------------------
bool TMyYokogawa::DoAutoTuning()
{
    int &Task=iStartAutoTuningTask;
    switch(Task)
    {
        case 1:
            Addr=0;
            Task=100;
            break;
        case 100:
            while(1)
            {
                if(Addr>=MAX_CONTROL_UNIT)
                    return true;
                if(bAutoTuningDevice[Addr]==true)
                {
                    bAutoTuningDevice[Addr]=false;
                    WriteData(Addr, 104, 1);
                    Task=200;
                    Com2Delay.Set(2);
                    Com2Delay.On();
                    break;
                }
                Addr++;
            }
            break;
        case 200:
            if(Com2Delay.Off())
                Task=100;
            break;
    }
    return false;
}

//==============================================================================
bool TMyYokogawa::DoReadPIDData()       //2011.03.10 Q_Q V208D Temp PID Add
{
  return false;
}

//==============================================================================
bool TMyYokogawa::DoWritePIDData()       //2011.03.10 Q_Q V208D Temp PID Add
{
  return false;
}