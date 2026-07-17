// =============================================================================
//  tests/test_automation.cpp -- Automation/automation.cpp (TfAutomation)
//  "network wiring + small helpers" translation wave (W906-Automation).
//
//  AI(W906-Automation) 20260716: new file.
//
//  No external test framework: a tiny PASS/FAIL harness (same style as
//  tests/test_auto9045.cpp / tests/test_serversocket.cpp) prints per-case
//  results and a final summary, returning non-zero on any failure.
//
//  ORACLE STRATEGY (per this project's testing convention -- prefer a real,
//  independently-derivable oracle over invented numbers):
//    * crc16(): the exact golden algorithm (init 65535, XOR, /2, XOR 40961 if
//      odd, 8x per byte) was independently re-implemented in Python and run
//      against several inputs to get ground-truth values (not hand-guessed):
//        crc16("A")   -> 28799
//        crc16("ABC") -> 34128
//        crc16("123") -> 31349
//        crc16("")    -> 65535
//    * SplitDataBySoh/GetCRC/AddBlock/IsStartWord/charcmp: pure string/byte
//      logic, hand-traced against the golden source line-by-line (see each
//      case's comment).
//    * Wire-format bytes (CommandProcess/SendClient/SendServer): hand-built
//      from golden's own HEAD/head construction formula (STX+CMD+SOH+
//      V_TOTAL+SOH+Data[0]+SOH+Data[1]+...+ETX), byte-for-byte.
//
//  SCOPE: exercises the in-scope translated surface (see automation.h's own
//  file-head scope note). Does NOT exercise ProcessBuffer/ProcessBuffer1/
//  SendReportRequest (GATED no-op stubs this wave -- nothing to verify beyond
//  "does not crash/does not dispatch", covered indirectly by the
//  OLPServerClientRead/tmrOLPTimer cases below that call through them).
//
//  SIDE EFFECTS (documented, same posture as test_auto9045.cpp's own
//  disclosed-side-effects section): WriteTCPDataToTextFile/SaveRecord write
//  real files under D:\<HandlerType>_Log\... on the machine running this
//  test (debug/comm log paths, never a production recipe file). SaveRecord's
//  own file write is only reached from inside ShowMSG once Memo/MemoChar/
//  MemoCode->Lines->Count exceeds 500 -- never true in this short run, so it
//  is never actually reached even where IniConfig.bN08_1SaveOLPLog is
//  temporarily toggled true to open ShowRecord's logging guard.
//  WriteTCPDataToTextFile IS exercised once (Part 8) and does write one line
//  to D:\HT9045_Log\TCP_Data\<today>.tcp -- flagged here, not hidden.
// =============================================================================
#include "Automation/automation.h"

#include "MachineDefine.h"
#include "MachineType.h"
#include "cprod.h"
#include "cmydef.h"
#include "Config.h"
#include "common.h"
#include "canary_support.h"
#include "FormsFacade.h"
#include "Public/cJSON.h"

#include <cstdio>
#include <cstring>
#include <vector>

// TU-local extern redeclarations: automation.cpp defines these as plain
// file-scope globals with external linkage (matching golden) but does not
// expose them via automation.h's public surface (see that header's own
// "File-scope globals" comment -- only HandlerType/aDataPath are exposed
// there, since nothing else in the translated tree needs the rest yet).
extern AnsiString STX;
extern AnsiString SOH;
extern AnsiString ETX;
extern AnsiString ReceiveString;

static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

static AnsiString VecToAnsi(const std::vector<char> &v)
{
    AnsiString s;
    for (size_t i = 0; i < v.size(); ++i)
        s += v[i];
    return s;
}

// ===========================================================================
//  PART 1 -- the 6 zero-dependency free functions
// ===========================================================================
static void Part1_FreeFunctions()
{
    printf("\n[1] Free functions (SplitDataBySoh/GetCRC/AddBlock/IsStartWord/crc16/charcmp)\n");

    // SplitDataBySoh: golden :212-223 -- extracts everything before the
    // first SOH (0x01) [or ETX (0x03) if no SOH present], consuming the
    // delimiter too.
    {
        AnsiString soh1; soh1.sprintf("%c", 0x01);
        AnsiString etx1; etx1.sprintf("%c", 0x03);
        SOH = soh1;   // SplitDataBySoh reads the file-scope SOH/ETX globals
        ETX = etx1;

        AnsiString buf = AnsiString("CMD") + soh1 + AnsiString("REST");
        AnsiString got = SplitDataBySoh(buf);
        CHECK(got == "CMD", "SplitDataBySoh extracts up to the first SOH");
        CHECK(buf == "REST", "SplitDataBySoh consumes the extracted part + delimiter");

        AnsiString buf2 = AnsiString("ONLYETX") + etx1;
        AnsiString got2 = SplitDataBySoh(buf2);
        CHECK(got2 == "ONLYETX", "SplitDataBySoh falls back to ETX when no SOH present (golden :216-218)");
    }

    // GetCRC: golden :225-231 -- takes the first 5 chars, but (preserved
    // quirk) only deletes 1 char from Buffer, not 5.
    {
        AnsiString buf = "ABCDEFGH";
        AnsiString crc = GetCRC(buf);
        CHECK(crc == "ABCDE", "GetCRC extracts exactly the first 5 chars");
        CHECK(buf == "BCDEFGH", "GetCRC's preserved quirk: only 1 char removed from Buffer, not 5");
    }

    // AddBlock: golden :233-238 -- wraps in [ ].
    CHECK(AddBlock("0001") == "[0001]", "AddBlock wraps its argument in [ ]");
    CHECK(AddBlock("") == "[]", "AddBlock on an empty string still wraps (2-char result)");

    // IsStartWord: golden :240-248 -- true + consumes 1 char iff w is found
    // at position 1.
    {
        AnsiString stxG; stxG.sprintf("%c", 0x02);
        AnsiString buf = stxG + "REST";
        bool r = IsStartWord(buf, stxG);
        CHECK(r == true, "IsStartWord returns true when w is at position 1");
        CHECK(buf == "REST", "IsStartWord consumes exactly 1 char on match");

        AnsiString buf2 = "XREST";
        bool r2 = IsStartWord(buf2, stxG);
        CHECK(r2 == false, "IsStartWord returns false when w is not at position 1");
        CHECK(buf2 == "XREST", "IsStartWord leaves Buffer untouched on no-match");
    }

    // crc16: golden :250-275 -- independently re-derived oracle (see file
    // header). n excludes any trailing NUL (raw char* + explicit length).
    {
        char a[] = "A";
        CHECK(crc16(a, 1) == "28799", "crc16(\"A\") == 28799 (independently re-derived oracle)");
        char abc[] = "ABC";
        CHECK(crc16(abc, 3) == "34128", "crc16(\"ABC\") == 34128");
        char n123[] = "123";
        CHECK(crc16(n123, 3) == "31349", "crc16(\"123\") == 31349");
        char empty[] = "";
        CHECK(crc16(empty, 0) == "65535", "crc16(\"\") == 65535 (identity -- no bytes ever flip the init value)");
    }

    // charcmp: golden :2279-2288 (ChungHung 20120413 add).
    {
        char b1[4] = {1, 2, 3, 4};
        char b2[4] = {1, 2, 3, 4};
        char b3[4] = {1, 2, 3, 9};
        CHECK(charcmp(b1, b2, 4) == true, "charcmp: identical buffers -> true");
        CHECK(charcmp(b1, b3, 4) == false, "charcmp: differing buffers -> false");
    }
}

// ===========================================================================
//  PART 2 -- event API (GetEventNum/SetEventResult/GetEventResult/
//  CompareEvent/ClearEvent/GetEventStrResult)
// ===========================================================================
static void Part2_EventApi(TfAutomation *fa)
{
    printf("\n[2] Event API (GetEventNum/SetEventResult/GetEventResult/CompareEvent/ClearEvent/GetEventStrResult)\n");

    int num = -99;
    bool got = fa->GetEventNum(num, "PRODUCTION_REPLY", "0003");
    CHECK(got == true, "GetEventNum finds a free slot (fresh instance -> always succeeds)");
    CHECK(num >= 0, "GetEventNum returns a valid slot index");
    CHECK(fa->GetEventResult(num) == false, "freshly-registered event starts un-fired (def=false)");

    // CompareEvent: golden :189-203 -- matches when str contains BOTH the
    // CommandString AND the NumString (or NumString=="").
    fa->CompareEvent("...PRODUCTION_REPLY...0003...");
    CHECK(fa->GetEventResult(num) == true, "CompareEvent fires the event when both substrings are present");

    fa->ClearEvent(num);
    CHECK(fa->GetEventResult(num) == false, "ClearEvent resets def=false");

    // GetEventStrResult: golden :2396-2419 -- 3 special-cased (cmdstr,numstr)
    // pairs map to 1/2/3, everything else (including a cleared slot) -> 4.
    CHECK(fa->GetEventStrResult(num) == 4, "GetEventStrResult on a cleared slot -> 4 (no match)");

    int num2 = -1;
    fa->SetEventResult(0, false, "PRODUCTION_REQUEST", "0007");
    num2 = 0;
    CHECK(fa->GetEventStrResult(num2) == 2, "GetEventStrResult(\"PRODUCTION_REQUEST\",\"0007\") -> 2 (golden :2408-2411)");

    // out-of-range num is rejected by every accessor (golden's own bounds
    // guard `num>=0 && num<iMaxEvent`).
    CHECK(fa->GetEventResult(-1) == false, "GetEventResult(-1) -> false (out-of-range guard)");
    CHECK(fa->GetEventResult(999) == false, "GetEventResult(999) -> false (out-of-range guard)");
    CHECK(fa->SetEventResult(999, true, "X", "Y") == false, "SetEventResult(999,...) -> false (out-of-range guard)");
}

// ===========================================================================
//  PART 3 -- GetTimeInfo() format sanity (golden :277-286: "%04d%02d%02d%02d%02d%02d")
// ===========================================================================
static void Part3_GetTimeInfo(TfAutomation *fa)
{
    printf("\n[3] GetTimeInfo() format\n");

    AnsiString t = fa->GetTimeInfo();
    CHECK(t.Length() == 14, "GetTimeInfo() returns exactly 14 chars (YYYYMMDDHHMMSS)");
    bool allDigits = true;
    for (int i = 1; i <= t.Length(); ++i)
        if (t[i] < '0' || t[i] > '9') allDigits = false;
    CHECK(allDigits, "GetTimeInfo() is all-digit");
    AnsiString yearPart = t.SubString(1, 4);
    CHECK(yearPart.ToInt() >= 2024 && yearPart.ToInt() <= 2100, "GetTimeInfo()'s year prefix is plausible");
}

// ===========================================================================
//  PART 4 -- ctor customer-code branching (golden :57-147)
// ===========================================================================
static void Part4_CtorBranching()
{
    printf("\n[4] Ctor customer-code branching\n");

    int savedCC = CUSTOMER_CODE;

    CUSTOMER_CODE = CC_ASE_CL;
    TfAutomation *fa1 = new TfAutomation(NULL);
    CHECK(fa1->OLPServer->Port == 5001, "CC_ASE_CL: OLPServer->Port stays at the .dfm default (5001, never assigned this branch)");
    CHECK(fa1->OLPServer->IsActiveNow() == true, "CC_ASE_CL: OLPServer->Open() called");
    CHECK(fa1->gbAutomation->Visible == true, "CC_ASE_CL: gbAutomation->Visible == true");

    CUSTOMER_CODE = CC_PTI;
    TfAutomation *fa2 = new TfAutomation(NULL);
    CHECK(fa2->OLPServer->Port == 6671, "CC_PTI: OLPServer->Port forced to 6671 (Sam 20210812)");
    CHECK(fa2->OLPServer->IsActiveNow() == true, "CC_PTI: OLPServer->Open() called");
    CHECK(fa2->gbAutomation->Visible == true, "CC_PTI: gbAutomation->Visible == true");

    CUSTOMER_CODE = 0;   // no matching branch -> the else path
    TfAutomation *fa3 = new TfAutomation(NULL);
    CHECK(fa3->OLPServer->IsActiveNow() == false, "default (unmatched CUSTOMER_CODE): OLPServer->Close() called");
    CHECK(fa3->gbAutomation->Visible == false, "default: gbAutomation->Visible == false");
    CHECK(fa3->OLPClient->Address == AnsiString("10.33.10.70"), "default: OLPClient->Address stays at the .dfm default (never assigned this branch)");
    CHECK(fa3->OLPClient->Port == 5002, "default: OLPClient->Port stays at the .dfm default (5002)");
    CHECK(fa3->cbbOLPCommand->Text == "0001", "cbbOLPCommand->Text starts at the .dfm default");
    CHECK(fa3->edinputIP->Text == "192.168.120.143", "edinputIP->Text starts at the .dfm default");
    CHECK(fa3->edinputport->Text == "6670", "edinputport->Text starts at the .dfm default");
    CHECK(fa3->TestMode == 0, "TestMode starts at 0");
    CHECK(fa3->bReceive == false && fa3->bReceive2 == false, "bReceive/bReceive2 start false");
    CHECK(fa3->CommandBuffer->Count == 0, "CommandBuffer starts empty");
    CHECK(fa3->CommandClientReplyBuffer->Count == 0, "CommandClientReplyBuffer starts empty");

    CUSTOMER_CODE = savedCC;
}

// ===========================================================================
//  PART 5 -- DoCommandBuffer + CommandBuffer bookkeeping (golden :937-947)
// ===========================================================================
static void Part5_DoCommandBuffer(TfAutomation *fa)
{
    printf("\n[5] DoCommandBuffer + CommandBuffer bookkeeping\n");

    int before = fa->CommandBuffer->Count;
    fa->DoCommandBuffer("ALARM_REQUEST", "SUB", "Some Alarm Message", 7, "JAM0109");
    CHECK(fa->CommandBuffer->Count == before + 1, "DoCommandBuffer appends exactly one entry");

    MyProcessBuffer *p = (MyProcessBuffer *)fa->CommandBuffer->Items[fa->CommandBuffer->Count - 1];
    CHECK(p->Command == "ALARM_REQUEST", "the pushed entry's Command round-trips");
    CHECK(p->SubCommand == "SUB", "the pushed entry's SubCommand round-trips");
    CHECK(p->AlarmMessage == "Some Alarm Message", "the pushed entry's AlarmMessage round-trips");
    CHECK(p->ErrType == 7, "the pushed entry's ErrType round-trips");
    CHECK(p->AlarmID == "JAM0109", "the pushed entry's AlarmID round-trips");
    CHECK(p->TimeString.Length() == 14, "the pushed entry's TimeString is stamped via GetTimeInfo()");
}

// ===========================================================================
//  PART 6 -- CommandProcess -> SendClient wire format, NO connection yet
//  (falls through to CommandClientReplyBuffer, golden :855-862)
// ===========================================================================
static void Part6_CommandProcessNoConnection(TfAutomation *fa)
{
    printf("\n[6] CommandProcess -> SendClient wire format (no connection)\n");

    AnsiString Data[4];
    Data[0] = "AAA";
    Data[1] = "BBB";
    int before = fa->CommandClientReplyBuffer->Count;
    fa->CommandProcess("TEST_CMD", 2, Data, true, 0);

    CHECK(fa->CommandClientReplyBuffer->Count == before + 1, "SendClient's no-connection path buffers the reply for later retry");

    AnsiString expected = STX + AnsiString("TEST_CMD") + SOH + AnsiString("2") + SOH + AnsiString("AAA") + SOH + AnsiString("BBB") + ETX;
    AnsiString actual = fa->CommandClientReplyBuffer->Strings[fa->CommandClientReplyBuffer->Count - 1];
    CHECK(actual == expected, "buffered reply matches golden's HEAD wire-format construction exactly");
}

// ===========================================================================
//  PART 7 -- CommandProcess -> SendClient wire format, WITH a Sim-accepted
//  OLPServer connection (bytes actually go out via SendBuf)
// ===========================================================================
static void Part7_CommandProcessWithConnection(TfAutomation *fa)
{
    printf("\n[7] CommandProcess -> SendClient wire format (Sim connection accepted)\n");

    fa->OLPServer->Open();   // realistic precondition: a peer cannot connect to a closed listener
    TCustomWinSocket *conn = fa->OLPServer->SimAcceptConnection("127.0.0.1", 12345);
    CHECK(conn != 0, "OLPServer->SimAcceptConnection returns a connection");
    CHECK(fa->OnLine->Enabled == true, "OLPServerClientConnect (wired) sets OnLine->Enabled=true on a single connection");

    fa->iSocketHandle = conn->SocketHandle;   // as OLPServerClientRead would have set it from a real inbound read

    AnsiString Data[2];
    Data[0] = "0";
    fa->CommandProcess("REPORT_GRANT", 1, Data, true, conn->SocketHandle);

    AnsiString expected = STX + AnsiString("REPORT_GRANT") + SOH + AnsiString("1") + SOH + AnsiString("0") + ETX;
    AnsiString onWire = VecToAnsi(conn->SimTxBuffer());
    CHECK(onWire == expected, "bytes actually sent via Connections[0]->SendBuf match the golden wire format");
}

// ===========================================================================
//  PART 8 -- WriteTCPDataToTextFile (real file write -- see file-head
//  SIDE EFFECTS note) + SendServer via OLPClient
// ===========================================================================
static void Part8_WriteAndSendServer(TfAutomation *fa)
{
    printf("\n[8] WriteTCPDataToTextFile (real log write) + SendServer via OLPClient\n");

    fa->DoCommandBuffer("MESSAGE_REQUEST", "", "hello from test_automation", 0, "");
    int before = fa->CommandBuffer->Count;
    CHECK(before >= 1, "a pending entry exists before WriteTCPDataToTextFile drains it");
    fa->WriteTCPDataToTextFile();
    CHECK(fa->CommandBuffer->Count == 0, "WriteTCPDataToTextFile drains CommandBuffer via ->Clear() (golden :932)");

    fa->OnLine2->Enabled = true;   // as OLPClientConnect would have set it
    AnsiString S = STX + AnsiString("PING") + ETX;
    AnsiString S2 = "[STX][PING][ETX]";
    fa->SendServer(S, S2);
    AnsiString onWire = VecToAnsi(fa->OLPClient->Socket->SimTxBuffer());
    CHECK(onWire == S, "SendServer sends the exact bytes via OLPClient->Socket->SendBuf when OnLine2->Enabled");
}

// ===========================================================================
//  PART 9 -- ShowMSG / ShowCharHex / ShowRecord (real TStringList-backed
//  Memo/MemoChar/MemoCode, chkViewComm->Checked toggled per golden's own guard)
// ===========================================================================
static void Part9_MemoLogging(TfAutomation *fa)
{
    printf("\n[9] ShowMSG / ShowCharHex / ShowRecord\n");

    // ShowMSG itself is private (golden automation.h private: section) --
    // exercised indirectly via the public ShowRecord(), which calls it after
    // building its "[Receive]/[Send]" prefix. golden ShowMSG: Lines->Add(S)
    // then an UNCONDITIONAL trailing Lines->Add("") -- 2 new lines per call
    // (golden :2435-2449).
    IniConfig.bN08_1SaveOLPLog = true;   // open ShowRecord's guard for this one call
    fa->chkViewComm->Checked = false;
    int before = fa->Memo->Lines->Count;
    fa->ShowRecord(true, "hello", 42);
    CHECK(fa->Memo->Lines->Count == before + 2, "ShowRecord->ShowMSG appends the message line + a trailing blank line");
    AnsiString msgLine = fa->Memo->Lines->GetString(fa->Memo->Lines->Count - 2);
    CHECK(msgLine.Pos("[Receive]") != 0 && msgLine.Pos("hello") != 0, "the [Receive]-prefixed message line round-trips (golden :773-786)");
    IniConfig.bN08_1SaveOLPLog = false;

    fa->chkViewComm->Checked = false;
    int beforeChar = fa->MemoChar->Lines->Count;
    fa->ShowCharHex("X");
    CHECK(fa->MemoChar->Lines->Count == beforeChar, "ShowCharHex is a no-op while chkViewComm->Checked==false (golden :723-724)");

    fa->chkViewComm->Checked = true;
    fa->ShowCharHex("A");
    CHECK(fa->MemoChar->Lines->Count == beforeChar + 2, "ShowCharHex appends 2 lines to MemoChar once enabled (char row + blank)");
    CHECK(fa->MemoCode->Lines->Count >= 2, "ShowCharHex also appends to MemoCode (hex row + blank)");
    // 'A' == 0x41 -> hex row should contain "41 "
    AnsiString hexRow = fa->MemoCode->Lines->GetString(fa->MemoCode->Lines->Count - 2);
    CHECK(hexRow.Pos("41") != 0, "ShowCharHex's hex dump of 'A' contains \"41\"");

    IniConfig.bN08_1SaveOLPLog = false;   // keep ShowRecord's own guard closed -> no disk write
    fa->chkViewComm->Checked = false;
    int beforeRec = fa->Memo->Lines->Count;
    fa->ShowRecord(true, "unused-while-guard-closed", 0);
    CHECK(fa->Memo->Lines->Count == beforeRec, "ShowRecord is a no-op while both its guards are closed (golden :775-776)");
}

// ===========================================================================
//  PART 10 -- GetHTMLJSONDatas / GetMachineStatus / GetWorkOrder / GetMainTemp
//  (via the FormsFacade fMain facade)
// ===========================================================================
static void Part10_HtmlJsonAndFacade(TfAutomation *fa)
{
    printf("\n[10] GetHTMLJSONDatas / GetMachineStatus / GetWorkOrder / GetMainTemp\n");

    fMain->palMainStatus->Caption = "RUNNING";
    fMain->cbSetupFileName->Text = "RECIPE_A";
    fMain->edWorkTemperBase->Text = "25.0";

    CHECK(fa->GetMachineStatus() == "RUNNING", "GetMachineStatus() passes through fMain->palMainStatus->Caption");
    CHECK(fa->GetWorkOrder() == "RECIPE_A", "GetWorkOrder() passes through fMain->cbSetupFileName->Text");
    CHECK(fa->GetMainTemp() == "25.0", "GetMainTemp() passes through fMain->edWorkTemperBase->Text");

    double temps[10];
    fa->GetTemperatures(temps);
    bool identity = true;
    for (int i = 0; i < 10; ++i)
        if (temps[i] != (double)i) identity = false;
    CHECK(identity, "GetTemperatures() fills temps[i]=i (golden :2489-2495 verbatim placeholder body)");

    AnsiString json = fa->GetHTMLJSONDatas();
    cJSON *root = cJSON_Parse(json.c_str());
    CHECK(root != 0, "GetHTMLJSONDatas() produces parseable JSON");
    if (root != 0)
    {
        cJSON *ms = cJSON_GetObjectItem(root, "machineStatus");
        CHECK(ms != 0 && strcmp(ms->valuestring, "RUNNING") == 0, "JSON machineStatus field round-trips");
        cJSON *wo = cJSON_GetObjectItem(root, "workOrder");
        CHECK(wo != 0 && strcmp(wo->valuestring, "RECIPE_A") == 0, "JSON workOrder field round-trips");
        cJSON *mt = cJSON_GetObjectItem(root, "MainTemp");
        CHECK(mt != 0 && strcmp(mt->valuestring, "25.0") == 0, "JSON MainTemp field round-trips");
        cJSON *temps_j = cJSON_GetObjectItem(root, "temperatures");
        CHECK(temps_j != 0, "JSON temperatures object present");
        if (temps_j != 0)
        {
            cJSON *a1 = cJSON_GetObjectItem(temps_j, "A1");
            CHECK(a1 != 0 && strcmp(a1->valuestring, "0.0") == 0, "JSON temperatures.A1 == \"0.0\" (temps[0]=0, sensor index i+1)");
            cJSON *a10 = cJSON_GetObjectItem(temps_j, "A10");
            CHECK(a10 != 0 && strcmp(a10->valuestring, "9.0") == 0, "JSON temperatures.A10 == \"9.0\" (temps[9]=9)");
        }
        cJSON_Delete(root);
    }
}

// ===========================================================================
//  PART 11 -- OLPServerClientRead via SimPushReceive (frame parsing +
//  bReceive/ReceiveString bookkeeping; ProcessBuffer itself is a GATED no-op)
// ===========================================================================
static void Part11_OLPServerClientRead(TfAutomation *fa)
{
    printf("\n[11] OLPServerClientRead via SimPushReceive\n");

    TCustomWinSocket *conn = fa->OLPServer->SimAcceptConnection("127.0.0.1", 55000);
    ReceiveString = "";   // reset the shared parse buffer between test parts
    fa->bReceive = false;
    fa->bReceiveHasData = false;

    AnsiString frame = STX + AnsiString("ECHO") + SOH + AnsiString("0") + ETX;
    conn->SimPushReceive(frame.c_str(), frame.Length());

    CHECK(fa->bReceive == false, "bReceive is reset to false again at the end of OLPServerClientRead (ProcessBuffer runs, then resets it)");
    CHECK(ReceiveString.Length() == 0, "the fully-framed message is consumed out of ReceiveString");
    CHECK(fa->iSocketHandle == conn->SocketHandle, "iSocketHandle is updated from the event's own Socket param (golden :370)");
}

// ===========================================================================
//  PART 12 -- tmrOLPTimer smoke test (first-call bfirst branch only; static
//  locals inside tmrOLPTimer are process-wide, so this is called exactly
//  once across the whole binary -- matches golden's own singleton-timer
//  design, see automation.h's own EVENT WIRING note)
// ===========================================================================
static void Part12_TmrOLPTimerSmoke(TfAutomation *fa)
{
    printf("\n[12] tmrOLPTimer smoke test\n");

    bool savedOLP = CosFunction.bOLPFunction;
    bool savedInit = InitialOK;
    CosFunction.bOLPFunction = true;
    InitialOK = true;
    IniConfig.sN08OlpIP = "10.1.1.1";
    IniConfig.sN08OlpPort = "";   // exercise the "" -> "6670" fallback (golden :456-458)

    fa->tmrOLPTimer(NULL);

    CHECK(fa->edinputIP->Text == "10.1.1.1", "tmrOLPTimer's first-call branch copies IniConfig.sN08OlpIP into edinputIP->Text");
    CHECK(fa->edinputport->Text == "6670", "tmrOLPTimer's first-call branch falls back to \"6670\" when sN08OlpPort is empty");

    CosFunction.bOLPFunction = savedOLP;
    InitialOK = savedInit;
}

int main()
{
    Part1_FreeFunctions();

    TfAutomation *fa = new TfAutomation(NULL);

    Part2_EventApi(fa);
    Part3_GetTimeInfo(fa);
    Part4_CtorBranching();
    Part5_DoCommandBuffer(fa);
    Part6_CommandProcessNoConnection(fa);
    Part7_CommandProcessWithConnection(fa);
    Part8_WriteAndSendServer(fa);
    Part9_MemoLogging(fa);
    Part10_HtmlJsonAndFacade(fa);
    Part11_OLPServerClientRead(fa);
    Part12_TmrOLPTimerSmoke(fa);

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
