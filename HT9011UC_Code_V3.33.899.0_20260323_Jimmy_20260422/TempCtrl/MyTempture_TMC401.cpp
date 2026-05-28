//---------------------------------------------------------------------------
//Function account : 讀取TMC401溫度
//Input : Addr :設定位址 、Channel : 哪個Channel 、SelectReadNumber : 選擇從第幾個開始讀取
//Date : 2007/08/10
//Editor : Mathew
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <Filectrl.hpp>
#pragma hdrstop
#include "MyTempture_TMC401.h"
//#include "cpublic.h"
//#include "cmydef.h"
//---------------------------------------------------------------------------
unsigned int Crc_16_create(unsigned char *string, unsigned char length)
{
    unsigned char Bitloop;
    unsigned int Crc_tmp=0xffff;

    while(length--)
    {
        Crc_tmp^=*string++;
        for(Bitloop=0; Bitloop<8; Bitloop++)
        {
            if(Crc_tmp & 0x01)
            {
                Crc_tmp=(Crc_tmp>>1)^0xa001;
            }
            else
            {
                Crc_tmp=Crc_tmp>>1;
            }
        }
    }
    return(Crc_tmp);
}
//---------------------------------------------------------------------------
const int TEMP_ERR_COUNT_ALRM=1;
const int OVER_TEMP=150;
TDateTime tReciveCOM2Time;//08.09.02.01 han test TMC401
float TMyTMC401::ProcessData()
{
    int i, iAddress, iChannel, iTempLow, iTempHigh, iReciveLow, iReciveHigh;
    AnsiString aCRC_CHeck, aCRC_Low, aCRC_High, asStr="";//08.09.02.01
    char data[1024];
    strcpy(data,Com2Buffer);
    unsigned char CRCstr[256]="";
    char str[256]="";
    float f;

    for(i=0; i<Receivelen; i++)
        CRCstr[i]=Com2ReceiveByte[i];

    i=Crc_16_create(CRCstr, Receivelen-2);

    aCRC_CHeck = IntToHex(i, 4);             //將CRC檢查值轉成16進制  Mathew 2007/08/09
    aCRC_Low   = aCRC_CHeck.SubString(3, 2); //CRC16 Low
    aCRC_High  = aCRC_CHeck.SubString(1, 2); //CRC16 High

    aCRC_Low   = "0x"+aCRC_Low;
    aCRC_High  = "0x"+aCRC_High;        //將CRC16 High值轉成字串
    iTempLow   = aCRC_Low.ToInt();      //CRC16 Low
    iTempHigh  = aCRC_High.ToInt();     //CRC16 High   自算

    iReciveLow=CRCstr[Receivelen-2];    //CRC16 Low  回傳
    iReciveHigh=CRCstr[Receivelen-1];   //CRC16 High

    if((iReciveLow==iTempLow)&&(iReciveHigh==iTempHigh))
    {
        if(CRCstr[1]==0x03)  // function code
        {
            if(CRCstr[2]==0x02)
            {
                iTempLow=StrToInt(CRCstr[4]);// aCRC_Low.ToInt(); //StrToInt(str[4]);
                iTempHigh=StrToInt(CRCstr[3]);//StrToInt(str[3]);
                i=(iTempHigh*256)+iTempLow;
                sprintf(str, "0x%02x%02x", CRCstr[3], CRCstr[4]);
                f=StrToInt(str);
                return f/10.0;
            }
        }
        else if(CRCstr[1]==0x83)
        {
            if(CRCstr[2]==0x01)
                ShowMyMessage("Error 01：指定不支援的功能碼。");
            else if(CRCstr[2]==0x02)
                ShowMyMessage("Error 02：指定不支援的地址。");
            else if(CRCstr[2]==0x03)
                ShowMyMessage("Error 03：超出一次可傳送的最大資料長度。");
            else if(CRCstr[2]==0x04)
                ShowMyMessage("Error 04：CRC16錯誤。");
        }
        else if(CRCstr[1]==0x06)   //寫溫度 Mathew 2007/08/10
        {
            iTempLow=StrToInt(CRCstr[5]);
            iTempHigh=StrToInt(CRCstr[4]);
            i=((iTempHigh*256)+iTempLow);
        }
        else if(CRCstr[1]==0x86)
        {
            if(CRCstr[2]==0x01)
                ShowMyMessage("Error 01：指定不支援的功能碼。");
            else if(CRCstr[2]==0x02)
                ShowMyMessage("Error 02：指定不支援的地址。");
            else if(CRCstr[2]==0x03)
                ShowMyMessage("Error 03：超出一次可傳送的最大資料長度。");
            else if(CRCstr[2]==0x04)
                ShowMyMessage("Error 04：CRC16錯誤。");
        }
    }
    ///else
    ///{
        return 0;
    ///}
}
//------------------------------------------------------------------------------
// 對address=Index溫控器送出欲讀取之 Registor
//------------------------------------------------------------------------------
void TMyTMC401::ReadData(int Addr,int Command)
{
    char data[8];
    short *p;
    data[0]=Addr/4;         // controller index
    data[1]=0x03;           // function code ,read multi register
    Command+=Addr%4;
    p=(short*)&data[2];
    *p=Command;
    data[4]=0x00;
    data[5]=1;              //要連續讀出的數目
    p=(short*)&data[6];
    *p=Crc_16_create(data,6);  //CRC檢查  Mathew 2007/08/09
    Comm2->WriteCommData(data, 8);
}
//------------------------------------------------------------------------------
// 對address=Index溫控器送出欲設定之 Registor
//------------------------------------------------------------------------------
void TMyTMC401::WriteData(int Addr, int Command, short Value)
{
    int i, iTemp[2], iWriteTemp, iDataChannel, iDataHigh, iDataLow, iChannel;
    float fTemp;
    char data[8]="";
    unsigned long len=8;
    int iChannelTemp;
    iChannelTemp=Addr%4;
    Addr/=4;
    AnsiString aCRC_CHeck, aCRC_Low, aCRC_High;

    fTemp=Value*10;             // 先用float存再轉到int避掉小數點後方好幾位數的亂碼.
    iWriteTemp=fTemp;                       // x10是因為寫入是以0.1為單位.

    iDataChannel  =   0xC8+iChannelTemp;    //0xC8+iChannel;  Mathew 08.09.09.01
    iDataHigh     =   iWriteTemp/256;
    iDataLow      =   iWriteTemp%256;

    data[0]=Addr;  //Address.
    data[1]=0x06;
    data[2]=0x00;
    data[3]=iDataChannel;   //Select Channel.
    data[4]=iDataHigh;      //寫入的溫度 High.
    data[5]=iDataLow;       //寫入的溫度 Low.

    i=Crc_16_create(data,6);    //CRC檢查  Mathew 2007/08/10.

    aCRC_CHeck  =   IntToHex(i,4);  //將CRC檢查值轉成16進制.
    aCRC_Low    =   aCRC_CHeck.SubString(3, 2);
    aCRC_High   =   aCRC_CHeck.SubString(1, 2);

    aCRC_Low="0x"+aCRC_Low;
    aCRC_High="0x"+aCRC_High;  //將CRC16 High值轉成字串.
    iTemp[0]=aCRC_Low.ToInt(); //將CRC16 High值轉成整數.
    iTemp[1]=aCRC_High.ToInt();

    data[6]=iTemp[0];
    data[7]=iTemp[1];

    Comm2->WriteCommData(data, len);
}
#define ERROR_CODE      1
#define OK_CODE         0
//------------------------------------------------------------------------------
// 掃描所有溫控器目前溫度值 並儲存於 fTemp[] 中
//------------------------------------------------------------------------------
bool TMyTMC401::DoReadCurrentTemp()
{
    int &Task=iReadCurrentTempTask;

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
            ReadData(Addr, 0x0000);
            Com2Delay.Clear();
            Com2Delay.Set(10);
            Com2Delay.On();
            Task=300;
            break;
          case 300:
            if(Com2ReceiveOK)
            {
                RecordTempture(Addr, ProcessData());
                Task=400;
                break;
            }
            if(Com2Delay.Off())
            {
                //if(CommRetry[Addr])
                if(CommRetry[Addr] > 0 && CommRetry[Addr] <= _MAX_RETRY )     // 2010.08.31 , Joye
                {
                    CommRetry[Addr]--;
                    Comm2->StopComm();
                    Task=350;
                }
                else
                {
                    CommRetry[Addr]=_MAX_RETRY;
                    RecordTempture(Addr, 999);
                    Task=600;
                    break;
                }
            }
            break;
        case 350:
            Comm2->StartComm();
            Task=200;
            break;
        case 400:
            Com2Delay.Clear();
            Com2Delay.Set(1);
            Com2Delay.On();
            Task=500;
            break;
        case 500:
            if(Com2Delay.Off())
                Task=600;
            break;
        case 600:
            Addr++;
            Task=100;
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
// 對溫控器群組送出欲設定之溫度值
//------------------------------------------------------------------------------
bool TMyTMC401::DoStartSetTemp()
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
bool TMyTMC401::ReadTargetTemp(int Addr)
{
    int &Task=iStartReadSetTempTask;
    int pos,P;
    AnsiString S, S1, S2;
    int pH, pL, i;
    float p, Read, Temp;

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
                /*
                if(Com2Buffer[3]!='0' || Com2Buffer[4]!='3' )
                {
                    Task=400;
                    break;
                }
                S=Com2Buffer;
                S=S.SubString(8,4);
                S=AnsiString("0x")+S;
                fReadSetTemp[Addr]=StrToInt(S)/10.0;
                Task=400;
                break;
                */
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
bool TMyTMC401::DoAutoTuning()
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

bool TMyTMC401::DoReadPIDData()       //2011.03.10 Q_Q V208D Temp PID Add
{
  return false;
}
bool TMyTMC401::DoWritePIDData()       //2011.03.10 Q_Q V208D Temp PID Add
{
  return false;
}
