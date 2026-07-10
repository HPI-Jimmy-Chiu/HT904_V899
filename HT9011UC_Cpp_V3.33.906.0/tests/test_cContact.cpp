// tests/test_cContact.cpp
// Verification harness for the cContact calc-core translation (ComputeMaxIndexForceLimit /
// ComputeMinForce).
//
// Exercises the translated public API with input->expected-output values hand-derived from the
// BCB6 formula in the ORIGINAL golden reference
//   HT9011UC_Code_V3.33.906.0_20260618/cContact.cpp:18893-19022
//
// LIMITATION (stated explicitly, same as test_ContactForce.cpp): we CANNOT run the original BCB6
// binary (no Borland compiler in this environment).  Verification here is therefore:
//   (1) the translation compiles clean under MinGW g++, AND
//   (2) its outputs equal values hand-computed / transcribed from the BCB6 source (golden
//       file:line cited per case).
//
// No external test framework: a tiny check harness prints PASS/FAIL per case and a final summary,
// and returns non-zero on ANY failure.

#include "cContact.h"
#include <cmath>
#include <cstdio>

static int g_pass = 0;
static int g_fail = 0;

static bool feq(double a, double b, double eps = 1e-9)
{
    return std::fabs(a - b) <= eps * (1.0 + std::fabs(b));
}

static void check_d(const char* name, double got, double expected)
{
    if (feq(got, expected))
    {
        printf("PASS  %-56s got=%.4f  exp=%.4f\n", name, got, expected);
        ++g_pass;
    }
    else
    {
        printf("FAIL  %-56s got=%.4f  exp=%.4f\n", name, got, expected);
        ++g_fail;
    }
}

int main()
{
    printf("=== cContact translation verification ===\n");
    printf("(values hand-derived/transcribed from BCB6 cContact.cpp; original binary NOT run)\n\n");

    // =========================================================================================
    // ComputeMaxIndexForceLimit -- BCB6 cContact.cpp:18893-18955
    // =========================================================================================
    printf("-- ComputeMaxIndexForceLimit: fixed-kg INDEX_PRESS_TYPE branches (golden :18896-18931) --\n");
    {
        // Every fixed-kg branch is independent of dDutCount/iTestMode/bD27UseSingleSite85kg/
        // ItemIndex -- pass harmless defaults (0 / SingleSite / false / 0) to prove that.
        check_d("e240KG -> 240", ComputeMaxIndexForceLimit(e240KG, 0, SingleSite, false, 0), 240.0);
        check_d("e120KG -> 120", ComputeMaxIndexForceLimit(e120KG, 0, SingleSite, false, 0), 120.0);
        check_d("e500KG -> 500", ComputeMaxIndexForceLimit(e500KG, 0, SingleSite, false, 0), 500.0);
        check_d("e400KG -> 400", ComputeMaxIndexForceLimit(e400KG, 0, SingleSite, false, 0), 400.0);
        check_d("e360KG -> 360", ComputeMaxIndexForceLimit(e360KG, 0, SingleSite, false, 0), 360.0);
        check_d("e260KG -> 260", ComputeMaxIndexForceLimit(e260KG, 0, SingleSite, false, 0), 260.0);
        check_d("e160KG -> 160", ComputeMaxIndexForceLimit(e160KG, 0, SingleSite, false, 0), 160.0);
        check_d("e640KG -> 640", ComputeMaxIndexForceLimit(e640KG, 0, SingleSite, false, 0), 640.0);
        check_d("e800KG -> 800", ComputeMaxIndexForceLimit(e800KG, 0, SingleSite, false, 0), 800.0);
    }

    printf("-- ComputeMaxIndexForceLimit: e85KG default branch (golden :18932-18953) --\n");
    {
        // dDutCount==1, iTestMode in {SingleSite,DualSite,QualSite2X2N}, bD27UseSingleSite85kg=true,
        // ItemIndex==1 -> 55 (golden :18940-18943).
        check_d("e85KG SingleSite  DutCount=1 Item=1 -> 55",
                ComputeMaxIndexForceLimit(e85KG, 1, SingleSite, true, 1), 55.0);
        check_d("e85KG DualSite    DutCount=1 Item=1 -> 55",
                ComputeMaxIndexForceLimit(e85KG, 1, DualSite, true, 1), 55.0);
        check_d("e85KG QualSite2X2N DutCount=1 Item=1 -> 55",
                ComputeMaxIndexForceLimit(e85KG, 1, QualSite2X2N, true, 1), 55.0);

        // ItemIndex==2 -> 85 (golden :18944-18947).
        check_d("e85KG SingleSite  DutCount=1 Item=2 -> 85",
                ComputeMaxIndexForceLimit(e85KG, 1, SingleSite, true, 2), 85.0);
        check_d("e85KG DualSite    DutCount=1 Item=2 -> 85",
                ComputeMaxIndexForceLimit(e85KG, 1, DualSite, true, 2), 85.0);
        check_d("e85KG QualSite2X2N DutCount=1 Item=2 -> 85",
                ComputeMaxIndexForceLimit(e85KG, 1, QualSite2X2N, true, 2), 85.0);

        // FAITHFUL GOLDEN GAP: ItemIndex==0 (or any value other than 1/2) inside the inner
        // if/else-if with NO trailing else -- dMaxLimit stays at its LOCAL initializer 0.0, NOT 85
        // (golden :18940-18948 has no else at all after the ItemIndex==2 case).
        check_d("e85KG SingleSite DutCount=1 Item=0 -> 0.0 (faithful gap, NOT 85)",
                ComputeMaxIndexForceLimit(e85KG, 1, SingleSite, true, 0), 0.0);
        check_d("e85KG SingleSite DutCount=1 Item=3 -> 0.0 (faithful gap, NOT 85)",
                ComputeMaxIndexForceLimit(e85KG, 1, SingleSite, true, 3), 0.0);
        check_d("e85KG SingleSite DutCount=1 Item=-1 -> 0.0 (faithful gap, NOT 85)",
                ComputeMaxIndexForceLimit(e85KG, 1, SingleSite, true, -1), 0.0);

        // dDutCount==2 (not 1) -> falls to the outer else -> 85, regardless of ItemIndex
        // (golden :18934 guard fails, so :18949-18951 fires).
        check_d("e85KG SingleSite DutCount=2 Item=1 -> 85 (DutCount guard)",
                ComputeMaxIndexForceLimit(e85KG, 2, SingleSite, true, 1), 85.0);
        check_d("e85KG SingleSite DutCount=2 Item=2 -> 85 (DutCount guard)",
                ComputeMaxIndexForceLimit(e85KG, 2, SingleSite, true, 2), 85.0);

        // iTestMode NOT in {SingleSite,DualSite,QualSite2X2N} (e.g. TriSite1X3) -> outer else -> 85
        // (golden :18935-18937 guard fails).
        check_d("e85KG TriSite1X3 DutCount=1 Item=1 -> 85 (iTestMode guard)",
                ComputeMaxIndexForceLimit(e85KG, 1, TriSite1X3, true, 1), 85.0);

        // bD27UseSingleSite85kg==false -> outer else -> 85 (golden :18938 guard fails).
        check_d("e85KG SingleSite DutCount=1 Item=1 bD27=false -> 85",
                ComputeMaxIndexForceLimit(e85KG, 1, SingleSite, false, 1), 85.0);
    }

    // =========================================================================================
    // ComputeMinForce -- BCB6 cContact.cpp:18957-19022
    // =========================================================================================
    printf("-- ComputeMinForce: default diameter ladder (golden :18959-19003) --\n");
    {
        // (dKitDiameter, dTagOffset, dTagOffsetNS, bNSKitPress, bD04MinForceByFile, file*5)
        // bD04MinForceByFile=false everywhere in this block -> pure ladder defaults.
        check_d("dia=20 -> 0.5",
                ComputeMinForce(20, 0, 0, false, false, 0, 0, 0, 0, 0, 0), 0.5);
        check_d("dia=28 -> 1.5 (KYEC alias of 30mm bucket)",
                ComputeMinForce(28, 0, 0, false, false, 0, 0, 0, 0, 0, 0), 1.5);
        check_d("dia=30 -> 1.5",
                ComputeMinForce(30, 0, 0, false, false, 0, 0, 0, 0, 0, 0), 1.5);
        check_d("dia=40 -> 4.0",
                ComputeMinForce(40, 0, 0, false, false, 0, 0, 0, 0, 0, 0), 4.0);
        check_d("dia=58 -> 8.0 (KYEC alias of 60mm bucket)",
                ComputeMinForce(58, 0, 0, false, false, 0, 0, 0, 0, 0, 0), 8.0);
        check_d("dia=60 -> 8.0",
                ComputeMinForce(60, 0, 0, false, false, 0, 0, 0, 0, 0, 0), 8.0);
        check_d("dia=80 -> 15.0",
                ComputeMinForce(80, 0, 0, false, false, 0, 0, 0, 0, 0, 0), 15.0);
    }

    printf("-- ComputeMinForce: bD04MinForceByFile override (\"only if larger\") per bucket --\n");
    {
        // dia=20: ladder default 0.5.  Smaller file value (0.1) must NOT override; larger (2.0) must.
        check_d("dia=20 file=0.1 (smaller) -> stays 0.5",
                ComputeMinForce(20, 0, 0, false, true, 0, 0.1, 0, 0, 0, 0), 0.5);
        check_d("dia=20 file=2.0 (larger) -> overrides to 2.0",
                ComputeMinForce(20, 0, 0, false, true, 0, 2.0, 0, 0, 0, 0), 2.0);

        // dia=30 (and its 28 alias): ladder default 1.5.
        check_d("dia=30 file=1.0 (smaller) -> stays 1.5",
                ComputeMinForce(30, 0, 0, false, true, 0, 0, 1.0, 0, 0, 0), 1.5);
        check_d("dia=30 file=3.0 (larger) -> overrides to 3.0",
                ComputeMinForce(30, 0, 0, false, true, 0, 0, 3.0, 0, 0, 0), 3.0);
        check_d("dia=28 file=1.0 (smaller) -> stays 1.5 (30mm field alias)",
                ComputeMinForce(28, 0, 0, false, true, 0, 0, 1.0, 0, 0, 0), 1.5);
        check_d("dia=28 file=3.0 (larger) -> overrides to 3.0 (30mm field alias)",
                ComputeMinForce(28, 0, 0, false, true, 0, 0, 3.0, 0, 0, 0), 3.0);

        // dia=40: ladder default 4.0.
        check_d("dia=40 file=2.0 (smaller) -> stays 4.0",
                ComputeMinForce(40, 0, 0, false, true, 0, 0, 0, 2.0, 0, 0), 4.0);
        check_d("dia=40 file=6.0 (larger) -> overrides to 6.0",
                ComputeMinForce(40, 0, 0, false, true, 0, 0, 0, 6.0, 0, 0), 6.0);

        // dia=60 (and its 58 alias): ladder default 8.0.
        check_d("dia=60 file=5.0 (smaller) -> stays 8.0",
                ComputeMinForce(60, 0, 0, false, true, 0, 0, 0, 0, 5.0, 0), 8.0);
        check_d("dia=60 file=10.0 (larger) -> overrides to 10.0",
                ComputeMinForce(60, 0, 0, false, true, 0, 0, 0, 0, 10.0, 0), 10.0);
        check_d("dia=58 file=5.0 (smaller) -> stays 8.0 (60mm field alias)",
                ComputeMinForce(58, 0, 0, false, true, 0, 0, 0, 0, 5.0, 0), 8.0);
        check_d("dia=58 file=10.0 (larger) -> overrides to 10.0 (60mm field alias)",
                ComputeMinForce(58, 0, 0, false, true, 0, 0, 0, 0, 10.0, 0), 10.0);

        // dia=80: ladder default 15.0.
        check_d("dia=80 file=10.0 (smaller) -> stays 15.0",
                ComputeMinForce(80, 0, 0, false, true, 0, 0, 0, 0, 0, 10.0), 15.0);
        check_d("dia=80 file=20.0 (larger) -> overrides to 20.0",
                ComputeMinForce(80, 0, 0, false, true, 0, 0, 0, 0, 0, 20.0), 20.0);
    }

    printf("-- ComputeMinForce: else-branch (unmapped diameter) SLK contact-offset selection --\n");
    {
        // dKitDiameter=100 hits none of the fixed buckets -> golden :19004-19020 else branch.
        // bNSKitPress=false -> dContactOffset; bNSKitPress=true -> dContactOffset_NS.
        check_d("dia=100 bNSKitPress=false -> dTagContactOffset",
                ComputeMinForce(100, 6.5, 9.5, false, false, 0, 0, 0, 0, 0, 0), 6.5);
        check_d("dia=100 bNSKitPress=true -> dTagContactOffsetNS",
                ComputeMinForce(100, 6.5, 9.5, true, false, 0, 0, 0, 0, 0, 0), 9.5);

        // else-branch's own bD04MinForceByFile override uses the PLAIN dD04MinForceByFile field
        // (not one of the _NNmm variants) -- golden :19015-19019.
        check_d("dia=100 offset=6.5 file=3.0 (smaller) -> stays 6.5",
                ComputeMinForce(100, 6.5, 9.5, false, true, 3.0, 0, 0, 0, 0, 0), 6.5);
        check_d("dia=100 offset=6.5 file=9.0 (larger) -> overrides to 9.0",
                ComputeMinForce(100, 6.5, 9.5, false, true, 9.0, 0, 0, 0, 0, 0), 9.0);
    }

    // -----------------------------------------------------------------------
    // Summary
    // -----------------------------------------------------------------------
    printf("\n=== SUMMARY: %d passed, %d failed (total %d) ===\n",
           g_pass, g_fail, g_pass + g_fail);
    if (g_fail != 0)
    {
        printf("RESULT: FAIL\n");
        return 1;
    }
    printf("RESULT: ALL PASS\n");
    return 0;
}
