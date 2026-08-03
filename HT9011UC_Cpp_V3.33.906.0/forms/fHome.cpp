// =============================================================================
//  forms/fHome.cpp  --  definitions for the fHome facade
//
//  AI(W906-W7-L2) 20260803: first home for TfHome; see forms/fHome.h for the
//  golden citations, the macro-collision check and the per-member offline
//  justifications.  Follows forms/fNote.cpp exactly.
// =============================================================================
#include "forms/fHome.h"

// The initialiser list is in DECLARATION ORDER (iHomeStep, fShow, fAbort) so
// there is no -Wreorder under the -Wall -Wextra this library is built with.
// Every value is read straight out of golden's own constructor, `__fastcall
// TfHome::TfHome(TComponent* Owner)` at uhome.cpp:106-113 -- iHomeStep=1
// (uhome.cpp:109), fShow=false (uhome.cpp:110), fAbort=false (uhome.cpp:111).
// Golden's fourth ctor statement, `ledHome->Visible=false;` (uhome.cpp:112), is
// pure VCL widget state with no member on this facade and is deliberately not
// carried over.
TfHome::TfHome() : iHomeStep(1), fShow(false), fAbort(false) {}

// Golden creates this form UNCONDITIONALLY at startup -- HT9045.cpp:175
// `Application->CreateForm(__classid(TfHome), &fHome);`, inside the flat
// unguarded CreateForm list at HT9045.cpp:166-245 (re-read in full; there is no
// enclosing if/config gate on any entry in that list).  So `fHome` is non-NULL
// for the whole life of the program after WinMain, which is precisely why
// golden's ckernel.cpp:372 and :373 dereference it with no null guard.  A live
// instance here is therefore the FAITHFUL offline state, not a convenience: a
// NULL global would turn those two unguarded golden writes into a segfault.
// Same allocate-at-load idiom as forms/fNote.cpp.
TfHome *fHome = new TfHome();
