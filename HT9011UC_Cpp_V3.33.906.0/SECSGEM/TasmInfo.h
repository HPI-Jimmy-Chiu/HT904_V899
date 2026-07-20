//---------------------------------------------------------------------------
//  SECSGEM/TasmInfo.h -- CPU/disk/memory "asm info" helpers THGem::FormCreate
//  registers as SV10-19 (CPU frequency/manufacturer/type, disk C/D total+free
//  space, memory load/total/avail).
//
//  Translation wave: W906-SvEcDataItem
//  Translator: AI(W906-SvEcDataItem) 20260720
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/TasmInfo.h (17
//  lines) + TasmInfo.cpp (108 lines, cp950).
//
//  PORT-ONLY DEVIATION (flagged, see TasmInfo.cpp for the full rationale):
//  golden's GetCPUFreq/GetManID/GetCPUType are implemented with Borland-
//  syntax x86 inline assembly (`_asm { RDTSC }` / `__asm { mov eax,veax \n
//  cpuid ... }`), which MinGW g++ does not accept (different inline-asm
//  dialect entirely). Per design doc §5.3's own recommended "more
//  conservative" option, these 3 are ported as fixed placeholder returns
//  (see .cpp) rather than reimplemented with GCC-dialect inline asm/
//  intrinsics -- these 3 values are host-queried-only SV data (SV10/11/12,
//  registered by THGem::FormCreate) with ZERO machine-behavior consumer
//  anywhere in this tree (grep-confirmed), so a placeholder cannot desync
//  any state machine or test oracle. GetDiskFreeSpaceMB/GetGlobalMemoryStatusKB
//  are plain Win32 (GetDiskFreeSpaceExA/GlobalMemoryStatusEx) and are ported
//  faithfully, byte-for-byte semantics preserved.
//
//  golden's `#define cpuid dw 0xa20f` / `#define RDTSC dw 310Fh` (BCB6 inline-
//  asm mnemonic macros) are NOT reproduced here -- they only exist to feed
//  the gone `__asm{}` blocks; irrelevant to this port (see .cpp).
//---------------------------------------------------------------------------
#ifndef SECSGEM_TASMINFO_H
#define SECSGEM_TASMINFO_H

#include "vclcompat/vcl_compat.h"

// golden: long GetCPUFreq(); -- PORT-ONLY placeholder, see .cpp.
long GetCPUFreq();

// golden: AnsiString GetManID(); -- PORT-ONLY placeholder, see .cpp.
AnsiString GetManID();

// golden: AnsiString GetCPUType(); -- PORT-ONLY placeholder, see .cpp.
AnsiString GetCPUType();

// golden uHGemEquipment.cpp-callers pass 'c'/'d'; faithful Win32 port.
bool GetDiskFreeSpaceMB(char Disk, int &iTotSpace, int &iTotFree);

// faithful Win32 port (GlobalMemoryStatusEx).
void GetGlobalMemoryStatusKB(unsigned long &ulMemoryLoad, unsigned long &ulTotalPhys, unsigned long &ulAvailPhys);

#endif // SECSGEM_TASMINFO_H
