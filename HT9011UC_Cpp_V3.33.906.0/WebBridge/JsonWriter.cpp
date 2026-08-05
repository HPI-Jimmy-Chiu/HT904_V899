// AI(W906-WebBridge) 20260805: JSON writer + UTF-8 sanitiser. Policy and the
// reasons behind it are documented in JsonWriter.h -- read that first.
#include "WebBridge/JsonWriter.h"

#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <cmath>
#include <cstring>

#if defined(_WIN32)
#  include <windows.h>
#endif

namespace webbridge {

// ===========================================================================
//  UTF-8 validation / repair
// ===========================================================================
namespace {

// Returns the length in bytes of the well-formed UTF-8 sequence starting at
// s[i], or 0 if there is no well-formed sequence there. Rejects overlong
// encodings, UTF-16 surrogates (U+D800..U+DFFF -- illegal in UTF-8 and rejected
// by browsers' JSON.parse of the transport frame) and anything > U+10FFFF.
std::size_t Utf8SequenceLen(const std::string& s, std::size_t i) {
    const std::size_t n = s.size();
    const unsigned char c0 = static_cast<unsigned char>(s[i]);

    if (c0 < 0x80) {
        return 1;                                   // ASCII
    }
    if (c0 < 0xC2) {
        return 0;                                   // continuation byte, or overlong C0/C1
    }
    std::size_t need;
    unsigned int cp;
    if (c0 < 0xE0)      { need = 1; cp = c0 & 0x1Fu; }
    else if (c0 < 0xF0) { need = 2; cp = c0 & 0x0Fu; }
    else if (c0 < 0xF5) { need = 3; cp = c0 & 0x07u; }
    else                { return 0; }               // F5..FF: beyond U+10FFFF

    if (i + need >= n) {
        return 0;                                   // truncated at end of string
    }
    for (std::size_t k = 1; k <= need; ++k) {
        const unsigned char cc = static_cast<unsigned char>(s[i + k]);
        if ((cc & 0xC0u) != 0x80u) {
            return 0;
        }
        cp = (cp << 6) | (cc & 0x3Fu);
    }
    if (need == 1 && cp < 0x80u)     { return 0; }  // overlong
    if (need == 2 && cp < 0x800u)    { return 0; }  // overlong
    if (need == 3 && cp < 0x10000u)  { return 0; }  // overlong
    if (cp > 0x10FFFFu)              { return 0; }
    if (cp >= 0xD800u && cp <= 0xDFFFu) { return 0; } // surrogate
    return need + 1;
}

const char kReplacement[] = "\xEF\xBF\xBD";   // U+FFFD

// Per-byte repair: keep every byte that starts a well-formed sequence, replace
// anything else with U+FFFD. Always returns valid UTF-8.
std::string RepairUtf8(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    std::size_t i = 0;
    while (i < s.size()) {
        const std::size_t len = Utf8SequenceLen(s, i);
        if (len == 0) {
            out.append(kReplacement, 3);
            ++i;
        } else {
            out.append(s, i, len);
            i += len;
        }
    }
    return out;
}

#if defined(_WIN32)
// CP950 (Big5 with the Microsoft extensions) -> UTF-8. Returns false if the
// input is not wholly valid CP950, so a corrupt string falls through to the
// per-byte repair rather than being half-converted.
bool Cp950ToUtf8(const std::string& s, std::string* out) {
    const int wideLen = ::MultiByteToWideChar(
        950, MB_ERR_INVALID_CHARS, s.data(), static_cast<int>(s.size()), NULL, 0);
    if (wideLen <= 0) {
        return false;
    }
    std::wstring wide;
    wide.resize(static_cast<std::size_t>(wideLen));
    if (::MultiByteToWideChar(950, MB_ERR_INVALID_CHARS, s.data(),
                              static_cast<int>(s.size()), &wide[0], wideLen) != wideLen) {
        return false;
    }
    const int utf8Len = ::WideCharToMultiByte(
        CP_UTF8, 0, wide.data(), wideLen, NULL, 0, NULL, NULL);
    if (utf8Len <= 0) {
        return false;
    }
    std::string utf8;
    utf8.resize(static_cast<std::size_t>(utf8Len));
    if (::WideCharToMultiByte(CP_UTF8, 0, wide.data(), wideLen, &utf8[0], utf8Len,
                              NULL, NULL) != utf8Len) {
        return false;
    }
    out->swap(utf8);
    return true;
}
#endif

} // namespace

bool IsValidUtf8(const std::string& s) {
    std::size_t i = 0;
    while (i < s.size()) {
        const std::size_t len = Utf8SequenceLen(s, i);
        if (len == 0) {
            return false;
        }
        i += len;
    }
    return true;
}

std::string SanitizeToUtf8(const std::string& raw) {
    if (raw.empty()) {
        return raw;
    }
    // (a) already valid UTF-8 -- pass through untouched.
    if (IsValidUtf8(raw)) {
        return raw;
    }
#if defined(_WIN32)
    // (b) assume legacy Big5 / CP950 (what the V899 tree and machine config
    //     files actually contain) and transcode.
    std::string converted;
    if (Cp950ToUtf8(raw, &converted) && IsValidUtf8(converted)) {
        return converted;
    }
#endif
    // (c) last resort: byte-wise U+FFFD replacement. Never emits invalid UTF-8.
    return RepairUtf8(raw);
}

// ===========================================================================
//  Escaping / quoting
// ===========================================================================
std::string JsonQuote(const std::string& raw) {
    const std::string s = SanitizeToUtf8(raw);
    std::string out;
    out.reserve(s.size() + 2);
    out.push_back('"');
    for (std::size_t i = 0; i < s.size(); ++i) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        switch (c) {
            case '"':  out.append("\\\"");  break;
            case '\\': out.append("\\\\");  break;
            case '\b': out.append("\\b");   break;
            case '\f': out.append("\\f");   break;
            case '\n': out.append("\\n");   break;
            case '\r': out.append("\\r");   break;
            case '\t': out.append("\\t");   break;
            default:
                if (c < 0x20u) {
                    // Remaining C0 controls -- RFC 8259 forbids them raw.
                    static const char kHex[] = "0123456789abcdef";
                    out.append("\\u00");
                    out.push_back(kHex[(c >> 4) & 0x0F]);
                    out.push_back(kHex[c & 0x0F]);
                } else {
                    // >= 0x80 bytes are already known-valid UTF-8 at this point.
                    out.push_back(static_cast<char>(c));
                }
                break;
        }
    }
    out.push_back('"');
    return out;
}

// ===========================================================================
//  Numbers
// ===========================================================================
namespace {

// Some locales use ',' as the decimal separator; printf honours the locale, JSON
// does not. Rewrite it so the frame stays parseable wherever the machine ships.
void ForceJsonDecimalPoint(std::string* text) {
    const struct lconv* lc = ::localeconv();
    if (lc == 0 || lc->decimal_point == 0) {
        return;
    }
    const char sep = lc->decimal_point[0];
    if (sep == '.' || sep == '\0') {
        return;
    }
    for (std::size_t i = 0; i < text->size(); ++i) {
        if ((*text)[i] == sep) {
            (*text)[i] = '.';
        }
    }
}

} // namespace

std::string JsonNumber(wb_int64 v) {
    char tmp[32];
    // No %lld under MSVC's older CRTs and no <inttypes.h> guarantee under MinGW
    // 6.3 -- format by hand, which is also locale-proof.
    bool neg = false;
    unsigned long long mag;
    if (v < 0) {
        neg = true;
        // -(min) overflows; go through unsigned.
        mag = static_cast<unsigned long long>(-(v + 1)) + 1ull;
    } else {
        mag = static_cast<unsigned long long>(v);
    }
    int pos = 31;
    tmp[pos] = '\0';
    if (mag == 0) {
        tmp[--pos] = '0';
    }
    while (mag > 0) {
        tmp[--pos] = static_cast<char>('0' + static_cast<int>(mag % 10ull));
        mag /= 10ull;
    }
    if (neg) {
        tmp[--pos] = '-';
    }
    return std::string(&tmp[pos]);
}

std::string JsonNumber(double v) {
    // NaN and +/-Inf are not representable in JSON. Emitting them makes the
    // browser throw on JSON.parse and lose the whole frame, so map to null --
    // which the web side already renders as "---" (unknown).
    if (v != v) {
        return "null";                       // NaN
    }
    if (v > 1.7976931348623157e308 || v < -1.7976931348623157e308) {
        return "null";                       // +/-Inf
    }

    // Shortest form that round-trips exactly. 15 digits covers essentially all
    // real temperature/position values; 16 and 17 are the exactness backstops.
    char buf[64];
    for (int prec = 15; prec <= 17; ++prec) {
        std::snprintf(buf, sizeof(buf), "%.*g", prec, v);
        std::string text(buf);
        ForceJsonDecimalPoint(&text);
        if (std::strtod(text.c_str(), 0) == v) {
            return text;
        }
    }
    std::snprintf(buf, sizeof(buf), "%.17g", v);
    std::string text(buf);
    ForceJsonDecimalPoint(&text);
    return text;
}

// ===========================================================================
//  JsonWriter
// ===========================================================================
JsonWriter::JsonWriter()
    : needComma_(false), keyPending_(false), ok_(true) {}

void JsonWriter::Clear() {
    buf_.clear();
    stack_.clear();
    needComma_  = false;
    keyPending_ = false;
    ok_         = true;
}

bool JsonWriter::Ok() const {
    return ok_ && stack_.empty() && !keyPending_;
}

void JsonWriter::BeforeValue() {
    if (!stack_.empty() && stack_.back() == kCtxObject && !keyPending_) {
        ok_ = false;              // object members need a Key() first
    }
    if (needComma_) {
        buf_.push_back(',');
    }
    keyPending_ = false;
    needComma_  = true;
}

JsonWriter& JsonWriter::Key(const std::string& name) {
    if (stack_.empty() || stack_.back() != kCtxObject || keyPending_) {
        ok_ = false;
    }
    if (needComma_) {
        buf_.push_back(',');
        needComma_ = false;
    }
    buf_.append(JsonQuote(name));
    buf_.push_back(':');
    keyPending_ = true;
    return *this;
}

JsonWriter& JsonWriter::BeginObject() {
    BeforeValue();
    buf_.push_back('{');
    stack_.push_back(static_cast<char>(kCtxObject));
    needComma_ = false;
    return *this;
}

JsonWriter& JsonWriter::EndObject() {
    if (stack_.empty() || stack_.back() != kCtxObject || keyPending_) {
        ok_ = false;
        return *this;
    }
    stack_.pop_back();
    buf_.push_back('}');
    needComma_ = true;
    return *this;
}

JsonWriter& JsonWriter::BeginArray() {
    BeforeValue();
    buf_.push_back('[');
    stack_.push_back(static_cast<char>(kCtxArray));
    needComma_ = false;
    return *this;
}

JsonWriter& JsonWriter::EndArray() {
    if (stack_.empty() || stack_.back() != kCtxArray || keyPending_) {
        ok_ = false;
        return *this;
    }
    stack_.pop_back();
    buf_.push_back(']');
    needComma_ = true;
    return *this;
}

JsonWriter& JsonWriter::Null() {
    BeforeValue();
    buf_.append("null");
    return *this;
}

JsonWriter& JsonWriter::Bool(bool v) {
    BeforeValue();
    buf_.append(v ? "true" : "false");
    return *this;
}

JsonWriter& JsonWriter::Number(wb_int64 v) {
    BeforeValue();
    buf_.append(JsonNumber(v));
    return *this;
}

JsonWriter& JsonWriter::Number(double v) {
    BeforeValue();
    buf_.append(JsonNumber(v));
    return *this;
}

JsonWriter& JsonWriter::String(const std::string& v) {
    BeforeValue();
    buf_.append(JsonQuote(v));
    return *this;
}

JsonWriter& JsonWriter::RawValue(const std::string& jsonText) {
    BeforeValue();
    buf_.append(jsonText);
    return *this;
}

// ===========================================================================
//  JsonValue
// ===========================================================================
JsonValue::JsonValue()
    : kind_(kNull), b_(false), i_(0), d_(0.0) {}

JsonValue JsonValue::MakeNull() {
    return JsonValue();
}

JsonValue JsonValue::MakeBool(bool v) {
    JsonValue x;
    x.kind_ = kBool;
    x.b_ = v;
    return x;
}

JsonValue JsonValue::MakeInt(wb_int64 v) {
    JsonValue x;
    x.kind_ = kInt;
    x.i_ = v;
    return x;
}

JsonValue JsonValue::MakeDouble(double v) {
    JsonValue x;
    x.kind_ = kDouble;
    x.d_ = v;
    return x;
}

JsonValue JsonValue::MakeString(const std::string& v) {
    JsonValue x;
    x.kind_ = kString;
    x.s_ = v;
    return x;
}

bool JsonValue::operator==(const JsonValue& o) const {
    if (kind_ != o.kind_) {
        return false;            // null != "" -- see JsonWriter.h note 1
    }
    switch (kind_) {
        case kNull:   return true;
        case kBool:   return b_ == o.b_;
        case kInt:    return i_ == o.i_;
        case kDouble:
            // Bitwise-ish compare so a NaN-valued tag does not look like it is
            // changing on every tick (NaN != NaN would make the differ chatter).
            if (d_ != d_ && o.d_ != o.d_) {
                return true;
            }
            return d_ == o.d_;
        case kString: return s_ == o.s_;
    }
    return false;
}

std::string JsonValue::ToJson() const {
    switch (kind_) {
        case kNull:   return "null";
        case kBool:   return b_ ? "true" : "false";
        case kInt:    return JsonNumber(i_);
        case kDouble: return JsonNumber(d_);
        case kString: return JsonQuote(s_);
    }
    return "null";
}

void JsonValue::WriteTo(JsonWriter& w) const {
    switch (kind_) {
        case kNull:   w.Null();       break;
        case kBool:   w.Bool(b_);     break;
        case kInt:    w.Number(i_);   break;
        case kDouble: w.Number(d_);   break;
        case kString: w.String(s_);   break;
        default:      w.Null();       break;
    }
}

} // namespace webbridge
