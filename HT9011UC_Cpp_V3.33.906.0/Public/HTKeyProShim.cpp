// ===========================================================================
//  Public/HTKeyProShim.cpp
//  Implementation of KeyPro_GetLevel (see HTKeyProShim.h).
//
//  Translator: AI(W?) 20260626
//
//  Resolves KEYPRO_GET_LEVEL from KeyProDLL.dll at run time and calls it; when
//  the DLL/proc is unavailable (offline / no dongle), returns 1 (valid dongle)
//  so the SECS/GEM gate in CheckKeyPro (HS_Function.cpp:4933) passes.
//
//  DECORATION (verified by `objdump -x Public/KeyProDLL.dll`):
//    The export-name table lists the symbol UNDECORATED despite __stdcall:
//        KEYPRO_GET_DATE_INFO, KEYPRO_GET_ERROR_CODE, KEYPRO_GET_ID,
//        KEYPRO_GET_LEVEL, KEYPRO_WRITE_DATE_INFO, KEYPRO_WRITE_ID_INOF
//    So GetProcAddress must use the plain string "KEYPRO_GET_LEVEL".  We try
//    that first, then fall back to the MSVC-style "_KEYPRO_GET_LEVEL@4" for
//    robustness against a differently-built DLL.
//
//  We do NOT static-link KeyProDLL.lib (32-bit OMF Borland import lib;
//  unusable by MinGW).
// ===========================================================================
#include "HTKeyProShim.h"

#if defined(_WIN32) && !defined(KEYPRO_STUB)
#  include <windows.h>
#endif

namespace {

// Real DLL export ABI: int __stdcall KEYPRO_GET_LEVEL(unsigned int).
// Keep __stdcall so a genuine DLL links/calls correctly on Windows.
#if defined(_WIN32) && !defined(KEYPRO_STUB)
typedef int(__stdcall* KeyProGetLevelFn)(unsigned int);

// Resolution state, computed once (lazy).  Not guarded by a mutex: the live
// call site (CheckKeyPro) is invoked from the UI thread only, matching the
// original single-threaded VCL call pattern.
bool             g_bResolved = false;  // have we attempted resolution yet?
KeyProGetLevelFn g_pfnGetLevel = 0;    // resolved entry, or 0 if unavailable

void ResolveOnce()
{
    if (g_bResolved)
        return;
    g_bResolved = true;

    // Lazy LoadLibrary; cache the handle implicitly via the resolved fn ptr.
    // (We intentionally leak the HMODULE for process lifetime, exactly like a
    // statically-imported DLL would stay mapped -- no FreeLibrary needed.)
    HMODULE h = ::LoadLibraryA("KeyProDLL.dll");
    if (h == 0)
        return;  // DLL absent -> offline stub path (g_pfnGetLevel stays 0)

    // Borland exports the def-name undecorated; try that first.
    FARPROC p = ::GetProcAddress(h, "KEYPRO_GET_LEVEL");
    if (p == 0)
        // Fall back to MSVC __stdcall decoration (_name@bytes).
        p = ::GetProcAddress(h, "_KEYPRO_GET_LEVEL@4");

    if (p != 0)
        g_pfnGetLevel = reinterpret_cast<KeyProGetLevelFn>(p);
    // If still 0, the offline stub path applies.
}
#endif // _WIN32 && !KEYPRO_STUB

} // anonymous namespace

int KeyPro_GetLevel(unsigned int iIndex)
{
#if defined(_WIN32) && !defined(KEYPRO_STUB)
    ResolveOnce();
    if (g_pfnGetLevel != 0)
        return g_pfnGetLevel(iIndex);
    // DLL / dongle absent: offline valid-dongle stub.
    return 1;
#else
    // Non-Windows build or KEYPRO_STUB forced: offline valid-dongle stub.
    (void)iIndex;
    return 1;
#endif
}
