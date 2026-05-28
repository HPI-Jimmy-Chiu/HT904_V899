//==============================================================================
//
//   PANASONIC  KT4H 系列
//
//==============================================================================
#include <vcl.h>
#include <stdio.h>
#include <Filectrl.hpp>
#pragma hdrstop
#include "MyTempture_KT4H.h"
//#include "cpublic.h"
//#include "cmydef.h"

#define KT4H_CR        13
#define KT4H_LF        10

const unsigned char T_HEX2ASCII[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
const unsigned char T_ASXII2HEX[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};
//------------------------------------------------------------------------------
unsigned char T_HEX2ASCII_Mac(unsigned char hex2ascii)
{
    return(T_HEX2ASCII[(hex2ascii)&0x0f]);
}
//------------------------------------------------------------------------------
unsigned char T_ASXII2HEX_Mac(unsigned char ascii2hex)
{
    return(T_ASXII2HEX[ascii2hex-'0']);
}
//------------------------------------------------------------------------------
// create check sum code
//------------------------------------------------------------------------------
unsigned char TMyPanasonic::Create_LCR(unsigned char *Sptr, unsigned char length)
{
    unsigned char Btmp,Btmp1;
    Btmp1=0;
    do
    {
        Btmp=T_ASXII2HEX_Mac(*Sptr);           //CONV 0-9 A-F
        Sptr++;
        Btmp=(Btmp<<4)|T_ASXII2HEX_Mac(*Sptr); //CONV 0-9 A-F
        Btmp1+=Btmp;    //MATH LRC
        Sptr++;
        --length;
    }
    while(--length);    //check EVEN
    return ((~Btmp1)+1);//LRC CODE
}
//------------------------------------------------------------------------------
// 對address=Index溫控器送出欲讀取之 Registor
//------------------------------------------------------------------------------
void TMyPanasonic::ReadData(int Addr, int Command)
{
    int iFunctionCode=3;
    unsigned char Btmp1;
    sprintf(SendBuffer, ":%02X%02d%04X000100%c%c", Addr+1, iFunctionCode, Command, KT4H_CR, KT4H_LF);
    Btmp1=Create_LCR(&SendBuffer[1], 12);
    SendBuffer[13]=T_HEX2ASCII_Mac(Btmp1>>4);   //Set return number by byte
    SendBuffer[14]=T_HEX2ASCII_Mac(Btmp1);
    Comm2->WriteCommData(SendBuffer, strlen(SendBuffer));
}
//------------------------------------------------------------------------------
// 對address=Index溫控器送出欲設定之 Registor
//------------------------------------------------------------------------------
void TMyPanasonic::WriteData(int Addr, int Command, short Value)
{
    unsigned char Btmp1;
    AnsiString tmp1;
    int iFunctionCode=6;
    char str[256];

    if(Value>=0)
    {
        tmp1=IntToHex(Value, 4);
        sprintf(SendBuffer, ":%02X%02d%04X%4s00%c%c", Addr+1, iFunctionCode, Command, tmp1, KT4H_CR, KT4H_LF);
    }
    else
    {
         sprintf(str,"%04X",Value);
         tmp1=str;
         tmp1=tmp1.SubString(5, 4);
         sprintf(SendBuffer, ":%02X%02d%04X%4s00%c%c", Addr+1, iFunctionCode, Command, tmp1.c_str(), KT4H_CR, KT4H_LF);
    }

    Btmp1=Create_LCR(&SendBuffer[1], 12);
    SendBuffer[13]=T_HEX2ASCII_Mac(Btmp1>>4);   //Set return number by byte
    SendBuffer[14]=T_HEX2ASCII_Mac(Btmp1);
    Comm2->WriteCommData(SendBuffer, strlen(SendBuffer));
}
#define ERROR_CODE      1
#define OK_CODE         0
//------------------------------------------------------------------------------
// check check-sum
//------------------------------------------------------------------------------
unsigned char TMyPanasonic::Check_LRC(void)
{
    unsigned char Btmp, Btmp1;
    char str[]={"0x00"};
    str[2]=Com2Buffer[11];
    str[3]=Com2Buffer[12];
    Btmp1=StrToInt(str);
    Btmp=Create_LCR(&Com2Buffer[1],10);
    if(Btmp != Btmp1)
        return(ERROR_CODE);
    return(OK_CODE);
}
//------------------------------------------------------------------------------
// 掃描所有溫控器目前溫度值 並儲存於 fTemp[] 中
//------------------------------------------------------------------------------
bool TMyPanasonic::DoReadCurrentTemp()
{
    int &Task=iReadCurrentTempTask;
    int i;
    AnsiString S;

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
            ReadData(Addr, 0x0080);
            Com2Delay.Clear();
            Com2Delay.Set(10);
            Com2Delay.On();
            Task=300;
            break;
          case 300:
            if(Com2ReceiveOK)
            {
                if(Check_LRC()==ERROR_CODE) //check LRC CODE
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
                        fTemp[Addr]=999;
                        RecordTempture(Addr, 999);
                        Task=400;
                    }
                    break;
                }
                if(Com2Buffer[3]!='0' || Com2Buffer[4]!='3')
                {
                    Task=400;
                    break;
                }
                S=Com2Buffer;
                S=S.SubString(8, 4);
                RecordTempture(Addr, HexStrToInt(S.c_str())/10.0);
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
bool TMyPanasonic::DoStartSetTemp()
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
                    WriteData(Addr, 0x0001, fSetTemp[Addr]*10.0+0.5); //  +0.5 避免浮點運算造成資料不正確
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
                    Task=500;                       // 2011.07.18 , Joye , 跳過 0x0015 的寫入
            }
            break;
        case 300:
            WriteData(Addr, 0x0015, fBias[Addr]*10.0+0.5); //  +0.5 避免浮點運算造成資料不正確
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
bool TMyPanasonic::ReadTargetTemp(int Addr)
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
            Task=200;
            break;
        case 200:
            CommRetry[Addr]=_MAX_RETRY;
            Com2ReceiveOK=false;
            ReadData(Addr, 1);  // read SP1  ,預加溫之溫度,
            Com2Delay.Clear();
            Com2Delay.Set(10);
            Com2Delay.On();
            Task=300;
            break;
          case 300:
            if(Com2ReceiveOK)
            {
                if(Check_LRC()==ERROR_CODE) //check LRC CODE
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
                if(Com2Buffer[3]!='0' || Com2Buffer[4]!='3')
                {
                    Task=400;
                    break;
                }
                S=Com2Buffer;
                S=S.SubString(8, 4);
                fReadSetTemp[Addr]=HexStrToInt(S.c_str())/10.0;
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
bool TMyPanasonic::DoAutoTuning()
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
                    WriteData(Addr, 0x0003, 1);
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
bool TMyPanasonic::DoReadPIDData()       //2011.03.10 Q_Q V208D Temp PID Add
{
    return false;
}
bool TMyPanasonic::DoWritePIDData()       //2011.03.10 Q_Q V208D Temp PID Add
{
    return false;
}