// =============================================================================
//  tests/test_FileListBox.cpp -- vclcompat::TFileListBox acceptance test.
//
//  AI(W906-SpoolCluster) 20260721: new file.
//
//  Exercises the new TFileListBox shim directly (no THGem involved):
//    [1] Mask-with-directory-prefix (contains '\\') scan: real synchronous
//        rescan via Refresh(), filters by mask, skips subdirectories (even
//        one whose NAME matches the glob), case-sensitive Sort() still
//        yields the expected chronological order for zero-padded
//        date/time-style filenames (the exact shape DoSpool relies on).
//    [2] Update() performs an equally real rescan (not a stale/no-op) --
//        proven by adding a file between calls and observing Count change.
//    [3] Directory+Mask fallback branch (Mask has NO '\\') -- Directory is
//        combined via IncludeTrailingBackslash(Directory)+Mask.
//    [4] "."/".." are never present in Items (defensive skip, even though a
//        specific "*.dat" pattern would not normally surface them from
//        FindFirstFileA in practice).
//
//  SAFETY: all file I/O below happens under a dedicated scratch directory
//  (tests/filelistbox_test_scratch, relative to the test binary's own CWD),
//  never a production path. Cleaned up best-effort at the end of each test.
// =============================================================================
#include "vclcompat/FileListBox.h"
#include "vclcompat/SysUtils.h"

#include <cstdio>
#include <cstring>

using vclcompat::AnsiString;
using vclcompat::TFileListBox;
using vclcompat::ForceDirectories;
using vclcompat::FileExists;
using vclcompat::DirectoryExists;
using vclcompat::DeleteFile;
using vclcompat::RemoveDir;
using vclcompat::IncludeTrailingBackslash;

// --------------------------------------------------------------------------
//  Tiny PASS / FAIL harness (same style as the other vclcompat tests, e.g.
//  test_serversocket.cpp).
// --------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                   \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---------------------------------------------------------------------------
//  Helper: write a tiny file with the given name under dir (creates/
//  overwrites; contents are irrelevant to every test here).
// ---------------------------------------------------------------------------
static void writeScratchFile(const AnsiString& dir, const char* name)
{
    AnsiString path = IncludeTrailingBackslash(dir) + AnsiString(name);
    FILE* f = fopen(path.c_str(), "wb");
    if (f) {
        fwrite("x", 1, 1, f);
        fclose(f);
    }
}

// ===========================================================================
//  [1] Mask-with-directory-prefix scan: filter + skip-subdirectory + sort
// ===========================================================================
static void test_mask_with_directory_scan()
{
    printf("\n[1] TFileListBox: Mask with directory prefix -- filter/skip-dir/sort\n");

    const AnsiString kDir = "filelistbox_test_scratch_1";
    ForceDirectories(kDir);

    // 3 spool-shaped filenames, deliberately created OUT of chronological
    // order on disk, to prove Sort() (not creation order) drives the result.
    writeScratchFile(kDir, "2026_07_21 11_00_00 000.dat");
    writeScratchFile(kDir, "2026_07_21 09_00_00 000.dat");
    writeScratchFile(kDir, "2026_07_21 10_00_00 000.dat");
    // A non-matching extension -- must NOT appear.
    writeScratchFile(kDir, "notes.txt");
    // A SUBDIRECTORY whose name itself matches the "*.dat" glob -- proves
    // the faDirectory filter (not just extension matching) is what excludes
    // it. FindFirstFileA matches directory names against wildcards exactly
    // like file names, so without the explicit filter this would wrongly
    // appear in Items.
    ForceDirectories(IncludeTrailingBackslash(kDir) + AnsiString("dirlooksmatched.dat"));

    TFileListBox flb;
    flb.Mask = kDir + AnsiString("\\*.dat");   // contains '\\' -> used verbatim
    flb.Refresh();

    CHECK(flb.Items->Count == 3, "3 matching .dat files found (subdir + .txt excluded)");
    if (flb.Items->Count == 3) {
        CHECK(AnsiString(flb.Items->Strings[0]) == "2026_07_21 09_00_00 000.dat", "Strings[0] is the chronologically-earliest file");
        CHECK(AnsiString(flb.Items->Strings[1]) == "2026_07_21 10_00_00 000.dat", "Strings[1] is the middle file");
        CHECK(AnsiString(flb.Items->Strings[2]) == "2026_07_21 11_00_00 000.dat", "Strings[2] is the chronologically-latest file (FIFO tail)");
    }

    // "."/".." must never leak into Items.
    bool sawDot = false, sawDotDot = false;
    for (int i = 0; i < flb.Items->Count; i++) {
        if (AnsiString(flb.Items->Strings[i]) == ".") sawDot = true;
        if (AnsiString(flb.Items->Strings[i]) == "..") sawDotDot = true;
    }
    CHECK(sawDot == false, "\".\" never appears in Items");
    CHECK(sawDotDot == false, "\"..\" never appears in Items");

    // Cleanup (best-effort).
    DeleteFile(kDir + AnsiString("\\2026_07_21 11_00_00 000.dat"));
    DeleteFile(kDir + AnsiString("\\2026_07_21 09_00_00 000.dat"));
    DeleteFile(kDir + AnsiString("\\2026_07_21 10_00_00 000.dat"));
    DeleteFile(kDir + AnsiString("\\notes.txt"));
    RemoveDir(kDir + AnsiString("\\dirlooksmatched.dat"));
    RemoveDir(kDir);
}

// ===========================================================================
//  [2] Update() performs an equally real rescan (not a no-op / not stale)
// ===========================================================================
static void test_update_is_a_real_rescan()
{
    printf("\n[2] TFileListBox: Update() performs a real rescan, same as Refresh()\n");

    const AnsiString kDir = "filelistbox_test_scratch_2";
    ForceDirectories(kDir);
    writeScratchFile(kDir, "a.dat");

    TFileListBox flb;
    flb.Mask = kDir + AnsiString("\\*.dat");
    flb.Update();
    CHECK(flb.Items->Count == 1, "Update() alone (no prior Refresh()) populates Items");

    // Add a second file, then call Update() again -- if Update() were a
    // no-op / stale cache, Count would stay 1.
    writeScratchFile(kDir, "b.dat");
    flb.Update();
    CHECK(flb.Items->Count == 2, "Update() re-scans and sees the newly-added file");

    // Now Refresh() after removing a file -- symmetric proof for Refresh().
    DeleteFile(kDir + AnsiString("\\a.dat"));
    flb.Refresh();
    CHECK(flb.Items->Count == 1, "Refresh() re-scans and sees the removed file drop out");

    DeleteFile(kDir + AnsiString("\\b.dat"));
    RemoveDir(kDir);
}

// ===========================================================================
//  [3] Directory+Mask fallback (Mask has no '\\')
// ===========================================================================
static void test_directory_plus_mask_fallback()
{
    printf("\n[3] TFileListBox: Directory+Mask fallback branch (Mask has no '\\\\')\n");

    const AnsiString kDir = "filelistbox_test_scratch_3";
    ForceDirectories(kDir);
    writeScratchFile(kDir, "one.dat");
    writeScratchFile(kDir, "two.dat");

    TFileListBox flb;
    flb.Directory = kDir;    // no trailing backslash -- IncludeTrailingBackslash must add one
    flb.Mask = "*.dat";      // no '\\' -> Directory+Mask branch used
    flb.Refresh();

    CHECK(flb.Items->Count == 2, "Directory+Mask fallback finds both files");

    DeleteFile(kDir + AnsiString("\\one.dat"));
    DeleteFile(kDir + AnsiString("\\two.dat"));
    RemoveDir(kDir);
}

// ===========================================================================
static void test_empty_directory_yields_zero_count()
{
    printf("\n[4] TFileListBox: empty directory -> Items->Count == 0 (no crash)\n");

    const AnsiString kDir = "filelistbox_test_scratch_4";
    ForceDirectories(kDir);

    TFileListBox flb;
    flb.Mask = kDir + AnsiString("\\*.dat");
    flb.Refresh();
    CHECK(flb.Items->Count == 0, "no matching files -> Count 0, no crash");

    RemoveDir(kDir);
}

// ===========================================================================
int main()
{
    printf("=============================================================\n");
    printf(" test_FileListBox : vclcompat::TFileListBox\n");
    printf("=============================================================\n");

    test_mask_with_directory_scan();
    test_update_is_a_real_rescan();
    test_directory_plus_mask_fallback();
    test_empty_directory_yields_zero_count();

    printf("\n-------------------------------------------------------------\n");
    printf(" RESULT: %d passed, %d failed\n", g_pass, g_fail);
    printf("-------------------------------------------------------------\n");
    return (g_fail == 0) ? 0 : 1;
}
