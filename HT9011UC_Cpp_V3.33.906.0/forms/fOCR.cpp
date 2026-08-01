// =============================================================================
//  forms/fOCR.cpp  --  definitions for the fOCR facade
//
//  AI(W906-W7-L1-Wave0) 20260801: NEW FILE.  See forms/fOCR.h for the golden
//  provenance and the offline-default rationale.
// =============================================================================
#include "forms/fOCR.h"

// Offline there is no OCR ring light to drive, so this is a no-op sink -- the
// same shape every other hardware-side facade method in forms/ uses.
void TfOCR::ChangeLightValue(int /*Channel*/, int /*Value*/) {}
TfOCR *fOCR = new TfOCR();
