// =============================================================================
//  forms/fSetup.cpp  --  definitions for the fSetup facade
//
//  AI(W906-W7-L2) 20260803: first home for TfSetup; see forms/fSetup.h for the
//  golden citations and the branch the offline value selects.  Follows
//  forms/fNote.cpp exactly.
// =============================================================================
#include "forms/fSetup.h"

// fShow=false is golden's own ctor value, csetup.cpp:163, inside `__fastcall
// TfSetup::TfSetup(TComponent* Owner)` at csetup.cpp:123.  It is NOT chosen for
// being "safe" -- forms/fSetup.h names the arm it opens (golden ckernel.cpp
// :413-439, the START-time motor-home ladder, which can refuse the start).
TfSetup::TfSetup() : fShow(false) {}

// Golden creates this form UNCONDITIONALLY at startup -- HT9045.cpp:183
// `Application->CreateForm(__classid(TfSetup), &fSetup);`, in the same flat
// unguarded CreateForm list as fHome.  Golden ckernel.cpp:411 dereferences
// fSetup with no null guard, which only makes sense because of that.  So a live
// instance is the faithful offline state; NULL would be a segfault at :411.
TfSetup *fSetup = new TfSetup();
