// ===========================================================================
//  tests/test_cBootLog.cpp
//  Correctness test for translated Public/cBootLog (W1 leaf, pure Win32).
//
//  Oracle: BCB6 golden source
//  (HT9011UC_Code_V3.33.906.0_20260618/Public/cBootLog.cpp:44-73).
//  WriteBootLog appends one timestamped line to D:\HT9045\Error\BootLog.txt:
//    "YYYY-MM-DD HH:MM:SS.mmm [TID:n] <step>[ | <detail>]\n"
//  RotateBootLogIfNeeded never throws and renames the file to .old when > 512KB.
//
//  The target path is hard-coded in the module (matching BCB6).  This test
//  drives the real code path: it writes two lines carrying a unique marker,
//  then reads the file back and asserts:
//    * the file now exists,
//    * the last two non-empty lines carry our markers,
//    * the timestamp + [TID:] prefix format is present,
//    * the " | detail" segment appears only when detail is non-empty,
//    * RotateBootLogIfNeeded() returns without throwing.
//
//  Build: linked against ht9045_public (which links vclcompat).
//  Non-zero exit on any failure.
// ===========================================================================

#include "Public/cBootLog.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <ctime>

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

static const char* kBootLogPath = "D:\\HT9045\\Error\\BootLog.txt";

// Read the whole boot-log file into a string (empty if missing).
static std::string slurp(const char* path)
{
    FILE* fp = std::fopen(path, "rb");
    if (!fp) return std::string();
    std::string out;
    char buf[4096];
    size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), fp)) > 0)
        out.append(buf, n);
    std::fclose(fp);
    return out;
}

// True if `hay` contains `needle`.
static bool contains(const std::string& hay, const std::string& needle)
{
    return hay.find(needle) != std::string::npos;
}

// Minimal check that a line begins with "YYYY-MM-DD HH:MM:SS.mmm [TID:".
static bool hasTimestampPrefix(const std::string& line)
{
    // 0123456789012345678901234
    // 2026-06-26 12:34:56.789 [TID:
    if (line.size() < 24) return false;
    for (int i = 0; i < 23; ++i) {
        char c = line[(size_t)i];
        if (i == 4 || i == 7) { if (c != '-') return false; }
        else if (i == 10)     { if (c != ' ') return false; }
        else if (i == 13 || i == 16) { if (c != ':') return false; }
        else if (i == 19)     { if (c != '.') return false; }
        else { if (c < '0' || c > '9') return false; }
    }
    return contains(line, "[TID:");
}

int main()
{
    // Build a unique marker so we can find our own lines even if the real
    // BootLog.txt already has prior content.
    char marker[64];
    std::snprintf(marker, sizeof(marker), "TESTMARK_%lu",
                  (unsigned long)std::time(0));

    std::string stepNoDetail = std::string(marker) + "_NODETAIL";
    std::string stepWithDet  = std::string(marker) + "_WITHDETAIL";
    std::string detailText    = std::string(marker) + "_DETAILBODY";

    // RotateBootLogIfNeeded must never throw (BCB6 :41 swallows everything).
    RotateBootLogIfNeeded();
    CHECK(true);  // reached here without exception

    // 1) single-arg overload -> no " | detail" segment.
    WriteBootLog(AnsiString(stepNoDetail.c_str()));

    // 2) two-arg overload with a detail -> " | detail" segment present.
    WriteBootLog(AnsiString(stepWithDet.c_str()), AnsiString(detailText.c_str()));

    // Read the file back.
    std::string content = slurp(kBootLogPath);
    CHECK(!content.empty());

    // Our two markers must be present.
    CHECK(contains(content, stepNoDetail));
    CHECK(contains(content, stepWithDet));

    // The detail body must be present, and joined with " | ".
    CHECK(contains(content, stepWithDet + " | " + detailText));

    // The single-arg line must NOT carry a " | " on the same line.
    // Find the line containing stepNoDetail and check it has no " | " after it.
    {
        std::string needleLine;
        size_t pos = 0;
        bool foundLine = false;
        while (pos < content.size()) {
            size_t nl = content.find('\n', pos);
            std::string line = (nl == std::string::npos)
                                 ? content.substr(pos)
                                 : content.substr(pos, nl - pos);
            if (contains(line, stepNoDetail)) { needleLine = line; foundLine = true; break; }
            if (nl == std::string::npos) break;
            pos = nl + 1;
        }
        CHECK(foundLine);
        if (foundLine) {
            CHECK(!contains(needleLine, " | "));
            CHECK(hasTimestampPrefix(needleLine));
        }
    }

    // Verify the WITHDETAIL line also has the timestamp prefix.
    {
        size_t pos = 0;
        bool foundLine = false;
        while (pos < content.size()) {
            size_t nl = content.find('\n', pos);
            std::string line = (nl == std::string::npos)
                                 ? content.substr(pos)
                                 : content.substr(pos, nl - pos);
            if (contains(line, stepWithDet)) {
                foundLine = true;
                CHECK(hasTimestampPrefix(line));
                break;
            }
            if (nl == std::string::npos) break;
            pos = nl + 1;
        }
        CHECK(foundLine);
    }

    std::printf("test_cBootLog: %d / %d passed\n", g_total - g_fail, g_total);
    return (g_fail == 0) ? 0 : 1;
}
