// =============================================================================
//  forms/fTrayForm.cpp  --  definitions for the fTrayForm facade
//
//  AI(W906-W7-L1-Wave0) 20260801: NEW FILE.  See forms/fTrayForm.h for the
//  golden provenance and the offline-default rationale.
// =============================================================================
#include "forms/fTrayForm.h"

TfTrayForm::TfTrayForm() : asErrorMsg("") {}
bool TfTrayForm::IsEnableColorSensor() { return false; }   // offline: no MU-N colour sensor installed
TfTrayForm *fTrayForm = new TfTrayForm();
