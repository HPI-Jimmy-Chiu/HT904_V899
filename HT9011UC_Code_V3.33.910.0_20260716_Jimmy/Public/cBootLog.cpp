//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260423: BootLog implementation (pure Win32, no VCL deps in hot path)
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "cBootLog.h"
#include <stdio.h>
#include <windows.h>
#include <SysUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#define BOOT_LOG_DIR   "D:\\HT9045\\Error"
#define BOOT_LOG_FILE  "D:\\HT9045\\Error\\BootLog.txt"
#define BOOT_LOG_MAX   (512 * 1024)
//---------------------------------------------------------------------------
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
void RotateBootLogIfNeeded()
{
    try
    {
        if(!DirectoryExists(BOOT_LOG_DIR))
            ForceDirectories(BOOT_LOG_DIR);
        if(GetBootLogSize() > BOOT_LOG_MAX)
        {
            AnsiString sBak = AnsiString(BOOT_LOG_FILE) + ".old";
            DeleteFile(sBak.c_str());
            MoveFile(BOOT_LOG_FILE, sBak.c_str());
        }
    }
    catch(...) { /* never fail boot log */ }
}
//---------------------------------------------------------------------------
void WriteBootLog(AnsiString sStep)
{
    WriteBootLog(sStep, "");
}
//---------------------------------------------------------------------------
void WriteBootLog(AnsiString sStep, AnsiString sDetail)
{
    try
    {
        if(!DirectoryExists(BOOT_LOG_DIR))
            ForceDirectories(BOOT_LOG_DIR);

        FILE* fp = fopen(BOOT_LOG_FILE, "a");
        if(fp == NULL) return;

        SYSTEMTIME st;
        GetLocalTime(&st);
        fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d.%03d [TID:%lu] %s",
                st.wYear, st.wMonth, st.wDay,
                st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
                (unsigned long)GetCurrentThreadId(),
                sStep.c_str());
        if(sDetail.Length() > 0)
            fprintf(fp, " | %s", sDetail.c_str());
        fprintf(fp, "\n");
        fflush(fp);
        fclose(fp);
    }
    catch(...) { /* never fail boot log */ }
}
//---------------------------------------------------------------------------
