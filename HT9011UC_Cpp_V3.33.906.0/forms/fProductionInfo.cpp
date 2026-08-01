// =============================================================================
//  forms/fProductionInfo.cpp  --  definitions for the fProductionInfo facade
//
//  AI(W906-W7-L1-Wave0) 20260801: NEW FILE.  See forms/fProductionInfo.h for the
//  golden provenance, the two-requests merge, and the pre-existing out-arm
//  variant extern situation this definition resolves.
// =============================================================================
#include "forms/fProductionInfo.h"

TfProductionInfo::TfProductionInfo() : sLoadMO_TestFlow("") {}
// Offline: the real body refreshes the production-info form's per-tray IC-count
// display (golden ProductionInfo.h:377); there is no headless equivalent, so this
// is a no-op sink -- the same idiom every other UI-refresh facade method uses.
void TfProductionInfo::CalTrayICCount(int /*iWitchTray*/) {}
TfProductionInfo *fProductionInfo = new TfProductionInfo();
