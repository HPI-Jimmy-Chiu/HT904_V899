// =============================================================================
//  tests/test_ga1_language.cpp -- GA-1-B5 standalone verification harness
//
//  Verifies language.cpp's TfLan CSV load/query engine (LoadLanguageFromFile /
//  GotLanString / UpdateCompomentToStrList / IsMainFormNotChange) against the
//  REAL machine's D:\HT9045\system\Language.csv (190 data rows, all under
//  "fMain"), plus the bUseLanguageCSV==false fallback path.
//
//  NOT wired into tests/CMakeLists.txt -- out of this wave's write boundary
//  (only language.cpp/language.h/this file/the report are authorized writes).
//  See _ga1_b5_report.md for the CMake target fragment a future integration
//  wave should add (mirrors the existing test_IniFiles/test_HTMD5 shape).
//
//  This file is SELF-CONTAINED: it supplies its own minimal definitions for
//  the 3 extern globals/functions language.cpp references that this ad hoc
//  build does not link (LastSet, MyDBIProcess, LogSoftwareOffTime) -- their
//  real homes (LastSet.cpp, aHotPlateSubstrate.cpp, acarry_shims.cpp) all
//  live in the "ht9045_sm" god-stack library, which is far heavier than this
//  standalone check needs. Same "isolated verification build provides its own
//  stub symbols" precedent as other single-purpose ad hoc builds in this repo
//  history (e.g. the W7-L2 ckernel.cpp linkability wave). If this test is
//  ever wired into the real CMake build alongside ht9045_sm, THESE 3
//  definitions must be deleted first (ODR clash with the real ones).
// =============================================================================
#include "language.h"
#include "LastSet.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>

// ---- stand-in definitions for symbols this ad hoc build does not link -----
// AI(W906-GA1-B5-integrate) 20260804: the 3 standalone stand-ins (LastSet /
// MyDBIProcess / LogSoftwareOffTime) were removed at integrate time -- this
// target now links the ht9045_sm RESCAN group, which carries the real
// definitions (LastSet.cpp / uHGemEquipment.cpp / cmydef.cpp).

// -----------------------------------------------------------------------------
static int g_fail = 0;
#define CHECK(cond, msg) do { \
        if (!(cond)) { std::printf("FAIL: %s (%s:%d)\n", msg, __FILE__, __LINE__); ++g_fail; } \
        else { std::printf("ok:   %s\n", msg); } \
    } while (0)

int main()
{
    // -------------------------------------------------------------------
    // Part 1 -- real-CSV oracle. golden's ctor hardcodes
    // FullFileName="D:\\HT9045\\system\\Language.csv" (language.cpp, ctor);
    // that exact path exists on this machine (191 lines: 1 header-shaped row
    // + 190 "fMain,<Name>,<EString>,<CString>,<EHint>,<CHint>" data rows,
    // verified by direct decode this wave). No synthetic CSV is needed for
    // this half of the test.
    // -------------------------------------------------------------------
    {
        TfLan lan;
        CHECK(lan.bUseLanguageCSV == true, "ctor: bUseLanguageCSV true (real Language.csv found via FileExists)");

        lan.LoadLanguageFromFile();
        lan.LanguageFormName = "fMain";

        // 3 known keys (verified present in the real CSV this wave: rows
        // "fMain,tsMain,Main,Main,,", "fMain,sbCloseProgram,Exit,<CH>,,",
        // "fMain,sbConfig,Config,<CH>,,").
        const char* knownNames[] = { "tsMain", "sbCloseProgram", "sbConfig" };
        const char* expectedEnglish[] = { "Main", "Exit", "Config" };

        for (int i = 0; i < 3; ++i)
        {
            AnsiString cap, hint;
            LastSet.iLanguageCountry = 0;   // English column (EString/EHint)
            bool ok = lan.GotLanString(AnsiString(knownNames[i]), &cap, &hint);
            char msg[160];
            std::snprintf(msg, sizeof(msg), "GotLanString(fMain_%s) English: found + non-empty", knownNames[i]);
            CHECK(ok && cap.Length() > 0, msg);
            std::snprintf(msg, sizeof(msg), "GotLanString(fMain_%s) English text matches CSV EString column", knownNames[i]);
            CHECK(cap == expectedEnglish[i], msg);

            AnsiString capCn, hintCn;
            LastSet.iLanguageCountry = 1;   // Chinese column (CString/CHint)
            bool okCn = lan.GotLanString(AnsiString(knownNames[i]), &capCn, &hintCn);
            std::snprintf(msg, sizeof(msg), "GotLanString(fMain_%s) Chinese: found + non-empty", knownNames[i]);
            CHECK(okCn && capCn.Length() > 0, msg);
        }

        // Missing key -> false (mapLanList lookup misses).
        {
            AnsiString cap, hint;
            LastSet.iLanguageCountry = 0;
            bool ok = lan.GotLanString(AnsiString("ThisControlDoesNotExist_GA1B5"), &cap, &hint);
            CHECK(ok == false, "GotLanString: unknown key -> false");
        }

        // Empty Name -> false (golden's own early-out: Name=="").
        {
            AnsiString cap, hint;
            bool ok = lan.GotLanString(AnsiString(""), &cap, &hint);
            CHECK(ok == false, "GotLanString: empty Name -> false (golden early-out)");
        }

        // LanguageFormName=="" -> false, even for a Name that IS a valid
        // suffix under fMain (golden's own early-out, checked before the
        // form/name key is even built).
        {
            TfLan lan2;
            lan2.LoadLanguageFromFile();
            AnsiString cap, hint;
            bool ok = lan2.GotLanString(AnsiString("tsMain"), &cap, &hint);
            CHECK(ok == false, "GotLanString: LanguageFormName empty -> false (golden early-out)");
        }
    }

    // -------------------------------------------------------------------
    // Part 2 -- bUseLanguageCSV==false fallback path (golden's else-branch,
    // language.cpp LoadLanguageFromFile). Force the flag directly (rather
    // than relying on a missing file) so the test is deterministic
    // regardless of what exists on the running machine.
    //
    // OBSERVATION (golden quirk, not a bug -- documented in _ga1_b5_report.md
    // too): the else-branch DOES build a usable mapLanList["From_Name"]=0
    // entry (with a matching LanList[0] placeholder row), but GotLanString
    // can never reach it: GotLanString's FIRST check is
    // `bUseLanguageCSV==false -> return false` unconditionally, before the
    // map is even consulted. So with no CSV, EVERY GotLanString call returns
    // false, regardless of key -- verified below.
    // -------------------------------------------------------------------
    {
        TfLan lan;
        lan.bUseLanguageCSV = false;
        lan.LoadLanguageFromFile();     // builds the placeholder "From_Name"=0 row (else-branch)

        lan.LanguageFormName = "From";
        AnsiString cap, hint;
        bool ok = lan.GotLanString(AnsiString("Name"), &cap, &hint);
        CHECK(ok == false, "bUseLanguageCSV=false: GotLanString always false, even for the placeholder's own From_Name key");

        lan.LanguageFormName = "fMain";
        bool ok2 = lan.GotLanString(AnsiString("tsMain"), &cap, &hint);
        CHECK(ok2 == false, "bUseLanguageCSV=false: GotLanString false for an otherwise-known key too");
    }

    // -------------------------------------------------------------------
    // Part 3 -- UpdateCompomentToStrList / IsMainFormNotChange, against a
    // small self-built synthetic list (this part does not touch any file).
    // -------------------------------------------------------------------
    {
        TfLan lan;
        lan.LoadLanguageFromFile();
        lan.LanguageFormName = "fTestForm";

        // Excluded name (IsMainFormNotChange's own hardcoded list) -> UpdateCompomentToStrList
        // must reject it even with a real Caption.
        bool rejectedExcluded = lan.UpdateCompomentToStrList(AnsiString("Panel17"), AnsiString("whatever"));
        CHECK(rejectedExcluded == false, "UpdateCompomentToStrList: excluded name (IsMainFormNotChange) -> false");

        // Empty Name/Caption -> false.
        bool rejectedEmptyName = lan.UpdateCompomentToStrList(AnsiString(""), AnsiString("Cap"));
        CHECK(rejectedEmptyName == false, "UpdateCompomentToStrList: empty Name -> false");
        bool rejectedEmptyCap = lan.UpdateCompomentToStrList(AnsiString("SomeCtrl"), AnsiString(""));
        CHECK(rejectedEmptyCap == false, "UpdateCompomentToStrList: empty Caption -> false");

        // A genuinely new, non-excluded name -> true, and (English branch)
        // immediately queryable back via GotLanString (UpdateCompomentToStrList
        // seeds English==Chinese by design, "預設中英文相同").
        bool added = lan.UpdateCompomentToStrList(AnsiString("btnGA1B5Probe"), AnsiString("GA1B5 Probe Caption"));
        CHECK(added == true, "UpdateCompomentToStrList: new non-excluded name -> true");

        AnsiString cap, hint;
        LastSet.iLanguageCountry = 0;
        bool found = lan.GotLanString(AnsiString("btnGA1B5Probe"), &cap, &hint);
        CHECK(found && cap == AnsiString("GA1B5 Probe Caption"), "GotLanString: freshly-added row is immediately queryable (English==Chinese seed)");

        // IsMainFormNotChange directly: known excluded vs known non-excluded.
        CHECK(lan.IsMainFormNotChange(AnsiString("labScanAOI")) == true, "IsMainFormNotChange: last entry of golden's exclusion list -> true");
        CHECK(lan.IsMainFormNotChange(AnsiString("NotInTheExclusionList")) == false, "IsMainFormNotChange: arbitrary name -> false");
    }

    std::printf("\n%s\n", g_fail == 0 ? "ALL CHECKS PASSED" : "SOME CHECKS FAILED");
    return g_fail == 0 ? 0 : 1;
}
