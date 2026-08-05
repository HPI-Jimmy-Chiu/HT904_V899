// ===========================================================================
//  WebBridge/TagSnapshot.cpp
//  AI(W906-WebBridge-State) 20260805: new file.
//
//  See TagSnapshot.h for the full threading contract. The two things this file
//  must get right, and the reason each matters:
//
//   1. The publisher's critical section is ONE container swap + ONE increment.
//      Anything longer is time the UI thread can be made to wait by a socket
//      thread, and the UI thread owes SECS/GEM a reply inside 30 s.
//
//   2. A reader either sees generation N in full or generation N-1 in full,
//      never a mix. The map and its generation are copied inside the SAME
//      critical section, which is what makes the pair consistent. Splitting
//      them into two lock acquisitions is the classic way to reintroduce the
//      torn read; test_wb_state.cpp exists to catch exactly that regression.
//
//  WHY Win32 CRITICAL_SECTION AND NOT std::mutex
//  ---------------------------------------------
//  This tree's MinGW.org GCC 6.3.0 is built with `Thread model: win32` and its
//  libstdc++ provides no usable <mutex>/<thread> -- probed empirically:
//  `'mutex' is not a member of 'std'`. The port tree already settled this the
//  same way (vclcompat/SyncObjs.h, vclcompat/ClientSocket.cpp,
//  vclcompat/ServerSocket.cpp, MyPLC/ModbusTCPClient.h all use raw
//  CRITICAL_SECTION). A CRITICAL_SECTION *is* a mutex, it is what MSVC's own
//  std::mutex wraps on this platform, and using it keeps ONE source that
//  compiles under both toolchains instead of two #ifdef'd halves.
//
//  This layer deliberately does NOT reuse vclcompat::TCriticalSection: the web
//  bridge must stay independent of vcl.h / vclcompat so it can be built, tested
//  and reasoned about without the VCL shim layer.
// ===========================================================================
#include "WebBridge/TagSnapshot.h"

#include <atomic>
#include <utility>   // std::swap

#if defined(_WIN32)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN   // keep winsock.h out; the socket layer needs winsock2.h
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#endif

#include "WebBridge/Sync.h"

namespace webbridge {

// ===========================================================================
//  File-local lock primitive
// ===========================================================================
namespace {

// WbLock/WbGuard used to be defined here, and identically in the sibling file.
// Both copies were consolidated into WebBridge/Sync.h on 20260805, which also
// serves WebBridgeServer.cpp. The typedef keeps every WbLock use below
// unchanged. Note Sync.h is Win32-only by #error rather than by silent no-op:
// the previous no-op branch made a non-Windows host build compile while
// providing NO mutual exclusion at all, which is a worse outcome than not
// building, and this product is Windows-only anyway.
typedef WbMutex WbLock;

// Current OS thread id, as an integer. Used only for publisher misuse
// detection, never for dispatch.
unsigned long currentThreadId()
{
#if defined(_WIN32)
    return static_cast<unsigned long>(::GetCurrentThreadId());
#else
    return 0UL;
#endif
}

} // anonymous namespace

// ===========================================================================
//  Pure helpers
// ===========================================================================
TagPatch diffMaps(const TagMap& from, const TagMap& to)
{
    TagPatch patch;

    // Present in `to`: new tag, or a changed value.
    //
    // Both maps are sorted by the same comparator, so this could be a linear
    // merge walk. It is written as a lookup-per-tag instead because clarity
    // wins at ~234 tags / 10 Hz, and because a merge walk gets the
    // Null-vs-absent edge cases wrong far too easily.
    TagMap::const_iterator it = to.begin();
    for (; it != to.end(); ++it) {
        TagMap::const_iterator prev = from.find(it->first);
        if (prev == from.end()) {
            patch.changed.insert(*it);            // appeared
        } else if (!(prev->second == it->second)) {
            // operator!= via operator== so the Null != String("") rule in
            // TagValue is the single definition of "changed".
            patch.changed.insert(*it);            // value changed
        }
    }

    // Present in `from`, gone in `to`.
    TagMap::const_iterator old = from.begin();
    for (; old != from.end(); ++old) {
        if (to.find(old->first) == to.end()) {
            patch.removed.push_back(old->first);  // disappeared
        }
    }

    return patch;
}

void applyPatch(TagMap& target, const TagPatch& patch)
{
    TagMap::const_iterator it = patch.changed.begin();
    for (; it != patch.changed.end(); ++it) {
        target[it->first] = it->second;
    }
    for (size_t i = 0; i < patch.removed.size(); ++i) {
        target.erase(patch.removed[i]);
    }
}

// ===========================================================================
//  TagSnapshot::Impl
// ===========================================================================
struct TagSnapshot::Impl {
    // --- publisher-owned, NOT guarded by `lock` (single-writer by contract) --
    TagMap staging;

    // --- guarded by `lock` -------------------------------------------------
    WbLock        lock;
    TagMap        front;
    std::uint64_t generation;

    // --- misuse detection --------------------------------------------------
    //  32-bit atomics so no 64-bit atomic helper (libatomic / cmpxchg8b) is
    //  needed on this tree's 32-bit MinGW target. A Windows thread id IS 32-bit
    //  (DWORD), and a violation counter that wraps at 4e9 has stopped being
    //  informative long before that.
    std::atomic<unsigned long> publisherTid;
    std::atomic<unsigned long> violations;

    Impl() : staging(), lock(), front(), generation(0), publisherTid(0), violations(0) {}
};

// ===========================================================================
//  Construction
// ===========================================================================
TagSnapshot::TagSnapshot() : impl_(new Impl())
{
}

TagSnapshot::~TagSnapshot()
{
    delete impl_;
    impl_ = 0;
}

// ===========================================================================
//  PUBLISH PATH -- UI THREAD ONLY
// ===========================================================================
namespace {

// Record/verify the publishing thread. Returns silently either way; the caller
// is a real-time-ish machine thread and must not be thrown out of.
void notePublisherThread(std::atomic<unsigned long>& tid,
                         std::atomic<unsigned long>& violations)
{
    const unsigned long me = currentThreadId();
    unsigned long expected = 0UL;

    // First publisher wins the slot. compare_exchange_strong so two threads
    // racing to be first cannot both believe they are the publisher.
    if (tid.compare_exchange_strong(expected, me)) {
        return;             // we are now THE publisher
    }
    if (expected != me) {
        violations.fetch_add(1UL);   // someone else is publishing: contract broken
    }
}

} // anonymous namespace

void TagSnapshot::beginPublish()
{
    notePublisherThread(impl_->publisherTid, impl_->violations);

    // Clear, not "merge into current": a tick publishes the whole tag set, so a
    // tag that stops being staged genuinely goes absent. Merging would make
    // stale tags immortal, and a stale tag on an operator screen is worse than
    // a missing one.
    impl_->staging.clear();
}

void TagSnapshot::stage(const std::string& tag, const TagValue& value)
{
    notePublisherThread(impl_->publisherTid, impl_->violations);
    impl_->staging[tag] = value;
}

void TagSnapshot::stageFrom(const TagMap& tags)
{
    notePublisherThread(impl_->publisherTid, impl_->violations);
    TagMap::const_iterator it = tags.begin();
    for (; it != tags.end(); ++it) {
        impl_->staging[it->first] = it->second;
    }
}

std::uint64_t TagSnapshot::commitPublish()
{
    notePublisherThread(impl_->publisherTid, impl_->violations);

    std::uint64_t gen = 0;
    {
        WbGuard g(impl_->lock);
        // ------------------------------------------------------------------
        //  THE critical section. std::map::swap is O(1) (it exchanges internal
        //  pointers), so this is a handful of pointer writes plus one add.
        //  Nothing here allocates, compares, formats or touches I/O -- which is
        //  the entire point of double buffering rather than copying into the
        //  front buffer under the lock.
        //
        //  The generation is incremented INSIDE the same critical section as
        //  the swap. That is what makes (tags, generation) a consistent pair
        //  for every reader; two separate locks here would let a reader observe
        //  the new generation number with the old tags.
        // ------------------------------------------------------------------
        impl_->front.swap(impl_->staging);
        impl_->generation += 1;
        gen = impl_->generation;
    }

    // The old front buffer is now in `staging`. It is cleared by the next
    // beginPublish(), not here, so this call stays O(1) -- freeing ~234 map
    // nodes is real work and it does not belong on the publish path's tail
    // where it would sit between the swap and the caller's next tick.
    return gen;
}

std::uint64_t TagSnapshot::publish(const TagMap& tags)
{
    beginPublish();
    stageFrom(tags);
    return commitPublish();
}

size_t TagSnapshot::stagedTagCount() const
{
    // Publisher-side buffer: no lock, because by contract only the publishing
    // thread touches it.
    return impl_->staging.size();
}

// ===========================================================================
//  READ PATH -- ANY THREAD
// ===========================================================================
TagSnapshotView TagSnapshot::read() const
{
    TagSnapshotView view;
    {
        WbGuard g(impl_->lock);
        // Copy, never a reference: the front buffer can be swapped out from
        // under the caller the moment this lock is released.
        view.tags       = impl_->front;
        view.generation = impl_->generation;
    }
    return view;   // moved out; the copy above happened under the lock
}

std::uint64_t TagSnapshot::generation() const
{
    WbGuard g(impl_->lock);
    return impl_->generation;
}

size_t TagSnapshot::publishedTagCount() const
{
    WbGuard g(impl_->lock);
    return impl_->front.size();
}

TagPatch TagSnapshot::diffFrom(const TagSnapshotView& lastSent) const
{
    // Fast path: peek the generation only. If it has not advanced, no publish
    // has happened since this connection's last frame, so by construction
    // nothing changed and there is no reason to copy ~234 tags.
    TagSnapshotView now;
    {
        WbGuard g(impl_->lock);
        if (impl_->generation == lastSent.generation) {
            TagPatch none;
            none.generation = impl_->generation;
            return none;
        }
        now.tags       = impl_->front;
        now.generation = impl_->generation;
    }

    // Comparison runs OUTSIDE the lock, on a private copy. Serialising the
    // result to JSON also happens outside, in the caller.
    TagPatch patch  = diffMaps(lastSent.tags, now.tags);
    patch.generation = now.generation;
    return patch;
}

TagPatch TagSnapshot::diffFrom(const TagMap& lastSent) const
{
    TagSnapshotView now = read();          // copy under the lock
    TagPatch patch      = diffMaps(lastSent, now.tags);   // compare outside it
    patch.generation    = now.generation;
    return patch;
}

// ===========================================================================
//  MISUSE DETECTION
// ===========================================================================
std::uint64_t TagSnapshot::publisherViolations() const
{
    return static_cast<std::uint64_t>(impl_->violations.load());
}

std::uint64_t TagSnapshot::publisherThreadId() const
{
    return static_cast<std::uint64_t>(impl_->publisherTid.load());
}

} // namespace webbridge
