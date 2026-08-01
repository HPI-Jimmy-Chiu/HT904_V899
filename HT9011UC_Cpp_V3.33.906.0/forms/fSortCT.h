// =============================================================================
//  forms/fSortCT.h  --  non-VCL stand-in for golden's fSortCT form pointer
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.h by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Content moved VERBATIM apart
//  from the F0-e virtual destructor; TfSortCTPanel is now a typedef onto
//  vclcompat::TPanel (see forms/FormWidgets.h).  Facade-wide contract: see
//  forms/fMain.h.
// =============================================================================
#ifndef FORMS_FSORTCT_H
#define FORMS_FSORTCT_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  TfSortCT -- non-VCL stub (golden cSortCT.h).  pnlHP1/pnlHP2 are TPanel* in
//  the golden; the leaves only assign ->Caption an AnsiString (HowManyIC()).
//  `fSortCT->pnlHP1->Caption=...` compiles against the unified TPanel stand-in.
//  Used inside `if(CosFunction.bShowHPICCount)` (default false).
// ===========================================================================
class TfSortCT
{
public:
    TfSortCTPanel *pnlHP1;
    TfSortCTPanel *pnlHP2;
    // -- W6.3 ADD: tray panels the TRAY-ARM ENGINE (acatchtray.cpp) derefs -------
    //    golden cSortCT.h TPanel* members; the catchtray engine only assigns
    //    ->Caption (AnsiString; HowManyIC() returns int -> AnsiString int-ctor).
    TfSortCTPanel *pnlLoad;                       // [DATA] golden cSortCT.h:59  (loader IC-count panel)
    TfSortCTPanel *pnlLoadCID;                    // [DATA] golden cSortCT.h:265 (loader cover-ID panel)
    TfSortCTPanel *pnlCoverTrayD;                 // [DATA] golden cSortCT.h:280 (color cover-tray-ID panel)
    TfSortCTPanel *pnlTrayCnt[6];                 // [DATA] golden cSortCT.h:337 (per-Auto tray-count panels)
    // AI(W906-W7-L1-Wave0) 20260801: golden cSortCT.h:338 `TPanel
    // *pnlTrayID[MAX_AUTO_TRAY];` -- the per-Auto tray-ID caption panels.
    // asendic_Auto.cpp:1156 reads `fSortCT->pnlTrayID[Pos]->Caption` (read-only in
    // this family), so the ctor must allocate all 6.
    // DIMENSION: golden's MAX_AUTO_TRAY is 6 (golden MachineType.h:396, read this
    // pass).  Written as the literal 6 rather than the macro because forms/
    // headers include only forms/FormWidgets.h -> vclcompat/{vcl_compat,Controls,
    // StringGrid}.h and MAX_AUTO_TRAY is not visible there -- the same convention
    // the sibling pnlTrayCnt[6] one line up already follows.
    TfSortCTPanel *pnlTrayID[6];                  // [DATA] golden cSortCT.h:338 (TPanel*[MAX_AUTO_TRAY==6])
    TfSortCT();
    virtual ~TfSortCT() {}
};
extern TfSortCT *fSortCT;

#endif // FORMS_FSORTCT_H
