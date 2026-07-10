//---------------------------------------------------------------------------
//  SECSGEM/SecsEventReport.h -- Sim-first EventReport() entry point
//
//  Integration wave: W5-comms INTEGRATE (serial pass after the 6-unit W5
//  parallel translate batch: SECSGEM-base / Interface / CCLink / CanBus /
//  MyPLC / EtherCAT-comms)
//  Author: AI(W5-comms-Integrate) 20260710
//
//  WHAT THIS IS -- NOT a golden translation
//  -----------------------------------------------------------------------
//  There is no golden `SecsEventReport.cpp`.  Golden's real EventReport() is
//  a FREE function (NOT a member of HTGem) defined in
//  UsecegemMainFrom.cpp:191 -- `void EventReport(unsigned Ceid)
//  { HGem->EventReport(1, Ceid); }` -- forwarding into THGem::EventReport
//  (uHGemEquipment.cpp:7703), the actual byte-level SECS/GEM protocol engine
//  (HSMS socket, message queue, ...).  THGem / uHGemEquipment.h/.cpp remain
//  explicitly OUT OF SCOPE (see SECSGEM/uHGemClass.h/.cpp file-head notes +
//  MIGRATION_ROADMAP.md) -- this integrate pass is NOT the wave that builds a
//  real HSMS engine.
//
//  This header gives csystem.cpp's existing W7C1_EVENTREPORT_*/
//  W7C2_EVENTREPORT_* no-op seam macros (csystem.cpp:1029-1030, 2321-2323) a
//  REAL, offline-safe call target instead of `do {} while(0)`: a free
//  function `EventReport(unsigned Ceid)` with an OBSERVABLE side effect
//  (records the last-reported CEID + a running count -- the same "Sim HAL"
//  shape as TSimIOBackend/TMySimMotor: real, inspectable state; no real
//  hardware/socket).
//
//  WHY THIS DOES NOT INSTANTIATE HTGem (uHGemClass.h)
//  -----------------------------------------------------------------------
//  HTGem::~HTGem() calls `MyDBIProcess(AnsiString,AnsiString,AnsiString)`
//  (3-arg) inside its catch block (uHGemClass.cpp:168).  No production TU in
//  this tree defines that 3-arg overload today -- only
//  tests/test_uHGemClass.cpp supplies a local stub for it (aHotPlateSubstrate
//  .cpp's MyDBIProcess is a DIFFERENT 2-arg overload).  If this file
//  constructed a global `HTGem` instance, every executable that links
//  ht9045_sm (i.e. essentially the whole W6/W7 test suite) would need to
//  independently resolve that 3-arg symbol -- a needless, wide-blast-radius
//  coupling for what is meant to be a minimal Sim counter.  So EventReport()
//  below is a plain free function, decoupled from HTGem's lifecycle; ht9045_
//  secsgem (this library) still carries uHGemClass.cpp/HTGem link-complete
//  and instantiable (per its own unit test), but nothing in csystem.cpp's
//  call path references it, so it stays a dormant, unextracted archive
//  member for every OTHER executable that links this library (the same
//  "compiled but never pulled in" pattern already used for e.g. the dead
//  in-arm/out-arm site variants elsewhere in this tree).
//
//  Call sites pass `SECS_EVENT.XXX` (SecsEventType.h) as the Ceid argument.
//---------------------------------------------------------------------------

#ifndef SecsEventReportH
#define SecsEventReportH

// Sim-first EventReport entry point (see file-head note above).
void EventReport(unsigned Ceid);

// Sim observability hooks for test/verify oracles (mirrors the TSimIOBackend
// pattern of exposing internal state for assertions).  Saved/restored by
// callers the same way other seeded globals are in this test suite.
extern unsigned      g_SimLastEventReportCeid;
extern unsigned long g_SimEventReportCount;
void ResetSimEventReport();

#endif
