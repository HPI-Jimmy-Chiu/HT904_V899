// =============================================================================
//  Motor/vendor_offline_galil.cpp
//    OFFLINE LINK LAYER for the Galil DMC32 motion-controller SDK.
//
//  AI(W906-PT-W4-integrate) 20260809.  NOT a translation of any golden unit -- a
//  build artefact, sibling to Motor/vendor_offline_motionnet.cpp (read that file's
//  banner first; the reasoning is the same and is only summarised here).
//
//  WHY IT EXISTS
//  PT-W4 landed Motor/myGALILmotor.cpp, the one unit in the tree that talks to a
//  Galil DMC-series controller.  Golden funnels every controller access through
//  seven DMC32 entry points, and the port keeps those calls ACTIVE -- most of them
//  sit under `#ifndef SOFT_SIMULTE`, and this build does NOT define SOFT_SIMULTE,
//  so they compile in.  There is no Galil import library usable from MinGW, so
//  without this file the archive carries seven unresolved symbols that would stay
//  invisible until something first referenced myGALILmotor.cpp -- exactly the trap
//  that let 173 MotionNet symbols hide for several waves (see the sibling file).
//
//  THE SET IS MEASURED, NOT GUESSED
//    grep -oE '\bDMC[A-Za-z0-9_]*[[:space:]]*\(' Motor/myGALILmotor.cpp | sort -u
//  gives exactly these seven, and myGALILmotor.cpp's own banner (:382-388) lists
//  the same seven with their DMCCOM.H line numbers.  No eighth, no vendor symbol
//  from any other Galil header (dmcdrc.h / dmcdrco.h / DMCMLIB.H are included by
//  the vendor umbrella but nothing in the port calls into them).
//
//  RETURN SEMANTICS -- "there is no controller", in the vendor's own vocabulary
//  Every body returns DMCERROR_CONTROLLER (DMCCOM.H:134 == -3), which the SDK
//  documents as the controller not responding.  Deliberately NOT DMCNOERROR (0):
//  golden checks `if(rc!=0)` at essentially every call site (e.g.
//  myGALILmotor.cpp:1357, :3676, and the `return (DMCCommand(...)==0)` idiom at
//  :1078), so returning success would tell the ported index-arm state machines that
//  a four-axis vector move completed when no hardware exists, and they would then
//  trust positions and motion-done flags that never came from a controller.
//  Returning the failure code puts them on the path golden already takes on a
//  machine where the controller is absent -- behaviour golden handles everywhere,
//  with retries and SoftStop, rather than behaviour invented here.
//
//  OUT-PARAMETERS ARE LEFT UNTOUCHED, for the same reason as the sibling file: some
//  of these pointers are inputs (DMCCommand's `PSZ pszCommand`), and a blanket
//  memset would corrupt caller data.  A caller that reads pchResponse after a
//  failure return was already wrong against the real DLL.
//
//  NO MACRO LEVER IS NEEDED HERE, unlike MN200 (which required MN200DLL_EXPORTS to
//  turn dllimport into dllexport).  DMCCOM.H declares these inside `extern "C"`
//  with `GALILCALL` == plain `__stdcall` (DMCCOM.H:8/:13/:46) and no
//  __declspec(dllimport) anywhere, so callers already emit direct `_DMCxxx@N`
//  references that a static definition satisfies.  Checked before writing this.
//
//  SIGNATURES ARE NOT HAND-TYPED: each parameter list is copied verbatim from
//  DMCCOM.H, so the compiler validates all seven signatures and the linker
//  validates all seven __stdcall @N decorations.  DMCCOM.H is on AGENTS.md's
//  forbidden-to-modify list; it is included, never edited.
//
//  RETIREMENT: when a real Galil SDK is linked, delete this file and link the
//  vendor library.  No consumer knows this layer exists.
// =============================================================================
#include "MachineDefine.h"

#include <windows.h>                 // HWND, LONG, USHORT, ULONG -- DMCCOM.H needs them
#include "Motor/vendor/DMCCOM.H"     // READ-ONLY vendor header: HANDLEDMC/PHANDLEDMC/PSZ/
                                     // LPCHAR, GALILCALL, DMCERROR_CONTROLLER (:134)

// myGALILmotor.cpp defines a self-referential function-like macro
//   #define DMCCommand(h, cmd, resp, cb) DMCCommand((h), (PSZ)(cmd), (resp), (cb))
// to insert the one PSZ cast at its 30 golden call sites.  That macro is TU-local to
// myGALILmotor.cpp and is NOT in effect here -- this file defines the real function,
// so the definition below must be, and is, the plain vendor signature.

LONG FAR GALILCALL DMCOpen(USHORT usController, HWND hwnd, PHANDLEDMC phdmc) { return DMCERROR_CONTROLLER; }
LONG FAR GALILCALL DMCClose(HANDLEDMC hdmc) { return DMCERROR_CONTROLLER; }
LONG FAR GALILCALL DMCCommand(HANDLEDMC hdmc, PSZ pszCommand, LPCHAR pchResponse, ULONG cbResponse) { return DMCERROR_CONTROLLER; }
LONG FAR GALILCALL DMCClear(HANDLEDMC hdmc) { return DMCERROR_CONTROLLER; }
LONG FAR GALILCALL DMCReset(HANDLEDMC hdmc) { return DMCERROR_CONTROLLER; }
LONG FAR GALILCALL DMCDiagnosticsOff(HANDLEDMC hdmc) { return DMCERROR_CONTROLLER; }
LONG FAR GALILCALL DMCGetControllerDesc(USHORT usController, PSZ pszControllerDesc, ULONG cbControllerDesc) { return DMCERROR_CONTROLLER; }

// -- end of the 7-entry offline Galil layer -----------------------------------
