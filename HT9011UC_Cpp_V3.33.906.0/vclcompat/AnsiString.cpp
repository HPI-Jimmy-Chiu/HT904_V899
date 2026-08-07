// ===========================================================================
//  vclcompat/AnsiString.cpp  -- implementation of the BCB6 AnsiString shim.
//  Semantics mirror BCB6 exactly; see AnsiString.h for the contract.
// ===========================================================================
#include "vclcompat/AnsiString.h"

#include <vector>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdio>
#include <stdexcept>

namespace vclcompat {

// ---------------------------------------------------------------------------
//  Number formatting (ctors)
// ---------------------------------------------------------------------------
void AnsiString::assignInt(long long v) {
    // std::to_string is portable across MinGW MSVCRT (which lacks %lld) and MSVC.
    data_ = std::to_string(v);
}

// BCB6 FloatToStr: "general" format, 15 significant digits, trailing zeros and
// a trailing decimal point stripped. 3.0 -> "3", 1.5 -> "1.5",
// 141.3 -> "141.3", 25371.828 -> "25371.828".
void AnsiString::assignDouble(double v) {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.15g", v);
    std::string s(buf);
    // %g already trims trailing zeros for us in the fractional mantissa, and
    // uses the shortest of %e/%f. BCB6 uses '.' as decimal sep in the default
    // (US) locale, which matches %g here. Normalise the exponent form to be
    // safe (BCB6 rarely emits exponents for the magnitudes this project uses,
    // but %.15g may for very large/small values -- keep as-is; faithful).
    data_ = s;
}

// ---------------------------------------------------------------------------
//  Search (1-based; 0 == not found)
// ---------------------------------------------------------------------------
int AnsiString::posImpl(const std::string& sub) const {
    if (sub.empty()) return 0;            // BCB6: Pos("") returns 0
    std::string::size_type p = data_.find(sub);
    return (p == std::string::npos) ? 0 : static_cast<int>(p) + 1;  // 1-based
}

int AnsiString::lastDelimImpl(const std::string& delims) const {
    if (delims.empty()) return 0;
    std::string::size_type p = data_.find_last_of(delims);
    return (p == std::string::npos) ? 0 : static_cast<int>(p) + 1;  // 1-based
}

// ---------------------------------------------------------------------------
//  Slicing (1-based start; start<=0 clamped to 1 per BCB6)
// ---------------------------------------------------------------------------
AnsiString AnsiString::SubString(int start, int len) const {
    if (start <= 0) start = 1;            // BCB6 quirk: index 0 behaves like 1
    if (len <= 0) return AnsiString();
    int n = Length();
    if (start > n) return AnsiString();
    // available chars from `start` (1-based) to end
    int avail = n - (start - 1);
    if (len > avail) len = avail;
    return AnsiString(data_.substr(static_cast<size_t>(start - 1),
                                   static_cast<size_t>(len)));
}

// ---------------------------------------------------------------------------
//  In-place edits (1-based)
// ---------------------------------------------------------------------------
AnsiString& AnsiString::Delete(int index, int count) {
    int n = Length();
    if (index < 1 || index > n || count <= 0) return *this;  // BCB6: no-op out of range
    int avail = n - (index - 1);
    if (count > avail) count = avail;
    data_.erase(static_cast<size_t>(index - 1), static_cast<size_t>(count));
    return *this;
}

AnsiString& AnsiString::Insert(const AnsiString& s, int index) {
    int n = Length();
    if (index < 1) index = 1;             // clamp to front
    if (index > n + 1) index = n + 1;     // clamp to append
    data_.insert(static_cast<size_t>(index - 1), s.data_);
    return *this;
}

void AnsiString::SetLength(int n) {
    if (n < 0) n = 0;
    data_.resize(static_cast<size_t>(n));
}

// ---------------------------------------------------------------------------
//  Case / trim copies
// ---------------------------------------------------------------------------
AnsiString AnsiString::UpperCase() const {
    std::string r = data_;
    for (size_t i = 0; i < r.size(); ++i)
        r[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(r[i])));
    return AnsiString(r);
}

AnsiString AnsiString::LowerCase() const {
    std::string r = data_;
    for (size_t i = 0; i < r.size(); ++i)
        r[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(r[i])));
    return AnsiString(r);
}

// BCB6 Trim removes chars <= ' ' (space and all control chars) from both ends.
AnsiString AnsiString::Trim() const {
    size_t b = 0, e = data_.size();
    while (b < e && static_cast<unsigned char>(data_[b]) <= ' ') ++b;
    while (e > b && static_cast<unsigned char>(data_[e - 1]) <= ' ') --e;
    return AnsiString(data_.substr(b, e - b));
}

AnsiString AnsiString::TrimLeft() const {
    size_t b = 0, e = data_.size();
    while (b < e && static_cast<unsigned char>(data_[b]) <= ' ') ++b;
    return AnsiString(data_.substr(b, e - b));
}

// ---------------------------------------------------------------------------
//  Numeric conversions
// ---------------------------------------------------------------------------
// AI(W906-PT-W1-integrate) 20260807: BCB6's StrToInt / StrToIntDef parse through
//   Val(), which accepts the Pascal '$' and the C '0x' hex prefixes as well as
//   decimal.  Both functions here were decimal-ONLY, which is where it bit:
//   golden EJ1N/TextProcess.cpp:347-356 implements HexStrToInt by PREPENDING
//   "0x" and calling StrToIntDef(S,-1) -- against this shim that body returns -1
//   for every input, including a plain "FF".  EJ1N/TextProcess.h's gate register
//   prescribed retiring vclcompat::HexStrToInt in favour of exactly that body,
//   which would have zeroed every IO_Table / Mot_Table hex Port column and every
//   CCLink LRC byte to -1.  Fixed at the root instead of documented around.
//   The identical parse already existed, correct, at vclcompat/IniFiles.cpp:23
//   (parseIntDef) for TIniFile::ReadInteger -- the two are now consistent.
static bool vc_parseIntBCB6(const AnsiString& in, long& out) {
    AnsiString t = in.Trim();
    if (t.IsEmpty()) return false;
    const char* p = t.c_str();
    char* end = 0;
    const std::string& s = t.str();
    if (s[0] == '$') {                                          // Pascal hex
        if (s.size() < 2) return false;
        out = std::strtol(p + 1, &end, 16);
        return !(end == p + 1 || *end != '\0');
    }
    if (s.size() >= 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {   // C hex
        out = std::strtol(p, &end, 16);
        return !(end == p || *end != '\0');
    }
    out = std::strtol(p, &end, 10);                             // decimal
    return !(end == p || *end != '\0');
}

int AnsiString::ToInt() const {
    // BCB6 StrToInt: leading/trailing space tolerated; full string must be a
    // valid integer (decimal, '$'hex or '0x'hex) or it raises EConvertError.
    // We throw std::runtime_error.
    long v = 0;
    if (!vc_parseIntBCB6(*this, v)) {
        if (Trim().IsEmpty()) throw std::runtime_error("AnsiString::ToInt: empty");
        throw std::runtime_error("AnsiString::ToInt: not an integer");
    }
    return static_cast<int>(v);
}

int AnsiString::ToIntDef(int def) const {
    long v = 0;
    if (!vc_parseIntBCB6(*this, v)) return def;
    return static_cast<int>(v);
}

double AnsiString::ToDouble() const {
    AnsiString t = Trim();
    if (t.IsEmpty()) throw std::runtime_error("AnsiString::ToDouble: empty");
    const char* p = t.c_str();
    char* end = 0;
    double v = std::strtod(p, &end);
    if (end == p || *end != '\0')
        throw std::runtime_error("AnsiString::ToDouble: not a number");
    return v;
}

// ---------------------------------------------------------------------------
//  printf-family backend (vsnprintf with growable buffer)
// ---------------------------------------------------------------------------
std::string AnsiString::formatString(const char* fmt, ...) {
    if (!fmt) return std::string();
    va_list ap;
    va_start(ap, fmt);
    va_list ap2;
    va_copy(ap2, ap);
    int needed = std::vsnprintf(0, 0, fmt, ap);
    va_end(ap);
    if (needed < 0) { va_end(ap2); return std::string(); }
    std::vector<char> buf(static_cast<size_t>(needed) + 1);
    std::vsnprintf(buf.data(), buf.size(), fmt, ap2);
    va_end(ap2);
    return std::string(buf.data(), static_cast<size_t>(needed));
}

} // namespace vclcompat
