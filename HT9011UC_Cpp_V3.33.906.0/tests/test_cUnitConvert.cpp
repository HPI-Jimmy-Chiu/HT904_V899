// ===========================================================================
//  tests/test_cUnitConvert.cpp
//  Golden oracle test for the W2 partial extract of cUnitConvert.
//
//  Tests ONLY the two pure functions that were translated:
//    iUnitMultiply100(double)   -- BCB6 cUnitConvert.cpp:13-20
//    iUnitMultiply1000(double)  -- BCB6 cUnitConvert.cpp:22-25
//
//  The Do*Convert stubs are NOT tested here (deferred to W6/W7).
//
//  Toolchain: MinGW g++ 6.3+, C++17.  Non-zero exit on any failure.
// ===========================================================================
#include "cUnitConvert.h"

#include <cstdio>
#include <cmath>     // std::fabs

static int g_fail  = 0;
static int g_total = 0;

static void checkInt(int got, int expect, const char* expr, const char* file, int line)
{
    ++g_total;
    if (got != expect) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got %d  expected %d\n",
                    file, line, expr, got, expect);
    }
}
#define CHECK_INT(expr, expect) checkInt((expr), (expect), #expr, __FILE__, __LINE__)

int main()
{
    // ===================================================================
    //  iUnitMultiply100
    //  BCB6 cUnitConvert.cpp:13-20
    //  FormatFloat("0.00", Data*100.0) -> atoi()
    //  The indirection through FormatFloat normalises IEEE 754 noise:
    //    e.g. 0.03*100 = 2.9999... -> "3.00" -> 3 (not 2).
    // ===================================================================

    // Basic positive values
    CHECK_INT(iUnitMultiply100(1.00),  100);   // 100.00 -> "100.00" -> 100
    CHECK_INT(iUnitMultiply100(0.00),    0);   //   0.00 -> "0.00"   ->   0
    CHECK_INT(iUnitMultiply100(2.50),  250);   // 250.00 -> "250.00" -> 250

    // FP-noise guard: values whose raw double product would be .999...
    // 0.03 * 100.0 can be 2.9999999... in IEEE 754 double.
    // FormatFloat("0.00", 2.9999...) rounds to "3.00"; atoi = 3.
    CHECK_INT(iUnitMultiply100(0.03),    3);

    // Typical mm offset values used by the handler (BCB6 cprod.h range)
    CHECK_INT(iUnitMultiply100(12.34), 1234);  // 1234.00 -> 1234
    CHECK_INT(iUnitMultiply100(0.10),    10);  //   10.00 ->   10
    CHECK_INT(iUnitMultiply100(0.05),     5);  //    5.00 ->    5

    // Negative values (offsets can be negative)
    CHECK_INT(iUnitMultiply100(-1.00), -100);
    CHECK_INT(iUnitMultiply100(-0.05),   -5);

    // Non-trivial fractional: 2.344 * 100 = 234.4 -> "234.40" -> 234
    CHECK_INT(iUnitMultiply100(2.344),  234);

    // 2.345 * 100.0: exact double is 234.49999... (2.345 is not representable)
    // FormatFloat("0.00", 234.4999...) -> "234.50" -> atoi = 234
    // (The fractional ".50" is discarded by atoi.)
    CHECK_INT(iUnitMultiply100(2.345),  234);

    // Larger value
    CHECK_INT(iUnitMultiply100(100.00), 10000);

    // ===================================================================
    //  iUnitMultiply1000
    //  BCB6 cUnitConvert.cpp:22-25
    //  return (int)(Data * 1000.0)  -- truncation toward zero
    // ===================================================================

    // NOTE: 1.234 in IEEE 754 double = 1.23399999...
    //   1.23399999... * 1000.0 = 1233.999... -> truncation = 1233 (not 1234).
    //   This matches BCB6 behavior: return Data*1000.0; with implicit double->int.
    //   iUnitMultiply1000 does NOT use FormatFloat normalisation (unlike x100).
    //   BCB6 cUnitConvert.cpp:24 confirms: return Data*1000.0; (no FormatFloat).
    CHECK_INT(iUnitMultiply1000(1.234),  1233);   // 1233.999... truncates to 1233
    CHECK_INT(iUnitMultiply1000(0.000),     0);
    CHECK_INT(iUnitMultiply1000(1.000),  1000);
    CHECK_INT(iUnitMultiply1000(0.001),     1);   // 1.0 -> 1
    CHECK_INT(iUnitMultiply1000(2.500),  2500);
    CHECK_INT(iUnitMultiply1000(-1.234), -1233);  // -1233.999... truncates to -1233

    // ===================================================================
    //  Summary
    // ===================================================================
    if (g_fail == 0) {
        std::printf("PASS  %d/%d tests passed\n", g_total, g_total);
        return 0;
    } else {
        std::printf("FAIL  %d/%d tests failed\n", g_fail, g_total);
        return 1;
    }
}
