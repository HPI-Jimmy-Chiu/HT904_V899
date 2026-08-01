// =============================================================================
//  asendic.h  --  tray-move / cylinder-lifter free-function interface
//
//  Mirrored (SUBSET) from: HT9011UC_Code_V3.33.906.0_20260618/asendic.h
//  Translation wave: W6.0 (canary-support scaffold for asendic_Empty)
//  Translator: AI(W6.0-SCAFFOLD) 20260626
//
//  The golden asendic.h is a GOD-HEADER: it #includes the 7 sibling SM headers
//  (asendic_Loader/_Loader_RT/_Auto/_Auto_RT/_Auto2/_Empty/_Color) and declares
//  the whole tray-handling free-function surface.  Pulling those siblings would
//  drag the entire Loader/Auto/Color SM god-stack into the W6.1 canary build.
//
//  This SUBSET declares ONLY the tray-move / cylinder-lifter helpers that
//  asendic_Empty.cpp calls:
//      CylinderUp / CylinderMiddle / CylinderLower (lifter GoUp/GoMiddle/GoDown)
//      TrayMoveIn / TrayMoveOut       (belt feed in/out)
//      TrayCylinMoveIn / TrayCylinMoveOut (under-conveyor cylinder move)
//      TrayMoveStatus                 (tray-move state query)
//  plus asendic_Empty.h (the canary's own SM prototypes).
//
//  The sibling-SM god-includes + the rest of the free-function surface are kept
//  in aArmHeader.h (gated #if 0).  Signatures here are VERBATIM from the golden
//  (asendic.h:16-33).  Sim bodies live in asendic.cpp.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef asendicH
#define asendicH
//---------------------------------------------------------------------------

#include "vclcompat/vcl_compat.h"   // AnsiString
#include "asendic_Empty.h"          // the canary's own SM prototypes + task ints

// AI(W906-W7-L1-Wave0) 20260801: golden asendic.h:14 `extern int
// iTrayZLoadTrayToWaitTask;` -- DECLARATION ONLY, landed here once in the
// serialized pass so the W7-L1 agents that only READ it do not each add their own.
// The DEFINITION belongs to the Loader bundle: golden asendic_Loader.cpp:51
// `int iTrayZLoadTrayToWaitTask=1;` (note the non-zero initial value -- it is the
// SM's idle cursor, and golden :1471 resets it to 1, :1478 binds `int &Task=` to
// it).  Verified absent tree-wide before adding.  Every other file (golden
// asendic_Auto_RT.cpp writes it at :951) externs only.
extern int iTrayZLoadTrayToWaitTask;                                            // golden asendic.h:14

// --- lifter cylinder GoUp / GoMiddle / GoDown (golden asendic.h:16-18) -------
extern bool CylinderUp(int CylinderName);
extern bool CylinderMiddle(int CylinderName);
extern bool CylinderLower(int CylinderName);

// --- belt feed in/out + under-conveyor cylinder move (golden asendic.h:28-33) -
extern void TrayMoveIn(bool bMove, int iAxis, int iPos=0);
extern void TrayMoveOut(bool bMove, int iAxis, int iPos=0);
extern void TrayCylinMoveIn(int iAxis);
extern void TrayCylinMoveOut(int iAxis);
// AI(W906-W7-L1-Wave0) 20260801: NO Sim SEAM IS BEING ADDED TO TrayMoveStatus,
// and that is a decision, not an oversight -- recorded here because the gap is
// invisible otherwise.  asendic.cpp's current body is a hardwired `return 0`, so
// every caller sees "idle" forever.  Two W7-L1 recons asked for a settable
// g_SimTrayMoveStatus / TrayMoveIn-Out call-count seam; both were declined
// because the Wave-3 (asendic.cpp L1b) landing replaces this whole family with
// REAL translated bodies (golden ~62 lines for TrayMoveStatus, ~177 for the four
// TrayMove*), so a seam added now would be written twice and could leave a dead
// override behind.  CONSEQUENCE the Wave-1/2 agents must record rather than
// paper over: while `return 0` stands, DoLoaderTrayToFront re-arms both of its
// timers on every case-100 pass, which makes the JAM1012 arm and the DUMMY
// early-out structurally unreachable.  Those arms are explicitly NOT COVERED
// until Wave 3 -- do not claim coverage for them, and do not add a seam here.
extern int  TrayMoveStatus(int iAxis, AnsiString sFun);                         //Sam 20210325 : unified tray-move status

#endif
