// =============================================================================
//  tests/test_agv_portscan.cpp  --  VERIFY: Automation/AGV_PortScan.{h,cpp}
//
//  Translation wave: W5-Automation-cont (AGV_PortScan unit)
//  Author: AI(W5-Automation-PortScan) 20260713
//
//  Same style/limitations as tests/test_agv_e84.cpp (read that file's own
//  header first if unfamiliar): no Borland binary exists, so "equivalence"
//  here == clean g++ compile/link + observable state landing exactly where
//  the golden switch(Task)/if-ladder logic says it must, given a known,
//  deterministic Sim-HAL / global-config input. Oracles hand-derived from the
//  golden source (see AGV_PortScan.h/.cpp comments for the exact golden line
//  citations), NOT from this translation.
//
//  SCOPE NOTE: NowE84Status()/E84StatusChange() write/read a `static`
//  (TU-local, not header-exposed) comparison array -- same information-
//  hiding choice AGV_E84.cpp made for E84LoadDelay/E84UnLoadDelay. There is
//  no accessor to assert the internal diff logic directly, so those two are
//  SMOKE-tested only (must not crash across a state transition) -- a
//  logging-only function pair, proportionate coverage (matches
//  test_agv_e84.cpp's own O5 "logging smoke test" precedent).
// =============================================================================
#include "vclcompat/vcl_compat.h"
#include "cmydef.h"
#include "MachineType.h"
#include "cprod.h"
#include "mysensor.h"
#include "myswitch.h"
#include "Motor/mymotor.h"
#include "canary_support.h"
#include "FormsFacade.h"
#include "Automation/AGV_E84.h"
#include "Automation/AGV_predicates.h"
#include "Automation/AGV_PortScan.h"
#include "SECSGEM/SecsEventType.h"
#include "SECSGEM/SecsEventReport.h"
#include <cstdio>

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as test_agv_e84.cpp)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---------------------------------------------------------------------------
//  Sim-HAL forcing helpers (mirrors test_agv_e84.cpp's own helpers exactly --
//  sentinel ISABase == -1 matches none of the real routing enum values).
// ---------------------------------------------------------------------------
static const int kSentinelISABase = -1;

static void ForceSensorOn(TMySensor &s)
{
    s.Enable  = true;
    s.ISABase = kSentinelISABase;
    s.Type    = 0;      // IsOn() -> !ret, ret stays false -> true
}

static void ForceSensorOff(TMySensor &s)
{
    s.Enable  = true;
    s.ISABase = kSentinelISABase;
    s.Type    = 1;      // IsOn() -> ret, ret stays false -> false (IsOff()==true)
}

int main()
{
    printf("=== AGV_PortScan (E84 telemetry + AMR/AGV port-scan family) verification ===\n");
    printf("(no Borland binary available; verification = compiles/links clean +\n");
    printf(" state landing exactly where the golden switch/if ladder says it must)\n\n");

    // -----------------------------------------------------------------------
    // [O1] bIsStackBusy -- always true, in-range or out-of-range (golden
    // :1385-1408: the guarded if-body is an empty stub, never filled in).
    // -----------------------------------------------------------------------
    printf("[O1] bIsStackBusy() -- unconditional true (golden stub, empty if-body)\n");
    CHECK(bIsStackBusy(1)  == true, "bIsStackBusy(1)  == true (Loader port, in-range)");
    CHECK(bIsStackBusy(15) == true, "bIsStackBusy(15) == true (Fix6 port, in-range)");
    CHECK(bIsStackBusy(0)  == true, "bIsStackBusy(0)  == true (out-of-range low, guard body still empty)");
    CHECK(bIsStackBusy(16) == true, "bIsStackBusy(16) == true (out-of-range high, guard body still empty)");

    // -----------------------------------------------------------------------
    // [O2] bScanFixPortState_SPIL -- busy-gate early return, Empty/Arrived
    // transition, and the EventReport(SECS_EVENT.Fix1PortStatusChanged+iPos)
    // oracle (golden :1549-1580).
    // -----------------------------------------------------------------------
    printf("\n[O2] bScanFixPortState_SPIL(iPos) -- busy gate + Empty/Arrived + EventReport\n");
    {
        const int iPos = 0;                       // Fix1 -> iAutoIndex[eFix1+0]==0 -> iFix==0
        bPortIsBusy[ePortFix1+iPos] = true;
        iPortStatus[ePortFix1+iPos] = -999;        // sentinel: must NOT change while busy
        ResetSimEventReport();
        bScanFixPortState_SPIL(iPos);
        CHECK(iPortStatus[ePortFix1+iPos] == -999, "busy gate: iPortStatus untouched while bPortIsBusy==true");
        CHECK(g_SimEventReportCount == 0,          "busy gate: EventReport NOT called while bPortIsBusy==true");

        bPortIsBusy[ePortFix1+iPos] = false;
        iLastPortStatus[ePortFix1+iPos] = -999;    // force a "changed" transition on the next call
        ForceSensorOff(Sen[SnFixedTrayDetect[0]]);  // iFix==0 -> IsOff()==true -> eFixEmpty branch
        ResetSimEventReport();
        bScanFixPortState_SPIL(iPos);
        CHECK(iPortStatus[ePortFix1+iPos] == eFixEmpty,     "Sen OFF -> iPortStatus==eFixEmpty");
        CHECK(iLastPortStatus[ePortFix1+iPos] == eFixEmpty, "Sen OFF -> iLastPortStatus==eFixEmpty");
        CHECK(g_SimLastEventReportCeid == SECS_EVENT.Fix1PortStatusChanged+iPos,
              "Sen OFF -> EventReport(SECS_EVENT.Fix1PortStatusChanged+0)");
        CHECK(g_SimEventReportCount == 1, "Sen OFF -> EventReport fired exactly once (state genuinely changed)");

        ForceSensorOn(Sen[SnFixedTrayDetect[0]]);   // now IsOn()==true -> eFixFTrayArrived branch
        ResetSimEventReport();
        bScanFixPortState_SPIL(iPos);
        CHECK(iPortStatus[ePortFix1+iPos] == eFixFTrayArrived, "Sen ON -> iPortStatus==eFixFTrayArrived");
        CHECK(g_SimEventReportCount == 1, "Sen ON -> EventReport fired again (state changed Empty->Arrived)");
    }

    // -----------------------------------------------------------------------
    // [O3] bScanLoadPortState_Analog / bScanUnLoadPortState_Analog /
    // bScanFixPortState_Analog -- deterministic sensor combo -> exact
    // candidateState formula + EventReport oracle (golden :1582-1702).
    // -----------------------------------------------------------------------
    printf("\n[O3] *_Analog family -- deterministic sensor combo -> exact state + EventReport\n");
    {
        // bScanLoadPortState_Analog(0) (Loader): inner=3 (SureTray ON),
        // outer=4 (TrayIsFull.Enable==false so the Enable&&IsOn() branch is
        // skipped entirely, falls to the SelectHasTray.IsOff() branch) ->
        // candidateState = 3*10+4 = 34 (golden :1592-1614).
        ForceSensorOn(Sen[SnLoaderSureTray]);
        Sen[SnLoaderIsFull].Enable = false;         // golden: `Sen[...].Enable && Sen[...].IsOn()` short-circuits false
        ForceSensorOff(Sen[SnLoaderTrayHasTray]);
        iLastPortStatus[ePortLoader] = -999;
        ResetSimEventReport();
        bScanLoadPortState_Analog(0);
        CHECK(iPortStatus[ePortLoader] == 34, "bScanLoadPortState_Analog(0) -> iPortStatus[ePortLoader]==34 (3*10+4)");
        CHECK(g_SimLastEventReportCeid == SECS_EVENT.LoadPortStatusChanged+0,
              "bScanLoadPortState_Analog(0) -> EventReport(SECS_EVENT.LoadPortStatusChanged+0)");

        // bScanUnLoadPortState_Analog(0) (Auto1): inner=5 (TrayDetect OFF),
        // outer=1 (TrayIsFull.Enable==false, SureHasTray ON) ->
        // candidateState = 5*10+1 = 51 (golden :1634-1656).
        ForceSensorOff(Sen[SnAuto1TrayDetect]);
        Sen[SnAuto1IsFull].Enable = false;
        ForceSensorOn(Sen[SnAuto1_Tray_Car]);
        iLastPortStatus[ePortAuto1] = -999;
        ResetSimEventReport();
        bScanUnLoadPortState_Analog(0);
        CHECK(iPortStatus[ePortAuto1] == 51, "bScanUnLoadPortState_Analog(0) -> iPortStatus[ePortAuto1]==51 (5*10+1)");
        CHECK(g_SimLastEventReportCeid == SECS_EVENT.Auto1PortStatusChanged+0,
              "bScanUnLoadPortState_Analog(0) -> EventReport(SECS_EVENT.Auto1PortStatusChanged+0)");

        // bScanFixPortState_Analog(0) (Fix1): Detect OFF -> currentState=5,
        // then HowManyDevice()==0 (offline Sim default, no config loaded) ->
        // overridden to currentState=1 (golden :1673-1692: the iCountFix==0
        // branch runs unconditionally after the Detect check, since it is a
        // separate `if`, not `else if`).
        ForceSensorOff(Sen[SnFixedTrayDetect[0]]);
        iLastPortStatus[ePortFix1] = -999;
        ResetSimEventReport();
        bScanFixPortState_Analog(0);
        CHECK(iPortStatus[ePortFix1] == 1, "bScanFixPortState_Analog(0) -> iPortStatus[ePortFix1]==1 (HowManyDevice()==0 override)");
        CHECK(g_SimLastEventReportCeid == SECS_EVENT.Fix1PortStatusChanged+0,
              "bScanFixPortState_Analog(0) -> EventReport(SECS_EVENT.Fix1PortStatusChanged+0)");
    }

    // -----------------------------------------------------------------------
    // [O4] bScanLoadPortState_ATK -- first-call bFirstIn init sweep, then a
    // forced Loader-tray transition -> EventReport(PortStateUpdated) oracle
    // (golden :1716-1896).
    // -----------------------------------------------------------------------
    printf("\n[O4] bScanLoadPortState_ATK() -- bFirstIn sweep + forced transition\n");
    {
        // NOTE: `eATKPortStatus` (eATKNoState=0/eATKTransfer=1/eATKReadyToLoad=2/
        // eATKReadyToUnload=3) is deliberately TU-local to AGV_PortScan.cpp,
        // NOT exposed via the header -- matches golden itself, which also
        // declares this enum only inside AGV.cpp, never AGV.h (verified by
        // grep). This test therefore asserts on the equivalent integer
        // literals directly, annotated with the enumerator name they mean.
        MOT[MMTrayZ].fHasTray = false;
        MOT[MMTrayY].fHasTray = false;
        ResetSimEventReport();
        bScanLoadPortState_ATK();     // 1st call ever in this process: bFirstIn sweep runs
        CHECK(iLoadStateATK[etLoader] == 2 /*eATKReadyToLoad*/,
              "1st call: no tray anywhere -> etLoader settles at eATKReadyToLoad(2)");
        unsigned long countAfterFirst = g_SimEventReportCount;
        CHECK(countAfterFirst >= 1, "1st call: at least one PortStateUpdated fired (NoState -> ReadyToLoad transitions)");

        MOT[MMTrayZ].fHasTray = true;   // force a genuine Loader-tray-present transition
        ResetSimEventReport();
        bScanLoadPortState_ATK();
        CHECK(iCurrStateATK[etLoader] == 1 /*eATKTransfer*/, "MOT[MMTrayZ].fHasTray=true -> etLoader==eATKTransfer(1)");
        CHECK(iThisPortNo == ePortLoader,               "iThisPortNo==ePortLoader on the Loader transition");
        CHECK(iThisPortStatus == 1 /*eATKTransfer*/,    "iThisPortStatus==eATKTransfer(1) on the Loader transition");
        CHECK(g_SimLastEventReportCeid == SECS_EVENT.PortStateUpdated,
              "EventReport(SECS_EVENT.PortStateUpdated) fired for the Loader transition");
        CHECK(g_SimEventReportCount == 1, "exactly one EventReport this tick (only Loader's state actually changed)");

        MOT[MMTrayZ].fHasTray = false;   // restore -- avoid leaking state into later sections
    }

    // -----------------------------------------------------------------------
    // [O5] ScanLoadPort() dispatcher -- SPIL / Analog / ATK routing (golden
    // :1350-1383). Each branch's ACTIVE call is verified via an observable
    // side effect unique to that branch's callee.
    // -----------------------------------------------------------------------
    printf("\n[O5] ScanLoadPort() dispatcher routing\n");
    {
        // -- SPIL branch: fLotInfo->ALedLoader->Value / aLedAuto[]->Value are
        //    written UNCONDITIONALLY at the top of bScanLoadPortState_SPIL /
        //    bScanUnLoadPortState_SPIL, before any early-return -- a
        //    dependency-free routing oracle (golden :1420,:1500-1502).
        IniConfig.bA65_BundleIDList = true;
        IniConfig.bSPILFunction     = true;
        CUSTOMER_CODE = 0;                 // must NOT equal CC_MAXIM/_THAILAND (would be unreachable anyway: SPIL checked first)
        bAskStopPort[ePortLoader] = true;
        bAskStopPort[ePortAuto1]  = true;
        bAskStopPort[ePortAuto2]  = false;
        bAskStopPort[ePortAuto3]  = true;
        fLotInfo->ALedLoader->Value  = false;
        fLotInfo->aLedAuto[0]->Value = true;   // will be overwritten to bAskStopPort[ePortAuto1]==true (no visible change, but exercised)
        fLotInfo->aLedAuto[1]->Value = false;
        fLotInfo->aLedAuto[2]->Value = false;
        bPortIsBusy[ePortLoader] = true;       // short-circuit the rest of bScanLoadPortState_SPIL's body (not under test here)
        bPortIsBusy[ePortAuto1]  = true;
        bPortIsBusy[ePortAuto2]  = true;
        bPortIsBusy[ePortAuto3]  = true;
        ScanLoadPort();
        CHECK(fLotInfo->ALedLoader->Value  == true,  "SPIL branch routed: fLotInfo->ALedLoader->Value == bAskStopPort[ePortLoader]");
        CHECK(fLotInfo->aLedAuto[0]->Value == true,  "SPIL branch routed: fLotInfo->aLedAuto[0]->Value == bAskStopPort[ePortAuto1]");
        CHECK(fLotInfo->aLedAuto[1]->Value == false, "SPIL branch routed: fLotInfo->aLedAuto[1]->Value == bAskStopPort[ePortAuto2]");
        CHECK(fLotInfo->aLedAuto[2]->Value == true,  "SPIL branch routed: fLotInfo->aLedAuto[2]->Value == bAskStopPort[ePortAuto3]");

        // -- Analog branch: CUSTOMER_CODE==CC_MAXIM -- verified via the same
        //    iPortStatus[ePortLoader] oracle as [O3] above.
        IniConfig.bA65_BundleIDList = false;   // fall out of the SPIL branch
        IniConfig.bSPILFunction     = false;
        CUSTOMER_CODE = CC_MAXIM;
        ForceSensorOn(Sen[SnLoaderSureTray]);
        Sen[SnLoaderIsFull].Enable = false;
        ForceSensorOff(Sen[SnLoaderTrayHasTray]);
        iLastPortStatus[ePortLoader] = -999;
        ScanLoadPort();
        CHECK(iPortStatus[ePortLoader] == 34, "Analog branch routed: bScanLoadPortState_Analog(0) ran -> iPortStatus[ePortLoader]==34");

        // -- ATK branch: AGV_IsATK_AMR()==true (USE_COVER_TRAYID==tCID_NFC &&
        //    bA65_BundleIDList==true && CUSTOMER_CODE==CC_AMKOR_Korea) --
        //    verified via the same iThisPortNo/EventReport oracle as [O4].
        CUSTOMER_CODE = CC_AMKOR_Korea;
        USE_COVER_TRAYID = tCID_NFC;
        IniConfig.bA65_BundleIDList = true;
        CHECK(AGV_IsATK_AMR() == true, "precondition: AGV_IsATK_AMR()==true under this config");
        // Reset the sticky-by-design ATK "last reported" state + iThisPortNo/
        // iThisPortStatus to sentinels BEFORE forcing the transition, so this
        // check proves a GENUINE transition fired on THIS call rather than
        // silently re-observing a stale value left over from [O4] above
        // (iLoadStateATK[etLoader] persists across calls by design -- it is
        // the golden "last reported" latch, golden :1742/:1747).
        iLoadStateATK[etLoader] = 2 /*eATKReadyToLoad*/;   // matches the "no tray" state (fHasTray is still false here)
        iThisPortNo = -999;
        iThisPortStatus = -999;
        MOT[MMTrayZ].fHasTray = true;
        ResetSimEventReport();
        ScanLoadPort();
        CHECK(iThisPortNo == ePortLoader,      "ATK branch routed: bScanLoadPortState_ATK() ran -> iThisPortNo==ePortLoader");
        CHECK(iThisPortStatus == 1 /*eATKTransfer*/, "ATK branch routed: iThisPortStatus==eATKTransfer(1) (genuine transition, not stale)");
        CHECK(g_SimEventReportCount >= 1,      "ATK branch routed: EventReport fired");
        MOT[MMTrayZ].fHasTray = false;          // restore
        USE_COVER_TRAYID = tCIDNotUse;          // restore
        IniConfig.bA65_BundleIDList = false;    // restore
        CUSTOMER_CODE = 0;                      // restore
    }

    // -----------------------------------------------------------------------
    // [O6] DoE84LoaderScan(Pos) -- SystemStart guard, case 1->100->1000 cycle,
    // and direct-seeded case 2000/2100 (golden :1149-1217).
    // -----------------------------------------------------------------------
    printf("\n[O6] DoE84LoaderScan(Pos) -- SystemStart guard + case ladder\n");
    {
        const int Pos = 0;
        SystemStart = false;
        iLoaderScanTask[Pos] = 1;
        DoE84LoaderScan(Pos);
        CHECK(iLoaderScanTask[Pos] == 1, "SystemStart==false -> guard returns immediately, cursor unmoved");

        SystemStart = true;
        fMain->labAutomation->Caption = "";     // != "SECS GEM 4:Enable"
        DoE84LoaderScan(Pos);   // case 1 -> 100
        CHECK(iLoaderScanTask[Pos] == 100, "case 1 -> 100 (bE84Loaderflag reset, unconditional advance)");
        CHECK(bE84Loaderflag[Pos] == false, "case 1 sets bE84Loaderflag[Pos]=false");

        DoE84LoaderScan(Pos);   // case 100, Caption mismatch -> stays at 100
        CHECK(iLoaderScanTask[Pos] == 100, "case 100: Caption != \"SECS GEM 4:Enable\" -> cursor stays at 100");

        fMain->labAutomation->Caption = "SECS GEM 4:Enable";
        DoE84LoaderScan(Pos);   // case 100 -> 1000
        CHECK(iLoaderScanTask[Pos] == 1000, "case 100: Caption matches -> advances to 1000");

        ForceSensorOn(Sen[SnLoaderTrayHasTray_AGV]);   // IsOff()==false -> the cycle-back else branch
        DoE84LoaderScan(Pos);   // case 1000, Sen ON -> cycles back to 100
        CHECK(iLoaderScanTask[Pos] == 100, "case 1000: Sen ON (tray present) -> cycles back to 100 (golden :1193-1197)");

        // Direct-seed case 2000 (bypasses the real-time iCount>=5 accumulation
        // -- same "seed the cursor directly" technique used elsewhere in this
        // test suite for hard-to-reach-via-real-ticks cases).
        iTrayEnable[0]=3; iTrayEnable[1]=7; iTrayEnable[2]=2;
        iLoaderScanTask[Pos] = 2000;
        DoE84LoaderScan(Pos);
        CHECK(iLoaderScanTask[Pos] == 100, "case 2000 -> 100 (asSupplementBin formatted, iTrayEnable[Pos] cleared)");
        CHECK(iTrayEnable[Pos] == 0, "case 2000: iTrayEnable[Pos] reset to 0");
        CHECK(asSupplementBin == "P1:3,P2:7,P3:2", "case 2000: asSupplementBin formatted verbatim (golden :1205)");

        // case 2100: bE84Loaderflag[Pos]==false -> advances to 100.
        bE84Loaderflag[Pos] = false;
        iLoaderScanTask[Pos] = 2100;
        DoE84LoaderScan(Pos);
        CHECK(iLoaderScanTask[Pos] == 100, "case 2100: bE84Loaderflag[Pos]==false -> advances to 100");

        // case 2100: bE84Loaderflag[Pos]==true -> no else, cursor unmoved.
        bE84Loaderflag[Pos] = true;
        iLoaderScanTask[Pos] = 2100;
        DoE84LoaderScan(Pos);
        CHECK(iLoaderScanTask[Pos] == 2100, "case 2100: bE84Loaderflag[Pos]==true -> no else-branch, cursor unmoved (golden :1210-1215)");
    }

    // -----------------------------------------------------------------------
    // [O7] DoE84UnloaderScan(Pos) -- case 1->100->1000->1050->1100 cycle +
    // direct-seeded case 1100/1200 (golden :1219-1288).
    // -----------------------------------------------------------------------
    printf("\n[O7] DoE84UnloaderScan(Pos) -- case ladder + tray-count threshold\n");
    {
        const int Pos = 1;   // distinct slot from [O6]'s Pos=0, avoids cross-contamination
        iUnloaderScanTask[Pos] = 1;
        DoE84UnloaderScan(Pos);   // case 1 -> 100
        CHECK(iUnloaderScanTask[Pos] == 100, "case 1 -> 100");
        CHECK(bE84Unloaderflag[Pos] == false, "case 1 sets bE84Unloaderflag[Pos]=false");

        DoE84UnloaderScan(Pos);   // case 100 -> 1000
        CHECK(iUnloaderScanTask[Pos] == 1000, "case 100 -> 1000 (iCount[Pos] reset)");

        TestIF_File.iUnloaderTrayCount[Pos]      = 3;
        TestIF_File.iLoaderUnloaderTrayCount[Pos] = 5;
        DoE84UnloaderScan(Pos);   // case 1000, 3>=5 false -> cycles back to 100
        CHECK(iUnloaderScanTask[Pos] == 100, "case 1000: iUnloaderTrayCount < iLoaderUnloaderTrayCount -> cycles back to 100 (golden :1249)");

        iUnloaderScanTask[Pos] = 1000;
        TestIF_File.iUnloaderTrayCount[Pos]      = 5;
        TestIF_File.iLoaderUnloaderTrayCount[Pos] = 5;
        DoE84UnloaderScan(Pos);   // case 1000, 5>=5 true -> 1050 (break, same tick)
        CHECK(iUnloaderScanTask[Pos] == 1050, "case 1000: iUnloaderTrayCount >= iLoaderUnloaderTrayCount -> advances to 1050");
        CHECK(bE84Unloaderflag[Pos] == true,  "case 1000 (threshold met): bE84Unloaderflag[Pos]=true");
        CHECK(TestIF_File.iUnloaderTrayCount[Pos] == 0, "case 1000 (threshold met): iUnloaderTrayCount[Pos] reset to 0");

        DoE84UnloaderScan(Pos);   // case 1050 -> 1100 (arms ScanUnloaderDelayTime)
        CHECK(iUnloaderScanTask[Pos] == 1100, "case 1050 -> 1100 (delay armed)");

        // case 1100, Caption mismatch, timer freshly armed (2000ms, real
        // elapsed time here is microseconds) -> stays at 1100 (no timeout yet,
        // no ShowMyMessage). Deliberately NOT exercising the "timer actually
        // expired -> ShowMyMessage" branch here: it is coupled to 2 real
        // wall-clock seconds (TQPF_Timer uses QueryPerformanceCounter, see
        // myTimer.cpp) and this suite -- like test_agv_e84.cpp's own O3 scope
        // note -- deliberately keeps proportionate, non-time-dependent
        // coverage rather than adding a real sleep.
        fMain->labAutomation->Caption = "";
        DoE84UnloaderScan(Pos);
        CHECK(iUnloaderScanTask[Pos] == 1100, "case 1100: Caption mismatch + timer not yet expired -> cursor stays at 1100");

        // Caption-match branch, tested on a FRESH slot (Pos=2) seeded
        // directly at 1100 -- the Caption check short-circuits before the
        // timer is ever consulted, so no real-time coupling here.
        const int Pos2 = 2;
        iUnloaderScanTask[Pos2] = 1100;
        fMain->labAutomation->Caption = "SECS GEM 4:Enable";
        DoE84UnloaderScan(Pos2);
        CHECK(iUnloaderScanTask[Pos2] == 100, "case 1100 (Pos2): Caption matches -> advances to 100");
        CHECK(asSupplementBin.Pos("P4:") == 1, "case 1100 (Pos2): asSupplementBin formatted with the P4/P5/P6 template (golden :1267)");

        // case 1200: bE84Unloaderflag[Pos]==false -> writes palRemoveTray, advances to 100.
        bE84Unloaderflag[Pos] = false;
        iUnloaderScanTask[Pos] = 1200;
        fLotInfo->palRemoveTray->Enabled = true;
        fLotInfo->palRemoveTray->Visible = true;
        DoE84UnloaderScan(Pos);
        CHECK(iUnloaderScanTask[Pos] == 100, "case 1200: bE84Unloaderflag[Pos]==false -> advances to 100");
        CHECK(fLotInfo->palRemoveTray->Enabled == false, "case 1200: fLotInfo->palRemoveTray->Enabled set false");
        CHECK(fLotInfo->palRemoveTray->Visible == false, "case 1200: fLotInfo->palRemoveTray->Visible set false");

        // case 1200: bE84Unloaderflag[Pos]==true -> no else, cursor unmoved.
        bE84Unloaderflag[Pos] = true;
        iUnloaderScanTask[Pos] = 1200;
        DoE84UnloaderScan(Pos);
        CHECK(iUnloaderScanTask[Pos] == 1200, "case 1200: bE84Unloaderflag[Pos]==true -> no else-branch, cursor unmoved");
    }

    // -----------------------------------------------------------------------
    // [O8] NowE84Status() / E84StatusChange() -- smoke test only (internal
    // comparison state is TU-local/static, no accessor -- see file-head note).
    // -----------------------------------------------------------------------
    printf("\n[O8] NowE84Status()/E84StatusChange() -- logging smoke test\n");
    NowE84Status();
    E84StatusChange();
    SW[SwE84_1_LREQ].Enable = true; SW[SwE84_1_LREQ].ISABase = kSentinelISABase; SW[SwE84_1_LREQ].Type = 1;
    SW[SwE84_1_LREQ].On();
    E84StatusChange();    // must detect the LREQ transition and log it without crashing
    SW[SwE84_1_LREQ].Off();
    E84StatusChange();    // and the reverse transition
    printf("  (ran without crashing)\n");
    ++g_pass;

    // -----------------------------------------------------------------------
    printf("\n=== SUMMARY: %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
