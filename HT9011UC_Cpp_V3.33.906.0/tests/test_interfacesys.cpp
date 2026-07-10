// =============================================================================
//  test_interfacesys.cpp  --  W5 VERIFY: Interface/InterfaceSYS (GPIB/ESD/
//                              AutoUpdate/EventLog WM_COPYDATA IPC bridge)
//
//  Translation wave: W5 (comms), whole-file unit.
//  Author: AI(W5-InterfaceSYS-Translate) 20260710
//
//  PURPOSE
//  -------
//  Proves Interface/InterfaceSYS.cpp (in lib ht9045_sm, see translate report
//  for the exact add_library wiring) LINKS and its ACTIVE logic behaves per
//  the golden reference, D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618 's
//  Interface/InterfaceSYS.cpp (READ-ONLY), with NO real GPIB/ESD/AutoUpdate/
//  EventLog external program attached (FindWindow legitimately returns NULL
//  in this offline test process -- that IS the golden's own "no such window"
//  safe-default path, exercised for real, not mocked).
//
//  NO EXTERNAL FRAMEWORK: a tiny check harness prints PASS/FAIL per case and
//  a final summary, and returns non-zero on ANY failure (same style as
//  tests/test_ContactForce.cpp / tests/test_cContact.cpp).
//
//  LOCAL STAND-IN GLOBALS (NOT production shims -- see translate report
//  section 4/5): golden main.cpp:15151-15156 defines bGPIBError / bEcho /
//  bUnderTest / bExist / iBin[4][8]; main.cpp itself is not translated yet
//  (W7).  InterfaceSYS.cpp (and atester.cpp) only `extern`-declare them.
//  AI(W5-Automation-Integrate) 20260710: bEcho/bExist/bUnderTest are now
//  DEFINED FOR REAL in atester_shims.cpp (ht9045_sm), needed by the new
//  atester_32Site.cpp translation -- this test TU's own former local copies
//  of those three would now be a duplicate-definition ODR clash at link, so
//  they were REMOVED here; this test just reads/writes the ht9045_sm-owned
//  globals instead (identical semantics -- extern bool, mutable, offline
//  default false).  bGPIBError / iBin[4][8] have no such shim yet (nothing
//  else in ht9045_sm defines them), so THIS test TU still supplies their
//  one-and-only definition for the whole link.
// =============================================================================
#include "Interface/InterfaceSYS.h"

#include "aHotPlateSubstrate.h"   // TestSocket (TMyKitSuck) / fBarCode
#include "canary_support.h"       // LastSet (.iTester)
#include "cprod.h"                // TestIF (.iSiteMap)
#include "cmydef.h"               // ON_LINE/OFF_LINE, TestISTimeOut, bTimeOutForNoFullSite

#include <cstdio>
#include <cstring>

static int g_pass = 0;
static int g_fail = 0;

static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected) { printf("PASS  %-58s got=%d\n", name, (int)got); ++g_pass; }
    else { printf("FAIL  %-58s got=%d exp=%d\n", name, (int)got, (int)expected); ++g_fail; }
}

static void check_i(const char* name, long got, long expected)
{
    if (got == expected) { printf("PASS  %-58s got=%ld\n", name, got); ++g_pass; }
    else { printf("FAIL  %-58s got=%ld exp=%ld\n", name, got, expected); ++g_fail; }
}

static void check_s(const char* name, const std::string& got, const std::string& expected)
{
    if (got == expected) { printf("PASS  %-58s got=\"%s\"\n", name, got.c_str()); ++g_pass; }
    else { printf("FAIL  %-58s got=\"%s\" exp=\"%s\"\n", name, got.c_str(), expected.c_str()); ++g_fail; }
}

// -----------------------------------------------------------------------
// LOCAL STAND-IN definitions for golden main.cpp's not-yet-translated
// globals (see file header note above).  bEcho/bExist/bUnderTest are now
// defined in atester_shims.cpp (ht9045_sm) -- NOT redefined here (see header
// note); only `extern`-declare them so this file's reads/writes bind to the
// real ht9045_sm-owned globals.
// -----------------------------------------------------------------------
bool bGPIBError = false;
extern bool bEcho, bExist, bUnderTest;
unsigned int iBin[4][8];

int main()
{
    printf("=== Interface/InterfaceSYS translation verification ===\n");
    printf("(no real GPIB/ESD/AutoUpdate/EventLog program attached; FindWindow()\n");
    printf(" legitimately returns NULL here -- that is golden's own safe default)\n\n");

    // -------------------------------------------------------------------
    // Byte typedef sanity (vclcompat does not have this primitive yet --
    // see translate report; locally guarded-typedef'd in InterfaceSYS.h).
    // -------------------------------------------------------------------
    check_i("sizeof(Byte)==1", (long)sizeof(Byte), 1);

    // -------------------------------------------------------------------
    // M_V default ctor (golden InterfaceSYS.h:236-245) zero-inits.
    // -------------------------------------------------------------------
    printf("-- M_V() default ctor --\n");
    {
        M_V v;
        check_i("bModeType==0",    v.bModeType, 0);
        check_i("bCommandType==0", v.bCommandType, 0);
        check_i("bCommand==0",     v.bCommand, 0);
        check_i("iDataSize==0",    (long)v.iDataSize, 0);
        check_i("iMessageSize==0", (long)v.iMessageSize, 0);
        check_i("bData[0]==0",     v.bData[0], 0);
        check_i("bMessage[0]==0",  v.bMessage[0], 0);
    }

    // -------------------------------------------------------------------
    // GPIBInterface_Data::PushData / PushMessage -- pure vector-building
    // logic, zero HAL / zero cross-module dependency (golden :74-138).
    // -------------------------------------------------------------------
    printf("-- GPIBInterface_Data::PushData/PushMessage (pure logic) --\n");
    {
        GPIBInterface_Data gd("NoSuchClass", "NoSuchCaption", 0x1234);

        int idata[3] = { 10, 20, 255 };
        gd.PushData(idata, 3);
        check_i("PushData(int[]) size",   (long)gd.vData.size(), 3);
        check_i("PushData(int[]) [0]",    gd.vData[0], 10);
        check_i("PushData(int[]) [1]",    gd.vData[1], 20);
        check_i("PushData(int[]) [2]",    gd.vData[2], 255);

        bool bdata[4] = { true, false, true, true };
        gd.PushData(bdata, 4);
        check_i("PushData(bool[]) size",  (long)gd.vData.size(), 4);
        check_i("PushData(bool[]) '1'",   gd.vData[0], (Byte)'1');
        check_i("PushData(bool[]) '0'",   gd.vData[1], (Byte)'0');
        check_i("PushData(bool[]) '1'#2", gd.vData[2], (Byte)'1');

        int imsg[2] = { 7, 8 };
        gd.PushMessage(imsg, 2);
        check_i("PushMessage(int[]) size", (long)gd.vMessage.size(), 2);
        check_i("PushMessage(int[]) [1]",  gd.vMessage[1], 8);

        bool bmsg[2] = { false, true };
        gd.PushMessage(bmsg, 2);
        check_i("PushMessage(bool[]) '0'", gd.vMessage[0], (Byte)'0');
        check_i("PushMessage(bool[]) '1'", gd.vMessage[1], (Byte)'1');

        Byte bytemsg[3] = { 1, 2, 3 };
        gd.PushMessage(bytemsg, 3);
        check_i("PushMessage(Byte[]) size", (long)gd.vMessage.size(), 3);
        check_i("PushMessage(Byte[]) [2]",  gd.vMessage[2], 3);

        std::vector<Byte> vsrc;
        vsrc.push_back(9); vsrc.push_back(9); vsrc.push_back(9);
        gd.PushMessage(vsrc, 3);
        check_i("PushMessage(vector<Byte>) size", (long)gd.vMessage.size(), 3);
        check_i("PushMessage(vector<Byte>) [0]",  gd.vMessage[0], 9);

        // Clear() (golden :158-165) resets scalar fields + both vectors.
        gd.bModeType = TYPE_HANDLER_GPIB; gd.bCommandType = CommandType_TEST; gd.bCommand = 0xFF;
        gd.Clear();
        check_i("Clear() bModeType",    gd.bModeType, 0);
        check_i("Clear() bCommandType", gd.bCommandType, 0);
        check_i("Clear() bCommand",     gd.bCommand, 0);
        check_i("Clear() vData empty",    (long)gd.vData.size(), 0);
        check_i("Clear() vMessage empty", (long)gd.vMessage.size(), 0);

        // bFindProgram() (golden :68-75): no such window exists in this test
        // process -- real Win32 FindWindow(), not mocked -- must return false.
        check_b("bFindProgram() -> false (no such window)", gd.bFindProgram(), false);

        // SendStructMessage_SendMessage() (golden :132-157) early-returns when
        // bFindProgram()==false -- must not crash and must be a true no-op.
        gd.SendStructMessage_SendMessage();
        printf("PASS  %-58s (no crash)\n", "SendStructMessage_SendMessage() no-op");
        ++g_pass;
    }

    // NOTE: `_CloseGpibProgram` (golden :168-176) has no header declaration --
    // by golden design it is private to Interface/InterfaceSYS.cpp, reachable
    // only via _OnMyCopyMsg_Interface's CommandType_CLOSE route.  Exercised
    // below in the "_OnMyCopyMsg_Interface dispatch: CommandType_CLOSE" block
    // instead of calling it directly, so this test does not widen the
    // module's public surface beyond what golden itself exposes.

    // -------------------------------------------------------------------
    // _RunTestProgram / _RunTestProgram_BarMess (golden :185-232): the exact
    // functions atester.cpp's gated GetTesterResult calls at golden :1521/
    // :1525 (see translate report) once its own gate is lifted.
    // -------------------------------------------------------------------
    printf("-- _RunTestProgram (LastSet.iTester==ON_LINE, TestISTimeOut==false) --\n");
    {
        LastSet.iTester = ON_LINE;
        TestISTimeOut = false;
        bool sites[4] = { true, false, true, true };
        bool ok = _RunTestProgram(4, sites);
        check_b("_RunTestProgram() returns true", ok, true);
        check_b("bEcho==false after call",      bEcho, false);
        check_b("bExist==false after call",     bExist, false);
        check_b("bUnderTest==true after call",  bUnderTest, true);
        check_i("GPIBSend->bModeType",    GPIBSend->bModeType, TYPE_HANDLER_GPIB);
        check_i("GPIBSend->bCommandType", GPIBSend->bCommandType, CommandType_TEST);
        check_b("bCommand has TEST_REAL bit (ON_LINE)",
                (GPIBSend->bCommand & CommandType_TEST_REAL) != 0, true);
        check_b("bCommand has NO TEST_TIMEOUT bit (TestISTimeOut==false)",
                (GPIBSend->bCommand & CommandType_TEST_TIMEOUT) != 0, false);
        check_i("vData size == iDataSize",  (long)GPIBSend->vData.size(), 4);
        check_i("vData[0] == '1' (true)",   GPIBSend->vData[0], (Byte)'1');
        check_i("vData[1] == '0' (false)",  GPIBSend->vData[1], (Byte)'0');
    }
    printf("-- _RunTestProgram (LastSet.iTester==OFF_LINE, TestISTimeOut==true) --\n");
    {
        LastSet.iTester = OFF_LINE;
        TestISTimeOut = true;
        bool sites[2] = { false, false };
        _RunTestProgram(2, sites);
        check_b("bCommand has NO TEST_REAL bit (OFF_LINE)",
                (GPIBSend->bCommand & CommandType_TEST_REAL) != 0, false);
        check_b("bCommand HAS TEST_TIMEOUT bit (TestISTimeOut==true)",
                (GPIBSend->bCommand & CommandType_TEST_TIMEOUT) != 0, true);
        TestISTimeOut = false; // restore for subsequent checks
    }
    printf("-- _RunTestProgram_BarMess --\n");
    {
        LastSet.iTester = ON_LINE;
        bool sites[2] = { true, true };
        Byte msg[3] = { 'O', 'K', 0 };
        bool ok = _RunTestProgram_BarMess(2, sites, 3, msg);
        check_b("_RunTestProgram_BarMess() returns true", ok, true);
        check_i("GPIBSend->bCommandType", GPIBSend->bCommandType, CommandType_BARCODE);
        check_i("vMessage size == iMessageSize", (long)GPIBSend->vMessage.size(), 3);
        check_i("vMessage[0] == 'O'", GPIBSend->vMessage[0], (Byte)'O');
    }

    // -------------------------------------------------------------------
    // _SendADDRToInterfaceProgram / _SendInformationToInterfaceProgram
    // (golden :234-256): simple field-setters + PushData, no gaps.
    // -------------------------------------------------------------------
    printf("-- _SendADDRToInterfaceProgram --\n");
    {
        _SendADDRToInterfaceProgram(42);
        check_i("GPIBSend->bModeType",    GPIBSend->bModeType, TYPE_HANDLER_GPIB);
        check_i("GPIBSend->bCommandType", GPIBSend->bCommandType, CommandType_INFSEND);
        check_i("GPIBSend->bCommand",     GPIBSend->bCommand, CommandType_INFSEND_SENDGPIBADDR);
        check_i("vData[0] == 42",         (long)GPIBSend->vData[0], 42);
    }
    printf("-- _SendInformationToInterfaceProgram --\n");
    {
        int idat[2] = { 5, 6 };
        _SendInformationToInterfaceProgram((Byte)TYPE_HANDLER_GPIB, (Byte)CommandType_TEST, 0x02, idat, 2);
        check_i("GPIBSend->bModeType",    GPIBSend->bModeType, TYPE_HANDLER_GPIB);
        check_i("GPIBSend->bCommandType", GPIBSend->bCommandType, CommandType_TEST);
        check_i("GPIBSend->bCommand",     GPIBSend->bCommand, 0x02);
        check_i("vData size",             (long)GPIBSend->vData.size(), 2);
    }

    // -------------------------------------------------------------------
    // _GPIBGetCommand_TEST (golden :256-370, private to the TU; exercised
    // indirectly via _OnMyCopyMsg_Interface below, and here directly is not
    // possible since it has no header declaration -- matching golden, which
    // also never exposes it outside the TU).  So the two branches are
    // exercised through _OnMyCopyMsg_Interface's CommandType_TESTECHO route.
    // -------------------------------------------------------------------
    printf("-- _OnMyCopyMsg_Interface dispatch + _GPIBGetCommand_TEST (TESTECHOOK bit set) --\n");
    {
        // Controlled small grid: 2x2 (within TestSocket's default 2x1 / bounds
        // 4x8) so the iBin[][] decode is easy to hand-verify.
        TestSocket.iShtRow = 2;
        TestSocket.iShtCol = 2;
        TestIF.iSiteMap[0][0] = 1;   // -> iBin[0][0] = bData[0]
        TestIF.iSiteMap[0][1] = 2;   // -> iBin[0][1] = bData[1]
        TestIF.iSiteMap[1][0] = 0;   // <=0 -> iBin[1][0] = 0 (golden Hung 20111108 guard)
        TestIF.iSiteMap[1][1] = 3;   // -> iBin[1][1] = bData[2]
        for (int i = 0; i < 4; ++i) for (int j = 0; j < 8; ++j) iBin[i][j] = 0xDEAD;

        M_V pv;
        pv.bModeType    = TYPE_HANDLER_GPIB;
        pv.bCommandType = CommandType_TESTECHO;
        pv.bCommand     = CommandType_TESTECHOOK;   // takes the TESTECHOOK branch
        pv.iDataSize    = 3;
        pv.bData[0] = 11; pv.bData[1] = 22; pv.bData[2] = 33;
        const char* msg = "OK";                     // no "BARCODEERR:" prefix -> else-arm (gated, no crash)
        pv.iMessageSize = (unsigned int)strlen(msg);
        memcpy(pv.bMessage, msg, strlen(msg));

        COPYDATASTRUCT cds;
        cds.dwData = 0;
        cds.cbData = sizeof(M_V);
        cds.lpData = &pv;

        bEcho = false; bExist = false; bUnderTest = true; bGPIBError = true; // sentinels
        _OnMyCopyMsg_Interface((LPARAM)&cds);

        check_i("iBin[0][0] == bData[0] (site 1)", (long)iBin[0][0], 11);
        check_i("iBin[0][1] == bData[1] (site 2)", (long)iBin[0][1], 22);
        check_i("iBin[1][0] == 0 (siteMap<=0 guard)", (long)iBin[1][0], 0);
        check_i("iBin[1][1] == bData[2] (site 3)", (long)iBin[1][1], 33);
        // Golden TESTECHOOK-branch tail (:349-353): bGPIBError=false, bEcho=true,
        // bUnderTest=false, bExist=true -- unconditionally, "BARCODEERR:" or not.
        check_b("bGPIBError==false (TESTECHOOK branch)", bGPIBError, false);
        check_b("bEcho==true",     bEcho, true);
        check_b("bUnderTest==false", bUnderTest, false);
        check_b("bExist==true",    bExist, true);
    }

    printf("-- _OnMyCopyMsg_Interface dispatch + _GPIBGetCommand_TEST (TESTECHOOK bit clear) --\n");
    {
        for (int i = 0; i < 4; ++i) for (int j = 0; j < 8; ++j) iBin[i][j] = 0xDEAD;

        M_V pv;
        pv.bModeType    = TYPE_HANDLER_GPIB;
        pv.bCommandType = CommandType_TESTECHO;
        pv.bCommand     = 0x00;                     // TESTECHOOK bit clear -> else branch
        pv.iDataSize    = 3;
        pv.bData[0] = 44; pv.bData[1] = 55; pv.bData[2] = 66;

        COPYDATASTRUCT cds;
        cds.dwData = 0; cds.cbData = sizeof(M_V); cds.lpData = &pv;

        _OnMyCopyMsg_Interface((LPARAM)&cds);

        check_i("iBin[0][0] == bData[0] (site 1)", (long)iBin[0][0], 44);
        check_i("iBin[0][1] == bData[1] (site 2)", (long)iBin[0][1], 55);
        check_i("iBin[1][1] == bData[2] (site 3)", (long)iBin[1][1], 66);
        // FAITHFUL golden ASYMMETRY (:363 vs :349): the non-TESTECHOOK ("else")
        // arm sets bGPIBError=TRUE, unlike the TESTECHOOK arm's false above --
        // a golden twin-branch quirk preserved verbatim, not "fixed".
        check_b("bGPIBError==true (else branch -- ASYMMETRIC vs TESTECHOOK arm)", bGPIBError, true);
        check_b("bEcho==true",      bEcho, true);
        check_b("bUnderTest==false", bUnderTest, false);
        check_b("bExist==true",     bExist, true);
    }

    printf("-- _OnMyCopyMsg_Interface dispatch: CommandType_CLOSE routes to _CloseGpibProgram --\n");
    {
        GPIBSend->bCommandType = 0xAA; // sentinel
        M_V pv;
        pv.bModeType    = TYPE_HANDLER_GPIB;
        pv.bCommandType = CommandType_CLOSE;
        COPYDATASTRUCT cds; cds.dwData = 0; cds.cbData = sizeof(M_V); cds.lpData = &pv;
        _OnMyCopyMsg_Interface((LPARAM)&cds);
        check_i("GPIBSend->bCommandType == CommandType_CLOSE (dispatched)",
                GPIBSend->bCommandType, CommandType_CLOSE);
    }

    printf("-- _OnMyCopyMsg_Interface dispatch: non-GPIB bModeType is a no-op --\n");
    {
        bEcho = false; // sentinel: must remain false (no TESTECHO branch taken)
        M_V pv;
        pv.bModeType    = 0xFF;                       // != TYPE_HANDLER_GPIB
        pv.bCommandType = CommandType_TESTECHO;
        COPYDATASTRUCT cds; cds.dwData = 0; cds.cbData = sizeof(M_V); cds.lpData = &pv;
        _OnMyCopyMsg_Interface((LPARAM)&cds);
        check_b("bEcho unchanged (dispatch ignored non-GPIB bModeType)", bEcho, false);
    }

    // -------------------------------------------------------------------
    // Direct oracle for the 1-based AnsiString "BARCODEERR:" parse ladder
    // that _GPIBGetCommand_TEST runs (golden :296-322).  fBarCode's write of
    // the parsed tokens is currently gated (see translate report), so the
    // token COUNT (k) has no externally observable effect through the public
    // API today; this block re-runs the IDENTICAL AnsiString call sequence
    // standalone to prove the 1-based Pos/Delete/SubString/operator[]
    // semantics compose correctly for this exact real-world usage pattern
    // (the same class of bug KNOWLEDGE.md flags as the #1 migration risk).
    // -------------------------------------------------------------------
    printf("-- direct oracle: golden's BARCODEERR AnsiString parse ladder (:296-322) --\n");
    {
        AnsiString s = "BARCODEERR:AAA,BBB,CCC\r";
        check_b("Pos(\"BARCODEERR:\")==1", s.Pos("BARCODEERR:") == 1, true);
        s.Delete(1, 11);
        check_s("after Delete(1,11)", s.c_str(), "AAA,BBB,CCC\r");
        if (s.Pos(";") != 0) s = s.SubString(1, s.Pos(";") - 1);
        if (s.Pos("\r") != 0) s = s.SubString(1, s.Pos("\r") - 1);
        check_s("after \\r strip", s.c_str(), "AAA,BBB,CCC");

        enum { kTestGpibMaxItem = 8 };   // local stand-in for the .cpp-private `iGPIBMAXItem` (32);
                                         // only need room for 3 tokens here.
        AnsiString asReMess[kTestGpibMaxItem] = { "" };
        int k = 0;
        for (int j = 0; j < s.Length(); j++)
        {
            if (s[j + 1] != ',')
                asReMess[k] = asReMess[k] + s[j + 1];
            else
                k++;
        }
        check_i("token count k (2 commas -> k==2)", k, 2);
        check_s("asReMess[0]==\"AAA\"", asReMess[0].c_str(), "AAA");
        check_s("asReMess[1]==\"BBB\"", asReMess[1].c_str(), "BBB");
        check_s("asReMess[2]==\"CCC\"", asReMess[2].c_str(), "CCC");
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
