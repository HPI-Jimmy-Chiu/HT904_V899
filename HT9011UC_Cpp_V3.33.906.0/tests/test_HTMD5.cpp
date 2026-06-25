// ===========================================================================
//  tests/test_HTMD5.cpp
//  Basic correctness test for the translated Public/HTMD5 module.
//
//  Verifies:
//    1. md5() returns the correct RFC 1321 test-vector digests.
//    2. MD5 class: incremental update() matches single-shot ctor.
//    3. CheckFilenameExtension() wildcard matching behaviour.
//    4. md5_File() on a non-existent path returns "File not exists!".
//    5. SearchFile() on a non-existent directory returns 0 (no crash).
//    6. TStrings* polymorphism: TStringList* accepted where TStrings* expected.
//
//  Build: linked against ht9045_public (which in turn links vclcompat).
//  Non-zero exit on any failure.
// ===========================================================================

#include "Public/HTMD5.h"

#include <cstdio>
#include <cstring>
#include <string>

static int g_fail  = 0;
static int g_total = 0;

static void check(bool cond, const char* expr, const char* file, int line) {
    ++g_total;
    if (!cond) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n", file, line, expr);
    }
}
#define CHECK(cond) check((cond), #cond, __FILE__, __LINE__)

static void checkEq(const AnsiString& got, const char* expect,
                    const char* what, const char* file, int line) {
    ++g_total;
    if (std::string(got.c_str()) != std::string(expect)) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got \"%s\" expected \"%s\"\n",
                    file, line, what, got.c_str(), expect);
    }
}
#define CHECK_EQ(got, expect) checkEq((got), (expect), #got, __FILE__, __LINE__)

// ---------------------------------------------------------------------------
//  RFC 1321 MD5 test vectors
//  Source: https://www.rfc-editor.org/rfc/rfc1321  Appendix A.5
// ---------------------------------------------------------------------------
static const struct { const char* input; const char* digest; } kVectors[] = {
    { "",               "d41d8cd98f00b204e9800998ecf8427e" },
    { "a",              "0cc175b9c0f1b6a831c399e269772661" },
    { "abc",            "900150983cd24fb0d6963f7d28e17f72" },
    { "message digest", "f96b697d7cb7938d525a2f31aaf161d0" },
    { "abcdefghijklmnopqrstuvwxyz",
                        "c3fcd3d76192e4007dfb496cca67e13b" },
};
static const int kNVectors = (int)(sizeof(kVectors)/sizeof(kVectors[0]));

int main()
{
    // -----------------------------------------------------------------------
    // 1. Free function md5() - RFC 1321 vectors
    // -----------------------------------------------------------------------
    for (int i = 0; i < kNVectors; ++i)
    {
        AnsiString result = md5(AnsiString(kVectors[i].input));
        checkEq(result, kVectors[i].digest,
                kVectors[i].input[0] ? kVectors[i].input : "(empty)",
                __FILE__, __LINE__);
    }

    // -----------------------------------------------------------------------
    // 2. MD5 class: incremental update matches single-shot ctor
    // -----------------------------------------------------------------------
    {
        // Single-shot via ctor
        std::string s("The quick brown fox jumps over the lazy dog");
        MD5 m1(s);
        AnsiString h1 = m1.hexdigest();

        // Incremental: feed in 3 pieces
        MD5 m2;
        m2.update("The quick brown", 15);
        m2.update(" fox jumps over", 15);
        m2.update(" the lazy dog",   13);
        m2.finalize();
        AnsiString h2 = m2.hexdigest();

        checkEq(h1, "9e107d9d372bb6826bd81d3542a419d6", "quick-brown-fox single-shot", __FILE__, __LINE__);
        checkEq(h2, "9e107d9d372bb6826bd81d3542a419d6", "quick-brown-fox incremental", __FILE__, __LINE__);
        CHECK(h1 == h2);
    }

    // -----------------------------------------------------------------------
    // 3. CheckFilenameExtension wildcard matching
    // -----------------------------------------------------------------------
    CHECK(CheckFilenameExtension("test.Data",   "*.Data")  == true);
    CHECK(CheckFilenameExtension("test.data",   "*.Data")  == true);  // case-insensitive
    CHECK(CheckFilenameExtension("test.txt",    "*.Data")  == false);
    CHECK(CheckFilenameExtension("abc.exe",     "*.exe")   == true);
    CHECK(CheckFilenameExtension("abc.exe.bak", "*.exe")   == false);
    CHECK(CheckFilenameExtension("file.txt",    "*.*")     == true);
    CHECK(CheckFilenameExtension("noext",       "*.*")     == false);
    CHECK(CheckFilenameExtension("a.b",         "?.b")     == true);
    CHECK(CheckFilenameExtension("ab.b",        "?.b")     == false);

    // -----------------------------------------------------------------------
    // 4. md5_File on a non-existent path
    // -----------------------------------------------------------------------
    {
        AnsiString r = md5_File("C:\\does_not_exist_XYZ.bin");
        CHECK(r == "File not exists!");
    }

    // -----------------------------------------------------------------------
    // 5. SearchFile on a non-existent directory returns 0, no crash
    // -----------------------------------------------------------------------
    {
        TStringList* sl = new TStringList();
        int n = SearchFile("C:\\does_not_exist_XYZ_dir", "*.txt", sl);
        CHECK(n == 0);
        CHECK(sl->Count == 0);
        delete sl;
    }

    // -----------------------------------------------------------------------
    // 6. TStrings* polymorphism: TStringList* passed as TStrings*
    // -----------------------------------------------------------------------
    {
        TStringList* sl = new TStringList();
        TStrings* base = sl;   // must compile: TStringList IS-A TStrings
        base->Add("hello");
        base->Add("world");
        CHECK(base->GetCount() == 2);
        checkEq(base->GetString(0), "hello", "TStrings->GetString(0)", __FILE__, __LINE__);
        checkEq(base->GetString(1), "world", "TStrings->GetString(1)", __FILE__, __LINE__);
        base->Clear();
        CHECK(base->GetCount() == 0);
        delete sl;
    }

    // -----------------------------------------------------------------------
    // Summary
    // -----------------------------------------------------------------------
    std::printf("test_HTMD5: %d / %d passed\n", g_total - g_fail, g_total);
    return (g_fail == 0) ? 0 : 1;
}
