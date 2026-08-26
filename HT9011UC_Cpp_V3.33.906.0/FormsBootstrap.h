//---------------------------------------------------------------------------
//  FormsBootstrap.h -- explicit, caller-driven construction of the translated
//  form globals that golden builds inside HT9045.cpp's WinMain.
//
//  AI(W906-FW-BOOTSTRAP-W35) 20260827: NEW FILE.
//
//  InitForms() must be CALLED; it is deliberately not a static initializer.
//  Cross-TU dynamic initialization has no defined order and this tree has
//  already paid for that (88 of 134 ctest binaries SEGFAULTed before main()
//  when fLaserSensor's ctor ran against a still-NULL elLaser -- see
//  docs/KNOWLEDGE.md, "V906 homecoming SIOF lottery").
//
//  The rationale, the golden citations, the per-ctor audit and the measured
//  behaviour delta all live in FormsBootstrap.cpp's banner -- read that before
//  adding a row or adding a caller.
//---------------------------------------------------------------------------
#ifndef FormsBootstrapH
#define FormsBootstrapH

// Constructs the translated form globals golden creates in WinMain.
// Idempotent: every row is guarded, so a second call constructs nothing.
// Safe to call only after static initialization has completed (i.e. from
// inside main()/WinMain or later), never from another TU's initializer.
void InitForms();

#endif // FormsBootstrapH
