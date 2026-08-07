// =============================================================================
//  tests/test_ptw1_mystringlist.cpp -- PT-W1 golden-oracle test for
//  Public/MyStringList.cpp (class TMyStringList)
//
//  AI(W906-PT-W1-Tests) 20260807.
//
//  THE ORACLE IS GOLDEN, NEVER THE PORT.  Every expected value below was derived
//  BY HAND from
//    D:/HT9045/HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/Public/MyStringList.cpp
//    D:/HT9045/HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/Public/MyStringList.h
//  (cited as "golden .cpp:NNN" / "golden .h:NN" on each assertion), NOT read off
//  the port.  If an assertion fails, the port is wrong -- do not weaken it.
//
//  DO-NOT-TOUCH-REAL-CONFIG: every object built here has its Path pointed at a
//  scratch directory under the TEST BINARY's working directory (ctest runs each
//  test in CMAKE_CURRENT_BINARY_DIR).  Nothing under D:/HT9045/system,
//  D:/HT9045/config or D:/HT9045/CFG is read or written, and the golden default
//  Path "D:\HandlerLog" is only ever READ from the freshly constructed object,
//  never saved to (MySaveToFileShareMode returns at golden .cpp:237 before
//  GetFileName can create a directory when MyList is empty).
//
//  TIME-DEPENDENT NAMES: GetFileName() stamps the current date/time into the
//  file name.  Rather than freeze the clock (which golden gives no seam for),
//  the expected string is rebuilt for EVERY second in the window [t0,t1] that
//  brackets the call, and the assertion passes if the actual matches any of
//  them.  That is exact, not fuzzy: it just admits that the second may tick.
//
//  NOT CALLED HERE: MyInsert2DMappingToFile (golden .cpp:806-...) dereferences
//  fLotInfo->edtSysLotID, a VCL form pointer that is null in a test process.
//  MySaveToFileShareMode's bHanaTrayMap branch (golden .cpp:242-287) likewise
//  dereferences fSCKART.  Both are left to a UI-capable harness.
// =============================================================================
#include "Public/MyStringList.h"

#include "cmydef.h"          // CUSTOMER_CODE, OFF_LINE
#include "MachineType.h"     // CC_ASE_KaohSiung, CC_CYPRESS
#include "Config.h"          // IniConfig
#include "LastSet.h"         // LastSet
#include "common.h"          // MyForceDirectories

#include <windows.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>

static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { std::printf("  PASS: %s\n", msg); ++g_pass; }                \
        else      { std::printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

#define CHECK_INT(actual, expected, msg)                                        \
    do {                                                                        \
        const long _a = (long)(actual);                                         \
        const long _e = (long)(expected);                                       \
        if (_a == _e) { std::printf("  PASS: %s\n", msg); ++g_pass; }            \
        else { std::printf("  FAIL: %s  -- golden expects %ld, port returned %ld  (line %d)\n", \
                           msg, _e, _a, __LINE__); ++g_fail; }                  \
    } while (0)

#define CHECK_STR(actual, expected, msg)                                        \
    do {                                                                        \
        const AnsiString _a = (actual);                                         \
        const AnsiString _e = (expected);                                       \
        if (_a == _e) { std::printf("  PASS: %s\n", msg); ++g_pass; }            \
        else { std::printf("  FAIL: %s\n         golden: [%s]\n         port  : [%s]  (line %d)\n", \
                           msg, _e.c_str(), _a.c_str(), __LINE__); ++g_fail; }  \
    } while (0)

// ---------------------------------------------------------------------------
//  scratch-directory plumbing (build dir only -- never the real config tree)
// ---------------------------------------------------------------------------
static AnsiString g_scratchRoot;

static AnsiString ScratchDir(const char* leaf)
{
    AnsiString d = g_scratchRoot + "\\" + leaf;
    ::CreateDirectoryA(g_scratchRoot.c_str(), NULL);
    ::CreateDirectoryA(d.c_str(), NULL);
    return d;
}

// Escapes nothing: prints a byte string with CR and LF made visible, so a
// failure message shows exactly which line terminator the port emitted.
static std::string Visible(const std::string& s)
{
    std::string out;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if      (s[i] == '\r') out += "\\r";
        else if (s[i] == '\n') out += "\\n";
        else                   out += s[i];
    }
    return out;
}

static bool ReadWholeFile(const AnsiString& path, std::string& out)
{
    out.clear();
    std::FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) return false;
    char buf[4096];
    std::size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0) out.append(buf, n);
    std::fclose(f);
    return true;
}

static void CheckFileBytes(const AnsiString& path,
                           const std::string& expected,
                           const char* msg,
                           int line)
{
    std::string actual;
    const bool ok = ReadWholeFile(path, actual);
    if (ok && actual == expected) {
        std::printf("  PASS: %s\n", msg);
        ++g_pass;
    } else {
        std::printf("  FAIL: %s\n", msg);
        std::printf("         golden bytes: [%s]\n", Visible(expected).c_str());
        if (ok) std::printf("         port   bytes: [%s]\n", Visible(actual).c_str());
        else    std::printf("         port   bytes: <file could not be opened: %s>\n", path.c_str());
        std::printf("         (line %d)\n", line);
        ++g_fail;
    }
}
#define CHECK_FILE(path, expected, msg) CheckFileBytes((path), (expected), (msg), __LINE__)

// Same reporting shape as CHECK_FILE but for an in-memory string, so CR and LF
// stay visible instead of wrecking the report's own layout.
#define CHECK_BYTES(actual, expected, msg)                                      \
    do {                                                                        \
        const std::string _a = (actual);                                        \
        const std::string _e = (expected);                                       \
        if (_a == _e) { std::printf("  PASS: %s\n", msg); ++g_pass; }            \
        else { std::printf("  FAIL: %s\n         golden bytes: [%s]\n"          \
                           "         port   bytes: [%s]  (line %d)\n",          \
                           msg, Visible(_e).c_str(), Visible(_a).c_str(), __LINE__); \
               ++g_fail; }                                                      \
    } while (0)

// ---------------------------------------------------------------------------
//  Time-window helper.  Builds every candidate string the golden format could
//  have produced for a call that happened somewhere in [t0,t1].
// ---------------------------------------------------------------------------
typedef void (*FmtFn)(char* dst, std::size_t cap, const std::tm& tm_, const void* ctx);

static bool CheckTimeStamped(const AnsiString& actual,
                             std::time_t t0, std::time_t t1,
                             FmtFn fmt, const void* ctx,
                             const char* msg, int line)
{
    char cand[1024];
    std::string firstCandidate;
    for (std::time_t t = t0; t <= t1; ++t) {
        std::tm tmv = *std::localtime(&t);
        fmt(cand, sizeof(cand), tmv, ctx);
        if (firstCandidate.empty()) firstCandidate = cand;
        if (actual == AnsiString(cand)) {
            std::printf("  PASS: %s\n", msg);
            ++g_pass;
            return true;
        }
    }
    std::printf("  FAIL: %s\n", msg);
    std::printf("         golden: [%s]%s\n", firstCandidate.c_str(),
                (t1 > t0) ? "  (and the other seconds in the call window)" : "");
    std::printf("         port  : [%s]  (line %d)\n", actual.c_str(), line);
    ++g_fail;
    return false;
}
#define CHECK_TS(actual, t0, t1, fmt, ctx, msg) \
    CheckTimeStamped((actual), (t0), (t1), (fmt), (ctx), (msg), __LINE__)

struct NameCtx { const char* dir; const char* file; };

// golden .cpp:523  "%s\\%s.csv"                       (FixedFile)
static void fmt_fixed(char* d, std::size_t c, const std::tm&, const void* v)
{ const NameCtx* x = (const NameCtx*)v; std::snprintf(d, c, "%s\\%s.csv", x->dir, x->file); }

// golden .cpp:489 path "%s\\%04d"  +  .cpp:634 name "%s\\%s_%04d.csv"   (TByYear)
static void fmt_byyear(char* d, std::size_t c, const std::tm& t, const void* v)
{ const NameCtx* x = (const NameCtx*)v;
  std::snprintf(d, c, "%s\\%04d\\%s_%04d.csv", x->dir, t.tm_year+1900, x->file, t.tm_year+1900); }

// golden .cpp:489 path "%s\\%04d"  +  .cpp:615 name "%s\\%s_%04d%02d.csv" (TByMonth)
static void fmt_bymonth(char* d, std::size_t c, const std::tm& t, const void* v)
{ const NameCtx* x = (const NameCtx*)v;
  std::snprintf(d, c, "%s\\%04d\\%s_%04d%02d.csv",
                x->dir, t.tm_year+1900, x->file, t.tm_year+1900, t.tm_mon+1); }

// golden .cpp:493 path "%s\\%04d\\%02d" + .cpp:609 name "%s\\%s_%04d%02d%02d.csv" (TByDay)
static void fmt_byday(char* d, std::size_t c, const std::tm& t, const void* v)
{ const NameCtx* x = (const NameCtx*)v;
  std::snprintf(d, c, "%s\\%04d\\%02d\\%s_%04d%02d%02d.csv",
                x->dir, t.tm_year+1900, t.tm_mon+1,
                x->file, t.tm_year+1900, t.tm_mon+1, t.tm_mday); }

// golden .cpp:489 path "%s\\%04d" (TByMin=10 >= TByMonth=8!)  +
// golden .cpp:629 name "%s\\%s_%04d%02d%02d_%02d%02d.TXT"
static void fmt_bymin(char* d, std::size_t c, const std::tm& t, const void* v)
{ const NameCtx* x = (const NameCtx*)v;
  std::snprintf(d, c, "%s\\%04d\\%s_%04d%02d%02d_%02d%02d.TXT",
                x->dir, t.tm_year+1900,
                x->file, t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min); }

// golden .cpp:499 path "%s\\%04d\\%02d\\%02d" + .cpp:543 name "%s\\%s_%04d%02d%02d %02d.csv"
static void fmt_byhour(char* d, std::size_t c, const std::tm& t, const void* v)
{ const NameCtx* x = (const NameCtx*)v;
  std::snprintf(d, c, "%s\\%04d\\%02d\\%02d\\%s_%04d%02d%02d %02d.csv",
                x->dir, t.tm_year+1900, t.tm_mon+1, t.tm_mday,
                x->file, t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour); }

// golden .cpp:499 path + .cpp:553-554 name, iHour = SystemHour - SystemHour%4
static void fmt_by4hour(char* d, std::size_t c, const std::tm& t, const void* v)
{ const NameCtx* x = (const NameCtx*)v;
  std::snprintf(d, c, "%s\\%04d\\%02d\\%02d\\%s_%04d%02d%02d %02d.csv",
                x->dir, t.tm_year+1900, t.tm_mon+1, t.tm_mday,
                x->file, t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour - t.tm_hour%4); }

// golden .cpp:499 path + .cpp:527 name "%s\\%s_%04d%02d%02d %02d%02d%02d.csv"
static void fmt_bymaxline(char* d, std::size_t c, const std::tm& t, const void* v)
{ const NameCtx* x = (const NameCtx*)v;
  std::snprintf(d, c, "%s\\%04d\\%02d\\%02d\\%s_%04d%02d%02d %02d%02d%02d.csv",
                x->dir, t.tm_year+1900, t.tm_mon+1, t.tm_mday,
                x->file, t.tm_year+1900, t.tm_mon+1, t.tm_mday,
                t.tm_hour, t.tm_min, t.tm_sec); }

// ---------------------------------------------------------------------------
//  [1] constructors -- the documented defaults (golden .cpp:18-33, :35-56)
// ---------------------------------------------------------------------------
static void test_constructors()
{
    std::printf("\n[1] constructors -- golden .cpp:18-33 (default) and :35-56 (3-arg)\n");

    {
        TMyStringList sl;   // golden .cpp:18-33
        CHECK_INT((int)sl.MaxLineCount, 1000, "default ctor: MaxLineCount==1000  (golden .cpp:20)");
        CHECK_STR((AnsiString)sl.Path, "D:\\HandlerLog", "default ctor: Path==\"D:\\HandlerLog\"  (golden .cpp:21)");
        CHECK_STR((AnsiString)sl.FirstRow, "", "default ctor: FirstRow==\"\"  (golden .cpp:22)");
        CHECK_INT((int)(TSaveType)sl.SaveType, (int)TByDay, "default ctor: SaveType==TByDay  (golden .cpp:23)");
        CHECK((bool)sl.AutoSave == true, "default ctor: AutoSave==true  (golden .cpp:24)");
        CHECK(sl.bUseFTRT == false, "default ctor: bUseFTRT==false  (golden .cpp:25)");
        CHECK(sl.bFilePathWithDate == true, "default ctor: bFilePathWithDate==true  (golden .cpp:26)");
        CHECK(sl.bHanaTrayMap == false, "default ctor: bHanaTrayMap==false  (golden .cpp:27)");
        CHECK((bool)sl.FixedFile == false, "default ctor: FixedFile(HTSaveFixedFile)==false  (golden .cpp:28)");
        CHECK(sl.MyList != NULL, "default ctor: MyList is allocated  (golden .cpp:29)");
        CHECK_INT(sl.MyList->Count, 0, "default ctor: MyList starts empty  (golden .cpp:29)");
        CHECK_STR(sl.sPrevFileName, "", "default ctor: sPrevFileName==\"\"  (golden .cpp:30)");
        CHECK(sl.bChangeFile == false, "default ctor: bChangeFile==false  (golden .cpp:31)");
        // Deliberately NOT adding text to this one: its Path is the real
        // D:\HandlerLog and the destructor calls MySaveToFile (golden .cpp:62).
        // With MyList empty that returns at golden .cpp:237-238 without touching
        // the disk, which is exactly why this object is left empty.
    }

    {
        const AnsiString dir = ScratchDir("ctor3");
        TMyStringList sl(dir, "CTOR3", "H1,H2");   // golden .cpp:35-56
        CHECK_INT((int)sl.MaxLineCount, 1, "3-arg ctor: MaxLineCount==1  (golden .cpp:37)");
        CHECK_STR((AnsiString)sl.Path, dir, "3-arg ctor: Path==sPath  (golden .cpp:38)");
        CHECK_STR((AnsiString)sl.FileName, "CTOR3", "3-arg ctor: FileName==sFileName  (golden .cpp:39)");
        CHECK_STR((AnsiString)sl.FirstRow, "H1,H2", "3-arg ctor: FirstRow==sFirstRow  (golden .cpp:40)");
        // golden .cpp:42-45: TByHour only for CC_ASE_KaohSiung, otherwise TByDay.
        const TSaveType expect = (CUSTOMER_CODE == CC_ASE_KaohSiung) ? TByHour : TByDay;
        std::printf("    (CUSTOMER_CODE==%d, CC_ASE_KaohSiung==%d)\n", CUSTOMER_CODE, CC_ASE_KaohSiung);
        CHECK_INT((int)(TSaveType)sl.SaveType, (int)expect,
                  "3-arg ctor: SaveType is TByHour iff CUSTOMER_CODE==CC_ASE_KaohSiung, else TByDay  (golden .cpp:42-45)");
        CHECK((bool)sl.AutoSave == true, "3-arg ctor: AutoSave==true  (golden .cpp:47)");
        CHECK(sl.bUseFTRT == false, "3-arg ctor: bUseFTRT==false  (golden .cpp:48)");
        CHECK(sl.bFilePathWithDate == true, "3-arg ctor: bFilePathWithDate==true  (golden .cpp:49)");
        CHECK(sl.bHanaTrayMap == false, "3-arg ctor: bHanaTrayMap==false  (golden .cpp:50)");
        CHECK((bool)sl.FixedFile == false, "3-arg ctor: FixedFile==false  (golden .cpp:51)");
        CHECK_INT(sl.MyList->Count, 0, "3-arg ctor: MyList starts empty  (golden .cpp:52)");
        CHECK(sl.bChangeFile == false, "3-arg ctor: bChangeFile==false  (golden .cpp:54)");
    }
}

// ---------------------------------------------------------------------------
//  [2] property binding -- read yields the field, write routes via the setter
//      (golden .h:78-86 __property read=/write= pairs, setters golden .cpp:73-116)
// ---------------------------------------------------------------------------
static void test_properties()
{
    std::printf("\n[2] __property translation -- read=<field> / write=<setter> round trips\n");

    const AnsiString dir = ScratchDir("props");
    TMyStringList sl(dir, "PROPS", "");

    // golden .h:78 Path {read=HTPath, write=SetPath}; setter golden .cpp:73-76
    sl.Path = "X:\\alpha";
    CHECK_STR((AnsiString)sl.Path, "X:\\alpha", "Path: write then read round trips  (golden .h:78 / .cpp:75)");
    // golden .h:79 FileName; setter golden .cpp:78-81
    sl.FileName = "beta";
    CHECK_STR((AnsiString)sl.FileName, "beta", "FileName: write then read round trips  (golden .h:79 / .cpp:80)");
    // golden .h:80 FirstRow; setter golden .cpp:83-86
    sl.FirstRow = "a,b,c";
    CHECK_STR((AnsiString)sl.FirstRow, "a,b,c", "FirstRow: write then read round trips  (golden .h:80 / .cpp:85)");
    // golden .h:81 MaxLineCount; setter golden .cpp:88-91.  Golden's setter has
    // NO validation at all, so a negative value must be stored verbatim.
    sl.MaxLineCount = 42;
    CHECK_INT((int)sl.MaxLineCount, 42, "MaxLineCount: write then read round trips  (golden .h:81 / .cpp:90)");
    sl.MaxLineCount = -7;
    CHECK_INT((int)sl.MaxLineCount, -7, "MaxLineCount: golden's setter does NOT clamp; -7 is stored  (golden .cpp:88-91)");
    // golden .h:82 SaveType; setter golden .cpp:93-96
    sl.SaveType = TByMonth;
    CHECK_INT((int)(TSaveType)sl.SaveType, (int)TByMonth, "SaveType: write then read round trips  (golden .h:82 / .cpp:95)");
    // golden .h:83 AutoSave; setter golden .cpp:98-101
    sl.AutoSave = false;
    CHECK((bool)sl.AutoSave == false, "AutoSave: write then read round trips  (golden .h:83 / .cpp:100)");
    sl.AutoSave = true;
    CHECK((bool)sl.AutoSave == true, "AutoSave: write true then read  (golden .h:83 / .cpp:100)");
    // golden .h:84 FixedFile {read=HTSaveFixedFile, write=SetSaveFixedFile}; .cpp:113-116
    sl.FixedFile = true;
    CHECK((bool)sl.FixedFile == true, "FixedFile: write then read round trips  (golden .h:84 / .cpp:115)");
    sl.FixedFile = false;
    // golden .h:85 SaveSameFolder; setter golden .cpp:103-106
    sl.SaveSameFolder = true;
    CHECK((bool)sl.SaveSameFolder == true, "SaveSameFolder: write then read round trips  (golden .h:85 / .cpp:105)");
    sl.SaveSameFolder = false;
    // golden .h:86 SaveByLotID; setter golden .cpp:108-111
    sl.SaveByLotID = true;
    CHECK((bool)sl.SaveByLotID == true, "SaveByLotID: write then read round trips  (golden .h:86 / .cpp:110)");
    sl.SaveByLotID = false;

    // Property-to-property assignment, which BCB6 supports directly: the VALUE
    // must move, and the destination must keep binding to its OWN object.
    TMyStringList other(dir, "OTHER", "");
    other.MaxLineCount = 55;
    sl.MaxLineCount = other.MaxLineCount;
    CHECK_INT((int)sl.MaxLineCount,    55, "property=property copies the VALUE  (golden .h:81 semantics)");
    CHECK_INT((int)other.MaxLineCount, 55, "property=property leaves the SOURCE intact  (golden .h:81 semantics)");
    sl.MaxLineCount = 3;
    CHECK_INT((int)other.MaxLineCount, 55, "property=property did NOT alias the two objects together");

    sl.MaxLineCount = 1000;   // stop the dtor's MySaveToFile from writing
    sl.AutoSave = false;
    other.AutoSave = false;
}

// ---------------------------------------------------------------------------
//  [3] GetFileName -- the file-name policy engine (golden .cpp:467-639)
// ---------------------------------------------------------------------------
static void test_GetFileName()
{
    std::printf("\n[3] GetFileName -- 8 of the TSaveType / FixedFile branches\n");

    const AnsiString dir = ScratchDir("names");
    // golden .cpp:497 and :568 read this; 0 selects the 00:00 (not 08:00) layout.
    IniConfig.iN10UploadProductMethod = 0;
    // golden .cpp:592-593: the Cypress-offline name only fires for CC_CYPRESS AND
    // an offline tester.  Force the ordinary branch so the expectation is single
    // valued, and say so.
    const int savedTester = LastSet.iTester;
    LastSet.iTester = OFF_LINE + 1;
    std::printf("    (LastSet.iTester forced != OFF_LINE so golden .cpp:592-593 cannot fire)\n");

    TMyStringList sl(dir, "LOG", "");
    sl.AutoSave = false;         // nothing here should ever write a data file
    sl.bUseFTRT = false;         // golden .cpp:533/:599/:619 FT/RT suffixes off

    NameCtx ctx;
    ctx.dir  = dir.c_str();
    ctx.file = "LOG";
    std::time_t t0, t1;

    // --- FixedFile wins over EVERYTHING (golden .cpp:481-484 path, :521-524 name)
    sl.FixedFile = true;
    sl.SaveType  = TByMin;                  // deliberately a "loud" SaveType
    sl.bFilePathWithDate = true;            // deliberately on
    CHECK_STR(sl.GetFileName(), dir + "\\LOG.csv",
              "FixedFile: <Path>\\<FileName>.csv, no date folder, no date stamp  (golden .cpp:483/:523)");
    sl.FixedFile = false;

    // --- TByYear (=9): >= TByMonth so the folder is by YEAR (golden .cpp:487-489)
    //     name golden .cpp:634 "%s\\%s_%04d.csv"
    sl.SaveType = TByYear;
    t0 = std::time(NULL); { AnsiString got = sl.GetFileName(); t1 = std::time(NULL);
    CHECK_TS(got, t0, t1, fmt_byyear, &ctx,
             "TByYear: <Path>\\<YYYY>\\<FileName>_<YYYY>.csv  (golden .cpp:489/:634)"); }

    // --- TByMonth (=8): >= TByMonth so the folder is still by YEAR
    //     name golden .cpp:615 "%s\\%s_%04d%02d.csv"
    sl.SaveType = TByMonth;
    t0 = std::time(NULL); { AnsiString got = sl.GetFileName(); t1 = std::time(NULL);
    CHECK_TS(got, t0, t1, fmt_bymonth, &ctx,
             "TByMonth: <Path>\\<YYYY>\\<FileName>_<YYYYMM>.csv  (golden .cpp:489/:615)"); }

    // --- TByDay (=7): < TByMonth but >= TByDay -> folder is <YYYY>\<MM>
    //     (golden .cpp:491-494), name golden .cpp:609
    sl.SaveType = TByDay;
    t0 = std::time(NULL); { AnsiString got = sl.GetFileName(); t1 = std::time(NULL);
    CHECK_TS(got, t0, t1, fmt_byday, &ctx,
             "TByDay: <Path>\\<YYYY>\\<MM>\\<FileName>_<YYYYMMDD>.csv  (golden .cpp:493/:609)"); }

    // --- TByMin (=10): the ORDINAL quirk.  TByMin sorts ABOVE TByMonth (=8), so
    //     golden .cpp:487 sends a by-MINUTE log into the by-YEAR folder, while
    //     the NAME (golden .cpp:629) carries the full minute stamp and a .TXT
    //     extension.  This mismatch is golden's, and it is load bearing.
    sl.SaveType = TByMin;
    t0 = std::time(NULL); { AnsiString got = sl.GetFileName(); t1 = std::time(NULL);
    CHECK_TS(got, t0, t1, fmt_bymin, &ctx,
             "TByMin: by-YEAR folder (TByMin=10 >= TByMonth=8) but a _HHMM.TXT name  (golden .cpp:487-489/:629)"); }

    // --- TByHour (=1): < TByDay -> the 3-deep <YYYY>\<MM>\<DD> folder
    //     (golden .cpp:495-499), name golden .cpp:543 (note the SPACE before HH)
    sl.SaveType = TByHour;
    t0 = std::time(NULL); { AnsiString got = sl.GetFileName(); t1 = std::time(NULL);
    CHECK_TS(got, t0, t1, fmt_byhour, &ctx,
             "TByHour: <Path>\\<YYYY>\\<MM>\\<DD>\\<FileName>_<YYYYMMDD> <HH>.csv  (golden .cpp:499/:543)"); }

    // --- TBy4Hour (=3): same folder, hour floored to a multiple of 4
    //     (golden .cpp:551-555, iHour = SystemHour - SystemHour%4)
    sl.SaveType = TBy4Hour;
    t0 = std::time(NULL); { AnsiString got = sl.GetFileName(); t1 = std::time(NULL);
    CHECK_TS(got, t0, t1, fmt_by4hour, &ctx,
             "TBy4Hour: hour floored to a multiple of 4  (golden .cpp:553-554)"); }

    // --- TByMaxLineCount (=0): folder as above, name carries HHMMSS
    //     (golden .cpp:525-528)
    sl.SaveType = TByMaxLineCount;
    t0 = std::time(NULL); { AnsiString got = sl.GetFileName(); t1 = std::time(NULL);
    CHECK_TS(got, t0, t1, fmt_bymaxline, &ctx,
             "TByMaxLineCount: <FileName>_<YYYYMMDD> <HHMMSS>.csv  (golden .cpp:527)"); }

    // --- bFilePathWithDate==false collapses the folder to Path itself
    //     (golden .cpp:515-518); the NAME still carries its date stamp.
    sl.bFilePathWithDate = false;
    sl.SaveType = TByDay;
    {
        NameCtx flat; flat.dir = dir.c_str(); flat.file = "LOG";
        char want[1024];
        std::time_t ta = std::time(NULL);
        AnsiString got = sl.GetFileName();
        std::time_t tb = std::time(NULL);
        bool ok = false;
        for (std::time_t t = ta; t <= tb && !ok; ++t) {
            std::tm tv = *std::localtime(&t);
            std::snprintf(want, sizeof(want), "%s\\%s_%04d%02d%02d.csv",
                          flat.dir, flat.file, tv.tm_year+1900, tv.tm_mon+1, tv.tm_mday);
            ok = (got == AnsiString(want));
        }
        if (ok) { std::printf("  PASS: bFilePathWithDate==false: no date FOLDER, date stamp still in the NAME  (golden .cpp:517/:609)\n"); ++g_pass; }
        else    { std::printf("  FAIL: bFilePathWithDate==false: no date FOLDER, date stamp still in the NAME  (golden .cpp:517/:609)\n"
                              "         golden: [%s]\n         port  : [%s]  (line %d)\n", want, got.c_str(), __LINE__); ++g_fail; }
    }

    // --- an EMPTY Path is repaired to the golden default (golden .cpp:475-476)
    {
        TMyStringList e;
        e.AutoSave = false;
        e.Path = "";
        e.FileName = "E";
        e.FixedFile = true;                 // keeps GetFileName off the date tree
        const AnsiString got = e.GetFileName();
        CHECK_STR(got, AnsiString("D:\\HandlerLog") + "\\E.csv",
                  "empty Path is rewritten to \"D:\\HandlerLog\" before use  (golden .cpp:475-476)");
    }

    LastSet.iTester = savedTester;
}

// ---------------------------------------------------------------------------
//  [4] save/load round trip + FirstRow (golden .cpp:139-153, :225-228, :230-398)
// ---------------------------------------------------------------------------
static void test_roundtrip_and_FirstRow()
{
    std::printf("\n[4] AddText -> MySaveToFile -> file, and the FirstRow header rule\n");

    const AnsiString dir = ScratchDir("roundtrip");
    const AnsiString target = dir + "\\RT.csv";
    ::DeleteFileA(target.c_str());

    TMyStringList sl(dir, "RT", "HDR");
    sl.FixedFile     = true;      // pin the name: <dir>\RT.csv (golden .cpp:483/:523)
    sl.MaxLineCount  = 1000;      // so AddText does not auto-flush (golden .cpp:148)
    sl.AutoSave      = true;

    sl.AddText("L1");
    CHECK_INT(sl.MyList->Count, 1, "AddText buffers into MyList  (golden .cpp:146)");
    sl.AddText("L2");
    CHECK_INT(sl.MyList->Count, 2, "AddText below MaxLineCount does NOT flush  (golden .cpp:148)");

    // ROOT-CAUSE PROBE, asserted separately so a failure below is diagnosable.
    // Every writer in this class serialises through MyList->Text.  In BCB6 that
    // is TStrings::GetTextStr, which appends LineBreak (sLineBreak == CRLF)
    // after EVERY string INCLUDING the last -- see golden .cpp:263/:267/:301/
    // :305/:338/:340/:372/:374/:421/:449/:788, all of which concatenate
    // MyList->Text straight into the bytes that reach the disk.
    CHECK_BYTES(std::string(AnsiString(sl.MyList->Text).c_str()), std::string("L1\r\nL2\r\n"),
                "MyList->Text terminates EVERY line with CRLF, including the last  (BCB6 TStrings::GetTextStr)");

    sl.MySaveToFile();

    // golden .cpp:299-306: the file did not exist and HTFirstRow!="" , so
    //   Str = HTFirstRow + "\r\n" + MyList->Text
    // and the write is a raw ::WriteFile (golden .cpp:322), i.e. NO text-mode
    // translation -- the bytes of Str are the bytes on disk:
    //   "HDR" CRLF "L1" CRLF "L2" CRLF
    CHECK_FILE(target, std::string("HDR\r\nL1\r\nL2\r\n"),
               "first save of a NEW file writes FirstRow + CRLF + MyList->Text  (golden .cpp:301 / VCL TStrings::Text)");
    CHECK_STR(sl.sLastFileName, target, "MySaveToFile records sLastFileName  (golden .cpp:297)");
    CHECK_INT(sl.MyList->Count, 0, "MySaveToFile clears MyList on the way out  (golden .cpp:397)");

    // Second save: the file NOW EXISTS, so golden .cpp:299 takes the else branch
    // and NO header is emitted -- the payload is appended (golden .cpp:320 seeks
    // to FILE_END before writing).
    sl.AddText("L3");
    sl.MySaveToFile();
    CHECK_FILE(target, std::string("HDR\r\nL1\r\nL2\r\nL3\r\n"),
               "second save APPENDS and does NOT repeat FirstRow  (golden .cpp:299-305/:320)");

    // FirstRow=="" suppresses the header even for a brand new file
    // (golden .cpp:299 requires HTFirstRow!="").
    const AnsiString target2 = dir + "\\NOHDR.csv";
    ::DeleteFileA(target2.c_str());
    sl.FileName = "NOHDR";
    sl.FirstRow = "";
    sl.AddText("only");
    sl.MySaveToFile();
    CHECK_FILE(target2, std::string("only\r\n"),
               "FirstRow==\"\" writes no header even on a new file  (golden .cpp:299)");

    // Changing the target file arms bChangeFile / sPrevFileName
    // (golden .cpp:291-296).  The FIRST save could not arm it because
    // sLastFileName was still "" at that point.
    CHECK(sl.bChangeFile == true, "bChangeFile set once the computed name changes  (golden .cpp:295)");
    CHECK_STR(sl.sPrevFileName, target, "sPrevFileName holds the PREVIOUS file  (golden .cpp:294)");

    // AutoSave==false is a hard stop before anything is computed or written
    // (golden .cpp:237-238).
    const AnsiString target3 = dir + "\\NOSAVE.csv";
    ::DeleteFileA(target3.c_str());
    sl.FileName = "NOSAVE";
    sl.AutoSave = false;
    sl.AddText("dropped");
    sl.MySaveToFile();
    CHECK(FileExists(target3) == false,
          "AutoSave==false writes NOTHING  (golden .cpp:237-238)");
    CHECK_INT(sl.MyList->Count, 1,
              "AutoSave==false also leaves MyList untouched (the early return skips .cpp:397)");
    sl.MyList->Clear();
}

// ---------------------------------------------------------------------------
//  [5] MaxLineCount trimming (golden .cpp:148-152) and the Add* asymmetry
// ---------------------------------------------------------------------------
static void test_MaxLineCount()
{
    std::printf("\n[5] MaxLineCount -- AddText flushes at >=, AddTextWithDateTime* at >\n");

    const AnsiString dir = ScratchDir("maxline");
    const AnsiString target = dir + "\\ML.csv";
    ::DeleteFileA(target.c_str());

    TMyStringList sl(dir, "ML", "");
    sl.FixedFile    = true;
    sl.AutoSave     = true;
    sl.MaxLineCount = 3;

    // golden .cpp:146-152: ADD FIRST, then flush when Count >= HTMaxLineCount.
    sl.AddText("a");
    CHECK_INT(sl.MyList->Count, 1, "AddText #1 of 3: buffered  (golden .cpp:146-148)");
    sl.AddText("b");
    CHECK_INT(sl.MyList->Count, 2, "AddText #2 of 3: buffered  (golden .cpp:146-148)");
    sl.AddText("c");
    CHECK_INT(sl.MyList->Count, 0, "AddText #3 of 3: Count>=MaxLineCount so it FLUSHES and clears  (golden .cpp:148-152)");
    CHECK_FILE(target, std::string("a\r\nb\r\nc\r\n"),
               "the flush wrote exactly the 3 buffered lines  (golden .cpp:150/:305)");

    // MaxLineCount==1 flushes on EVERY AddText (1 >= 1).
    const AnsiString target1 = dir + "\\ML1.csv";
    ::DeleteFileA(target1.c_str());
    sl.FileName     = "ML1";
    sl.MaxLineCount = 1;
    sl.AddText("x");
    CHECK_INT(sl.MyList->Count, 0, "MaxLineCount==1: every AddText flushes immediately  (golden .cpp:148)");
    CHECK_FILE(target1, std::string("x\r\n"), "MaxLineCount==1 wrote the single line  (golden .cpp:150)");

    // golden .cpp:174-179 is the OTHER shape: test BEFORE adding, with `>` not
    // `>=`.  With MaxLineCount==2 the buffer therefore reaches 3 before it
    // flushes, and the flush happens on the call that then adds line 1 of the
    // next batch.  Golden really is asymmetric between AddText and these three.
    const AnsiString target2 = dir + "\\MLDT.csv";
    ::DeleteFileA(target2.c_str());
    sl.FileName     = "MLDT";
    sl.MaxLineCount = 2;
    sl.MyList->Clear();
    sl.AddTextWithDateTime("d1");
    CHECK_INT(sl.MyList->Count, 1, "AddTextWithDateTime #1: 0>2 false, so add  (golden .cpp:174-179)");
    sl.AddTextWithDateTime("d2");
    CHECK_INT(sl.MyList->Count, 2, "AddTextWithDateTime #2: 1>2 false, so add  (golden .cpp:174-179)");
    sl.AddTextWithDateTime("d3");
    CHECK_INT(sl.MyList->Count, 3, "AddTextWithDateTime #3: 2>2 FALSE, so the buffer overshoots to 3  (golden .cpp:174)");
    sl.AddTextWithDateTime("d4");
    CHECK_INT(sl.MyList->Count, 1, "AddTextWithDateTime #4: 3>2 true -> flush+clear, THEN add  (golden .cpp:174-179)");

    sl.AutoSave = false;
    sl.MyList->Clear();
}

// ---------------------------------------------------------------------------
//  [6] the timestamp formats (golden .cpp:182-198 / :200-215)
// ---------------------------------------------------------------------------
static void test_timestamp_formats()
{
    std::printf("\n[6] AddTextWithDateTime2 -- the returned stamp, char by 1-BASED char\n");

    const AnsiString dir = ScratchDir("stamps");
    TMyStringList sl(dir, "TS", "");
    sl.AutoSave     = false;      // never writes; only the returned string matters
    sl.MaxLineCount = 1000;

    // golden .cpp:186: "%04d/%02d/%02d %02d:%02d:%02d,%s"
    //   4+1+2+1+2 +1+ 2+1+2+1+2 +1+ 1  =  21 characters for a 1-char message.
    const std::time_t before = std::time(NULL);
    const AnsiString s = sl.AddTextWithDateTime2("X");
    const std::time_t after = std::time(NULL);

    std::printf("    returned: [%s]\n", s.c_str());
    CHECK_INT(s.Length(), 21, "AddTextWithDateTime2(\"X\").Length()==21  (golden .cpp:186)");
    // 1-BASED positions.  Position 1 is the first digit of the year and position
    // Length() is the message -- both ends asserted, per the 1-based rule.
    CHECK_STR(s.SubString(5, 1),  "/", "separator at 1-based position 5 is '/'   (golden .cpp:186)");
    CHECK_STR(s.SubString(8, 1),  "/", "separator at 1-based position 8 is '/'   (golden .cpp:186)");
    CHECK_STR(s.SubString(11, 1), " ", "separator at 1-based position 11 is ' '  (golden .cpp:186)");
    CHECK_STR(s.SubString(14, 1), ":", "separator at 1-based position 14 is ':'  (golden .cpp:186)");
    CHECK_STR(s.SubString(17, 1), ":", "separator at 1-based position 17 is ':'  (golden .cpp:186)");
    CHECK_STR(s.SubString(20, 1), ",", "separator at 1-based position 20 is ','  (golden .cpp:186)");
    CHECK_STR(s.SubString(21, 1), "X", "the MESSAGE is at 1-based position 21 (LAST char)  (golden .cpp:186)");
    // The year/month/day really are today's, i.e. GetTimeInfo ran (golden .cpp:185).
    {
        char want[32];
        bool ok = false;
        for (std::time_t t = before; t <= after && !ok; ++t) {
            std::tm tv = *std::localtime(&t);
            std::snprintf(want, sizeof(want), "%04d/%02d/%02d", tv.tm_year+1900, tv.tm_mon+1, tv.tm_mday);
            ok = (s.SubString(1, 10) == AnsiString(want));
        }
        if (ok) { std::printf("  PASS: 1-based positions 1..10 are today's %%04d/%%02d/%%02d  (golden .cpp:185-186)\n"); ++g_pass; }
        else    { std::printf("  FAIL: 1-based positions 1..10 are today's date\n"
                              "         golden: [%s]\n         port  : [%s]  (line %d)\n",
                              want, s.SubString(1,10).c_str(), __LINE__); ++g_fail; }
    }
    CHECK_INT(sl.MyList->Count, 1, "AddTextWithDateTime2 also buffers the line it returns  (golden .cpp:196)");
    sl.MyList->Clear();
}

// ---------------------------------------------------------------------------
//  [7] GetLastLine (golden .cpp:641-669)
// ---------------------------------------------------------------------------
static void test_GetLastLine()
{
    std::printf("\n[7] GetLastLine -- std::getline count over the CURRENT file name\n");

    const AnsiString dir = ScratchDir("lastline");
    const AnsiString target = dir + "\\GLL.csv";
    ::DeleteFileA(target.c_str());

    TMyStringList sl(dir, "GLL", "HDR");
    sl.FixedFile    = true;
    sl.MaxLineCount = 1000;
    sl.AutoSave     = true;

    // golden .cpp:646: no file -> the whole read is skipped and iCount stays 0.
    CHECK_INT(sl.GetLastLine(), 0, "GetLastLine()==0 when the file does not exist  (golden .cpp:644-646)");
    CHECK_STR(sl.sLastFileName, target, "GetLastLine records sLastFileName even when absent  (golden .cpp:667)");

    // Write "HDR\r\nL1\r\nL2\r\n" -- 3 newline-terminated records, so
    // std::getline (golden .cpp:656) succeeds 3 times and fails on the 4th.
    sl.AddText("L1");
    sl.AddText("L2");
    sl.MySaveToFile();
    CHECK_INT(sl.GetLastLine(), 3,
              "GetLastLine()==3 for a file of 3 newline-terminated records  (golden .cpp:652-659)");

    sl.AutoSave = false;
}

// ---------------------------------------------------------------------------
//  [8] SetLotData (golden .cpp:118-137)
// ---------------------------------------------------------------------------
static void test_SetLotData()
{
    std::printf("\n[8] SetLotData -- the by-Lot mirror name, including its zero-date quirk\n");

    const AnsiString dir = ScratchDir("lotdata");

    // golden .cpp:123-126: an EMPTY ID blanks sLotFileName outright.
    {
        TMyStringList sl(dir, "LOT", "");
        sl.AutoSave = false;
        sl.SetLotData("", "T0");
        CHECK_STR(sl.sLotFileName, "", "SetLotData(\"\",...) clears sLotFileName  (golden .cpp:123-126)");
    }

    // golden .cpp:133-136: an explicit LotFileName is taken verbatim, no build.
    {
        TMyStringList sl(dir, "LOT", "");
        sl.AutoSave = false;
        sl.SetLotData("LOT1", "T1", "Z:\\given\\name.csv");
        CHECK_STR(sl.sLotFileName, "Z:\\given\\name.csv",
                  "SetLotData with an explicit LotFileName stores it verbatim  (golden .cpp:133-136)");
    }

    // golden .cpp:127-132 builds the name from HTPath and the CLASS's
    // SystemYear/Month/Date members -- and SetLotData does NOT call GetTimeInfo()
    // first.  On a freshly constructed object those members are still the
    // zero-filled instance memory BCB6 hands every TObject descendant, so the
    // golden path really is "<Path>\ByLotID\0000\00\00".  This is golden's
    // behaviour, not a port artefact; it is asserted so a "helpful" future edit
    // that adds a GetTimeInfo() call gets caught.
    {
        IniConfig.sMachineType     = "MT";
        IniConfig.SocketHandlerID  = "SH";
        TMyStringList sl(dir, "LOT", "");
        sl.AutoSave = false;
        sl.SetLotData("LOT2", "T2");
        const AnsiString want = dir + "\\ByLotID\\0000\\00\\00"
                                    + "\\MT_SH_LOT2_T2_ByLotEventLog.csv";
        CHECK_STR(sl.sLotFileName, want,
                  "SetLotData builds <Path>\\ByLotID\\<Y>\\<M>\\<D>\\<MT>_<SH>_<ID>_<Time>_ByLotEventLog.csv "
                  "with the UNINITIALISED zero date  (golden .cpp:129-131, no GetTimeInfo call)");
    }
}

// ---------------------------------------------------------------------------
//  [9] MySaveFileByFileName (golden .cpp:439-465) -- date-free, so exact
// ---------------------------------------------------------------------------
static void test_MySaveFileByFileName()
{
    std::printf("\n[9] MySaveFileByFileName -- explicit path+name, CRLF folded to LF\n");

    const AnsiString dir    = ScratchDir("byname");
    const AnsiString target = dir + "\\EXPLICIT.csv";
    ::DeleteFileA(target.c_str());

    TMyStringList sl(dir, "UNUSED", "H");
    sl.AutoSave     = true;
    sl.MaxLineCount = 1000;
    sl.MyList->Add("a");
    sl.MyList->Add("b");

    sl.MySaveFileByFileName(dir, "EXPLICIT.csv");

    // golden .cpp:447-456: new file + FirstRow!="" -> "H" + CRLF + Text, and then
    // EVERY "\r\n" is replaced by "\n" (golden .cpp:456) before the append.
    // Golden's VCL Text is "a\r\nb\r\n", so
    //   pre-replace   : "H\r\na\r\nb\r\n"
    //   post-replace  : "H\na\nb\n"
    // and the write is fputs into a stream opened with fopen(...,"a") -- TEXT
    // mode on Windows (golden .cpp:457/:460), which expands every '\n' back to
    // CRLF.  So the bytes that land on disk are "H\r\na\r\nb\r\n".  (This
    // fold-then-unfold round trip is why golden's StringReplace looks like a
    // no-op here: its real effect is to NORMALISE mixed input, not to change
    // the terminator that is finally written.)
    CHECK_FILE(target, std::string("H\r\na\r\nb\r\n"),
               "MySaveFileByFileName: CRLF folded to LF, then text-mode fputs expands it back  (golden .cpp:449/:456/:457/:460)");
    CHECK_INT(sl.MyList->Count, 0, "MySaveFileByFileName clears MyList  (golden .cpp:464)");

    // Second call: the file exists, so no header (golden .cpp:447).
    sl.MyList->Add("c");
    sl.MySaveFileByFileName(dir, "EXPLICIT.csv");
    CHECK_FILE(target, std::string("H\r\na\r\nb\r\nc\r\n"),
               "second MySaveFileByFileName appends without repeating FirstRow  (golden .cpp:447-453)");

    sl.AutoSave = false;
}

// ---------------------------------------------------------------------------
//  main
// ---------------------------------------------------------------------------
int main()
{
    std::setvbuf(stdout, 0, _IONBF, 0);

    char cwd[MAX_PATH];
    ::GetCurrentDirectoryA(sizeof(cwd), cwd);
    g_scratchRoot = AnsiString(cwd) + "\\_ptw1_msl_scratch";

    std::printf("=================================================================\n");
    std::printf("  PT-W1 -- Public/MyStringList.cpp golden-oracle test\n");
    std::printf("  oracle: golden Public/MyStringList.{h,cpp} (Big5/cp950)\n");
    std::printf("  scratch: %s\n", g_scratchRoot.c_str());
    std::printf("=================================================================\n");

    test_constructors();
    test_properties();
    test_GetFileName();
    test_roundtrip_and_FirstRow();
    test_MaxLineCount();
    test_timestamp_formats();
    test_GetLastLine();
    test_SetLotData();
    test_MySaveFileByFileName();

    std::printf("\n-----------------------------------------------------------------\n");
    std::printf("  test_ptw1_mystringlist: %d passed, %d failed\n", g_pass, g_fail);
    std::printf("  RESULT: %s\n", g_fail == 0 ? "PASS" : "FAIL");
    std::printf("-----------------------------------------------------------------\n");
    return (g_fail == 0) ? 0 : 1;
}
