// ===========================================================================
//  KYECFTP/FTPClient_Transfer.h
//  TfFTPClient's 4 real FTP-transfer "session" functions, translated as free
//  functions against KYECFTP/MiniFtpEngine.h's Nmftp::TNMFTP (Phase 1) and
//  KYECFTP/FTPClient_EventHandlers.h's NMFTP1* handler family (a parallel
//  Phase-2 sibling unit -- READ BOTH HEADERS FIRST if you have not; this unit
//  is built directly on top of them and reuses their actual public interface,
//  not a guessed one).
//
//  Golden: HT9011UC_Code_V3.33.906.0_20260618/KYECFTP/FTPClient.cpp
//    :107-389    LoadFileFormServer2(AnsiString FtpPath, AnsiString Source)
//    :390-846    UploadFileToServer2(AnsiString FtpPath, AnsiString Source, bool bZip=false)
//    :4877-4953  Download_2DSortingList(AnsiString FtpPath, AnsiString Source, int iTimeOut=20000)
//    :5267-5387  Download_2DID_WhiteList(AnsiString FtpPath)
//  Golden decl home: FTPClient.h:176,177,200,201 (TfFTPClient public section).
//  Exact bounds re-verified by grep against golden (bracketed on both sides by
//  the next/previous `__fastcall TfFTPClient::` function) -- NOT the task
//  brief's estimates verbatim; see "DISCREPANCY vs. scoping" in the .cpp for
//  the 3 corrected line ranges (LoadFileFormServer2 283 lines not ~76;
//  UploadFileToServer2 457 lines not... task brief's own numbers were for
//  the OTHER two functions, which matched closely: Download_2DSortingList is
//  77 lines not ~76 -- off by one due to the trailing "---" comment line;
//  Download_2DID_WhiteList is 121 lines, matching the brief exactly).
//
//  Translation wave: W5-Final-KYECFTP (Phase 2b, following Phase 1's
//  MiniFtpEngine.{h,cpp} and the parallel Phase-2a FTPClient_EventHandlers
//  sibling). Author: AI(W5-Final-FTPClientTransfer) 20260711.
//
//  ARCHITECTURE -- extract-calc-core (KNOWLEDGE.md convention): TfFTPClient
//  itself (a VCL TForm, FTPClient.h) is NOT translated. Only the 4 named
//  session functions are, as plain free functions under their EXACT golden
//  names (no prefix -- auto9045.cpp / AGV_E84.cpp precedent: bare names are
//  kept when collision-checked clean; grepped, zero collision anywhere in
//  this tree before writing).
//
//  STATE DESIGN (judgment call, flag for review) -- 3 more golden
//  TfFTPClient-adjacent globals this unit's 2 functions touch that the
//  FTPClient_EventHandlers sibling did NOT already cover:
//    FTP_DownloadFail  golden FTPClient.cpp:46, a bare FILE-SCOPE global (NOT
//                       a TfFTPClient member) -- shared with 2 out-of-scope
//                       sibling functions in the same golden TU
//                       (ShowFTPModal :992/1042, DownloadPasswordFormServer
//                       :4499/4529/4536). This is the FIRST unit to translate
//                       any part of FTPClient.cpp that touches it -- defined
//                       here; a future sibling unit translating
//                       ShowFTPModal/DownloadPasswordFormServer MUST extern
//                       this, not redeclare it.
//    bPIDTransferErr   golden FTPClient.h:159 (TfFTPClient PUBLIC member,
//                       "Eastsun 20260522 整合: PID Transfer Error flag").
//                       Per-instance, but golden only ever constructs ONE
//                       TfFTPClient (`fFTPClient`) -- same demotion-to-extern
//                       reasoning FTPClient_EventHandlers.h already applied
//                       to bError/bListOk (itself following Automation/
//                       AGV_E84.cpp's bE84Loaderflag precedent). Assigned for
//                       real only inside ShowFTPModal (golden :1190/1202, out
//                       of THIS unit's scope) -- THIS unit's UploadFileToServer2
//                       only READS it (`bPIDTransferErr==false` guards, golden
//                       :522/600). A future sibling translating ShowFTPModal
//                       MUST extern this same global, not redeclare it.
//    NMFTP3            golden FTPClient.cpp:1214, a bare file-scope
//                       `TNMFTP *NMFTP3;` (NOT a class member either) --
//                       genuinely owned/assigned for real by ShowFTPModal /
//                       FormClose / N25_ReadAutoStartFileFromFTP /
//                       CheckFTPConnection (ALL out of this unit's scope).
//                       THIS unit's Download_2DSortingList/Download_2DID_
//                       WhiteList only ever READ it via `NMFTP3!=NULL` --
//                       see "DISCOVERED GOLDEN QUIRK" in the .cpp for why
//                       that guard is checking an entirely unrelated
//                       variable from their own local `NMFTP2` engine
//                       instance. Declared/defined here (defaults NULL,
//                       never assigned by this TU) -- a future sibling
//                       translating ShowFTPModal etc. MUST extern this same
//                       pointer, not redeclare it.
//
//  Include directly: #include "KYECFTP/FTPClient_Transfer.h"
//  (also pulls in KYECFTP/FTPClient_EventHandlers.h + MiniFtpEngine.h
//  transitively).
// ===========================================================================
#ifndef KYECFTP_FTPCLIENT_TRANSFER_H
#define KYECFTP_FTPCLIENT_TRANSFER_H

#include "KYECFTP/FTPClient_EventHandlers.h"

// ---------------------------------------------------------------------------
//  3 more golden TfFTPClient-adjacent globals -- see STATE DESIGN note above.
//  Defined in the .cpp (this is the one TU that owns them until a future
//  sibling unit translates the rest of FTPClient.cpp).
// ---------------------------------------------------------------------------
extern bool           FTP_DownloadFail;  // golden FTPClient.cpp:46 (bare file-scope global)
extern bool           bPIDTransferErr;   // golden FTPClient.h:159  (TfFTPClient public member, demoted -- singleton)
extern Nmftp::TNMFTP* NMFTP3;            // golden FTPClient.cpp:1214 (bare file-scope global, likewise)

// ---------------------------------------------------------------------------
//  The 4 in-scope session functions (golden names preserved verbatim,
//  golden default-argument values preserved verbatim; `__fastcall` dropped --
//  neutralized to nothing by vcl_compat.h, same convention as auto9045.h).
// ---------------------------------------------------------------------------
void LoadFileFormServer2(AnsiString FtpPath, AnsiString Source);                          // golden :107-389
void UploadFileToServer2(AnsiString FtpPath, AnsiString Source, bool bZip = false);       // golden :390-846
void Download_2DSortingList(AnsiString FtpPath, AnsiString Source, int iTimeOut = 20000); // golden :4877-4953
void Download_2DID_WhiteList(AnsiString FtpPath);                                         // golden :5267-5387

// ---------------------------------------------------------------------------
//  Test-only surface (additive, NOT part of the golden API):
//    FTPClientTransfer_ResetStateForTest() -- resets FTP_DownloadFail/
//      bPIDTransferErr/NMFTP3 to golden's initial values (false/false/NULL)
//      and forwards to FTPClientEvt_ResetStateForTest() so a test doesn't
//      have to call both resets separately. Mirrors that sibling's own
//      naming/behavior convention.
//    FTPClientTransfer_SetFastDelayForTest(bool) -- see the "delay()"
//      judgment-call note in the .cpp: golden's own busy-wait constants
//      (50/100/200/500ms, several per call) are faithfully preserved via a
//      real blocking Sleep() by default; this escape hatch lets this unit's
//      own tests skip the wall-clock cost. Defaults to false (real delay,
//      golden-faithful timing) so nothing changes for a hypothetical future
//      production build that links this TU without ever touching the switch.
// ---------------------------------------------------------------------------
void FTPClientTransfer_ResetStateForTest();
void FTPClientTransfer_SetFastDelayForTest(bool bFast);

#endif // KYECFTP_FTPCLIENT_TRANSFER_H
