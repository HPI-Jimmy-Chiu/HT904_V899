// =============================================================================
//  uModbusCommand.cpp  --  Modbus RTU/TCP master frame encode/decode helper.
//
//  Faithful translation of golden EJ1N/uModbusCommand.cpp (159 lines, BCB6,
//  Big5/cp950 -- carries one Chinese comment, "新增可讀寫負數").
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 group "ej1n".  See uModbusCommand.h's own
//  banner for ROLE / the one golden dead-declaration (GetClientDecodeReadSingleRTU).
//
//  WAVE SCOPE -- ACTIVE, faithful, verbatim bodies -- ALL 13 golden function
//  definitions (golden line numbers):
//    uModbusCommand::uModbusCommand                golden :10-13
//    uModbusCommand::~uModbusCommand                golden :15-18
//    uModbusCommand::Clear                          golden :20-26
//    uModbusCommand::GetClientEncodeSingleTCP       golden :30-41
//    uModbusCommand::GetClientEncodeWriteMultipleTCP golden :43-59
//    uModbusCommand::GetClientEncodeSingleRTU       golden :63-72
//    uModbusCommand::CRC_Check                      golden :74-95
//    uModbusCommand::DecodeToListIntRTU             golden :97-111
//    uModbusCommand::DecodeToListIntTCP             golden :113-127
//    uModbusCommand::TwoBytes2Int                   golden :129-135
//    uModbusCommand::TwoBytes2Short                 golden :137-141
//    uModbusCommand::AddListValue                   golden :143-147
//    uModbusCommand::ClearListValue                 golden :149-153
//    uModbusCommand::GetDataLength                  golden :155-158
//   GetClientDecodeReadSingleRTU (golden .h :41) is NOT among these 13 --
//   golden itself never defines it either (see uModbusCommand.h banner).
//
//  GOLDEN QUIRKS / BUGS preserved VERBATIM, not fixed (per this wave's rule
//  3, "do not improve arithmetic, do not fix bugs"):
//   1. `memset(cSet,0x0,sizeof(cSet));` (golden :33,46,66 --
//      GetClientEncodeSingleTCP/GetClientEncodeWriteMultipleTCP/
//      GetClientEncodeSingleRTU). `cSet` is a `char*` PARAMETER, so
//      `sizeof(cSet)` is `sizeof(char*)` (the pointer's own size, 4 or 8
//      bytes on this build), NOT the caller's actual buffer length. Only the
//      first few bytes of the real send buffer are ever zeroed by this call;
//      every byte this function itself goes on to assign is still
//      overwritten correctly afterward, so the frame content golden/this
//      port produce is IDENTICAL -- only bytes past the frame (which the
//      caller's `iLen` output tells it to ignore) are affected, and even
//      those are whatever `strSendUse[SendCommLength]` last held, exactly as
//      in golden. Kept exactly as golden wrote it.
//   2. `*(unsigned int *) &cSet[N]=swap16(value);` (golden :34-40,47-53,57,
//      69-71 -- every TCP/RTU field write). Each write stores a FULL 4-byte
//      `unsigned int` into a char buffer at a "2-byte field" offset, so
//      consecutive field writes overlap by 2 bytes (e.g. the Transaction-ID
//      write at cSet[0..3] is partially clobbered, then correctly
//      re-clobbered, by the Protocol-ID write at cSet[2..5]). Golden's own
//      field ORDER guarantees every 2-byte field's correct final value is
//      written by a LATER statement than whatever half-overwrote it
//      earlier, so the net byte pattern golden produces (and this port
//      reproduces, byte-for-byte, running the identical statements in the
//      identical order) is the intended Modbus frame -- this is a real
//      strict-aliasing-violating idiom, not a "translation risk"; the
//      arithmetic/pointer casts are kept EXACTLY as golden wrote them.
//   3. `ZeroMemory(iListValue, MaxListValue);` (golden :151,
//      ClearListValue) -- `iListValue` is `int[MaxListValue]` (40 bytes on
//      this build), but the byte count passed is `MaxListValue` (10), so
//      only the first 10 of 40 bytes (2.5 of the 10 ints) are actually
//      zeroed; `iListValueLen=0;` on the next line is what actually makes
//      AddListValue's `iListValue[iListValueLen++]=ivalue;` overwrite every
//      slot a caller can still reach anyway (only the FIRST
//      iListValueLen`<=MaxListValue values are ever read back, and every
//      one of those gets a fresh AddListValue write before being read), so
//      this has NO observable effect on the frame data this class ever
//      returns to a caller -- reproduced verbatim regardless.
//   4. `GetDataLength(int HexData){ return static_cast<int>(HexData)*0.5; }`
//      (golden :155-158) -- the redundant `static_cast<int>` on an
//      already-`int` parameter, the float multiply, and the implicit
//      narrowing back to `int` on return are all kept exactly as golden
//      wrote them (equivalent to `HexData/2` for even inputs, which is the
//      only case this class's own callers ever pass -- a Modbus byte count
//      is always even).
//
//  VCL/Borland conversions: #pragma hdrstop + #pragma package(smart_init)
//  dropped (golden :2 and :6). `ZeroMemory` comes from <windows.h>, reached
//  via MachineDefine.h (same path every other EJ1N unit uses). `byte`
//  likewise via <windows.h> -> rpcndr.h.  No __fastcall / __property /
//  __published anywhere in this .cpp (golden has none either).  Numeric
//  semantics, magic numbers (0xFFFF, 0xa001, 0x03/0x06/0x10 function codes,
//  the 12/8-byte frame-header lengths) kept EXACT.
//
//  MINIMAL NECESSARY ADAPTATION (1, flagged inline with //AI at the line):
//   golden `CRC_Check(&cSet[0], 6)` (golden :71, GetClientEncodeSingleRTU)
//   passes a `char*` where CRC_Check's own declared parameter is
//   `unsigned char*`; BCB6 accepted the implicit signed/unsigned pointer
//   conversion, MinGW g++ does not (`invalid conversion from 'char*' to
//   'unsigned char*'`). Cast added at the call site: same bytes, same call.
//
//  Big5: the one Chinese comment ("新增可讀寫負數", golden :53,107,123,137)
//  decoded via cp950 and preserved as UTF-8. Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "uModbusCommand.h" ; #pragma package(smart_init)
// =============================================================================
#include "MachineDefine.h"          // de-VCL'd include hub (vclcompat umbrella + <windows.h>)
#include "EJ1N/uModbusCommand.h"    // this unit's own contract
//---------------------------------------------------------------------------
#define swap16(x) ((((x) & 0xFF)<<8) | (((x)>>8) & 0xFF))
//---------------------------------------------------------------------------
uModbusCommand::uModbusCommand()
{
    Clear();
}
//---------------------------------------------------------------------------
uModbusCommand::~uModbusCommand()
{
//
}
//---------------------------------------------------------------------------
void uModbusCommand::Clear()
{
    StartAddress=0;
    SlaveID=1;
    bisWrite=false;
    iDataCount=0;
}
//---------------------------------------------------------------------------
//TCP
//---------------------------------------------------------------------------
void uModbusCommand::GetClientEncodeSingleTCP(char* cSet, int &iLen)
{
    iLen=12;
    memset(cSet,0x0,sizeof(cSet));
    *(unsigned int *) &cSet[0]=swap16(0);           //2byte Transaction ID
    *(unsigned int *) &cSet[2]=swap16(0);           //2byte Protocal ID
    *(unsigned int *) &cSet[4]=swap16(iLen-5-1);    //2byte Length
    cSet[6]=SlaveID;                                //1byte slave
    cSet[7]=bisWrite?6:3;                           //1byte Function Code  3=read  6=write  16=write muti
    *(unsigned int *) &cSet[8]=swap16(StartAddress);//2byte Start register addr
    *(unsigned int *) &cSet[10]=swap16(iListValue[0]);  //2byte Data
}
//---------------------------------------------------------------------------
void uModbusCommand::GetClientEncodeWriteMultipleTCP(char* cSet, int &iLen)
{
    iLen=12+1+(iListValueLen*2);//head + wordcount + datas
    memset(cSet,0x0,sizeof(cSet));
    *(unsigned int *) &cSet[0]=swap16(0);           //2byte Transaction ID
    *(unsigned int *) &cSet[2]=swap16(0);           //2byte Protocal ID
    *(unsigned int *) &cSet[4]=swap16(iLen-5-1);    //2byte Length
    cSet[6]=SlaveID;                                //1byte slave
    cSet[7]=16;                                     //1byte Function Code  16=write muti
    *(unsigned int *) &cSet[8]=swap16(StartAddress);//2byte Start register addr
    *(unsigned int *) &cSet[10]=swap16(iListValueLen);  //2byte Word Count
    cSet[12]=iListValueLen*2;                       // 1byte Bytes count
    for(int i=0; i<iListValueLen; i++)    //set sv
    {
        *(unsigned int *) &cSet[13+i*2]=swap16(iListValue[i]);
    }
}
//---------------------------------------------------------------------------
//RTU
//---------------------------------------------------------------------------
void uModbusCommand::GetClientEncodeSingleRTU(char* cSet, int &iLen)
{
    iLen=8;
    memset(cSet, 0x0, sizeof(cSet));
    cSet[0]=SlaveID;            //Station number 1byte
    cSet[1]=bisWrite?6:3;       //Function Code 1byte 3=read    6=write  16=write muti
    *(unsigned int *) &cSet[2]=swap16(StartAddress);    //StartAddress 2byte
    *(unsigned int *) &cSet[4]=swap16(iListValue[0]);   //Length 2byte
    *(unsigned int *) &cSet[6]=CRC_Check((unsigned char *)&cSet[0], 6);  //CRC 2byte
}
//---------------------------------------------------------------------------
unsigned int uModbusCommand::CRC_Check(unsigned char *ary, unsigned int len)
{
    unsigned int crc=0;
    int Bitloop=0;
    crc=0xFFFF;
    while(len--)
    {
        crc^=*ary++;
        for(Bitloop=0; Bitloop<8; Bitloop++)
        {
            if(crc&01)
            {
                crc=(crc>>1)^0xa001;
            }
            else
            {
                crc=crc>>1;
            }
        }
    }
    return (crc);
}
//---------------------------------------------------------------------------
void uModbusCommand::DecodeToListIntRTU(char* cSet, int iLen)
{
    ClearListValue();
    if(iLen>6)
    {
        if(cSet[1]==0x03)
        {
            iDataCount=GetDataLength((int)cSet[2]);
            for(int i=0; i<iDataCount; i++)
            {
                AddListValue(TwoBytes2Short(cSet[3+i*2], cSet[4+i*2]));      //Jimmychiu 20230216 新增可讀寫負數
            }
        }
    }
}
//---------------------------------------------------------------------------
void uModbusCommand::DecodeToListIntTCP(char* cSet, int iLen)
{
    ClearListValue();
    if(iLen>9)
    {
        if(cSet[7]==0x03)
        {
            iDataCount=GetDataLength((int)cSet[8]);
            for(int i=0; i<iDataCount; i++)
            {
                AddListValue(TwoBytes2Short(cSet[9+i*2], cSet[10+i*2]));     //Jimmychiu 20230216 新增可讀寫負數
            }
        }
    }
}
//---------------------------------------------------------------------------
int uModbusCommand::TwoBytes2Int(byte _H , byte _L)
{
    int backnit=0;
    backnit=_L&0xFF;
    backnit|=(_H<<8)&0xFF00;
    return backnit;
}
//---------------------------------------------------------------------------
int uModbusCommand::TwoBytes2Short(byte _H , byte _L)                       //Jimmychiu 20230216 新增可讀寫負數
{
    int backnit=TwoBytes2Int(_H,_L);
    return backnit>=0x8000?backnit-0x10000:backnit;
}
//---------------------------------------------------------------------------
void uModbusCommand::AddListValue(int ivalue)
{
    if(iListValueLen<MaxListValue)
        iListValue[iListValueLen++]=ivalue;
}
//---------------------------------------------------------------------------
void uModbusCommand::ClearListValue()
{
    ZeroMemory(iListValue, MaxListValue);
    iListValueLen=0;
}
//---------------------------------------------------------------------------
int uModbusCommand::GetDataLength(int HexData)
{
    return static_cast<int>(HexData)*0.5;
}
//---------------------------------------------------------------------------
