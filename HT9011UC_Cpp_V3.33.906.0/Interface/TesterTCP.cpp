//---------------------------------------------------------------------------
//  Interface/TesterTCP.cpp -- see TesterTCP.h for full scope / provenance /
//    NOT-ATTEMPTED notes.
//---------------------------------------------------------------------------
#include "Interface/TesterTCP.h"

#include "Config.h"                  // IniConfig (bN06_CopyTesterFile, asN06_TesterPath, bA57_1SaveArmSpeedByMachine)
#include "CosFunction.h"             // CosFunction (bSaveArmSpeedByMachine)
#include "common.h"                  // DataPath, sSaveByMachine, WriteIniData(AnsiString,AnsiString,AnsiString,int);
                                      // transitively (via vclcompat/vcl_compat.h) also brings in the
                                      // already-ACTIVE vclcompat SysUtils free functions this file calls
                                      // directly: FileExists / IncludeTrailingBackslash.
#include "canary_support.h"          // RecordProcess, ShowMyMessage -- hard-boundary file: READ-ONLY, not edited
                                      // (used only inside Gated_MyForceDirectories below)

#include <cstdlib>                   // system()

//===========================================================================
//  Gated dependencies (TU-local, internal linkage -- cannot collide with any
//  other translation unit). Each stands in for a genuinely not-yet-available
//  golden dependency; cited at every call site below.
//===========================================================================
namespace {

// ---------------------------------------------------------------------------
// Gated_MyForceDirectories -- golden common.h:262, declared inside common.h's
// own `#if 0 // TODO(wave-file)` block (this TU cannot forward-declare the
// real name without an ODR/redeclaration hazard once common.h is ungated).
// This is a VERBATIM copy of the already-reviewed faithful re-implementation
// established by the sibling Interface/TesterTCP_Socket.cpp (same golden
// dependency, same real body read from common.cpp:1667-1717) -- reproduced
// here rather than shared via a header because each gated dependency in this
// project is intentionally TU-local (see docs/KNOWLEDGE.md); not scope creep,
// just the established per-TU gate idiom applied to the same golden function.
// AI(W906-fire-verify) 20260716: restating the sibling file's own caveat here
// too (fire-wave fidelity review noted a reader of this file in isolation
// would otherwise miss it) -- golden's `catch(Exception&e)` branch (return
// -2, e.Message) is merged into this generic `catch(...)` (return -3, fixed
// message). Harmless at both call sites in THIS file (their int return value
// is discarded either way), but worth knowing before reusing this helper
// somewhere that DOES branch on -2 vs -3.
// ---------------------------------------------------------------------------
int Gated_MyForceDirectories(AnsiString Directory, AnsiString Function = "")
{
    if (Directory == "")
    {
        RecordProcess("Directory value is NULL!", Function);
        return -1;
    }

    if (Directory[Directory.Length()] != '\\')
    {
        int attr = FileGetAttr(Directory);
        if (attr != -1)
        {
            if (!(attr & faDirectory))
                Directory = ExtractFilePath(Directory);
        }
        else
        {
            AnsiString sExt = ExtractFileExt(Directory).LowerCase();
            if (sExt == ".txt" || sExt == ".csv")
                Directory = ExtractFilePath(Directory);
        }
    }

    if (Directory == "")
    {
        RecordProcess("Directory value is NULL!", Function);
        return -1;
    }
    else
    {
        try
        {
            if (DirectoryExists(Directory) == false)
                ForceDirectories(Directory);
        }
        catch (...)
        {
            AnsiString Str;
            Str.sprintf("%s -- %s", Directory, Function);
            ShowMyMessage("Create directory fail!", Str, Str);
            return -3;
        }
    }

    return 1;
}

// ---------------------------------------------------------------------------
// IncludeTrailingPathDelimiter -- BCB6 synonym for IncludeTrailingBackslash
// (same TU-local synonym idiom already established by cpublic.cpp:135-138,
// which carries the identical "AI(...) -- BCB6 synonym" note). Both golden
// CopyRecipeToTester (:1065) and CopyRecipeFromTester (:1085) call it under
// this exact name.
// ---------------------------------------------------------------------------
inline AnsiString IncludeTrailingPathDelimiter(const AnsiString& p)
{
    return IncludeTrailingBackslash(p);
}

} // anonymous namespace

// ---------------------------------------------------------------------------
// MySleep(DWORD) -- golden common.h:261. common.h's own declaration is inside
// a `#if 0 // TODO(wave-timing)` block, so `#include "common.h"` alone does
// not expose it. A REAL, ACTIVE offline no-op definition already exists in
// acarry_shims.cpp (confirmed reused the same way by Automation/auto9045.cpp,
// CCLink/MyCCLink.cpp and KYECFTP/FTPClient_Transfer.cpp's own identical
// `extern void MySleep(DWORD dwMilliseconds);` redeclaration + "REAL
// definition: acarry_shims.cpp" comment) -- reused here via the same local
// extern redeclaration, not a new/fourth duplicate definition. NOTE: this
// wave's CMakeLists.txt is not yet wired for this new file (integrate stage
// owns that), so this extern is unresolved until this TU is linked alongside
// acarry_shims.cpp; test_TesterTCP.cpp (this wave's own test) supplies its
// own local no-op definition instead, matching the tests/test_FTPClient_
// Transfer.cpp / tests/test_MyCCLink.cpp precedent for a standalone test binary.
// ---------------------------------------------------------------------------
extern void MySleep(DWORD dwMilliseconds);

//===========================================================================
//  golden TfTesterTCP::CopyRecipeToTester(AnsiString FileName), TesterTCP.cpp:1058-1077.
//===========================================================================
void TesterTCP_CopyRecipeToTester(AnsiString FileName)
{
    AnsiString str1, str2, str3;
    // Steven 20250327/20230710: OS測試機的工作檔也要上傳
    // ("the OS Tester's own work-file also needs to be uploaded")
    if (IniConfig.bN06_CopyTesterFile == true)
    {
        str1.sprintf("%s%s\\OS_Setting.zip", DataPath, FileName);
        str2.sprintf("%s", IncludeTrailingPathDelimiter(IniConfig.asN06_TesterPath));
        if (FileExists(str1))
        {
            str3.sprintf("e \"%s\" -o\"%s\" -y", str1, str2);

            // AI(W906-D-Interface-TesterTCP) 20260716: golden calls
            //   hInstance = ShellExecute(this,"open","D:\\HT9045\\7z.exe",
            //                            str3.c_str(),NULL,SW_HIDE);
            //   if(int(hInstance)<=32) { ; }   // golden's own empty-branch idiom, TesterTCP.cpp:1070-1073
            // (async fire-and-forget launch; golden's follow-up MySleep(500)
            // is its own throttle to give the just-launched external process a
            // head start before returning). VCL ShellExecute has no
            // vclcompat/portable-C++ equivalent anywhere in this tree yet
            // (would pull in a new <shellapi.h>+shell32 link dependency not
            // shared by anything else translated so far). Translated instead
            // to this project's OWN established idiom for invoking this exact
            // 7z.exe, already used by Automation/auto9045.cpp's
            // DoULRequest/DoDLRequest and KYECFTP/FTPClient_Transfer.cpp: a
            // system() call on a "D:\\HT9045\\7z.exe ..." command line.
            // AI(W906-fire-verify) 20260716: fire-wave fidelity review flagged
            // (medium) that a bare system() call BLOCKS the caller until 7z
            // actually finishes, unlike golden's async ShellExecute+MySleep(500)
            // best-effort throttle -- a real behavior divergence, dormant only
            // because nothing wires this function to a live call site yet.
            // Fixed here by routing through the cmd.exe `start /B` launcher
            // (still system(), still zero new headers/deps, same idiom as the
            // rest of this codebase's 7z.exe seams) so the external process is
            // detached/backgrounded instead of blocking -- this also restores
            // the ORIGINAL purpose of the golden-faithful MySleep(500) call
            // right below (a head-start throttle for a still-running external
            // process), which a blocking system() had made a redundant no-op.
            // `start` reads its first quoted token as a window title (which
            // would otherwise misparse if `cmd` itself started with a quoted
            // path) -- the explicit `""` empty title below sidesteps that
            // ambiguity unconditionally, regardless of `cmd`'s own contents.
            AnsiString cmd;
            cmd.sprintf("D:\\HT9045\\7z.exe %s", str3);   // capital "D:" preserved verbatim from golden :1069
            AnsiString shellCmd;
            shellCmd.sprintf("start \"\" /B %s", cmd);
            system(shellCmd.c_str());

            MySleep(500);
        }
    }
}

//===========================================================================
//  golden TfTesterTCP::CopyRecipeFromTester(AnsiString FileName), TesterTCP.cpp:1079-1097.
//===========================================================================
void TesterTCP_CopyRecipeFromTester(AnsiString FileName)
{
    AnsiString str1, str2, str3;
    // Steven 20250327/20230710: OS測試機的工作檔也要上傳
    // ("the OS Tester's own work-file also needs to be uploaded")
    if (IniConfig.bN06_CopyTesterFile == true)
    {
        str1.sprintf("%s%s\\OS_Setting.zip", DataPath, FileName);
        str2.sprintf("%s%s.ini", IncludeTrailingPathDelimiter(IniConfig.asN06_TesterPath), FileName);
        if (FileExists(str2))
        {
            str3.sprintf("a -tzip \"%s\" \"%s\"", str1, str2);

            // AI(W906-D-Interface-TesterTCP) 20260716: same ShellExecute ->
            // system() translation rationale as CopyRecipeToTester above
            // (golden TesterTCP.cpp:1089 `ShellExecute(this,"open",
            // "D:\\HT9045\\7z.exe",str3.c_str(),NULL,SW_HIDE);`, return value
            // discarded by golden too -- no `hInstance` even captured here).
            // AI(W906-fire-verify) 20260716: same `start /B` non-blocking fix
            // as CopyRecipeToTester above -- see that function's comment for
            // the full rationale (fire-wave fidelity review, medium severity).
            AnsiString cmd;
            cmd.sprintf("D:\\HT9045\\7z.exe %s", str3);   // capital "D:" preserved verbatim from golden :1089
            AnsiString shellCmd;
            shellCmd.sprintf("start \"\" /B %s", cmd);
            system(shellCmd.c_str());

            MySleep(2000);   // Steven 20240105: add delay (golden TesterTCP.cpp:1090)
        }
        else
        {
            // golden's own dead branch: a commented-out
            // `str.sprintf("N06, step 1 : File %s does not exists!", str2);`
            // with no live statement -- empty-branch idiom, nothing to
            // translate (golden TesterTCP.cpp:1093-1095).
        }
    }
}

//===========================================================================
//  golden TfTesterTCP::btnSaveClick(TObject *Sender), TesterTCP.cpp:1099-1115.
//===========================================================================
void TesterTCP_btnSaveClick(bool bChkAuto1_OSChecked,
                             bool bChkAuto2_OSChecked,
                             bool bChkAuto3_OSChecked,
                             AnsiString LastFileName)
{
    AnsiString szDir = "";
    if (CosFunction.bSaveArmSpeedByMachine && IniConfig.bA57_1SaveArmSpeedByMachine)
    {
        szDir = sSaveByMachine;
    }
    else
    {
        szDir.sprintf("%s%s", DataPath, LastFileName);
    }
    Gated_MyForceDirectories(szDir);
    szDir += "\\ArmCondition.Data";
    WriteIniData(szDir, "Output Arm", "JSCC_OS_Use Auto1", bChkAuto1_OSChecked ? 1 : 0);
    WriteIniData(szDir, "Output Arm", "JSCC_OS_Use Auto2", bChkAuto2_OSChecked ? 1 : 0);
    WriteIniData(szDir, "Output Arm", "JSCC_OS_Use Auto3", bChkAuto3_OSChecked ? 1 : 0);
}
