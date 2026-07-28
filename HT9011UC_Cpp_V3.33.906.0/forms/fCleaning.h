// =============================================================================
//  forms/fCleaning.h  --  non-VCL stand-in for golden's fCleaning form pointer
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.h by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Content moved VERBATIM apart
//  from the F0-e virtual destructor; TfLotInfoEdit now comes from
//  forms/FormWidgets.h.  Facade-wide contract: see forms/fMain.h.
// =============================================================================
#ifndef FORMS_FCLEANING_H
#define FORMS_FCLEANING_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  W906-AutoCleanFoundation ADD (20260721): TfCleaning -- non-VCL stand-in
//  (golden AutoClean/uCleaning.h `class TfCleaning : public TForm`). Golden's
//  real class backs the 2,916-line AutoClean settings-panel VCL form
//  (uCleaning.cpp) -- entirely OUT OF SCOPE this wave, per the task brief;
//  this is ONLY a minimal facade stand-in for the handful of members the
//  translated AutoClean/AutoClean.cpp (+ this wave's Part D free functions)
//  actually deref, same "form as junk drawer" idiom as TfMain/TfLotInfo/
//  TfSortCT above.
// ===========================================================================
class TfCleaning
{
public:
    TfLotInfoEdit *edCleaningCount;   // [DATA] golden uCleaning.h:69 (TEdit*); SearchCleanNum (next-wave)
    int  iDeviceCount;                 // [DATA] golden uCleaning.h -- offline default 0
    bool bResetCleanCount;             // [DATA] golden uCleaning.h -- offline default false
    bool b1x2SiteAbClosePutDummy;      // [DATA] golden uCleaning.h -- ACTIVE: SetShuttleIcForSpecialMode (AutoClean.cpp) + CleanPadCountCanSupport2Arm
    TfCleaning();
    virtual ~TfCleaning() {}
};
extern TfCleaning *fCleaning;

#endif // FORMS_FCLEANING_H
