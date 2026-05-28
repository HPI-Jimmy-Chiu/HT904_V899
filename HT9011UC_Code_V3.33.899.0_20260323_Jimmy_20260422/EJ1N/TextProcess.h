//---------------------------------------------------------------------------

#ifndef TextProcessH
#define TextProcessH

//---------------------------------------------------------------------------
#define swap16(x) ((((x) & 0xFF)<<8) | (((x)>>8) & 0xFF))
//---------------------------------------------------------------------------
extern AnsiString MyDeCodeASCII(int iInPut);                                    //10進制 轉 ASCII Code
extern byte       MyASCIIToDec(AnsiString asCode);                              //ASCII Code 轉 10進制
extern AnsiString CountHexToByte(AnsiString asHCode);                           //16進制轉2進制
extern int        HexStrToInt(char *str);                                       //16進制轉10進制
extern int        HexStrToInt(AnsiString str);                                  //16進制轉10進制
extern int        SetBCC(AnsiString SendText);                                  //計算BCC
extern int        CountByteToInt(AnsiString asBCode);                           //2進制轉10進制
extern AnsiString CountByteToHex(AnsiString asBCode);                           //2進制轉16進制
extern AnsiString StringConvert2Hex(AnsiString S);                              //Sam 20240604 : 新增 BinDisplay TFT
extern int Convert2Byte(char *sData);
extern int ConvertByte(char *sData);
extern unsigned float2hex(unsigned char typecode, double value, int dataformat);
extern unsigned char A_Create_LCR(unsigned char *Sptr, unsigned char length);
extern unsigned char A_Create_LRC(unsigned char *Sptr, unsigned char length);

extern const unsigned char T_HEX2ASCII[16];
extern const unsigned char T_ASXII2HEX[23];
extern unsigned char T_HEX2ASCII_Mac(unsigned char hex2ascii);
extern unsigned char T_ASXII2HEX_Mac(unsigned char ascii2hex);
extern bool GetCOMPortStatus(AnsiString Com);                                   //Steven 20120217 : Com Port改成可定義
extern double MyFormatFloat(double dValue, int iDigi);                          //Steven 20241014 : 指定小數點位數
#endif
