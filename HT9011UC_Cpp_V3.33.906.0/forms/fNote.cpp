// =============================================================================
//  forms/fNote.cpp  --  definitions for the fNote facade
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.cpp by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Body moved VERBATIM.
// =============================================================================
#include "forms/fNote.h"

// --- W906-AutoCleanFoundation ADD: TfNote (first home, see forms/fNote.h) --
TfNote::TfNote() : bMyServoOffInArm(false), iMyServoOffInArmPosX(0), iMyServoOffInArmPosY(0), aJamCodeFilePath("") {}
TfNote *fNote = new TfNote();
