// =============================================================================
//  ATC/ATC_WinWay.cpp  --  AMD-M Winway ATC (temperature controller) Modbus-RTU
//                          driver over a serial TComm port.
//
//  Faithful translation of golden ATC/ATC_WinWay.cpp (161 lines, BCB6,
//  Big5/cp950 -- this file itself is pure ASCII, no Chinese comments).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2, group "atc". Sibling of ATC/TCPData.{h,cpp},
//  landed in the SAME wave.
//
//  ROLE: see ATC_WinWay.h's own banner. This TU defines all 10 golden methods.
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim bodies): all 10 golden function definitions,
//     whole file --
//       GETCRC16              golden :11-31    ATC_WinWay ctor    golden :33-40
//       OpenCommPort           golden :42-68    CloseCommPort      golden :70-82
//       GetPT                  golden :84-104   GetPT_NoCommand    golden :106-110
//       SetPT(double)          golden :112-115  SetPT(char*)       golden :117-123
//       GetST                  golden :125-129  SetST(double)      golden :131-154
//       MySleep(DWORD)         golden :156-159
//   SATISFIED-BY-SUBSTRATE (already real in this tree, nothing shimmed here):
//     Spcomm::TComm::CommName/StartComm/StopComm/WriteCommData -- all real,
//     vclcompat/Comm.h (already part of the vcl_compat.h umbrella). CreateFile
//     /CloseHandle/HANDLE/GENERIC_READ/GENERIC_WRITE/OPEN_EXISTING/
//     FILE_ATTRIBUTE_NORMAL/FILE_FLAG_OVERLAPPED/INVALID_HANDLE_VALUE/Sleep --
//     <windows.h> via vcl_compat.h (same "does NOT set WIN32_LEAN_AND_MEAN"
//     precedent EJ1N/TextProcess.cpp's banner already documents for the
//     identical CreateFile/CloseHandle pair).
//   GATE (#if 0): NONE.
//
//  VCL/Borland conversions: `#pragma hdrstop` dropped (golden :2). Golden's
//  `#include "MachineDefine.h"` kept (brings vcl_compat.h + windows.h). `#pragma
//  package(smart_init)` dropped (golden :161). No __fastcall / __property in
//  this file (golden has none either). `ATC_WinWay :: ATC_WinWay(...)` /
//  `bool ATC_WinWay :: OpenCommPort()` etc. -- golden's own spaced `::`
//  qualifier style is kept verbatim (purely cosmetic, not a Borland-specific
//  construct; standard C++ accepts the whitespace either way).
//
//  MINIMAL NECESSARY ADAPTATIONS (2, both flagged inline with //AI at the line):
//   (a) golden :87 `DWord data;` -- see ATC_WinWay.h's own banner for the full
//       grep-and-rationale writeup. The identifier `DWord` (this exact
//       casing) is undefined anywhere in this repo snapshot; a local
//       `typedef unsigned int DWord;` was added to this unit's own header so
//       the golden call site here compiles UNCHANGED.
//   (b) golden :95, :148 `GETCRC16(&str[0], 6)` -- `str` is `char[8]`,
//       GETCRC16 takes `unsigned char*`. BCB6 accepts the implicit char*<->
//       unsigned char* pointer conversion; ISO C++ does not (verified: g++
//       rejects it outright, "invalid conversion from 'char*' to 'unsigned
//       char*' [-fpermissive]", not merely a warning). Both call sites gain
//       an explicit `(unsigned char*)` cast -- same bytes, same address,
//       zero behaviour delta, purely a strict-typing satisfier (same class of
//       fix as EJ1N/TextProcess.cpp's `const char *ASCII` adaptation).
//
//  Numeric semantics kept EXACT: `int _tmp = (byte)_temp[3] * 255 +
//  (byte)_temp[4];` (golden :120) multiplies by 255, NOT 256/0x100 -- this
//  looks like an off-by-one against a standard big-endian 16-bit reassembly,
//  but it is golden's own arithmetic and is NOT "corrected" here. `byte`
//  (lower-case) resolves via windows.h -> rpcndr.h, the SAME transitive
//  source already relied on by cmydef.h:127 / common.cpp:369 / EJ1N/
//  TextProcess.h's own banner note -- no new shim needed for it.
// =============================================================================
#include "MachineDefine.h"

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
    data=GETCRC16((unsigned char*)&str[0], 6);   //AI(W906-PT-W2) 20260807: explicit cast -- see MINIMAL NECESSARY ADAPTATION in banner (char*->unsigned char* is implicit in BCB6, ill-formed under strict g++)
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
    data=GETCRC16((unsigned char*)&str[0], 6);   //AI(W906-PT-W2) 20260807: explicit cast -- see MINIMAL NECESSARY ADAPTATION in banner (char*->unsigned char* is implicit in BCB6, ill-formed under strict g++)
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
