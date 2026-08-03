// =============================================================================
//  forms/fAOI.cpp  --  definitions for the FrmAOI facade
//
//  AI(W906-W7-L2) 20260803: first home for TFrmAOI.  forms/fAOI.h carries the
//  golden citations, the TFrmAOI-vs-TfAOI naming deviation, and -- most
//  importantly -- the full argument for why this global is NOT initialised to
//  NULL despite the W7-L2 brief asking for NULL.  Read that block before
//  changing the line at the bottom of this file.
// =============================================================================
#include "forms/fAOI.h"

// bSimulateTopBtm=false: golden's TFrmAOI ctor (fAOI.cpp:128) does not assign
// the member at all, so false is what VCL's instance zero-fill gives golden.
// Written explicitly here so the value is reviewable and so this facade does
// not depend on zero-init for a heap-allocated object.
TFrmAOI::TFrmAOI() : bSimulateTopBtm(false) {}

// -----------------------------------------------------------------------------
//  A LIVE INSTANCE, NOT NULL -- deliberate, and contrary to the W7-L2 brief.
//  Golden HT9045.cpp:233 `Application->CreateForm(__classid(TFrmAOI), &FrmAOI);`
//  is unconditional (verified by reading the whole CreateForm list,
//  HT9045.cpp:166-245: 80 calls, zero guards).  FrmAOI is therefore non-NULL on
//  every real machine by the time SoftStart can be set, so golden ckernel.cpp
//  :502's `if(FrmAOI!=NULL)` is TRUE and :504 runs.  A NULL here would have made
//  the ported :504 permanently unreachable while the translation claimed
//  fidelity.  forms/fAOI.h gives the full argument and the measurement showing
//  this choice carries no behavioural risk (bSimulateTopBtm has no reader
//  anywhere in golden).
//
//  NOTE FOR THE ckernel.cpp TRANSLATION: keep golden's `if(FrmAOI!=NULL)` guard
//  verbatim.  It is golden's line; it simply evaluates true here, as it does on
//  a real machine.  Do not "simplify" it away on the grounds that this pointer
//  is now known non-NULL.
// -----------------------------------------------------------------------------
TFrmAOI *FrmAOI = new TFrmAOI();
