// ===========================================================================
//  vclcompat/SysUtils.cpp -- BCB6 SysUtils free functions (used subset).
// ===========================================================================
#include "vclcompat/SysUtils.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <string>
#include <sys/stat.h>

#if defined(_WIN32)
#  include <windows.h>
#  include <direct.h>
// windows.h defines A/W macros that would rename our function DEFINITIONS to
// their *A variants (e.g. `#define DeleteFile DeleteFileA`), so the public
// vclcompat::DeleteFile / CopyFile symbols would never be emitted and callers
// would fail to link. Undefine the ones that collide with our API names; the
// real Win32 entry points are still reachable via their explicit *A spellings
// (CopyFileA / DeleteFileA / etc.) used inside the implementations below.
#  undef DeleteFile
#  undef CopyFile
#  undef RemoveDirectory
#  undef CreateDirectory
#  undef FindFirstFile
#  undef FindNextFile
#  undef FindClose
#  undef SetFileAttributes
#  undef GetFileAttributes
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

// HexStrToInt : faithful net behaviour of the golden-ref project function
//   (EJ1N/TextProcess.cpp:347)
//       AnsiString S = (str.AnsiPos("0x")==0) ? "0x"+str : str;
//       return StrToIntDef(S, -1);
//   i.e. ensure a "0x" prefix, then hex-parse with a -1 default.  BCB6's
//   StrToIntDef also accepts the Pascal '$' hex prefix, so we honour both.
//   Leading/trailing whitespace is trimmed (BCB6 StrToInt tolerates it).
int HexStrToInt(const AnsiString& s) {
    AnsiString t = s.Trim();
    if (t.IsEmpty()) return -1;
    const std::string& raw = t.str();

    // strip an existing "0x"/"0X" or "$" prefix; parse the remaining digits.
    std::string digits;
    if (raw.size() >= 2 && raw[0] == '0' && (raw[1] == 'x' || raw[1] == 'X')) {
        digits = raw.substr(2);
    } else if (raw[0] == '$') {
        digits = raw.substr(1);
    } else {
        digits = raw;          // bare hex digits ("0x" gets conceptually prepended)
    }
    if (digits.empty()) return -1;

    char* end = 0;
    long v = std::strtol(digits.c_str(), &end, 16);   // base-16 parse
    if (end == digits.c_str() || *end != '\0') return -1;  // unparseable -> -1
    return static_cast<int>(v);
}
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
//  StringReplace (AI(W5-Final-TesterTCPSocket) 20260711 -- see SysUtils.h)
// ---------------------------------------------------------------------------
AnsiString StringReplace(const AnsiString& S, const AnsiString& OldPattern,
                         const AnsiString& NewPattern, TReplaceFlags Flags) {
    const std::string& oldp = OldPattern.str();
    if (oldp.empty()) return S;   // no-op guard: avoids an infinite replace loop

    std::string hay = S.str();
    const std::string& newp = NewPattern.str();
    bool ignoreCase = Flags.Has(rfIgnoreCase);

    std::string hayCmp = hay;
    std::string oldCmp = oldp;
    if (ignoreCase) {
        for (size_t i = 0; i < hayCmp.size(); ++i) hayCmp[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(hayCmp[i])));
        for (size_t i = 0; i < oldCmp.size(); ++i) oldCmp[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(oldCmp[i])));
    }

    std::string out;
    size_t pos = 0;
    bool all = Flags.Has(rfReplaceAll);
    for (;;) {
        size_t found = hayCmp.find(oldCmp, pos);
        if (found == std::string::npos) {
            out += hay.substr(pos);
            break;
        }
        out += hay.substr(pos, found - pos);
        out += newp;
        pos = found + oldp.size();
        if (!all) {
            out += hay.substr(pos);
            break;
        }
    }
    return AnsiString(out);
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
    // AI(W906-fire-integrate) 20260716: BUG FIX -- this MinGW/msvcrt stat()
    // implementation FAILS (returns nonzero / ENOENT) on a path with a
    // trailing '/' or '\\' even when the directory genuinely exists
    // (empirically reproduced: stat("dir") succeeds, stat("dir\\") and
    // stat("dir/") both fail on this toolchain). Real BCB6/VCL
    // DirectoryExists tolerates a trailing backslash fine, and this
    // codebase's own directory-path globals (DataPath/sSaveByMachine/
    // asN06_TesterPath/OffsetPath/DefaultPath/...) ALWAYS carry one (golden
    // convention) -- so without this fix, DirectoryExists(anyOfThose) was
    // unconditionally false even immediately after successfully creating
    // that exact directory, and ForceDirectories' own final `return
    // DirectoryExists(path);` inherited the same false-negative for any
    // trailing-separator path. Surfaced by tests/test_TesterTCP.cpp's
    // btnSaveClick(SaveByMachine branch) case (Interface/TesterTCP.cpp calls
    // Gated_MyForceDirectories(sSaveByMachine) -- a trailing-backslash
    // global -- and the test independently re-checks DirectoryExists on the
    // same global). Strip a single trailing separator before stat() (down to
    // length 1, so a bare "\\"/"/" root is left alone) -- matches real BCB6
    // semantics without changing behavior for any already-passing caller
    // (grepped: no existing test/production call site relies on
    // DirectoryExists returning false for a trailing-separator path).
    std::string p = path.str();
    while (p.size() > 1 && (p[p.size() - 1] == '/' || p[p.size() - 1] == '\\'))
        p.erase(p.size() - 1);
    struct stat st;
    if (stat(p.c_str(), &st) != 0) return false;
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
//  directory removal / file attributes
// ---------------------------------------------------------------------------
bool RemoveDir(const AnsiString& path) {
#if defined(_WIN32)
    return ::RemoveDirectoryA(path.c_str()) != 0;
#else
    return rmdir(path.c_str()) == 0;
#endif
}

int FileSetAttr(const AnsiString& path, int attr) {
#if defined(_WIN32)
    // BCB6 FileSetAttr: 0 on success, otherwise a Win32 error code.
    if (::SetFileAttributesA(path.c_str(), static_cast<DWORD>(attr)) != 0) return 0;
    return static_cast<int>(::GetLastError());
#else
    (void)path; (void)attr;   // no attribute model on POSIX; treat as success.
    return 0;
#endif
}

int FileGetAttr(const AnsiString& path) {
#if defined(_WIN32)
    DWORD a = ::GetFileAttributesA(path.c_str());
    if (a == INVALID_FILE_ATTRIBUTES) return -1;
    return static_cast<int>(a);
#else
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return -1;
    int attr = 0;
    if ((st.st_mode & S_IFMT) == S_IFDIR) attr |= faDirectory;
    return attr;
#endif
}

// ---------------------------------------------------------------------------
//  FILE-SYSTEM SEARCH (FindFirst/FindNext/FindClose + TSearchRec)
//  Win32-backed (FindFirstFileA/FindNextFileA/FindClose).  BCB6 VCL semantics
//  reproduced exactly (see SysUtils.h header comment for the filter rule).
// ---------------------------------------------------------------------------
#if defined(_WIN32)

// Map a WIN32_FIND_DATAA into the BCB6-visible TSearchRec fields, masking the
// attribute to the fa* bit set the way BCB6 does (Win32 attrs are a superset).
static void fillSearchRec(TSearchRec& sr, const WIN32_FIND_DATAA& fd) {
    sr.Name = AnsiString(fd.cFileName);
    sr.Attr = static_cast<int>(fd.dwFileAttributes) & faAnyFile;  // mask to fa* bits
    // size: combine high/low into __int64
    sr.Size = (static_cast<long long>(fd.nFileSizeHigh) << 32)
            | static_cast<long long>(fd.nFileSizeLow);
    // time: BCB6 stores a DOS-packed last-write time (Integer).
    FILETIME lft;
    WORD dosDate = 0, dosTime = 0;
    if (::FileTimeToLocalFileTime(&fd.ftLastWriteTime, &lft) &&
        ::FileTimeToDosDateTime(&lft, &dosDate, &dosTime)) {
        sr.Time = (static_cast<int>(dosDate) << 16) | static_cast<int>(dosTime);
    } else {
        sr.Time = 0;
    }
}

// BCB6 ExcludeAttr: bits that, if present on an entry, REJECT it. Computed as
//   (NOT Attr) AND (faHidden | faSysFile | faVolumeID | faDirectory).
// (Archive/ReadOnly are never filtered -- they are "always allowed" volatiles.)
static int computeExcludeAttr(int attr) {
    const int filterable = faHidden | faSysFile | faVolumeID | faDirectory;
    return (~attr) & filterable;
}

// Advance until the current FindData satisfies the ExcludeAttr filter.
// Returns 0 if a matching entry is in sr.FindData, non-zero when exhausted.
static int skipExcluded(TSearchRec& sr) {
    WIN32_FIND_DATAA* fd = static_cast<WIN32_FIND_DATAA*>(sr.FindData);
    for (;;) {
        int entryAttr = static_cast<int>(fd->dwFileAttributes) & faAnyFile;
        if ((entryAttr & sr.ExcludeAttr) == 0) {     // not excluded -> keep
            fillSearchRec(sr, *fd);
            return 0;
        }
        if (!::FindNextFileA(static_cast<HANDLE>(sr.FindHandle), fd)) {
            return static_cast<int>(::GetLastError());   // exhausted
        }
    }
}

int FindFirst(const AnsiString& path, int attr, TSearchRec& sr) {
    // (re)initialise the record's search state.
    if (sr.FindHandle && sr.FindHandle != INVALID_HANDLE_VALUE)
        ::FindClose(static_cast<HANDLE>(sr.FindHandle));
    if (!sr.FindData) sr.FindData = new WIN32_FIND_DATAA;
    sr.FindHandle  = INVALID_HANDLE_VALUE;
    sr.ExcludeAttr = computeExcludeAttr(attr);

    WIN32_FIND_DATAA* fd = static_cast<WIN32_FIND_DATAA*>(sr.FindData);
    HANDLE h = ::FindFirstFileA(path.c_str(), fd);
    if (h == INVALID_HANDLE_VALUE) {
        return static_cast<int>(::GetLastError());   // non-zero: no match (BCB6)
    }
    sr.FindHandle = h;
    return skipExcluded(sr);   // honour the attribute filter on the first hit
}

int FindNext(TSearchRec& sr) {
    if (!sr.FindHandle || sr.FindHandle == INVALID_HANDLE_VALUE || !sr.FindData)
        return -1;
    WIN32_FIND_DATAA* fd = static_cast<WIN32_FIND_DATAA*>(sr.FindData);
    if (!::FindNextFileA(static_cast<HANDLE>(sr.FindHandle), fd))
        return static_cast<int>(::GetLastError());   // exhausted -> non-zero
    return skipExcluded(sr);
}

void FindClose(TSearchRec& sr) {
    if (sr.FindHandle && sr.FindHandle != INVALID_HANDLE_VALUE) {
        ::FindClose(static_cast<HANDLE>(sr.FindHandle));
        sr.FindHandle = INVALID_HANDLE_VALUE;
    }
    if (sr.FindData) {
        delete static_cast<WIN32_FIND_DATAA*>(sr.FindData);
        sr.FindData = 0;
    }
}

#else   // ----- non-Windows fallback (not the production target) -------------
// The migration target is Windows/MinGW; a POSIX implementation is provided
// only so the unit can compile in a portable test sandbox. It honours Name/
// Attr (file-vs-dir) and the faDirectory filter bit; Size/Time are best-effort.
} // namespace vclcompat  (re-open after including dirent below)
#include <dirent.h>
#include <fnmatch.h>
namespace vclcompat {
struct PosixFind {
    DIR*        dir;
    std::string dirPath;     // directory portion (with trailing sep)
    std::string pattern;     // glob portion (e.g. "*.txt")
};
static int posixNext(TSearchRec& sr) {
    PosixFind* pf = static_cast<PosixFind*>(sr.FindData);
    struct dirent* de;
    while ((de = readdir(pf->dir)) != 0) {
        if (fnmatch(pf->pattern.c_str(), de->d_name, 0) != 0) continue;
        std::string full = pf->dirPath + de->d_name;
        struct stat st;
        int attr = 0;
        long long size = 0;
        if (stat(full.c_str(), &st) == 0) {
            if ((st.st_mode & S_IFMT) == S_IFDIR) attr |= faDirectory;
            else size = static_cast<long long>(st.st_size);
        }
        if ((attr & sr.ExcludeAttr) != 0) continue;   // honour filter
        sr.Name = AnsiString(de->d_name);
        sr.Attr = attr;
        sr.Size = size;
        sr.Time = 0;
        return 0;
    }
    return -1;
}
int FindFirst(const AnsiString& path, int attr, TSearchRec& sr) {
    if (sr.FindData) { /* stale */ FindClose(sr); }
    std::string p = path.str();
    std::string dirPart, pat;
    int s = lastSepPos(p);
    if (s < 0) { dirPart = "./"; pat = p; }
    else { dirPart = p.substr(0, static_cast<size_t>(s) + 1); pat = p.substr(static_cast<size_t>(s) + 1); }
    if (pat.empty()) pat = "*";
    PosixFind* pf = new PosixFind;
    pf->dir = opendir(dirPart.c_str());
    pf->dirPath = dirPart;
    pf->pattern = pat;
    if (!pf->dir) { delete pf; return -1; }
    sr.FindData = pf;
    sr.FindHandle = pf;
    sr.ExcludeAttr = (~attr) & (faHidden | faSysFile | faVolumeID | faDirectory);
    return posixNext(sr);
}
int FindNext(TSearchRec& sr) {
    if (!sr.FindData) return -1;
    return posixNext(sr);
}
void FindClose(TSearchRec& sr) {
    if (sr.FindData) {
        PosixFind* pf = static_cast<PosixFind*>(sr.FindData);
        if (pf->dir) closedir(pf->dir);
        delete pf;
        sr.FindData = 0;
        sr.FindHandle = 0;
    }
}
#endif  // _WIN32

// TSearchRec destructor: release any dangling Win32/POSIX search state so a
// rec going out of scope without an explicit FindClose does not leak.
TSearchRec::~TSearchRec() {
    if (FindData || (FindHandle && FindHandle !=
#if defined(_WIN32)
            INVALID_HANDLE_VALUE
#else
            0
#endif
        )) {
        FindClose(*this);
    }
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

// ---------------------------------------------------------------------------
//  TryStrToFloat (BCB6 SysUtils)
//  common.cpp:1053-1054, 1200-1201 -- used to detect whether a stored INI
//  string and a new value are both numeric before comparing them as doubles
//  in WriteIniData(AnsiString) / WriteIniData1.
//
//  BCB6 semantics: locale-independent '.' decimal parse; returns false when
//  the string is empty, NULL, or contains non-numeric characters.  The result
//  double reference is only modified on success, exactly matching BCB6's
//  TryStrToFloat(const string, double&):bool signature.
// ---------------------------------------------------------------------------
bool TryStrToFloat(const char* s, double& value) {
    if (!s || *s == '\0') return false;
    char* end = nullptr;
    // strtod is locale-sensitive on some platforms; we force '.' by using a
    // locale-independent parse identical to the IniFiles ReadFloat shim.
    double v = std::strtod(s, &end);
    if (end == s) return false;                     // no conversion at all
    // skip trailing whitespace
    while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n') ++end;
    if (*end != '\0') return false;                 // garbage after the number
    value = v;
    return true;
}

} // namespace vclcompat
