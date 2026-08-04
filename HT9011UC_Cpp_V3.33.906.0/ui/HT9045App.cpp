// =============================================================================
//  ui/HT9045App.cpp -- Gate A application skeleton (W7-U0)
//
//  AI(W906-GateA-0) 20260804: NEW FILE. Carries golden WinMain's semantics
//  (HT9045.cpp:127-301) into MFC InitInstance. Deviation ledger -- every one
//  is a recorded decision (docs/DESIGN_GateA.md D-A0-*), not an omission:
//
//   [DEV-1] OSK kiosk block SKIPPED (golden :132-138: taskkill OSK.exe + 4
//           `reg add HKCU\...\Osk` writes). Machine-kiosk behaviour, not a
//           startup precondition; writing the registry of a dev machine from
//           a smoke run would be an unwanted side effect. Restore verbatim in
//           the deployment wave (tracked in DESIGN_GateA.md D-A0-1).
//   [DEV-2] exe-path check (golden :150-155) kept FAITHFUL by default --
//           MessageBox("Please copy HT9045.EXE to D:\HT9045\EXE") + exit --
//           but bypassable with --devpath so the build-dir exe can run during
//           development. Golden has no such switch.
//   [DEV-3] the 118 Application->CreateForm calls (golden :166-283; 115 unique
//           pointers, 3 golden copy-paste duplicates -- see DESIGN_GateA.md
//           D-A0-2 and docs/RECON_GateA_FormRegistry.md) are NOT here yet.
//           Gate A's FormRegistry (GA-3) will construct the ported facade
//           objects in golden's order; today only the placeholder dialog
//           exists. The count-down of that gap lives in the DESIGN doc.
//   [DEV-4] `Application->Run()` maps to a modal DoModal() on the main dialog
//           (standard MFC dialog-app shape): InitInstance returns FALSE so
//           CWinApp::Run never spins a second loop.
//   [DEV-5] LogSoftwareOnTime (golden :160,:163) not called yet -- its home
//           (main.cpp) is untranslated until GA-3; BootLog checkpoints are
//           carried since Public/cBootLog.cpp is already ported (W1).
//
//  FAITHFUL pieces: single-instance CreateMutex("MyMutexName")+GetLastError
//  (golden :142-144, same mutex name on purpose -- it also mutually excludes
//  against a running BCB6 HT9045.exe, which golden's own semantics imply);
//  BootLog rotate + "WinMain Enter" checkpoint order (golden :130-131);
//  try/catch envelope with BootLog capture (golden :146,:289-299).
// =============================================================================
#include "HT9045App.h"
#include "GateAPlaceholderDlg.h"
#include "RegisterCustomClasses.h"   // AI(W906-GateA-4-C5) 20260804: HT9045_RegisterAllCustomClasses

#include "Public/cBootLog.h"    // RotateBootLogIfNeeded / WriteBootLog (ported W1)

CHT9045App theApp;

CHT9045App::CHT9045App()
    : m_iSmokeCloseMs(0)
    , m_bDevPathBypass(false)
{
}

BOOL CHT9045App::InitInstance()
{
    // ---- golden :130-131 -------------------------------------------------
    RotateBootLogIfNeeded();
    WriteBootLog("WinMain Enter");

    // [DEV-1] golden :132-138 OSK kiosk block deliberately skipped (see banner).

    // ---- command line (Gate A only; golden has none) -----------------------
    for (int i = 1; i < __argc; i++)
    {
        if (lstrcmpA(__argv[i], "--smoke") == 0 && i + 1 < __argc)
            m_iSmokeCloseMs = atoi(__argv[++i]);
        else if (lstrcmpA(__argv[i], "--devpath") == 0)
            m_bDevPathBypass = true;
    }
    if (m_iSmokeCloseMs > 0)
    {
        // Batch/smoke rule: a smoke run must never block on ANY modal fault
        // dialog (project hard rule; same rationale as the ctest INTERFACE
        // guard library). Interactive runs keep default error UI.
        ::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    }

    // ---- golden :142-144: single instance ---------------------------------
    ::CreateMutexA(NULL, FALSE, "MyMutexName");
    if (::GetLastError())
    {
        WriteBootLog("WinMain duplicate instance, exit");   // Gate A addition
        return FALSE;
    }

    try
    {
        // ---- golden :150-155: exe path check, [DEV-2] --------------------
        char szExe[MAX_PATH] = {0};
        ::GetModuleFileNameA(NULL, szExe, MAX_PATH);
        AnsiString asDir = szExe;                       // golden :150 semantics
        if (asDir.Pos("D:\\HT9045\\EXE\\") == 0)
        {
            if (!m_bDevPathBypass)
            {
                ::MessageBoxA(0, "Please copy HT9045.EXE to D:\\HT9045\\EXE",
                              "EXE Path Error", MB_OK);  // golden :153 verbatim
                return FALSE;
            }
            WriteBootLog("WinMain ExePath BYPASSED (--devpath)", asDir);
        }
        else
        {
            WriteBootLog("WinMain ExePath OK");          // golden :161
        }

        // AI(W906-GateA-4-C5) 20260804: register ALL 16 custom window classes
        // ONCE, before any dialog is created (W7 plan SS3-C1 ruling). An
        // unregistered class in a DIALOGEX template fails the whole
        // CreateDialog, so this must precede the first generated form.
        if (!HT9045_RegisterAllCustomClasses())
        {
            WriteBootLog("WinMain RegisterAllCustomClasses FAILED");
            return FALSE;
        }
        WriteBootLog("Custom classes registered");

        // [DEV-3] FormRegistry lands in GA-3; placeholder dialog only for now.
        WriteBootLog("Application Initialize Done");     // golden :164 checkpoint kept

        CGateAPlaceholderDlg dlg(m_iSmokeCloseMs);
        m_pMainWnd = &dlg;
        dlg.DoModal();                                    // [DEV-4]

        WriteBootLog("Application->Run returned (normal exit)");   // golden :287
    }
    catch (CException* e)                                 // golden :289-294 shape
    {
        char szMsg[256] = {0};
        e->GetErrorMessage(szMsg, sizeof(szMsg));
        WriteBootLog("WinMain CAUGHT Exception", szMsg);
        e->ReportError();
        e->Delete();
    }
    catch (...)                                           // golden :295-299
    {
        WriteBootLog("WinMain CAUGHT unknown exception (possible AV / hardware not ready)");
    }

    return FALSE;   // dialog app: everything ran inside InitInstance
}

int CHT9045App::ExitInstance()
{
    CWinApp::ExitInstance();    // MFC cleanup; its return value is discarded on purpose
    return 0;                   // golden WinMain returns 0 on EVERY path (:144/:154/:300)
}
