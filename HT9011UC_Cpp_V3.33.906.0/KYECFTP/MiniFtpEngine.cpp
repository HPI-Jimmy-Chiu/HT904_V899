// ===========================================================================
//  KYECFTP/MiniFtpEngine.cpp
//  Implementation of the Nmftp::TNMFTP FTP engine shim.
//  See MiniFtpEngine.h for the full provenance / scoped-API-surface /
//  Sim-Real policy / design-decision notes -- this file only implements what
//  is documented there.
//
//  AI(W5-Final-KYECFTP) 20260711: new file.
// ===========================================================================
#include "KYECFTP/MiniFtpEngine.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#if defined(_WIN32)
#  include <windows.h>
#endif

namespace Nmftp {

// ===========================================================================
//  Reply-code classification / command framing / PASV parsing
//  (free functions, exposed in the header for isolated unit testing).
// ===========================================================================
TFtpReplyClass ClassifyFtpReply(int Code)
{
    if (Code < 100 || Code > 599)
        return rcMalformed;

    switch (Code / 100)
    {
        case 1: return rcPreliminary;
        case 2: return rcSuccess;
        case 3: return rcIntermediate;
        case 4: return rcTransientFailure;
        case 5: return rcPermanentFailure;
        default: return rcMalformed;
    }
}

AnsiString BuildFtpCommandLine(const char* Verb, const AnsiString& Arg)
{
    AnsiString line(Verb ? Verb : "");
    if (Arg.Length() > 0)
    {
        line += " ";
        line += Arg;
    }
    line += "\r\n";
    return line;
}

bool ParsePasvReply(const AnsiString& ReplyLine, AnsiString& OutIp, int& OutPort)
{
    const std::string& s = ReplyLine.str();

    for (size_t i = 0; i < s.size(); ++i)
    {
        size_t p = i;
        int vals[6];
        bool ok = true;

        for (int k = 0; k < 6 && ok; ++k)
        {
            size_t start = p;
            while (p < s.size() && std::isdigit(static_cast<unsigned char>(s[p])))
                ++p;
            if (p == start) { ok = false; break; }

            vals[k] = std::atoi(s.substr(start, p - start).c_str());

            if (k < 5)
            {
                if (p < s.size() && s[p] == ',')
                    ++p;
                else
                    { ok = false; break; }
            }
        }

        if (ok)
        {
            char buf[64];
            std::snprintf(buf, sizeof(buf), "%d.%d.%d.%d", vals[0], vals[1], vals[2], vals[3]);
            OutIp = buf;
            OutPort = vals[4] * 256 + vals[5];
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
//  Small local helpers (not exposed -- internal wire plumbing only).
// ---------------------------------------------------------------------------
namespace {

int ParseReplyCode(const AnsiString& Line)
{
    const std::string& s = Line.str();
    if (s.size() < 3)
        return 0;
    if (!std::isdigit(static_cast<unsigned char>(s[0])) ||
        !std::isdigit(static_cast<unsigned char>(s[1])) ||
        !std::isdigit(static_cast<unsigned char>(s[2])))
        return 0;
    return (s[0] - '0') * 100 + (s[1] - '0') * 10 + (s[2] - '0');
}

const char* FtpTypeVerb(TFTPTransferMode Mode_)
{
    switch (Mode_)
    {
        case MODE_ASCII: return "A";
        case MODE_IMAGE: return "I";
        case MODE_BYTE:  return "L 8";   // judgment call -- see file banner
        default:         return "I";
    }
}

bool ReadWholeFile(const AnsiString& Path, std::vector<char>& Out)
{
    Out.clear();
    std::FILE* f = std::fopen(Path.c_str(), "rb");
    if (!f)
        return false;

    std::fseek(f, 0, SEEK_END);
    long sz = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);

    if (sz > 0)
    {
        Out.resize(static_cast<size_t>(sz));
        size_t got = std::fread(&Out[0], 1, static_cast<size_t>(sz), f);
        Out.resize(got);
    }
    std::fclose(f);
    return true;
}

bool WriteWholeFile(const AnsiString& Path, const std::vector<char>& Data)
{
    std::FILE* f = std::fopen(Path.c_str(), "wb");
    if (!f)
        return false;
    if (!Data.empty())
        std::fwrite(&Data[0], 1, Data.size(), f);
    std::fclose(f);
    return true;
}

} // anonymous namespace

// ===========================================================================
//  TNMFTP::Impl
// ===========================================================================
struct TNMFTP::Impl {
    Scktcomp::TClientSocket* pControl;
    Scktcomp::TClientSocket* pData;       // NULL until the first transfer; then
                                          // persists (Active=false between
                                          // transfers) so a test's
                                          // DebugDataSocket() can inspect the
                                          // last transfer after the call
                                          // returns -- see CloseDataChannel_.
    TFTPTransferMode currentMode;
    std::string ctrlBuf;                  // leftover partial control-channel
                                          // bytes between ReadOneCtrlLine_ calls
    TNMFTP::TFtpSimServerHook simHook;

    // Transport-level (ClientSocket) connect outcome, captured by the
    // lambdas wired onto pControl in the ctor below -- see Connect().
    bool bTransportOk;
    int  lastTransportErr;

    Impl()
        : pControl(0)
        , pData(0)
        , currentMode(MODE_ASCII)
        , bTransportOk(false)
        , lastTransportErr(0)
    {
    }
};

// ===========================================================================
//  Construction / destruction
// ===========================================================================
TNMFTP::TNMFTP(vclcompat::TComponent* AOwner)
    : vclcompat::TComponent(AOwner)
    , Host()
    , UserID()
    , Password()
    , Port(21)              // evidenced default -- see file banner
    , Vendor(NMOS_AUTO)
    , TimeOut(30000)
    , Passive(true)
    , ParseList(false)
    , Name()
    , Connected(false)
    , LastErrorNo(0)
    , CurrentDir(this)
    , OnListItem()
    , OnSuccess()
    , OnFailure()
    , OnError()
    , OnStatus()
    , OnConnectionFailed()
    , OnTransactionStart()
    , OnTransactionStop()
    , OnAuthenticationFailed()
    , OnUnSupportedFunction()
    , OnConnect()
    , OnDisconnect()
    , pImpl_(new Impl())
{
    pImpl_->pControl = new Scktcomp::TClientSocket(this);

    Impl* self = pImpl_;
    pImpl_->pControl->OnConnect =
        [self](TObject*, Scktcomp::TCustomWinSocket*) { self->bTransportOk = true; };
    pImpl_->pControl->OnError =
        [self](TObject*, Scktcomp::TCustomWinSocket*, Scktcomp::TErrorEvent, int& err)
        {
            self->bTransportOk = false;
            self->lastTransportErr = err;
        };
}

TNMFTP::~TNMFTP()
{
    delete pImpl_->pData;
    delete pImpl_->pControl;
    delete pImpl_;
}

// ===========================================================================
//  SIM / test surface
// ===========================================================================
void TNMFTP::SetSimMode(bool bSim)
{
    pImpl_->pControl->SetSimMode(bSim);
    if (pImpl_->pData)
        pImpl_->pData->SetSimMode(bSim);
}

bool TNMFTP::IsSimMode() const { return pImpl_->pControl->IsSimMode(); }

void TNMFTP::SetSimServerHook(TFtpSimServerHook Hook) { pImpl_->simHook = Hook; }

Scktcomp::TClientSocket* TNMFTP::DebugControlSocket() const { return pImpl_->pControl; }
Scktcomp::TClientSocket* TNMFTP::DebugDataSocket()    const { return pImpl_->pData; }

void TNMFTP::FireSimHook_(const char* Channel, const AnsiString& RawLine)
{
    if (pImpl_->simHook)
        pImpl_->simHook(Channel, RawLine);
}

// ===========================================================================
//  Wire plumbing (private)
// ===========================================================================
void TNMFTP::SendCtrlCmd_(const char* Verb, const AnsiString& Arg)
{
    AnsiString line(Verb ? Verb : "");
    if (Arg.Length() > 0)
    {
        line += " ";
        line += Arg;
    }
    AnsiString wire = line;
    wire += "\r\n";

    pImpl_->pControl->Socket->SendBuf(
        const_cast<char*>(wire.c_str()), wire.Length());

    FireSimHook_("CTRL", line);
}

bool TNMFTP::ReadOneCtrlLine_(AnsiString& OutLine, int& OutCode)
{
    std::string& buf = pImpl_->ctrlBuf;
    int elapsedMs = 0;

    for (;;)
    {
        size_t nl = buf.find('\n');
        if (nl != std::string::npos)
        {
            std::string one = buf.substr(0, nl);
            if (!one.empty() && one[one.size() - 1] == '\r')
                one.erase(one.size() - 1);
            buf.erase(0, nl + 1);

            OutLine = AnsiString(one.c_str());
            OutCode = ParseReplyCode(OutLine);
            return true;
        }

        char chunk[1024];
        int n = pImpl_->pControl->Socket->ReceiveBuf(chunk, sizeof(chunk));
        if (n > 0)
        {
            buf.append(chunk, static_cast<size_t>(n));
            continue;
        }

        if (pImpl_->pControl->IsSimMode())
            return false;   // SIM: nothing queued, nothing more will ever
                            // arrive without another hook call -- give up now.

#if defined(_WIN32)
        const int kPollMs = 10;
        ::Sleep(kPollMs);
        elapsedMs += kPollMs;
        if (elapsedMs >= TimeOut)
            return false;
#else
        return false;
#endif
    }
}

bool TNMFTP::DoPasv_(AnsiString& OutIp, int& OutPort)
{
    SendCtrlCmd_("PASV");

    AnsiString line;
    int code = 0;
    if (!ReadOneCtrlLine_(line, code))
    {
        LastErrorNo = -1;
        return false;
    }
    if (ClassifyFtpReply(code) != rcSuccess)
    {
        LastErrorNo = code;
        FireError_(code, line);
        return false;
    }
    if (!ParsePasvReply(line, OutIp, OutPort))
    {
        LastErrorNo = code;
        FireError_(code, AnsiString("PASV reply could not be parsed: ") + line);
        return false;
    }
    return true;
}

bool TNMFTP::OpenDataChannel_(const AnsiString& Ip, int DataPort)
{
    delete pImpl_->pData;   // discard the PREVIOUS transfer's data channel (no-op if NULL)

    pImpl_->pData = new Scktcomp::TClientSocket(this);
    pImpl_->pData->SetSimMode(pImpl_->pControl->IsSimMode());
    pImpl_->pData->Address = Ip;
    pImpl_->pData->Port = DataPort;
    pImpl_->pData->Active = true;

    bool ok = pImpl_->pData->IsActiveNow();
    if (ok)
        FireSimHook_("DATA-OPEN");
    return ok;
}

void TNMFTP::CloseDataChannel_()
{
    if (!pImpl_->pData)
        return;

    // Fires BEFORE the data socket is deactivated so a test's hook can push
    // the control channel's still-pending completion reply right here (see
    // the "SIM / TEST SURFACE" note in the header) or read back what an
    // Upload just sent via DebugDataSocket()->Socket->SimTxBuffer().
    FireSimHook_("DATA-CLOSE");

    pImpl_->pData->Active = false;
    // Deliberately NOT deleted here -- kept alive (Active=false) so
    // DebugDataSocket() remains valid after the owning call returns; the
    // NEXT OpenDataChannel_() call (or ~TNMFTP()) frees it.
}

void TNMFTP::FireError_(int Code, const AnsiString& Msg)
{
    if (OnError)
        OnError(this, static_cast<vclcompat::Word>(Code), Msg);
}

void TNMFTP::FireCmdFailure_(TCmdType Cmd, int Code)
{
    // 502 "Command not implemented" (RFC 959 4.2.2) is the standard signal
    // for "this server doesn't support that command at all" -- distinct
    // from "understood but failed" (every other non-2xx code). See the
    // Mode()/classification design note in the header.
    if (Code == 502)
    {
        if (OnUnSupportedFunction)
            OnUnSupportedFunction(Cmd);
        return;
    }

    bool handled = false;
    if (OnFailure)
        OnFailure(handled, Cmd);
}

void TNMFTP::SimpleCommand_(const char* Verb, const AnsiString& Arg, TCmdType Cmd)
{
    if (!Connected)
    {
        FireCmdFailure_(Cmd);
        return;
    }

    SendCtrlCmd_(Verb, Arg);

    AnsiString line;
    int code = 0;
    if (!ReadOneCtrlLine_(line, code))
    {
        LastErrorNo = -1;
        FireCmdFailure_(Cmd);
        return;
    }

    LastErrorNo = code;
    if (ClassifyFtpReply(code) == rcSuccess)
    {
        if (OnSuccess)
            OnSuccess(Cmd);
    }
    else
    {
        FireError_(code, line);
        FireCmdFailure_(Cmd, code);
    }
}

void TNMFTP::ReadAllDataBytes_(std::vector<char>& Out)
{
    Out.clear();
    if (!pImpl_->pData)
        return;

    if (pImpl_->pData->IsSimMode())
    {
        // SIM: the hook already pushed the COMPLETE payload synchronously at
        // "DATA-OPEN" time (see header) -- a single drain-to-empty loop is
        // exact, no timing ambiguity possible.
        char chunk[4096];
        int n;
        while ((n = pImpl_->pData->Socket->ReceiveBuf(chunk, sizeof(chunk))) > 0)
            Out.insert(Out.end(), chunk, chunk + n);
        return;
    }

#if defined(_WIN32)
    // REAL mode: no reliable peer-close signal is available (see the
    // "REAL-mode data-channel EOF detection gap" note in the header) -- use
    // a byte-idle heuristic bounded by TimeOut overall.
    const DWORD kIdleMs = 300;
    DWORD startTick = ::GetTickCount();
    DWORD lastByteTick = startTick;
    char chunk[4096];
    for (;;)
    {
        int n = pImpl_->pData->Socket->ReceiveBuf(chunk, sizeof(chunk));
        if (n > 0)
        {
            Out.insert(Out.end(), chunk, chunk + n);
            lastByteTick = ::GetTickCount();
            continue;
        }

        DWORD now = ::GetTickCount();
        if (now - lastByteTick > kIdleMs)
            break;
        if (now - startTick > static_cast<DWORD>(TimeOut))
            break;
        ::Sleep(5);
    }
#endif
}

void TNMFTP::WriteAllDataToChannel_(const std::vector<char>& Bytes)
{
    if (!pImpl_->pData || Bytes.empty())
        return;

    const size_t chunkSize = 4096;
    size_t off = 0;
    while (off < Bytes.size())
    {
        size_t n = Bytes.size() - off < chunkSize ? Bytes.size() - off : chunkSize;
        pImpl_->pData->Socket->SendBuf(
            const_cast<char*>(&Bytes[off]), static_cast<int>(n));
        off += n;
    }
}

// ===========================================================================
//  Connect / Disconnect
// ===========================================================================
void TNMFTP::Connect()
{
    if (Connected)
        return;   // idempotent

    pImpl_->ctrlBuf.clear();
    LastErrorNo = 0;

    pImpl_->pControl->Address = Host;
    pImpl_->pControl->Port = Port;
    pImpl_->bTransportOk = false;
    pImpl_->lastTransportErr = 0;

    pImpl_->pControl->Active = true;   // SIM: succeeds synchronously; REAL:
                                        // connect()s now (ClientSocket.cpp)

    if (!pImpl_->bTransportOk)
    {
        LastErrorNo = pImpl_->lastTransportErr;
        if (OnConnectionFailed)
            OnConnectionFailed(this);
        return;
    }

    // The server speaks first (the "220" banner is unsolicited, not a reply
    // to any client command) -- give a SIM server hook a synchronous chance
    // to push it now, via the same hook mechanism as every other reply (see
    // the header's "SIM / TEST SURFACE" section, "CTRL-CONNECT" tag).
    FireSimHook_("CTRL-CONNECT");

    AnsiString line;
    int code = 0;

    // "220" welcome banner.
    if (!ReadOneCtrlLine_(line, code) || ClassifyFtpReply(code) != rcSuccess)
    {
        LastErrorNo = code;
        if (code != 0)
            FireError_(code, line);
        if (OnConnectionFailed)
            OnConnectionFailed(this);
        pImpl_->pControl->Active = false;
        return;
    }

    // USER <id>
    SendCtrlCmd_("USER", UserID);
    if (!ReadOneCtrlLine_(line, code))
    {
        LastErrorNo = -1;
        if (OnConnectionFailed)
            OnConnectionFailed(this);
        pImpl_->pControl->Active = false;
        return;
    }

    // 331 => need a password; anything else falls through to the
    // success/failure check below (some servers accept USER alone, 230).
    if (ClassifyFtpReply(code) == rcIntermediate)
    {
        SendCtrlCmd_("PASS", Password);
        if (!ReadOneCtrlLine_(line, code))
        {
            LastErrorNo = -1;
            if (OnConnectionFailed)
                OnConnectionFailed(this);
            pImpl_->pControl->Active = false;
            return;
        }
    }

    if (ClassifyFtpReply(code) != rcSuccess)
    {
        LastErrorNo = code;
        FireError_(code, line);
        bool handled = false;
        if (OnAuthenticationFailed)
            OnAuthenticationFailed(handled);
        pImpl_->pControl->Active = false;
        return;
    }

    Connected = true;
    LastErrorNo = 0;
    if (OnConnect)
        OnConnect(this);
}

void TNMFTP::Disconnect()
{
    CloseDataChannel_();

    bool wasConnected = Connected;
    pImpl_->pControl->Active = false;
    Connected = false;

    if (wasConnected && OnDisconnect)
        OnDisconnect(this);
}

void TNMFTP::RequestCloseSocket()
{
    // Golden always pairs this with (and usually right after) Abort()/
    // Disconnect() as final teardown before `delete`ing the TNMFTP object --
    // no golden call site distinguishes its effect from Disconnect(), so
    // this engine treats them as equivalent (see file banner).
    Disconnect();
}

void TNMFTP::Abort()
{
    if (pImpl_->pData)
        CloseDataChannel_();

    if (Connected)
    {
        // Best-effort, reply discarded -- see the Async-vs-sync design note:
        // every golden call site invokes Abort() as unconditional cleanup
        // AFTER a synchronous transfer call has already returned, so there
        // is realistically nothing in flight to abort.
        SendCtrlCmd_("ABOR");
        AnsiString line;
        int code = 0;
        ReadOneCtrlLine_(line, code);
    }
}

// ===========================================================================
//  Simple single-reply commands
// ===========================================================================
void TNMFTP::ChangeDir(const AnsiString& Dir)      { SimpleCommand_("CWD", Dir, cmdChangeDir); }
void TNMFTP::MakeDirectory(const AnsiString& Dir)  { SimpleCommand_("MKD", Dir, cmdMakeDir); }
void TNMFTP::Delete(const AnsiString& RemoteFile)  { SimpleCommand_("DELE", RemoteFile, cmdDelete); }

void TNMFTP::Mode(TFTPTransferMode NewMode)
{
    pImpl_->currentMode = NewMode;
    if (!Connected)
        return;

    SendCtrlCmd_("TYPE", AnsiString(FtpTypeVerb(NewMode)));

    // Read+discard: keeps control-channel line-framing in sync for the NEXT
    // command. No cmdType classification exists for Mode() in golden's
    // exhaustive TCmdType switch -- see the header's design-decision note.
    AnsiString line;
    int code = 0;
    ReadOneCtrlLine_(line, code);
    LastErrorNo = code;
}

AnsiString TNMFTP::QueryCurrentDir_()
{
    if (!Connected)
    {
        bool handled = false;
        if (OnFailure)
            OnFailure(handled, cmdCurrentDir);
        return AnsiString("");
    }

    SendCtrlCmd_("PWD");

    AnsiString line;
    int code = 0;
    if (!ReadOneCtrlLine_(line, code) || ClassifyFtpReply(code) != rcSuccess)
    {
        LastErrorNo = code;
        FireError_(code, line);
        FireCmdFailure_(cmdCurrentDir, code);
        return AnsiString("");
    }

    // "257 \"<path>\" [comment]" (RFC 959 4.1.3) -- extract the quoted path.
    const std::string& s = line.str();
    size_t q1 = s.find('"');
    size_t q2 = (q1 == std::string::npos) ? std::string::npos : s.find('"', q1 + 1);

    AnsiString result =
        (q1 != std::string::npos && q2 != std::string::npos)
            ? AnsiString(s.substr(q1 + 1, q2 - q1 - 1).c_str())
            : AnsiString("");

    LastErrorNo = 0;
    if (OnSuccess)
        OnSuccess(cmdCurrentDir);
    return result;
}

TNMFTP::CurrentDirProxy::operator AnsiString() const
{
    return owner_->QueryCurrentDir_();
}

// ===========================================================================
//  Transfers
// ===========================================================================
void TNMFTP::Download(const AnsiString& RemoteFile, const AnsiString& LocalFile)
{
    if (!Connected)
    {
        FireCmdFailure_(cmdDownload);
        return;
    }

    AnsiString ip;
    int dataPort = 0;
    if (!DoPasv_(ip, dataPort))
    {
        FireCmdFailure_(cmdDownload);
        return;
    }
    if (!OpenDataChannel_(ip, dataPort))
    {
        FireCmdFailure_(cmdDownload);
        return;
    }

    SendCtrlCmd_("RETR", RemoteFile);

    AnsiString line;
    int code = 0;
    if (!ReadOneCtrlLine_(line, code) || ClassifyFtpReply(code) != rcPreliminary)
    {
        LastErrorNo = code;
        FireError_(code, line);
        CloseDataChannel_();
        FireCmdFailure_(cmdDownload, code);
        return;
    }

    if (OnTransactionStart)
        OnTransactionStart(this);

    std::vector<char> bytes;
    ReadAllDataBytes_(bytes);
    WriteWholeFile(LocalFile, bytes);

    CloseDataChannel_();
    if (OnTransactionStop)
        OnTransactionStop(this);

    if (!ReadOneCtrlLine_(line, code) || ClassifyFtpReply(code) != rcSuccess)
    {
        LastErrorNo = code;
        FireError_(code, line);
        FireCmdFailure_(cmdDownload, code);
        return;
    }

    LastErrorNo = 0;
    if (OnSuccess)
        OnSuccess(cmdDownload);
}

void TNMFTP::Upload(const AnsiString& LocalFile, const AnsiString& RemoteFile)
{
    if (!Connected)
    {
        FireCmdFailure_(cmdUpload);
        return;
    }

    std::vector<char> bytes;
    // Missing local file -> proceed with zero bytes rather than inventing a
    // pre-flight failure golden never checks for (golden calls Upload()
    // without a FileExists guard at every site) -- the server-side STOR
    // reply drives success/failure either way.
    ReadWholeFile(LocalFile, bytes);

    AnsiString ip;
    int dataPort = 0;
    if (!DoPasv_(ip, dataPort))
    {
        FireCmdFailure_(cmdUpload);
        return;
    }
    if (!OpenDataChannel_(ip, dataPort))
    {
        FireCmdFailure_(cmdUpload);
        return;
    }

    SendCtrlCmd_("STOR", RemoteFile);

    AnsiString line;
    int code = 0;
    if (!ReadOneCtrlLine_(line, code) || ClassifyFtpReply(code) != rcPreliminary)
    {
        LastErrorNo = code;
        FireError_(code, line);
        CloseDataChannel_();
        FireCmdFailure_(cmdUpload, code);
        return;
    }

    if (OnTransactionStart)
        OnTransactionStart(this);

    WriteAllDataToChannel_(bytes);

    CloseDataChannel_();
    if (OnTransactionStop)
        OnTransactionStop(this);

    if (!ReadOneCtrlLine_(line, code) || ClassifyFtpReply(code) != rcSuccess)
    {
        LastErrorNo = code;
        FireError_(code, line);
        FireCmdFailure_(cmdUpload, code);
        return;
    }

    LastErrorNo = 0;
    if (OnSuccess)
        OnSuccess(cmdUpload);
}

void TNMFTP::Nlist()
{
    if (!Connected)
    {
        FireCmdFailure_(cmdNList);
        return;
    }

    AnsiString ip;
    int dataPort = 0;
    if (!DoPasv_(ip, dataPort))
    {
        FireCmdFailure_(cmdNList);
        return;
    }
    if (!OpenDataChannel_(ip, dataPort))
    {
        FireCmdFailure_(cmdNList);
        return;
    }

    SendCtrlCmd_("NLST");

    AnsiString line;
    int code = 0;
    if (!ReadOneCtrlLine_(line, code) || ClassifyFtpReply(code) != rcPreliminary)
    {
        LastErrorNo = code;
        FireError_(code, line);
        CloseDataChannel_();
        FireCmdFailure_(cmdNList, code);
        return;
    }

    if (OnTransactionStart)
        OnTransactionStart(this);

    std::vector<char> bytes;
    ReadAllDataBytes_(bytes);

    CloseDataChannel_();
    if (OnTransactionStop)
        OnTransactionStop(this);

    if (!ReadOneCtrlLine_(line, code) || ClassifyFtpReply(code) != rcSuccess)
    {
        LastErrorNo = code;
        FireError_(code, line);
        FireCmdFailure_(cmdNList, code);
        return;
    }

    // Split the drained bytes into bare, newline-delimited filenames and
    // fire OnListItem for each ONE (matches golden's NMFTP1ListItem
    // accumulation, FTPClient.cpp:1799) BEFORE the final OnSuccess -- an
    // empty listing (zero lines, 2xx completion) is still a SUCCESS, see
    // the header's Nlist wire-sequence note.
    std::string s(bytes.empty() ? std::string() : std::string(bytes.begin(), bytes.end()));
    size_t pos = 0;
    while (pos < s.size())
    {
        size_t nl = s.find('\n', pos);
        std::string one = (nl == std::string::npos) ? s.substr(pos) : s.substr(pos, nl - pos);
        if (!one.empty() && one[one.size() - 1] == '\r')
            one.erase(one.size() - 1);
        if (!one.empty() && OnListItem)
            OnListItem(AnsiString(one.c_str()));
        if (nl == std::string::npos)
            break;
        pos = nl + 1;
    }

    LastErrorNo = 0;
    if (OnSuccess)
        OnSuccess(cmdNList);
}

} // namespace Nmftp
