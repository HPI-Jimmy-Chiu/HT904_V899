// ===========================================================================
//  KYECFTP/FTPClient_EventHandlers.h
//  TfFTPClient's "NMFTP1" TNMFTP event-handler bodies (status/success/failure
//  logging), translated as free functions matching Nmftp::TNMFTP's
//  std::function callback-slot signatures (see KYECFTP/MiniFtpEngine.h --
//  READ THAT FILE FIRST if you have not; this unit is built directly on top
//  of it and reuses its TCmdType/AnsiString/TObject/TComponent/Word types).
//
//  Golden: HT9011UC_Code_V3.33.906.0_20260618/KYECFTP/FTPClient.cpp
//    :1799-1829  NMFTP1ListItem(AnsiString Listing)
//    :1960-1986  NMFTP1Success(TCmdType Trans_Type)
//    :1987-1992  NMFTP1AuthenticationFailed(bool &Handled)
//    :1993-1997  NMFTP1TransactionStop(TObject *Sender)
//    :1998-2002  NMFTP1TransactionStart(TObject *Sender)
//    :2003-2008  NMFTP1ConnectionFailed(TObject *Sender)
//    :2009-2035  NMFTP1Failure(bool &Handled, TCmdType Trans_Type)
//    :2036-2061  NMFTP1UnSupportedFunction(TCmdType Trans_Type)
//    :2086-2091  NMFTP1Error(TComponent *Sender, WORD Errno, AnsiString Errmsg)
//    :2092-2097  NMFTP1Status(TComponent *Sender, AnsiString Status)
//  Golden decl home: FTPClient.h:112-121 (TfFTPClient __published section).
//  Exact bounds re-verified by grep against golden (not guessed) -- every
//  function above is bracketed on both sides by an unrelated, purely-VCL
//  handler (cbTesterIDChange/memoFTPDblClick/rgInputMethodClick/FilterList)
//  that this unit deliberately does NOT translate (out of scope: no FTP
//  calc/state, pure widget wiring for a form this wave does not build).
//
//  Translation wave: W5-Final-KYECFTP (Phase 2, following Phase 1's
//  MiniFtpEngine.{h,cpp}). Author: AI(W5-Final-FTPClientEventHandlers) 20260711.
//
//  DISCOVERED GOLDEN QUIRK #1 (preserved verbatim, NOT fixed) -- the
//  NMFTP1/NMFTP2 naming is misleading: grepping EVERY `->OnXxx = NMFTPnYyy;`
//  assignment in the whole 5439-line file shows the *NMFTP1Xxx* handler
//  BODIES (this unit's scope) are overwhelmingly wired onto a *TNMFTP*
//  variable literally named `NMFTP2` (LoadFileFormServer2/UploadFileToServer2/
//  CheckFTPConnection/Download_2DSortingList/Download_2DID_WhiteList, golden
//  :137-145/429-437/4474-4482/4893-4901/5283-5291 -- five IDENTICAL 9-line
//  blocks), while a SEPARATE, silent (MyDBIProcess-only, no memoFTP) sibling
//  family *NMFTP2Xxx* (golden :4111-4237, OUT OF THIS UNIT'S SCOPE) is wired
//  onto a `pFTP`/`NMFTP2`-named object elsewhere (:3664-3666, :4038-4046).
//  Both families' underlying `TNMFTP` component instance variable names are
//  themselves inconsistent/reused across call sites (per MiniFtpEngine.h's
//  own file banner: "no single fixed instance name"). This unit implements
//  ONLY the NMFTP1Xxx family (the memoFTP-logging one); the NMFTP2Xxx sibling
//  family is a different, not-yet-translated unit (flagged under "symbols
//  another parallel unit might touch" below).
//
//  DISCOVERED GOLDEN QUIRK #2 (preserved verbatim, NOT fixed) -- of the 5
//  wiring blocks cited above, NONE of them assigns `OnUnSupportedFunction`
//  (each block is the SAME 9 lines: OnListItem/OnSuccess/OnFailure/OnError/
//  OnStatus/OnConnectionFailed/OnTransactionStart/OnTransactionStop/
//  OnAuthenticationFailed -- grepped exhaustively, confirmed identical every
//  time). `NMFTP1UnSupportedFunction` (golden :2036-2061) is therefore a
//  REAL, fully-implemented, but permanently DEAD function in golden -- no
//  TNMFTP instance ever fires it. Only the sibling `NMFTP2UnSupportedFunction`
//  gets wired (golden :3664, :4046), to the `pFTP`/silent-logging family.
//  This unit still translates NMFTP1UnSupportedFunction faithfully (the task
//  scope explicitly names it) and exposes it as a normal callable function --
//  it is simply not included in FTPClientEvt_WireNMFTP1Handlers() below, to
//  match golden's own dead-wiring exactly. See test file for a test that
//  calls it directly (proving the function itself is correct) without
//  wiring it through an actual TNMFTP instance (proving nothing in THIS
//  unit "fixes" the omission).
//
//  DISCOVERED GOLDEN QUIRK #3 (preserved verbatim, NOT fixed) -- the
//  `bool &Handled` output parameter of NMFTP1Failure and
//  NMFTP1AuthenticationFailed is NEVER assigned inside either golden body
//  (read the full golden text -- confirmed, no `Handled=` token in either
//  function). MiniFtpEngine.cpp's own engine-side caller (FireCmdFailure_)
//  independently confirms this parameter is vestigial end-to-end: it
//  initializes `handled=false` before firing OnFailure and never reads it
//  back afterward either. This unit's translated bodies leave `Handled`
//  untouched too, exactly matching golden.
//
//  STATE DESIGN (judgment call, flag for review) -- golden's `bListOk`
//  (TfFTPClient private member) and `bError` (TfFTPClient public member,
//  comment "KaiChen 20190530: changed to public") are per-form-INSTANCE
//  fields, but golden only ever constructs ONE `TfFTPClient` (the
//  `extern PACKAGE TfFTPClient *fFTPClient;` singleton, FTPClient.h:207) --
//  same reasoning Automation/AGV_E84.cpp's translation used to justify
//  demoting `bE84Loaderflag`/`bE84Unloaderflag` from per-instance fields to
//  plain externs. `tmpList` (TStringList*) and `bTempList` (bool, default
//  false) are NOT demoted -- they are ALREADY genuine file-scope (unit-level,
//  outside the class) globals in golden itself (FTPClient.cpp:42/44), shared
//  by many OTHER out-of-scope functions in the same TU (ShowFTPModal,
//  DoPassword, CheckSetupFileNameFromServer, plUnloadALLClick, ...). Kept
//  under their EXACT golden bare names (grep-verified zero collision
//  anywhere in this tree) so that a future Phase-2 unit translating the rest
//  of FTPClient.cpp can `#include` this header and reuse -- not redefine --
//  the SAME two globals, matching golden's own single-TU sharing.
//
//  WIDGET-TOUCH SEAM (NEW additive infrastructure, judgment call, flag for
//  review) -- the only 3 VCL touches in this scope are `memoFTP->Lines->Add`
//  (a TMemo log), `lstServerFile->Items->Add` and `ListBox1->Items->Add`
//  (two TListBox item lists). No `TfFTPClient` facade exists anywhere yet
//  (grepped FormsFacade.h/atester_shims.h/canary_support.h -- confirmed).
//  Per this task's own instruction ("route through a simple log-sink stub
//  matching the FormsFacade no-op pattern already established elsewhere,
//  e.g. atester.cpp's fMain->LightOn()"), rather than editing the banned
//  FormsFacade.h myself, this file defines its OWN tiny local seam
//  (FTPClientEvt_LogSink below) with a default Sim implementation that
//  CAPTURES lines into an observable buffer (not a bare no-op like
//  TfMain::LightOn()) -- chosen over a bare no-op because NMFTP1ListItem's
//  entire purpose is populating a list a Phase-2 caller reads back
//  (ShowFTPModal etc.), so an inspectable double is far more useful for this
//  unit's own tests and for whichever future wave builds the real
//  `TfFTPClient` facade. See "shared-file wiring" in the hand-off report for
//  exactly what a future FormsFacade extension should look like.
//
//  GATED DEPENDENCY -- `fLotInfo->sJSCCOSFileName` (golden FTPClient.cpp:1813,
//  `TfLotInfo`/uLotInfo.h member) does not exist on FormsFacade.h's `TfLotInfo`
//  today (grepped -- confirmed absent). Gated via a TU-local stand-in
//  returning AnsiString("") (see .cpp) -- conservative default: since this
//  branch only matters when `CUSTOMER_CODE==CC_JSCC_OS` (a single named
//  customer), and `AnsiPos("")` reports "not found" (0), the JSCC_OS-only
//  filter degrades to "never matches" (bAdd=false for every .zip entry)
//  rather than crashing or silently admitting everything -- flagged for
//  human review since this is the one behavior that is NOT provably
//  equivalent to golden for that specific customer.
//
//  Include directly: #include "KYECFTP/FTPClient_EventHandlers.h"
//  (also pulls in KYECFTP/MiniFtpEngine.h transitively).
// ===========================================================================
#ifndef KYECFTP_FTPCLIENT_EVENTHANDLERS_H
#define KYECFTP_FTPCLIENT_EVENTHANDLERS_H

#include "KYECFTP/MiniFtpEngine.h"
#include <functional>
#include <vector>

// ---------------------------------------------------------------------------
//  Golden file-scope globals (FTPClient.cpp:42/44) -- see STATE DESIGN note
//  above. Reused verbatim by any future Phase-2 FTPClient.cpp translation.
// ---------------------------------------------------------------------------
extern vclcompat::TStringList* tmpList;   // golden: `TStringList *tmpList;` (:42), lazily new'd
extern bool                    bTempList; // golden: `bool bTempList=false;` (:44)

// ---------------------------------------------------------------------------
//  Demoted TfFTPClient singleton-instance members -- see STATE DESIGN note
//  above (bare golden names, singleton-instance argument, AGV_E84 precedent).
// ---------------------------------------------------------------------------
extern bool bListOk; // golden: TfFTPClient private member (FTPClient.h:132)
extern bool bError;  // golden: TfFTPClient public  member (FTPClient.h:149)

// Test-only: resets bListOk/bError/tmpList/bTempList to golden's initial
// values (false/false/NULL-then-lazy/false) and clears the Sim log-sink
// capture buffers. NOT part of the golden API -- additive test surface,
// mirrors SecsEventReport.h's g_SimLastEventReportCeid reset convention.
void FTPClientEvt_ResetStateForTest();

// ---------------------------------------------------------------------------
//  FTPClientEvt_LogSink -- widget-touch seam. See WIDGET-TOUCH SEAM note
//  above. Default-installs a Sim capturing implementation (observable via
//  the 3 accessors below); call SetFTPClientEvtLogSink() to replace it (a
//  future FormsFacade TfFTPClient wiring would install real widget calls
//  here instead).
// ---------------------------------------------------------------------------
struct FTPClientEvt_LogSink {
    std::function<void(const AnsiString&)> AddMemoLine;       // memoFTP->Lines->Add
    std::function<void(const AnsiString&)> AddServerFileItem; // lstServerFile->Items->Add
    std::function<void(const AnsiString&)> AddListBox1Item;   // ListBox1->Items->Add
};

void SetFTPClientEvtLogSink(const FTPClientEvt_LogSink& Sink);
void ResetFTPClientEvtLogSinkToSimDefault(); // reinstalls the capturing Sim sink + clears its buffers

const std::vector<AnsiString>& FTPClientEvt_SimMemoLines();
const std::vector<AnsiString>& FTPClientEvt_SimServerFileItems();
const std::vector<AnsiString>& FTPClientEvt_SimListBox1Items();

// ---------------------------------------------------------------------------
//  The 10 NMFTP1 event-handler bodies. Signatures match Nmftp::TNMFTP's
//  TFTP*Event std::function typedefs EXACTLY (MiniFtpEngine.h:407-413) so
//  each can be assigned directly to the matching `nmftp.OnXxx = ...;` slot.
// ---------------------------------------------------------------------------
void FTPClientEvt_NMFTP1ListItem(AnsiString Listing);                       // golden :1799
void FTPClientEvt_NMFTP1Success(TCmdType Trans_Type);                       // golden :1960
void FTPClientEvt_NMFTP1AuthenticationFailed(bool &Handled);                // golden :1987
void FTPClientEvt_NMFTP1TransactionStop(TObject *Sender);                  // golden :1993
void FTPClientEvt_NMFTP1TransactionStart(TObject *Sender);                 // golden :1998
void FTPClientEvt_NMFTP1ConnectionFailed(TObject *Sender);                 // golden :2003
void FTPClientEvt_NMFTP1Failure(bool &Handled, TCmdType Trans_Type);       // golden :2009
void FTPClientEvt_NMFTP1UnSupportedFunction(TCmdType Trans_Type);          // golden :2036 (DEAD in golden -- see QUIRK #2)
void FTPClientEvt_NMFTP1Error(TComponent *Sender, vclcompat::Word Errno,
                               AnsiString Errmsg);                         // golden :2086
void FTPClientEvt_NMFTP1Status(TComponent *Sender, AnsiString Status);     // golden :2092

// ---------------------------------------------------------------------------
//  FTPClientEvt_WireNMFTP1Handlers -- reproduces golden's own most common
//  9-line wiring block VERBATIM (golden :137-145, repeated identically at
//  :429-437/:4474-4482/:4893-4901/:5283-5291) -- deliberately does NOT wire
//  OnUnSupportedFunction (see QUIRK #2 above; not a bug in this unit, a
//  faithful reproduction of golden's own omission).
// ---------------------------------------------------------------------------
void FTPClientEvt_WireNMFTP1Handlers(Nmftp::TNMFTP& Ftp);

#endif // KYECFTP_FTPCLIENT_EVENTHANDLERS_H
