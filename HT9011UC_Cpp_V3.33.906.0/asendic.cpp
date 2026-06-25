// =============================================================================
//  asendic.cpp  --  SIM bodies for the tray-move / cylinder-lifter helpers
//
//  Mirrors (SIM-SHAPE) of: HT9011UC_Code_V3.33.906.0_20260618/asendic.cpp
//                          (CylinderUp/Middle/Lower :150/285/397,
//                           TrayMoveIn/Out :1268/1367, TrayCylinMoveIn/Out :1349/
//                           1358, TrayMoveStatus :1449).
//  Translation wave: W6.0 (canary-support scaffold for asendic_Empty)
//  Translator: AI(W6.0-SCAFFOLD) 20260626
//
//  WHY SIM-SHAPE (not a verbatim port)
//  -----------------------------------
//  The golden CylinderUp/Middle/Lower are FULL lifter state machines (the
//  Loader/Empty/Color "Tray Group GoUp/GoDown" mechanism) keyed on globals that
//  are NOT yet in the target: iLifterTask[][], LifterTime[][], iTrayZMotor[],
//  LOAD_Z_USE_MOTOR[], Prod.TrayZ_Up[].  A verbatim port would drag the whole
//  lifter god-stack into the W6.1 canary build.
//
//  Per the W6.1 plan these helpers ship as "Sim bodies (manipulate Cylinder[]/
//  MOT[] + return done) or minimal stubs that advance state."  So each helper
//  here drives the SAME Cylinder[] surface the golden ultimately commands and
//  reports "motion done" so the Empty-tray SM advances:
//      CylinderUp(C_*_Up)     -> Cylinder[C_*_Up].On()    , return true (at top)
//      CylinderMiddle(C_*_Up) -> Cylinder[C_*_Middle].On(), return true (middle)
//      CylinderLower(C_*_Up)  -> Cylinder[C_*_Up].Off()
//                                + Cylinder[C_*_Middle].Off(), return true (down)
//  The Part mapping (0=Load,1=Empty,2=Color) and the C_*_Up -> C_*_Middle
//  pairing are FAITHFUL to the golden (asendic.cpp:154-168).
//  TrayMove*/TrayCylinMove* are no-op belt commands (no belt model in sim);
//  TrayMoveStatus returns 0 ("idle / arrived") so the DUMMY-path SM proceeds.
//
//  When the real lifter SM + LifterTime/iLifterTask globals are translated
//  (W6.x), replace these bodies with the verbatim golden state machines.
// =============================================================================
#include "asendic.h"
#include "mycylin.h"    // Cylinder[]
#include "cmydef.h"     // C_Load_Up/C_Load_Middle/C_Empty_Up/C_Empty_Middle/
                        // C_Color_Up/C_Color_Middle

// ---------------------------------------------------------------------------
//  Helper: map a lifter "Up" cylinder id to its paired "Middle" id.
//  Faithful to golden asendic.cpp:154-168 (CylinderUp Part/CylinderMidd setup).
// ---------------------------------------------------------------------------
static int LifterMiddleOf(int CylinderName)
{
    if(CylinderName==C_Load_Up)   return C_Load_Middle;
    if(CylinderName==C_Empty_Up)  return C_Empty_Middle;
    if(CylinderName==C_Color_Up)  return C_Color_Middle;
    return CylinderName+1;                                                       // golden default: CylinderMidd=CylinderName+1
}

// ---------------------------------------------------------------------------
//  CylinderUp -- tray group rises to the top (separation height).
//  SIM: command both lifter cylinders ON and report done.
// ---------------------------------------------------------------------------
bool CylinderUp(int CylinderName)                                               //Part 0=LD 1=Empty 2=color
{
    int CylinderMidd = LifterMiddleOf(CylinderName);
    if(Cylinder[CylinderMidd].Enable)
        Cylinder[CylinderMidd].On();
    if(Cylinder[CylinderName].Enable)
        Cylinder[CylinderName].On();
    return true;    // SIM: motion done (TODO(W6.x): golden lifter state machine)
}

// ---------------------------------------------------------------------------
//  CylinderMiddle -- tray group at the middle (split) height.
//  SIM: command the middle cylinder ON, report done.
// ---------------------------------------------------------------------------
bool CylinderMiddle(int CylinderName)
{
    int CylinderMidd = LifterMiddleOf(CylinderName);
    if(Cylinder[CylinderMidd].Enable)
        Cylinder[CylinderMidd].On();
    return true;    // SIM: motion done (TODO(W6.x): golden lifter state machine)
}

// ---------------------------------------------------------------------------
//  CylinderLower -- tray group descends to the lowest position.
//  SIM: command both lifter cylinders OFF, report done.
// ---------------------------------------------------------------------------
bool CylinderLower(int CylinderName)
{
    int CylinderMidd = LifterMiddleOf(CylinderName);
    if(Cylinder[CylinderName].Enable)
        Cylinder[CylinderName].Off();
    if(Cylinder[CylinderMidd].Enable)
        Cylinder[CylinderMidd].Off();
    return true;    // SIM: motion done (TODO(W6.x): golden lifter state machine)
}

// ---------------------------------------------------------------------------
//  TrayMoveIn / TrayMoveOut -- belt feed in/out command.
//  SIM: no belt model offline; the command is a no-op.  (TODO(W6.x): drive the
//  conveyor-belt switch surface the golden commands.)
// ---------------------------------------------------------------------------
void TrayMoveIn(bool /*bMove*/, int /*iAxis*/, int /*iPos*/)
{
}
void TrayMoveOut(bool /*bMove*/, int /*iAxis*/, int /*iPos*/)
{
}

// ---------------------------------------------------------------------------
//  TrayCylinMoveIn / TrayCylinMoveOut -- under-conveyor cylinder move.
//  SIM: no-op (eUnderCoveyor mode not modelled offline).
// ---------------------------------------------------------------------------
void TrayCylinMoveIn(int /*iAxis*/)
{
}
void TrayCylinMoveOut(int /*iAxis*/)
{
}

// ---------------------------------------------------------------------------
//  TrayMoveStatus -- query tray-move state (Sam 20210325).
//  SIM: 0 == idle / arrived, so the DUMMY-path SM proceeds without waiting on a
//  belt model.  (TODO(W6.x): return the real tray-move state.)
// ---------------------------------------------------------------------------
int TrayMoveStatus(int /*iAxis*/, AnsiString /*sFun*/)
{
    return 0;
}
