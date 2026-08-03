// =============================================================================
//  forms/fAOI.h  --  non-VCL stand-in for golden's FrmAOI form pointer
//
//  AI(W906-W7-L2) 20260803: FIRST home for TFrmAOI anywhere in this migrated
//  tree (golden fAOI.h:25 `class TFrmAOI : public TForm`).  Landed by the W7-L2
//  substrate pass ahead of the translation of golden ckernel.cpp.
//  forms/fNote.{h,cpp} convention; the forms/fMain.h facade contract binds this
//  file too.
//
//  FrmAOI has been NAMED in this tree before without being landed --
//  aoutarm9045.cpp:711/:714/:802 record an out-arm Rotate/AOI/FixAI predicate
//  left inside `#if 0 // TODO(W7)` partly BECAUSE FrmAOI had no home.  This
//  facade does NOT un-gate that block (different wave, different file, and the
//  block needs tRotate / tAOISetup / ScannerAOIIF as well).  Reported, not done.
//
//  ---------------------------------------------------------------------------
//  CLASS NAME: `TFrmAOI`, NOT `TfAOI`.  DEVIATION FROM THE BRIEF, ON GOLDEN'S
//  AUTHORITY.
//  ---------------------------------------------------------------------------
//  The W7-L2 assignment asked for "TfAOI / global FrmAOI".  Golden fAOI.h:25 is
//  `class TFrmAOI : public TForm`, fAOI.h:414 is `__fastcall TFrmAOI(TComponent*
//  Owner);` and fAOI.h:492 is `extern PACKAGE TFrmAOI *FrmAOI;`.  Golden wins.
//  Same deviation, same reason, as forms/fRotate.h's TFrmRotate.
//
//  ---------------------------------------------------------------------------
//  ##  THE NULL QUESTION -- I DID NOT DO WHAT THE BRIEF ASKED.  HERE IS WHY.  ##
//  ---------------------------------------------------------------------------
//  The W7-L2 assignment stated that because golden null-checks this pointer at
//  ckernel.cpp:502 (`if(FrmAOI!=NULL)`), "a null global is the faithful offline
//  state and the class exists only so the guarded block compiles", and asked for
//  `FrmAOI = NULL`.  I checked that premise against golden instead of taking it,
//  and it does not hold:
//
//    golden HT9045.cpp:233
//        Application->CreateForm(__classid(TFrmAOI), &FrmAOI);
//
//  That line sits in the flat CreateForm list at HT9045.cpp:166-245, which I
//  read end to end: 80 consecutive CreateForm calls, no `if`, no config gate, no
//  machine-type switch anywhere in the block.  fMain (:166), fNote (:173), COM2
//  (:174), fHome (:175), fSetup (:183), fContact (:184), FrmRotate (:226),
//  Zteach (:230) and FrmAOI (:233) are all created the same unconditional way.
//  So in golden, FrmAOI is non-NULL for the entire life of the process after
//  WinMain, and ScanSystemSensor -- which can only run once SoftStart has been
//  set from the UI, i.e. long after WinMain -- ALWAYS takes the true arm at
//  :502 and ALWAYS executes :504.  The `!=NULL` guard is defensive coding added
//  by the same 20260515 change that introduced the field (both lines carry the
//  "Eastsun 20260515 F023" tag), not evidence of a reachable NULL state.
//
//  Landing NULL would therefore have made ckernel.cpp:504 permanently dead code
//  in the ported tree, unreachable by any test, while the translation claimed to
//  be faithful.  That is the specific failure mode this project has been burned
//  by.  So forms/fAOI.cpp allocates a live instance, and golden's `if(FrmAOI!=
//  NULL)` guard must still be transcribed VERBATIM by the ckernel translation --
//  it is golden's line, it simply evaluates true, exactly as it does on a real
//  machine.
//
//  RISK OF MY CHOICE, STATED: none that I can find.  See bSimulateTopBtm below
//  -- the field has no reader anywhere in golden, so executing :504 changes
//  nothing observable; it only makes the branch live and testable.
//
//  ---------------------------------------------------------------------------
//  GOLDEN ckernel.cpp CENSUS FOR FrmAOI -- MEASURED OVER ALL 2589 LINES
//  ---------------------------------------------------------------------------
//    :46   #include "fAOI.h"                (the include golden added for this)
//    :502  if(FrmAOI!=NULL)                 (read)  ScanSystemSensor
//    :504  FrmAOI->bSimulateTopBtm=true;    (write) ScanSystemSensor
//  One member.  Golden's TFrmAOI is a 784-line header with a large inspection
//  API; none of the rest is reachable from ckernel.cpp and none is landed.
// =============================================================================
#ifndef FORMS_FAOI_H
#define FORMS_FAOI_H

// Dependency-free header: one bool.  See forms/fHome.h for the reasoning.

class TFrmAOI
{
public:
    // -----------------------------------------------------------------------
    //  [DATA] golden fAOI.h:472 `bool bSimulateTopBtm;`
    //  Golden's own comment on that line: "Eastsun 20260515 F023: E2 lock
    //  simulate during run" -- i.e. "the Top/Bottom AOI inspection is pinned to
    //  simulate mode because a lot is running".
    //
    //  OFFLINE VALUE false.  Golden's ctor `__fastcall TFrmAOI::TFrmAOI
    //  (TComponent* Owner)` (fAOI.cpp:128) does NOT assign it -- same VCL
    //  zero-fill situation as forms/fRotate.h's flags, so false is golden's real
    //  initial value.
    //
    //  THE BRANCH false SELECTS -- and an honest measurement that constrains how
    //  much that claim is worth.  I scanned ALL 887 files of the golden tree
    //  (every extension, not just .cpp/.h) for this identifier.  It appears
    //  exactly twice:
    //      fAOI.h:472      the declaration
    //      ckernel.cpp:504 the write, `FrmAOI->bSimulateTopBtm=true;`
    //  There is NO READER of bSimulateTopBtm anywhere in golden V3.33.906.0.
    //  As of this golden snapshot the field is write-only -- the F023 feature
    //  that consumes it has evidently not landed on the AOI side yet.  So the
    //  truthful statement is: false selects no golden branch today, because no
    //  golden code branches on it.  It is landed anyway because golden
    //  ckernel.cpp:504 writes it and a faithful ScanSystemSensor must perform
    //  that write; the field is the storage that write needs, and false is the
    //  correct pre-START value so a test can observe the transition false->true
    //  across SoftStart.  That observable transition is, right now, the entire
    //  behavioural content of golden ckernel.cpp:502-505.
    // -----------------------------------------------------------------------
    bool bSimulateTopBtm;

    TFrmAOI();
    virtual ~TFrmAOI() {}
};

// golden: extern PACKAGE TFrmAOI *FrmAOI;   (fAOI.h:492)
extern TFrmAOI *FrmAOI;

#endif // FORMS_FAOI_H
