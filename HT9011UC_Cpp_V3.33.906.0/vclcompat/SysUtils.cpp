// ===========================================================================
//  vclcompat/SysUtils.cpp -- BCB6 SysUtils free functions (used subset).
// ===========================================================================
#include "vclcompat/SysUtils.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <sys/stat.h>

#if defined(_WIN32)
#  include <windows.h>
#  include <direct.h>
#else
#  include <unistd.h>
#  include <sys/types.h>
#endif

namespace vclcompat {

// ---------------------------------------------------------------------------
//  numbers <-> strings
// ---------------------------------------------------------------------------
AnsiString IntToStr(int v) { return AnsiString(v); }

// BCB6 IntToHex: uppercase hex, zero-padded to at least `digits` chars.
// (e.g. IntToHex(255,2) -> "FF", IntToHex(10,4) -> "000A".)
AnsiString IntToHex(long long value, int digits) {
    if (digits < 0) digits = 0;
    // Build uppercase hex manually (no 'll' printf modifier -- MinGW's msvcrt
    // printf mishandles %llX, so format the 64-bit value digit-by-digit).
    unsigned long long v = static_cast<unsigned long long>(value);
    char tmp[24];
    int n = 0;
    if (v == 0ULL) {
        tmp[n++] = '0';
    } else {
        while (v != 0ULL && n < (int)sizeof(tmp)) {
            int d = static_cast<int>(v & 0xFULL);
            tmp[n++] = static_cast<char>(d < 10 ? ('0' + d) : ('A' + d - 10));
            v >>= 4;
        }
    }
    std::string out;
    for (int pad = n; pad < digits; ++pad) out += '0';   // zero-pad to >= digits
    for (int i = n - 1; i >= 0; --i) out += tmp[i];       // reverse to MSB-first
    return AnsiString(out);
}
AnsiString IntToHex(int value, int digits) {
    return IntToHex(static_cast<long long>(static_cast<unsigned int>(value)), digits);
}

int    StrToInt(const AnsiString& s)            { return s.ToInt(); }
int    StrToIntDef(const AnsiString& s, int d)  { return s.ToIntDef(d); }
double StrToFloat(const AnsiString& s)          { return s.ToDouble(); }

double StrToFloatDef(const AnsiString& s, double def) {
    AnsiString t = s.Trim();
    if (t.IsEmpty()) return def;
    const char* p = t.c_str();
    char* end = 0;
    double v = std::strtod(p, &end);
    if (end == p || *end != '\0') return def;
    return v;
}

AnsiString FloatToStr(double v) { return AnsiString(v); }  // FloatToStr == AnsiString(double)

// ---- FormatFloat : Delphi picture-string formatter ------------------------
//  Supported picture tokens (the ones the source actually uses, e.g.
//  "0", "0.0", "0.00", "0.000", "0.0000", "#0.00", "0.00%", "00.0"):
//    '0' : digit placeholder, always shown (pads with 0)
//    '#' : digit placeholder, shown only if significant
//    '.' : decimal point (first one is the separator)
//    ',' : thousands grouping (presence anywhere left of '.' enables grouping)
//    '%' : scale by 100 and emit a literal '%'
//  Other chars are emitted literally. Negative numbers get a leading '-'.
AnsiString FormatFloat(const AnsiString& fmtA, double value) {
    const std::string fmt = fmtA.str();
    if (fmt.empty()) return AnsiString(value);   // Delphi: empty fmt -> general

    bool percent = (fmt.find('%') != std::string::npos);
    bool grouping = false;
    int intPlaceholders = 0;     // count of '0'/'#' before decimal
    int decZeros = 0;            // '0' after decimal (mandatory)
    int decHashes = 0;           // '#' after decimal (optional)
    bool seenDot = false;
    for (size_t i = 0; i < fmt.size(); ++i) {
        char c = fmt[i];
        if (c == '.') { seenDot = true; continue; }
        if (c == ',') { grouping = true; continue; }
        if (c == '0' || c == '#') {
            if (!seenDot) intPlaceholders++;
            else if (c == '0') decZeros++;
            else decHashes++;
        }
    }
    int totalDec = decZeros + decHashes;

    double v = value;
    if (percent) v *= 100.0;

    bool neg = false;
    if (v < 0) { neg = true; v = -v; }

    // round to totalDec decimals
    char numbuf[64];
    std::snprintf(numbuf, sizeof(numbuf), "%.*f", totalDec, v);
    std::string num(numbuf);

    // split integer / fractional
    std::string intPart, fracPart;
    std::string::size_type dotp = num.find('.');
    if (dotp == std::string::npos) { intPart = num; }
    else { intPart = num.substr(0, dotp); fracPart = num.substr(dotp + 1); }

    // pad integer part to required mandatory digits ('0' count before dot).
    // count mandatory '0' placeholders before the dot:
    int mandIntZeros = 0;
    { bool dot = false;
      for (size_t i = 0; i < fmt.size(); ++i) {
          if (fmt[i] == '.') { dot = true; }
          else if (!dot && fmt[i] == '0') mandIntZeros++;
      } }
    while (static_cast<int>(intPart.size()) < mandIntZeros)
        intPart.insert(intPart.begin(), '0');
    if (intPart.empty()) intPart = "0";

    // trim trailing optional ('#') fractional digits that are zero.
    if (totalDec > 0) {
        // fracPart currently has exactly totalDec chars (from %.*f).
        int keepMin = decZeros;  // mandatory fractional digits
        int len = static_cast<int>(fracPart.size());
        while (len > keepMin && fracPart[len - 1] == '0') { fracPart.erase(len - 1); --len; }
    }

    // grouping (thousands) on integer part
    if (grouping && intPart.size() > 3) {
        std::string g;
        int cnt = 0;
        for (int i = static_cast<int>(intPart.size()) - 1; i >= 0; --i) {
            g.insert(g.begin(), intPart[static_cast<size_t>(i)]);
            if (++cnt % 3 == 0 && i > 0) g.insert(g.begin(), ',');
        }
        intPart.swap(g);
    }

    std::string out;
    if (neg) out += '-';
    out += intPart;
    if (!fracPart.empty()) { out += '.'; out += fracPart; }
    if (percent) out += '%';
    return AnsiString(out);
}

AnsiString FloatToStrF(double v, TFloatFormat fmt, int precision, int digits) {
    char buf[64];
    switch (fmt) {
        case ffFixed:
            std::snprintf(buf, sizeof(buf), "%.*f", digits, v);
            return AnsiString(std::string(buf));
        case ffExponent:
            std::snprintf(buf, sizeof(buf), "%.*e", (precision > 0 ? precision - 1 : 0), v);
            return AnsiString(std::string(buf));
        case ffNumber: {
            // fixed with thousands grouping
            AnsiString num = FloatToStrF(v, ffFixed, precision, digits);
            return num; // grouping omitted (not exercised by source); faithful enough
        }
        case ffGeneral:
        default:
            std::snprintf(buf, sizeof(buf), "%.*g", (precision > 0 ? precision : 15), v);
            return AnsiString(std::string(buf));
    }
}

// ---------------------------------------------------------------------------
//  filesystem
// ---------------------------------------------------------------------------
bool FileExists(const AnsiString& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return false;
    return (st.st_mode & S_IFMT) != S_IFDIR;   // exists and is not a directory
}

bool DirectoryExists(const AnsiString& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return false;
    return (st.st_mode & S_IFMT) == S_IFDIR;
}

bool CreateDir(const AnsiString& path) {
#if defined(_WIN32)
    return _mkdir(path.c_str()) == 0;
#else
    return mkdir(path.c_str(), 0777) == 0;
#endif
}

bool ForceDirectories(const AnsiString& path) {
    std::string p = path.str();
    if (p.empty()) return false;
    // normalise: walk components, creating each.
    if (DirectoryExists(path)) return true;
    std::string cur;
    for (size_t i = 0; i < p.size(); ++i) {
        char c = p[i];
        cur += c;
        if (c == '/' || c == '\\') {
            if (cur.size() > 1) {
                AnsiString seg(cur.substr(0, cur.size() - 1));
                if (!DirectoryExists(seg) && !seg.IsEmpty()) CreateDir(seg);
            }
        }
    }
    if (!DirectoryExists(path)) CreateDir(path);
    return DirectoryExists(path);
}

bool DeleteFile(const AnsiString& path) {
    return std::remove(path.c_str()) == 0;
}

bool CopyFile(const AnsiString& src, const AnsiString& dst, bool failIfExists) {
#if defined(_WIN32)
    return ::CopyFileA(src.c_str(), dst.c_str(), failIfExists ? TRUE : FALSE) != 0;
#else
    if (failIfExists && FileExists(dst)) return false;
    FILE* in = std::fopen(src.c_str(), "rb");
    if (!in) return false;
    FILE* out = std::fopen(dst.c_str(), "wb");
    if (!out) { std::fclose(in); return false; }
    char buf[8192]; size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), in)) > 0) std::fwrite(buf, 1, n, out);
    std::fclose(in); std::fclose(out);
    return true;
#endif
}

static int lastSepPos(const std::string& p) {
    for (int i = static_cast<int>(p.size()) - 1; i >= 0; --i)
        if (p[static_cast<size_t>(i)] == '\\' || p[static_cast<size_t>(i)] == '/') return i;
    return -1;
}

AnsiString ExtractFilePath(const AnsiString& path) {
    const std::string& p = path.str();
    int s = lastSepPos(p);
    if (s < 0) return AnsiString();
    return AnsiString(p.substr(0, static_cast<size_t>(s) + 1));  // include trailing sep
}

AnsiString ExtractFileName(const AnsiString& path) {
    const std::string& p = path.str();
    int s = lastSepPos(p);
    return AnsiString(p.substr(static_cast<size_t>(s + 1)));
}

AnsiString ExtractFileExt(const AnsiString& path) {
    const std::string& p = path.str();
    int s = lastSepPos(p);
    for (int i = static_cast<int>(p.size()) - 1; i > s; --i) {
        if (p[static_cast<size_t>(i)] == '.')
            return AnsiString(p.substr(static_cast<size_t>(i)));  // include the '.'
    }
    return AnsiString();
}

AnsiString ChangeFileExt(const AnsiString& path, const AnsiString& newExt) {
    const std::string& p = path.str();
    int s = lastSepPos(p);
    int dot = -1;
    for (int i = static_cast<int>(p.size()) - 1; i > s; --i) {
        if (p[static_cast<size_t>(i)] == '.') { dot = i; break; }
    }
    std::string base = (dot >= 0) ? p.substr(0, static_cast<size_t>(dot)) : p;
    return AnsiString(base + newExt.str());
}

AnsiString IncludeTrailingBackslash(const AnsiString& path) {
    const std::string& p = path.str();
    if (p.empty()) return AnsiString("\\");
    char last = p[p.size() - 1];
    if (last == '\\' || last == '/') return path;
    return AnsiString(p + "\\");
}

// ---------------------------------------------------------------------------
//  misc
// ---------------------------------------------------------------------------
void Sleep(int milliseconds) {
#if defined(_WIN32)
    ::Sleep(static_cast<DWORD>(milliseconds < 0 ? 0 : milliseconds));
#else
    if (milliseconds > 0) usleep(static_cast<useconds_t>(milliseconds) * 1000);
#endif
}

} // namespace vclcompat
