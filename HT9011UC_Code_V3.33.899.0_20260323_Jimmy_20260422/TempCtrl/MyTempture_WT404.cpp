//==============================================================================
//
//   士林電機
//   Shihlin  WT404 系列
//
//==============================================================================
#include <vcl.h>
#include <stdio.h>
#include <Filectrl.hpp>
#pragma hdrstop
#include "MyTempture_WT404.h"
//#include "cpublic.h"
//#include "cmydef.h"

TMyWT404::TMyWT404()
{
    ComParity = Odd;
}
//------------------------------------------------------------------------------
// 對address=Index溫控器送出欲讀取之 Registor
//------------------------------------------------------------------------------
void TMyWT404::ReadData(int Addr, int Command)
{
    Addr++;     //要從1開始
    sprintf(SendBuffer, ":%02d03%04s0001", Addr, IntToHex(Command, 4));
    AnsiString LRC=WT404LRC(SendBuffer);
    sprintf(SendBuffer, ":%02d03%04s0001%s\r\n", Addr, IntToHex(Command, 4), LRC);
    Comm2->WriteCommData(SendBuffer, strlen(SendBuffer));
}
//------------------------------------------------------------------------------
// 對address=Index溫控器送出欲設定之 Registor
//------------------------------------------------------------------------------
void TMyWT404::WriteData(int Addr, int Command, short Value)
{
    Addr++;     //要從1開始
    sprintf(SendBuffer, ":%02d06%04s%s", Addr, IntToHex(Command, 4), IntToHex(Value, 4));
    AnsiString LRC=WT404LRC(SendBuffer);
    sprintf(SendBuffer, ":%02d06%04s%s%s\r\n", Addr, IntToHex(Command, 4), IntToHex(Value, 4), LRC);
    Comm2->WriteCommData(SendBuffer, strlen(SendBuffer));
}
//------------------------------------------------------------------------------
// 掃描所有溫控器目前溫度值 並儲存於 fTemp[] 中
//------------------------------------------------------------------------------
bool TMyWT404::DoReadCurrentTemp()
{
    int &Task=iReadCurrentTempTask;
    AnsiString S;
    int i;
    float Read;

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
            ReadData(Addr, 138);    //138=008A(H)，讀取PV溫度
            Com2Delay.Clear();
            Com2Delay.Set(10);
            Com2Delay.On();
            Task=300;
            break;
        case 300:
            if(Com2ReceiveOK)
            {
                S=Com2Buffer;
                if(S.Length()<11)  //資料異常時，只會傳回10個位元
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
                    }
                    break;
                }

                S=S.SubString(8, 4);    //8~11位元為溫度值
                Read=(HexStrToInt(S.c_str()))/10.0f;

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
bool TMyWT404::DoStartSetTemp()
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
                    WriteData(Addr, 0, fSetTemp[Addr]*10.0+0.5); //0=0000(H)，設定SV值  +0.5 避免浮點運算造成資料不正確
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
                Task=300;
            break;
        case 300:
            WriteData(Addr, 100, fBias[Addr]*10.0+0.5); //100=0064(H)，設定SV補償值  +0.5 避免浮點運算造成資料不正確
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
bool TMyWT404::ReadTargetTemp(int Addr)
{
    int &Task=iStartReadSetTempTask;

    int pos,P;
    AnsiString S, S1, S2;
    int attr, pH, pL, i;
    float p, Read, Temp;

    switch(Task)
    {
        case 1:
            if(bHasUnit==false)
                return false;
            Addr=0;
            CommRetry[Addr]=_MAX_RETRY;
            Task=200;
            break;
        case 200:
            Com2ReceiveOK=false;
            ReadData(Addr, 0);  //0=0000(H)，讀取PV溫度,
            Com2Delay.Clear();
            Com2Delay.Set(10);
            Com2Delay.On();
            Task=300;
            break;
        case 300:
            if(Com2ReceiveOK)
            {
                S=Com2Buffer;
                if(S.Length()<11)  //資料異常時，只會傳回10個位元
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
                    }
                    break;
                }

                S=S.SubString(8, 4);
                Read=(HexStrToInt(S.c_str()))/10.0f;

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
bool TMyWT404::DoAutoTuning()
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
                    WriteData(Addr, 2, 1);   //2=0002(H)，AT
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
//------------------------------------------------------------------------------
// 計算LRC (檢查碼)
//------------------------------------------------------------------------------
AnsiString TMyWT404::WT404LRC(AnsiString str)
{
    AnsiString str2="";
    str=str.SubString(2, 12);   //移除第一個:
    int LRC=0;
    for(int i=1; i<str.Length(); i+=2)
    {
        str2=str.SubString(i, 2);           //取得兩個字元
        LRC+=HexStrToInt(str2.c_str());     //計算兩個字元的16進位轉10進位
    }

    LRC=0xFF-LRC+1;                         //計算2補數
    str=IntToHex(LRC, 2);
    str=str.SubString(str.Length()-1, 2) ;
    return str;
}
bool TMyWT404::DoReadPIDData()        //2011.03.10 Q_Q V208D Temp PID Add
{
  return false;
}
bool TMyWT404::DoWritePIDData()       //2011.03.10 Q_Q V208D Temp PID Add
{
  return false;
}
