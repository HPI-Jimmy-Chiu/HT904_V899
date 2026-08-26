#include "MachineDefine.h"
#pragma hdrstop

#include "myio.h"
#include "cmydef.h"
//#include "cmydef.h"
#include "MyPLC_IO_Modbus.h"
#include "ModbusTCPClient.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

extern TPLCIOInfo g_tPLCIOInfo;

unsigned int MyLongMask[32]=
    {0x00000001,0x00000002,0x00000004,0x00000008,0x00000010,0x00000020,0x00000040,0x00000080,
     0x00000100,0x00000200,0x00000400,0x00000800,0x00001000,0x00002000,0x00004000,0x00008000,
     0x00010000,0x00020000,0x00040000,0x00080000,0x00100000,0x00200000,0x00400000,0x00800000,
     0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000};

unsigned int lOutPortData[128];
unsigned int lInPortData[128];

int  OutPortIndex[128];
int  InPortIndex[128];
byte OutPortData[128];
byte InPortData[128];
int  OutPos=0;
int  InPos=0;
//---------------------------------------------------------------------------
//void CheckWindowsIsXP()
//{
//    AnsiString Str;
//    OSVERSIONINFO osvi;
//    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
//    osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
//    GetVersionEx(&osvi);
//    int a=osvi.dwMajorVersion;                                                  //主版本
//    int b=osvi.dwMinorVersion;                                                  //次版本
//    bIsWinXP=(a==5 && b==1);
//    if(a==4 && b==0)  Str.sprintf("Windows NT Ver:%d.%d\n", a, b);
//    if(a==4 && b==10) Str.sprintf("Windows 98 Ver:%d.%d\n", a, b);
//    if(a==4 && b==90) Str.sprintf("Windows ME Ver:%d.%d\n", a, b);
//    if(a==5 && b==0)  Str.sprintf("Windows 2000 Ver:%d.%d\n", a, b);
//    if(a==5 && b==1)  Str.sprintf("Windows XP Ver:%d.%d\n", a, b);
//    if(a==6 && b==0)  Str.sprintf("Windows Vista Ver:%d.%d\n", a, b);
//    if(a==6 && b==1)  Str.sprintf("Windows 7 Ver:%d.%d\n", a, b);
//}
//---------------------------------------------------------------------------
void InitialMyOutIOData()
{
    for(int i=0; i<128; i++)
    {
        OutPortIndex[i]=0;
        OutPortData[i]=0;
    }
}
//---------------------------------------------------------------------------
int IOSetOutport(int port)
{
    static bool flag=true;

    if(flag)
    {
        InitialMyOutIOData();
        flag=false;
    }

    if(TTL_CARD_TYPE>0)                                                         //Steven 20200901 : Win10 & Win7也可以用TTL
        return 999;

    if(port==0)
        return 999;

    for(int i=0; i<OutPos; i++)
        if(OutPortIndex[i]==port)
            return i;

    if(OutPos>=100)
        return 999;                                                             // 不能發生 !!!

    EnableNTPort(port, port);
    OutPortIndex[OutPos]=port;
    OutPos++;
    return (OutPos-1);
};
//---------------------------------------------------------------------------
bool IOSetInport(int port)
{
    if(TTL_CARD_TYPE>0)                                                         //Steven 20200901 : Win10 & Win7也可以用TTL
        return 999;

    if(port==0)
        return false;

    for(int i=0; i<InPos; i++)
        if(InPortIndex[i]==port)
            return true;

    if(InPos>=100)
        return false;                                                           // 不能發生 !!!

    EnableNTPort(port, port);
    InPortIndex[InPos]=port;
    InPos++;
    return true;
};
extern bool IdleCheckSafeDoorByCylinder(int iPort, int iBit);
//---------------------------------------------------------------------------
void IOBitOn(int port, int bit)
{
    //******************************************************************************
    //  注意!! IdleCheckSafeDoorByCylinder為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(IdleCheckSafeDoorByCylinder(port, bit)==true)                            // 2015.01.15 , Joye , Safe door check
        return;

    int ret=-1;
    AnsiString S="";
    if(TTL_CARD_TYPE>0)                                                         //Steven 20200901 : Win10 & Win7也可以用TTL
        return;

    if(port<0x210)
    {
        if((bit>=32 || bit<0 || port==0)==true)
            return;
        ret=IOSetOutport(port);
        if(ret==999 || ret<0 || ret>127)
            return;
        lOutPortData[ret]|=MyLongMask[bit];
    }
    else
    {
        if((bit>=8 || bit<0 || port==0)==true)
            return;
        ret=IOSetOutport(port);
        if(ret==999 || ret<0 || ret>127)
            return;
        OutPortData[ret]|=MyBitMask[bit];
        outportb(port,OutPortData[ret]);
    }
}
//---------------------------------------------------------------------------
void IOBitOff(int port, int bit)
{
    //******************************************************************************
    //  注意!! IdleCheckSafeDoorByCylinder為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(IdleCheckSafeDoorByCylinder(port, bit)==true)                            // 2015.01.15 , Joye , Safe door check
        return;

    int ret=-1;
    if(TTL_CARD_TYPE>0)                                                         //Steven 20200901 : Win10 & Win7也可以用TTL
        return;

    if(port<0x210)
    {
        if((bit>=32 || bit<0 || port==0)==true)
            return;
        ret=IOSetOutport(port);
        if(ret==999 || ret<0 || ret>127)
            return;
        lOutPortData[ret]&=~MyLongMask[bit];
    }
    else
    {
        if((bit>=8 || bit<0 || port==0)==true)
            return;
        ret=IOSetOutport(port);
        if(ret==999 || ret<0 || ret>127)
            return;
        OutPortData[ret]&=~MyBitMask[bit];
        outportb(port,OutPortData[ret]);
    }
}
//---------------------------------------------------------------------------
void IOByteOut(int port, unsigned int Byte)
{
    //******************************************************************************
    //  注意!! IdleCheckSafeDoorByCylinder為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(IdleCheckSafeDoorByCylinder(port, -1)==true)                             //Steven 20230703 : Add for MotionNet IO check SafeDoor
        return;

    int ret=-1;
    if(TTL_CARD_TYPE>0)
        return;

    if(port<0x210)
    {
        ret=IOSetOutport(port);
        if(ret==999 || ret<0 || ret>127)
            return;
        lOutPortData[ret]=Byte;
    }
    else
    {
        ret=IOSetOutport(port);
        if(ret==999 || ret<0 || ret>127)
            return;
        OutPortData[ret]=Byte;
        outportb(port,OutPortData[ret]);
    }
}
//---------------------------------------------------------------------------
bool IOOutBitStatus(int port,int bit)
{
    int ret=-1;

    if(TTL_CARD_TYPE>0)
        return false;

    if(port<0x210)
    {
        ret=IOSetOutport(port);
        if(ret==999 || ret<0 || ret>127)
            return false;
        if(lOutPortData[ret] & MyLongMask[bit])
            return true;
        else
            return false;
    }
    else
    {
        int ret;
        if(bit>=8 || bit<0)
            return false;
        ret=IOSetOutport(port);
        if(ret==999 || ret<0 || ret>127)
            return false;
        if(OutPortData[ret] & MyBitMask[bit])
            return true;
        else
            return false;
    }
}
//---------------------------------------------------------------------------
byte IOOutByteStatus(int port)
{
    int ret=-1;

    if(TTL_CARD_TYPE>0)
        return false;

    ret=IOSetOutport(port);
    if(ret==999 || ret<0 || ret>127)
        return false;
    return OutPortData[ret];
}
//---------------------------------------------------------------------------
bool IOInputBit(int port, int bit)
{
    if(TTL_CARD_TYPE>0)
        return false;

    WORD wInputStartRegNum = g_tPLCIOInfo.m_wInputStartRegNum;

    if(port>=0 &&                                                               //Steven 20240103 : 加入保護
       bit>=0  &&
       bPLCIO[port][bit])
    {                                                                           //Jason 20230619 增加安全PLC部分
        if(port>=wInputStartRegNum)                                             //Austin 20190218.01 增加PLC IO for CE
        {
            if(iPLCSafetyVer==202204)
            {
                return bPLCInData[0][port-wInputStartRegNum+2][bit];            //joe 20220406.01 debug safe plc
            }
            else
            {
                return bPLCInData[0][port-wInputStartRegNum+1][bit];           //第1個Byte為系統狀態 預留
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(bit>=8 || bit<0)                                                     //Steven 20160520 : TTL IO加上保護
            return false;

        if(IOSetInport(port)==false) return false;
        byte b;
        b=inportb(port);
        if(b&MyBitMask[bit])
            return true;
        else
            return false;
    }
}
//---------------------------------------------------------------------------
byte IOInputByte(int port)
{
    if(TTL_CARD_TYPE>0)                                                         //Steven 20200901 : Win10 & Win7也可以用TTL
        return false;

    if(IOSetInport(port)==false)
        return false;
    return inportb(port);
}
//---------------------------------------------------------------------------
int  BackOutPortIndex[128];
byte BackOutPortData[128];
unsigned int BackOutLongData[128];
void BackUpOutputData()
{
    for(int i=0; i<128; i++)
    {
        BackOutLongData[i]=lOutPortData[i];
        BackOutPortData[i]=OutPortData[i];
    }
}
//---------------------------------------------------------------------------
void RestoreOutputData()
{
    for(int i=0; i<OutPos; i++)
    {
        OutPortData[i]=BackOutPortData[i];
        IOByteOut(OutPortIndex[i], OutPortData[i]);
    }
}
//---------------------------------------------------------------------------
