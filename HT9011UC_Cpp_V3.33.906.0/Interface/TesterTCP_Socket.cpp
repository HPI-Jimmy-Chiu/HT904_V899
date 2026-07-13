//---------------------------------------------------------------------------
//  Interface/TesterTCP_Socket.cpp -- see TesterTCP_Socket.h for full scope /
//  provenance / discrepancy notes.
//---------------------------------------------------------------------------
#include "Interface/TesterTCP_Socket.h"

#include <cstdio>

//===========================================================================
//  Gated dependencies (TU-local, internal linkage -- cannot collide with any
//  other translation unit). Each stands in for a genuinely not-yet-available
//  golden dependency; cited at every call site below.
//===========================================================================
namespace {

// ---------------------------------------------------------------------------
// Gated_WriteDataToFile -- golden common.h:255-256 declares
//   `extern void WriteDataToFile(char*, char*, bool=false);` (and an
//   AnsiString overload that just forwards to it) inside common.h's own
//   `#if 0 // TODO(wave-file)` block (common.h:295-302) -- the DECLARATION
//   itself is gated, not merely the body, so this unit cannot even forward-
//   declare the real name without an ODR/redeclaration hazard once common.h
//   is ungated. Faithfully re-implemented locally instead of stubbed as a
//   no-op: golden's real body (common.cpp:1607-1622, read directly from the
//   golden tree for fidelity) is a trivial fopen(bOverWrite?"w":"a") +
//   fputs(data)+fputs("\n") -- self-contained, no other module dependency,
//   so there is no reason to degrade AddTCPIPCommunicationLog's whole
//   raison d'etre (durable per-hour TCP comms log files) to a silent no-op.
// ---------------------------------------------------------------------------
void Gated_WriteDataToFile(const char* cFilePath, const char* cData, bool bOverWrite = false)
{
    FILE* pFile = bOverWrite ? std::fopen(cFilePath, "w") : std::fopen(cFilePath, "a");
    if (pFile != 0)
    {
        std::fputs(cData, pFile);
        std::fputs("\n", pFile);
        std::fclose(pFile);
    }
}

// ---------------------------------------------------------------------------
// Gated_MyForceDirectories -- golden common.h:262 (also inside the same
// #if 0 TODO(wave-file) block as above). Faithfully re-implemented from the
// real golden body (common.cpp:1667-1717, read directly from the golden
// tree) reusing already-ACTIVE vclcompat SysUtils primitives (FileGetAttr /
// ExtractFileExt / ExtractFilePath / DirectoryExists / ForceDirectories) and
// the already-ACTIVE canary_support.h RecordProcess/ShowMyMessage -- this is
// a genuine faithful port, not a stub. One golden branch is simplified: the
// real body's `catch(Exception& e)` (a VCL Exception subtype) is merged into
// the generic `catch(...)` below, since vclcompat has no Exception shim and
// none of ForceDirectories/DirectoryExists throw C++ exceptions today anyway
// (grepped) -- the try/catch is kept for defensive parity with golden (same
// harmless-dead-code posture already documented for ClientSocket.h's Open()
// try/catch wrap).
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
// Gated_LogClientSocketExceptionError -- golden Public/WinSocketErrorCode.cpp
// (see WinSocketErrorCode.h: "DEFERRED (W-UI wave): couples to VCL
// TClientSocket->Name/Address/Port and MyDBIProcess. Omitted from this
// translation unit."). Conservative no-op: golden's own role here is
// best-effort DIAGNOSTIC logging of an already-caught exception (both call
// sites below are inside `catch(...)` blocks that have already done the only
// behaviorally-load-bearing thing, e.g. ClientSocket_TCPIP->Close() in the
// Error handler) -- dropping the extra log line is behavior-neutral for the
// connect/error control flow. TObject* covers BOTH real call shapes (golden
// passes a TClientSocket* at one site and the raw event Sender TObject* at
// the other) since Scktcomp::TClientSocket : TComponent : TObject.
// ---------------------------------------------------------------------------
void Gated_LogClientSocketExceptionError(TObject* /*Sender*/, const AnsiString& /*Context*/)
{
}

} // anonymous namespace

// ---------------------------------------------------------------------------
// AI(W5-Final-Integrate) 20260711: golden VCL Graphics.hpp colour constants
// (clGreen/clRed), needed by the fLotInfo->labTCPIPStatus->Color writes below
// (FormsFacade wiring lifted this pass -- see file-head DISCREPANCY note).
// Real TColor/clGreen/clRed are gated `#if 0` in cmydef.cpp (see
// MIGRATION_ROADMAP.md's "W0-TAIL" DEFERRED entry, not yet linkable) -- same
// TU-local-const idiom already established by acatchtray_shims.h's
// clWhite/clYellow (real VCL colour values, BGR-packed TColor).
// ---------------------------------------------------------------------------
static const TColor clGreen = 0x00008000;   // golden Graphics.hpp clGreen
static const TColor clRed   = 0x000000FF;   // golden Graphics.hpp clRed

//===========================================================================
//  TesterTCPSocketState / singleton instance
//===========================================================================
TesterTCPSocketState::TesterTCPSocketState()
    : bEnableTCPIPChannelConnect(true)   // golden ctor TesterTCP.cpp:63
    , bConnectOK(false)
    , bConnect(false)
    , bTCPError(false)                   // golden ctor TesterTCP.cpp:65
    , ErrorMessage()
    , sTCPIPRecevieData()
    , Tag(0)
    , SocketTCPIPReceiveList(new TStringList())   // golden ctor TesterTCP.cpp:61 (`new TStringList`)
    , mmTCPIPCommLog()
    , bTimerTCPIPConnectEnabled(false)
{
    SocketTCPIPReceiveList->Clear();     // golden ctor TesterTCP.cpp:66
}

TesterTCPSocketState::~TesterTCPSocketState()
{
    SocketTCPIPReceiveList->Clear();     // golden FormDestroy TesterTCP.cpp:113
    delete SocketTCPIPReceiveList;
}

TesterTCPSocketState TesterTCPSocket;

Scktcomp::TClientSocket* TesterTCPSocket_ClientSocket = new Scktcomp::TClientSocket(0);

void TesterTCPSocket_Init()
{
    // AI(W5-Final-TesterTCPSocket) 20260711: reproduces the golden .dfm's
    // declarative OnConnect/OnDisconnect/OnError/OnRead bindings (see the
    // header's JUDGMENT CALL note). Idempotent -- re-assigning the same
    // std::function targets repeatedly is harmless.
    TesterTCPSocket_ClientSocket->OnConnect    = TesterTCPSocket_OnConnect;
    TesterTCPSocket_ClientSocket->OnDisconnect = TesterTCPSocket_OnDisconnect;
    TesterTCPSocket_ClientSocket->OnError      = TesterTCPSocket_OnError;
    TesterTCPSocket_ClientSocket->OnRead       = TesterTCPSocket_OnRead;
}

//===========================================================================
//  Event handlers
//===========================================================================
// golden TfTesterTCP::ClientSocket_TCPIPConnect, TesterTCP.cpp:166-174.
void TesterTCPSocket_OnConnect(TObject* /*Sender*/, TCustomWinSocket* /*Socket*/)
{
    TesterTCPSocket_AddTCPIPCommunicationLog(0, "Connect to tester OK");

    // AI(W5-Final-Integrate) 20260711: fLotInfo->labTCPIPStatus (golden
    // uLotInfo.h, TLabel* -- Caption/Color) -- TfLotInfo::labTCPIPStatus added
    // to FormsFacade.h this integrate pass; gate lifted (golden TesterTCP.cpp:170-171).
    fLotInfo->labTCPIPStatus->Caption = "ON-LINE";
    fLotInfo->labTCPIPStatus->Color   = clGreen;

    TesterTCPSocket.bConnectOK = true;
    TesterTCPSocket.bConnect   = false;
}

// golden TfTesterTCP::ClientSocket_TCPIPDisconnect, TesterTCP.cpp:176-183.
void TesterTCPSocket_OnDisconnect(TObject* /*Sender*/, TCustomWinSocket* /*Socket*/)
{
    TesterTCPSocket_AddTCPIPCommunicationLog(0, "Disconnect from tester.");

    // AI(W5-Final-Integrate) 20260711: gate lifted (golden TesterTCP.cpp:180-181).
    fLotInfo->labTCPIPStatus->Caption = "OFF-LINE";
    fLotInfo->labTCPIPStatus->Color   = clRed;

    TesterTCPSocket.bConnectOK = false;
}

// golden TfTesterTCP::ClientSocket_TCPIPError, TesterTCP.cpp:615-633.
void TesterTCPSocket_OnError(TObject* Sender, TCustomWinSocket* /*Socket*/,
                             TErrorEvent /*ErrorEvent*/, int& ErrorCode)
{
    TesterTCPSocket.ErrorMessage = GetErrorMsg(Sender, ErrorCode);
    TesterTCPSocket_AddTCPIPCommunicationLog(0, AnsiString("Tester_TCPIP_Error : ") + TesterTCPSocket.ErrorMessage);
    TesterTCPSocket.bTCPError = true;

    // AI(W5-Final-Integrate) 20260711: gate lifted (golden TesterTCP.cpp:621-622).
    fLotInfo->labTCPIPStatus->Caption = "ERROR";
    fLotInfo->labTCPIPStatus->Color   = clRed;

    try
    {
        TesterTCPSocket_ClientSocket->Close();
    }
    catch (...)
    {
        Gated_LogClientSocketExceptionError(Sender, "TCP/IP Tester Error");
    }
    ErrorCode = 0;
}

// golden TfTesterTCP::ClientSocket_TCPIPRead, TesterTCP.cpp:322-345.
void TesterTCPSocket_OnRead(TObject* /*Sender*/, TCustomWinSocket* Socket)
{
    if (InitialOK == false)
        return;

    AnsiString EthernetBuffer;
    int iBufferLenght = Socket->ReceiveLength();
    if (iBufferLenght > 0)
    {
        EthernetBuffer = Socket->ReceiveText();
        TesterTCPSocket.SocketTCPIPReceiveList->Add(EthernetBuffer);

        EthernetBuffer = StringReplace(EthernetBuffer, "\r\n", "", TReplaceFlags() << rfReplaceAll);
        TesterTCPSocket.sTCPIPRecevieData = TesterTCPSocket.SocketTCPIPReceiveList->Strings[0];
        AnsiString Log;
        Log.sprintf("Recv, %s", EthernetBuffer);
        TesterTCPSocket_AddTCPIPCommunicationLog(TesterTCPSocket.Tag, Log);
    }
}

//===========================================================================
//  Timer / command / log
//===========================================================================
// golden TfTesterTCP::TimerTCPIPConnectTimer, TesterTCP.cpp:185-239.
void TesterTCPSocket_TimerTCPIPConnectTimer()
{
    static bool bTimerRun = false;
    if (InitialOK == false || bTimerRun == true || LastSet.iTester == OFF_LINE)
        return;
    bTimerRun = true;

    static int Count1 = 20;

    if (TesterTCPSocket.bTCPError)
    {
        ShowMyMessage("TCP/IP error for tester!", TesterTCPSocket.ErrorMessage);
        TesterTCPSocket.bTCPError = false;
    }

    if (TesterTCPSocket_ClientSocket->Active &&
        TesterTCPSocket_ClientSocket->Address == TestIF_File.asTester_Address &&
        TesterTCPSocket_ClientSocket->Port == TestIF_File.iTester_Port)
    {
        // same IP/Port as before -- do not reconnect (golden's own empty-branch idiom)
    }
    else
    {
        if (TesterTCPSocket_ClientSocket->Active)
        {
            TesterTCPSocket_ClientSocket->Close();
            TesterTCPSocket_SendTCPIPCommand(0, "Disconnecting from tester.", "");
            Count1 = 0;
        }
    }

    if (TesterTCPSocket_ClientSocket->Active == false && TesterTCPSocket.bEnableTCPIPChannelConnect == true)
    {
        TesterTCPSocket.bTCPError = false;
        Count1++;
        if (Count1 > 30)
        {
            TesterTCPSocket_ClientSocket->Address = TestIF_File.asTester_Address;
            TesterTCPSocket_ClientSocket->Port    = TestIF_File.iTester_Port;
            TesterTCPSocket_SendTCPIPCommand(0, "ClientSocket connecting..", "");
            try
            {
                TesterTCPSocket_ClientSocket->Open();
            }
            catch (...)
            {
                Gated_LogClientSocketExceptionError(TesterTCPSocket_ClientSocket, "TCP/IP Tester Connect");
            }
            TesterTCPSocket_SendTCPIPCommand(0, "ClientSocket Open", "");
            Count1 = 0;
        }
    }
    bTimerRun = false;
}

// golden TfTesterTCP::SendTCPIPCommand(int,AnsiString,AnsiString=""), TesterTCP.cpp:241-274.
void TesterTCPSocket_SendTCPIPCommand(int index, AnsiString Msg, AnsiString Msg2)
{
    if (InitialOK == false)   // golden also guards `mmTCPIPCommLog==NULL`, always-false
        return;               // here since mmTCPIPCommLog is a plain value member, not a pointer

    AnsiString Log;
    GetTimeInfo();

    // AI(W5-Final-TesterTCPSocket) 20260711: FAITHFUL GOLDEN QUIRK, preserved
    // verbatim -- golden literally does `Msg2+=Msg2+"OffLine";`, i.e. it
    // DUPLICATES Msg2 (not merely appends "OffLine"): "foo" becomes
    // "foofooOffLine". This looks like a copy-paste bug (probably meant
    // `Msg2+="OffLine";`) but per this project's translation policy we do
    // not "fix" odd-looking golden logic -- reproduced exactly.
    if (LastSet.iTester == OFF_LINE)
    {
        Msg2 += Msg2 + "OffLine";
    }

    if (Msg2 == "")
        Log.sprintf("Action, %s", Msg);
    else
        Log.sprintf("Send, %s", Msg2);

    TesterTCPSocket_AddTCPIPCommunicationLog(index, Log);

    if (Msg2 == "")
    {
        // golden empty-branch idiom (nothing to send)
    }
    else
    {
        if (LastSet.iTester == OFF_LINE)
            return;

        if (TesterTCPSocket_ClientSocket->Active)
        {
            TesterTCPSocket_ClientSocket->Socket->SendText(Msg2 + "\r\n");
        }
    }
}

// golden TfTesterTCP::AddTCPIPCommunicationLog(int,AnsiString), TesterTCP.cpp:276-300.
void TesterTCPSocket_AddTCPIPCommunicationLog(int /*index*/, AnsiString Str)
{
    AnsiString Path, Name, Log;
    Name = "TCPIP";

    if (Str == " ")
        Log.sprintf(" ");
    else
        Log.sprintf("%04d-%02d-%02d,%02d:%02d:%02d.%03d,%s,%s",
                     SystemYear, SystemMonth, SystemDate,
                     SystemHour, SystemMin, SystemSec, SystemMSec,
                     Name, Str);

    if (TesterTCPSocket.mmTCPIPCommLog.Lines.Count > 2000)
    {
        TesterTCPSocket.mmTCPIPCommLog.Clear();
        // AI(W5-Final-Integrate) 20260711: gate lifted (golden TesterTCP.cpp:290) --
        // TfLotInfo::mmTesterLog added to FormsFacade.h this integrate pass
        // (reusing the existing TfMainMemo/TfMainMemoLines shape, same shape
        // fAGV->mmE84Log uses).
        fLotInfo->mmTesterLog->Clear();
    }

    TesterTCPSocket.mmTCPIPCommLog.Lines.Add(Log);
    // AI(W5-Final-Integrate) 20260711: gate lifted (golden TesterTCP.cpp:294).
    fLotInfo->mmTesterLog->Lines->Add(Log);

    Path.sprintf("%s\\%04d_%02d\\%04d_%02d_%02d", asTestTCPIPLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate);
    Gated_MyForceDirectories(Path);
    Path.sprintf("%s\\%04d_%02d\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asTestTCPIPLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate, SystemHour);
    Gated_WriteDataToFile(Path.c_str(), Log.c_str());
}

//===========================================================================
//  Click handlers
//===========================================================================
// golden TfTesterTCP::btTCPIP_TriggerClick, TesterTCP.cpp:302-307.
void TesterTCPSocket_btTCPIP_TriggerClick(AnsiString strSendCMD)
{
    TesterTCPSocket_SendTCPIPCommand(0, " ", strSendCMD);
}

// golden TfTesterTCP::btTCPIP_ConnectClick, TesterTCP.cpp:309-313.
void TesterTCPSocket_btTCPIP_ConnectClick()
{
    TesterTCPSocket.bTimerTCPIPConnectEnabled     = true;   // golden TimerTCPIPConnect->Enabled=true
    TesterTCPSocket.bEnableTCPIPChannelConnect    = true;
}

// golden TfTesterTCP::btTCPIP_DisconnectClick, TesterTCP.cpp:315-320.
void TesterTCPSocket_btTCPIP_DisconnectClick()
{
    TesterTCPSocket.bEnableTCPIPChannelConnect = false;
    TesterTCPSocket.bTimerTCPIPConnectEnabled  = false;     // golden TimerTCPIPConnect->Enabled=false
    TesterTCPSocket_ClientSocket->Close();
}
