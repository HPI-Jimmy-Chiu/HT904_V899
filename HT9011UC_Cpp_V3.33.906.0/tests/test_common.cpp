// ===========================================================================
//  tests/test_common.cpp
//
//  W906-CommonWaveFile -- smoke tests for the 6 functions un-gated this wave
//  in common.cpp/.h:
//    WriteDataToFile(char*,char*,bool) / WriteDataToFile(AnsiString,AnsiString,bool)
//    CheckFileIsEmpty(AnsiString)
//    ReadDataFromFile(AnsiString)
//    MyForceDirectories(AnsiString,AnsiString)
//    GetLastOpenFN() / WriteLastDataFN(AnsiString)
//
//  All file I/O happens under a SCRATCH directory (TEMP/TMP, falls back to
//  cwd) -- never a production path. `LastDataPath` (a mutable global common.cpp
//  otherwise seeds to "D:\\HT9045\\SetUp.inf") is repointed to a scratch file
//  before exercising GetLastOpenFN/WriteLastDataFN, mirroring the asGeneralPath
//  repoint idiom already established in test_ini_helpers.cpp.
//
//  Self-contained CHECK harness (matches the other vclcompat/common tests).
// ===========================================================================
#include "vclcompat/vcl_compat.h"
#include "common.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef DeleteFile
#  undef DeleteFile
#endif
#ifdef CopyFile
#  undef CopyFile
#endif

static int g_fail = 0;
static int g_total = 0;

static void check(bool cond, const char* expr, const char* file, int line) {
    ++g_total;
    if (!cond) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n", file, line, expr);
    }
}
#define CHECK(cond) check((cond), #cond, __FILE__, __LINE__)

static AnsiString tmpPath(const char* leaf) {
    const char* t = std::getenv("TEMP");
    if (!t || !*t) t = std::getenv("TMP");
    if (!t || !*t) t = ".";
    return AnsiString(t) + AnsiString("\\") + AnsiString(leaf);
}

int main() {
    // =====================================================================
    //  A) WriteDataToFile(char*,...) + ReadDataFromFile round-trip.
    // =====================================================================
    {
        AnsiString path = tmpPath("ht9045_common_wdtf.txt");
        DeleteFile(path);   // start clean (ignore result)

        // NOTE on assertions below: WriteDataToFile/ReadDataFromFile both use
        // golden's literal TEXT-mode fopen (no "b"), which has a real, VERIFIED
        // golden bug documented on ReadDataFromFile's own comment in
        // common.cpp -- CRLF translation means the tail bytes of the returned
        // buffer (right before the forced final NUL) can be uninitialized
        // malloc() garbage, not real content, whenever the source has embedded
        // newlines (which WriteDataToFile's files always do, since it appends
        // one unconditionally). So checks here use strncmp against a known-safe
        // PREFIX length, never a full strcmp/NUL-terminated comparison, to
        // avoid asserting on that undefined tail.
        WriteDataToFile(path.c_str(), "line one", true);   // overwrite/create
        char* buf1 = ReadDataFromFile(path);
        CHECK(buf1 != NULL);
        if (buf1 != NULL) {
            CHECK(std::strncmp(buf1, "line one", 8) == 0);
            free(buf1);   // OWNERSHIP CONTRACT: caller frees
        }

        // Default bOverWrite=false -> append.
        WriteDataToFile(path.c_str(), "line two");
        char* buf2 = ReadDataFromFile(path);
        CHECK(buf2 != NULL);
        if (buf2 != NULL) {
            CHECK(std::strncmp(buf2, "line one\nline two", 17) == 0);
            free(buf2);
        }

        // AnsiString overload forwards to the char* overload -- prove it does
        // NOT recurse (would stack-overflow/crash the whole test process if it
        // did; this is the exact bug this wave's own build+gdb pass caught and
        // fixed by switching the char* overload's params to `const char*`).
        WriteDataToFile(AnsiString(path), AnsiString("line three"), false);
        char* buf3 = ReadDataFromFile(path);
        CHECK(buf3 != NULL);
        if (buf3 != NULL) {
            CHECK(std::strncmp(buf3, "line one\nline two\nline three", 28) == 0);
            free(buf3);
        }

        // AnsiString overwrite=true truncates.
        WriteDataToFile(AnsiString(path), AnsiString("fresh"), true);
        char* buf4 = ReadDataFromFile(path);
        CHECK(buf4 != NULL);
        if (buf4 != NULL) {
            CHECK(std::strncmp(buf4, "fresh", 5) == 0);
            free(buf4);
        }

        DeleteFile(path);
    }

    // =====================================================================
    //  B) ReadDataFromFile on a missing file -> NULL.
    // =====================================================================
    {
        AnsiString path = tmpPath("ht9045_common_no_such_file.txt");
        DeleteFile(path);
        char* buf = ReadDataFromFile(path);
        CHECK(buf == NULL);
    }

    // =====================================================================
    //  C) CheckFileIsEmpty -- existing-empty / existing-non-empty / missing.
    //     GOLDEN BUG (verified empirically -- see CheckFileIsEmpty's own
    //     comment in common.cpp): the return value is effectively INVERTED
    //     from what the function's name suggests.
    //       - missing file            -> true
    //       - existing, has content   -> true
    //       - existing, genuinely empty -> false
    //     i.e. true means "missing OR non-empty"; false means "exists and is
    //     empty". Do not naively assume true=="is empty" here.
    // =====================================================================
    {
        AnsiString emptyPath = tmpPath("ht9045_common_empty.txt");
        DeleteFile(emptyPath);
        // NOTE: WriteDataToFile always appends a trailing "\n" after cData, so
        // even an empty cData would yield a 1-byte file, not a genuinely empty
        // one. Use fopen/fclose directly to produce a true 0-byte file.
        FILE* f = fopen(emptyPath.c_str(), "w");
        CHECK(f != NULL);
        if (f != NULL) fclose(f);
        CHECK(CheckFileIsEmpty(emptyPath) == false);   // golden bug: genuinely-empty -> false

        AnsiString nonEmptyPath = tmpPath("ht9045_common_nonempty.txt");
        DeleteFile(nonEmptyPath);
        WriteDataToFile(nonEmptyPath.c_str(), "not empty", true);
        CHECK(CheckFileIsEmpty(nonEmptyPath) == true);   // golden bug: has content -> true

        AnsiString missingPath = tmpPath("ht9045_common_missing.txt");
        DeleteFile(missingPath);
        CHECK(CheckFileIsEmpty(missingPath) == true);   // missing -> true (same as "has content")

        DeleteFile(emptyPath);
        DeleteFile(nonEmptyPath);
    }

    // =====================================================================
    //  D) MyForceDirectories -- creates a nested scratch directory.
    // =====================================================================
    {
        AnsiString base = tmpPath("ht9045_common_mfd_root");
        AnsiString nested = base + AnsiString("\\a\\b\\c\\");

        // Best-effort pre-clean (ignore result; directory may not exist yet).
        RemoveDir(base + AnsiString("\\a\\b\\c"));
        RemoveDir(base + AnsiString("\\a\\b"));
        RemoveDir(base + AnsiString("\\a"));
        RemoveDir(base);

        CHECK(DirectoryExists(nested) == false);
        int ret = MyForceDirectories(nested, AnsiString("test_common"));
        CHECK(ret == 1);
        CHECK(DirectoryExists(nested) == true);

        // Idempotent: calling again on an already-existing directory is a
        // no-op success (golden: DirectoryExists(...)==false guards ForceDirectories).
        int ret2 = MyForceDirectories(nested, AnsiString("test_common"));
        CHECK(ret2 == 1);

        // Empty path -> -1 (golden: RecordProcess + return -1).
        CHECK(MyForceDirectories(AnsiString(""), AnsiString("test_common")) == -1);

        // Best-effort cleanup (reverse order; ignore failures).
        RemoveDir(base + AnsiString("\\a\\b\\c"));
        RemoveDir(base + AnsiString("\\a\\b"));
        RemoveDir(base + AnsiString("\\a"));
        RemoveDir(base);
    }

    // =====================================================================
    //  E) GetLastOpenFN / WriteLastDataFN round-trip against a repointed
    //     LastDataPath (never touches the real D:\HT9045\SetUp.inf default).
    // =====================================================================
    {
        AnsiString savedLastDataPath = LastDataPath;   // restore at the end

        AnsiString scratchPath = tmpPath("ht9045_common_lastdata.inf");
        DeleteFile(scratchPath);
        LastDataPath = scratchPath;

        // E1) File does not exist yet -> GetLastOpenFN returns the "Fail Open"
        //     default (golden: ShowMyMessage("LastData does not exist") -- our
        //     stub is a no-op, so only the return value is observable).
        CHECK(FileExists(LastDataPath) == false);
        AnsiString r1 = GetLastOpenFN();
        CHECK(r1 == AnsiString("Fail Open"));

        // E2) WriteLastDataFN bootstrap branch (LastDataPath still absent at
        //     call time -> golden's FileCreate/FileWrite/FileClose path,
        //     substituted this wave with TStringList Add+SaveToFile -- see
        //     common.cpp's own deviation note on WriteLastDataFN).
        WriteLastDataFN(AnsiString("SETUP_A.INI"));
        CHECK(FileExists(LastDataPath) == true);
        AnsiString r2 = GetLastOpenFN();
        CHECK(r2 == AnsiString("SETUP_A.INI"));

        // E3) WriteLastDataFN if-branch (LastDataPath now exists -> TStringList
        //     LoadFromFile/Clear/Add/SaveToFile path).
        WriteLastDataFN(AnsiString("SETUP_B.INI"));
        AnsiString r3 = GetLastOpenFN();
        CHECK(r3 == AnsiString("SETUP_B.INI"));

        DeleteFile(LastDataPath);
        LastDataPath = savedLastDataPath;
    }

    if (g_fail == 0)
        std::printf("test_common: ALL %d checks passed\n", g_total);
    else
        std::printf("test_common: %d/%d checks FAILED\n", g_fail, g_total);

    return g_fail == 0 ? 0 : 1;
}
