// ===========================================================================
//  tests/test_ExternFunction.cpp
//  Correctness tests for translated Public/ExternFunction (W1 pure-logic leaves).
//
//  Oracles derived directly from BCB6 golden source
//  (HT9011UC_Code_V3.33.906.0_20260618/Public/ExternFunction.cpp:126-220):
//    * Math_AdjustAngle  -- two-branch angle normalisation (formula oracle).
//    * String_JustNumber -- leading-numeric-prefix extraction + dot canonicalise.
//    * String_EraseLSpace / String_EraseRSpace -- in-place 1-based space strip.
//    * RecordTime        -- clock()-based stopwatch (invariant: start==0, read>=0).
//
//  Build: linked against ht9045_public (which links vclcompat).
//  Non-zero exit on any failure.
// ===========================================================================

#include "Public/ExternFunction.h"

#include <cstdio>
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

static void checkEqStr(const AnsiString& got, const char* expect,
                       const char* what, const char* file, int line) {
    ++g_total;
    if (std::string(got.c_str()) != std::string(expect)) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got \"%s\" expected \"%s\"\n",
                    file, line, what, got.c_str(), expect);
    }
}
#define CHECK_EQ_STR(got, expect) checkEqStr((got), (expect), #got, __FILE__, __LINE__)

static void checkEqD(double got, double expect, const char* what,
                     const char* file, int line) {
    ++g_total;
    double diff = got - expect;
    if (diff < 0) diff = -diff;
    if (diff > 1e-9) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got %.10g expected %.10g\n",
                    file, line, what, got, expect);
    }
}
#define CHECK_EQ_D(got, expect) checkEqD((got), (expect), #got, __FILE__, __LINE__)

int main()
{
    // -----------------------------------------------------------------------
    // 1. Math_AdjustAngle  (BCB6 :126-151)
    //    bSign==true  branch: only adds 360 while <= -360 AND angle<0;
    //                 positive angles + angles already in (-360,0] untouched.
    //    bSign==false branch: wrap into [0,360).
    // -----------------------------------------------------------------------
    // bSign == true (default)
    CHECK_EQ_D(Math_AdjustAngle(-450, true),  -90.0);   // -450 + 360 = -90
    CHECK_EQ_D(Math_AdjustAngle(-360, true),    0.0);   // -360 + 360 = 0
    CHECK_EQ_D(Math_AdjustAngle(-359, true), -359.0);   // already > -360, untouched
    CHECK_EQ_D(Math_AdjustAngle(-90,  true),  -90.0);   // negative but > -360, untouched
    CHECK_EQ_D(Math_AdjustAngle(-720, true),    0.0);   // -720 +360 +360 = 0
    CHECK_EQ_D(Math_AdjustAngle(-721, true),   -1.0);   // -721 +360 +360 = -1
    CHECK_EQ_D(Math_AdjustAngle( 90,  true),   90.0);   // positive -> else?  no: bSign&&<0 false -> else branch wraps; 90 in [0,360)
    CHECK_EQ_D(Math_AdjustAngle( 0,   true),    0.0);   // not <0 -> else branch -> stays 0
    // bSign == false  -> always wrap into [0,360)
    CHECK_EQ_D(Math_AdjustAngle(-450, false), 270.0);   // -450 +360 = -90 +360 = 270
    CHECK_EQ_D(Math_AdjustAngle( 720, false),   0.0);   // 720 -360 -360 = 0
    CHECK_EQ_D(Math_AdjustAngle( 370, false),  10.0);   // 370 -360 = 10
    CHECK_EQ_D(Math_AdjustAngle(-30,  false), 330.0);   // -30 +360 = 330
    CHECK_EQ_D(Math_AdjustAngle( 359, false), 359.0);   // already in range
    // default arg is bSign=true
    CHECK_EQ_D(Math_AdjustAngle(-450),        -90.0);

    // -----------------------------------------------------------------------
    // 2. String_JustNumber  (BCB6 :153-185)
    // -----------------------------------------------------------------------
    CHECK_EQ_STR(String_JustNumber("  -12.3abc"), "-12.3");
    CHECK_EQ_STR(String_JustNumber(".5"),         "0.5");
    CHECK_EQ_STR(String_JustNumber("+.5"),        "+0.5");
    CHECK_EQ_STR(String_JustNumber("1.2.3"),      "1.2");    // 2nd dot stops loop
    CHECK_EQ_STR(String_JustNumber("abc"),        "");
    CHECK_EQ_STR(String_JustNumber("123"),        "123");
    CHECK_EQ_STR(String_JustNumber("  42 "),      "42");     // leading spaces stripped, trailing space stops
    CHECK_EQ_STR(String_JustNumber("-0.001x"),    "-0.001");
    CHECK_EQ_STR(String_JustNumber("+7"),         "+7");
    CHECK_EQ_STR(String_JustNumber(""),           "");
    CHECK_EQ_STR(String_JustNumber("3.14159"),    "3.14159");
    // sign only accepted at position 1; an internal sign stops the scan
    CHECK_EQ_STR(String_JustNumber("12-34"),      "12");

    // -----------------------------------------------------------------------
    // 3. String_EraseLSpace / String_EraseRSpace  (BCB6 :187-203)  (in-place)
    // -----------------------------------------------------------------------
    {
        AnsiString s("   hello");
        String_EraseLSpace(s);
        CHECK_EQ_STR(s, "hello");
    }
    {
        AnsiString s("world   ");
        String_EraseRSpace(s);
        CHECK_EQ_STR(s, "world");
    }
    {
        AnsiString s("   both sides   ");
        String_EraseLSpace(s);
        String_EraseRSpace(s);
        CHECK_EQ_STR(s, "both sides");        // interior spaces preserved
    }
    {
        AnsiString s("     ");                // all spaces -> empty
        String_EraseLSpace(s);
        CHECK_EQ_STR(s, "");
    }
    {
        AnsiString s("     ");
        String_EraseRSpace(s);
        CHECK_EQ_STR(s, "");
    }
    {
        AnsiString s("nospace");              // no-op
        String_EraseLSpace(s);
        String_EraseRSpace(s);
        CHECK_EQ_STR(s, "nospace");
    }
    {
        AnsiString s("");                     // empty stays empty (no underflow)
        String_EraseLSpace(s);
        String_EraseRSpace(s);
        CHECK_EQ_STR(s, "");
    }

    // -----------------------------------------------------------------------
    // 4. RecordTime  (BCB6 :205-220)
    //    Invariants: start returns 0; a subsequent read returns >= 0 ticks.
    // -----------------------------------------------------------------------
    {
        int started = RecordTime(true);
        CHECK(started == 0);
        // burn a little CPU so elapsed is well-defined and non-negative
        volatile double acc = 0.0;
        for (int i = 0; i < 2000000; ++i) acc += (double)i * 0.5;
        (void)acc;
        int elapsed = RecordTime(false);
        CHECK(elapsed >= 0);
    }

    // -----------------------------------------------------------------------
    std::printf("test_ExternFunction: %d / %d passed\n", g_total - g_fail, g_total);
    return (g_fail == 0) ? 0 : 1;
}
