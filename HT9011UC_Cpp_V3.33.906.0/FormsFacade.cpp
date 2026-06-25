// =============================================================================
//  FormsFacade.cpp  --  definitions for the non-VCL form-pointer facade
//
//  Translation wave: W6.0 (scaffold) ; EXTENDED W6.2 (in-arm HP geometry)
//  Translator: AI(W6.0-SCAFFOLD) 20260626 ; AI(W6.2-INARM) 20260626
//
//  W6.1: fAGV->IsATK_AMR() (false offline).
//  W6.2: fMain / fSortCT / fLotInfo / fOffSet / fSCKART -- all offline no-op /
//        false, constructed once at load.  See FormsFacade.h for the member-by-
//        member documentation + golden homes.
// =============================================================================
#include "FormsFacade.h"

// --- W6.1 ------------------------------------------------------------------
TfAGV *fAGV = new TfAGV();

bool TfAGV::IsATK_AMR()
{
    // Offline sim: no ATK AMR is connected.  TODO(W6.x/W7): wire to the real
    // AMR-mode state when the AGV/AMR subsystem is translated.
    return false;
}

// --- W6.2: TfMain ----------------------------------------------------------
TfMain::TfMain()
{
    slAutoSiteMapLog = new TfMainSiteMapLog();      // golden main.h:1486 (TMyStringList*)
}
void TfMain::DebugOneCycleHotPlate(AnsiString /*sfunc*/) {}     // debug log sink (offline no-op)
bool TfMain::Pause(AnsiString /*Func*/) { return false; }      // offline never pauses
void TfMain::ShowTestHeadComp(bool /*bRefresh*/) {}
void TfMain::ReStartAutoSiteMapping(bool /*bStart*/) {}
TfMain *fMain = new TfMain();

// --- W6.2: TfSortCT --------------------------------------------------------
TfSortCT::TfSortCT()
{
    pnlHP1 = new TfSortCTPanel();
    pnlHP2 = new TfSortCTPanel();
}
TfSortCT *fSortCT = new TfSortCT();

// --- W6.2: TfLotInfo -------------------------------------------------------
TfLotInfo::TfLotInfo()
{
    cbRunMode = new TfLotInfoRunMode();             // offline: Visible=false
}
TfLotInfo *fLotInfo = new TfLotInfo();

// --- W6.2: TfOffSet --------------------------------------------------------
bool TfOffSet::UseAutoOffsetFunction(AnsiString /*sName*/) { return false; }  // no auto-offset offline
bool TfOffSet::UseInArmSetupTeach(int /*iWhich*/)          { return false; }  // no setup-teach offline
TfOffSet *fOffSet = new TfOffSet();

// --- W6.2: TfSCKART --------------------------------------------------------
TfSCKART::TfSCKART() : iInputJamCnt(0) {}
TfSCKART *fSCKART = new TfSCKART();
