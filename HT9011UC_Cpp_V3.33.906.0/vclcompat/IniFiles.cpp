// ===========================================================================
//  vclcompat/IniFiles.cpp  -- implementation of the BCB6 TIniFile / TMemIniFile
//  shims.  Semantics mirror BCB6 exactly; see IniFiles.h for the contract.
// ===========================================================================
#include "vclcompat/IniFiles.h"
#include "vclcompat/SysUtils.h"   // FileExists

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>

namespace vclcompat {

// ===========================================================================
//  Local parsing helpers -- BCB6-faithful, locale-independent.
// ===========================================================================
namespace {

// BCB6 StrToIntDef-like: decimal, or Pascal '$' / C '0x' hex prefix; a leading
// sign is tolerated; the WHOLE trimmed string must parse or we return def.
// (TIniFile::ReadInteger == StrToIntDef(ReadString(...), Default) in BCB6.)
int parseIntDef(const std::string& raw, int def) {
    // trim (chars <= ' ', matching AnsiString::Trim)
    size_t b = 0, e = raw.size();
    while (b < e && static_cast<unsigned char>(raw[b]) <= ' ') ++b;
    while (e > b && static_cast<unsigned char>(raw[e - 1]) <= ' ') --e;
    if (b >= e) return def;
    std::string s = raw.substr(b, e - b);

    const char* p = s.c_str();
    char* end = 0;
    long v;
    if (s.size() >= 1 && s[0] == '$') {                 // Pascal hex literal
        v = std::strtol(p + 1, &end, 16);
        if (end == p + 1 || *end != '\0') return def;
    } else if (s.size() >= 2 && s[0] == '0' &&
               (s[1] == 'x' || s[1] == 'X')) {          // C hex literal
        v = std::strtol(p, &end, 16);
        if (end == p || *end != '\0') return def;
    } else {                                            // decimal
        v = std::strtol(p, &end, 10);
        if (end == p || *end != '\0') return def;
    }
    return static_cast<int>(v);
}

// Locale-independent decimal parse ('.' separator).  Whole trimmed string must
// parse or we return def.  (TIniFile::ReadFloat semantics.)
double parseFloatDef(const std::string& raw, double def) {
    size_t b = 0, e = raw.size();
    while (b < e && static_cast<unsigned char>(raw[b]) <= ' ') ++b;
    while (e > b && static_cast<unsigned char>(raw[e - 1]) <= ' ') --e;
    if (b >= e) return def;
    std::string s = raw.substr(b, e - b);
    const char* p = s.c_str();
    char* end = 0;
    double v = std::strtod(p, &end);
    if (end == p || *end != '\0') return def;
    return v;
}

} // anonymous namespace

// ===========================================================================
//  TIniStore
// ===========================================================================

// ASCII case-insensitive byte compare; bytes >= 0x80 (Big5 lead/trail) compared
// verbatim so multibyte section/value names round-trip without corruption.
bool TIniStore::iequal(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        unsigned char ca = static_cast<unsigned char>(a[i]);
        unsigned char cb = static_cast<unsigned char>(b[i]);
        if (ca < 0x80) ca = static_cast<unsigned char>(std::toupper(ca));
        if (cb < 0x80) cb = static_cast<unsigned char>(std::toupper(cb));
        if (ca != cb) return false;
    }
    return true;
}

TIniStore::Section* TIniStore::findSection(const AnsiString& name) {
    for (size_t i = 0; i < sections_.size(); ++i)
        if (iequal(sections_[i].name, name.str())) return &sections_[i];
    return 0;
}

const TIniStore::Section* TIniStore::findSection(const AnsiString& name) const {
    for (size_t i = 0; i < sections_.size(); ++i)
        if (iequal(sections_[i].name, name.str())) return &sections_[i];
    return 0;
}

bool TIniStore::SectionExists(const AnsiString& section) const {
    return findSection(section) != 0;
}

bool TIniStore::ValueExists(const AnsiString& section, const AnsiString& ident) const {
    const Section* s = findSection(section);
    if (!s) return false;
    for (size_t i = 0; i < s->items.size(); ++i)
        if (iequal(s->items[i].key, ident.str())) return true;
    return false;
}

AnsiString TIniStore::ReadRaw(const AnsiString& section, const AnsiString& ident,
                              bool& found) const {
    found = false;
    const Section* s = findSection(section);
    if (!s) return AnsiString();
    for (size_t i = 0; i < s->items.size(); ++i) {
        if (iequal(s->items[i].key, ident.str())) {
            found = true;
            return AnsiString(s->items[i].val);
        }
    }
    return AnsiString();
}

void TIniStore::WriteRaw(const AnsiString& section, const AnsiString& ident,
                         const AnsiString& value) {
    Section* s = findSection(section);
    if (!s) {
        Section ns;
        ns.name = section.str();
        sections_.push_back(ns);
        s = &sections_.back();
    }
    for (size_t i = 0; i < s->items.size(); ++i) {
        if (iequal(s->items[i].key, ident.str())) {
            s->items[i].val = value.str();
            return;
        }
    }
    KeyVal kv;
    kv.key = ident.str();
    kv.val = value.str();
    s->items.push_back(kv);
}

void TIniStore::FillSectionKeys(const AnsiString& section, TStrings* dest) const {
    if (!dest) return;
    dest->Clear();
    const Section* s = findSection(section);
    if (!s) return;
    for (size_t i = 0; i < s->items.size(); ++i)
        dest->Add(AnsiString(s->items[i].key));
}

void TIniStore::FillSectionNames(TStrings* dest) const {
    if (!dest) return;
    dest->Clear();
    for (size_t i = 0; i < sections_.size(); ++i)
        dest->Add(AnsiString(sections_[i].name));
}

// ---------------------------------------------------------------------------
//  Disk I/O.  INI grammar (BCB6 / Win32 profile):
//    * '[name]' on its own line opens a section (trailing ']' may be absent;
//      we take up to the last ']' or end of line, then trim).
//    * 'key=value' adds a key under the current section. The FIRST '=' splits;
//      key is trimmed of surrounding blanks; value keeps everything after '='
//      VERBATIM (no trim) -- BCB6 preserves trailing/internal value spaces, and
//      this family round-trips '%0.4f' / Big5 text that must not be mangled.
//    * Keys before any [section] are dropped (no global section in this family).
//    * ';' / '#' comment lines and blank lines are skipped. (BCB6 only treats
//      ';' as a comment; we also skip '#'-lines defensively. The real config
//      files in this project use neither inside the keys this family reads.)
//    * Line endings: CR, LF, or CRLF all accepted (real files are CRLF).
// ---------------------------------------------------------------------------
void TIniStore::LoadFromFile(const AnsiString& path) {
    sections_.clear();

    std::FILE* fp = std::fopen(path.c_str(), "rb");
    if (!fp) return;   // missing file => empty store (reads then fall to Default)

    std::string content;
    char buf[8192];
    size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), fp)) > 0)
        content.append(buf, n);
    std::fclose(fp);

    Section* cur = 0;
    size_t i = 0, len = content.size();
    while (i < len) {
        // extract one raw line (without the EOL bytes)
        size_t start = i;
        while (i < len && content[i] != '\n' && content[i] != '\r') ++i;
        std::string line = content.substr(start, i - start);
        // consume EOL (handle CRLF as one)
        if (i < len && content[i] == '\r') { ++i; if (i < len && content[i] == '\n') ++i; }
        else if (i < len && content[i] == '\n') { ++i; }

        // left-trim for structural detection (value bytes handled separately)
        size_t b = 0;
        while (b < line.size() && static_cast<unsigned char>(line[b]) <= ' ') ++b;
        if (b >= line.size()) continue;                 // blank
        char c0 = line[b];
        if (c0 == ';' || c0 == '#') continue;           // comment

        if (c0 == '[') {
            size_t close = line.rfind(']');
            std::string name = (close != std::string::npos && close > b)
                               ? line.substr(b + 1, close - b - 1)
                               : line.substr(b + 1);
            // trim section name
            size_t sb = 0, se = name.size();
            while (sb < se && static_cast<unsigned char>(name[sb]) <= ' ') ++sb;
            while (se > sb && static_cast<unsigned char>(name[se - 1]) <= ' ') --se;
            Section ns;
            ns.name = name.substr(sb, se - sb);
            sections_.push_back(ns);
            cur = &sections_.back();
            continue;
        }

        // key=value (first '=' splits)
        size_t eq = line.find('=', b);
        if (eq == std::string::npos) continue;           // not a key line; skip
        if (!cur) continue;                              // key before any section

        std::string key = line.substr(b, eq - b);
        // trim key
        size_t kb = 0, ke = key.size();
        while (kb < ke && static_cast<unsigned char>(key[kb]) <= ' ') ++kb;
        while (ke > kb && static_cast<unsigned char>(key[ke - 1]) <= ' ') --ke;
        std::string k = key.substr(kb, ke - kb);
        if (k.empty()) continue;

        std::string v = line.substr(eq + 1);             // value: verbatim
        KeyVal kv; kv.key = k; kv.val = v;
        cur->items.push_back(kv);
    }
}

void TIniStore::SaveToFile(const AnsiString& path) const {
    std::FILE* fp = std::fopen(path.c_str(), "wb");
    if (!fp) return;
    for (size_t s = 0; s < sections_.size(); ++s) {
        std::fputc('[', fp);
        std::fwrite(sections_[s].name.data(), 1, sections_[s].name.size(), fp);
        std::fputs("]\r\n", fp);                          // CRLF (Win32 convention)
        for (size_t k = 0; k < sections_[s].items.size(); ++k) {
            const KeyVal& kv = sections_[s].items[k];
            std::fwrite(kv.key.data(), 1, kv.key.size(), fp);
            std::fputc('=', fp);
            std::fwrite(kv.val.data(), 1, kv.val.size(), fp);
            std::fputs("\r\n", fp);
        }
    }
    std::fclose(fp);
}

// ===========================================================================
//  TIniFile
// ===========================================================================
TIniFile::TIniFile(const AnsiString& fileName)
    : FileName(fileName), writeThrough_(true) {
    // Win32 TIniFile binds lazily; we eagerly load the current contents so reads
    // see existing keys.  (A missing file => empty store; first write creates it.)
    store_.LoadFromFile(fileName);
}

// Tag ctor: TMemIniFile path -- eager full load, but NO write-through.
TIniFile::TIniFile(const AnsiString& fileName, MemTag)
    : FileName(fileName), writeThrough_(false) {
    store_.LoadFromFile(fileName);
}

TIniFile::~TIniFile() {
    // Base TIniFile is write-through; nothing buffered to flush here. (The Mem
    // subclass overrides destruction order via its own dtor -> UpdateFile.)
}

void TIniFile::flush() {
    store_.SaveToFile(FileName);
}

void TIniFile::UpdateFile() {
    // TIniFile: Win32 writes are immediate -> commit / no-op.  For the Mem
    // variant (writeThrough_==false) this is the ONLY disk flush.
    if (!writeThrough_)
        flush();
}

bool TIniFile::SectionExists(const AnsiString& section) {
    return store_.SectionExists(section);
}

bool TIniFile::ValueExists(const AnsiString& section, const AnsiString& ident) {
    return store_.ValueExists(section, ident);
}

AnsiString TIniFile::ReadString(const AnsiString& section, const AnsiString& ident,
                                const AnsiString& def) {
    bool found = false;
    AnsiString v = store_.ReadRaw(section, ident, found);
    return found ? v : def;
}

int TIniFile::ReadInteger(const AnsiString& section, const AnsiString& ident, int def) {
    bool found = false;
    AnsiString v = store_.ReadRaw(section, ident, found);
    if (!found) return def;
    return parseIntDef(v.str(), def);
}

double TIniFile::ReadFloat(const AnsiString& section, const AnsiString& ident, double def) {
    bool found = false;
    AnsiString v = store_.ReadRaw(section, ident, found);
    if (!found) return def;
    return parseFloatDef(v.str(), def);
}

bool TIniFile::ReadBool(const AnsiString& section, const AnsiString& ident, bool def) {
    bool found = false;
    AnsiString v = store_.ReadRaw(section, ident, found);
    if (!found) return def;
    // BCB6 ReadBool == (ReadInteger(...) != 0).  Unparseable -> treat as the
    // integer default derived from `def` so the result equals `def`.
    int iv = parseIntDef(v.str(), def ? 1 : 0);
    return iv != 0;
}

TDateTime TIniFile::ReadDateTime(const AnsiString& section, const AnsiString& ident,
                                 const TDateTime& def) {
    bool found = false;
    AnsiString v = store_.ReadRaw(section, ident, found);
    if (!found) return def;
    if (v.Trim().IsEmpty()) return def;
    return StrToDateTime(v);
}

void TIniFile::WriteString(const AnsiString& section, const AnsiString& ident,
                           const AnsiString& value) {
    store_.WriteRaw(section, ident, value);
    if (writeThrough_) flush();
}

void TIniFile::WriteInteger(const AnsiString& section, const AnsiString& ident, int value) {
    WriteString(section, ident, AnsiString(value));     // AnsiString(int) -> decimal text
}

void TIniFile::WriteBool(const AnsiString& section, const AnsiString& ident, bool value) {
    WriteString(section, ident, AnsiString(value ? 1 : 0));  // '0'/'1'
}

void TIniFile::WriteDateTime(const AnsiString& section, const AnsiString& ident,
                             const TDateTime& value) {
    WriteString(section, ident, DateTimeToStr(value));
}

void TIniFile::ReadSection(const AnsiString& section, TStrings* dest) {
    store_.FillSectionKeys(section, dest);
}

void TIniFile::ReadSections(TStrings* dest) {
    store_.FillSectionNames(dest);
}

// ===========================================================================
//  TMemIniFile
// ===========================================================================
TMemIniFile::TMemIniFile(const AnsiString& fileName)
    : TIniFile(fileName, TIniFile::kMem) {   // eager full load, no write-through
}

TMemIniFile::~TMemIniFile() {
    // Safety-net flush: the BCB6 idiom always calls UpdateFile() before delete
    // (common.cpp CloseIniFileMem 362-363), but flush here too so an in-memory
    // mutation is never silently lost if a caller forgets.  Idempotent.
    flush();
}

} // namespace vclcompat
