// ===========================================================================
//  vclcompat/MemoryStream.cpp
//  Implementation of the headless vclcompat::TMemoryStream shim (see
//  MemoryStream.h for the full scope / semantics notes).
//
//  AI(W906-uHGemEquipment-BucketC) 20260717: new file.
// ===========================================================================
#include "vclcompat/MemoryStream.h"

#include <stdexcept>
#include <cstring>

namespace vclcompat {

TMemoryStream::TMemoryStream() : Size(this), Position(0)
{
}

TMemoryStream::SizeProxy::operator int() const
{
    return static_cast<int>(owner_->FData.size());
}

void TMemoryStream::WriteBuffer(const void* Buffer, int Count)
{
    if (Buffer == 0 || Count <= 0)
        return;

    std::size_t endPos = static_cast<std::size_t>(Position) + static_cast<std::size_t>(Count);
    if (endPos > FData.size())
        FData.resize(endPos);

    std::memcpy(&FData[static_cast<std::size_t>(Position)], Buffer, static_cast<std::size_t>(Count));
    Position += Count;
}

void TMemoryStream::ReadBuffer(void* Buffer, int Count)
{
    if (Count <= 0)
        return;

    std::size_t endPos = static_cast<std::size_t>(Position) + static_cast<std::size_t>(Count);
    if (endPos > FData.size())
        throw std::runtime_error("EReadError");   // real VCL: EReadError

    std::memcpy(Buffer, &FData[static_cast<std::size_t>(Position)], static_cast<std::size_t>(Count));
    Position += Count;
}

int TMemoryStream::Seek(int Offset, unsigned short Origin)
{
    switch (Origin)
    {
        case soFromCurrent:
            Position = Position + Offset;
            break;
        case soFromEnd:
            Position = static_cast<int>(FData.size()) + Offset;
            break;
        case soFromBeginning:
        default:
            Position = Offset;
            break;
    }
    return Position;
}

void TMemoryStream::Clear()
{
    FData.clear();
    Position = 0;
}

void TMemoryStream::LoadFromStream(TMemoryStream* Source)
{
    if (Source == 0)
        return;

    Source->Position = 0;
    FData = Source->FData;
    Position = 0;
}

} // namespace vclcompat
