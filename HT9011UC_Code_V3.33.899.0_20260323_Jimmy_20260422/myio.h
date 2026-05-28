//---------------------------------------------------------------------------
#ifndef myioH
#define myioH
//---------------------------------------------------------------------------

//void CheckWindowsIsXP();

int  IOSetOutport(int port);
bool IOSetInport(int port);
void IOBitOn(int port, int bit);
void IOBitOff(int port, int bit);
void IOByteOut(int port, unsigned int Byte);
bool IOOutBitStatus(int port, int bit);
byte IOOutByteStatus(int port);
bool IOInputBit(int port, int bit);
unsigned int IOInputLongByte(int port);
byte IOInputByte(int port);
void BackUpOutputData();
void RestoreOutputData();
void InitialMyIOData();
#endif
