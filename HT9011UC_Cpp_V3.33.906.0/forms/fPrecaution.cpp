// =============================================================================
//  forms/fPrecaution.cpp  --  definitions for the fPrecaution facade
//
//  AI(W906-FW3-HSP1) 20260828: new file, FW wave FW3-HSP1. See forms/
//  fPrecaution.h for the full GATE REGISTER / ACTIVE evidence / DEVIATIONS.
// =============================================================================
#include "forms/fPrecaution.h"

// AI(W906-FW3-HSP1) 20260828: TfPrecaution/fPrecaution were FREE tree-wide --
// same idiom as forms/fCleaning.cpp:45. Ctor is a plain field-only body
// (golden :14-17 is empty past `: TForm(Owner)`), so this static-init `new`
// touches nothing beyond its own already-`new`'d widget stand-ins -- no SIOF
// risk (docs/KNOWLEDGE.md "static-init ctor 不可碰 NULL 全域").
TfPrecaution *fPrecaution = new TfPrecaution();

TfPrecaution::TfPrecaution()
{
    // golden :14-17 -- empty ctor body past `: TForm(Owner)`. Widget fields
    // are NSDMI-`new`'d in the header.
}

// ---------------------------------------------------------------------------
void TfPrecaution::FormCreate(TObject *Sender)   // golden :27-30
{
    (void)Sender;
    fShow = false;
}

// ---------------------------------------------------------------------------
void TfPrecaution::tm_CheckEditEmptyTimer(TObject *Sender)   // golden :45-74
{
    (void)Sender;
    bool bEmpty[10] = {true, true, true, true, true, true, true, true, true, true};
    if (fShow == false)
        return;

    bEmpty[0] = pnPrecautionRecordDocumentNo->Caption.IsEmpty();
    bEmpty[1] = pnNoteContents->Caption.IsEmpty();
    bEmpty[2] = pnApprovedManager->Caption.IsEmpty();
    bEmpty[3] = pnPrecautionStartTime->Caption.IsEmpty();
    bEmpty[4] = pnCloseType->Caption.IsEmpty();
    bEmpty[5] = pnWatchmakers->Caption.IsEmpty();
    // bEmpty[6] = edPromptDay->Text.IsEmpty();   // golden :57 -- commented out in golden itself, kept verbatim

    bEmpty[7] = pnCloseName->Caption.IsEmpty();
    bEmpty[8] = pnPrecautionEndTime->Caption.IsEmpty();

    if (MemoHandlerPrecautionRecord->Lines->Count == 0 ||
        bEmpty[0] == true ||
        bEmpty[1] == true ||
        bEmpty[2] == true ||
        bEmpty[3] == true ||
        bEmpty[4] == true ||
        bEmpty[5] == true
        // || bEmpty[6] == true   // golden :69 -- commented out in golden itself, kept verbatim
       )
    {
        Close();   // DEVIATION D-2 -- port-only no-op
    }
}

// -- GATE REGISTER -- see forms/fPrecaution.h for the full reasoning.
// FormShow / DoIniDataToForm / FormClose are declared in forms/fPrecaution.h
// and intentionally have NO definition here -- calling any of them is a
// LINK ERROR by design (same idiom as forms/fMesSystem.cpp's GATED bucket).
