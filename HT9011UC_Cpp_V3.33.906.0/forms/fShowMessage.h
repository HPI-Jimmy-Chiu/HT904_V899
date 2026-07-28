// =============================================================================
//  forms/fShowMessage.h  --  non-VCL stand-in for golden's fShowMessage pointer
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.h by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Content moved VERBATIM apart
//  from the F0-e virtualisation.  See forms/fMain.h for the facade-wide
//  contract.
// =============================================================================
#ifndef FORMS_FSHOWMESSAGE_H
#define FORMS_FSHOWMESSAGE_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  W906-AutoCleanFoundation ADD (20260721): TfShowMessage -- FIRST home for
//  fShowMessage (golden cUnitConvert.h note: "W7 VCL form pointer" -- another
//  confirmed no-home global, same class of gap as TfNote just above). Needed
//  by CleanSetSpeed's (in-scope) tail call `fShowMessage->ShowSpeed(bool)`.
//  Even smaller than TfNote: one method, zero data, offline no-op (a debug
//  "speed values changed" popup that never shows without a UI).
// ===========================================================================
class TfShowMessage
{
public:
    virtual void ShowSpeed(bool bShow);   // [METHOD] golden cUnitConvert.h -- offline: no-op (no UI to show a debug popup on)
    virtual ~TfShowMessage() {}
};
extern TfShowMessage *fShowMessage;

#endif // FORMS_FSHOWMESSAGE_H
