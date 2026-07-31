// ===========================================================================
//  tests/test_w7_f2_sckart_state.cpp
//  W7-F2 -- SckArt state: merge proof + golden-divergence characterization.
//
//  Two jobs, deliberately in one target because they are two halves of the same
//  analysis (docs/W7_UI_ARCHITECTURE_PLAN.md SS3-C4 / SS6-F2 / SS9-R9):
//
//  PART A -- MERGE PROOF.  W7-F2 made SckArtRemainderState derive from
//    SckArtState instead of re-declaring the 9 golden TfSCKART fields the two
//    had in common.  The merge is only behaviour-neutral if the base ctor
//    produces exactly the values the derived ctor used to produce, so every one
//    of those 9 is asserted here on BOTH types.  If a later wave changes either
//    ctor, this fails.
//
//  PART B -- GOLDEN-DIVERGENCE CHARACTERIZATION.  Diffing the ported SckArt
//    copies field by field against golden turned up two places where the ported
//    tree does NOT match golden.  Plan SS6-F2 says such a find is REPORTED, not
//    silently reconciled, so W7-F2 changed no values -- it pinned the current
//    (diverging) values here instead.  Each assertion below states the golden
//    value in its message.  THESE ARE NOT "CORRECT" EXPECTATIONS: a future wave
//    that fixes the divergence MUST flip the constant here in the same commit.
//    Failing to notice is exactly what this file exists to prevent.
//
//    D1  iLOTSTATUS_* status codes.  Golden's TfSCKART ctor assigns the whole
//        family explicitly -- golden Automation/SCK_ART.cpp:43-49:
//            iLOTSTATUS_NONE=0  _W=1  _T=2  _L=3  _R=4  _F=5  _A=6
//        csystem.cpp's two seams zero-initialise the four they carry
//        (W7C1's iLOTSTATUS_L, W7C2's iLOTSTATUS_W/_R/_A).
//
//    D2  iTesterType.  Golden Automation/SCK_ART.cpp:42 sets 0, then :111-114
//        `else { iTesterType=1; }` -- so golden constructs 1 for every
//        CUSTOMER_CODE except CC_SCK.  All ported copies hardcode 0.
//
//  PART C -- THE SEAM CONSTANTS THEMSELVES (AI(W906-W7-F2fix) 20260729, added by
//    the review track).  PART B alone was NOT a pin on D1/D2, even though this
//    file's own header and two csystem.cpp comment blocks said it was.  PART B
//    builds its own local SckArtState and feeds SckArt_SetLotStatus a literal 0
//    or 3 -- a true statement about that function, but one that stays green no
//    matter what csystem.cpp's seams construct.  The SIX initialisers that
//    actually carry D1 and D2 had NO observer at all, so a later wave could have
//    "fixed" or further broken a real behavioural divergence with the whole
//    suite green.  PART C closes that: it reads csystem.cpp's own source text
//    and asserts each of the six.  See PART C's own block for why a text-level
//    guard is the only kind available here, and for the proof that it can fail.
//
//  ALSO PINNED: SckArt_SetLotStatus's full 0..7 mapping, which IS a faithful
//  translation of golden :639-667 and is the thing D1 corrupts by feeding it the
//  wrong input.  Keeping both in one file makes the interaction visible.
//
//  Toolchain: MinGW g++, C++14. Non-zero exit on any failure. No window, no
//  globals mutated beyond the local state objects.  PART C reads ONE file
//  (csystem.cpp) read-only; nothing is written anywhere.
// ===========================================================================
#include "Automation/SCK_ART.h"
#include "Automation/SCK_ART_Remainder.h"

#include <cstdio>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>

static int g_fail  = 0;
static int g_total = 0;

static void check(bool ok, const char* what, const char* file, int line)
{
    ++g_total;
    if (!ok) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n", file, line, what);
    }
}
#define CHECK(cond, what) check((cond), (what), __FILE__, __LINE__)

// ===========================================================================
//  PART C support -- read csystem.cpp and parse a ctor initialiser out of it.
//  AI(W906-W7-F2fix) 20260729.
// ===========================================================================

//  Locate the ported tree root from THIS file's own absolute path.  Verified,
//  not assumed: with the "MinGW Makefiles" generator CMake invokes g++ with the
//  absolute source path (checked in the generated
//  tests/CMakeFiles/test_w7_f2_sckart_state.dir/build.make of a fresh
//  configure), so __FILE__ is `<root>\tests\test_w7_f2_sckart_state.cpp`.
//  Deriving the path this way instead of from a target_compile_definitions
//  keeps the fix inside this file -- tests/CMakeLists.txt is owned by another
//  track this round and must not be touched.
static std::string w7f2_portedRoot()
{
    std::string self(__FILE__);
    std::string::size_type a = self.find_last_of("/\\");          // strip filename
    if (a == std::string::npos) return std::string();
    std::string dir = self.substr(0, a);
    std::string::size_type b = dir.find_last_of("/\\");           // strip "tests"
    if (b == std::string::npos) return std::string();
    return dir.substr(0, b);
}

static bool w7f2_readFile(const std::string &path, std::string &out)
{
    std::ifstream f(path.c_str(), std::ios::in | std::ios::binary);
    if (!f) return false;
    std::ostringstream ss;
    ss << f.rdbuf();
    out = ss.str();
    return !out.empty();
}

//  Strip `// ... end-of-line` comments so a future explanatory comment placed
//  INSIDE an initialiser list cannot be mistaken for the initialiser itself.
static std::string w7f2_stripLineComments(const std::string &s)
{
    std::string r;
    r.reserve(s.size());
    bool inComment = false;
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        if (!inComment && s[i] == '/' && i + 1 < s.size() && s[i + 1] == '/') { inComment = true; ++i; continue; }
        if (inComment) { if (s[i] == '\n') { inComment = false; r += '\n'; } continue; }
        r += s[i];
    }
    return r;
}

//  Return the mem-initialiser value for `field` inside `ctorName`'s init-list.
//  Anchored BY SYMBOL NAME, never by line number (plan SS5 / KNOWLEDGE Gotcha 10:
//  ported line numbers rot immediately).  Fails loudly -- and therefore turns the
//  test red -- if the ctor, the init-list, the field, or a plain integer literal
//  cannot be found, so PART C can never silently pass.
static bool w7f2_ctorInitValue(const std::string &src, const std::string &ctorName,
                               const std::string &field, long &out, std::string &why)
{
    const std::string needle = ctorName + "():";
    std::string::size_type c = src.find(needle);
    if (c == std::string::npos) {
        why = "ctor init-list `" + needle + "` not found in csystem.cpp";
        return false;
    }
    std::string::size_type open = src.find('{', c);
    if (open == std::string::npos) {
        why = "no ctor body `{` after `" + needle + "`";
        return false;
    }
    const std::string region = w7f2_stripLineComments(src.substr(c, open - c));

    std::string::size_type f = region.find(field);
    while (f != std::string::npos) {
        const bool leftOk = (f == 0) ||
            !(std::isalnum(static_cast<unsigned char>(region[f - 1])) || region[f - 1] == '_');
        std::string::size_type p = f + field.size();
        while (p < region.size() && (region[p] == ' ' || region[p] == '\t' || region[p] == '\n' || region[p] == '\r')) ++p;
        if (leftOk && p < region.size() && region[p] == '(') {
            ++p;
            while (p < region.size() && (region[p] == ' ' || region[p] == '\t')) ++p;
            bool neg = false;
            if (p < region.size() && (region[p] == '-' || region[p] == '+')) { neg = (region[p] == '-'); ++p; }
            if (p >= region.size() || !std::isdigit(static_cast<unsigned char>(region[p]))) {
                why = ctorName + "'s `" + field + "(...)` initialiser is not a plain integer literal";
                return false;
            }
            long v = 0;
            while (p < region.size() && std::isdigit(static_cast<unsigned char>(region[p])))
                v = v * 10 + (region[p++] - '0');
            out = neg ? -v : v;
            return true;
        }
        f = region.find(field, f + 1);
    }
    why = "`" + field + "` is not in " + ctorName + "'s init-list";
    return false;
}

//  One PART C pin: assert the seam initialiser still holds `expectPorted`.
static void w7f2_pinSeamInit(const std::string &src, const char *ctorName,
                             const char *field, long expectPorted,
                             const char *label, const char *file, int line)
{
    long v = -12345;
    std::string why;
    if (!w7f2_ctorInitValue(src, ctorName, field, v, why)) {
        std::string msg = std::string(label) + "  COULD NOT BE CHECKED: " + why +
                          ".  This pin exists because nothing else observes that initialiser -- "
                          "re-anchor it, do not delete it.";
        check(false, msg.c_str(), file, line);
        return;
    }
    if (v != expectPorted) {
        char buf[512];
        std::snprintf(buf, sizeof(buf),
                      "%s  -- source says %s::%s(%ld), this pin expected %ld",
                      label, ctorName, field, v, expectPorted);
        check(false, buf, file, line);
        return;
    }
    check(true, label, file, line);
}
#define PIN_SEAM(src, ctor, field, expect, label) \
    w7f2_pinSeamInit((src), (ctor), (field), (expect), (label), __FILE__, __LINE__)

int main()
{
    // =====================================================================
    //  PART A -- merge proof: the 9 formerly-duplicated fields construct
    //  identically on the base and on the derived type.
    //  (Field list and golden line numbers: golden Automation/SCK_ART.h:236
    //  sLOTSTATUS, :251 iCurrentStatus, :249 iTesterType, :245 iInputCount,
    //  :248 iFTRTCount, :250 iManualRejectCnt, :285 iNeedRT, :261 dCurrYield,
    //  :263 iCurrent93KARTStep.)
    // =====================================================================
    SckArtState          base;
    SckArtRemainderState rem;

    CHECK(base.sLOTSTATUS         == AnsiString("NONE"), "A1  SckArtState.sLOTSTATUS ctor value is \"NONE\" (golden SCK_ART.cpp:51)");
    CHECK(rem.sLOTSTATUS          == AnsiString("NONE"), "A2  SckArtRemainderState inherits the same \"NONE\" (merge must not change it)");
    CHECK(base.iCurrentStatus     == 0,                  "A3  SckArtState.iCurrentStatus ctor value is 0 (== golden iLOTSTATUS_NONE)");
    CHECK(rem.iCurrentStatus      == 0,                  "A4  SckArtRemainderState.iCurrentStatus matches the base");
    CHECK(base.iInputCount        == 0,                  "A5  SckArtState.iInputCount ctor value is 0");
    CHECK(rem.iInputCount         == 0,                  "A6  SckArtRemainderState.iInputCount matches the base");
    CHECK(base.iFTRTCount         == 0,                  "A7  SckArtState.iFTRTCount ctor value is 0");
    CHECK(rem.iFTRTCount          == 0,                  "A8  SckArtRemainderState.iFTRTCount matches the base");
    CHECK(base.iManualRejectCnt   == 0,                  "A9  SckArtState.iManualRejectCnt ctor value is 0");
    CHECK(rem.iManualRejectCnt    == 0,                  "A10 SckArtRemainderState.iManualRejectCnt matches the base");
    CHECK(base.iNeedRT            == 0,                  "A11 SckArtState.iNeedRT ctor value is 0");
    CHECK(rem.iNeedRT             == 0,                  "A12 SckArtRemainderState.iNeedRT matches the base");
    CHECK(base.dCurrYield         == 0.0,                "A13 SckArtState.dCurrYield ctor value is 0.0");
    CHECK(rem.dCurrYield          == 0.0,                "A14 SckArtRemainderState.dCurrYield matches the base");
    CHECK(base.iCurrent93KARTStep == 0,                  "A15 SckArtState.iCurrent93KARTStep ctor value is 0");
    CHECK(rem.iCurrent93KARTStep  == 0,                  "A16 SckArtRemainderState.iCurrent93KARTStep matches the base");

    // The merge is only useful if the sibling free functions can now be called
    // on a remainder state through the base reference. Prove the binding works
    // and that it writes through to the derived object (no slicing).
    SckArt_SetLotStatus(rem, 4);
    CHECK(rem.sLOTSTATUS    == AnsiString("LOTSTATUS_R"), "A17 SckArt_SetLotStatus() binds a SckArtRemainderState& to SckArtState& and writes through");
    CHECK(rem.iCurrentStatus == 4,                        "A18 ...and the derived object sees iCurrentStatus, i.e. nothing was sliced");

    // Derived-only fields still construct as before the merge.
    SckArtRemainderState rem2;
    CHECK(rem2.iCurrentFlexARTStep == 0,   "A19 derived-only iCurrentFlexARTStep still 0 (golden SCK_ART.h:264)");
    CHECK(rem2.iLotCount           == 0,   "A20 derived-only iLotCount still 0 (golden SCK_ART.h:246)");
    CHECK(rem2.iInputJamCnt        == 0,   "A21 derived-only iInputJamCnt still 0 (golden SCK_ART.cpp:54)");
    CHECK(rem2.iOutputJamCnt       == 0,   "A22 derived-only iOutputJamCnt still 0 (golden SCK_ART.cpp:55)");
    CHECK(rem2.sSetupFilePath      == AnsiString(""), "A23 derived-only sSetupFilePath still \"\" (golden SCK_ART.cpp:40)");
    CHECK(rem2.sLotStartTime       == AnsiString(""), "A24 derived-only sLotStartTime still \"\" (golden SCK_ART.cpp:41)");

    // The 3 base fields the remainder never used are inherited and inert.
    CHECK(rem2.bLdCntExdInputCnt  == false, "A25 inherited-but-unused bLdCntExdInputCnt is false (golden SCK_ART.h:350)");
    CHECK(rem2.bBackUpInArmMode   == false, "A26 inherited-but-unused bBackUpInArmMode is false (golden SCK_ART.h:279)");
    CHECK(rem2.bBackUpOutArmMode  == false, "A27 inherited-but-unused bBackUpOutArmMode is false (golden SCK_ART.h:280)");

    // =====================================================================
    //  PART B(i) -- the faithful half: SckArt_SetLotStatus's mapping.
    //  golden Automation/SCK_ART.cpp:639-667. This is CORRECT and must stay.
    // =====================================================================
    {
        SckArtState st;
        int combo = -99;

        SckArt_SetLotStatus(st, 1, &combo);
        CHECK(st.sLOTSTATUS == AnsiString("LOTSTATUS_W") && st.iCurrentStatus == 1 && combo == 1,
              "B1  SetLotStatus(1) -> \"LOTSTATUS_W\" (golden :643-645)");
        SckArt_SetLotStatus(st, 2, &combo);
        CHECK(st.sLOTSTATUS == AnsiString("LOTSTATUS_T") && st.iCurrentStatus == 2 && combo == 2,
              "B2  SetLotStatus(2) -> \"LOTSTATUS_T\" (golden :646-648)");
        SckArt_SetLotStatus(st, 3, &combo);
        CHECK(st.sLOTSTATUS == AnsiString("LOTSTATUS_L") && st.iCurrentStatus == 3 && combo == 3,
              "B3  SetLotStatus(3) -> \"LOTSTATUS_L\" (golden :649-651)");
        SckArt_SetLotStatus(st, 4, &combo);
        CHECK(st.sLOTSTATUS == AnsiString("LOTSTATUS_R") && st.iCurrentStatus == 4 && combo == 4,
              "B4  SetLotStatus(4) -> \"LOTSTATUS_R\" (golden :652-654)");
        SckArt_SetLotStatus(st, 5, &combo);
        CHECK(st.sLOTSTATUS == AnsiString("LOTSTATUS_F") && st.iCurrentStatus == 5 && combo == 5,
              "B5  SetLotStatus(5) -> \"LOTSTATUS_F\" (golden :655-657)");
        SckArt_SetLotStatus(st, 6, &combo);
        CHECK(st.sLOTSTATUS == AnsiString("LOTSTATUS_A") && st.iCurrentStatus == 6 && combo == 6,
              "B6  SetLotStatus(6) -> \"LOTSTATUS_A\" (golden :658-660)");
        SckArt_SetLotStatus(st, 0, &combo);
        CHECK(st.sLOTSTATUS == AnsiString("NONE") && st.iCurrentStatus == 0 && combo == 0,
              "B7  SetLotStatus(0) -> default: \"NONE\" (golden :661-662); 0 is golden's iLOTSTATUS_NONE");
        CHECK(SckArt_GetLotStatus(st) == AnsiString("NONE"),
              "B8  GetLotStatus() returns sLOTSTATUS verbatim (golden :669-672)");
    }

    // =====================================================================
    //  PART B(ii) -- the OBSERVABLE COST of divergence D1.
    //  csystem.cpp's DoCleanOutFinishCheck calls SetLotStatus(iLOTSTATUS_L).
    //  Golden's iLOTSTATUS_L is 3 (golden SCK_ART.cpp:46); W7C1_TfSCKARTSeam
    //  constructs it 0. Reproduce both feeds and pin what each produces, so the
    //  cost of the divergence is stated as an executable fact, not prose.
    //
    //  AI(W906-W7-F2fix) 20260729: B9/B10 used to be labelled "DIVERGENCE D1
    //  PINNED".  They are NOT a pin on the divergence -- they feed literals to
    //  SckArt_SetLotStatus and would stay green if the seam's iLOTSTATUS_L became
    //  3, or 7, or anything else.  The seam initialiser itself is pinned by PART C
    //  (checks C2/C4/C5/C6).  Labels corrected; the assertions themselves are
    //  sound and unchanged.
    // =====================================================================
    {
        SckArtState asGolden;
        SckArt_SetLotStatus(asGolden, 3);              // golden iLOTSTATUS_L == 3
        CHECK(asGolden.sLOTSTATUS == AnsiString("LOTSTATUS_L") && asGolden.iCurrentStatus == 3,
              "B9  reference: with golden's iLOTSTATUS_L(3) the lot status becomes \"LOTSTATUS_L\"");

        SckArtState asPorted;
        SckArt_SetLotStatus(asPorted, 0);              // W7C1_TfSCKARTSeam's iLOTSTATUS_L == 0
        CHECK(asPorted.sLOTSTATUS == AnsiString("NONE") && asPorted.iCurrentStatus == 0,
              "B10 COST OF DIVERGENCE D1 (not the pin -- see PART C): feeding SckArt_SetLotStatus "
              "the 0 that csystem.cpp's W7C1_TfSCKARTSeam constructs for iLOTSTATUS_L, instead of "
              "golden's 3 (golden SCK_ART.cpp:46), records \"NONE\" instead of \"LOTSTATUS_L\". "
              "W7C2_TfSCKARTSeam has the same defect for iLOTSTATUS_W(golden 1, SCK_ART.cpp:44) / "
              "iLOTSTATUS_R(golden 4, :47) / iLOTSTATUS_A(golden 6, :49). REPORTED not fixed (plan "
              "SS6-F2). This assertion is a property of SckArt_SetLotStatus and stays TRUE even "
              "after the seams are fixed -- it is PART C, not this, that must be updated then.");
    }

    // =====================================================================
    //  PART B(iii) -- DIVERGENCE D2 in the two STATE STRUCTS, pinned.
    //  golden SCK_ART.cpp:42 sets iTesterType=0, then :111-114 sets it to 1 for
    //  every CUSTOMER_CODE except CC_SCK. Both ported state structs hardcode 0.
    //  These two ARE genuine pins: they read the constructed field of the real
    //  types.  The csystem.cpp seams' own iTesterType shadows are separate
    //  storage and are pinned by PART C (C2/C6) instead.
    // =====================================================================
    {
        SckArtState          st;
        SckArtRemainderState rs;
        CHECK(st.iTesterType == 0,
              "B11 DIVERGENCE D2 PINNED (SckArtState): iTesterType constructs 0 = Flex. Golden "
              "constructs 1 = 93K for every CUSTOMER_CODE except CC_SCK (golden SCK_ART.cpp:42 "
              "then :111-114 `else { iTesterType=1; }`). REPORTED not fixed (plan SS6-F2). WHEN A "
              "FUTURE WAVE RE-DERIVES THE CUSTOMER BRANCH, UPDATE THIS ASSERTION.");
        CHECK(rs.iTesterType == 0,
              "B12 DIVERGENCE D2 PINNED (SckArtRemainderState): same field, now inherited from "
              "SckArtState after the W7-F2 merge, so the divergence is single-sourced -- one fix "
              "site instead of two.");
    }

    // =====================================================================
    //  PART C -- THE SIX csystem.cpp SEAM INITIALISERS THAT CARRY D1 AND D2.
    //  AI(W906-W7-F2fix) 20260729 -- review-track HIGH-1.
    //
    //  WHY A SOURCE-TEXT PIN AND NOT A RUNTIME ONE.  `struct W7C1_TfSCKARTSeam`
    //  and `struct W7C2_TfSCKARTSeam`, and their single instances
    //  W7C1_fSCKART_ext / W7C2_fSCKART_ext, are declared `static` at file scope
    //  INSIDE csystem.cpp and reached only through the file-local W7C1_SCKART /
    //  W7C2_SCKART macros.  They have no external linkage and appear in no
    //  header, so no test translation unit can name them -- checked, not assumed.
    //  There is therefore no runtime observation available, and the six
    //  divergent values exist in exactly one observable place: the source text
    //  of the two ctor initialiser lists.  That is what PART C reads.
    //
    //  WHAT THIS IS AND IS NOT.  It is a text-level regression guard on six
    //  specific initialisers, anchored by symbol name.  It is NOT a behavioural
    //  observation, and it does NOT prove the seams behave like golden -- they
    //  demonstrably do not, which is the whole point.  The residual exposure
    //  (nothing offline can exercise the ART branches these constants gate) is
    //  recorded in docs/W7-UI-SKIPPED.md under W7-F2-fix, per plan SS12.6.
    //
    //  WHY IT IS LOAD-BEARING.  Perturb any one of the six initialisers in
    //  csystem.cpp and the matching check below goes red, printing both the value
    //  found in the source and the value expected.  Proven by doing exactly that
    //  during this fix round -- see the W7-F2-fix report.
    //
    //  WHEN A FUTURE WAVE ACTUALLY FIXES THE SEAMS, the honest edit is to change
    //  the expected value here from the ported 0 to the golden constant in the
    //  SAME change, not to delete the check.
    // =====================================================================
    {
        const std::string root = w7f2_portedRoot();
        const std::string path = root + "/csystem.cpp";
        std::string src;
        const bool got = w7f2_readFile(path, src);
        CHECK(got, "C0  csystem.cpp is readable from this test (path derived from __FILE__); "
                   "without it the six pins below cannot run and MUST NOT be treated as passing");

        if (got) {
            // --- D1: the four iLOTSTATUS_* constants ------------------------
            PIN_SEAM(src, "W7C1_TfSCKARTSeam", "iLOTSTATUS_L", 0,
                     "C1  D1 PINNED: W7C1_TfSCKARTSeam ctor constructs iLOTSTATUS_L(0); golden "
                     "Automation/SCK_ART.cpp:46 constructs 3");
            PIN_SEAM(src, "W7C2_TfSCKARTSeam", "iLOTSTATUS_W", 0,
                     "C2  D1 PINNED: W7C2_TfSCKARTSeam ctor constructs iLOTSTATUS_W(0); golden "
                     "Automation/SCK_ART.cpp:44 constructs 1");
            PIN_SEAM(src, "W7C2_TfSCKARTSeam", "iLOTSTATUS_R", 0,
                     "C3  D1 PINNED: W7C2_TfSCKARTSeam ctor constructs iLOTSTATUS_R(0); golden "
                     "Automation/SCK_ART.cpp:47 constructs 4");
            PIN_SEAM(src, "W7C2_TfSCKARTSeam", "iLOTSTATUS_A", 0,
                     "C4  D1 PINNED: W7C2_TfSCKARTSeam ctor constructs iLOTSTATUS_A(0); golden "
                     "Automation/SCK_ART.cpp:49 constructs 6");

            // --- D2: the two seam-local iTesterType shadows -----------------
            //   These are NOT the SckArtState field B11 pins -- each seam owns a
            //   separate top-level `iTesterType`, and it is the seam's copy (not
            //   the embedded `core`'s) that every csystem.cpp call site reads.
            PIN_SEAM(src, "W7C1_TfSCKARTSeam", "iTesterType", 0,
                     "C5  D2 PINNED (behavioural): W7C1_TfSCKARTSeam ctor constructs iTesterType(0); "
                     "golden Automation/SCK_ART.cpp:42 then :113 constructs 1 for every CUSTOMER_CODE "
                     "except CC_SCK. With 0 the `W7C1_SCKART->iTesterType==0` GATE in "
                     "DoCleanOutFinishCheck is TRUE, so the port ENTERS the whole Flex-ART block that "
                     "golden SKIPS -- see Automation/SCK_ART.cpp's SckArtState ctor note");
            PIN_SEAM(src, "W7C2_TfSCKARTSeam", "iTesterType", 0,
                     "C6  D2 PINNED (behavioural): W7C2_TfSCKARTSeam ctor constructs iTesterType(0); "
                     "golden constructs 1 except for CC_SCK, which inverts all five "
                     "`W7C2_SCKART->iTesterType==0/==1` reads in DoART_AfterCleanOut");
        }
    }

    // =====================================================================
    //  Summary
    // =====================================================================
    if (g_fail == 0) {
        std::printf("PASS  %d/%d checks passed\n", g_total, g_total);
        return 0;
    }
    std::printf("FAIL  %d/%d checks failed\n", g_fail, g_total);
    return 1;
}
