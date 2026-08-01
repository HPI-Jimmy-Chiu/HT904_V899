// =============================================================================
//  forms/fOCR.h  --  non-VCL stand-in for golden's fOCR form pointer
//
//  AI(W906-W7-L1-Wave0) 20260801: NEW FILE.  First home for fOCR (golden OCR.h
//  `class TfOCR : public TForm`, global at golden OCR.h:331 `extern PACKAGE
//  TfOCR *fOCR;`).  Verified absent from the ported tree before writing: the only
//  `fOCR` token anywhere was a prose mention in a csystem.cpp comment (:440), no
//  class and no global -- so there is no collision.
//
//  WHY IT IS NEEDED: golden asendic_Loader.cpp calls
//  fOCR->ChangeLightValue(Channel, Value) at exactly 4 sites -- :1301, :1302,
//  :2379, :2380 (all `(1, 0)` / `(2, 0)`, i.e. both ring-light channels off).
//
//  WHY IT LIVES IN forms/ EVEN THOUGH IT IS NOT A WIDGET: golden's TfOCR is a
//  VCL TForm like every other facade member here, and the pointer spelling the
//  SMs use is `fOCR->`.  ChangeLightValue drives OCR ring-light HARDWARE through
//  that form, so this is a hardware sink with a form-pointer shape, not a W7-U
//  widget: the offline body is a no-op because there is no ring light to dim.
//
//  Facade-wide contract: see forms/fMain.h.
// =============================================================================
#ifndef FORMS_FOCR_H
#define FORMS_FOCR_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  TfOCR -- non-VCL stub (golden OCR.h)
// ===========================================================================
class TfOCR
{
public:
    virtual void ChangeLightValue(int Channel, int Value);  // [METHOD] golden OCR.h:324 -- offline: no-op
    virtual ~TfOCR() {}
};
extern TfOCR *fOCR;     // golden: extern PACKAGE TfOCR *fOCR; (OCR.h:331)

#endif // FORMS_FOCR_H
