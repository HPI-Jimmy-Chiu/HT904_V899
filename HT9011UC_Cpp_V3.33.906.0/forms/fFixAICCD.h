// =============================================================================
//  forms/fFixAICCD.h  --  non-VCL stand-in for golden's fFixAICCD form pointer
//
//  AI(W906-W7-L1-Wave0) 20260801: NEW FILE, and it RETIRES a null global.
//
//  WHY THIS IS A CRASH FIX, NOT A STUB-GAP FILL
//  --------------------------------------------
//  Before this file, the tree's only `fFixAICCD` was `TfFixAICCD *fFixAICCD = 0;`
//  at aoutarm_shims.cpp:112, next to a TU-local `class TfFixAICCD { public: void
//  OutArmCycleCounterUpdate(); };` at :110 that carries only ONE of the two
//  methods golden's form needs.  golden asendic_Auto.cpp dereferences the pointer
//  UNCONDITIONALLY on main paths -- :153, :184, :248, :404, :454, :1920 (the
//  tray-eject path) and :2113 -- so a null global is a crash waiting for the
//  first translated Auto tray eject, not a benign no-op.  Standing up one real,
//  non-null object here fixes all of them at once and gives the second method
//  (bCheckUnloaderHasAiNG, which the TU-local class does not have) a home.
//
//  GOLDEN TYPE CORRECTION -- bCheckUnloaderHasAiNG RETURNS void, NOT bool.
//  golden FixAICCD.h:147 declares `void bCheckUnloaderHasAiNG(int iMot);`
//  (Sam 20240325, DamageTrayMapping).  The leading `b` in the name is misleading
//  and the W7-L1 recon read it as a bool; every one of golden's 7 live call sites
//  listed above calls it as a bare STATEMENT and discards nothing, which is
//  consistent only with void.  Declared void here, matching golden.
//
//  BOUNDARY NOTE: aoutarm_shims.cpp and the aoutarm9045_* variants belong to
//  W7-A2.  Repointing them at this header was explicitly authorised for this
//  serialized pass (no other writer is active).  It is also unavoidable rather
//  than optional: FormsFacade.h now includes this header, and those TUs include
//  FormsFacade.h, so leaving their own `class TfFixAICCD` / differently-typed
//  `extern ... *fFixAICCD` in place would be a hard redefinition/conflicting-
//  declaration compile error, not a latent ODR nit.
//
//  Facade-wide contract: see forms/fMain.h.
// =============================================================================
#ifndef FORMS_FFIXAICCD_H
#define FORMS_FFIXAICCD_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  TfFixAICCD -- non-VCL stub (golden FixAICCD.h)
// ===========================================================================
class TfFixAICCD
{
public:
    // Offline: the real body refreshes the Fix-AI-CCD form's out-arm cycle
    // counter display.  No headless equivalent -> no-op sink.
    virtual void OutArmCycleCounterUpdate();            // [METHOD] golden FixAICCD.h:130 -- offline: no-op
    // Offline: the real body drives the DamageTrayMapping AI-NG check against the
    // Fix-AI CCD for the given unloader motor.  No camera offline -> no-op sink.
    // Return type is golden's: void (golden FixAICCD.h:147), see the file head.
    virtual void bCheckUnloaderHasAiNG(int iMot);       // [METHOD] golden FixAICCD.h:147 -- offline: no-op
    virtual ~TfFixAICCD() {}
};
extern TfFixAICCD *fFixAICCD;   // golden: extern PACKAGE TfFixAICCD *fFixAICCD; (FixAICCD.h:152)

#endif // FORMS_FFIXAICCD_H
