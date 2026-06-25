//---------------------------------------------------------------------------
// Translated from BCB6: Public/cBootLog.h (cBootLog.h:1-26)
// AI(migration-w1) 20260626: Replace <vcl.h> -> vclcompat; drop Borland pragmas.
// Original purpose: Always-on file-based boot/crash tracer (added 20260423).
//   WriteBootLog appends a timestamped line to D:\HT9045\Error\BootLog.txt.
//   RotateBootLogIfNeeded renames to .old when > 512 KB.
//   Pure Win32 + AnsiString -- no VCL form / global Prod deps.
//---------------------------------------------------------------------------
#ifndef cBootLogH
#define cBootLogH
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"
//---------------------------------------------------------------------------
// Append one line to D:\HT9045\Error\BootLog.txt with timestamp + thread id.
// Format: "YYYY-MM-DD HH:MM:SS.mmm [TID:n] step[ | detail]\n"
// Safe to call from any thread, never throws.
//---------------------------------------------------------------------------
void WriteBootLog(AnsiString sStep);
void WriteBootLog(AnsiString sStep, AnsiString sDetail);

// Rotate BootLog.txt -> BootLog.txt.old when exceeding ~512 KB.
// Call once at WinMain entry before any other WriteBootLog call.
void RotateBootLogIfNeeded();
//---------------------------------------------------------------------------
#endif
