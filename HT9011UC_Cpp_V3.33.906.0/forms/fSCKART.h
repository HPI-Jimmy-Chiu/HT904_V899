// =============================================================================
//  forms/fSCKART.h  --  non-VCL stand-in for golden's fSCKART form pointer
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.h by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Content moved VERBATIM apart
//  from the F0-e virtualisation; TfSortCTPanel / TfLotInfoEdit now come from
//  forms/FormWidgets.h.  Facade-wide contract: see forms/fMain.h.
//
//  RECONCILIATION DEBT THIS FILE IS PART OF -- read before extending it.
//  AI(W906-W7-F2fix) 20260729: COUNT CORRECTED FROM FOUR TO FIVE.  This banner
//  used to say "FOUR places ... Do NOT invent a fifth" while listing
//  csystem.cpp's TWO independent seam structs as a single bullet; plan SS3-C4
//  also says "four" but its four OMIT this file's TfSCKART.  Neither four is the
//  union.  Re-derived this pass by grepping every declaration of the overlapping
//  golden-TfSCKART field names across the whole ported tree: golden has exactly
//  ONE fSCKART object; the ported tree spreads its state over FIVE declaration
//  sites, and the "fifth" the old banner forbade inventing already existed when
//  it was written.
//    1. this TfSCKART (the real global `fSCKART`),
//    2. Automation/SCK_ART.h's `SckArtState`,
//    3. Automation/SCK_ART_Remainder.h's `SckArtRemainderState`
//         -- since W7-F2 this one `: public SckArtState`, so it is the ONLY site
//            that no longer duplicates the 9 overlapping fields,
//    4. csystem.cpp's `W7C1_TfSCKARTSeam`  (shadow fields + its own embedded
//         `SckArtState core`),
//    5. csystem.cpp's `W7C2_TfSCKARTSeam`  (ditto).
//  Because #4 and #5 each EMBED a SckArtState, the number of live copies of
//  those 9 fields at runtime is higher still than five.
//  Do NOT invent a SIXTH.  Plan SS6-F2 also warns these may already have
//  diverged BEHAVIOURALLY, so the merge is analysis work, not mechanical
//  aliasing -- and any divergence found must be reported, not quietly
//  reconciled.  Two such divergences ARE now on record and pinned: see
//  Automation/SCK_ART_Remainder.h's "WHY A SEPARATE SckArtRemainderState" block
//  for the full enumeration, and tests/test_w7_f2_sckart_state.cpp for the pins.
// =============================================================================
#ifndef FORMS_FSCKART_H
#define FORMS_FSCKART_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  TfSCKART -- non-VCL stub (golden Automation/SCK_ART.h).  iInputJamCnt is
//  only incremented (inside `if(CosFunction.bUseSCKART)`, default false).
// ===========================================================================
class TfSCKART
{
public:
    int iInputJamCnt;                              // [DATA] golden SCK_ART.h:288
    // -- W6.3 ADD: members the TRAY-ARM ENGINE (acatchtray.cpp) derefs -----------
    int iFTRTCount;                                // [DATA] golden SCK_ART.h (FT/RT count; CC_TERAPOWER ART)
    int iInputCount;                               // [DATA] golden SCK_ART.h (ART input count)
    virtual int  CheckLoadingCount();              // [METHOD] golden SCK_ART.h -- offline: 0 (no ART loading mismatch; golden compares ==0/==1/==2)
    // -- W6.5 ADD: member the SHUTTLE ENGINE (acarry.cpp) derefs ----------------
    virtual void AddOutputJamCnt(int row, int col, int ret, int iBinOnCarryKit=0);  // [METHOD] golden Automation/SCK_ART.h:302 -- offline: no-op (inside if(bUseSCKART), default false)
    // -- W5-Automation INTEGRATE ADD: members Automation/AMR.cpp + HANA_ART.cpp derefs --
    int  iCurrent93KARTStep;                       // [DATA] golden SCK_ART.h:263 -- needed by both AMR.cpp and HANA_ART.cpp
    AnsiString sLotID;                              // [DATA] golden SCK_ART.h:237
    AnsiString sProcessCode;                        // [DATA] golden SCK_ART.h:238
    virtual void DoARTLotStart(AnsiString _sLotID, AnsiString _sProcessCode, int _iLotCount);  // [METHOD] golden SCK_ART.h:347 -- offline no-op
    virtual void AccessFile(bool bRead, int iAccess=-1);    // [METHOD] golden SCK_ART.h:271 -- offline no-op
    TfSortCTPanel *palLotNumber;                    // [DATA] golden SCK_ART.h:71  (TPanel* lot-number caption)
    TfSortCTPanel *palTestCnt;                      // [DATA] golden SCK_ART.h:70  (TPanel* FT/RT test-count caption)
    TfSortCTPanel *palRTTryCnt;                     // [DATA] golden SCK_ART.h:75  (TPanel* RT-try-count caption)
    TfSortCTPanel *pnlProcessCode;                  // [DATA] golden SCK_ART.h:181 (TPanel* process-code caption)
    TfLotInfoEdit *edlRTTryCnt;                     // [DATA] golden SCK_ART.h:78  (TEdit* RT-try-count edit)
    TfSCKART();
    virtual ~TfSCKART() {}
};
extern TfSCKART *fSCKART;

#endif // FORMS_FSCKART_H
