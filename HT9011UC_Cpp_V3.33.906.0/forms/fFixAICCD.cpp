// =============================================================================
//  forms/fFixAICCD.cpp  --  definitions for the fFixAICCD facade
//
//  AI(W906-W7-L1-Wave0) 20260801: NEW FILE.  The global below is deliberately a
//  REAL object, not the `= 0` that aoutarm_shims.cpp:112 used to hold: golden
//  asendic_Auto.cpp derefs fFixAICCD unconditionally on main paths (see
//  forms/fFixAICCD.h for the 7 golden line citations).
// =============================================================================
#include "forms/fFixAICCD.h"

void TfFixAICCD::OutArmCycleCounterUpdate() {}              // offline: cycle-counter UI no-op
void TfFixAICCD::bCheckUnloaderHasAiNG(int /*iMot*/) {}     // offline: no Fix-AI CCD -> no-op (golden returns void)
TfFixAICCD *fFixAICCD = new TfFixAICCD();
