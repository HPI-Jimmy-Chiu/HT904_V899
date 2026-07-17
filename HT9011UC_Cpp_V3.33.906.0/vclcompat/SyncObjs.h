// ===========================================================================
//  vclcompat/SyncObjs.h
//  Headless BCB6 TCriticalSection compatibility shim (VCL SyncObjs.hpp).
//
//  AI(W906-uHGemEquipment-BucketC) 20260717: new file.
//
//  FIRST CONSUMER: SECSGEM/uHGemEquipment.h's `csSFCodeResponse` member
//  (golden uHGemEquipment.h:723, `TCriticalSection *csSFCodeResponse;`) and
//  the golden `TFixedCriticalSection : public TCriticalSection` (golden
//  uHGemEquipment.h:25-29, transcribed verbatim in uHGemEquipment.h -- this
//  header only supplies the BASE class; TFixedCriticalSection itself stays
//  declared in uHGemEquipment.h, see that file's own note).
//
//  SCOPE: real VCL SyncObjs::TCriticalSection wraps a Win32 CRITICAL_SECTION
//  (re-entrant / recursive by construction -- the SAME OS thread may
//  Acquire() it more than once without deadlocking itself, as long as it
//  Releases() the same number of times). A raw Win32 CRITICAL_SECTION is
//  therefore the natural backing store -- and the ONLY option this
//  toolchain's libstdc++ actually offers: this tree's MinGW.org GCC 6.3.0 has
//  no working <mutex> (std::mutex/std::recursive_mutex are declared but not
//  defined -- confirmed empirically, `'mutex' is not a member of 'std'` on a
//  throwaway probe), so std::recursive_mutex is not usable here. This matches
//  the SAME already-established pattern vclcompat/ClientSocket.cpp/
//  ServerSocket.cpp already use for their own internal locking (raw
//  CRITICAL_SECTION + EnterCriticalSection/LeaveCriticalSection), just
//  promoted to a small reusable public class here since golden's
//  TCriticalSection is a real, directly-instantiated THGem member type
//  (`csSFCodeResponse`), not a private implementation detail.
//
//  Only Acquire()/Release() are called anywhere in this wave's scope
//  (grepped: SECSGEM/uHGemEquipment.cpp's DoProcessSFNoResponse/
//  CheckSFCodeResponse/SendLocalDataFrom, all guarding csSFCodeResponse;
//  clientGemRead's own lock is a TFixedCriticalSection, declared separately
//  in uHGemEquipment.h) -- Enter()/Leave() are real VCL aliases for the
//  identical operation, kept here for completeness/fidelity even though
//  nothing in this port's current scope calls them by that name.
//
//  Header-only (inline methods) -- no matching .cpp, no new CMake source
//  entry required beyond this header existing on disk.
// ===========================================================================
#ifndef VCLCOMPAT_SYNCOBJS_H
#define VCLCOMPAT_SYNCOBJS_H

#if defined(_WIN32)
#include <windows.h>
#endif

namespace vclcompat {

class TCriticalSection {
public:
#if defined(_WIN32)
    TCriticalSection() { ::InitializeCriticalSection(&cs_); }
    ~TCriticalSection() { ::DeleteCriticalSection(&cs_); }

    void Acquire() { ::EnterCriticalSection(&cs_); }
    void Release() { ::LeaveCriticalSection(&cs_); }
#else
    // Non-Windows fallback: this project targets Windows exclusively (BCB6/
    // VCL origin), but a no-op keeps this header self-contained if ever
    // pulled into a portability probe build.
    TCriticalSection() {}
    ~TCriticalSection() {}

    void Acquire() {}
    void Release() {}
#endif

    // Non-copyable: a real VCL TCriticalSection is never meaningfully copied
    // either (every golden consumer holds it via a pointer, `new`d once).
    TCriticalSection(const TCriticalSection&) = delete;
    TCriticalSection& operator=(const TCriticalSection&) = delete;

    // Real VCL aliases for Acquire/Release (SyncObjs.hpp). Not called by any
    // golden site in this wave's scope, kept for interface completeness.
    void Enter() { Acquire(); }
    void Leave() { Release(); }

private:
#if defined(_WIN32)
    CRITICAL_SECTION cs_;
#endif
};

} // namespace vclcompat

// Matches vclcompat/StringGrid.h's own posture: NOT brought into the global
// namespace by vcl_compat.h (this shim is not yet wired into that umbrella
// header -- consumers add their own `using vclcompat::TCriticalSection;`).
// SECSGEM/uHGemEquipment.h does exactly that (see its own include block).

#endif // VCLCOMPAT_SYNCOBJS_H
