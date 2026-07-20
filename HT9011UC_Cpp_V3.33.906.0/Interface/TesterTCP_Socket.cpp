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

// ---------------------------------------------------------------------------
// AI(W906-TesterTCPTimer) 20260720: golden TesterTCP.cpp:21-22 file-header
// externs for main.cpp-owned globals (main.cpp itself untranslated, W7).
// iBin production definition lives in atester_shims.cpp (ht9045_sm); bEcho in
// the same file. bExist is imported here for golden-line fidelity only (golden
// :22 externs it alongside bEcho even though TimerProcessTCPDataTimer/
// SimulateBin never read/write it -- reproduced verbatim, not used).
// ---------------------------------------------------------------------------
extern unsigned int iBin[4][8];
extern bool bEcho, bExist;

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
    , SimulateStart(false)               // golden ctor TesterTCP.cpp:64
    , bTimerProcessTCPDataEnabled(false) // golden .dfm:2948 Enabled=False
{
    SocketTCPIPReceiveList->Clear();     // golden ctor TesterTCP.cpp:66

    // AI(W906-TesterTCPTimer) 20260720: golden ctor TesterTCP.cpp:49-59 (the
    // timer/SimulateBin-relevant subset -- cbSimulateBin[i]->ItemIndex=0 is
    // already the TesterTCPSocket_Combo member ctor default, not repeated here).
    for (int i = 0; i < 32; i++)
    {
        plSite[i].Caption = "--";
        labOcr[i].Caption = "";
    }
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
//  Protocol-decode pump + Simulate tail
//===========================================================================
// golden comment TesterTCP.cpp:347: "SocketTCPIPReceiveList有可能命令會分兩段進來,
// 需要進一步處理" (translation: the receive list may get a command split across
// two arrivals, needs further handling) -- golden's OWN self-admission that
// split-command reassembly is NOT implemented (each queued entry is treated as
// one complete command). Faithfully NOT implemented here either (see design
// doc DESIGN_TesterTCP_TimerProcessTCPDataTimer.md deviation table's own
// framing: "golden自己就沒做, 每 chunk 一命令").
//
// golden TfTesterTCP::TimerProcessTCPDataTimer, TesterTCP.cpp:349-552.
// AI(W906-TesterTCPTimer) 20260720: golden signature is
// `void __fastcall TimerProcessTCPDataTimer(TObject *Sender)`; Sender is never
// read in the body (verified full read) -- dropped here, matching this file's
// established TimerTCPIPConnectTimer convention (extract-calc-core).
void TesterTCPSocket_TimerProcessTCPDataTimer()
{
    static bool bTimerRun=false;
    // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #1 (vestigial reentrancy
    // guard) -- bTimerRun is read (guard below) and reset to false at the
    // very end of every path (normal return AND the catch(...) handler), but
    // NO STATEMENT in the whole golden function ever sets it to true. Compare
    // the sibling TesterTCPSocket_TimerTCPIPConnectTimer() above, which DOES
    // `bTimerRun = true;` right after its own guard. This branch is dead code
    // (bTimerRun==true never happens in production) -- preserved verbatim,
    // NOT armed here (do not "fix" by adding a true-assignment).
    AnsiString str="", asECHO="";
    AnsiString StrBarCodeTCPIP;
    bool bBarCodeError=false;
    int iLen=0, iBinCheck[32], iLen1;
    try
    {
        if(InitialOK==false ||
           bTimerRun==true ||
           TesterTCPSocket.bTCPError==true)                                    //Steven 20231113 : TCP/IP Test 連線異常處置
        {
            return;
        }

        AnsiString sPrefix, sReply="", sPrefix2;
        AnsiString Str2, str3="";

        if(TesterTCPSocket.SocketTCPIPReceiveList->Count>0)
        {
            TesterTCPSocket.sTCPIPRecevieData=TesterTCPSocket.SocketTCPIPReceiveList->Strings[0];

            sReply="";
            Str2=StringReplace(TesterTCPSocket.sTCPIPRecevieData, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
            TStringList *sList = new TStringList();
            TStringList *sBarCodeList = new TStringList();
            TStringList *sBinList = new TStringList();
            sList->SetText(Str2.c_str());
            sPrefix=sList->Strings[0];

            if(sPrefix=="WORKFILE_OK")
            {
                // golden empty-branch idiom (TesterTCP.cpp:380-382) -- consumed, no reply.
            }
            else if(sPrefix=="WORKFILE_FAIL")
            {
                ShowMyMessage("There does not have setup at tester side. Please make a new one", "請在測試機端建立新的工作檔", fMain->cbSetupFileName->Text);
            }
            else if(sPrefix=="BARCODE?")
            {
                StrBarCodeTCPIP="";
                for(int i=0; i<32; i++)
                {
                    StrBarCodeTCPIP+=fMain->tBarCodeList->Strings[i];
                    asTCPIPBarCode[i]=fMain->tBarCodeList->Strings[i];
                    // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #4 -- literal
                    // double semicolon, golden TesterTCP.cpp:394 verbatim.
                    TesterTCPSocket.labOcr[i].Caption=fMain->tBarCodeList->Strings[i];;
                    if(i==31)
                        StrBarCodeTCPIP+=";";
                    else
                        StrBarCodeTCPIP+=",";
                }
                sReply.sprintf("BARCODE:%s", StrBarCodeTCPIP);
            }
            else if(sPrefix.Pos("ECHOCODE:")!=0)
            {
                bBarCodeError=false;
                // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #5 -- iLen is
                // assigned here (golden TesterTCP.cpp:405) but never read in
                // THIS branch (the actual delete-count below is the fixed
                // literal 9, not iLen+1); iLen IS a shared function-scope
                // local reused for real in the BINON branch further down.
                // Dead store here, preserved verbatim.
                iLen=Str2.Pos(":");
                // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #6 -- in-place
                // mutate + self-assign idiom (golden TesterTCP.cpp:406);
                // vclcompat AnsiString::Delete(index,count) returns *this so
                // this is a direct, faithful translation, not a rewrite.
                Str2=Str2.Delete(1, 9);
                iLen1=Str2.Pos(";");
                Str2=Str2.SubString(1, iLen1-1);
                Str2=StringReplace(Str2, ",", "\n", TReplaceFlags()<<rfReplaceAll);
                sBarCodeList->SetText(Str2.c_str());
                if(sBarCodeList->Count!=32)                                     //Jimmychiu 20231002 : check barcode list length is 32
                {
                    bBarCodeError=true;
                    TesterTCPSocket_AddTCPIPCommunicationLog(0, AnsiString().sprintf("barcode list count is not 32, real count is %d , command:%s", sBarCodeList->Count, Str2));
                }
                else
                {
                    for(int i=0; i<32; i++)
                    {
                        if(sBarCodeList->Strings[i]!=asTCPIPBarCode[i])
                        {
                            bBarCodeError=true;
                        }
                    }
                }

                if(bBarCodeError==true)
                {
                    sReply.sprintf("ECHOCODENG");
                }
                else
                {
                    sReply.sprintf("ECHOCODEOK");
                }
            }
            else if(sPrefix=="Test Arm?")
            {
                if(iWhichArmDown==1)
                {
                    sReply.sprintf("TestArm:1");
                }
                else if(iWhichArmDown==2)
                {
                    sReply.sprintf("TestArm:2");
                }
                else
                {
                    sReply.sprintf("TestArm:0");
                }
            }
            else if(sPrefix=="TempArm?")
            {
                asTCPIPTemperature="";
                fMain->WritePERSITETemperature();
                // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #14 -- the
                // wrapper (FormsFacade.cpp TfMain::WritePERSITETemperature,
                // golden Command.cpp:940) unconditionally does
                // `asTCPIPTemperature.sprintf("%s\r", sRet);` in the
                // TCP_IP_MODE branch -- even an empty sRet becomes "\r", which
                // is != "" here, so this reply ALWAYS fires (minimum
                // "TempArm:") once TestIF.iTestType==TCP_IP_MODE. Preserved
                // faithfully (see FormsFacade.cpp for the wrapper itself).
                if(asTCPIPTemperature!="")
                {
                    // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #15 --
                    // strips "\r" only, NOT "\n" (golden TesterTCP.cpp:457
                    // verbatim).
                    asTCPIPTemperature=StringReplace(asTCPIPTemperature, "\r", "", TReplaceFlags()<<rfReplaceAll);
                    sReply.sprintf("TempArm:%s",asTCPIPTemperature.c_str());
                }
            }
            else if(sPrefix.Pos("BINON:")!=0)
            {
                ZeroMemory(iBinCheck, sizeof(iBinCheck));
                for(int i=0; i<32; i++)
                {
                    fMain->tTestResult->Strings[i]=AnsiString("-1");            //wei 20160302 0 ==> -1  因為不測試為-1
                    TesterTCPSocket.plSite[i].Caption="--";
                }
                // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #7 -- asECHO
                // captures sTCPIPRecevieData BEFORE the \r\n-stripped Str2 is
                // used, i.e. it keeps the ORIGINAL embedded CRLF (golden
                // TesterTCP.cpp:469); the ECHO: reply built from it below
                // therefore carries an embedded CRLF mid-string, and
                // SendTCPIPCommand appends its OWN trailing "\r\n" on top.
                asECHO=TesterTCPSocket.sTCPIPRecevieData;
                iLen=Str2.Pos(":");
                Str2=Str2.SubString(iLen+1,Str2.Length());
                Str2=StringReplace(Str2, ",", "\n", TReplaceFlags()<<rfReplaceAll);
                Str2=StringReplace(Str2, ";", "\n", TReplaceFlags()<<rfReplaceAll);
                sBinList->SetText(Str2.c_str());
                // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #8 -- row
                // order is REVERSED via the hardcoded `Strings[3-i]` (not
                // `Strings[MAX_SOCKET_ROW-1-i]`), column width is hardcoded
                // `*8` (not `*MAX_SOCKET_COL`), and each column digit is
                // extracted by hardcoded `SubString(MAX_SOCKET_COL-j,1)`
                // (single-character, right-to-left) -- golden
                // TesterTCP.cpp:477-480 verbatim. NOT rewritten to the
                // "obviously intended" MAX_SOCKET_ROW/COL-relative forms.
                for(int i=0; i<MAX_SOCKET_ROW; i++)
                {
                    str3=sBinList->Strings[3-i];
                    for(int j=0; j<MAX_SOCKET_COL; j++)
                    {
                        iBinCheck[i*8+j]=atoi(str3.SubString(MAX_SOCKET_COL-j, 1).c_str());
                    }
                }

                for(int i=0; i<MAX_SOCKET_ROW; i++)
                {
                    for(int j=0; j<MAX_SOCKET_COL; j++)
                    {
                        if(TestIF.iSiteMap[i][j]>0 && TestIF.iSiteMap[i][j]<=MAX_SOCKET_TOTAL)
                        {
                            iBin[i][j]=iBinCheck[TestIF.iSiteMap[i][j]-1];
                            fMain->tTestResult->Strings[TestIF.iSiteMap[i][j]-1]=iBin[i][j];  //Steven 20141230 : 修正SECS GEM參數
                            TesterTCPSocket.plSite[TestIF.iSiteMap[i][j]-1].Caption=iBin[i][j];
                        }
                        else
                        {
                            iBin[i][j]=0;
                        }
                    }
                }

                sReply.sprintf("ECHO:%s", asECHO.c_str());
            }
            else if(sPrefix=="ECHOOK")
            {
                bTimeOutForNoFullSite=false;
                bEcho=true;
                TestIntervalsTime.LatchCycleTime(true);

                // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #16 -- logs a
                // single space " " (golden TesterTCP.cpp:509), the same
                // "blank line" idiom AddTCPIPCommunicationLog special-cases
                // (see that function's `if(Str==" ") Log.sprintf(" ");` above).
                TesterTCPSocket_AddTCPIPCommunicationLog(0, " ");
            }
            else if(sPrefix=="GETOSSETUP")
            {
                sList->Delete(0);
                fMain->SVID1190_OSSetup=sList->CommaText;
            }
// AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #10 -- whole dead branch,
// commented out verbatim in golden itself (TesterTCP.cpp:516-525); preserved
// as a comment, not resurrected (edTPSVersion3/asGetTPSVersion/EventReport are
// not translated -- resurrecting this would need new, unbuilt dependencies).
//            else if(sPrefix=="GetTPSVersionOK")
//            {
//                Str2=StringReplace(Str2, ",", "\n", TReplaceFlags()<<rfReplaceAll);
//                sList->SetText(Str2.c_str());
//                sPrefix2=sList->Strings[1];
//
//                edTPSVersion3->Text=sPrefix2;
//                asGetTPSVersion[2]=sPrefix2;
//                EventReport(SECS_EVENT.TesterTCPReady);
//            }

            if(sReply!="")
                TesterTCPSocket_SendTCPIPCommand(0, " ", sReply);
            // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #2 -- only sList
            // is ever delete'd here; sBarCodeList and sBinList (both `new`'d
            // above) are NEVER delete'd on ANY path through this function --
            // golden TesterTCP.cpp:529-530 vs :375-376. A genuine per-message
            // leak of 2 TStringList objects, preserved verbatim (do not add
            // `delete sBarCodeList;`/`delete sBinList;`; see the design doc's
            // deviation table -- this is a FIDELITY requirement, not an
            // oversight).
            sList->Clear();
            delete sList;
            TesterTCPSocket.SocketTCPIPReceiveList->Delete(0);
        }

        // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #9 -- dead operation:
        // Clear() on an already-empty list is a no-op; golden TesterTCP.cpp:534-535
        // verbatim (preserved, not removed).
        if(TesterTCPSocket.SocketTCPIPReceiveList->Count==0)
            TesterTCPSocket.SocketTCPIPReceiveList->Clear();

        if(LastSet.iTester==OFF_LINE && TesterTCPSocket.SimulateStart)
        {
            // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #12 -- SimulateStart
            // has NO true-setter anywhere in the whole golden tree (grepped
            // --include=*.cpp/*.h): ctor default false (TesterTCP.cpp:64), the
            // read here, and the self-clear below (:543) are the only 3 write/
            // read sites, plus the bare declaration (h:243). This tail is
            // therefore DEAD in production golden; preserved faithfully as a
            // public bool (TesterTCPSocket.SimulateStart) tests can drive
            // directly, matching golden's own "publicly writable" shape.
            TesterTCPSocket_SimulateBin();
            bTimeOutForNoFullSite=false;
            bEcho=true;
            TestIntervalsTime.LatchCycleTime(true);
            TesterTCPSocket.SimulateStart=false;
        }
    }
    catch(...)
    {
        //發生error
        // AI(W906-TesterTCPTimer) 20260720: GOLDEN QUIRK #3 -- str is built
        // here (golden TesterTCP.cpp:549) but never read/logged/used
        // afterward -- a dead store inside the catch handler, preserved
        // verbatim.
        str.sprintf("bTimerRun=%d , %s", bTimerRun, TesterTCPSocket.sTCPIPRecevieData);
    }
    bTimerRun=false;
}

// golden TfTesterTCP::SimulateBin, TesterTCP.cpp:554-613 (golden itself has NO
// __fastcall on this one -- a plain method; matches this port's free-function
// signature with no fastcall marking either).
void TesterTCPSocket_SimulateBin()
{
    int ct=0, iSiteNo;
    AnsiString str="";
    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            if(TestIF.iSiteMap[i][j]>0 && TestIF.iSiteMap[i][j]<=MAX_SOCKET_TOTAL)
            {
                iSiteNo=TestIF.iSiteMap[i][j]-1;
                if(     TesterTCPSocket.cbSimulateBin[iSiteNo].ItemIndex==0)    //2012-10-11    Dell Fix
                    ct=1;
                else if(TesterTCPSocket.cbSimulateBin[iSiteNo].ItemIndex==1)
                    ct=2;
                else if(TesterTCPSocket.cbSimulateBin[iSiteNo].ItemIndex==2)
                    ct=3;
                else if(TesterTCPSocket.cbSimulateBin[iSiteNo].ItemIndex==3)
                    ct=4;
                else if(TesterTCPSocket.cbSimulateBin[iSiteNo].ItemIndex==4)
                    ct=5;
                else if(TesterTCPSocket.cbSimulateBin[iSiteNo].ItemIndex==5)
                    ct=6;
                else if(TesterTCPSocket.cbSimulateBin[iSiteNo].ItemIndex==6)
                    ct=random(5)+1;
                else if(TesterTCPSocket.cbSimulateBin[iSiteNo].ItemIndex==7)
                    ct=random(10)+1;
                else
                    ct=random(15)+1;

                iBin[i][j]=ct;
                fMain->tTestResult->Strings[iSiteNo]=iBin[i][j];                //Steven 20141230 : 修正SECS GEM參數
                TesterTCPSocket.plSite[iSiteNo].Caption=iBin[i][j];
            }
            else
            {
                iBin[i][j]=0;
            }
        }
    }

    for(int i=0; i<MAX_SOCKET_TOTAL; i++)
    {
        if(fMain->tTestResult->Strings[31-i]!="-1")
        {
            str+=fMain->tTestResult->Strings[31-i];
        }
        else
        {
            str+="0";
        }

        if(i==31)
            str+=";";
        else
            str+=",";
    }

    TesterTCPSocket_AddTCPIPCommunicationLog(0, str);
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
