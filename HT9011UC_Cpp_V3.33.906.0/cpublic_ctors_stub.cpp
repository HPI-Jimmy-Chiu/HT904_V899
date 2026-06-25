// =============================================================================
//  cpublic_ctors_stub.cpp  -- Minimal constructor / destructor stubs
//
//  Translation wave: W4 (HAL motor layer prerequisite)
//  Translator: AI(W4) 20260626
//
//  WHY THIS FILE EXISTS
//  ----------------------
//  cprod.cpp defines `RUN_INFO RunInfo` (line 64, active) but the ctor body
//  is inside the cprod.cpp `#if 0 // TODO(W6)` block (lines 127-3979).
//  cpublic.cpp has TMyQueue10/TMyStrQueue100/TMyTimerQueue100 ctors similarly
//  gated `#if 0 // TODO(W3/W5/W6/W7)`.  These static-lifetime objects are
//  constructed at program startup; any TU that links ht9045_globals gets the
//  linker error if the ctors are absent.
//
//  These stubs provide bare-minimum no-op ctors/dtors so the motor-layer tests
//  (and any other W4 test binaries that link ht9045_globals) can link and run.
//  Replace with the full translated bodies in W6 (cprod) and W3/W5 (cpublic).
//
//  NOTE: Do NOT include cmydef.h / cpublic.h / cprod.h here -- those headers
//  pull in global state that requires the full translated stack.  Use only
//  forward declarations.
// =============================================================================
#include "vclcompat/vcl_compat.h"   // AnsiString for TStringList member

// ---------------------------------------------------------------------------
//  Forward declarations (replicate the minimum layout from cpublic.h / cprod.h)
// ---------------------------------------------------------------------------

// --- TMyQueue10 ---
// cpublic.h:241  typedef struct TMyQueue10 { ... TMyQueue10(); ... }
struct TMyQueue10_stub;

// --- TMyStrQueue100 ---
struct TMyStrQueue100_stub;

// --- TMyTimerQueue100 ---
struct TMyTimerQueue100_stub;

// ---------------------------------------------------------------------------
//  Include the real headers ONLY for the type we need to define ctors for.
//  cpublic.h is safe to include partially (it only depends on vcl_compat +
//  windows.h at the header level, no function-body coupling).
// ---------------------------------------------------------------------------
#include "cpublic.h"    // TMyQueue10 / TMyStrQueue100 / TMyTimerQueue100 types
#include "cprod.h"      // RUN_INFO type

// ---------------------------------------------------------------------------
//  TMyQueue10 ctor / dtor stubs
// ---------------------------------------------------------------------------
TMyQueue10::TMyQueue10()
{
    Alias  = "";
    bInt   = true;
    iIndex = -1;
    iCount = -1;
    for (int i = 0; i < MAX_Q_10; i++)
    {
        iData[i]   = 0;
        dData[i]   = 0.0;
        DateTime[i] = "";
    }
}

// ---------------------------------------------------------------------------
//  TMyStrQueue100 ctor stub
// ---------------------------------------------------------------------------
TMyStrQueue100::TMyStrQueue100()
{
    sLastFileName = "";
    iIndex = 0;
    iCount = 0;
    for (int i = 0; i < MAX_Q_100; i++)
    {
        sData1[i]   = "";
        sData2[i]   = "";
        DateTime[i] = "";
    }
}

// ---------------------------------------------------------------------------
//  TMyTimerQueue100 ctor stub
// ---------------------------------------------------------------------------
TMyTimerQueue100::TMyTimerQueue100()
{
    sLastFileName = "";
    iIndex = 0;
    iCount = 0;
    for (int i = 0; i < MAX_Q_100; i++)
    {
        sStartTime[i] = "";
        sEndTime[i]   = "";
        DateTime[i]   = "";
        sTimeStr[i]   = "";
        dElapsedTime[i] = 0.0;
    }
}

// ---------------------------------------------------------------------------
//  RUN_INFO ctor / dtor stubs
//  Only the two pointer members need to be initialized to avoid UB on delete.
//  All other fields (numeric/string) zero-initialize via value-init or default
//  AnsiString ctor.  InitialData() body is in the W6-gated block.
// ---------------------------------------------------------------------------
RUN_INFO::RUN_INFO()
{
    slEventLogFile = new TStringList();
    slExe          = new TStringList();
    // All other members: AnsiString defaults to "", numerics to 0, bools to false.
    // No InitialData() call -- that body is in the W6-gated block.
}

RUN_INFO::~RUN_INFO()
{
    if (slEventLogFile) { slEventLogFile->Clear(); delete slEventLogFile; slEventLogFile = NULL; }
    if (slExe)          { slExe->Clear();          delete slExe;          slExe = NULL; }
}
