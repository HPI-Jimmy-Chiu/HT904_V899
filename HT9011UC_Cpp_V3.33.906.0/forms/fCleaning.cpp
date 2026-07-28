// =============================================================================
//  forms/fCleaning.cpp  --  definitions for the fCleaning facade
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.cpp by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Body moved VERBATIM.
// =============================================================================
#include "forms/fCleaning.h"

// --- W906-AutoCleanFoundation ADD: TfCleaning -------------------------------
TfCleaning::TfCleaning() : iDeviceCount(0), bResetCleanCount(false), b1x2SiteAbClosePutDummy(false)
{
    edCleaningCount = new TfLotInfoEdit();
}
TfCleaning *fCleaning = new TfCleaning();
