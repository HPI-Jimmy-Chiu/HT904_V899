// =============================================================================
//  ProductionInfo/TfFTP.h  --  reusable FTP helper class (NOT a VCL form,
//                              despite the golden "Tf" naming convention).
//
//  Faithful translation of golden ProductionInfo/TfFTP.h (64 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("prodinfo" group), 2 of 3 units (after
//  ProductionInfo/FileInfo.{h,cpp}, a compile-time dependency of this one --
//  see FileInfo.h's own banner for why the group's nominal ordering was
//  adjusted).
//
//  ROLE: a small, self-contained, multi-instance FTP client wrapper (golden's
//  own comment: "JimmyChiu 20220120 add FTP class") -- directory navigation
//  with mkdir-on-demand (ChangeDir/ChangeDirectories/SplitPath/CleanPath/
//  NavigateToCommonPrefix/NavigateOrCreateFolders), filtered download
//  (DownloadFilterFile + 2 "get one name" variants), upload, delete, rename,
//  and a `std::map<int,AnsiString>` directory-listing cache (GetAllFolder/
//  GetFileList). Distinct from -- and NOT layered on top of -- golden
//  KYECFTP/FTPClient.cpp's `TfFTPClient` (a real VCL TForm with UI widgets):
//  TfFTP wraps the SAME underlying TNMFTP component family but exposes a
//  completely different, higher-level, form-free API (per KYECFTP/
//  MiniFtpEngine.h's own file banner, "OTHER GOLDEN TNMFTP CONSUMERS OUTSIDE
//  THIS UNIT'S SCOPE" note, which explicitly flags TfFTP as out-of-scope for
//  that engine's own original wave and names this exact API-shape
//  difference).
//
//  SUBSTRATE REUSE (not a fresh design): golden's `#include <NMFtp.hpp>` /
//  `TNMFTP` / `NMOS_AUTO` / `TCmdType` / `TObject` / `TComponent` are ALL
//  already real in this tree via KYECFTP/MiniFtpEngine.h's `Nmftp::TNMFTP`
//  engine (built for golden KYECFTP/FTPClient.cpp's OWN TNMFTP usage, in an
//  earlier wave) -- read that header's file banner FIRST if you have not;
//  this class is built directly on top of its documented SCOPED API SURFACE
//  (Connect/Disconnect/ChangeDir/MakeDirectory/Mode/Download/Upload/Nlist/
//  Delete/Abort/RequestCloseSocket + the 12 named events), with ZERO new
//  wire-protocol code of its own. `TNMFTP::Rename` is the ONE method this
//  class calls that MiniFtpEngine.h's OWN "NOT implemented" list (banner:
//  "List(), RemoveDir(), Rename(), ReInit(), ...") says golden's
//  FTPClient.cpp never calls -- see the GATE REGISTER below.
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim -- all 35 golden TfFTP.cpp function bodies;
//     see TfFTP.cpp's own banner for the full golden-line inventory).
//   SATISFIED-BY-SUBSTRATE: Nmftp::TNMFTP (KYECFTP/MiniFtpEngine.h, already
//     real, see above) stands in for golden's <NMFtp.hpp> TNMFTP.
//     vclcompat::TStringList (already real) stands in for golden's TStringList.
//
//  GATE REGISTER -- exactly ONE gate, re-cited at its call site in the .cpp:
//   (1) `pFTP->Rename(asSource, asTarget);`  golden TfFTP.cpp:335 (TfFTP::Rename).
//       WHY IT CANNOT LAND AS A REAL CALL: Nmftp::TNMFTP (KYECFTP/
//       MiniFtpEngine.h) has NO `Rename` member -- that header's own file
//       banner lists `Rename()` explicitly under "NOT implemented (grepped,
//       ZERO call sites in golden FTPClient.cpp -- do NOT add)", because the
//       engine's scope was built exhaustively from golden KYECFTP/
//       FTPClient.cpp's OWN call sites, which never call Rename. THIS
//       file (ProductionInfo/TfFTP.cpp) is a DIFFERENT golden translation
//       unit that DOES call it once -- a genuine cross-unit scope gap, not a
//       guess. This wave's own task scope is exactly the 3 ProductionInfo
//       files, not a MiniFtpEngine.h extension (a pre-existing file this wave
//       may not edit, HARD RULE 7).
//       Handled with the tree's #if 0/#else macro-pair idiom (aTester_Front.cpp
//       :150-156 / aRotateKIT.cpp precedent): golden's call stays VERBATIM in
//       the #if 0 arm, and the ACTIVE arm drops ONLY that one wire-call
//       statement -- golden's OWN very next statement, `return true;`
//       (unconditional, not gated on any Rename outcome -- golden's real
//       TNMFTP::Rename is void, so there was never a success code to check
//       here either), is kept VERBATIM, not rewritten to `false`.
//       WHY KEEPING `return true;` VERBATIM IS THE FAITHFUL DEFAULT: HARD
//       RULE 3 ("do not reorder branches") applies to the gate exactly as it
//       does to any other line -- the only thing with no substrate is the
//       single `pFTP->Rename(...)` call; golden's control flow around it
//       (find the match, attempt the rename, unconditionally report success)
//       is reproduced exactly, dropping only the one statement that cannot
//       compile.
//       BEHAVIOUR DELTA, STATED PLAINLY (the important one to weigh): on a
//       real FTP server, golden's `return true` already meant "we attempted
//       the rename", not "the server confirmed it" (golden never inspects a
//       Rename result). This port's `return true` means "we found a
//       matching filename" only -- the remote file is NEVER actually
//       renamed. Any caller treating this `true` as "renamed on the server"
//       will be wrong until this gate is retired. Retire this gate by
//       extending Nmftp::TNMFTP with a `Rename(const AnsiString& OldPath,
//       const AnsiString& NewPath)` method (RNFR OldPath -> 3xx -> RNTO
//       NewPath -> 2xx=OnSuccess(cmdRename)/else OnFailure -- TCmdType
//       already HAS a `cmdRename` enumerator, MiniFtpEngine.h:374, reserved
//       for exactly this) in a future wave that owns KYECFTP/MiniFtpEngine.h.
//
//  VCL/Borland conversions: `#include <NMFtp.hpp>` (golden :6) ->
//  "KYECFTP/MiniFtpEngine.h". `<map>` (golden :7) kept. No __property /
//  __published in this header (golden has none). `void __fastcall
//  GetFileList(AnsiString)` / the 12 `void __fastcall NMFTPxxx(...)` event
//  handlers (golden's own __fastcall-marked declarations) drop __fastcall
//  per HARD RULE 4 -- neutral no-op in this build, matching every other
//  ported unit's convention (Public/MyStringList.h precedent).
//
//  EVENT WIRING (mechanical adaptation, not a golden behaviour change):
//  golden's BCB6 `__closure` member-function pointers bind `pFTP->OnXxx=
//  NMFTPxxx;` directly (:36-47, ctor). Nmftp::TNMFTP's event slots are
//  `std::function<...>` (free-standing, no implicit `this`) -- the .cpp's
//  ctor wires each one through a `[this](...){ this->NMFTPxxx(...); }`
//  lambda instead. Net dispatch is IDENTICAL (each event still reaches the
//  exact same member-function body on the exact same `TfFTP` instance);
//  only the C++-level mechanism differs.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include <NMFtp.hpp> ; #include <map>
// =============================================================================
#ifndef ProductionInfo_TfFTPH
#define ProductionInfo_TfFTPH
//---------------------------------------------------------------------------
#include "KYECFTP/MiniFtpEngine.h"   // Nmftp::TNMFTP + NMOS_AUTO/MODE_ASCII/TCmdType/
                                     //   TObject/TComponent (brought to global scope by
                                     //   MiniFtpEngine.h's own `using namespace Nmftp;`)
                                     //   -- stands in for golden's <NMFtp.hpp>
#include <map>
//---------------------------------------------------------------------------
class TfFTP                                                                     //JimmyChiu 20220120 add FTP class
{
private:                                                                        // User declarations
    TNMFTP *pFTP;
    bool bflag;
    AnsiString asUserID;
    AnsiString asPassword;
    AnsiString asHost;
    int  Vendor;
    int  TimeOut;
    int  Port;
    bool Passive;
    bool bNListOk;
    bool bChangeDirOk;

    std::map<int, AnsiString> mapFileList;
    void GetFileList(AnsiString asFilePath);
    TStringList* SplitPath(AnsiString path);
    bool NavigateToCommonPrefix();
    bool NavigateOrCreateFolders(TStringList *targetFolders, AnsiString originalDir);
    AnsiString CleanPath(AnsiString path);
public:                                                                         // User declarations
    TfFTP(AnsiString _asUserID, AnsiString _asPassword, AnsiString _asHost);
    ~TfFTP();

    TStringList *lFolder;
    TStringList *lTempFolder;

    void Close();                                                               //Jimmychiu 20220901 新增Close FTP
    bool Connect();
    bool Connect(AnsiString _asUserID, AnsiString _asPassword, AnsiString _asHost, int _TimeOut=20000, int _Vendor=NMOS_AUTO, bool _Passive=true, int _Port=21);
    bool ChangeDir(AnsiString asFilePath);
    bool ChangeDirectories(AnsiString asFilePath);                              //Steven 20250716 : 多層次FTP資料夾切換
    bool DownloadFilterFile(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFilter, AnsiString asError);
    AnsiString DownloadFilterFile_Get1stFileName(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFilter, AnsiString asError);
    AnsiString DownloadFilterFile_GetLastFileName(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFilter, AnsiString asError);
    bool Upload(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFileName, AnsiString asError);
    bool Upload(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString sSourceFileName, AnsiString sTargetFileName, AnsiString asError);
    bool Delete(AnsiString sSourcesFilePath, AnsiString asFilter, AnsiString asError);
    std::map<int, AnsiString> GetAllFolder(AnsiString &sSourcesFilePath, AnsiString asError);
    void CheckFTPFilePath(AnsiString &aspath);
    void CheckLocalFilePath(AnsiString &aspath);
    bool Rename(const AnsiString &sourcePath, const AnsiString &targetPath, const AnsiString &sSourFileName, const AnsiString &sTarFileName);
    AnsiString RemoveAllTrailingSlashes(AnsiString sInput);

    bool bError;
    void NMFTP2Success(TCmdType Trans_Type);                                   //Steven 20220510 : 定義FTP動作事件成功
    void NMFTP2TransactionStop(TObject *Sender);
    void NMFTP2TransactionStart(TObject *Sender);
    void NMFTP2ConnectionFailed(TObject *Sender);
    void NMFTP2UnSupportedFunction(TCmdType Trans_Type);
    void NMFTP2Failure(bool &Handled, TCmdType Trans_Type);
    void NMFTP2AuthenticationFailed(bool &Handled);
    void NMFTP2Error(TComponent *Sender, vclcompat::Word Errno, AnsiString Errmsg);   //AI(W906-PT-W2) 20260807: golden `WORD` (real Win32 typedef, needs <windows.h>) -> `vclcompat::Word` (Comm.h, same width) for header self-sufficiency -- matches KYECFTP/FTPClient_EventHandlers.h:183's identical precedent/reasoning; net-identical type (both unsigned short)
    void NMFTP2Status(TComponent *Sender, AnsiString Status);
    void NMFTP2Connect(TObject *Sender);
    void NMFTP2Disconnect(TObject *Sender);
    void NMFTP2ListItem(AnsiString Listing);
};
#endif
