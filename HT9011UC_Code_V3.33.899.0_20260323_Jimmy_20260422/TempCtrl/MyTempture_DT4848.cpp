//==============================================================================
//
//   台達電
//   Delta DT4848 系列
//
// AT時，不可以對溫控器下Command!!
//==============================================================================
#include <vcl.h>
#include <stdio.h>
#include <Filectrl.hpp>
#pragma hdrstop
#include "MyTempture_DT4848.h"
//#include "cpublic.h"
//#include "cmydef.h"
//------------------------------------------------------------------------------
// 對address=Index溫控器送出欲讀取之 Registor
//------------------------------------------------------------------------------
void TMyDT4848::ReadData(int Addr, int Command)
{
    Addr++;     //要從1開始
    sprintf(SendBuffer, ":%02X03%04d0002", Addr, Command);
    AnsiString LRC=DT4848LRC(SendBuffer);
    sprintf(SendBuffer, ":%02X03%04d0002%s\r\n", Addr, Command, LRC); //Polling 現在溫度
    Comm2->WriteCommData(SendBuffer, strlen(SendBuffer));
}
//------------------------------------------------------------------------------
// 對address=Index溫控器送出欲設定之 Registor
//------------------------------------------------------------------------------
void TMyDT4848::WriteData(int Addr, int Command, short Value)
{
    Addr++;     //要從1開始
    sprintf(SendBuffer, ":%02X06%04d%s", Addr, Command, IntToHex(Value, 4));
    AnsiString LRC=DT4848LRC(SendBuffer);
    sprintf(SendBuffer, ":%02X06%04d%s%s\r\n", Addr, Command, IntToHex(Value, 4), LRC);
    Comm2->WriteCommData(SendBuffer, strlen(SendBuffer));
}
//------------------------------------------------------------------------------
// 掃描所有溫控器目前溫度值 並儲存於 fTemp[] 中
//------------------------------------------------------------------------------
bool TMyDT4848::DoReadCurrentTemp()
{
    int &Task=iReadCurrentTempTask;
    int i;
    AnsiString S;
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
            ReadData(Addr, 4700);   //4700H 讀取PV溫度
            Com2Delay.Clear();
            Com2Delay.Set(10);
            Com2Delay.On();
            Task=300;
            break;
          case 300:
            if(Com2ReceiveOK)
            {
                S=Com2Buffer;
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
bool TMyDT4848::DoStartSetTemp()
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
                    WriteData(Addr, 4701, fSetTemp[Addr]*10.0+0.5); //4701H，設定SV  +0.5 避免浮點運算造成資料不正確
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
                    Task=500;                       // 2011.07.18 , Joye , 跳過 4714H 的寫入
            }
            break;
        case 300:
            WriteData(Addr, 4714, fBias[Addr]*10.0+0.5); //4714H，溫度誤差  +0.5 避免浮點運算造成資料不正確
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
bool TMyDT4848::ReadTargetTemp(int Addr)
{
    int &Task=iStartReadSetTempTask;
    int i;
    AnsiString S;

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
            ReadData(Addr, 4701);  //4701H，讀取PV值
            Com2Delay.Clear();
            Com2Delay.Set(10);
            Com2Delay.On();
            Task=300;
            break;
        case 300:
            if(Com2ReceiveOK)
            {
                S=Com2Buffer;
                S=S.SubString(8, 4);    //8~11位元為溫度值
                fReadSetTemp[Addr]=(HexStrToInt(S.c_str()))/10.0f;
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
bool TMyDT4848::DoAutoTuning()
{
    AnsiString S;
    int &Task=iStartAutoTuningTask;
    static bool bStillAutoTuning=false;

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
                {
                    Task=400;
                    break;
                    //return true;
                }
                if(bAutoTuningDevice[Addr]==true)
                {
                    bAutoTuningDevice[Addr]=false;
                    WriteData(Addr, 4729, 1);   //4729H，AT
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
                Task=300;
                CommRetry[Addr]=_MAX_RETRY;
            }
            break;
        //下面要寫入自動判斷是不是已經完成AT
        case 300:   //檢查AT=1
            Com2ReceiveOK=false;
            ReadData(Addr, 4729);  //4729H，讀取AT值
            Com2Delay.Clear();
            Com2Delay.Set(10);
            Com2Delay.On();
            Task=310;
            break;
        case 310:
            if(Com2ReceiveOK)
            {
                S=Com2Buffer;
                S=S.SubString(8, 4);
                if(S=="0001")  //已經開啟AT
                    bAutoTuningDevice[Addr]=false;
                else        //未開始AT
                    bAutoTuningDevice[Addr]=true;
                Task=100;
            }
            break;
        case 400:   //檢查AT=1
            Addr=0;
            bStillAutoTuning=true;
            Task=410;
            break;
        case 410:
            while(1)
            {
                if(Addr>=MAX_CONTROL_UNIT)  //全部都讀取完成後,就離開
                {
                    bStopAutoTuning=true;
                    return true;
                }
                if(bStillAutoTuning && bHasUnitArray[Addr])//尚在AT的話,就繼續讀
                {
                    ReadData(Addr, 4729);
                    Task=420;
                    Com2Delay.Set(2);
                    Com2Delay.On();
                    break;
                }
                Addr++;                 //否則就去讀取下一個
                bStillAutoTuning=true;  //假設下一個還在AT
            }
            break;
        case 420:
            if(Com2ReceiveOK)
            {
                S=Com2Buffer;
                S=S.SubString(8, 4);
                if(S=="0001")  //尚在AT
                    bStillAutoTuning=true;
                else           //已經完成AT
                    bStillAutoTuning=false;
                Task=410;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
// 計算LRC (檢查碼)
//------------------------------------------------------------------------------
AnsiString TMyDT4848::DT4848LRC(AnsiString str)
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
//==============================================================================
bool TMyDT4848::DoReadPIDData()      //2011.03.10 Q_Q V208D Temp PID Add
{
    int &Task=iStartReadPIDDataTask;
    int i;
    AnsiString S;
    AnsiString PBuffer;
    AnsiString IBuffer;
    AnsiString DBuffer;

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
            ReadPIDData(Addr, 4708);
            Com2Delay.Clear();
            Com2Delay.Set(10);
            Com2Delay.On();
            Task=300;
            break;
        case 300:
            if(Com2ReceiveOK)
            {
                S=Com2Buffer;
                S=S.SubString(8, 12);                                           //8~20位元為PID值
                PBuffer=S.SubString(1,4);
                IBuffer=S.SubString(5,4);
                DBuffer=S.SubString(9,4);
                fReadSetPID[Addr][0]=HexStrToInt(PBuffer.c_str());
                fReadSetPID[Addr][1]=HexStrToInt(IBuffer.c_str());
                fReadSetPID[Addr][2]=HexStrToInt(DBuffer.c_str());
                Task=400;
                break;
            }
            if(Com2Delay.Off())
            {
                if(CommRetry[Addr] > 0 && CommRetry[Addr] <= _MAX_RETRY )
                {
                    CommRetry[Addr]--;
                    Task=200;
                }
                else
                {
                    CommRetry[Addr]=_MAX_RETRY;
                    fReadSetPID[Addr][0]=0;
                    fReadSetPID[Addr][1]=0;
                    fReadSetPID[Addr][2]=0;
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
bool TMyDT4848::DoWritePIDData()       //2011.03.10 Q_Q V208D Temp PID Add
{
    int i;
    int &Task=iStartWritePIDDataTask;
    static bool bflag[3]={false,false,false};
    AnsiString S;
    AnsiString PBuffer;
    AnsiString IBuffer;
    AnsiString DBuffer;
    static int Addr=0;

    switch(Task)
    {
        case 1:
            Addr=0;
            bflag[0]=false;
            bflag[1]=false;
            bflag[2]=false;
            Task=100;
            break;
        case 100:
            while(1)
            {
                if(Addr>=MAX_CONTROL_UNIT)
                    return true;

                if(bSetPID[Addr]==true)
                {
                    if(bflag[0]==false)
                    {
                        WritePIDData(Addr, "4708",iSetPID[Addr][0]);
                        Task=150;
                        Com2Delay.Set(2);
                        Com2Delay.On();
                        break;
                    }
                    if(bflag[1]==false)
                    {
                        WritePIDData(Addr, "4709",iSetPID[Addr][1]);
                        Task=150;
                        Com2Delay.Set(2);
                        Com2Delay.On();
                        break;
                    }
                    if(bflag[2]==false)
                    {
                        WritePIDData(Addr, "470A",iSetPID[Addr][2]);
                        Task=150;
                        Com2Delay.Set(2);
                        Com2Delay.On();
                        break;
                    }

                    if(bflag[0] &&  bflag[1] && bflag[2])
                    {
                        Task=200;
                        break;
                    }
                }
                else
                {
                    Task=200;
                    break;
                }
            }
            break;
        case 150:
            if(Com2Delay.Off())
            {
                Com2ReceiveOK=false;
                ReadPIDData(Addr, 4708);
                Com2Delay.Clear();
                Com2Delay.Set(5);
                Com2Delay.On();
                Task=160;
            }
            break;
        case 160:
            if(Com2ReceiveOK)
            {
                S=Com2Buffer;
                S=S.SubString(8, 12);                                           //8~20位元為PID值
                PBuffer=S.SubString(1,4);
                IBuffer=S.SubString(5,4);
                DBuffer=S.SubString(9,4);
                fReadSetPID[Addr][0]=HexStrToInt(PBuffer.c_str());
                fReadSetPID[Addr][1]=HexStrToInt(IBuffer.c_str());
                fReadSetPID[Addr][2]=HexStrToInt(DBuffer.c_str());
                Task=170;
                break;
            }
            if(Com2Delay.Off())
            {
                if(CommRetry[Addr] > 0 && CommRetry[Addr] <= _MAX_RETRY )
                {
                    CommRetry[Addr]--;
                    Task=150;
                    break;
                }
                else
                {
                    CommRetry[Addr]=_MAX_RETRY;
                    fReadSetPID[Addr][0]=0;
                    fReadSetPID[Addr][1]=0;
                    fReadSetPID[Addr][2]=0;
                    Addr++;
                    Task=100;
                    break;
                }
            }
            break;
        case 170:                   //check
            for(int i=0; i<3; i++)
            {
                if(fReadSetPID[Addr][i] == iSetPID[Addr][i])
                {
                    bflag[i]=true;
                }
            }
            Task=100;
            break;
        case 200:
            bflag[0]=false;
            bflag[1]=false;
            bflag[2]=false;
            bSetPID[Addr]=false;
            Addr++;
            Task=100;
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
void TMyDT4848::ReadPIDData(int Addr, int Command)     //2011.03.10 Q_Q V208D Temp PID Add
{
    Addr++;     //要從1開始
    sprintf(SendBuffer, ":%02x03%04d0003", Addr, Command);
    AnsiString LRC=DT4848LRC(SendBuffer);
    sprintf(SendBuffer, ":%02d03%04d0003%s\r\n", Addr, Command, LRC); //Polling 現在溫度
    Comm2->WriteCommData(SendBuffer, strlen(SendBuffer));
}
//------------------------------------------------------------------------------
void TMyDT4848::WritePIDData(int Addr, AnsiString Command, short Value)         //2011.03.10 Q_Q V208D Temp PID Add
{
    Addr++;     //要從1開始
    sprintf(SendBuffer, ":%02x06%04s%s", Addr, Command, IntToHex(Value, 4));
    AnsiString LRC=DT4848LRC(SendBuffer);
    sprintf(SendBuffer, ":%02d06%04s%s%s\r\n", Addr, Command, IntToHex(Value, 4), LRC);
    Comm2->WriteCommData(SendBuffer, strlen(SendBuffer));
}
//------------------------------------------------------------------------------