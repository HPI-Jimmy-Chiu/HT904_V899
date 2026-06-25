// =============================================================================
//  FormsFacade.cpp  --  definitions for the non-VCL form-pointer facade
//
//  Translation wave: W6.0 (decoupling scaffold for the W6.1 Empty-tray canary)
//  Translator: AI(W6.0-SCAFFOLD) 20260626
//
//  Provides the single satellite the W6.1 canary derefs: fAGV->IsATK_AMR().
//  fAGV is constructed once at load (new TfAGV); IsATK_AMR() returns false in
//  the offline sim (no ATK AMR attached -> DUMMY non-AMR branch).
// =============================================================================
#include "FormsFacade.h"

TfAGV *fAGV = new TfAGV();

bool TfAGV::IsATK_AMR()
{
    // Offline sim: no ATK AMR is connected.  TODO(W6.x/W7): wire to the real
    // AMR-mode state when the AGV/AMR subsystem is translated.
    return false;
}
