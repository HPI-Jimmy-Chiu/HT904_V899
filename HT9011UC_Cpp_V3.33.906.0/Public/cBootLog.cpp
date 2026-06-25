//---------------------------------------------------------------------------
// Translated from BCB6: Public/cBootLog.cpp (cBootLog.cpp:1-74)
// AI(migration-w1) 20260626: Replace <vcl.h>+<SysUtils.hpp> -> vclcompat;
//   drop #pragma hdrstop / #pragma package(smart_init).
//   Win32-only TU (MinGW-on-Windows); not Linux-portable -- acceptable,
//   target is Windows (same as BCB6 original).
//
// Original comment (cBootLog.cpp:2):
//   AI(ht9045-v899) 20260423: BootLog implementation (pure Win32, no VCL deps in hot path)
//---------------------------------------------------------------------------
#include "cBootLog.h"       // was: #include <vcl.h> + #include "cBootLog.h"
#include <stdio.h>
#include <windows.h>
// <SysUtils.hpp> was #included in BCB6 (cBootLog.cpp:9); replaced by vclcompat
// (already pulled in via cBootLog.h -> vcl_compat.h).
//---------------------------------------------------------------------------
// BCB6 cBootLog.cpp:13-15
#define BOOT_LOG_DIR   "D:\\HT9045\\Error"
#define BOOT_LOG_FILE  "D:\\HT9045\\Error\\BootLog.txt"
#define BOOT_LOG_MAX   (512 * 1024)
//---------------------------------------------------------------------------
// BCB6 cBootLog.cpp:17-26
// Returns the current byte-size of BootLog.txt via Win32 CreateFile/GetFileSize.
// Returns 0 if the file does not exist or cannot be opened.
static int GetBootLogSize()
{
    HANDLE h = CreateFile(BOOT_LOG_FILE, GENERIC_READ,
                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                          NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(h == INVALID_HANDLE_VALUE) return 0;
    DWORD dw = GetFileSize(h, NULL);
    CloseHandle(h);
    return (int)dw;
}
//---------------------------------------------------------------------------
// BCB6 cBootLog.cpp:28-42
// Ensures the log directory exists, then renames BootLog.txt -> BootLog.txt.old
// when the file exceeds BOOT_LOG_MAX bytes.
//
// NOTE on DeleteFile / MoveFile:
//   In BCB6 the calls resolve to Win32 macros (DeleteFileA / MoveFileA).
//   Here <windows.h> is included AFTER vclcompat, so the Win32 macros are
//   still in scope.  vclcompat::DeleteFile takes an AnsiString; the original
//   code passes .c_str() (a const char*), so it matches the Win32 macro
//   DeleteFileA directly.  We call ::DeleteFileA and ::MoveFileA explicitly
//   to avoid any ambiguity with vclcompat::DeleteFile.
void RotateBootLogIfNeeded()
{
    try
    {
        // BCB6 cBootLog.cpp:32-33
        if(!DirectoryExists(BOOT_LOG_DIR))
            ForceDirectories(BOOT_LOG_DIR);
        // BCB6 cBootLog.cpp:34-40
        if(GetBootLogSize() > BOOT_LOG_MAX)
        {
            AnsiString sBak = AnsiString(BOOT_LOG_FILE) + ".old";
            ::DeleteFileA(sBak.c_str());                     // BCB6: DeleteFile(sBak.c_str())
            ::MoveFileA(BOOT_LOG_FILE, sBak.c_str());        // BCB6: MoveFile(...)
        }
    }
    catch(...) { /* never fail boot log -- BCB6 cBootLog.cpp:41 */ }
}
//---------------------------------------------------------------------------
// BCB6 cBootLog.cpp:44-47
void WriteBootLog(AnsiString sStep)
{
    WriteBootLog(sStep, "");
}
//---------------------------------------------------------------------------
// BCB6 cBootLog.cpp:49-73
// Appends one timestamped line to BootLog.txt:
//   "YYYY-MM-DD HH:MM:SS.mmm [TID:n] <sStep>[ | <sDetail>]\n"
// Uses Win32 SYSTEMTIME / GetLocalTime / GetCurrentThreadId; fopen in append mode.
// Never throws (all exceptions swallowed to protect caller during early init).
void WriteBootLog(AnsiString sStep, AnsiString sDetail)
{
    try
    {
        // BCB6 cBootLog.cpp:53-54
        if(!DirectoryExists(BOOT_LOG_DIR))
            ForceDirectories(BOOT_LOG_DIR);

        // BCB6 cBootLog.cpp:56-57
        FILE* fp = fopen(BOOT_LOG_FILE, "a");
        if(fp == NULL) return;

        // BCB6 cBootLog.cpp:59-65
        SYSTEMTIME st;
        GetLocalTime(&st);
        fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d.%03d [TID:%lu] %s",
                st.wYear, st.wMonth, st.wDay,
                st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
                (unsigned long)GetCurrentThreadId(),
                sStep.c_str());
        // BCB6 cBootLog.cpp:66-68: only emit " | detail" when detail is non-empty
        if(sDetail.Length() > 0)
            fprintf(fp, " | %s", sDetail.c_str());
        // BCB6 cBootLog.cpp:68-70
        fprintf(fp, "\n");
        fflush(fp);
        fclose(fp);
    }
    catch(...) { /* never fail boot log -- BCB6 cBootLog.cpp:72 */ }
}
//---------------------------------------------------------------------------
