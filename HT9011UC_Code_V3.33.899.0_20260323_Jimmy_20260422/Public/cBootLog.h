//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260423: BootLog for crash diagnosis (always-on, file-based)
// Purpose:
//   Customer reported software dies during startup when 24V power is off.
//   Existing LogSoftwareOnTime() is gated by DEBUG_SOFTWARE_EXEC_TIME and
//   uses TMyStringList which itself may throw during early init.
//   This BootLog uses pure Win32 file I/O so it works at the earliest
//   possible moment in WinMain(), survives VCL exceptions, and gives
//   a deterministic last-step trace when WER tombstones the process.
//---------------------------------------------------------------------------
#ifndef cBootLogH
#define cBootLogH
//---------------------------------------------------------------------------
#include <vcl.h>
//---------------------------------------------------------------------------
// Append one line to D:\HT9045\Error\BootLog.txt with timestamp + thread id.
// Safe to call from any thread, never throws.
//---------------------------------------------------------------------------
void WriteBootLog(AnsiString sStep);
void WriteBootLog(AnsiString sStep, AnsiString sDetail);

// Rotate BootLog.txt -> BootLog.txt.old when exceeding ~512KB.
// Call once at WinMain entry.
void RotateBootLogIfNeeded();
//---------------------------------------------------------------------------
#endif
