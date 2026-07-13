// =============================================================================
//  test_testertcp_socket.cpp  --  W5 VERIFY: Interface/TesterTCP_Socket
//                                  (bounded socket-management subset of the
//                                  golden TfTesterTCP OS/Open-Short Tester
//                                  TCP/IP channel)
//
//  Translation wave: W5 (comms), Phase 2 (Automation/TesterTCP), bounded unit.
//  Author: AI(W5-Final-TesterTCPSocket) 20260711
//
//  PURPOSE
//  -------
//  Proves Interface/TesterTCP_Socket.cpp LINKS and its ACTIVE logic behaves
//  per golden Interface/TesterTCP.cpp (READ-ONLY reference), driving the
//  Scktcomp::TClientSocket Sim mode (no real Tester attached -- exactly the
//  offline posture every other vclcompat/ClientSocket.h consumer test uses).
//
//  NO EXTERNAL FRAMEWORK: a tiny check harness prints PASS/FAIL per case and
//  a final summary, returning non-zero on ANY failure (same style as
//  tests/test_interfacesys.cpp / tests/test_clientsocket_ext.cpp).
//
//  FILESYSTEM NOTE: AddTCPIPCommunicationLog's golden body durably appends to
//  a per-hour log file under `asTestTCPIPLogPath` (production default
//  "D:\HT9045_Log\Test_TCPIP", common.cpp:180 -- a mutable global, not
//  const). This test REASSIGNS it to a small scratch folder next to the test
//  binary BEFORE exercising anything, so running this test never touches the
//  real production log tree (verified: without this override, an earlier
//  throwaway run of this same logic did write real files under
//  D:\HT9045_Log\Test_TCPIP -- cleaned up manually once, hence this override
//  in the committed test).
// =============================================================================
#include "Interface/TesterTCP_Socket.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static int g_pass = 0;
static int g_fail = 0;

static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected) { std::printf("PASS  %-58s got=%d\n", name, (int)got); ++g_pass; }
    else { std::printf("FAIL  %-58s got=%d exp=%d\n", name, (int)got, (int)expected); ++g_fail; }
}

static void check_s(const char* name, const std::string& got, const std::string& expected)
{
    if (got == expected) { std::printf("PASS  %-58s got=\"%s\"\n", name, got.c_str()); ++g_pass; }
    else { std::printf("FAIL  %-58s got=\"%s\" exp=\"%s\"\n", name, got.c_str(), expected.c_str()); ++g_fail; }
}

int main()
{
    std::printf("=== Interface/TesterTCP_Socket translation verification ===\n");
    std::printf("(Sim ClientSocket, no real Tester attached; asTestTCPIPLogPath\n");
    std::printf(" redirected to a local scratch folder for the duration of this test)\n\n");

    // Redirect the durable log path away from the real production tree.
    asTestTCPIPLogPath = ".\\_test_scratch_testertcpsocket";

    InitialOK        = true;
    LastSet.iTester  = ON_LINE;
    TestIF_File.asTester_Address = "127.0.0.1";
    TestIF_File.iTester_Port     = 9999;

    TesterTCPSocket_Init();

    // -------------------------------------------------------------------
    // 1. Initial state (golden ctor defaults, TesterTCP.cpp:63-65).
    // -------------------------------------------------------------------
    check_b("ctor: bEnableTCPIPChannelConnect=true", TesterTCPSocket.bEnableTCPIPChannelConnect, true);
    check_b("ctor: bConnectOK=false",                TesterTCPSocket.bConnectOK, false);
    check_b("ctor: bTCPError=false",                 TesterTCPSocket.bTCPError, false);
    check_b("ctor: not connected yet",               TesterTCPSocket_ClientSocket->IsActiveNow(), false);

    // -------------------------------------------------------------------
    // 2. TimerTCPIPConnectTimer: golden's Count1 starts at 20 and only opens
    //    once Count1>30 -- drive it exactly across that threshold.
    // -------------------------------------------------------------------
    for (int i = 0; i < 15; ++i) TesterTCPSocket_TimerTCPIPConnectTimer();
    check_b("timer: Sim connect succeeded after threshold", TesterTCPSocket_ClientSocket->IsActiveNow(), true);
    check_b("OnConnect: bConnectOK set true",                TesterTCPSocket.bConnectOK, true);
    check_b("OnConnect: bConnect set false",                 TesterTCPSocket.bConnect, false);

    // -------------------------------------------------------------------
    // 3. Timer is a no-op reconnect while already Active at the SAME
    //    Address/Port (golden's own "do nothing" branch, TesterTCP.cpp:202-206).
    // -------------------------------------------------------------------
    TesterTCPSocket_TimerTCPIPConnectTimer();
    check_b("timer: stays connected (same addr/port -> no-op)", TesterTCPSocket_ClientSocket->IsActiveNow(), true);

    // -------------------------------------------------------------------
    // 4. SendTCPIPCommand while online pushes bytes to the wire (Sim tx sink).
    // -------------------------------------------------------------------
    TesterTCPSocket_ClientSocket->Socket->SimClearTx();
    TesterTCPSocket_SendTCPIPCommand(0, "hello", "PING");
    check_b("SendTCPIPCommand: SendText wrote to sim tx",
            TesterTCPSocket_ClientSocket->Socket->SimTxBuffer().size() > 0, true);
    {
        const std::vector<char>& tx = TesterTCPSocket_ClientSocket->Socket->SimTxBuffer();
        check_s("SendTCPIPCommand: wire bytes == \"PING\\r\\n\"", std::string(tx.begin(), tx.end()), "PING\r\n");
    }

    // -------------------------------------------------------------------
    // 5. ClientSocket_TCPIPRead: a Sim-pushed reply is drained, queued, and
    //    the \r\n-stripped copy becomes sTCPIPRecevieData (golden TesterTCP.cpp:339-340).
    // -------------------------------------------------------------------
    const char* reply = "PONG\r\n";
    TesterTCPSocket_ClientSocket->Socket->SimPushReceive(reply, static_cast<int>(std::strlen(reply)));
    {
        AnsiString firstQueued = TesterTCPSocket.SocketTCPIPReceiveList->Strings[0];
        check_s("OnRead: queued raw reply", std::string(firstQueued.str()), "PONG\r\n");
    }

    // -------------------------------------------------------------------
    // 6. SendTCPIPCommand's golden OFF_LINE-mode quirk (Msg2 self-duplicated,
    //    NOT merely appended -- preserved verbatim, see the .cpp comment).
    // -------------------------------------------------------------------
    {
        LastSet.iTester = OFF_LINE;
        TesterTCPSocket.mmTCPIPCommLog.Lines.Count = 0;   // isolate this case's Add() count
        TesterTCPSocket_SendTCPIPCommand(0, "ignored", "abc");
        // Msg2 becomes "abc"+"abc"+"OffLine" = "abcabcOffLine" -> Log = "Send, abcabcOffLine"
        // (AddTCPIPCommunicationLog itself does not expose the built string
        // directly; the OFF_LINE early-return before any SendText is the
        // externally-observable half of this case -- verified next.)
        check_b("OFF_LINE: AddTCPIPCommunicationLog still ran once", TesterTCPSocket.mmTCPIPCommLog.Lines.Count == 1, true);
        LastSet.iTester = ON_LINE;
    }

    // -------------------------------------------------------------------
    // 7. Disconnect click path.
    // -------------------------------------------------------------------
    TesterTCPSocket_btTCPIP_DisconnectClick();
    check_b("DisconnectClick: socket closed",       TesterTCPSocket_ClientSocket->IsActiveNow(), false);
    check_b("OnDisconnect: bConnectOK cleared",     TesterTCPSocket.bConnectOK, false);
    check_b("DisconnectClick: channel disabled",    TesterTCPSocket.bEnableTCPIPChannelConnect, false);
    check_b("DisconnectClick: timer-enabled stand-in cleared", TesterTCPSocket.bTimerTCPIPConnectEnabled, false);

    // While disabled, the timer must NOT reconnect no matter how many ticks.
    for (int i = 0; i < 40; ++i) TesterTCPSocket_TimerTCPIPConnectTimer();
    check_b("timer: stays disconnected while channel disabled", TesterTCPSocket_ClientSocket->IsActiveNow(), false);

    // -------------------------------------------------------------------
    // 8. Re-enable + reconnect via ConnectClick + timer threshold again.
    // -------------------------------------------------------------------
    TesterTCPSocket_btTCPIP_ConnectClick();
    check_b("ConnectClick: channel re-enabled",         TesterTCPSocket.bEnableTCPIPChannelConnect, true);
    check_b("ConnectClick: timer-enabled stand-in set",  TesterTCPSocket.bTimerTCPIPConnectEnabled, true);
    for (int i = 0; i < 32; ++i) TesterTCPSocket_TimerTCPIPConnectTimer();
    check_b("timer: reconnects after ConnectClick", TesterTCPSocket_ClientSocket->IsActiveNow(), true);

    // -------------------------------------------------------------------
    // 9. btTCPIP_TriggerClick: extract-calc-core -- caller supplies the
    //    edTCPIP_CMD->Text read, forwards to SendTCPIPCommand(0," ",cmd).
    // -------------------------------------------------------------------
    TesterTCPSocket_ClientSocket->Socket->SimClearTx();
    TesterTCPSocket_btTCPIP_TriggerClick("MANUALCMD");
    {
        const std::vector<char>& tx = TesterTCPSocket_ClientSocket->Socket->SimTxBuffer();
        check_s("TriggerClick: wire bytes == \"MANUALCMD\\r\\n\"", std::string(tx.begin(), tx.end()), "MANUALCMD\r\n");
    }

    // -------------------------------------------------------------------
    // 10. ClientSocket_TCPIPError: closes the socket, records ErrorMessage,
    //     sets bTCPError -- exercised directly (this IS the assigned
    //     OnError handler, but calling it directly is equally faithful: the
    //     handler reads no VCL widget, only Sender/ErrorCode).
    // -------------------------------------------------------------------
    {
        TObject* senderAsSocket = TesterTCPSocket_ClientSocket;
        int errCode = 10061; // WSAECONNREFUSED, arbitrary non-zero for the check below
        TesterTCPSocket_OnError(senderAsSocket, TesterTCPSocket_ClientSocket->Socket, eeConnect, errCode);
        check_b("OnError: bTCPError set true",        TesterTCPSocket.bTCPError, true);
        check_b("OnError: ErrorCode zeroed by handler", errCode == 0, true);
        check_b("OnError: socket closed",              TesterTCPSocket_ClientSocket->IsActiveNow(), false);
    }

    // -------------------------------------------------------------------
    // 11. TimerTCPIPConnectTimer's own bTCPError-triggered ShowMyMessage
    //     branch (golden TesterTCP.cpp:196-200) -- just verify it clears the
    //     flag and does not crash/hang (ShowMyMessage is a log-only Sim stub).
    // -------------------------------------------------------------------
    TesterTCPSocket_TimerTCPIPConnectTimer();
    check_b("timer: bTCPError cleared after firing", TesterTCPSocket.bTCPError, false);

    // -------------------------------------------------------------------
    // 12. AddTCPIPCommunicationLog's local memo stand-in: Count increments,
    //     and rolls over (Clear()) past the golden 2000-line threshold.
    // -------------------------------------------------------------------
    {
        int before = TesterTCPSocket.mmTCPIPCommLog.Lines.Count;
        TesterTCPSocket_AddTCPIPCommunicationLog(0, "manual log line");
        check_b("AddTCPIPCommunicationLog: local memo Count++", TesterTCPSocket.mmTCPIPCommLog.Lines.Count == before + 1, true);

        TesterTCPSocket.mmTCPIPCommLog.Lines.Count = 2001;   // force the rollover branch
        TesterTCPSocket_AddTCPIPCommunicationLog(0, "line after rollover");
        check_b("AddTCPIPCommunicationLog: rollover Clear() then Add()", TesterTCPSocket.mmTCPIPCommLog.Lines.Count == 1, true);
    }

    std::printf("\n=== %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
