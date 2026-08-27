// =============================================================================
//  forms/fObserveMagazine.cpp  --  TfObserveMagazine member bodies
//
//  AI(W906-FW3-OWM1) 20260828: new file. See forms/fObserveMagazine.h for the
//  full wave banner, GATE-name accounting and dependency audit. All 3 golden
//  bodies (ObserveMagazine.cpp, HT9011UC_Code_V3.33.906.0_20260618, 14 span
//  lines) are ACTIVE verbatim -- nothing in this file is gated.
// =============================================================================
#include "forms/fObserveMagazine.h"

// AI(W906-FW3-OWM1) 20260828: golden ObserveMagazine.h/.cpp both free
// tree-wide (class name + global name), so this facade declares the global --
// same idiom as forms/fCleaning.cpp:56 / forms/fMesSystem.cpp:597 /
// forms/fSmartDiagnostic.cpp:37. Safe at static-init: the ctor below touches
// only its own just-`new`'d fields (see header D-3), no other global.
TfObserveMagazine *fObserveMagazine = new TfObserveMagazine();

// golden :13-17
TfObserveMagazine::TfObserveMagazine()
{
    fShow = false;
}

// golden :19-22 (D-2: unread `TObject *Sender` dropped)
void TfObserveMagazine::FormShow()
{
    fShow = true;
}

// golden :24-28 (D-2: unread `TObject *Sender, TCloseAction &Action` dropped)
void TfObserveMagazine::FormClose()
{
    fShow = false;
}
