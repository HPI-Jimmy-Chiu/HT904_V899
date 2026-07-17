// ===========================================================================
//  vclcompat/MemoryStream.h
//  Headless BCB6 TMemoryStream compatibility shim (VCL Classes.hpp).
//
//  AI(W906-uHGemEquipment-BucketC) 20260717: new file.
//
//  FIRST CONSUMER: SECSGEM/uHGemEquipment.cpp's clientGemRead/
//  ProcessSocketReceiveData (golden :9008-9207) -- THGem's
//  `RecvMemoryBuffer`/`ProcBuffer`/`TempProcBuffer` members (golden
//  uHGemEquipment.h:718-720).
//
//  API = exactly the golden call sites (all in uHGemEquipment.cpp:9008-9207;
//  grepped, no other consumer in this wave's scope):
//    WriteBuffer(const void*, int)                    (:9020/:9071/:9126)
//    Size (read)                                       (:9056/:9062/:9064/:9080)
//    Position (write=0, read)                           (:9066/:9125/:9128)
//    ReadBuffer(void*, int)                            (:9067/:9083)
//    Clear()                                    (:9068/:9094/:9101/:9112/:9127/:9130/:9200)
//    Seek(int Offset, unsigned short Origin)            (:9070/:9082)
//    LoadFromStream(TMemoryStream*)                     (:9129)
//
//  SEMANTICS (VCL-faithful; full rationale in MemoryStream.cpp):
//    * Backing store: std::vector<unsigned char> FData (private). `Position`
//      is a plain public int field -- golden writes it bare
//      (`RecvMemoryBuffer->Position=0;`) -- every mutator below keeps it
//      coherent with FData's actual size.
//    * Size: READ-ONLY proxy (golden never assigns ->Size anywhere in this
//      wave's scope -- verified by grep). Returns FData.size().
//    * WriteBuffer: writes at Position, grows FData only if the write's end
//      extends past the existing size (bytes between the old Position and
//      the existing size are OVERWRITTEN; any existing tail BEYOND the
//      write's end is KEPT -- real VCL TStream::WriteBuffer behavior; golden's
//      own TempProcBuffer path at :9125-9129 relies on Clear() -- not
//      WriteBuffer -- being what actually resets size). Advances Position.
//    * ReadBuffer: copies exactly Count bytes from Position, advances
//      Position. On a genuine shortfall (Position+Count>size) throws
//      std::runtime_error("EReadError") (VCL raises EReadError); golden never
//      short-reads in this wave's scope (every call site reads ->Size first
//      and passes exactly that much) -- this is a tripwire, not a hot path.
//    * Seek(0, soFromBeginning) -> Position=0; Seek(0, soFromEnd) ->
//      Position=Size (both are the only two golden call shapes in this wave's
//      scope; soFromCurrent is implemented too, for interface completeness).
//      Returns the new Position.
//    * Clear(): FData.clear(); Position=0.
//    * LoadFromStream(TMemoryStream *Source): VCL TStream semantics -- seeks
//      Source to 0, copies Source's ENTIRE current content into `this`
//      (replacing `this`'s own prior content), leaves `this`->Position at 0.
//
//  Non-copyable (matches vclcompat::TStringGrid/TStringList's own no-copy
//  stance -- golden THGem is likewise never copied).
// ===========================================================================
#ifndef VCLCOMPAT_MEMORYSTREAM_H
#define VCLCOMPAT_MEMORYSTREAM_H

#include <vector>

namespace vclcompat {

// Golden Classes.hpp TSeekOrigin-equivalent constants (real VCL values: 0/1/2).
enum TMemoryStreamSeekOrigin {
    soFromBeginning = 0,
    soFromCurrent   = 1,
    soFromEnd       = 2
};

class TMemoryStream {
public:
    TMemoryStream();

    TMemoryStream(const TMemoryStream&) = delete;
    TMemoryStream& operator=(const TMemoryStream&) = delete;

    // ---- Size : read-only property proxy -----------------------------------
    class SizeProxy {
    public:
        explicit SizeProxy(const TMemoryStream* o) : owner_(o) {}
        operator int() const;
    private:
        const TMemoryStream* owner_;
    };
    SizeProxy Size;

    // ---- Position : plain public field (golden writes it bare) ------------
    int Position;

    void WriteBuffer(const void* Buffer, int Count);
    void ReadBuffer(void* Buffer, int Count);
    int  Seek(int Offset, unsigned short Origin);
    void Clear();
    void LoadFromStream(TMemoryStream* Source);

private:
    friend class SizeProxy;
    std::vector<unsigned char> FData;
};

} // namespace vclcompat

// Matches vclcompat/ServerSocket.h's own posture (`using namespace
// Scktcomp;` at its own file tail) -- golden code spells these unqualified
// (TMemoryStream, soFromBeginning, soFromEnd), so bring them into the global
// namespace directly from this header rather than relying on vcl_compat.h's
// umbrella (this shim is not yet wired into that umbrella -- see this file's
// own consumer, SECSGEM/uHGemEquipment.h, which includes this header
// directly).
#ifndef VCLCOMPAT_NO_GLOBAL_USING
using vclcompat::TMemoryStream;
using vclcompat::soFromBeginning;
using vclcompat::soFromCurrent;
using vclcompat::soFromEnd;
#endif

#endif // VCLCOMPAT_MEMORYSTREAM_H
