//---------------------------------------------------------------------------
#ifndef TasmInfoH
#define TasmInfoH
//---------------------------------------------------------------------------
#define cpuid dw 0xa20f         // 2013.11.08 , Joye , KYEC SECS/GEM
#define RDTSC dw 310Fh          // 2013.11.08 , Joye , KYEC SECS/GEM
//---------------------------------------------------------------------------

// 2013/09/16 lee start
long GetCPUFreq();
AnsiString GetManID();   //取得製造商ID
AnsiString GetCPUType();
// 2013/09/16 lee end
bool GetDiskFreeSpaceMB(char Disk,int &iTotSpace,int &iTotFree);  //2013/09/30 lee
void GetGlobalMemoryStatusKB(unsigned long &ulMemoryLoad,unsigned long &ulTotalPhys,unsigned long &ulAvailPhys);  //2013/09/30 lee

#endif
