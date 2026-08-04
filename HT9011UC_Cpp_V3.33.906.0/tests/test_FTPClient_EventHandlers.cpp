// =============================================================================
//  tests/test_FTPClient_EventHandlers.cpp
//  Acceptance test for KYECFTP/FTPClient_EventHandlers.{h,cpp} (the NMFTP1
//  event-handler bodies translated from golden FTPClient.cpp:1799-2097).
//
//  No external test framework (matches tests/test_MiniFtpEngine.cpp /
//  tests/test_ContactForce.cpp style): a tiny PASS/FAIL harness, non-zero
//  exit on any failure.
//
//  This TU supplies its OWN local definitions of `MyDBIProcess` (2-arg) and
//  `ShowMyMessage` (5-arg, 3 defaulted) so it links standalone -- WITHOUT
//  pulling in the giant ht9045_sm archive (canary_support.cpp / aHotPlate
//  Substrate.cpp) that owns the REAL production bodies. This is the SAME
//  established pattern already used by tests/test_config_loaders.cpp:53 and
//  tests/test_MyCCLink.cpp:73 for the identical reason -- see CMakeLists.txt's
//  own comment on ht9045_db's test target ("supplies the two stubbed
//  externals (MyDBIProcess / ShowMyMessage) that the gated UI/DB waves will
//  provide for real").
// =============================================================================
#include "KYECFTP/FTPClient_EventHandlers.h"
#include "cmydef.h"   // CUSTOMER_CODE / CC_JSCC_OS, for test [G]'s gated-dependency case

#include <cstdio>
#include <cstdlib>
#include <cstring>

// ---------------------------------------------------------------------------
//  Local stand-ins for the 2 externals FTPClient_EventHandlers.cpp calls.
//  Signatures must match EXACTLY (mangled-name linkage, not source text) --
//  see FTPClient_EventHandlers.cpp's own comments for why these particular
//  shapes were chosen (2-arg MyDBIProcess overload; ShowMyMessage matching
//  canary_support.h's declaration).
// ---------------------------------------------------------------------------
// AI(W906-GA1-B2-integrate) 20260804: seam-based spies (matching this file's
// local-extern convention) -- real bodies: aHotPlateSubstrate.cpp (MyDBIProcess
// sink) and canary_support.cpp (ShowMyMessage).
extern int        W906_MyDBIProcess_Count;
extern AnsiString W906_MyDBIProcess_LastS1;
extern AnsiString W906_MyDBIProcess_LastS2;
void W906_MyDBIProcess_Reset();
extern int        W906_ShowMyMessage_Count;
void W906_ShowMyMessage_Reset();

static std::vector<AnsiString> g_TestDbiLog;   // legacy spy log, no longer fed
// AI(W906-GA1-B2-integrate) 20260804: local MyDBIProcess stub RETIRED (real body now in link group)

static int g_TestShowMyMessageCalls = 0;
// AI(W906-GA1-B2-integrate) 20260804: local ShowMyMessage stub RETIRED (real body now in link group)

static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                   \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

static void ResetAll()
{
    FTPClientEvt_ResetStateForTest();
    g_TestDbiLog.clear();
    g_TestShowMyMessageCalls = 0;
    W906_MyDBIProcess_Reset();       // AI(W906-GA1-B2-integrate) 20260804: seam-based spies
    W906_ShowMyMessage_Reset();
}

// =============================================================================
//  [A] NMFTP1Success -- switch-on-TCmdType message text + bListOk/bError.
// =============================================================================
static void test_success()
{
    printf("\n[A] NMFTP1Success\n");
    ResetAll();

    FTPClientEvt_NMFTP1Success(cmdChangeDir);
    CHECK(FTPClientEvt_SimMemoLines().size() == 1 &&
          std::strcmp(FTPClientEvt_SimMemoLines().back().c_str(),
                      "Success -- ChangeDir successful") == 0,
          "cmdChangeDir -> exact golden message text logged to memoFTP");
    CHECK(bListOk == false, "cmdChangeDir -> bListOk NOT set (golden only sets it for List/NList)");
    CHECK(bError == false, "cmdChangeDir -> bError cleared to false");
    CHECK(W906_MyDBIProcess_Count == 1 && W906_MyDBIProcess_LastS1 == AnsiString("Exception") && W906_MyDBIProcess_LastS2 == AnsiString("Success -- ChangeDir successful"),
          "cmdChangeDir -> MyDBIProcess(\"Exception\", Str) called with the same Str");

    ResetAll();
    FTPClientEvt_NMFTP1Success(cmdNList);
    CHECK(bListOk == true, "cmdNList -> bListOk set true (golden :1973)");
    CHECK(std::strcmp(FTPClientEvt_SimMemoLines().back().c_str(),
                      "Success -- NList successful") == 0,
          "cmdNList -> exact golden message text");

    ResetAll();
    FTPClientEvt_NMFTP1Success(cmdList);
    CHECK(bListOk == true, "cmdList -> bListOk set true (golden :1972)");

    ResetAll();
    bError = true;
    FTPClientEvt_NMFTP1Success(cmdUpload);
    CHECK(bError == false, "any Success -> bError forced false (golden :1984, unconditional)");
}

// =============================================================================
//  [B] NMFTP1Failure -- switch-on-TCmdType message text; bListOk untouched;
//      Handled left unassigned (QUIRK #3).
// =============================================================================
static void test_failure()
{
    printf("\n[B] NMFTP1Failure\n");
    ResetAll();

    bool handled = false;
    FTPClientEvt_NMFTP1Failure(handled, cmdDownload);
    CHECK(std::strcmp(FTPClientEvt_SimMemoLines().back().c_str(),
                      "Failure -- Download failed") == 0,
          "cmdDownload -> exact golden message text");
    CHECK(bError == true, "NMFTP1Failure -> bError forced true (golden :2033)");
    CHECK(bListOk == false, "NMFTP1Failure -> bListOk NEVER touched by this handler (golden has no bListOk line in it at all)");
    CHECK(handled == false, "Handled left untouched by the handler (QUIRK #3) -- caller's initial false survives");
    CHECK(W906_MyDBIProcess_Count == 1 && W906_MyDBIProcess_LastS1 == AnsiString("Exception") && W906_MyDBIProcess_LastS2 == AnsiString("Failure -- Download failed"),
          "MyDBIProcess(\"Exception\", Str) called");

    ResetAll();
    handled = true; // caller pre-set true -- verify the handler really never touches it either way
    FTPClientEvt_NMFTP1Failure(handled, cmdCurrentDir);
    CHECK(handled == true, "Handled left untouched even when caller pre-set true");
    CHECK(std::strcmp(FTPClientEvt_SimMemoLines().back().c_str(),
                      "Failure -- CurrentDir failed") == 0,
          "cmdCurrentDir -> exact golden message text (last case in golden's switch, :2029)");
}

// =============================================================================
//  [C] NMFTP1UnSupportedFunction -- DEAD in golden (QUIRK #2), but the
//      function body itself must still be correct/callable directly.
// =============================================================================
static void test_unsupported()
{
    printf("\n[C] NMFTP1UnSupportedFunction (translated faithfully though golden never wires it)\n");
    ResetAll();

    FTPClientEvt_NMFTP1UnSupportedFunction(cmdRename);
    CHECK(std::strcmp(FTPClientEvt_SimMemoLines().back().c_str(),
                      "UnSupported -- Rename not supported by this server") == 0,
          "cmdRename -> exact golden message text");
    CHECK(bError == false, "NMFTP1UnSupportedFunction never touches bError (golden body has no bError line)");
    CHECK(bListOk == false, "NMFTP1UnSupportedFunction never touches bListOk either");
}

// =============================================================================
//  [D] NMFTP1AuthenticationFailed / NMFTP1ConnectionFailed -- fixed message
//      text + bError=true; Handled untouched for AuthenticationFailed.
// =============================================================================
static void test_auth_and_connection_failed()
{
    printf("\n[D] NMFTP1AuthenticationFailed / NMFTP1ConnectionFailed\n");
    ResetAll();

    bool handled = false;
    FTPClientEvt_NMFTP1AuthenticationFailed(handled);
    CHECK(std::strcmp(FTPClientEvt_SimMemoLines().back().c_str(), "Authentication Failed") == 0,
          "exact golden message text (golden :1989)");
    CHECK(bError == true, "bError forced true (golden :1990)");
    CHECK(handled == false, "Handled left untouched (QUIRK #3)");

    ResetAll();
    FTPClientEvt_NMFTP1ConnectionFailed(NULL);
    CHECK(std::strcmp(FTPClientEvt_SimMemoLines().back().c_str(), "Failure -- Connection Failed") == 0,
          "exact golden message text (golden :2005)");
    CHECK(bError == true, "bError forced true (golden :2006)");
}

// =============================================================================
//  [E] NMFTP1TransactionStart / NMFTP1TransactionStop -- fixed message text
//      only, bError untouched either way.
// =============================================================================
static void test_transaction_markers()
{
    printf("\n[E] NMFTP1TransactionStart / NMFTP1TransactionStop\n");
    ResetAll();
    bError = true; // pre-set -- prove these two handlers don't touch it

    FTPClientEvt_NMFTP1TransactionStart(NULL);
    CHECK(std::strcmp(FTPClientEvt_SimMemoLines().back().c_str(), "Data transfer start") == 0,
          "exact golden message text (golden :2000)");
    CHECK(bError == true, "TransactionStart does not touch bError (golden body has no bError line)");

    FTPClientEvt_NMFTP1TransactionStop(NULL);
    CHECK(std::strcmp(FTPClientEvt_SimMemoLines().back().c_str(), "Data transfer end") == 0,
          "exact golden message text (golden :1995)");
    CHECK(bError == true, "TransactionStop does not touch bError either");
}

// =============================================================================
//  [F] NMFTP1Error / NMFTP1Status -- route to ShowMyMessage. No capture
//      buffer exists on the real canary_support.cpp body (it's a printf-only
//      Sim stub) so this is a smoke test (call succeeds, no crash) rather
//      than a content assertion -- our LOCAL test stub only counts calls.
// =============================================================================
static void test_error_and_status()
{
    printf("\n[F] NMFTP1Error / NMFTP1Status (smoke test -- ShowMyMessage has no content-capture surface)\n");
    ResetAll();

    FTPClientEvt_NMFTP1Error(NULL, 500, "connection reset");
    CHECK(W906_ShowMyMessage_Count == 1, "NMFTP1Error calls ShowMyMessage exactly once");

    FTPClientEvt_NMFTP1Status(NULL, "227 Entering Passive Mode");
    CHECK(W906_ShowMyMessage_Count == 2, "NMFTP1Status calls ShowMyMessage exactly once more");
}

// =============================================================================
//  [G] NMFTP1ListItem -- .zip filtering / tmpList / lstServerFile / ListBox1
//      routing (bTempList branch), including the CC_JSCC_OS gated stand-in.
// =============================================================================
static void test_list_item()
{
    printf("\n[G] NMFTP1ListItem\n");
    ResetAll();

    // bTempList==false (default): a plain ".zip" entry is added to BOTH
    // tmpList (golden's own real global) and the lstServerFile log-sink slot,
    // stripped of its last 4 chars (".zip") via SubString(1, Length-4).
    FTPClientEvt_NMFTP1ListItem("ATC_Recipe.zip");
    CHECK(tmpList != NULL, "tmpList lazily allocated on first use (golden :1805-1806)");
    CHECK(tmpList->Count == 1 && std::strcmp(AnsiString(tmpList->Strings[0]).c_str(), "ATC_Recipe") == 0,
          "tmpList gets the .zip-stripped name");
    CHECK(FTPClientEvt_SimServerFileItems().size() == 1 &&
          std::strcmp(FTPClientEvt_SimServerFileItems().back().c_str(), "ATC_Recipe") == 0,
          "lstServerFile log-sink slot gets the same .zip-stripped name");
    CHECK(FTPClientEvt_SimListBox1Items().empty(), "ListBox1 slot untouched while bTempList==false");

    // A non-.zip entry is filtered out entirely (golden :1808 condition fails).
    ResetAll();
    FTPClientEvt_NMFTP1ListItem("readme.txt");
    CHECK(tmpList->Count == 0, "non-.zip entry: nothing added to tmpList");
    CHECK(FTPClientEvt_SimServerFileItems().empty(), "non-.zip entry: nothing added to lstServerFile slot");

    // DISCOVERED GOLDEN QUIRK #4 (see FTPClient_EventHandlers.cpp's own
    // comment at the call site): by De Morgan, golden's
    // `.Offset==0 || ATC_Recipe.zip==0` condition only EXCLUDES an entry
    // that contains BOTH ".Offset" AND the literal substring
    // "ATC_Recipe.zip" simultaneously -- a plain ".Offset" file WITHOUT
    // "ATC_Recipe.zip" in its name is NOT excluded (AnsiPos("ATC_Recipe.zip")
    // == 0 makes the OR true). Exercise both sides of this faithfully-
    // preserved (not "fixed") quirk:
    ResetAll();
    FTPClientEvt_NMFTP1ListItem("Foo.Offset.zip");
    CHECK(tmpList->Count == 1,
          "a plain '.Offset' name WITHOUT 'ATC_Recipe.zip' in it is NOT excluded (quirk #4)");

    ResetAll();
    FTPClientEvt_NMFTP1ListItem("ATC_Recipe.zip.Offset.zip");
    CHECK(tmpList->Count == 0,
          "a name containing BOTH '.Offset' AND literal 'ATC_Recipe.zip' IS excluded (quirk #4)");

    // bTempList==true: routes to the ListBox1 slot instead, unconditionally
    // (no .zip filtering at all in that branch -- golden :1825-1828).
    ResetAll();
    bTempList = true;
    FTPClientEvt_NMFTP1ListItem("anything-at-all.dat");
    CHECK(FTPClientEvt_SimListBox1Items().size() == 1 &&
          std::strcmp(FTPClientEvt_SimListBox1Items().back().c_str(), "anything-at-all.dat") == 0,
          "bTempList==true routes the RAW Listing straight to ListBox1, unfiltered");
    CHECK(FTPClientEvt_SimServerFileItems().empty(), "lstServerFile slot untouched while bTempList==true");

    // CUSTOMER_CODE==CC_JSCC_OS gated-dependency branch (golden :1812-1816):
    // with the gated fLotInfo->sJSCCOSFileName stand-in returning "" (see .h/
    // .cpp GATED DEPENDENCY note), AnsiPos("")==0 is always true, so bAdd is
    // forced false for EVERY .zip entry once this customer code is active.
    ResetAll();
    int savedCustomerCode = CUSTOMER_CODE;
    CUSTOMER_CODE = CC_JSCC_OS;
    FTPClientEvt_NMFTP1ListItem("SomeDevice.zip");
    CHECK(tmpList->Count == 0,
          "CUSTOMER_CODE==CC_JSCC_OS -> gated stand-in degrades to 'never matches' (bAdd=false), not a crash");
    CUSTOMER_CODE = savedCustomerCode;
}

// =============================================================================
//  [H] FTPClientEvt_WireNMFTP1Handlers -- reproduces golden's 9-line wiring
//      block; OnUnSupportedFunction/OnConnect/OnDisconnect deliberately left
//      unset (QUIRK #2 / NMFTP1 has no Connect/Disconnect handlers at all).
// =============================================================================
static void test_wire_events()
{
    printf("\n[H] FTPClientEvt_WireNMFTP1Handlers\n");
    ResetAll();

    Nmftp::TNMFTP ftp(0);
    FTPClientEvt_WireNMFTP1Handlers(ftp);

    CHECK(static_cast<bool>(ftp.OnListItem),             "OnListItem wired");
    CHECK(static_cast<bool>(ftp.OnSuccess),               "OnSuccess wired");
    CHECK(static_cast<bool>(ftp.OnFailure),               "OnFailure wired");
    CHECK(static_cast<bool>(ftp.OnError),                 "OnError wired");
    CHECK(static_cast<bool>(ftp.OnStatus),                "OnStatus wired");
    CHECK(static_cast<bool>(ftp.OnConnectionFailed),      "OnConnectionFailed wired");
    CHECK(static_cast<bool>(ftp.OnTransactionStart),      "OnTransactionStart wired");
    CHECK(static_cast<bool>(ftp.OnTransactionStop),       "OnTransactionStop wired");
    CHECK(static_cast<bool>(ftp.OnAuthenticationFailed),  "OnAuthenticationFailed wired");
    CHECK(!static_cast<bool>(ftp.OnUnSupportedFunction),
          "OnUnSupportedFunction deliberately NOT wired (QUIRK #2 -- matches golden's own dead-wiring)");
    CHECK(!static_cast<bool>(ftp.OnConnect),
          "OnConnect not wired (NMFTP1 family has no such handler in golden at all)");
    CHECK(!static_cast<bool>(ftp.OnDisconnect),
          "OnDisconnect not wired either");

    // End-to-end sanity: firing OnSuccess through the wired slot reaches our
    // handler and observably sets bListOk, exactly as a real TNMFTP would.
    ResetAll();
    FTPClientEvt_WireNMFTP1Handlers(ftp);
    ftp.OnSuccess(cmdList);
    CHECK(bListOk == true, "firing the wired OnSuccess slot reaches FTPClientEvt_NMFTP1Success");
}

// =============================================================================
int main()
{
    std::setvbuf(stdout, 0, _IONBF, 0);
    printf("=====================================================================\n");
    printf(" test_FTPClient_EventHandlers : NMFTP1 event-handler bodies (golden\n");
    printf(" FTPClient.cpp:1799-2097)\n");
    printf("=====================================================================\n");

    test_success();
    test_failure();
    test_unsupported();
    test_auth_and_connection_failed();
    test_transaction_markers();
    test_error_and_status();
    test_list_item();
    test_wire_events();

    printf("\n---------------------------------------------------------------------\n");
    printf(" RESULT: %d passed, %d failed\n", g_pass, g_fail);
    printf("---------------------------------------------------------------------\n");
    return (g_fail == 0) ? 0 : 1;
}
