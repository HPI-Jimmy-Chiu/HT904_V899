// ===========================================================================
//  vclcompat/IniFiles.h
//  BCB6 (Borland C++ Builder 6) TIniFile / TMemIniFile compatibility shim.
//
//  PURPOSE
//  -------
//  Recreate ONLY the TIniFile / TMemIniFile API surface that the HT9011UC
//  V906 BCB6 source actually drives -- specifically the common.cpp ini-helper
//  family (OpenIniFile / CheckAndReadIniData* / ReadIniData* / WriteIniData* /
//  CheckSectionExist / CheckKeyExist / *General / *Mem variants) -- with
//  semantics that mirror BCB6 EXACTLY, so the config-reader gateway translates
//  faithfully and low-risk.
//
//  WHY TWO CLASSES
//  ---------------
//  BCB6:
//    * TIniFile    -- a thin wrapper over the Win32 profile API
//                     (GetPrivateProfileString / WritePrivateProfileString).
//                     Win32 writes are IMMEDIATE; UpdateFile() is effectively a
//                     commit / no-op (but the method MUST exist -- callers call
//                     it before delete).  Win32 has a ~32KB-per-section read cap.
//    * TMemIniFile -- loads the ENTIRE file into memory (the whole point: avoid
//                     the 32KB/section cap).  Reads/writes hit the in-memory
//                     image; UpdateFile() is the ONLY thing that flushes the
//                     in-memory image back to disk.
//
//  In this portable layer BOTH classes are backed by the same in-memory store
//  (TIniStore).  The behavioural difference that the source depends on is the
//  FLUSH POLICY:
//    * TIniFile    -- every mutating call writes through to disk immediately
//                     (mirrors Win32 immediate-write).  UpdateFile() is a no-op
//                     commit.  (The 32KB cap is a Win32 artefact and is NOT
//                     reproduced -- it was never desirable behaviour and no
//                     correct caller relies on truncation.)
//    * TMemIniFile -- mutations touch memory only; disk is written by
//                     UpdateFile() (and by the destructor as a safety net,
//                     matching the BCB6 idiom that always pairs UpdateFile()
//                     with delete -- see common.cpp CloseIniFileMem 362-363).
//
//  BCB6 SEMANTICS REPLICATED (verified against the golden ref ini-helper family
//  D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\common.cpp:148-1487):
//    * DEFAULT FALLBACK: every Read*(Section,Ident,Default) returns Default
//      VERBATIM when the section OR key is absent (or the stored value is
//      unparseable for the numeric flavours).                       (common.cpp passim)
//    * CASE-INSENSITIVE section + key matching (Win32 profile + Delphi TStrings
//      name lookup are both case-insensitive).
//    * ReadInteger: BCB6 StrToIntDef-like parse -- decimal, or Pascal '$'/'0x'
//      hex prefix; leading sign tolerated; unparseable -> Default.   (common.cpp:428,445,548,604,987,1452)
//    * ReadFloat : locale-independent decimal parse ('.' separator);
//      unparseable -> Default.                                        (common.cpp:411,534,590,890,1440)
//    * ReadBool  : stored as '0'/'1'; nonzero integer => true; unparseable or
//      missing -> Default.                                            (common.cpp:462,520,576,636,1462)
//    * Doubles are persisted as '%0.4f' TEXT via WriteString (NOT WriteFloat)
//      and re-read via ReadFloat -- replicate the 4-decimal string round-trip.
//      There is NO WriteFloat call anywhere in common.cpp's ini family.
//                                                                     (common.cpp:407/409,888/958,1435/1438,1479)
//    * ReadDateTime / WriteDateTime : stored/parsed as a date+time STRING using
//      the app/system short-date+time format (here: DateTimeToStr /
//      StrToDateTime); missing -> Default.                            (common.cpp:504,506,1113)
//    * WriteString/WriteInteger/WriteBool/WriteDateTime create the section and
//      key if absent.
//    * ValueExists(Section,Ident) -> true iff the key exists under [Section].
//      SectionExists(Section)     -> true iff the [Section] header exists.
//    * FileName is a read-only property returning the bound path; OpenIniFile /
//      OpenIniFileMem compare INIFile->FileName!=FileName to decide reuse.
//                                                                     (common.cpp:328,350)
//
//  BIG5 / ENCODING: keys and values that this family reads/writes are ASCII;
//  section names and value strings MAY contain Big5 bytes.  The store keeps
//  raw bytes (std::string) untouched, so Big5 round-trips byte-for-byte.  Key
//  and section MATCHING is ASCII case-insensitive (toupper on bytes < 0x80);
//  Big5 lead/trail bytes (>=0x80) are compared verbatim, never case-folded,
//  which is safe because the matched identifiers in this family are ASCII.
//
//  Out of scope (not used by this batch): ReadBinaryStream / WriteBinaryStream,
//  EraseSection, DeleteKey, ReadSectionValues into a TStrings of "k=v" pairs
//  beyond the simple ReadSection / ReadSections enumerators provided here.
// ===========================================================================
#ifndef VCLCOMPAT_INIFILES_H
#define VCLCOMPAT_INIFILES_H

#include "vclcompat/AnsiString.h"
#include "vclcompat/TDateTime.h"
#include "vclcompat/TStringList.h"   // ReadSection(s) fill a TStrings*

#include <string>
#include <vector>

namespace vclcompat {

// ---------------------------------------------------------------------------
//  TIniStore -- shared in-memory representation of an INI file.
//
//  Preserves SECTION and KEY insertion order (BCB6 TStrings-backed ini keeps
//  order), keeps RAW value bytes (Big5-safe), and matches section/key names
//  ASCII-case-insensitively.  This is an implementation detail shared by
//  TIniFile and TMemIniFile; translated source never names it.
// ---------------------------------------------------------------------------
class TIniStore {
public:
    // ---- queries ----------------------------------------------------------
    bool       SectionExists(const AnsiString& section) const;
    bool       ValueExists(const AnsiString& section, const AnsiString& ident) const;
    // Returns the raw stored string for [section]ident, or `found=false` if
    // section/key absent (caller substitutes Default).
    AnsiString ReadRaw(const AnsiString& section, const AnsiString& ident,
                       bool& found) const;
    // ---- mutation ---------------------------------------------------------
    void       WriteRaw(const AnsiString& section, const AnsiString& ident,
                        const AnsiString& value);
    // ---- enumeration (BCB6 ReadSection / ReadSections) --------------------
    void       FillSectionKeys(const AnsiString& section, TStrings* dest) const;
    void       FillSectionNames(TStrings* dest) const;

    // ---- disk I/O ---------------------------------------------------------
    void       LoadFromFile(const AnsiString& path);   // replaces store; missing file => empty
    void       SaveToFile(const AnsiString& path) const;

private:
    struct KeyVal { std::string key; std::string val; };
    struct Section { std::string name; std::vector<KeyVal> items; };

    std::vector<Section> sections_;

    // ASCII case-insensitive byte compare (Big5 high bytes compared verbatim).
    static bool iequal(const std::string& a, const std::string& b);
    Section*       findSection(const AnsiString& name);
    const Section* findSection(const AnsiString& name) const;
};

// ===========================================================================
//  TIniFile  -- Win32-profile semantics: writes flush to disk immediately.
//               UpdateFile() is a commit/no-op (must exist; callers call it
//               before delete -- common.cpp:340,362,1418).
// ===========================================================================
class TIniFile {
public:
    explicit TIniFile(const AnsiString& fileName);
    virtual ~TIniFile();

    // ---- bound path (BCB6 read-only property TIniFile::FileName) ----------
    // Exposed as a public field so the source idiom `INIFile->FileName` (a
    // property read, common.cpp:328,350) compiles unchanged.  It is set once in
    // the ctor and never reassigned (read-only in spirit).
    AnsiString FileName;

    // ---- existence checks -------------------------------------------------
    bool SectionExists(const AnsiString& section);
    bool ValueExists(const AnsiString& section, const AnsiString& ident);

    // ---- typed reads (Default returned on miss/parse-failure) -------------
    AnsiString ReadString (const AnsiString& section, const AnsiString& ident, const AnsiString& def);
    int        ReadInteger(const AnsiString& section, const AnsiString& ident, int def);
    double     ReadFloat  (const AnsiString& section, const AnsiString& ident, double def);
    bool       ReadBool   (const AnsiString& section, const AnsiString& ident, bool def);
    TDateTime  ReadDateTime(const AnsiString& section, const AnsiString& ident, const TDateTime& def);

    // ---- typed writes (create section/key if absent) ---------------------
    void WriteString  (const AnsiString& section, const AnsiString& ident, const AnsiString& value);
    void WriteInteger (const AnsiString& section, const AnsiString& ident, int value);
    void WriteBool    (const AnsiString& section, const AnsiString& ident, bool value);
    void WriteDateTime(const AnsiString& section, const AnsiString& ident, const TDateTime& value);

    // ---- enumeration ------------------------------------------------------
    void ReadSection (const AnsiString& section, TStrings* dest); // key names under [section]
    void ReadSections(TStrings* dest);                            // all section names

    // ---- flush ------------------------------------------------------------
    // TIniFile: no-op commit (Win32 writes are immediate; the store already
    // wrote through to disk on each mutation).  MUST exist (common.cpp:340).
    void UpdateFile();

protected:
    TIniStore store_;
    // True for TIniFile (write-through), false for TMemIniFile (flush on
    // UpdateFile only).  Set by the ctors.
    bool writeThrough_;

    // Internal flush of the whole store to FileName.  Used by write-through
    // mutations (TIniFile) and by UpdateFile() (TMemIniFile).
    void flush();

private:
    // Tag ctor used by TMemIniFile to opt out of write-through and load eagerly.
    enum MemTag { kMem };
    TIniFile(const AnsiString& fileName, MemTag);
    friend class TMemIniFile;
};

// ===========================================================================
//  TMemIniFile  -- loads the ENTIRE file into memory at construction; reads /
//                  writes hit memory only; UpdateFile() is the ONLY disk flush
//                  (also flushed by the dtor as a safety net).
//                  common.cpp:353 (ctor), 362 (UpdateFile before delete).
// ===========================================================================
class TMemIniFile : public TIniFile {
public:
    explicit TMemIniFile(const AnsiString& fileName);
    virtual ~TMemIniFile();
    // UpdateFile() is inherited; for the Mem variant flush() actually writes the
    // in-memory image (writeThrough_==false, so mutations did NOT touch disk).
};

} // namespace vclcompat

#endif // VCLCOMPAT_INIFILES_H
