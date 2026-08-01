// =============================================================================
//  forms/fTrayForm.h  --  non-VCL stand-in for golden's fTrayForm form pointer
//
//  AI(W906-W7-L1-Wave0) 20260801: NEW FILE.  First home for fTrayForm (golden
//  cTrayForm.h `class TfTrayForm : public TForm`, global declared at golden
//  cTrayForm.h:285 `extern PACKAGE TfTrayForm *fTrayForm;`) anywhere in this
//  migrated tree -- verified absent before writing: the only `fTrayForm` tokens
//  in the ported tree were (a) cprod.cpp:2978/:2983 inside GetColorSensorIsMapping,
//  whose whole body sits in the cprod.cpp `#if 0 // TODO(W6)` block spanning
//  :184-4036 and is therefore not compiled, and (b) string data in the generated
//  tools/dfm2rc/layout_out/cTrayForm_layout.gen.cpp.  No class, no global, no
//  collision.  (Note: the unrelated plain-struct global named `TrayForm` in
//  cprod.h -- TrayForm.bEnableAMR / .LoaderToEmptyColor -- is a DIFFERENT object;
//  do not conflate the two.)
//
//  WHY IT IS NEEDED: golden asendic_Loader.cpp derefs it on the loader
//  colour-sensor path at 5 sites -- IsEnableColorSensor() at :1878, :2114, :2990,
//  :3000, and asErrorMsg at :1971, :1990, :1992, :2116, :2122.
//
//  Facade-wide contract: see forms/fMain.h.
// =============================================================================
#ifndef FORMS_FTRAYFORM_H
#define FORMS_FTRAYFORM_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  TfTrayForm -- non-VCL stub (golden cTrayForm.h)
// ===========================================================================
class TfTrayForm
{
public:
    // asErrorMsg MUST be CONCRETE AnsiString STORAGE, never an accessor: golden
    // passes it as a NON-CONST `AnsiString&` out-parameter into
    // GetColorSensorIsMapping (golden asendic_Loader.cpp:1971 and :2116) and then
    // reads it back as ShowErrorMessage's errPart argument (:1990/:1992/:2122).
    // This is also facade contract rule 2 (data members are storage).
    AnsiString asErrorMsg;                  // [DATA]   golden cTrayForm.h:279
    // IsEnableColorSensor() -- offline FALSE.  There is no MU-N colour sensor on
    // an offline handler, and golden's own guard shape (`if(fTrayForm->
    // IsEnableColorSensor()==false || ...)` at asendic_Loader.cpp:1878) treats
    // "no colour sensor installed" as the skip-the-colour-check path, which is
    // the faithful offline posture.
    virtual bool IsEnableColorSensor();     // [METHOD] golden cTrayForm.h:280 -- offline: false
    TfTrayForm();
    virtual ~TfTrayForm() {}
};
extern TfTrayForm *fTrayForm;   // golden: extern PACKAGE TfTrayForm *fTrayForm; (cTrayForm.h:285)

#endif // FORMS_FTRAYFORM_H
