// =============================================================================
//  HThreadCtrl.h  --  generic Win32 thread wrapper (declares HThreadCtrl).
//
//  Faithful translation of golden HThreadCtrl/HThreadCtrl.h (35 lines, BCB6,
//  Big5/cp950 -- but this file has NO Chinese text at all).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (group "tiny").
//
//  ROLE: header for the one class in this unit -- HThreadCtrl, a thin
//  CreateThread/WaitForSingleObject/TerminateThread wrapper.  A caller supplies
//  a plain `void (*)(LPVOID)` worker function pointer and a parameter; the
//  wrapper runs it in a spin loop that yields via MySleep(1) whenever it goes
//  more than `_sleep_delay_count` iterations without doing so.
//
//  No golden line-number citations follow in this header banner beyond the
//  class body itself (golden :1-35, verbatim below) -- there is nothing else
//  in the file to inventory.
//
//  VCL/Borland conversions: none needed.  Golden already spoke pure Win32 API
//  (HANDLE/DWORD/LPVOID/CALLBACK/INFINITE) with no __fastcall / __property /
//  __published anywhere in this header.  `#ifndef HThread_H` guard kept
//  byte-identical (note: NOT "HThreadCtrl_H" -- golden's own guard name).
// =============================================================================
#ifndef HThread_H
#define HThread_H

#include <windows.h>

typedef void (*PF_PROCESS)(LPVOID);

/*
Class Name      : HThreadCtrl
Version         : 1.00
Release Date    : 2008.10.14
*/
class HThreadCtrl
{
private:
    HANDLE      _handle;
    DWORD       _id;
    LPVOID      _parameter;
    PF_PROCESS  _process_func;  // Thread execute function pointer

    bool    _execute;

    int     _sleep_delay_count;

protected:
    static DWORD CALLBACK ThreadProcess( LPVOID lpParameter);

public:
    HThreadCtrl();
    ~HThreadCtrl();

    void Initial( PF_PROCESS Func , LPVOID Param );
    int OpenThread();
    int CloseThread( int iTimeOut = INFINITE );

    void SetSleepDelayCount( int iDelay );
};

#endif
