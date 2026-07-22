// tests/test_cContact.cpp
// Verification harness for the cContact calc-core translation (ComputeMaxIndexForceLimit /
// ComputeMinForce / ComputeIsRun2DCheck / kgTranToMitsubishikg / ComputeAutoHeightMaxKGTorque /
// ComputeTestZCompensationHight / ComputeIndexDownPos / ComputeIndexYSpeed / ComputeIndexZSpeed).
//
// Exercises the translated public API with input->expected-output values hand-derived from the
// BCB6 formula in the ORIGINAL golden reference
//   HT9011UC_Code_V3.33.906.0_20260618/cContact.cpp:18893-19022 (ComputeMaxIndexForceLimit/ComputeMinForce)
//   HT9011UC_Code_V3.33.906.0_20260618/cContact.cpp:21203-21219 (ComputeIsRun2DCheck)
//   HT9011UC_Code_V3.33.906.0_20260618/cContact.cpp:5261-5382   (kgTranToMitsubishikg/ComputeAutoHeightMaxKGTorque)
//   HT9011UC_Code_V3.33.906.0_20260618/cContact.cpp:18453-18673 (ComputeTestZCompensationHight/
//                                                                 ComputeIndexDownPos/ComputeIndexYSpeed/
//                                                                 ComputeIndexZSpeed)
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

// AI(W906-cContactLeaf) 20260721: check_b/check_i added for the new bool/int-returning leaves
// (ComputeIsRun2DCheck / kgTranToMitsubishikg / ComputeAutoHeightMaxKGTorque / ComputeIndexYSpeed /
// ComputeIndexZSpeed) -- same PASS/FAIL/summary shape as check_d above.
static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected)
    {
        printf("PASS  %-56s got=%-5s exp=%-5s\n", name, got ? "true" : "false", expected ? "true" : "false");
        ++g_pass;
    }
    else
    {
        printf("FAIL  %-56s got=%-5s exp=%-5s\n", name, got ? "true" : "false", expected ? "true" : "false");
        ++g_fail;
    }
}

static void check_i(const char* name, int got, int expected)
{
    if (got == expected)
    {
        printf("PASS  %-56s got=%-6d exp=%-6d\n", name, got, expected);
        ++g_pass;
    }
    else
    {
        printf("FAIL  %-56s got=%-6d exp=%-6d\n", name, got, expected);
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

    // =========================================================================================
    // AI(W906-cContactLeaf) 20260721: W906 cContact leaf-function wave (Step 1 + Step 2).
    // =========================================================================================

    // =========================================================================================
    // ComputeIsRun2DCheck -- BCB6 cContact.cpp:21203-21219
    //   All 6 AND-terms (the 3-way BAR_CODE_INSTALL OR-group counts as one term):
    //   bCheck2DHardware, iBarCodeInstall(in{ebctUseCCDMode,ebctInShtIntel,ebctEtherNetCCD}),
    //   bEnableBarCode, iInArmShtRow==2, iContactMode==CONTACT_NORMAL, bRun2DCheck==true.
    // =========================================================================================
    printf("-- ComputeIsRun2DCheck: all-true baseline + each term flipped false (golden :21206-21214) --\n");
    {
        check_b("all 6 terms true (ebctUseCCDMode) -> true",
                ComputeIsRun2DCheck(true, ebctUseCCDMode, true, 2, CONTACT_NORMAL, true), true);
        check_b("all 6 terms true (ebctInShtIntel) -> true",
                ComputeIsRun2DCheck(true, ebctInShtIntel, true, 2, CONTACT_NORMAL, true), true);
        check_b("all 6 terms true (ebctEtherNetCCD) -> true",
                ComputeIsRun2DCheck(true, ebctEtherNetCCD, true, 2, CONTACT_NORMAL, true), true);

        check_b("bCheck2DHardware=false -> false",
                ComputeIsRun2DCheck(false, ebctUseCCDMode, true, 2, CONTACT_NORMAL, true), false);
        check_b("iBarCodeInstall=ebctUninstall (not in OR-group) -> false",
                ComputeIsRun2DCheck(true, ebctUninstall, true, 2, CONTACT_NORMAL, true), false);
        check_b("bEnableBarCode=false -> false",
                ComputeIsRun2DCheck(true, ebctUseCCDMode, false, 2, CONTACT_NORMAL, true), false);
        check_b("iInArmShtRow=1 (!=2) -> false",
                ComputeIsRun2DCheck(true, ebctUseCCDMode, true, 1, CONTACT_NORMAL, true), false);
        check_b("iContactMode=CONTACT_AUTO_GET_HEIGHT (!=CONTACT_NORMAL) -> false",
                ComputeIsRun2DCheck(true, ebctUseCCDMode, true, 2, 1 /*CONTACT_AUTO_GET_HEIGHT*/, true), false);
        check_b("bRun2DCheck=false -> false",
                ComputeIsRun2DCheck(true, ebctUseCCDMode, true, 2, CONTACT_NORMAL, false), false);

        // Real-tree note (see cContact.h banner): bRun2DCheck currently has no live setter in the
        // translated tree, so under every reachable state TODAY this evaluates to false -- but the
        // FUNCTION ITSELF (tested here in isolation with an explicit true) must still return true
        // when every term is actually true.  All-false baseline, for completeness:
        check_b("all 6 terms false -> false",
                ComputeIsRun2DCheck(false, ebctUninstall, false, 0, 99, false), false);
    }

    // =========================================================================================
    // kgTranToMitsubishikg -- BCB6 cContact.cpp:5261-5271 (pure int clamp, kg/3, floor 15, ceil 30)
    // =========================================================================================
    printf("-- kgTranToMitsubishikg: kg/3 with [15,30] clamp (golden :5261-5271) --\n");
    {
        check_i("kg=0 -> 15 (0/3=0 < 15 floor)", kgTranToMitsubishikg(0), 15);
        check_i("kg=44 -> 15 (44/3=14 < 15 floor)", kgTranToMitsubishikg(44), 15);
        check_i("kg=45 -> 15 (45/3=15, boundary, not <15)", kgTranToMitsubishikg(45), 15);
        check_i("kg=60 -> 20 (60/3=20, mid-range unchanged)", kgTranToMitsubishikg(60), 20);
        check_i("kg=90 -> 30 (90/3=30, boundary, not >30)", kgTranToMitsubishikg(90), 30);
        check_i("kg=93 -> 30 (93/3=31 > 30 ceil)", kgTranToMitsubishikg(93), 30);
    }

    // =========================================================================================
    // ComputeAutoHeightMaxKGTorque -- BCB6 cContact.cpp:5273-5382
    // =========================================================================================
    printf("-- ComputeAutoHeightMaxKGTorque: representative branch coverage (golden :5273-5382) --\n");
    {
        static const int kEmptySiteMap[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};

        // golden :5277-5281 -- CONTACT_MANUAL_GET_HEIGHT && bD10ManualHeightComptibleWithNS -> 80
        check_i("ManualGetHeight + D10 compat -> 80",
                ComputeAutoHeightMaxKGTorque(CONTACT_MANUAL_GET_HEIGHT, true, false, 0,
                                              10.0, e85KG, SingleSite, 0, 0, kEmptySiteMap, 0),
                80);

        // golden :5282-5285 -- bD14_AutoHeightUseSetTorque -> iD14_AutoHeightUseSetTorque
        check_i("D14 use-set-torque -> iD14 value (77)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, true, 77,
                                              10.0, e85KG, SingleSite, 0, 0, kEmptySiteMap, 0),
                77);

        // golden :5286-5301 -- dKitDiameter<2.5 low-contact-force branch
        check_i("dia<2.5 + e85KG -> 22",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              2.0, e85KG, SingleSite, 0, 0, kEmptySiteMap, 0),
                22);
        check_i("dia<2.5 + e240KG + SingleSite -> 10",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              2.0, e240KG, SingleSite, 0, 0, kEmptySiteMap, 0),
                10);
        check_i("dia<2.5 + e240KG + DualSite -> 10",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              2.0, e240KG, DualSite, 0, 0, kEmptySiteMap, 0),
                10);
        check_i("dia<2.5 + e240KG + TriSite1X3 (other) -> 15",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              2.0, e240KG, TriSite1X3, 0, 0, kEmptySiteMap, 0),
                15);

        // golden :5302-5323 -- e240KG/e260KG ladder (dia>=2.5)
        check_i("e240KG DualSite dia=3.0 -> 15 (golden :5309-5310)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              3.0, e240KG, DualSite, 0, 0, kEmptySiteMap, 0),
                15);
        {
            // iOpenSiteCT<=4 via a 1-row/2-col open-site map (2 open sites) -- golden :5304-5307/5311-5312.
            int siteMap2Open[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
            siteMap2Open[0][0] = 1;
            siteMap2Open[0][1] = 1;
            check_i("e240KG _16Site2X8 dia=3.0 openCT=2(<=4) -> 20 (golden :5311-5312)",
                    ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                                  3.0, e240KG, _16Site2X8, 1, 2, siteMap2Open, 0),
                    20);
        }
        {
            // iOpenSiteCT<=1 -- golden :5313-5314.
            int siteMap1Open[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
            siteMap1Open[0][0] = 1;
            check_i("e240KG DualSite dia=4.0 openCT=1(<=1) -> 15 (golden :5313-5314)",
                    ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                                  4.0, e240KG, DualSite, 1, 1, siteMap1Open, 0),
                    15);
        }
        check_i("e240KG SingleSite(<=DualSite) dia=4.0 -> 25 (golden :5315-5316)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              4.0, e240KG, SingleSite, 0, 0, kEmptySiteMap, 0),
                25);
        check_i("e240KG _6Site2X3N(<=_6Site2X3N) dia=3.0 -> 20 (golden :5317-5320)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              3.0, e240KG, _6Site2X3N, 0, 0, kEmptySiteMap, 0),
                20);
        check_i("e260KG _16Site4X4 dia=5.0 (no bucket matches) -> 30 (golden :5321-5322 else)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              5.0, e260KG, _16Site4X4, 0, 0, kEmptySiteMap, 0),
                30);

        // golden :5324-5335 -- e400KG/e360KG: both sub-branches return 12.
        check_i("e400KG SingleSite -> 12 (golden :5326-5330)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              10.0, e400KG, SingleSite, 0, 0, kEmptySiteMap, 0),
                12);
        check_i("e360KG _8Site2X4 (>_6Site2X3N) -> 12 (golden :5331-5334, still 12)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              10.0, e360KG, _8Site2X4, 0, 0, kEmptySiteMap, 0),
                12);

        // golden :5336-5348 -- e500KG
        check_i("e500KG SingleSite(<=_6Site2X3N) -> 12",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              10.0, e500KG, SingleSite, 0, 0, kEmptySiteMap, 0),
                12);
        check_i("e500KG _8Site2X4 dia=5.0(<8.0, >=SingleSite) -> 12",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              5.0, e500KG, _8Site2X4, 0, 0, kEmptySiteMap, 0),
                12);
        check_i("e500KG _8Site2X4 dia=10.0(>=8.0) -> 20 (else)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              10.0, e500KG, _8Site2X4, 0, 0, kEmptySiteMap, 0),
                20);

        // golden :5349-5360 -- e640KG
        check_i("e640KG QualSite2X2N(<=QualSite2X2N) -> 12",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              10.0, e640KG, QualSite2X2N, 0, 0, kEmptySiteMap, 0),
                12);
        check_i("e640KG _8Site2X4(>QualSite2X2N) -> 20 (else)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              10.0, e640KG, _8Site2X4, 0, 0, kEmptySiteMap, 0),
                20);

        // golden :5361-5372 -- e800KG
        check_i("e800KG QualSite2X2N(<=QualSite2X2N) -> 12",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              10.0, e800KG, QualSite2X2N, 0, 0, kEmptySiteMap, 0),
                12);
        check_i("e800KG _8Site2X4(>QualSite2X2N) -> 20 (else)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              10.0, e800KG, _8Site2X4, 0, 0, kEmptySiteMap, 0),
                20);

        // golden :5373-5379 -- final else (indexPressType not any named bucket, e.g. e120KG/e160KG)
        check_i("e120KG DualSite dia=3.0 -> 20 (golden :5375-5376)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              3.0, e120KG, DualSite, 0, 0, kEmptySiteMap, 0),
                20);
        check_i("e120KG DualSite dia=4.0 -> 30 (golden :5377-5378)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              4.0, e120KG, DualSite, 0, 0, kEmptySiteMap, 0),
                30);
        check_i("e120KG SingleSite dia=10.0 -> 40 (kg stays at its initializer, no branch fires)",
                ComputeAutoHeightMaxKGTorque(CONTACT_NORMAL, false, false, 0,
                                              10.0, e120KG, SingleSite, 0, 0, kEmptySiteMap, 0),
                40);
    }

    // =========================================================================================
    // ComputeTestZCompensationHight -- BCB6 cContact.cpp:18453-18540
    // =========================================================================================
    printf("-- ComputeTestZCompensationHight: diameter gate + interpolation (golden :18453-18540) --\n");
    {
        // golden quirk: dKitDiameter NOT 8/40.2 -> function body is a complete no-op; the out
        // params must be left EXACTLY as the caller passed them in (sentinel values here).
        double offset1 = 111.0, offset2 = 222.0;
        static const double kZeroTable[3][15] = {{0}};
        ComputeTestZCompensationHight(12.0, 500.0, kZeroTable, offset1, offset2);
        check_d("dia=12.0 (not 8/40.2) -> offset1 untouched (111.0)", offset1, 111.0);
        check_d("dia=12.0 (not 8/40.2) -> offset2 untouched (222.0)", offset2, 222.0);

        // dia==8, dPress==120 (boundary, NOT >120) -> both offsets zeroed, nothing else runs.
        offset1 = -5.0; offset2 = -7.0;
        ComputeTestZCompensationHight(8.0, 120.0, kZeroTable, offset1, offset2);
        check_d("dia=8.0 dPress=120 (boundary) -> offset1=0", offset1, 0.0);
        check_d("dia=8.0 dPress=120 (boundary) -> offset2=0", offset2, 0.0);

        // dia==40.2, dPress=250 over a hand-built linear table -> bucket iLowLevel=1/iHightLevel=2
        // (row2 = {100,200,...,1400}; 250 > row2[1]=200, not > row2[2]=300).
        double table[3][15] = {{0}};
        for (int k = 0; k < 14; ++k)
            table[2][k] = 100.0 * (k + 1);   // row2: 100,200,300,...,1400
        table[0][1] = 10.0; table[0][2] = 16.0;   // row0 bucket 1/2
        table[1][1] = 20.0; table[1][2] = 32.0;   // row1 bucket 1/2
        offset1 = 0.0; offset2 = 0.0;
        ComputeTestZCompensationHight(40.2, 250.0, table, offset1, offset2);
        // iTotalOffset_1 = (10 + ((16-10)/60)*(250-200)) * 100 = (10 + 5)*100 = 1500
        // iTotalOffset_2 = (20 + ((32-20)/60)*(250-200)) * 100 = (20 + 10)*100 = 3000
        check_d("dia=40.2 dPress=250 -> offset1=1500 (interpolated)", offset1, 1500.0);
        check_d("dia=40.2 dPress=250 -> offset2=3000 (interpolated)", offset2, 3000.0);
    }

    // =========================================================================================
    // ComputeIndexDownPos -- BCB6 cContact.cpp:18628-18661
    // =========================================================================================
    printf("-- ComputeIndexDownPos: branch order + adjustments (golden :18628-18661) --\n");
    {
        check_d("bUserDefMaxContactHeight=true -> dUserDefMaxContactHeight (-99.0)",
                ComputeIndexDownPos(true, -99.0, Type_HT9046_LS, CC_QUALCOMM, eMixATC60_ATC33, eATCUninstall),
                -99.0);
        check_d("Type_HT9046_LS, no Qualcomm-mix -> fIndexDownPos_for9046LS (-148.0)",
                ComputeIndexDownPos(false, 0.0, Type_HT9046_LS, 0, 0, eATCUninstall),
                -148.0);
        check_d("Type_HT9046_LS + CC_QUALCOMM + eMixATC60_ATC33 -> -148-20=-168.0",
                ComputeIndexDownPos(false, 0.0, Type_HT9046_LS, CC_QUALCOMM, eMixATC60_ATC33, eATCUninstall),
                -168.0);
        check_d("ATC_SYSTEM!=eATCUninstall (not HT9046LS) -> fIndexDownPos_forATC (-146.0)",
                ComputeIndexDownPos(false, 0.0, Type_HT9045, 0, 0, eATC60),
                -146.0);
        check_d("else + CC_KYEC_LEE -> fIndexDownPos_for9045-5 = -140.0",
                ComputeIndexDownPos(false, 0.0, Type_HT9045, CC_KYEC_LEE, 0, eATCUninstall),
                -140.0);
        check_d("else + other customer code -> fIndexDownPos_for9045 = -135.0",
                ComputeIndexDownPos(false, 0.0, Type_HT9045, 0, 0, eATCUninstall),
                -135.0);
    }

    // =========================================================================================
    // ComputeIndexYSpeed / ComputeIndexZSpeed -- BCB6 cContact.cpp:18663-18673
    // =========================================================================================
    printf("-- ComputeIndexYSpeed/ComputeIndexZSpeed: CONTACT_DEVICE_MAP_CHECK selector (golden :18663-18673) --\n");
    {
        check_i("Y: iContactMode==CONTACT_DEVICE_MAP_CHECK -> MOT GailSpeed (500)",
                ComputeIndexYSpeed(30, CONTACT_DEVICE_MAP_CHECK, 500), 500);
        check_i("Y: iContactMode!=CONTACT_DEVICE_MAP_CHECK -> _fixValue (30)",
                ComputeIndexYSpeed(30, CONTACT_NORMAL, 500), 30);
        check_i("Z: iContactMode==CONTACT_DEVICE_MAP_CHECK -> MOT GailSpeed (600)",
                ComputeIndexZSpeed(40, CONTACT_DEVICE_MAP_CHECK, 600), 600);
        check_i("Z: iContactMode!=CONTACT_DEVICE_MAP_CHECK -> _fixValue (40)",
                ComputeIndexZSpeed(40, CONTACT_NORMAL, 600), 40);
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
