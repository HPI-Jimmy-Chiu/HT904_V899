// tests/test_urenesas_other.cpp
// Verification harness for the Automation/uRENESAS_Other.{h,cpp} translation
// (RENESAS host-protocol data-shape records + command/status/error-code
// constant tables).
//
// Exercises the translated public API against the exact literal values
// transcribed from the ORIGINAL golden reference
//   HT9011UC_Code_V3.33.906.0_20260618/Automation/uRENESAS_Other.h
//   HT9011UC_Code_V3.33.906.0_20260618/Automation/uRENESAS_Other.cpp
//
// LIMITATION (stated explicitly, same as test_ContactForce.cpp / test_cContact.cpp):
// we CANNOT run the original BCB6 binary (no Borland compiler in this
// environment).  Verification here is therefore:
//   (1) the translation compiles clean under MinGW g++ (-Wall -Wextra -Wshadow
//       -Wconversion -Wpedantic, zero warnings), AND
//   (2) its outputs equal the literal values transcribed directly from the
//       BCB6 source (golden file:line cited per case).
//
// No external test framework: a tiny check harness prints PASS/FAIL per case
// and a final summary, and returns non-zero on ANY failure.

#include "Automation/uRENESAS_Other.h"
#include <cstdio>
#include <string>

static int g_pass = 0;
static int g_fail = 0;

static void check_s(const char* name, const AnsiString& got, const char* expected)
{
    if (got == expected)
    {
        printf("PASS  %-46s got=\"%s\"\n", name, got.c_str());
        ++g_pass;
    }
    else
    {
        printf("FAIL  %-46s got=\"%s\"  exp=\"%s\"\n", name, got.c_str(), expected);
        ++g_fail;
    }
}

static void check_i(const char* name, int got, int expected)
{
    if (got == expected)
    {
        printf("PASS  %-46s got=%d\n", name, got);
        ++g_pass;
    }
    else
    {
        printf("FAIL  %-46s got=%d  exp=%d\n", name, got, expected);
        ++g_fail;
    }
}

int main()
{
    printf("=== Automation/uRENESAS_Other translation verification ===\n");
    printf("(values transcribed from BCB6 uRENESAS_Other.h/.cpp; original binary NOT run)\n\n");

    // -------------------------------------------------------------------
    // TRENESAS_CMD -- golden :171-233 (FGetXxx literals -> property table).
    // property->getter collapse: same identifier, now callable with ().
    // -------------------------------------------------------------------
    printf("-- TRENESAS_CMD (golden :171-233) --\n");
    check_s("OnLineCMD",              RENESAS_CMD.OnLineCMD(),              "00");
    check_s("OnLineReplyCMD",         RENESAS_CMD.OnLineReplyCMD(),         "01");
    check_s("ProductInfoCMD",         RENESAS_CMD.ProductInfoCMD(),         "10");
    check_s("ProductInfoReplyCMD",    RENESAS_CMD.ProductInfoReplyCMD(),    "11");
    check_s("StartCMD",               RENESAS_CMD.StartCMD(),               "20");
    check_s("StartReplyCMD",          RENESAS_CMD.StartReplyCMD(),          "21");
    check_s("HandlerStartCMD",        RENESAS_CMD.HandlerStartCMD(),        "50");
    check_s("HandlerStartReplyCMD",   RENESAS_CMD.HandlerStartReplyCMD(),   "51");
    check_s("EndCMD",                 RENESAS_CMD.EndCMD(),                 "30");
    check_s("EndReplyCMD",            RENESAS_CMD.EndReplyCMD(),            "31");
    check_s("LotEndCMD",              RENESAS_CMD.LotEndCMD(),              "40");
    check_s("LotEndReplyCMD",         RENESAS_CMD.LotEndReplyCMD(),         "41");
    check_s("TestEndCMD",             RENESAS_CMD.TestEndCMD(),             "70");   // Kirin 20220118
    check_s("TestEndReplyCMD",        RENESAS_CMD.TestEndReplyCMD(),        "71");   // Kirin 20220118
    check_s("SendAlarmCMD",           RENESAS_CMD.SendAlarmCMD(),           "90");   // Kirin 20211020
    check_s("SendAlarmReplyCMD",      RENESAS_CMD.SendAlarmReplyCMD(),      "91");   // Kirin 20211020
    check_s("SendCleanOutCMD",        RENESAS_CMD.SendCleanOutCMD(),        "B0");   // Kirin 20221115
    check_s("SendCleanOutReplyCMD",   RENESAS_CMD.SendCleanOutReplyCMD(),   "B1");   // Kirin 20221115

    // -------------------------------------------------------------------
    // TRENESAS_Status_Code -- golden :234-252.
    // -------------------------------------------------------------------
    printf("-- TRENESAS_Status_Code (golden :234-252) --\n");
    check_s("TestStart",     RENESAS_Status.TestStart(),     "00");
    check_s("ReTestStart",   RENESAS_Status.ReTestStart(),   "10");
    check_s("ReReTestStart", RENESAS_Status.ReReTestStart(), "20");
    check_s("TestEnd",       RENESAS_Status.TestEnd(),       "01");
    check_s("ReTestEnd",     RENESAS_Status.ReTestEnd(),     "11");
    check_s("ReReTestEnd",   RENESAS_Status.ReReTestEnd(),   "21");

    // -------------------------------------------------------------------
    // TRENESAS_Err_Index -- golden :330-355 (Kirin 20220127).
    // -------------------------------------------------------------------
    printf("-- TRENESAS_Err_Index (golden :330-355, Kirin 20220127) --\n");
    check_i("OK",                    RENESAS_Err_Index.OK(),                    0);
    check_i("HandlerNoHome",         RENESAS_Err_Index.HandlerNoHome(),         1);
    check_i("HandlerHaveDeviceData", RENESAS_Err_Index.HandlerHaveDeviceData(), 2);
    check_i("HandlerHaveDevice",     RENESAS_Err_Index.HandlerHaveDevice(),     3);
    check_i("CanNotLoadWorkFile",    RENESAS_Err_Index.CanNotLoadWorkFile(),    4);
    check_i("DisiableFTCT",          RENESAS_Err_Index.DisiableFTCT(),          5);
    check_i("OutOfOrder",            RENESAS_Err_Index.OutOfOrder(),            6);
    check_i("ARTIsDisable",          RENESAS_Err_Index.ARTIsDisable(),          7);
    check_i("RecvTestEnd",           RENESAS_Err_Index.RecvTestEnd(),           8);
    check_i("NoInputQty",            RENESAS_Err_Index.NoInputQty(),            9);   // Kirin 20220128
    check_i("NoRemoveTray",          RENESAS_Err_Index.NoRemoveTray(),          10);  // Kirin 20220714
    check_i("CleanOutFail",          RENESAS_Err_Index.CleanOutFail(),          11);  // Kirin 20221123
    check_i("CleanCountFail",        RENESAS_Err_Index.CleanCountFail(),        12);  // Kirin 20230814

    // -------------------------------------------------------------------
    // TRENESAS_EndFileData -- golden :253-284 (Kirin 20220218 InitData +
    // Kirin 20221115 "Add TwoPassBin").  Only assert what golden's ctor
    // actually guarantees: the ORIGINAL 8 fields are zeroed by InitData().
    // The 5 LATER fields (HdPickUp + the TwoPassBin quartet) are a FAITHFUL
    // GOLDEN GAP -- InitData() never touches them, so they are left
    // deliberately UNCHECKED here (their post-construction value is
    // indeterminate in both golden BCB6 and this translation; asserting a
    // specific value would be testing undefined behavior, not the
    // translation).  See uRENESAS_Other.h class-header comment.
    // -------------------------------------------------------------------
    printf("-- TRENESAS_EndFileData ctor->InitData() (golden :253-284) --\n");
    {
        TRENESAS_EndFileData d;
        check_i("HdPass",         d.HdPass,         0);
        check_i("HdFail",         d.HdFail,         0);
        check_i("HdRejectATPass", d.HdRejectATPass, 0);
        check_i("HdRejectATFail", d.HdRejectATFail, 0);
        check_i("HdRejectBT",     d.HdRejectBT,     0);
        check_i("TesterMatch",    d.TesterMatch,    0);
        check_i("TesterUnmatch",  d.TesterUnmatch,  0);
        check_i("TesterIgnore",   d.TesterIgnore,   0);

        // Re-InitData() after poking the checked fields must re-zero them
        // (exercises the golden method being called a 2nd time, not just
        // via the ctor).
        d.HdPass = 42;
        d.TesterIgnore = -7;
        d.InitData();
        check_i("HdPass after re-InitData()",       d.HdPass,       0);
        check_i("TesterIgnore after re-InitData()", d.TesterIgnore, 0);
    }

    // -------------------------------------------------------------------
    // Aggregate containers -- TRENESAS_RECV_CMD_DATA / TRENESAS_Send_CMD_DATA
    // (golden :113-125 / :211-225).  Pure composition, no logic: assign a
    // representative nested field on each of the 9 / 7 members and read it
    // back, proving the aggregate's member layout/types match golden.
    // -------------------------------------------------------------------
    printf("-- TRENESAS_RECV_CMD_DATA / TRENESAS_Send_CMD_DATA composition --\n");
    {
        TRENESAS_RECV_CMD_DATA recv;
        recv.OnLine.CMD          = "00";
        recv.OnLine.iRecv        = 1;
        recv.ProdInfo.LotID      = "LOT001";
        recv.ModeStart.Status    = "A";
        recv.HandlerStart.Date   = "20260710";
        recv.ModeEnd.Status      = "B";
        recv.LotEnd.LECD         = "X";
        recv.TestEnd.iRecv       = 2;
        recv.SendAlarm.CMD       = "90";
        recv.SendCleanOut.Date   = "20260710";

        check_s("recv.OnLine.CMD",        recv.OnLine.CMD,        "00");
        check_i("recv.OnLine.iRecv",      recv.OnLine.iRecv,      1);
        check_s("recv.ProdInfo.LotID",    recv.ProdInfo.LotID,    "LOT001");
        check_s("recv.ModeStart.Status",  recv.ModeStart.Status,  "A");
        check_s("recv.HandlerStart.Date", recv.HandlerStart.Date, "20260710");
        check_s("recv.ModeEnd.Status",    recv.ModeEnd.Status,    "B");
        check_s("recv.LotEnd.LECD",       recv.LotEnd.LECD,       "X");
        check_i("recv.TestEnd.iRecv",     recv.TestEnd.iRecv,     2);
        check_s("recv.SendAlarm.CMD",     recv.SendAlarm.CMD,     "90");
        check_s("recv.SendCleanOut.Date", recv.SendCleanOut.Date, "20260710");

        TRENESAS_Send_CMD_DATA send;
        send.OnLine_Reply.Result         = "OK";
        send.ProdInfo_Reply.Product      = "P1";
        send.ModeStart_Reply.Product     = "P2";
        send.HandlerStart_Reply.Comment  = "C1";
        send.ModeEnd_Reply.TotalQty      = "100";
        send.LotEnd_Reply.Result         = "OK";
        send.TestEnd_Reply.Comment       = "C2";

        check_s("send.OnLine_Reply.Result",        send.OnLine_Reply.Result,        "OK");
        check_s("send.ProdInfo_Reply.Product",     send.ProdInfo_Reply.Product,     "P1");
        check_s("send.ModeStart_Reply.Product",    send.ModeStart_Reply.Product,    "P2");
        check_s("send.HandlerStart_Reply.Comment", send.HandlerStart_Reply.Comment, "C1");
        check_s("send.ModeEnd_Reply.TotalQty",     send.ModeEnd_Reply.TotalQty,     "100");
        check_s("send.LotEnd_Reply.Result",        send.LotEnd_Reply.Result,        "OK");
        check_s("send.TestEnd_Reply.Comment",      send.TestEnd_Reply.Comment,      "C2");
    }

    // -------------------------------------------------------------------
    // Summary
    // -------------------------------------------------------------------
    printf("\n=== SUMMARY: %d passed, %d failed (total %d) ===\n",
           g_pass, g_fail, g_pass + g_fail);
    if (g_fail != 0)
    {
        printf("RESULT: FAIL\n");
        return 1;
    }
    printf("RESULT: ALL PASS\n");
    return 0;
}
