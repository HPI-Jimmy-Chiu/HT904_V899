// =============================================================================
//  test_bootstrap.cpp -- BATCH-SAFETY bootstrap linked into EVERY test target.
//
//  AI(W906-TestBootstrap) 20260728: added after an MSVC Debug-CRT test run
//  wedged the whole suite and spammed the developer's screen with modal
//  dialogs.
//
//  THE PROBLEM
//  -----------
//  These tests are run in batch (`ctest -jN`, 95 executables, often unattended
//  and often from an agent's background shell). On Windows, several failure
//  modes do NOT just return a non-zero exit code -- they raise a MODAL DIALOG
//  and block forever waiting for a human to click OK:
//
//    * MSVC **Debug CRT**: a failed assert() / _CrtDbg report pops the classic
//      "Debug Assertion Failed!" box.  Observed 20260728: an MSVC Debug build
//      of this suite reached 94/95 and then hung indefinitely on exactly this.
//      Nothing is written to the ctest log while it hangs, so the run just
//      looks stalled -- the log is actively misleading.
//    * MSVC CRT invalid-parameter handler: terminates via the same dialog path.
//    * abort(): the CRT's own abort message box, plus a WER report.
//    * Hard crashes (access violation, STATUS_STACK_OVERFLOW): the Windows
//      Error Reporting "program stopped working" box.  This is NOT MSVC-only --
//      this project has hit both a SegFault and a real STATUS_STACK_OVERFLOW
//      (the 16MB embedded THGem buffer, see DEVLOG 2026-07-27) under MinGW.
//
//  A blocking dialog in a batch run is strictly worse than a loud failure: it
//  converts "one test failed" into "the entire suite hangs and the machine
//  becomes unusable until someone dismisses N dialogs".
//
//  WHAT THIS DOES
//  --------------
//  Runs one static initializer, before main(), in every test executable, that
//  turns every one of those modal paths into stderr output + a non-zero exit.
//  It does NOT hide or suppress failures -- a failing test still fails, and now
//  it fails *visibly and immediately* with the diagnostic on stderr where ctest
//  captures it, instead of silently blocking.
//
//  This is TEST-ONLY infrastructure.  It is deliberately NOT linked into the
//  production libraries: the shipped application should keep Windows' default
//  error-reporting behaviour.
//
//  Wired in via `link_libraries(ht9045_test_bootstrap)` at the top of
//  tests/CMakeLists.txt, an INTERFACE library that carries this file as an
//  INTERFACE source -- so it is COMPILED INTO each test executable.  (It is
//  deliberately not a static library: an object whose only content is a static
//  initializer can be dropped by the linker when nothing references it.)
// =============================================================================
#ifdef _WIN32

#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>

#if defined(_MSC_VER)
#  include <crtdbg.h>
#endif

namespace {

#if defined(_MSC_VER)
// Replaces the CRT's default invalid-parameter behaviour (which ends in the
// same modal dialog).  Reports to stderr and exits non-zero so ctest records a
// real failure.
void HT9045_TestInvalidParameterHandler(const wchar_t *expression,
                                        const wchar_t *function,
                                        const wchar_t *file,
                                        unsigned int   line,
                                        uintptr_t      /*pReserved*/)
{
    fprintf(stderr,
            "[test_bootstrap] FATAL: CRT invalid parameter -- file=%ls line=%u "
            "function=%ls expression=%ls\n",
            file       ? file       : L"<unknown>",
            line,
            function   ? function   : L"<unknown>",
            expression ? expression : L"<unknown>");
    fflush(stderr);
    _exit(3);
}
#endif // _MSC_VER

struct HT9045_DisableModalErrorDialogs
{
    HT9045_DisableModalErrorDialogs()
    {
        // --- Win32 level (applies to BOTH MinGW and MSVC builds) -------------
        // SEM_FAILCRITICALERRORS  : no "there is no disk in the drive" box.
        // SEM_NOGPFAULTERRORBOX   : no WER "program stopped working" box on an
        //                           access violation / stack overflow.
        // SEM_NOOPENFILEERRORBOX  : no "file not found" box.
        SetErrorMode(SEM_FAILCRITICALERRORS |
                     SEM_NOGPFAULTERRORBOX  |
                     SEM_NOOPENFILEERRORBOX);

#if defined(_MSC_VER)
        // --- MSVC Debug CRT --------------------------------------------------
        // Send assert / error / warn reports to stderr instead of a dialog.
        // Without this, ONE failed assert in a Debug build blocks `ctest -jN`
        // indefinitely (observed 20260728).
        const int reports[] = { _CRT_ASSERT, _CRT_ERROR, _CRT_WARN };
        for (int i = 0; i < 3; ++i)
        {
            _CrtSetReportMode(reports[i], _CRTDBG_MODE_FILE);
            _CrtSetReportFile(reports[i], _CRTDBG_FILE_STDERR);
        }

        // abort(): suppress both the CRT's own message box and the WER report,
        // so it just terminates with a non-zero code that ctest can record.
        _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

        _set_invalid_parameter_handler(&HT9045_TestInvalidParameterHandler);
#endif // _MSC_VER
    }
};

// Static init -- runs before main() in whichever test executable compiles
// this TU.  Inert unless something actually fails.
HT9045_DisableModalErrorDialogs g_ht9045_disable_modal_error_dialogs;

} // anonymous namespace

#endif // _WIN32
