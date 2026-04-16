#include <vcl.h>
#pragma hdrstop

#include "TasmInfo.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//2013/09/16 lee start
DWORD deax;
DWORD debx;
DWORD decx;
DWORD dedx;
int startFreq,overFreq;                //2013/09/30  lee
//---------------------------------------------------------------------------
void ExeCPUID(DWORD veax)  //Initial CPU
{
    __asm
    {
        mov eax,veax
        cpuid
        mov deax,eax
        mov debx,ebx
        mov decx,ecx
        mov dedx,edx
    }
}
//---------------------------------------------------------------------------
long GetCPUFreq()       // Get CPU frequence : MHZ
{
    _asm
    {
        RDTSC
        mov startFreq,eax                //2013/09/30  lee
    }
    Sleep(50);
    _asm
    {
        RDTSC
        mov overFreq,eax                 //2013/09/30  lee
    }

    //return (over-start)/50000;          //2013/09/30  lee
    return (overFreq-startFreq)/50000;    //2013/09/30  lee
}
//---------------------------------------------------------------------------
AnsiString GetManID()   //取得製造商ID
{
    char ID[25];
    memset(ID,0,sizeof(ID));

    ExeCPUID(0);          //Initial
    memcpy(ID+0,&debx,4); //製造商 get
    memcpy(ID+4,&dedx,4);
    memcpy(ID+8,&decx,4);

    return AnsiString(ID);
}
//---------------------------------------------------------------------------
AnsiString GetCPUType()
{
    const DWORD id = 0x80000002; // start from 0x80000002 ,end 0x80000004
    char CPUType[49];// store CPU type
    memset(CPUType,0,sizeof(CPUType));//

    for(DWORD t = 0 ; t < 3 ; t++)
    {
        ExeCPUID(id+t);
        //save data
        memcpy(CPUType+16*t+ 0,&deax,4);
        memcpy(CPUType+16*t+ 4,&debx,4);
        memcpy(CPUType+16*t+ 8,&decx,4);
        memcpy(CPUType+16*t+12,&dedx,4);
    }

    return AnsiString(CPUType);
}
//2013/09/16 lee end
//2013/09/30 lee start
//---------------------------------------------------------------------------
bool GetDiskFreeSpaceMB(char Disk,int &iTotSpace,int &iTotFree)
{
    bool ret;
    char szDisk[10];
    char tempFreeSpc[256];
    __int64 freeSpace,totalSpace,totalFreeSpace ;
    szDisk[0]=Disk;
    szDisk[1]=':';
    szDisk[2]=0;

    ret=Sysutils::GetDiskFreeSpaceEx( szDisk, freeSpace, totalSpace, &totalFreeSpace);
    iTotSpace=totalSpace/(1024*1024);
    iTotFree =totalFreeSpace/(1024*1024);
    return ret;
}
//---------------------------------------------------------------------------
void GetGlobalMemoryStatusKB(unsigned long &ulMemoryLoad,unsigned long &ulTotalPhys,unsigned long &ulAvailPhys)
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx (&statex);
    ulMemoryLoad =statex.dwMemoryLoad;
    ulTotalPhys  =statex.ullTotalPhys/1024;
    ulAvailPhys  =statex.ullAvailPhys/1024;
}
//---------------------------------------------------------------------------