#include "MachineDefine.h"
#pragma hdrstop

#include "uModbusCommand.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
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
    *(unsigned int *) &cSet[6]=CRC_Check(&cSet[0], 6);  //CRC 2byte
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
