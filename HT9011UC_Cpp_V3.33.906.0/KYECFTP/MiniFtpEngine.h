// ===========================================================================
//  KYECFTP/MiniFtpEngine.h
//  A minimal, from-scratch FTP protocol engine standing in for the closed
//  BCB6 component TNMFTP (`#include <NMFtp.hpp>`, Borland "FastNet" suite).
//
//  AI(W5-Final-KYECFTP) 20260711: new file. This is a DESIGN unit, not a
//  translation -- NMFtp.hpp's source does not exist anywhere in this repo
//  (verified: no NMFtp.hpp, no vendor doc, unlike vclcompat/Comm.h's SPComm
//  precedent which could cite D:/HT9045/elec/Component/spcomm.hpp). Every
//  wire-level behavior below is INFERRED from how golden
//  KYECFTP/FTPClient.cpp actually calls TNMFTP (grepped exhaustively, not
//  guessed from memory of a real BCB6 component), the same methodology
//  vclcompat/ClientSocket.h used for TClientSocket. Read ClientSocket.h
//  FIRST if you have not -- MiniFtpEngine is built ON TOP of two of its
//  instances (one control-channel connection, one PASV data-channel
//  connection, opened fresh per transfer) and copies its Sim/Real,
//  "DELIBERATE DEVIATION" documentation tone verbatim.
//
//  SCOPED API SURFACE (every TNMFTP touch point in golden 906
//  KYECFTP/FTPClient.cpp, re-verified by grep against ALL call-site
//  variable names the golden source actually uses -- NMFTP1/NMFTP2/NMFTP3/
//  pFTP/Ptr; TNMFTP has NO single fixed instance name, golden creates a
//  fresh `new TNMFTP(this)` per call site and per routine):
//
//    Properties (grepped `->Prop =` / `->Prop` read across all 16+ call
//    sites, FTPClient.cpp:107-5427):
//      Host, UserID, Password              AnsiString, always set together
//      Port                                int; SEVEN "pFTP" call sites
//                                           (:2388-2415, :4274-4307,
//                                           :5103-5123, :5223-5243) NEVER
//                                           set it at all -- this is NOT a
//                                           gap in the golden code, it PROVES
//                                           the real TNMFTP default is 21
//                                           (those are real production
//                                           ASE_CL/N31/N32/N35 upload paths;
//                                           they would be non-functional
//                                           otherwise). MiniFtpEngine
//                                           defaults Port=21 on this evidence,
//                                           not as an unverified guess.
//      Vendor = NMOS_AUTO                  every site sets exactly this ONE
//                                           value, never anything else, and
//                                           it is NEVER READ anywhere in
//                                           golden. Confirmed via grep that
//                                           golden ONLY ever calls Nlist()
//                                           (bare NLST) -- it never calls a
//                                           rich List(), so there is no
//                                           OS-format list-parsing consumer
//                                           for Vendor to drive. Stored,
//                                           inert -- see enum note below.
//      TimeOut                             int ms; every site sets it
//                                           (5000-30000 observed)
//      Passive = true                      set at 8 sites, NEVER toggled
//                                           false anywhere in the 5439-line
//                                           file (grepped every `->Passive`
//                                           occurrence). The "pFTP" sites
//                                           never set it explicitly either --
//                                           consistent with a component
//                                           default of true. MiniFtpEngine
//                                           therefore ONLY ever implements
//                                           PASV data connections; there is
//                                           no active-mode (PORT command)
//                                           code path at all (not gated,
//                                           genuinely absent -- see below).
//      ParseList = true                    set at 3 "list" call sites only,
//                                           NEVER read. Stored, inert (same
//                                           reasoning as Vendor: only bare
//                                           Nlist() is ever called, so a
//                                           "parse the rich listing" flag
//                                           has nothing golden-side to feed).
//      Name                                AnsiString; write-then-read-via-
//                                           Sender-cast only for logging
//                                           (`MyDBIProcess("FTP", msg,
//                                           Ptr->Name)`, :4145 etc.) -- never
//                                           interpreted by TNMFTP itself.
//      Connected                            bool; READ-ONLY from golden's
//                                           perspective (never assigned,
//                                           :152/156/446/2301/2411/3316/...).
//      LastErrorNo                          int; READ-ONLY, used once to
//                                           build a diagnostic string (:818).
//      CurrentDir                           AnsiString; READ-ONLY
//                                           (`Cur=NMFTP3->CurrentDir;`,
//                                           :1339/:3905) -- see the CurrentDir
//                                           design-decision note below.
//
//    Methods (every method-call spelling found by grepping
//      `->(Connect|Disconnect|ChangeDir|MakeDirectory|Mode|Download|Upload|
//         Nlist|Delete|Abort|RequestCloseSocket|List|RemoveDir|Rename|
//         ReInit|UpRestore|DownRestore|Append|Allocate|DoCommand)\(` across
//      the ENTIRE file):
//      Connect()                 void, always called as a bare statement,
//                                 return value (if any) never used. Golden's
//                                 OWN call sites poll `Connected` in a
//                                 `delay(50)` retry loop afterward (up to 10x
//                                 = 500ms) -- textbook evidence the REAL
//                                 TNMFTP.Connect() is asynchronous. See the
//                                 "Async vs sync" design decision below.
//      Disconnect()               void; called from `catch` blocks after a
//                                 failed Connect() (:1310/:3880/:4585),
//                                 always immediately followed by Abort().
//      ChangeDir(AnsiString)      void (CWD)
//      MakeDirectory(AnsiString)  void (MKD); golden ALWAYS calls
//                                 ChangeDir first, MakeDirectory only in the
//                                 failure branch (mkdir-then-cd-again idiom,
//                                 :734-759) -- MiniFtpEngine does not encode
//                                 that idiom itself, it is FTPClient.cpp's
//                                 (a Phase-2 sibling unit's) call-site logic.
//      Mode(MODE_ASCII|MODE_IMAGE|MODE_BYTE)   void, no return, no cmd-type
//                                 classification (see enum/Mode() notes).
//      Download(RemoteFile, LocalFile)   void; arg order confirmed at
//                                 :229/:245 (`str02`=remote `Source+".zip"`,
//                                 `str01`=local `DataPath+...`) --
//                                 Download(Remote, Local).
//      Upload(LocalFile, RemoteFile)     void; arg order confirmed at
//                                 :542-544 (`str01`=local `ActivePath...`,
//                                 `str02`=remote `FtpPath...`) --
//                                 Upload(Local, Remote).
//      Nlist()                    void, ALWAYS zero-argument (grepped every
//                                 call site) -- lists CurrentDir/the last
//                                 ChangeDir'd directory. Golden NEVER calls
//                                 the rich List().
//      Delete(RemoteFile)         void (DELE)
//      Abort()                    void; called at the tail of EVERY golden
//                                 routine unconditionally (success or
//                                 failure), always paired with
//                                 RequestCloseSocket() right after. See the
//                                 "Async vs sync" note: in a synchronous
//                                 engine there is realistically never
//                                 anything in flight when Abort() runs.
//      RequestCloseSocket()       void; final teardown, object is `delete`d
//                                 by the caller shortly after every call
//                                 site -- never reused.
//
//    NOT implemented (grepped, ZERO call sites in golden -- do NOT add):
//      List(), RemoveDir(), Rename(), ReInit(), UpRestore(), DownRestore(),
//      Append(), Allocate(), DoCommand().
//
//    Events (every `->OnXxx = handler;` assignment across all call sites;
//      cross-checked against the golden TfFTPClient handler PROTOTYPES in
//      FTPClient.h, which pin the exact parameter shapes):
//      OnListItem(AnsiString Listing)
//      OnSuccess(TCmdType Trans_Type)
//      OnFailure(bool& Handled, TCmdType Trans_Type)
//      OnError(TComponent* Sender, WORD Errno, AnsiString Errmsg)
//      OnStatus(TComponent* Sender, AnsiString Status)
//      OnConnectionFailed(TObject* Sender)
//      OnTransactionStart(TObject* Sender)
//      OnTransactionStop(TObject* Sender)
//      OnAuthenticationFailed(bool& Handled)
//      OnUnSupportedFunction(TCmdType Trans_Type)
//      OnConnect(TObject* Sender)             (golden: NMFTP2Connect, :4225)
//      OnDisconnect(TObject* Sender)           (golden: NMFTP2Disconnect, :4232)
//
//  OTHER GOLDEN TNMFTP CONSUMERS OUTSIDE THIS UNIT'S SCOPE (flagged for
//  awareness, NOT built against here -- the task scoped this unit to
//  FTPClient.cpp only): ProductionInfo/TfFTP.{h,cpp} wraps TNMFTP behind a
//  COMPLETELY DIFFERENT higher-level API (`Connect(user,pw,host,timeout,
//  vendor,mode,port)`, `ChangeDirectories`, `Upload(dir,uploadpath,src,dst,
//  err)`, `CheckLocalFilePath`) -- NOT a drop-in match for MiniFtpEngine;
//  HS_Function.h / cTrayMapping.h / Automation/SCK_ART.h merely
//  `#include <NMFtp.hpp>` for a member pointer, not yet greped for call
//  shape. A future FTP wave translating any of those should re-grep its own
//  call sites rather than assuming this engine's surface covers them.
//
//  ASYNC VS SYNC (DELIBERATE DEVIATION, matching ClientSocket.h's own
//  precedent and tone): the real TNMFTP is asynchronous (event-driven);
//  golden's own `delay(int ms)` helper (FTPClient.cpp:66-80) is a literal
//  busy-wait `while + Application->ProcessMessages()` that EVERY call site
//  uses immediately after Connect()/around transfers -- proof the code was
//  always written to "call, then spin until the event fires or a poll count
//  runs out". MiniFtpEngine instead makes Connect()/Download()/Upload()/
//  Nlist()/Delete() straightforwardly SYNCHRONOUS: each one blocks until the
//  FTP command completes (or MiniFtpEngine's own bounded reply-wait, driven
//  by the `TimeOut` property, gives up) and fires its terminal event(s)
//  before returning. This is behaviorally equivalent for every observed
//  golden call site (a `delay(50)` retry loop that finds `Connected==true`
//  on its FIRST iteration behaves identically to one that finds it true
//  immediately) and is far simpler to implement/test. Flagged explicitly, as
//  ClientSocket.h flags its own Sim/Real policy deviation.
//
//  PASV-ONLY (byproduct of the Passive-always-true finding above): there is
//  no PORT-command / listening-socket code path in this engine at all.
//  Every data transfer opens exactly one additional outbound
//  Scktcomp::TClientSocket (the "data channel") to the IP:port parsed out of
//  the control channel's "227 Entering Passive Mode (h1,h2,h3,h4,p1,p2)"
//  reply, per RFC 959 4.1.2.
//
//  WIRE SEQUENCE (RFC 959 defaults, since no vendor source exists to verify
//  against -- documented per-command below; see MiniFtpEngine.cpp for the
//  literal command strings):
//    Connect  : open control TCP -> expect "220" banner -> "USER <id>" ->
//               "331" -> "PASS <pw>" -> "230" (or "230" directly on USER for
//               anonymous-style servers) -> Connected=true, fire OnConnect.
//               A 5xx anywhere in the login sequence (classically 530) fires
//               OnAuthenticationFailed instead of OnConnectionFailed (see
//               reply-classification table in the .cpp); a failure to open
//               the TCP socket at all, or no banking reply within TimeOut,
//               fires OnConnectionFailed.
//    ChangeDir: "CWD <dir>"      -> 2xx=OnSuccess(cmdChangeDir), else OnFailure
//    MakeDir  : "MKD <dir>"      -> 2xx=OnSuccess(cmdMakeDir),   else OnFailure
//    Mode     : "TYPE A"/"TYPE I"/"TYPE L 8" -- see Mode() note below; NOT
//               classified through OnSuccess/OnFailure (see note).
//    Download : "PASV" -> open data channel -> "RETR <remote>" -> 1xx ->
//               OnTransactionStart -> drain data channel to LocalFile ->
//               close data channel -> OnTransactionStop -> 2xx on control =>
//               OnSuccess(cmdDownload), else OnFailure.
//    Upload   : mirrors Download with "STOR <remote>", writing LocalFile's
//               bytes to the data channel.
//    Nlist    : "PASV" -> open data channel -> "NLST" -> 1xx ->
//               OnTransactionStart -> drain data channel, split into
//               newline-delimited bare filenames, fire OnListItem(name) for
//               EACH one (matches golden's NMFTP1ListItem accumulation
//               pattern) -> close data channel -> OnTransactionStop -> 2xx
//               on control => OnSuccess(cmdNList), else OnFailure. An empty
//               directory (zero listed names but a 2xx completion) is
//               treated as SUCCESS with zero OnListItem firings -- golden's
//               own NMFTP1Success unconditionally sets bListOk=true on
//               cmdNList regardless of item count, so "success with nothing
//               listed" must be a representable, non-failure outcome.
//    Delete   : "DELE <remote>"  -> 2xx=OnSuccess(cmdDelete), else OnFailure
//    CurrentDir (property read): "PWD" -> parse the quoted path out of the
//               "257 ..." reply -> ALSO fires OnSuccess(cmdCurrentDir) /
//               OnFailure(cmdCurrentDir) -- see the design-decision note.
//    Abort/RequestCloseSocket: see the Async-vs-sync note above; both are
//               best-effort local teardown, no OnSuccess/OnFailure firing.
//
//  DESIGN DECISION -- CurrentDir triggers a LIVE "PWD" round-trip on every
//  read (a property getter with a network side effect), rather than
//  returning a locally-cached string: golden's OWN TCmdType enum contains
//  `cmdCurrentDir`, and BOTH NMFTP1Success and NMFTP1Failure's exhaustive
//  switch statements classify it ("Success -- CurrentDir successful" /
//  "Failure -- CurrentDir failed", FTPClient.cpp:1971/2029) -- that dedicated
//  classification only makes sense if reading the property actually issues a
//  real FTP command whose reply can succeed or fail. A purely local cache
//  could never produce a `cmdCurrentDir` failure. This is the strongest
//  evidence available (no vendor source exists) and the Proxy idiom used
//  here (read-only, no `operator=` -- golden never assigns to CurrentDir)
//  mirrors vclcompat/ClientSocket.h's own `ActiveProxy` pattern.
//
//  DESIGN DECISION -- Mode()'s TYPE mapping: MODE_ASCII -> "TYPE A" and
//  MODE_IMAGE -> "TYPE I" are unambiguous RFC 959 mappings. MODE_BYTE has no
//  unambiguous RFC 959 equivalent to "Image/Binary" -- the two candidates
//  are (a) collapse it onto "TYPE I" (byte-oriented and image-oriented are
//  functionally identical on any modern 8-bit-byte OS), or (b) map it onto
//  RFC 959's actual "Local byte size" TYPE variant, "TYPE L <n>", using the
//  conventional n=8 (RFC 959 4.1.2, historically used by e.g. VMS-oriented
//  clients). Since the enum spells the mode "BYTE" (not just reusing
//  MODE_IMAGE), it is more likely the real component wanted a WIRE-VISIBLE
//  distinction from Image -- MiniFtpEngine maps MODE_BYTE -> "TYPE L 8".
//  Flagged: on virtually every real FTP server this is behaviorally
//  identical to "TYPE I" anyway, so this choice is unlikely to be
//  observable, but it is a genuine, unverifiable judgment call.
//  Mode()'s TYPE reply is read (to keep the control channel's line-framing
//  in sync for the NEXT command) but deliberately NOT run through the
//  OnSuccess/OnFailure classifier: golden's exhaustive TCmdType switch in
//  NMFTP1Success/NMFTP1Failure/NMFTP1UnSupportedFunction has NO case for any
//  "cmdMode"/"cmdType" value (16 cases, all accounted for above; none is
//  Mode-shaped) -- inventing an event firing not evidenced anywhere in
//  golden would be less faithful than simply not firing one.
//
//  DESIGN DECISION -- OnUnSupportedFunction vs OnFailure classification: FTP
//  reply "502 Command not implemented" (RFC 959 4.2.2) is the standard way a
//  server signals a command it does not support at all, as distinct from a
//  command it understood but failed to execute (4xx/5xx other codes) --
//  MiniFtpEngine fires OnUnSupportedFunction(cmd) on a literal 502 reply and
//  OnFailure(handled, cmd) on every other non-2xx reply to that command.
//
//  DESIGN DECISION -- REAL-mode data-channel EOF detection gap (inherited,
//  not fixed): vclcompat/ClientSocket.h's REAL-mode reader thread
//  (TCustomWinSocket::ReaderProc_) exits silently on `recv()` returning 0 or
//  an error WITHOUT flipping `bConnected` to false (verified by reading
//  ClientSocket.cpp) -- so `IsActiveNow()` cannot be used as a reliable
//  "peer closed the data connection" signal in REAL mode. This is a
//  pre-existing ClientSocket.h property, not something this unit is
//  authorized/wanted to change (a hard-boundary-adjacent shared shim other
//  units --  MyPLC/ModbusTCPClient.cpp today, more later per KNOWLEDGE.md --
//  already build against its current behavior). MiniFtpEngine's REAL-mode
//  data-drain therefore uses a pragmatic byte-idle heuristic (stop once no
//  new bytes have arrived for a short window, bounded by `TimeOut` overall)
//  instead of a clean EOF signal -- see ReadAllDataBytes_ in the .cpp.  SIM
//  mode (what this unit's own tests actually exercise) needs no such
//  heuristic: the SIM server hook (below) delivers a transfer's complete
//  payload synchronously in one shot, so a single drain-to-empty loop is
//  exact and deterministic.
//
//  SIM / TEST SURFACE (additive, NOT part of the BCB6 API -- mirrors
//  ClientSocket.h's own SimPushReceive/SimTxBuffer additions): MiniFtpEngine
//  is fully synchronous, so a Sim-mode "fake FTP server" cannot run on a
//  separate thread/timeline -- it is driven by a synchronous CALLBACK hook
//  instead. `SetSimServerHook()` registers a
//  `void(const char* Channel, const AnsiString& RawLine)` callback that
//  MiniFtpEngine invokes at exactly these points (all synchronous, all
//  still inside the blocking Connect()/Download()/... call):
//    "CTRL-CONNECT" -- fired once, right after the control channel's
//                    transport-level connect succeeds (SIM: always,
//                    synchronously) but BEFORE Connect() tries to read the
//                    "220" welcome banner. The server speaks first here (the
//                    banner is unsolicited, not a reply to any client
//                    command), so this is the hook's ONLY chance to push it
//                    -- there is no preceding "CTRL" firing to hang it off.
//                    RawLine is empty for this tag (nothing was "sent").
//    "CTRL"       -- fired right after a control-channel command LINE (sans
//                    CRLF) has been handed to the control socket's SendBuf().
//                    The hook should immediately call
//                    DebugControlSocket()->Socket->SimPushReceive(...) with
//                    the scripted server reply bytes for that command.
//    "DATA-OPEN"  -- fired right after the data channel's Active=true
//                    synchronously succeeds (SIM mode: this ALWAYS succeeds
//                    regardless of the parsed PASV IP:port, matching
//                    ClientSocket.h's own SIM DoConnect_ semantics). For a
//                    Download/Nlist, the hook should immediately push the
//                    full payload via
//                    DebugDataSocket()->Socket->SimPushReceive(...).
//    "DATA-CLOSE" -- fired after the data channel is marked inactive but
//                    BEFORE MiniFtpEngine reads the control channel's final
//                    completion reply ("226 ..."). For an Upload, the hook
//                    can inspect what was sent via
//                    DebugDataSocket()->Socket->SimTxBuffer() at this point
//                    (the data socket object is not deleted until the NEXT
//                    transfer, or destruction -- see DebugDataSocket()).
//                    Any transfer's hook should push the control channel's
//                    completion reply here, since no further "CTRL" hook
//                    fires between the data phase and that final read.
//  See tests/test_MiniFtpEngine.cpp for a worked SIM-mode fake-server
//  script exercising Connect/Login/CWD/NLST/RETR/STOR/DELE end to end.
//
//  Include directly: #include "KYECFTP/MiniFtpEngine.h" (NOT added to the
//  vcl_compat.h umbrella -- same "leave it to the integrate agent" posture
//  ClientSocket.h itself takes).
// ===========================================================================
#ifndef KYECFTP_MINIFTPENGINE_H
#define KYECFTP_MINIFTPENGINE_H

#include "vclcompat/AnsiString.h"
#include "vclcompat/TStringList.h"   // vclcompat::TObject
#include "vclcompat/Comm.h"          // vclcompat::TComponent (reuse, per Comm.h/ClientSocket.h)
#include "vclcompat/ClientSocket.h"  // Scktcomp::TClientSocket -- the transport this engine rides on
#include <vector>
#include <functional>

namespace Nmftp {

using vclcompat::AnsiString;
using vclcompat::TObject;
using vclcompat::TComponent;

// ---------------------------------------------------------------------------
//  TFTPVendorType -- golden's `Vendor` property type. ONLY NMOS_AUTO is ever
//  assigned anywhere in golden (grepped), and it is NEVER read (see file
//  banner) -- this engine treats it as a stored, inert field. Deliberately
//  NOT guessing the real component's other enumerator names/values (e.g.
//  NMOS_UNIX/NMOS_WIN/...): with zero golden evidence for them, inventing
//  plausible-sounding extra names would create false confidence without
//  adding any real capability (nothing in this engine or in golden ever
//  branches on Vendor).
// ---------------------------------------------------------------------------
enum TFTPVendorType { NMOS_AUTO };

// ---------------------------------------------------------------------------
//  TFTPTransferMode -- golden's `Mode()` argument. See the Mode() design
//  note in the file banner for the MODE_BYTE mapping judgment call.
// ---------------------------------------------------------------------------
enum TFTPTransferMode { MODE_ASCII, MODE_IMAGE, MODE_BYTE };

// ---------------------------------------------------------------------------
//  TCmdType -- golden's command-classification enum, reconstructed VERBATIM
//  from the exhaustive case-label sets of NMFTP1Success / NMFTP1Failure /
//  NMFTP1UnSupportedFunction (FTPClient.cpp:1960-2060) -- 16 named commands,
//  every one of which appears in at least one of those three switches.
//  Ordinal VALUES are not evidenced anywhere (case labels never rely on
//  ordinal order/arithmetic in golden) -- this engine's own ordering is a
//  free choice, taken from NMFTP1Success's case order.
// ---------------------------------------------------------------------------
enum TCmdType {
    cmdChangeDir, cmdMakeDir, cmdRemoveDir, cmdDelete, cmdRename, cmdReInit,
    cmdCurrentDir, cmdList, cmdNList, cmdUpRestore, cmdDownRestore,
    cmdDownload, cmdUpload, cmdAppend, cmdAllocate, cmdDoCommand
};

// ---------------------------------------------------------------------------
//  Reply-code classification -- RFC 959 3.2 classes. Exposed as a free
//  function (not just an internal .cpp detail) so it can be unit-tested in
//  ISOLATION from the Sim-server-hook machinery (see tests/test_MiniFtpEngine.cpp).
// ---------------------------------------------------------------------------
enum TFtpReplyClass {
    rcPreliminary,       // 1yz
    rcSuccess,           // 2yz
    rcIntermediate,      // 3yz (e.g. 331 "need password")
    rcTransientFailure,  // 4yz
    rcPermanentFailure,  // 5yz (e.g. 530 auth, 550 no such file, 502 unsupported)
    rcMalformed          // anything that isn't a 3-digit 1..5 leading code
};
TFtpReplyClass ClassifyFtpReply(int Code);

// Command-line framing helper (verb [+ ' ' + arg] + "\r\n"), exposed for
// isolated testing of the exact wire bytes MiniFtpEngine sends.
AnsiString BuildFtpCommandLine(const char* Verb, const AnsiString& Arg = AnsiString());

// PASV reply parser ("227 ... (h1,h2,h3,h4,p1,p2)" -> "h1.h2.h3.h4" + port).
// Tolerant of servers that omit the parentheses (RFC 959 does not mandate
// them) -- scans for the first run of 6 comma-separated integers anywhere in
// the line. Exposed for isolated testing.
bool ParsePasvReply(const AnsiString& ReplyLine, AnsiString& OutIp, int& OutPort);

// Event signatures -- pinned VERBATIM from the golden TfFTPClient handler
// prototypes (FTPClient.h, see file banner). `vclcompat::Word` (Comm.h) is
// the real BCB6 `WORD` (== unsigned short) OnError's `Errno` parameter uses.
typedef std::function<void(AnsiString Listing)>                         TFTPListItemEvent;
typedef std::function<void(TCmdType Trans_Type)>                        TFTPCmdEvent;         // OnSuccess, OnUnSupportedFunction
typedef std::function<void(bool& Handled, TCmdType Trans_Type)>         TFTPFailureEvent;      // OnFailure
typedef std::function<void(bool& Handled)>                              TFTPAuthFailedEvent;   // OnAuthenticationFailed
typedef std::function<void(TObject* Sender)>                            TFTPNotifyEvent;       // OnTransactionStart/Stop, OnConnectionFailed, OnConnect, OnDisconnect
typedef std::function<void(TComponent* Sender, vclcompat::Word Errno, AnsiString Errmsg)> TFTPErrorEvent;   // OnError
typedef std::function<void(TComponent* Sender, AnsiString Status)>      TFTPStatusEvent;       // OnStatus

// ---------------------------------------------------------------------------
//  TNMFTP -- the engine itself. Method/property/event NAMES are kept
//  IDENTICAL to golden's TNMFTP usage (per the task's Phase-2 drop-in
//  requirement) even though the real class this stands in for is not
//  present in this repo.
// ---------------------------------------------------------------------------
class TNMFTP : public vclcompat::TComponent {
public:
    explicit TNMFTP(vclcompat::TComponent* AOwner);
    virtual ~TNMFTP();

    // ---- properties (see file banner SCOPED API SURFACE) -------------------
    AnsiString     Host;
    AnsiString     UserID;
    AnsiString     Password;
    int            Port;          // default 21 -- see banner (evidenced, not guessed)
    TFTPVendorType Vendor;        // stored only, never interpreted -- see banner
    int            TimeOut;       // ms; bounds every blocking wait below
    bool           Passive;       // stored; PASV is ALWAYS used regardless -- see banner
    bool           ParseList;     // stored only, never interpreted -- see banner
    AnsiString     Name;          // free-form label, logging only

    bool           Connected;     // set by Connect()/Disconnect()/RequestCloseSocket() only
    int            LastErrorNo;   // last FTP reply code, or a WinSock/timeout sentinel; 0 = none yet

    // CurrentDir: READ triggers a live "PWD" round-trip -- see the design-
    // decision note in the file banner. Read-only (no operator=): golden
    // never assigns to it.
    class CurrentDirProxy {
    public:
        explicit CurrentDirProxy(TNMFTP* Owner) : owner_(Owner) {}
        operator AnsiString() const;
    private:
        TNMFTP* owner_;
    };
    CurrentDirProxy CurrentDir;

    // ---- methods (see file banner SCOPED API SURFACE) -----------------------
    void Connect();
    void Disconnect();
    void ChangeDir(const AnsiString& Dir);
    void MakeDirectory(const AnsiString& Dir);
    void Mode(TFTPTransferMode NewMode);
    void Download(const AnsiString& RemoteFile, const AnsiString& LocalFile);
    void Upload(const AnsiString& LocalFile, const AnsiString& RemoteFile);
    void Nlist();
    void Delete(const AnsiString& RemoteFile);
    void Abort();
    void RequestCloseSocket();

    // ---- events --------------------------------------------------------------
    TFTPListItemEvent   OnListItem;
    TFTPCmdEvent        OnSuccess;
    TFTPFailureEvent    OnFailure;
    TFTPErrorEvent      OnError;
    TFTPStatusEvent     OnStatus;
    TFTPNotifyEvent     OnConnectionFailed;
    TFTPNotifyEvent     OnTransactionStart;
    TFTPNotifyEvent     OnTransactionStop;
    TFTPAuthFailedEvent OnAuthenticationFailed;
    TFTPCmdEvent        OnUnSupportedFunction;
    TFTPNotifyEvent     OnConnect;
    TFTPNotifyEvent     OnDisconnect;

    // ---- SIM / test surface (additive, NOT part of the BCB6 API) -----------
    // Composition-root switch, same policy as ClientSocket.h: default Sim
    // (safe/offline/deterministic); propagates to both the control channel
    // and whichever data channel is currently open.
    void SetSimMode(bool bSim);
    bool IsSimMode() const;

    // See the file banner's "SIM / TEST SURFACE" section for the exact
    // contract (channel tags "CTRL" / "DATA-OPEN" / "DATA-CLOSE").
    typedef std::function<void(const char* Channel, const AnsiString& RawLine)> TFtpSimServerHook;
    void SetSimServerHook(TFtpSimServerHook Hook);

    // Test-only accessors to the underlying transport sockets, so a test's
    // sim-server hook can call SimPushReceive()/SimTxBuffer() on them.
    // DebugDataSocket() returns the MOST RECENTLY opened data channel (kept
    // alive, Active=false, until the NEXT transfer or destruction -- see
    // MiniFtpEngine.cpp's CloseDataChannel_/OpenDataChannel_) so a test can
    // still inspect it after the owning Download/Upload/Nlist call returns.
    Scktcomp::TClientSocket* DebugControlSocket() const;
    Scktcomp::TClientSocket* DebugDataSocket() const;

private:
    TNMFTP(const TNMFTP&);
    TNMFTP& operator=(const TNMFTP&);

    friend class CurrentDirProxy;
    AnsiString QueryCurrentDir_();

    void SendCtrlCmd_(const char* Verb, const AnsiString& Arg = AnsiString());
    bool ReadOneCtrlLine_(AnsiString& OutLine, int& OutCode);
    bool DoPasv_(AnsiString& OutIp, int& OutPort);
    bool OpenDataChannel_(const AnsiString& Ip, int DataPort);
    void CloseDataChannel_();
    void FireError_(int Code, const AnsiString& Msg);
    void FireCmdFailure_(TCmdType Cmd, int Code = 0);
    void SimpleCommand_(const char* Verb, const AnsiString& Arg, TCmdType Cmd);
    void ReadAllDataBytes_(std::vector<char>& Out);
    void WriteAllDataToChannel_(const std::vector<char>& Bytes);
    void FireSimHook_(const char* Channel, const AnsiString& RawLine = AnsiString());

    struct Impl;
    Impl* pImpl_;
};

} // namespace Nmftp

// Bring the FTP names into global scope, matching golden's implicit
// unqualified usage (TNMFTP, NMOS_AUTO, MODE_ASCII, cmdChangeDir, ...) --
// same umbrella-suppressible pattern as vclcompat/ClientSocket.h.
#ifndef MINIFTPENGINE_NO_GLOBAL_USING
using namespace Nmftp;
#endif

#endif // KYECFTP_MINIFTPENGINE_H
