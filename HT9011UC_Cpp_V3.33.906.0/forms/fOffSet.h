// =============================================================================
//  forms/fOffSet.h  --  non-VCL stand-in for golden's fOffSet form pointer
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.h by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Content moved VERBATIM apart
//  from the F0-e virtualisation.  See forms/fMain.h for the facade-wide
//  contract.
// =============================================================================
#ifndef FORMS_FOFFSET_H
#define FORMS_FOFFSET_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  TfOffSet -- non-VCL stub (golden cOffSet.h).  Both methods return false
//  offline (no auto-offset / no setup-teach configured).
// ===========================================================================
class TfOffSet
{
public:
    virtual bool UseAutoOffsetFunction(AnsiString sName);  // [METHOD] golden cOffSet.h:488 -> false
    virtual bool UseInArmSetupTeach(int iWhich);           // [METHOD] golden cOffSet.h:489 -> false
    virtual ~TfOffSet() {}
};
extern TfOffSet *fOffSet;

#endif // FORMS_FOFFSET_H
