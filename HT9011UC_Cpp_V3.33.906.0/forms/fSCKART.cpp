// =============================================================================
//  forms/fSCKART.cpp  --  definitions for the fSCKART facade
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.cpp by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Bodies moved VERBATIM.
// =============================================================================
#include "forms/fSCKART.h"

// --- W6.2: TfSCKART --------------------------------------------------------
TfSCKART::TfSCKART() : iInputJamCnt(0), iFTRTCount(0), iInputCount(0), iCurrent93KARTStep(0)
{
    // -- W5-Automation ADD --
    palLotNumber   = new TfSortCTPanel();
    palTestCnt     = new TfSortCTPanel();
    palRTTryCnt    = new TfSortCTPanel();
    pnlProcessCode = new TfSortCTPanel();
    edlRTTryCnt    = new TfLotInfoEdit();
}
int  TfSCKART::CheckLoadingCount() { return 0; }              // W7: offline -> 0 (no ART loading mismatch)
void TfSCKART::AddOutputJamCnt(int /*row*/, int /*col*/, int /*ret*/, int /*iBinOnCarryKit*/) {}  // W6.5: offline no-op
// -- W5-Automation ADD: AMR.cpp + HANA_ART.cpp method sinks (all offline no-op) --
void TfSCKART::DoARTLotStart(AnsiString /*_sLotID*/, AnsiString /*_sProcessCode*/, int /*_iLotCount*/) {}
void TfSCKART::AccessFile(bool /*bRead*/, int /*iAccess*/) {}
TfSCKART *fSCKART = new TfSCKART();
