// ===========================================================================
//  WebBridge/TagValue.cpp
//  AI(W906-WebBridge-State) 20260805: new file. See TagValue.h for the design
//  rationale (why not std::variant, why Null != "").
// ===========================================================================
#include "WebBridge/TagValue.h"

namespace webbridge {

// ---------------------------------------------------------------------------
//  Construction
// ---------------------------------------------------------------------------
TagValue::TagValue()
    : type_(TagType::Null), b_(false), i_(0), d_(0.0), s_()
{
}

TagValue TagValue::makeNull()
{
    return TagValue();
}

TagValue TagValue::makeBool(bool v)
{
    TagValue t;
    t.type_ = TagType::Bool;
    t.b_    = v;
    return t;
}

TagValue TagValue::makeInt(std::int64_t v)
{
    TagValue t;
    t.type_ = TagType::Int;
    t.i_    = v;
    return t;
}

TagValue TagValue::makeDouble(double v)
{
    TagValue t;
    t.type_ = TagType::Double;
    t.d_    = v;
    return t;
}

TagValue TagValue::makeString(const std::string& v)
{
    TagValue t;
    t.type_ = TagType::String;
    t.s_    = v;          // an empty v yields String("") -- deliberately NOT Null
    return t;
}

// ---------------------------------------------------------------------------
//  Inspection
// ---------------------------------------------------------------------------
bool TagValue::asBool(bool fallback) const
{
    return (type_ == TagType::Bool) ? b_ : fallback;
}

std::int64_t TagValue::asInt(std::int64_t fallback) const
{
    return (type_ == TagType::Int) ? i_ : fallback;
}

double TagValue::asDouble(double fallback) const
{
    return (type_ == TagType::Double) ? d_ : fallback;
}

const std::string& TagValue::asString() const
{
    // A function-local static empty string so this can return a reference for
    // the non-String case without every TagValue paying for a string it does
    // not use. Never written to, so no thread-safety question arises beyond
    // the (C++11-guaranteed, thread-safe) one-time initialisation.
    static const std::string kEmpty;
    return (type_ == TagType::String) ? s_ : kEmpty;
}

double TagValue::toNumber(double fallback) const
{
    if (type_ == TagType::Int)    return static_cast<double>(i_);
    if (type_ == TagType::Double) return d_;
    return fallback;
}

// ---------------------------------------------------------------------------
//  Equality (the diff predicate)
// ---------------------------------------------------------------------------
namespace {

// Diff-stability double comparison: see the operator== comment in TagValue.h.
// NaN compares equal to NaN so that a NaN-valued tag does not report "changed"
// on every publish tick. (x != x) is the include-free NaN test.
bool diffStableDoubleEq(double a, double b)
{
    if (a != a && b != b) return true;   // both NaN
    return a == b;                       // includes +0.0 == -0.0
}

} // anonymous namespace

bool TagValue::operator==(const TagValue& o) const
{
    if (type_ != o.type_) return false;   // <-- Null vs String("") lands here

    switch (type_) {
    case TagType::Null:   return true;                 // Null == Null
    case TagType::Bool:   return b_ == o.b_;
    case TagType::Int:    return i_ == o.i_;
    case TagType::Double: return diffStableDoubleEq(d_, o.d_);
    case TagType::String: return s_ == o.s_;           // exact bytes
    }
    return false;   // unreachable; keeps every compiler quiet
}

// ---------------------------------------------------------------------------
//  Diagnostics -- NOT the wire format
// ---------------------------------------------------------------------------
const char* TagValue::typeName() const
{
    switch (type_) {
    case TagType::Null:   return "Null";
    case TagType::Bool:   return "Bool";
    case TagType::Int:    return "Int";
    case TagType::Double: return "Double";
    case TagType::String: return "String";
    }
    return "?";
}

std::string TagValue::debugString() const
{
    switch (type_) {
    case TagType::Null:
        return "Null";
    case TagType::Bool:
        return b_ ? "Bool(true)" : "Bool(false)";
    case TagType::Int:
        return "Int(" + std::to_string(i_) + ")";
    case TagType::Double:
        return "Double(" + std::to_string(d_) + ")";
    case TagType::String:
        // No JSON escaping here on purpose -- this is for a printf in a test
        // log, not for a frame on the wire.
        return "Str(\"" + s_ + "\")";
    }
    return "?";
}

} // namespace webbridge
