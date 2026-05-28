#include "MachineDefine.h"
#pragma hdrstop

#include "ATC_WinWay.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//class ATC_WinWay
//---------------------------------------------------------------------------
// 2016.10.18 , Joye , AMD-M Winway ATC
unsigned int ATC_WinWay::GETCRC16(unsigned char *string, unsigned char length)
{
    unsigned char Bitloop;
    unsigned int CRC_Data=0xffff;
    while(length--)
    {
        CRC_Data^=*string++;
        for(Bitloop=0; Bitloop<8; Bitloop++)
        {
            if(CRC_Data & 0x01)
            {
                CRC_Data=(CRC_Data>>1)^0xa001;
            }
            else
            {
                CRC_Data=CRC_Data>>1;
            }
        }
    }
    return(CRC_Data);
}
//---------------------------------------------------------------------------
ATC_WinWay :: ATC_WinWay(TComm* Comm)
{
    WinwayCOM = Comm;
    bCommConnect = false;
    fPresentTemperature = 0.0;
    fSetTemperature = 0.0;
    iWinWaySendCount=0;
}
//---------------------------------------------------------------------------
bool ATC_WinWay :: OpenCommPort()
{
    if( bCommConnect == true )return true;
    try
    {
        HANDLE handle = CreateFile( WinwayCOM->CommName.c_str() ,
                                    GENERIC_READ | GENERIC_WRITE ,
                                    0 ,
                                    NULL ,
                                    OPEN_EXISTING ,
                                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED ,
                                    0 );
        if( handle == INVALID_HANDLE_VALUE )
        {
            return false;
        }
        CloseHandle( handle );
        WinwayCOM->StartComm();
        bCommConnect = true;
    }
    catch(...)
    {
        bCommConnect = false;
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool ATC_WinWay :: CloseCommPort()
{
    try
    {
        WinwayCOM->StopComm();
        bCommConnect = false;
    }
    catch(...)
    {
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
double  ATC_WinWay :: GetPT()
{
    if(!bCommConnect) return 0;
    DWord data;
    char str[8]={'\0'};
    str[0]=0x01;
    str[1]=0x03; //function code
    str[2]=0x00;
    str[3]=0x00;
    str[4]=0x00;
    str[5]=0x01;
    data=GETCRC16(&str[0], 6);
    str[6]=data & 0xff;
    str[7]=(data & 0xff00)>>8;
    WinwayCOM->WriteCommData(str,8);
    iWinWaySendCount++;
    //release
//    delete [] str;
    ///*************************///
    return fPresentTemperature;
}
//---------------------------------------------------------------------------
double  ATC_WinWay :: GetPT_NoCommand()
{
    if(!bCommConnect) return 0;
    return fPresentTemperature;
}
//---------------------------------------------------------------------------
void    ATC_WinWay :: SetPT(double _temp)
{
    fPresentTemperature = _temp;
}
//---------------------------------------------------------------------------
void    ATC_WinWay :: SetPT(char* _temp)
{
    if(_temp[1]!=0x03)return;
    int _tmp = (byte)_temp[3] * 255 +  (byte)_temp[4];
    iWinWaySendCount=0;
    fPresentTemperature = (double)_tmp*0.1;
}
//---------------------------------------------------------------------------
double  ATC_WinWay :: GetST()
{
    ///*************************///
    return fSetTemperature;
}
//---------------------------------------------------------------------------
void    ATC_WinWay :: SetST(double _temp)
{
    fSetTemperature = _temp;
    if(!bCommConnect) return ;

    if(fSetTemperature==0)
        return;
    ///*************************///
    int data;
    char str[8]={'\0'};
    int Tempdata=_temp*10;
    str[0]=0x01;
    str[1]=0x06; //function code
    str[2]=0x01;
    str[3]=0x2C;
    str[4]=(Tempdata & 0xff00)>>8;
    str[5]=Tempdata & 0xff;
    data=GETCRC16(&str[0], 6);
    str[6]=data & 0xff;
    str[7]=(data & 0xff00)>>8;
    WinwayCOM->WriteCommData(str,8);
    //release
//    delete [] str;
}
//---------------------------------------------------------------------------
void ATC_WinWay :: MySleep(DWORD dwMilliseconds)
{
    Sleep(dwMilliseconds);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
