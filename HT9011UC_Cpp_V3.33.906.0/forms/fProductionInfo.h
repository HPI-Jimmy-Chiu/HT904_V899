// =============================================================================
//  forms/fProductionInfo.h  --  non-VCL stand-in for golden's fProductionInfo
//                               form pointer
//
//  AI(W906-W7-L1-Wave0) 20260801: NEW FILE.  First real home for fProductionInfo
//  (golden ProductionInfo/ProductionInfo.h:23 `class TfProductionInfo : public
//  TForm`, global at :572 `extern PACKAGE TfProductionInfo *fProductionInfo;`).
//
//  IT MERGES TWO INDEPENDENT REQUESTS FOR ONE GOLDEN OBJECT: the Loader recon
//  asked for a new forms/ file carrying sLoadMO_TestFlow while the Auto recon
//  independently asked for a TU-LOCAL no-op macro for CalTrayICCount -- two
//  mechanisms for the same object.  One file, both members.
//
//  WHERE THEY ARE DEREFERENCED IN GOLDEN (re-derived this pass):
//    * sLoadMO_TestFlow -- asendic_Loader.cpp:1880 (`.SubString(1,2)!="RT"`).
//    * CalTrayICCount   -- asendic_Auto.cpp:405, :455, :1325, :1405, :2114,
//      :2363.  Several of those are MAIN-PATH, so the method is mandatory, not
//      optional.
//
//  PRE-EXISTING STATE THIS FILE DOES *NOT* TOUCH, RECORDED SO IT IS NOT MISSED.
//  Before this file existed, the symbol `fProductionInfo` had NO definition
//  anywhere in the ported tree, yet two out-arm variants declare their own
//  differently-typed extern for it and dereference it on live code paths:
//    * aoutarm9045_1x4_4_Back.cpp:180-181 (`TfProductionInfo_1x4_4_Back_stub`),
//      used at :2594;
//    * aoutarm9045_2x2_4_14.cpp:181-182   (`TfProductionInfo_2x2_4_14_stub`),
//      used at :2618.
//  Those objects simply are not pulled out of libht9045_sm.a by any current test
//  executable, which is why the missing definition has never surfaced as an
//  undefined reference.  Defining the global here can only IMPROVE that (an
//  undefined symbol becomes defined); it cannot duplicate anything.  Their
//  declarations still name a different TYPE for the same symbol, which is a
//  pre-existing ODR problem in files owned by W7-A2 -- repointing them is A2's
//  call, not this pass's, and their stub CalTrayICCount bodies are inline no-ops
//  that never touch the object, so nothing breaks meanwhile.
//
//  ALSO NOT ADDED: ClickPause() (golden ProductionInfo.h:464).  forms/fMain.h's
//  BtnPauseClick comment records golden's `#ifndef SOFT_SIMULTE
//  fProductionInfo->ClickPause()` second line as an untranslated gap.  That gap
//  belongs to whichever wave translates BtnPauseClick's second line; adding the
//  method here without a caller would be inventing surface.
//
//  Facade-wide contract: see forms/fMain.h.
// =============================================================================
#ifndef FORMS_FPRODUCTIONINFO_H
#define FORMS_FPRODUCTIONINFO_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  TfProductionInfo -- non-VCL stub (golden ProductionInfo/ProductionInfo.h)
// ===========================================================================
class TfProductionInfo
{
public:
    // Concrete AnsiString storage (facade contract rule 2).  Offline "" means
    // golden asendic_Loader.cpp:1880's `SubString(1,2)!="RT"` is TRUE, i.e. the
    // handler is treated as not running an RT-mode MO -- the faithful "no MO
    // downloaded yet" state for an offline handler.
    AnsiString sLoadMO_TestFlow;                    // [DATA]   golden ProductionInfo.h:281
    // Offline no-op: the real body updates the production-info form's per-tray IC
    // count display, which has no headless equivalent.
    virtual void CalTrayICCount(int iWitchTray);    // [METHOD] golden ProductionInfo.h:377 -- offline: no-op
                                    //   (parameter spelling `iWitchTray` is golden's own, kept verbatim)
    TfProductionInfo();
    virtual ~TfProductionInfo() {}
};
extern TfProductionInfo *fProductionInfo;   // golden: extern PACKAGE TfProductionInfo *fProductionInfo; (:572)

#endif // FORMS_FPRODUCTIONINFO_H
