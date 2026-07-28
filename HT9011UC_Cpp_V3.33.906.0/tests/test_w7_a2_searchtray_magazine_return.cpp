// =============================================================================
//  test_w7_a2_searchtray_magazine_return.cpp -- W7-A2 VERIFY:
//  SearchTrayToPlace_Magazine() return-type smoke test
//
//  Translation wave: W906 W7-A2
//  Author: AI(W906-W7-A2) 20260728
//  Corrected: AI(W906-A2-followup) 20260728 -- see "WHAT THIS DOES NOT PROVE"
//  below; the original banner overclaimed what a compile/link test can catch.
//
//  THE BUG THIS WAVE FIXED
//  ------------------------
//  golden declares `int  SearchTrayToPlace_Magazine();` (aoutarm9045.h:63) and
//  DEFINES it returning int, with the return value genuinely consumed at one
//  call site (aoutarm9045.cpp:1369: `return SearchTrayToPlace_Magazine();`).
//
//  Before this wave, the ported tree had a real ODR violation: 14 of the 27
//  aoutarm9045_*.cpp site-variant TUs forward-declared this function `void`,
//  13 declared it `int`, and the shim definition (aoutarm_shims.cpp) DEFINED
//  it `void`. This built and linked "successfully" under MinGW (ODR
//  violations across TUs are not required to be diagnosed). This wave
//  reconciled all 27 declaring TUs plus the shim definition to `int` (fix
//  verified by direct inspection of all 27 files, not by this test).
//
//  WHAT THIS TEST ACTUALLY CHECKS
//  -------------------------------
//  This TU forward-declares SearchTrayToPlace_Magazine() as `int`, calls it
//  through a plain assignment and through a `return SearchTrayToPlace_
//  Magazine();`-shaped helper (mirroring golden aoutarm9045.cpp:1369), and
//  links against ht9045_sm so the call resolves to the real, single,
//  unified definition in aoutarm_shims.cpp -- not a locally-defined stand-in.
//  A passing run proves: the symbol still exists, still links, and using it
//  as `int` from a fresh call site builds and runs without crashing.
//
//  WHAT THIS TEST DOES NOT PROVE (do not re-add this claim)
//  -----------------------------------------------------------
//  A function's return type is NOT part of its C++ (Itanium/MSVC) mangled
//  name. Verified directly: a two-TU repro with `int f();`/`int rv = f();`
//  in one TU and `void f(){}` in another built cleanly under MinGW g++ at
//  -Wall -Wextra, linked with no diagnostic, and ran -- "returning" whatever
//  garbage happened to be in the return register. Consequently:
//    * If aoutarm_shims.cpp's definition ever regresses back to `void`, this
//      TU will *not* fail to compile or link -- it will keep building and
//      passing (or fail its CHECKs with a garbage value, which is not the
//      same guarantee). A return-type ODR violation is not something any
//      single-TU compile/link test can catch; the standard does not require
//      it to be diagnosed at all.
//    * This TU includes none of the other 27 aoutarm9045_*.cpp site-variant
//      files, so it is structurally blind to a declaration regression in any
//      of them -- it can only ever observe its own forward declaration and
//      the shim definition it links against.
//  The real guard against this class of regression is manual/grep review of
//  all 27 declaring TUs (as done for this wave's fix); this test's value is
//  limited to a basic "the symbol still exists and links" smoke check.
// =============================================================================
#include <cstdio>
#include <cassert>

// Forward-declare exactly like every translated aoutarm9045_*.cpp site variant
// does (golden aoutarm9045.h:63) -- this is the "future call site" shape.
int SearchTrayToPlace_Magazine();

// Mirrors golden aoutarm9045.cpp:1369 verbatim: `return SearchTrayToPlace_Magazine();`
// inside an int-returning function.  Reproduces the shape of the one golden
// call site that genuinely consumes the return value (see banner for what
// this can and cannot prove).
static int MirrorGoldenCallSite_1369()
{
    return SearchTrayToPlace_Magazine();
}

static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

int main()
{
    printf("==== W7-A2 SearchTrayToPlace_Magazine return-type smoke test ====\n");

    // (1) Direct consumption: this TU's own declaration + the linked
    //     aoutarm_shims.cpp definition both agree on `int` today, so this
    //     builds, links, and runs. It does NOT prove the definition cannot
    //     silently regress to `void` -- see banner "WHAT THIS TEST DOES NOT
    //     PROVE".
    //     TODO(W7-A2 follow-up, LOW-1): `rv == 0` asserts the OFFLINE STAND-IN
    //     value from aoutarm_shims.cpp, not a golden spec -- golden's real
    //     body (aoutarm9045.cpp:1420) never returns a guaranteed 0; it returns
    //     2, iSortTrayIndex[k], OutArmSuck.iWhichAuto[i][j], or
    //     Prod.iIfErrorT6 depending on path (golden :1453,1469,1530,1535,
    //     1540,1546,1552,1558). Rewrite this CHECK when that body is really
    //     translated -- do not mistake a legitimate value change there for a
    //     regression.
    int rv = SearchTrayToPlace_Magazine();
    CHECK(rv == 0, "SearchTrayToPlace_Magazine() returns int 0 (offline stand-in value from aoutarm_shims.cpp, NOT golden's spec)");

    // (2) golden aoutarm9045.cpp:1369 call-site shape: `return SearchTrayToPlace_Magazine();`
    //     from inside an int-returning function -- this is the ONE golden call
    //     site that genuinely consumes the value (per W7 plan V7); it is not
    //     translated into this tree's aoutarm9045.cpp yet, so this reproduces
    //     that call-site shape against the offline stand-in.
    //     TODO(W7-A2 follow-up, LOW-1): same caveat as (1) -- `viaHelper == 0`
    //     is the offline stand-in value, not a golden invariant. Rewrite when
    //     golden aoutarm9045.cpp:1420 is really translated.
    int viaHelper = MirrorGoldenCallSite_1369();
    CHECK(viaHelper == 0, "return SearchTrayToPlace_Magazine(); (golden aoutarm9045.cpp:1369 shape) round-trips as int 0 (offline stand-in value, NOT golden's spec)");

    printf("==== SUMMARY: %d passed, %d failed ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
