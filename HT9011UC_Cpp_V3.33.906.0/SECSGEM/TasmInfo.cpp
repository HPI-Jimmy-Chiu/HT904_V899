//---------------------------------------------------------------------------
//  SECSGEM/TasmInfo.cpp -- see TasmInfo.h for the file-head scope note.
//
//  Translation wave: W906-SvEcDataItem
//  Translator: AI(W906-SvEcDataItem) 20260720
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/TasmInfo.cpp
//  (108 lines, cp950).
//---------------------------------------------------------------------------
#include "SECSGEM/TasmInfo.h"

#if defined(_WIN32) || defined(WIN32)
#  include <windows.h>
#endif

//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: PORT-ONLY deviation D1 (design doc §5.3,
// "更保守" option adopted). golden's ExeCPUID/GetCPUFreq/GetManID/GetCPUType
// (golden TasmInfo.cpp:11-79) execute Borland-syntax x86 inline assembly
// (`__asm { mov eax,veax \n cpuid \n mov deax,eax ... }` / `_asm { RDTSC }`)
// against 4 file-static DWORD scratch registers (deax/debx/decx/dedx) plus
// 2 file-static ints (startFreq/overFreq) -- MinGW g++'s inline-asm dialect
// (AT&T, GCC `asm volatile(...)` syntax) does not accept Borland's `_asm{}`/
// `__asm{}` block syntax at all (a hard compile error, not merely a
// semantic difference), so a byte-for-byte port is not possible without
// rewriting the assembly in GCC's own dialect (`__builtin_ia32_rdtsc()` /
// `__get_cpuid()`, both available on MinGW).
//
// Per the design doc's own risk assessment (§5.3 "替代方案（更保守...）"),
// the conservative fixed-placeholder route is taken instead of the
// intrinsic-rewrite route: these 3 functions feed exactly 3 SVs (SV10 "CPU
// Frequence", SV11 "CPU Manufacturer", SV12 "CPU Type" -- see THGem::
// FormCreate) that are HOST-QUERIED-ONLY data (a remote SECS/GEM host can
// read them via S1F3/S1F4, but nothing in this tree's own machine-behavior
// state machines ever reads GetCPUFreq()/GetManID()/GetCPUType()'s return
// value back -- grep-confirmed zero non-FormCreate call sites in golden).
// A placeholder value therefore cannot desync any state machine or test
// oracle; it only changes what a hypothetical remote host would see for 3
// read-only diagnostic SVs. GetCPUFreq->0, GetManID->"GenuineIntel",
// GetCPUType->"" (design doc §5.3's own literal suggestion,
// "GenuineIntel"/""/0 -- GetManID is the real leaf-0 vendor-ID string a
// genuine Intel CPU returns, the most plausible-looking of the 3
// placeholders; GetCPUType, the leaf-0x80000002-4 brand string, and
// GetCPUFreq, the RDTSC-measured clock, both have no similarly canonical
// placeholder, so "" / 0 are used).
//---------------------------------------------------------------------------
long GetCPUFreq()
{
    return 0;   // PORT-ONLY placeholder -- see file-head note above.
}
//---------------------------------------------------------------------------
AnsiString GetManID()
{
    return "GenuineIntel";   // PORT-ONLY placeholder -- see file-head note above.
}
//---------------------------------------------------------------------------
AnsiString GetCPUType()
{
    return "";   // PORT-ONLY placeholder -- see file-head note above.
}
//---------------------------------------------------------------------------
// golden TasmInfo.cpp:82-96 -- faithful Win32 port. golden calls Borland's
// `Sysutils::GetDiskFreeSpaceEx(szDisk, freeSpace, totalSpace, &totalFreeSpace)`
// (a VCL SysUtils wrapper over the same underlying Win32 API, __int64 out
// params); ported here directly against the real Win32
// `GetDiskFreeSpaceExA` (ULARGE_INTEGER out params) -- identical semantics,
// no vclcompat SysUtils shim needed for this one call. golden reads back
// ONLY the 3rd (`totalSpace`, matches Win32's lpTotalNumberOfBytes) and 4th
// (`totalFreeSpace`, matches Win32's lpTotalNumberOfFreeBytes) out-params
// into iTotSpace/iTotFree; the 2nd (`freeSpace`/lpFreeBytesAvailableToCaller)
// is golden-computed but never read back either -- preserved as a
// write-only out-param here too (matches golden's own dead-store).
//---------------------------------------------------------------------------
bool GetDiskFreeSpaceMB(char Disk, int &iTotSpace, int &iTotFree)
{
    char szDisk[10];
    szDisk[0] = Disk;
    szDisk[1] = ':';
    szDisk[2] = 0;

#if defined(_WIN32) || defined(WIN32)
    ULARGE_INTEGER freeSpace, totalSpace, totalFreeSpace;
    BOOL ret = ::GetDiskFreeSpaceExA(szDisk, &freeSpace, &totalSpace, &totalFreeSpace);
    iTotSpace = (int)(totalSpace.QuadPart / (1024 * 1024));
    iTotFree  = (int)(totalFreeSpace.QuadPart / (1024 * 1024));
    return ret != 0;
#else
    // Non-Windows test-harness build: no real disk to query -- conservative
    // zeroed placeholder (never reached on the project's real Win32 target).
    iTotSpace = 0;
    iTotFree  = 0;
    return false;
#endif
}
//---------------------------------------------------------------------------
// golden TasmInfo.cpp:98-106 -- faithful Win32 port (GlobalMemoryStatusEx).
//---------------------------------------------------------------------------
void GetGlobalMemoryStatusKB(unsigned long &ulMemoryLoad, unsigned long &ulTotalPhys, unsigned long &ulAvailPhys)
{
#if defined(_WIN32) || defined(WIN32)
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    ::GlobalMemoryStatusEx(&statex);
    ulMemoryLoad = statex.dwMemoryLoad;
    ulTotalPhys  = (unsigned long)(statex.ullTotalPhys / 1024);
    ulAvailPhys  = (unsigned long)(statex.ullAvailPhys / 1024);
#else
    ulMemoryLoad = 0;
    ulTotalPhys  = 0;
    ulAvailPhys  = 0;
#endif
}
