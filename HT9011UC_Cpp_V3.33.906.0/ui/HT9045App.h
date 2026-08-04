// =============================================================================
//  ui/HT9045App.h -- Gate A application skeleton (W7-U0)
//
//  AI(W906-GateA-0) 20260804: NEW FILE. This is the MFC CWinApp that will own
//  the ported HT9045.exe. Gate A scope (docs/GATE_A_FIRST_LIGHT_PLAN.md GA-0):
//  faithful WinMain *semantics* (mutex / BootLog / exe-path check) plus an
//  empty placeholder dialog, so the "exe links, opens, closes" milestone is
//  provable before any real form exists. The placeholder dialog is REPLACED
//  in GA-4 by the generated fMain dialog (tools/dfm2rc/rc_out/main.rc).
//
//  Golden reference for the semantics carried here:
//    D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\HT9045.cpp:127-301 (WinMain)
//  MFC supplies _tWinMain via the global `theApp` instance, so there is no
//  hand-written WinMain in this tree; golden's WinMain body maps onto
//  InitInstance() below. Deviations are listed in ui/HT9045App.cpp's banner
//  and mirrored in docs/DESIGN_GateA.md (they are decisions, not accidents).
//
//  MSVC-only: this header must never be included by any MinGW-built TU.
//  The whole ui/ directory is compiled solely inside the `if(HT9045_UI)`
//  block of the root CMakeLists.txt (option default OFF).
// =============================================================================
#ifndef HT9045App_H
#define HT9045App_H

#include <afxwin.h>

class CHT9045App : public CWinApp
{
public:
    CHT9045App();

    // Gate A command-line switches (parsed in InitInstance):
    //   --smoke <ms>   auto-close the placeholder dialog after <ms> milliseconds
    //                  and suppress WER fault dialogs (SetErrorMode) -- exists so
    //                  automated verification can run the exe without any modal
    //                  wait (hard project rule: batch runs must never block on a
    //                  dialog). 0 = interactive (default).
    //   --devpath      bypass golden's "exe must live in D:\HT9045\EXE\" check
    //                  (golden HT9045.cpp:151-155) so the build-dir exe can run
    //                  during development. Without it the check is FAITHFUL:
    //                  MessageBox + exit, same as golden.
    int   m_iSmokeCloseMs;   // 0 = interactive
    bool  m_bDevPathBypass;

    virtual BOOL InitInstance();

    // Golden WinMain returns 0 on EVERY path (HT9045.cpp:144 dup-instance,
    // :154 path error, :300 normal). MFC's default ExitInstance would instead
    // leak the wParam of the last modal-loop message as the process exit code
    // (observed: the smoke WM_TIMER with timer-id 1 produced exit code 1).
    // Overriding restores golden's exit-code contract.
    virtual int ExitInstance();
};

extern CHT9045App theApp;

#endif // HT9045App_H
