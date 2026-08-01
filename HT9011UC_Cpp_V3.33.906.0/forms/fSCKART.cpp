// =============================================================================
//  forms/fSCKART.cpp  --  definitions for the fSCKART facade
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.cpp by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Bodies moved VERBATIM.
// =============================================================================
#include "forms/fSCKART.h"
// AI(W906-W7-L1-Wave0) 20260801: cmydef.h pulls in CUSTOMER_CODE (cmydef.h:3181,
// `extern int`) and MachineType.h's CC_SCK, both needed by the golden-faithful
// iTesterType initialisation below.  Same cross-library shape forms/fMain.cpp
// already uses: ht9045_forms PUBLIC-links ht9045_globals (root CMakeLists.txt),
// which is where CUSTOMER_CODE is defined (cmydef.cpp:3412).
#include "cmydef.h"

// --- W6.2: TfSCKART --------------------------------------------------------
TfSCKART::TfSCKART() : iInputJamCnt(0), iFTRTCount(0), iInputCount(0), iCurrent93KARTStep(0)
{
    // -- W5-Automation ADD --
    palLotNumber   = new TfSortCTPanel();
    palTestCnt     = new TfSortCTPanel();
    palRTTryCnt    = new TfSortCTPanel();
    pnlProcessCode = new TfSortCTPanel();
    edlRTTryCnt    = new TfLotInfoEdit();
    // -- AI(W906-W7-L1-Wave0) 20260801 ADD: the 7 W7-L1 golden fields + 2 seams --
    iBundleInCnt   = 0;                 // golden SCK_ART.h:343 (golden TForm int member, zero-init)
    iBundleOutCnt  = 0;                 // golden SCK_ART.h:344 (ditto)
    sBundleList    = "";                // golden SCK_ART.h:342 (golden AnsiString member, empty)
    bFirstFullSkip = false;             // golden SCK_ART.h:351 (golden TForm bool member, zero-init)
    iNeedRT        = 0;                 // golden SCK_ART.h:285 (golden TForm int member, zero-init)
    iLOTSTATUS_W   = 1;                 // golden SCK_ART.cpp:44 -- explicit `iLOTSTATUS_W=1;`
    // iTesterType: golden's value is produced by TWO ctor regions, not one --
    // SCK_ART.cpp:42 `iTesterType=0;` and then :102-114
    // `if(CUSTOMER_CODE==CC_SCK){...widgets...} else { iTesterType=1; }`.
    // Reproduced verbatim as the conditional below so the value is right for
    // whatever CUSTOMER_CODE is in force when a TfSCKART is constructed.
    //
    // STATIC-INITIALISATION CAVEAT, stated so nobody over-reads the expression:
    // the GLOBAL `fSCKART` below is constructed during dynamic initialisation,
    // and CUSTOMER_CODE (cmydef.cpp:3412 `int CUSTOMER_CODE=0;`) is
    // constant-initialised, so it is guaranteed to read 0 -- never CC_SCK (947) --
    // at that moment.  The global therefore always ends up with 1, which IS
    // golden's constructed value for every customer except CC_SCK.  The
    // conditional still earns its place: a test that sets CUSTOMER_CODE and then
    // constructs its own TfSCKART gets golden's real behaviour on both arms.
    iTesterType    = (CUSTOMER_CODE==CC_SCK) ? 0 : 1;   // golden SCK_ART.cpp:42 + :102-114
    W906_SetLotStatus_LastArg = -1;     // PORT-ONLY SEAM: -1 == never called
    W906_SetLotStatus_Count   = 0;      // PORT-ONLY SEAM
}
int  TfSCKART::CheckLoadingCount() { return 0; }              // W7: offline -> 0 (no ART loading mismatch)
void TfSCKART::AddOutputJamCnt(int /*row*/, int /*col*/, int /*ret*/, int /*iBinOnCarryKit*/) {}  // W6.5: offline no-op
// -- W5-Automation ADD: AMR.cpp + HANA_ART.cpp method sinks (all offline no-op) --
void TfSCKART::DoARTLotStart(AnsiString /*_sLotID*/, AnsiString /*_sProcessCode*/, int /*_iLotCount*/) {}
void TfSCKART::AccessFile(bool /*bRead*/, int /*iAccess*/) {}
// -- AI(W906-W7-L1-Wave0) 20260801 ADD: SetLotStatus (golden SCK_ART.h:269, body
//    SCK_ART.cpp:639-667).  Offline this records into the two seams instead of
//    being an unobservable sink (facade contract rule 4).  golden's sLOTSTATUS /
//    iCurrentStatus / cbLotStatus->ItemIndex writes have no facade home and no
//    reader in this tree -- see forms/fSCKART.h for that documented gap.
void TfSCKART::SetLotStatus(int iStatus)
{
    W906_SetLotStatus_LastArg = iStatus;
    W906_SetLotStatus_Count++;
}
TfSCKART *fSCKART = new TfSCKART();
