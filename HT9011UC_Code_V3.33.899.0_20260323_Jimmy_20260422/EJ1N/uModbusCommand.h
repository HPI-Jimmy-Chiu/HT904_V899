//---------------------------------------------------------------------------

#ifndef uModbusCommandH
#define uModbusCommandH
//---------------------------------------------------------------------------
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#define CommLength 255
#define MaxListValue 10
//---------------------------------------------------------------------------
class uModbusCommand
{
private:
    int iDataCount;
public: // User declarations
    uModbusCommand();
    ~uModbusCommand();
    void Clear();
    //---------------------------------------------------------------------------
    //Encode
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    //TCP
    //---------------------------------------------------------------------------
    void GetClientEncodeSingleTCP(char* cSet,int &iLen);
    void GetClientEncodeWriteMultipleTCP(char* cSet,int &iLen);
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    //RTU
    //---------------------------------------------------------------------------
    void GetClientEncodeSingleRTU(char* cSet,int &iLen);
    unsigned int CRC_Check(unsigned char *ary, unsigned int len);
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    //Decode
    //---------------------------------------------------------------------------
    int GetClientDecodeReadSingleRTU(char* cSet,int &iLen);
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    int StartAddress;
    int SlaveID;
    bool bisWrite;
    //Receive Data Decode
    int iListValue[MaxListValue];
    int iListValueLen;
    void DecodeToListIntRTU(char* cSet,int iLen);
    void DecodeToListIntTCP(char* cSet,int iLen);
    int  TwoBytes2Int(byte _H,byte _L);
    int TwoBytes2Short(byte _H , byte _L);      //Jimmychiu 20230216 新增可讀寫負數
    void AddListValue(int ivalue);
    void ClearListValue();
    int GetDataLength(int HexData);
};
#endif
