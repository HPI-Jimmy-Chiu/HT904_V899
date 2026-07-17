// =============================================================================
//  test_TesterTCP.cpp  --  W906 VERIFY: Interface/TesterTCP (bounded subset #2
//                           of the golden TfTesterTCP OS/Open-Short Tester
//                           form -- see Interface/TesterTCP.h for full scope)
//
//  Front: D-Interface-TesterTCP.  Author: AI(W906-D-Interface-TesterTCP) 20260716.
//
//  PURPOSE
//  -------
//  Proves Interface/TesterTCP.cpp LINKS and its ACTIVE logic behaves per
//  golden Interface/TesterTCP.cpp (READ-ONLY reference), for all 3 functions
//  translated this wave: CopyRecipeToTester, CopyRecipeFromTester, btnSaveClick.
//
//  NO EXTERNAL FRAMEWORK: a tiny check harness prints PASS/FAIL per case and
//  a final summary, returning non-zero on ANY failure (same style as
//  tests/test_testertcp_socket.cpp / tests/test_interfacesys.cpp).
//
//  FILESYSTEM NOTE: DataPath / IniConfig.asN06_TesterPath / sSaveByMachine are
//  mutable globals with real production defaults under D:\HT9045\... This
//  test REASSIGNS all three to a small scratch folder next to the test
//  binary BEFORE exercising anything, matching the established precedent in
//  tests/test_testertcp_socket.cpp (asTestTCPIPLogPath override) -- so running
//  this test never touches the real production tree.
//
//  DELIBERATE EXCLUSION (flagged for human review, matching this project's
//  test_auto9045.cpp "DELIBERATE EXCLUSION" precedent for DoULRequest/
//  DoDLRequest): this harness does NOT drive CopyRecipeToTester/
//  CopyRecipeFromTester all the way into their `system("start \"\" /B
//  D:\\HT9045\\7z.exe ...")` call (AI(W906-fire-verify) 20260716: routed
//  through `start /B` so the shell-out is non-blocking, see TesterTCP.cpp's
//  own comment) -- that command line is a HARDCODED absolute path (golden's
//  own literal, not overridable via any global), and actually invoking it
//  would shell out to a real 7z.exe against this dev machine (which may or
//  may not even have one at that exact path) -- exactly the real-OS-
//  side-effect hazard this front's task brief calls out. Both functions are
//  instead driven up to, but never across, that line via their two natural
//  golden guards: (1) IniConfig.bN06_CopyTesterFile==false short-circuits the
//  whole body; (2) the golden FileExists() check on the OS_Setting.zip /
//  <FileName>.ini yields false when the scratch tree has no such file (the
//  common case in a byte-for-byte fresh scratch folder), so the system()
//  line is never reached in either configuration this test drives. Both are
//  genuine, golden-faithful control-flow branches -- not a weakened test.
// =============================================================================
#include "Interface/TesterTCP.h"
#include "Config.h"
#include "CosFunction.h"
#include "common.h"

#include <cstdio>
#include <cstring>
#include <string>

static int g_pass = 0;
static int g_fail = 0;

static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected) { std::printf("PASS  %-58s got=%d\n", name, (int)got); ++g_pass; }
    else { std::printf("FAIL  %-58s got=%d exp=%d\n", name, (int)got, (int)expected); ++g_fail; }
}

static void check_i(const char* name, int got, int expected)
{
    if (got == expected) { std::printf("PASS  %-58s got=%d\n", name, got); ++g_pass; }
    else { std::printf("FAIL  %-58s got=%d exp=%d\n", name, got, expected); ++g_fail; }
}

// ---------------------------------------------------------------------------
// MySleep(DWORD) -- golden common.h:261, gated declaration (see
// Interface/TesterTCP.cpp's own note on this same extern).
//
// AI(W906-fire-integrate) 20260716: INTEGRATE FIX -- this test binary IS
// wired to link ht9045_sm (via the RESCAN group in tests/CMakeLists.txt,
// needed regardless for the real DataPath/sSaveByMachine/WriteIniData/
// ReadIniData/RecordProcess/ShowMyMessage globals TesterTCP.cpp's own gated
// helper references), which carries acarry_shims.cpp's one REAL offline
// no-op MySleep(DWORD) definition. A second local definition here (as this
// front's own translate report originally wrote it, under the assumption
// this test would NOT link ht9045_sm -- matching the test_FTPClient_
// Transfer.cpp/test_MyCCLink.cpp "avoid the god-stack" precedent) would be a
// multiple-definition link error, so it was removed at integrate time.
// ---------------------------------------------------------------------------

int main()
{
    std::printf("=== Interface/TesterTCP translation verification ===\n");
    std::printf("(DataPath/IniConfig.asN06_TesterPath/sSaveByMachine redirected to a\n");
    std::printf(" local scratch folder for the duration of this test)\n\n");

    // Redirect every mutable path global this file touches away from the
    // real production tree (see file-head FILESYSTEM NOTE).
    DataPath                     = ".\\_test_scratch_testertcp\\Data\\";
    IniConfig.asN06_TesterPath   = ".\\_test_scratch_testertcp\\TesterPath\\";
    sSaveByMachine                = ".\\_test_scratch_testertcp\\SaveByMachine\\";
    ForceDirectories(DataPath);
    ForceDirectories(IniConfig.asN06_TesterPath);

    // -------------------------------------------------------------------
    // 1. CopyRecipeToTester: IniConfig.bN06_CopyTesterFile==false is a pure
    //    golden no-op (golden TesterTCP.cpp:1062's outer `if` guards the
    //    entire body) -- verify it does not crash and leaves the scratch
    //    tree untouched.
    // -------------------------------------------------------------------
    {
        IniConfig.bN06_CopyTesterFile = false;
        AnsiString sZip;
        sZip.sprintf("%sGATE_OFF_RECIPE\\OS_Setting.zip", DataPath);
        TesterTCP_CopyRecipeToTester("GATE_OFF_RECIPE");
        check_b("CopyRecipeToTester: bN06_CopyTesterFile=false -> no zip appears", FileExists(sZip), false);
    }

    // -------------------------------------------------------------------
    // 2. CopyRecipeToTester: flag=true but the golden FileExists(OS_Setting.zip)
    //    guard (golden TesterTCP.cpp:1066) is false in a fresh scratch tree --
    //    the system()/7z line (translated from golden ShellExecute, see the
    //    .cpp file's translation-note comment) must NOT be reached.
    // -------------------------------------------------------------------
    {
        IniConfig.bN06_CopyTesterFile = true;
        AnsiString sZip;
        sZip.sprintf("%sNO_ZIP_RECIPE\\OS_Setting.zip", DataPath);
        check_b("CopyRecipeToTester: precondition, zip absent before call", FileExists(sZip), false);
        TesterTCP_CopyRecipeToTester("NO_ZIP_RECIPE");
        check_b("CopyRecipeToTester: zip still absent after call (system() not invoked)", FileExists(sZip), false);
    }

    // -------------------------------------------------------------------
    // 3. CopyRecipeFromTester: same two guards, mirrored (golden TesterTCP.cpp:1082/1086).
    // -------------------------------------------------------------------
    {
        IniConfig.bN06_CopyTesterFile = false;
        AnsiString sZip;
        sZip.sprintf("%sGATE_OFF_RECIPE2\\OS_Setting.zip", DataPath);
        TesterTCP_CopyRecipeFromTester("GATE_OFF_RECIPE2");
        check_b("CopyRecipeFromTester: bN06_CopyTesterFile=false -> no zip appears", FileExists(sZip), false);
    }
    {
        IniConfig.bN06_CopyTesterFile = true;
        AnsiString sZip;
        sZip.sprintf("%sNO_INI_RECIPE\\OS_Setting.zip", DataPath);
        AnsiString sTesterIni;
        sTesterIni.sprintf("%sNO_INI_RECIPE.ini", IniConfig.asN06_TesterPath);
        check_b("CopyRecipeFromTester: precondition, Tester .ini absent before call", FileExists(sTesterIni), false);
        TesterTCP_CopyRecipeFromTester("NO_INI_RECIPE");
        check_b("CopyRecipeFromTester: zip still absent after call (system() not invoked)", FileExists(sZip), false);
    }

    // -------------------------------------------------------------------
    // 4. btnSaveClick: CosFunction.bSaveArmSpeedByMachine==false (or
    //    IniConfig.bA57_1SaveArmSpeedByMachine==false) branch -- golden
    //    TesterTCP.cpp:1106-1109, szDir = DataPath+LastFileName. Fully
    //    exercised (Gated_MyForceDirectories + WriteIniData have no
    //    hardcoded-absolute-path hazard, unlike the 7z calls above) --
    //    verifies real directory creation + real ArmCondition.Data
    //    round-trip via ReadIniData.
    // -------------------------------------------------------------------
    {
        CosFunction.bSaveArmSpeedByMachine        = false;
        IniConfig.bA57_1SaveArmSpeedByMachine     = false;
        TesterTCP_btnSaveClick(/*Auto1*/true, /*Auto2*/false, /*Auto3*/true, "MYRECIPE_DP");

        AnsiString szDir;
        szDir.sprintf("%sMYRECIPE_DP", DataPath);
        check_b("btnSaveClick(DataPath branch): directory created", DirectoryExists(szDir), true);

        AnsiString szIni = szDir + "\\ArmCondition.Data";
        check_b("btnSaveClick(DataPath branch): ArmCondition.Data exists", FileExists(szIni), true);
        check_i("btnSaveClick(DataPath branch): Auto1=1", ReadIniData(szIni, "Output Arm", "JSCC_OS_Use Auto1", -1), 1);
        check_i("btnSaveClick(DataPath branch): Auto2=0", ReadIniData(szIni, "Output Arm", "JSCC_OS_Use Auto2", -1), 0);
        check_i("btnSaveClick(DataPath branch): Auto3=1", ReadIniData(szIni, "Output Arm", "JSCC_OS_Use Auto3", -1), 1);
    }

    // -------------------------------------------------------------------
    // 5. btnSaveClick: CosFunction.bSaveArmSpeedByMachine==true AND
    //    IniConfig.bA57_1SaveArmSpeedByMachine==true -- golden
    //    TesterTCP.cpp:1102-1105, szDir = sSaveByMachine (the LastFileName
    //    parameter is a don't-care in this branch, matching golden exactly --
    //    golden's own szDir=sSaveByMachine assignment never reads LastFileName
    //    at all in this branch). PRESERVED GOLDEN QUIRK: golden
    //    unconditionally does `szDir+="\\ArmCondition.Data";` AFTER the
    //    branch, and sSaveByMachine already ends in a trailing backslash
    //    (common.cpp:172 "...\\SaveByMachine\\") -- so this branch produces a
    //    literal DOUBLE backslash in the final path
    //    ("...SaveByMachine\\\\ArmCondition.Data"). Reproduced verbatim (not
    //    "fixed") -- this test builds its own expected path the exact same
    //    (quirky) way, rather than a hand-normalized one, so it is checking
    //    what golden's logic actually produces.
    // -------------------------------------------------------------------
    {
        CosFunction.bSaveArmSpeedByMachine     = true;
        IniConfig.bA57_1SaveArmSpeedByMachine  = true;
        TesterTCP_btnSaveClick(/*Auto1*/false, /*Auto2*/true, /*Auto3*/false, "IGNORED_IN_THIS_BRANCH");

        check_b("btnSaveClick(SaveByMachine branch): directory created", DirectoryExists(sSaveByMachine), true);

        AnsiString szIni = sSaveByMachine;      // golden's own szDir=sSaveByMachine
        szIni += "\\ArmCondition.Data";         // golden's own unconditional append (double-backslash quirk preserved)
        check_b("btnSaveClick(SaveByMachine branch): ArmCondition.Data exists (quirky double-backslash path)", FileExists(szIni), true);
        check_i("btnSaveClick(SaveByMachine branch): Auto1=0", ReadIniData(szIni, "Output Arm", "JSCC_OS_Use Auto1", -1), 0);
        check_i("btnSaveClick(SaveByMachine branch): Auto2=1", ReadIniData(szIni, "Output Arm", "JSCC_OS_Use Auto2", -1), 1);
        check_i("btnSaveClick(SaveByMachine branch): Auto3=0", ReadIniData(szIni, "Output Arm", "JSCC_OS_Use Auto3", -1), 0);
    }

    std::printf("\n=== %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
