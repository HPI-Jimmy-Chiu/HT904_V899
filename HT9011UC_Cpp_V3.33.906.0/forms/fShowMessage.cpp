// =============================================================================
//  forms/fShowMessage.cpp  --  definitions for the fShowMessage facade
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.cpp by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Body moved VERBATIM.
// =============================================================================
#include "forms/fShowMessage.h"

// --- W906-AutoCleanFoundation ADD: TfShowMessage (first home) --------------
void TfShowMessage::ShowSpeed(bool /*bShow*/) {}
TfShowMessage *fShowMessage = new TfShowMessage();
