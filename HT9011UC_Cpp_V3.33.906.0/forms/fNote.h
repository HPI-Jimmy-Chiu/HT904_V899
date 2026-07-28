// =============================================================================
//  forms/fNote.h  --  non-VCL stand-in for golden's fNote form pointer
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.h by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Content moved VERBATIM apart
//  from the F0-e virtual destructor.  See forms/fMain.h for the facade-wide
//  contract.
// =============================================================================
#ifndef FORMS_FNOTE_H
#define FORMS_FNOTE_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  W906-AutoCleanFoundation ADD (20260721): TfNote -- FIRST home for fNote
//  (golden note.h `class TfNote : public TForm`) anywhere in this migrated
//  tree. Multiple earlier waves (Automation/AGV_PortScan.h's TfAGV::Timer2Timer
//  note, BarCode/BarCode_Bottom2DID.h's fNote->t2DCode/bMyServoOffInArm note)
//  found fNote had no home and deliberately left their own fNote-dependent
//  code gated/untranslated rather than stand up "a whole new TfNote facade
//  CLASS" for what was, in each of those cases, a single narrow call with no
//  other payoff. This wave's need is different in kind, not just degree: TWO
//  genuinely in-scope Part A functions (DoInArmPineRelease's servo-off-during-
//  alarm recovery cycle; InitialAutoCleanTask's SPIL FTP jam-code upload trace)
//  read/write exactly 4 plain-data fields -- no VCL lifecycle, no widgets, no
//  settings-editor surface like the deferred cases above. Standing up THIS
//  minimal a home costs nothing extra and unblocks a faithful (not gated)
//  translation of both functions; it does not retroactively un-gate any of
//  the earlier waves' own deferred fNote call sites (still all self-gated,
//  unaffected by this addition).
// ===========================================================================
class TfNote
{
public:
    bool       bMyServoOffInArm;     // [DATA] golden note.h:417ish (bool) -- "an alarm-triggered ServoOff on the in-arm is pending recovery"
    int        iMyServoOffInArmPosX; // [DATA] golden note.h:418 (int) -- encoder pos to jog back to (X)
    int        iMyServoOffInArmPosY; // [DATA] golden note.h:419 (int) -- encoder pos to jog back to (Y)
    AnsiString aJamCodeFilePath;     // [DATA] golden note.h:434 (AnsiString) -- SPIL FTP jam-code trace file path
    TfNote();
    virtual ~TfNote() {}
};
extern TfNote *fNote;   // golden: extern PACKAGE TfNote *fNote; (note.h:462)

#endif // FORMS_FNOTE_H
