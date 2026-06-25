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

// --- lifter cylinder GoUp / GoMiddle / GoDown (golden asendic.h:16-18) -------
extern bool CylinderUp(int CylinderName);
extern bool CylinderMiddle(int CylinderName);
extern bool CylinderLower(int CylinderName);

// --- belt feed in/out + under-conveyor cylinder move (golden asendic.h:28-33) -
extern void TrayMoveIn(bool bMove, int iAxis, int iPos=0);
extern void TrayMoveOut(bool bMove, int iAxis, int iPos=0);
extern void TrayCylinMoveIn(int iAxis);
extern void TrayCylinMoveOut(int iAxis);
extern int  TrayMoveStatus(int iAxis, AnsiString sFun);                         //Sam 20210325 : unified tray-move status

#endif
