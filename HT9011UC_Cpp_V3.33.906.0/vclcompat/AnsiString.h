// ===========================================================================
//  vclcompat/AnsiString.h
//  BCB6 (Borland C++ Builder 6) AnsiString compatibility shim.
//
//  PURPOSE
//  -------
//  Recreate ONLY the AnsiString API surface actually used by the
//  HT9011UC V906 BCB6 source, with semantics that mirror BCB6 *exactly*,
//  so that the bulk of the project can be translated faithfully,
//  name-preserving and low-risk (no need to rewrite ~1,910 1-based
//  off-by-one expressions).
//
//  Translated source files #include "vclcompat/vcl_compat.h" (which pulls
//  this in) instead of <vcl.h>.
//
//  CRITICAL BCB6 SEMANTICS REPLICATED (verified against the golden ref
//  D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618):
//    * 1-BASED indexing throughout:
//        - operator[](i): i==1 is the first char (also usable as l-value).
//        - Pos(sub):      returns 1-based index of first match, or 0 if none.
//                         (real code tests `==1`, `>=1`, `>0`; see e.g.
//                          ainarm*.cpp `.Pos("@SET_TEMPOK")==1`.)
//        - SubString(start,len): `start` is 1-based; start<=0 is clamped to 1
//                         (BCB6 quirk; real code uses `SubString(0, ...)`).
//        - Delete(index,count) / Insert(s,index): index is 1-based.
//        - LastDelimiter(delims): 1-based index of last delim char, 0 if none.
//    * AnsiString(int)    -> formatted decimal string  (e.g. 42 -> "42").
//    * AnsiString(double) -> FloatToStr-style string (up to 15 significant
//                         digits, trailing zeros stripped; e.g. 1.5 -> "1.5",
//                         141.3 -> "141.3", 3.0 -> "3").
//    * operator+ concatenates, with implicit int/double/char/char* operands
//      promoted to AnsiString via the ctors (the dominant build idiom).
//    * sprintf(fmt, ...) formats IN PLACE and returns *this (the single most
//      used member, 7026 uses). printf == sprintf alias. cat_printf appends.
//      To stay source-faithful with the BCB6 idiom `s.sprintf("%s", anAnsiStr)`
//      (args passed as raw AnsiString, NOT .c_str()), the printf-family are
//      variadic *templates* that auto-convert AnsiString args to const char*.
//    * UpperCase()/LowerCase()/Trim()/TrimLeft() return a NEW AnsiString.
//    * Length() is the char count; c_str() returns const char*.
//
//  Out of scope (not implemented): wide-char/UnicodeString, code-page
//  conversions, AnsiString reference-counting internals. Backed by
//  std::string; ASCII / single-byte (Big5 bytes pass through untouched).
// ===========================================================================
#ifndef VCLCOMPAT_ANSISTRING_H
#define VCLCOMPAT_ANSISTRING_H

#include <string>
#include <cstdarg>
#include <cstdio>
#include <type_traits>

namespace vclcompat {

class AnsiString {
public:
    // ---- construction -----------------------------------------------------
    AnsiString() {}
    AnsiString(const AnsiString& o) : data_(o.data_) {}
    AnsiString(const char* s) : data_(s ? s : "") {}
    AnsiString(const char* s, int len)
        : data_(s ? std::string(s, (len > 0 ? static_cast<size_t>(len) : 0)) : std::string()) {}
    AnsiString(const std::string& s) : data_(s) {}
    AnsiString(char c) : data_(1, c) {}

    // BCB6: AnsiString(int) / AnsiString(double) build a formatted number.
    AnsiString(int v)          { assignInt(static_cast<long long>(v)); }
    AnsiString(unsigned int v) { assignInt(static_cast<long long>(v)); }
    AnsiString(long v)         { assignInt(static_cast<long long>(v)); }
    AnsiString(long long v)    { assignInt(v); }
    AnsiString(double v)       { assignDouble(v); }

    AnsiString& operator=(const AnsiString& o) { data_ = o.data_; return *this; }
    AnsiString& operator=(const char* s)       { data_ = (s ? s : ""); return *this; }
    AnsiString& operator=(char c)              { data_.assign(1, c); return *this; }

    // BCB6 AnsiString has operator=(int)/operator=(double) that FORMAT the
    // number (e.g. `s = 7;` -> "7"), mirroring the AnsiString(int) ctor above.
    // Without these explicit overloads, `s = 7;` would bind to operator=(char)
    // (int->char is a standard conversion, preferred over the user-defined
    // int->AnsiString ctor path), silently storing a control byte instead of
    // the decimal text.  This bit the translated database.cpp loaders, whose
    // `mapMotTable[key] = i-1;` (database.cpp:1646) and `mapIOTable[key] = i-1;`
    // (database.cpp:1578) store the row index as formatted text in BCB6.
    AnsiString& operator=(int v)          { assignInt(static_cast<long long>(v)); return *this; }
    AnsiString& operator=(unsigned int v) { assignInt(static_cast<long long>(v)); return *this; }
    AnsiString& operator=(long v)         { assignInt(static_cast<long long>(v)); return *this; }
    AnsiString& operator=(long long v)    { assignInt(v); return *this; }
    AnsiString& operator=(double v)       { assignDouble(v); return *this; }

    // ---- basics ------------------------------------------------------------
    const char* c_str()  const { return data_.c_str(); }
    int         Length() const { return static_cast<int>(data_.size()); }
    bool        IsEmpty() const { return data_.empty(); }
    const std::string& str() const { return data_; }   // helper for SysUtils

    // ---- 1-based char access (operator[]; l-value capable) ----------------
    // BCB6: s[1] is the first char. Index is 1-based and unchecked, exactly
    // like BCB6 (caller is responsible for 1 <= i <= Length()).
    char&       operator[](int i)       { return data_[static_cast<size_t>(i - 1)]; }
    const char& operator[](int i) const { return data_[static_cast<size_t>(i - 1)]; }

    // ---- search (1-based; 0 == not found) ---------------------------------
    int Pos(const AnsiString& sub) const { return posImpl(sub.data_); }
    int Pos(const char* sub) const       { return posImpl(sub ? sub : ""); }
    int Pos(char c) const                { return posImpl(std::string(1, c)); }
    // AnsiPos: multibyte-aware variant in BCB6. For single-byte data this is
    // byte-identical to Pos; we forward to Pos (real code's miss/hit semantics
    // -- 1-based / 0 -- are what matters).
    int AnsiPos(const AnsiString& sub) const { return Pos(sub); }
    int AnsiPos(const char* sub) const       { return Pos(sub); }

    // LastDelimiter: 1-based index of the last char that is one of `delims`.
    int LastDelimiter(const AnsiString& delims) const { return lastDelimImpl(delims.data_); }
    int LastDelimiter(const char* delims) const       { return lastDelimImpl(delims ? delims : ""); }
    int LastDelimiter(char delim) const                { return lastDelimImpl(std::string(1, delim)); }

    // ---- slicing (1-based start; start<=0 clamped to 1, BCB6 quirk) -------
    AnsiString SubString(int start, int len) const;

    // ---- in-place edits (1-based) -----------------------------------------
    AnsiString& Delete(int index, int count);          // 1-based, returns *this
    AnsiString& Insert(const AnsiString& s, int index); // 1-based, returns *this
    void        SetLength(int n);

    // ---- copies (new string) ----------------------------------------------
    AnsiString UpperCase() const;
    AnsiString LowerCase() const;
    AnsiString Trim() const;
    AnsiString TrimLeft() const;

    // ---- numeric conversions ----------------------------------------------
    int    ToInt() const;             // throws-like BCB6 EConvertError -> we throw std::runtime_error
    int    ToIntDef(int def) const;   // returns def on parse failure
    double ToDouble() const;

    // ---- printf family (in place / append), variadic-template form --------
    // Auto-converts AnsiString arguments to const char* so the BCB6 idiom
    //   s.sprintf("%s", anAnsiString)   (raw AnsiString to %s)
    // compiles and behaves correctly under MinGW/standard C++.
    template <typename... Args>
    AnsiString& sprintf(const char* fmt, Args... args) {
        data_ = formatString(fmt, conv(args)...);
        return *this;
    }
    template <typename... Args>
    AnsiString& printf(const char* fmt, Args... args) {       // BCB6 alias of sprintf
        data_ = formatString(fmt, conv(args)...);
        return *this;
    }
    template <typename... Args>
    AnsiString& cat_printf(const char* fmt, Args... args) {   // append-formatted
        data_ += formatString(fmt, conv(args)...);
        return *this;
    }
    // Zero-arg cases (no variadic args): still valid printf calls.
    AnsiString& sprintf(const char* fmt)    { data_ = formatString(fmt); return *this; }
    AnsiString& printf(const char* fmt)     { data_ = formatString(fmt); return *this; }
    AnsiString& cat_printf(const char* fmt) { data_ += formatString(fmt); return *this; }

    // ---- concatenation -----------------------------------------------------
    AnsiString& operator+=(const AnsiString& o) { data_ += o.data_; return *this; }
    AnsiString& operator+=(const char* s)       { if (s) data_ += s; return *this; }
    AnsiString& operator+=(char c)              { data_ += c; return *this; }

private:
    std::string data_;

    void assignInt(long long v);
    void assignDouble(double v);
    int  posImpl(const std::string& sub) const;
    int  lastDelimImpl(const std::string& delims) const;

    // ---- printf helpers ----------------------------------------------------
    // conv(): pass-through for everything except AnsiString, which becomes
    // const char*. This is what lets %s accept a raw AnsiString.
    static const char* conv(const AnsiString& s) { return s.c_str(); }
    template <typename T>
    static T conv(T v) { return v; }   // POD / pointers / numbers unchanged

    // Actual C formatting via vsnprintf, growing the buffer as needed.
    static std::string formatString(const char* fmt, ...);
};

// ===========================================================================
//  Free operators
// ===========================================================================

// operator+ : returns a new AnsiString. Implicit ctors promote int/double/
// char/char* operands, matching BCB6's pervasive build idiom
// (e.g.  "X=" + AnsiString(i) + ", Y=" + dVal ).
inline AnsiString operator+(const AnsiString& a, const AnsiString& b) {
    AnsiString r(a); r += b; return r;
}
inline AnsiString operator+(const AnsiString& a, const char* b) {
    AnsiString r(a); r += b; return r;
}
inline AnsiString operator+(const char* a, const AnsiString& b) {
    AnsiString r(a); r += b; return r;
}
inline AnsiString operator+(const AnsiString& a, char b) {
    AnsiString r(a); r += b; return r;
}
inline AnsiString operator+(char a, const AnsiString& b) {
    AnsiString r(a); r += b; return r;
}
// int / double operands -> promote via ctor (BCB6 implicit number->string).
inline AnsiString operator+(const AnsiString& a, int b)    { return a + AnsiString(b); }
inline AnsiString operator+(int a, const AnsiString& b)    { return AnsiString(a) + b; }
inline AnsiString operator+(const AnsiString& a, double b) { return a + AnsiString(b); }
inline AnsiString operator+(double a, const AnsiString& b) { return AnsiString(a) + b; }

// ---- comparisons : vs AnsiString and vs const char* literal ---------------
inline bool operator==(const AnsiString& a, const AnsiString& b) { return a.str() == b.str(); }
inline bool operator!=(const AnsiString& a, const AnsiString& b) { return a.str() != b.str(); }
inline bool operator==(const AnsiString& a, const char* b)       { return a.str() == (b ? b : ""); }
inline bool operator!=(const AnsiString& a, const char* b)       { return a.str() != (b ? b : ""); }
inline bool operator==(const char* a, const AnsiString& b)       { return (a ? a : "") == b.str(); }
inline bool operator!=(const char* a, const AnsiString& b)       { return (a ? a : "") != b.str(); }

// ---- ordering : used by TStringList::Sort comparators ---------------------
inline bool operator<(const AnsiString& a, const AnsiString& b)  { return a.str() <  b.str(); }
inline bool operator>(const AnsiString& a, const AnsiString& b)  { return a.str() >  b.str(); }
inline bool operator<=(const AnsiString& a, const AnsiString& b) { return a.str() <= b.str(); }
inline bool operator>=(const AnsiString& a, const AnsiString& b) { return a.str() >= b.str(); }

} // namespace vclcompat

#endif // VCLCOMPAT_ANSISTRING_H
