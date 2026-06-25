// ---------------------------------------------------------------------------
// MIGRATION NOTE (W1 Public leaf utilities - HT9045 BCB6 -> Standard C++17)
// Provenance: HT9011UC_Code_V3.33.906.0_20260618/Public/WinSocketErrorCode.h
// Changes from BCB6 original:
//   - TObject* Sender param retained (unused by GetErrorMsg; caller may pass NULL).
//   - LogClientSocketExceptionError DEFERRED: couples to VCL TClientSocket
//     (->Name/Address/Port) and MyDBIProcess. Declared here as a stub comment
//     so downstream code that was merely #including the header still compiles.
//   - Removed #ifndef guard spelling kept identical (WinSocketErrorCodeH) so
//     translated TUs that include this header see the same macro as BCB6.
// ---------------------------------------------------------------------------
#ifndef WinSocketErrorCodeH
#define WinSocketErrorCodeH

#include "vclcompat/vcl_compat.h"   // AnsiString, TObject, IntToStr

// Maps a Winsock error code to a descriptive AnsiString.
// Sender is accepted for signature compatibility but is not used.
// BCB6 source: WinSocketErrorCode.cpp:12-314
AnsiString GetErrorMsg(TObject *Sender, int ErrorCode);

// DEFERRED (W-UI wave): LogClientSocketExceptionError couples to VCL
// TClientSocket (->Name/Address/Port) and MyDBIProcess (cMyDB).
// Omitted from this translation unit.

#endif // WinSocketErrorCodeH
