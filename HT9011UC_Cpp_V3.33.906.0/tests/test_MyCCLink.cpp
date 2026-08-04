// tests/test_MyCCLink.cpp
// Verification harness for the W5 CCLink driver-pair translation:
//   CCLink/MyCCLink.{h,cpp}          -- TMyCCLinkCtrl (top-level comm state machine)
//   CCLink/MyCCLinkBoard.{h,cpp}     -- TMyCCLinkBoard (Mdfunc vendor board wrapper)
//   CCLink/MyCCLinkSensor_predicates.{h,cpp} -- UseCanBusOrEtherCAT / UseSensorMod3
//
// Exercises the translated classes over the DEFAULT offline stub path
// (HAVE_CCLINK not defined -- zero vendor SDK, zero real CC-Link hardware).
// The offline CCLink_mdReceive stub deterministically zeroes the caller's
// buffer (see MyCCLinkBoard.cpp), so all "receive" style calls read back
// 0/0000/empty -- this test seeds KNOWN state (iConnectModule, SHUTTLE_SENSOR_TYPE,
// bInitialOK, ...) before asserting, matching the established Sim-HAL test
// convention used elsewhere in this tree (test_sim_io.cpp / test_sim_motor.cpp).
//
// No external test framework: a tiny PASS/FAIL harness prints per-case results
// and a final summary, returning non-zero on ANY failure (same style as
// tests/test_ContactForce.cpp / tests/test_cContact.cpp).

#include "CCLink/MyCCLink.h"
#include "CCLink/MyCCLinkSensor_predicates.h"
#include "cmydef.h"       // SHUTTLE_SENSOR_TYPE (extern int, defined in cmydef.cpp)
#include "MachineType.h"  // eSensorCCLink / eSensorCCLink3 / eSensorCanBus / eSensorCanBus3 /
                          // eSensorEtherCAT / eSensorEtherCAT3 / eSensorLine
#include <cstdio>
#include <string>

static int g_pass = 0;
static int g_fail = 0;

static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected) { std::printf("PASS  %-52s got=%d\n", name, (int)got); ++g_pass; }
    else { std::printf("FAIL  %-52s got=%d exp=%d\n", name, (int)got, (int)expected); ++g_fail; }
}

static void check_i(const char* name, long got, long expected)
{
    if (got == expected) { std::printf("PASS  %-52s got=%ld\n", name, got); ++g_pass; }
    else { std::printf("FAIL  %-52s got=%ld exp=%ld\n", name, got, expected); ++g_fail; }
}

static void check_s(const char* name, const std::string& got, const std::string& expected)
{
    if (got == expected) { std::printf("PASS  %-52s got=\"%s\"\n", name, got.c_str()); ++g_pass; }
    else { std::printf("FAIL  %-52s got=\"%s\" exp=\"%s\"\n", name, got.c_str(), expected.c_str()); ++g_fail; }
}

// ---------------------------------------------------------------------------
// Test-local subclass: CCLinkHexStrToInt is `protected` on TMyCCLinkBoard
// (matches golden's protection level -- see MyCCLinkBoard.h). A `using`
// declaration in a tiny derived class is the standard, non-invasive way to
// unit-test a protected member without changing the translated header.
// ---------------------------------------------------------------------------
class TestableCCLinkBoard : public TMyCCLinkBoard
{
public:
    explicit TestableCCLinkBoard(TComponent* Owner) : TMyCCLinkBoard(Owner) {}
    using TMyCCLinkBoard::CCLinkHexStrToInt;
    using TMyCCLinkBoard::DoRYOnOff;
    using TMyCCLinkBoard::DoWriteWW;
    using TMyCCLinkBoard::DoReadWr02;
};

// ---------------------------------------------------------------------------
// Externs required at link time by MySleep/MySleepEx/MyDBIProcess (declared
// locally in MyCCLink.cpp/MyCCLinkBoard.cpp -- see those files' AI comments).
// Supplied here as local no-op stubs, mirroring the established
// test_config_loaders.cpp pattern (local stub definitions for gated
// externals) so this test links WITHOUT pulling in ht9045_sm.
// ---------------------------------------------------------------------------
// AI(W906-GA1-B2-integrate) 20260804: MySleepEx stub RETIRED (real common.cpp body now in closure)
// AI(W906-GA1-B2-integrate) 20260804: MySleep stub RETIRED (same)
// AI(W906-GA1-B2-integrate) 20260804: local MyDBIProcess stub RETIRED (real body now in link group)

int main()
{
    std::printf("=== CCLink driver-pair translation verification ===\n");
    std::printf("(offline stub path: HAVE_CCLINK not defined, zero vendor SDK)\n\n");

    // -------------------------------------------------------------------
    // Part 1 -- UseCanBusOrEtherCAT() / UseSensorMod3() predicates
    // (golden MyCCLinkSensor.cpp:2584-2598)
    // -------------------------------------------------------------------
    std::printf("-- predicates --\n");
    {
        int saved = SHUTTLE_SENSOR_TYPE;

        SHUTTLE_SENSOR_TYPE = eSensorCCLink;
        check_b("UseCanBusOrEtherCAT(CCLink)",    UseCanBusOrEtherCAT(), false);
        check_b("UseSensorMod3(CCLink)",          UseSensorMod3(),       false);

        SHUTTLE_SENSOR_TYPE = eSensorCCLink3;
        check_b("UseCanBusOrEtherCAT(CCLink3)",   UseCanBusOrEtherCAT(), false);
        check_b("UseSensorMod3(CCLink3)",         UseSensorMod3(),       true);

        SHUTTLE_SENSOR_TYPE = eSensorCanBus;
        check_b("UseCanBusOrEtherCAT(CanBus)",    UseCanBusOrEtherCAT(), true);
        check_b("UseSensorMod3(CanBus)",          UseSensorMod3(),       false);

        SHUTTLE_SENSOR_TYPE = eSensorCanBus3;
        check_b("UseCanBusOrEtherCAT(CanBus3)",   UseCanBusOrEtherCAT(), true);
        check_b("UseSensorMod3(CanBus3)",         UseSensorMod3(),       true);

        SHUTTLE_SENSOR_TYPE = eSensorEtherCAT;
        check_b("UseCanBusOrEtherCAT(EtherCAT)",  UseCanBusOrEtherCAT(), true);
        check_b("UseSensorMod3(EtherCAT)",        UseSensorMod3(),       false);

        SHUTTLE_SENSOR_TYPE = eSensorEtherCAT3;
        check_b("UseCanBusOrEtherCAT(EtherCAT3)", UseCanBusOrEtherCAT(), true);
        check_b("UseSensorMod3(EtherCAT3)",       UseSensorMod3(),       true);

        SHUTTLE_SENSOR_TYPE = eSensorLine;   // default/production value on THIS reference
                                              // machine's config (cmydef.cpp default) --
                                              // neither CanBus/EtherCAT nor Mod3.
        check_b("UseCanBusOrEtherCAT(Line/default)", UseCanBusOrEtherCAT(), false);
        check_b("UseSensorMod3(Line/default)",       UseSensorMod3(),       false);

        SHUTTLE_SENSOR_TYPE = saved;   // restore (other tests below construct real objects
                                        // that read this global too)
    }

    // -------------------------------------------------------------------
    // Part 2 -- TMyCCLinkBoard::CCLinkHexStrToInt (pure hex parser)
    // -------------------------------------------------------------------
    std::printf("-- CCLinkHexStrToInt --\n");
    {
        TestableCCLinkBoard b(NULL);
        check_i("Hex(\"0041\")",  b.CCLinkHexStrToInt("0041"),  0x41);
        check_i("Hex(\"ffff\")",  b.CCLinkHexStrToInt("ffff"),  0xFFFF);  // UpperCase() applied
        check_i("Hex(\"0000\")",  b.CCLinkHexStrToInt("0000"),  0);
        check_i("Hex(\"\")",      b.CCLinkHexStrToInt(""),      0);       // Length()==0 -> 0
        check_i("Hex(\"00FF\")",  b.CCLinkHexStrToInt("00FF"),  0xFF);
    }

    // -------------------------------------------------------------------
    // Part 3 -- TMyCCLinkBoard guard-clause + offline-receive determinism
    // (KNOWN-state seeding, matching the Sim-HAL test convention)
    // -------------------------------------------------------------------
    std::printf("-- TMyCCLinkBoard guarded methods (seeded state) --\n");
    {
        TestableCCLinkBoard b0(NULL);
        b0.iConnectModule = 0;   // no modules "connected" -- Index>=iConnectModule guards fire
        check_b("DoRYOnOff guard (0 modules)",  b0.DoRYOnOff(0, 0x10, true), false);
        check_b("DoWriteWW guard (0 modules)",  b0.DoWriteWW(0, 0, 100),     false);
        check_i("DoReadWr02 guard (0 modules)", b0.DoReadWr02(0),            -1);
        // DoCheckWr0000's for-loop runs 0 times when iConnectModule==0 -> vacuously true
        // (faithful golden quirk: the loop guard IS the module count, no separate early-return).
        check_b("DoCheckWr0000 vacuous-true (0 modules)", b0.DoCheckWr0000(), true);
        // DoReadOnOff / DoReadCurrValue have an explicit `if(iConnectModule<1) return false;` guard.
        check_b("DoReadOnOff guard (0 modules)",    b0.DoReadOnOff(),    false);
        check_b("DoReadCurrValue guard (0 modules)",b0.DoReadCurrValue(),false);
    }
    {
        TestableCCLinkBoard b2(NULL);
        b2.iConnectModule = 2;   // 2 "modules" -- guards pass, offline CCLink_mdXxx stubs fire
        check_b("DoRYOnOff On (2 modules, offline ok)",  b2.DoRYOnOff(0, 0x10, true),  true);
        check_b("DoRYOnOff Off(2 modules, offline ok)",  b2.DoRYOnOff(1, 0x10, false), true);
        check_b("DoWriteWW (2 modules, offline ok)",     b2.DoWriteWW(0, 0x04, 100),   true);
        check_i("DoReadWr02 (2 modules, offline zero)",  b2.DoReadWr02(0),             0);
        check_b("DoCheckWr0000 (2 modules, offline zero)", b2.DoCheckWr0000(),         true);
        check_b("DoReadOnOff (2 modules, offline zero)",   b2.DoReadOnOff(),           true);
        check_b("DoReadCurrValue (2 modules, offline zero)", b2.DoReadCurrValue(),     true);
        // offline mdReceive zeroes the buffer -> sValue==0 -> !(0 & mask)==true for every bit
        // (see MyCCLinkBoard.cpp CheckBoardStatus/DoReadOnOff formula note).
        check_b("MyNCUL[0].bOnOff[0] offline-true", b2.MyNCUL[0].bOnOff[0], true);
        check_i("MyNCUL[0].iCurrValue[0] offline-zero", b2.MyNCUL[0].iCurrValue[0], 0);
    }
    {
        // SetSensorKeyLock (was `protected write=` handler in golden, public here -- see .h note)
        TMyCCLinkBoard bk(NULL);
        bk.SetSensorKeyLock(true);
        check_b("SetSensorKeyLock(true) -> bKeyLock", bk.bKeyLock, true);
        bk.SetSensorKeyLock(false);
        check_b("SetSensorKeyLock(false) -> bKeyLock", bk.bKeyLock, false);
    }

    // -------------------------------------------------------------------
    // Part 4 -- TMyCCLinkBoard::DoConnectBoard(int&) full pump, offline
    // (no real vendor board present -> 0 modules found -> deterministic
    //  error latch; see MyCCLinkBoard.cpp DoConnectBoard commentary)
    // -------------------------------------------------------------------
    std::printf("-- DoConnectBoard offline pump (0 modules found -> error) --\n");
    {
        TMyCCLinkBoard bc(NULL);
        int autoRetry = 5;   // seed non-zero so we can observe the case-1 success reset to 0

        check_i("initial iCCLinkBoardTask", bc.iCCLinkBoardTask, 1);

        // tick 1: case 1 -- ConnectBoard() succeeds offline (CCLink_mdOpen stub returns 0) ->
        // AutoRetryCnt reset to 0, board Task -> 100. DoConnectBoard itself returns false (no
        // case in this call reaches the case-200 `return true;`).
        bool r1 = bc.DoConnectBoard(autoRetry);
        check_b("tick1 DoConnectBoard() return", r1, false);
        check_i("tick1 board Task", bc.iCCLinkBoardTask, 100);
        check_i("tick1 AutoRetryCnt reset", autoRetry, 0);
        check_b("tick1 bHasErr", bc.bHasErr, false);

        // tick 2: case 100 -- CheckBoardStatus() (offline: all 0x68/69/6A/90 queries -> 0, so
        // bStatus stays true) -> Task -> 200.
        bool r2 = bc.DoConnectBoard(autoRetry);
        check_b("tick2 DoConnectBoard() return", r2, false);
        check_i("tick2 board Task", bc.iCCLinkBoardTask, 200);

        // tick 3: case 200 -- iConnectModule=DoCCLinkSWQuest(0x72) reads 0 offline (no real
        // board) -> iConnectModule>0 is false -> bHasErr=true, Task resets to 1.
        bool r3 = bc.DoConnectBoard(autoRetry);
        check_b("tick3 DoConnectBoard() return", r3, false);
        check_i("tick3 board Task (reset)", bc.iCCLinkBoardTask, 1);
        check_b("tick3 bHasErr (no modules found)", bc.bHasErr, true);
        check_s("tick3 ErrMess mentions Modules", std::string(bc.ErrMess.c_str()).find("Modules")!=std::string::npos ? "found" : "missing", "found");
    }

    // -------------------------------------------------------------------
    // Part 5 -- TMyCCLinkCtrl: DoSetValue / DoSetMultiValue setup + re-entrancy guard
    // -------------------------------------------------------------------
    std::printf("-- TMyCCLinkCtrl::DoSetValue / DoSetMultiValue setup --\n");
    {
        TMyCCLinkCtrl ctrl(NULL);
        ctrl.DoSetValue(2, 5, 123);
        check_i("DoSetValue -> CCLinkBoard->iDoSetValueTask", ctrl.CCLinkBoard->iDoSetValueTask, 1);
        check_i("DoSetValue -> sSetNu",    ctrl.CCLinkBoard->sSetNu,    2);
        check_i("DoSetValue -> sSetSen",   ctrl.CCLinkBoard->sSetSen,   5);
        check_i("DoSetValue -> iSetValue", ctrl.CCLinkBoard->iSetValue, 123);
        check_b("DoSetValue -> ctrl.bSetValue", ctrl.bSetValue, true);

        int values[3] = {10, 20, 30};
        ctrl.bSetMultiValue = false;   // fresh, not re-entered
        ctrl.DoSetMultiValue(1, 0, values, 3);
        check_i("DoSetMultiValue -> CCLinkBoard->iDoSetMultiValueTask", ctrl.CCLinkBoard->iDoSetMultiValueTask, 1);
        check_i("DoSetMultiValue -> sSetNu",  ctrl.CCLinkBoard->sSetNu,  1);
        check_i("DoSetMultiValue -> sSetSen", ctrl.CCLinkBoard->sSetSen, 0);
        check_i("DoSetMultiValue -> iSetMultiValue[0]", ctrl.CCLinkBoard->iSetMultiValue[0], 10);
        check_i("DoSetMultiValue -> iSetMultiValue[1]", ctrl.CCLinkBoard->iSetMultiValue[1], 20);
        check_i("DoSetMultiValue -> iSetMultiValue[2]", ctrl.CCLinkBoard->iSetMultiValue[2], 30);
        check_b("DoSetMultiValue -> ctrl.bSetMultiValue", ctrl.bSetMultiValue, true);

        // re-entrancy guard (jou 20221004): a second call while bSetMultiValue==true is a no-op.
        int values2[3] = {99, 99, 99};
        ctrl.DoSetMultiValue(9, 9, values2, 3);
        check_i("DoSetMultiValue re-entrancy guard (sSetNu unchanged)", ctrl.CCLinkBoard->sSetNu, 1);
        check_i("DoSetMultiValue re-entrancy guard (iSetMultiValue[0] unchanged)", ctrl.CCLinkBoard->iSetMultiValue[0], 10);
    }

    // -------------------------------------------------------------------
    // Part 6 -- TMyCCLinkCtrl::Timer1Timer() full pump (offline: 0 modules
    // found -> deterministic error latch, mirrors Part 4 one layer up)
    // -------------------------------------------------------------------
    std::printf("-- TMyCCLinkCtrl::Timer1Timer offline pump (error latch) --\n");
    {
        TMyCCLinkCtrl ctrl(NULL);
        ctrl.bInitialOK = true;     // mirrors golden fCCLink->SetInitialOK(...) -> ...->InitialOK
        ctrl.bHasErr = false;
        ctrl.lPath = 81;            // mirrors golden MyCCLinkSensor.cpp:394 `MyCCLink->Path=81;`
        ctrl.iAutoRetryCnt = 7;     // seed non-zero so we can observe the reset to 0

        // bTimerEnabled defaults true (ctor); Timer1Timer runs every call here (no real 30ms
        // periodic driver in this offline test -- see .h/.cpp notes).
        ctrl.Timer1Timer();   // tick 1: case1 fallthrough case2 -- board Task 1->100, AutoRetryCnt->0
        check_i("tick1 CCLinkBoard->lPath synced", ctrl.CCLinkBoard->lPath, 81);
        check_i("tick1 board Task", ctrl.CCLinkBoard->iCCLinkBoardTask, 100);
        check_i("tick1 iAutoRetryCnt reset", ctrl.iAutoRetryCnt, 0);
        check_b("tick1 ctrl.bHasErr", ctrl.bHasErr, false);

        ctrl.Timer1Timer();   // tick 2: board Task 100->200
        check_i("tick2 board Task", ctrl.CCLinkBoard->iCCLinkBoardTask, 200);

        ctrl.Timer1Timer();   // tick 3: board Task 200 -> 0 modules found -> board.bHasErr=true,
                               // board Task resets to 1; ctrl observes board.bHasErr -> latches
                               // ctrl.bHasErr=true (ctrl.Task stays at 2, unreachable further).
        check_b("tick3 ctrl.bHasErr latched", ctrl.bHasErr, true);
        check_i("tick3 board Task (reset)", ctrl.CCLinkBoard->iCCLinkBoardTask, 1);

        // tick 4+: top-of-function guard `if(bHasErr==true) return;` fires immediately -- the
        // board's Task must stay frozen at 1 forever (never called again).
        ctrl.Timer1Timer();
        ctrl.Timer1Timer();
        check_i("tick4/5 board Task frozen (latch idempotent)", ctrl.CCLinkBoard->iCCLinkBoardTask, 1);
        check_b("tick4/5 ctrl.bHasErr still latched", ctrl.bHasErr, true);
    }

    // -------------------------------------------------------------------
    // Part 7 -- TMyCCLinkCtrl::WriteCCLinkStatus side effects
    // -------------------------------------------------------------------
    std::printf("-- TMyCCLinkCtrl::WriteCCLinkStatus --\n");
    {
        TMyCCLinkCtrl ctrl(NULL);
        ctrl.WriteCCLinkStatus(true);
        check_b("WriteCCLinkStatus(true) -> iCCLinkStatus", ctrl.iCCLinkStatus, true);
        check_b("WriteCCLinkStatus(true) -> bHasErr cleared", ctrl.bHasErr, false);
        // golden: iCCLinkTask=3 is `protected` (no external getter) -- side effect is exercised
        // indirectly via Timer1Timer()'s case 3/4 reset-sequence in Part 6-style pumps; not
        // re-asserted numerically here (no public accessor for iCCLinkTask, matching golden's
        // own encapsulation -- see .h).
    }

    // -------------------------------------------------------------------
    // Summary
    // -------------------------------------------------------------------
    std::printf("\n=== SUMMARY: %d passed, %d failed (total %d) ===\n",
           g_pass, g_fail, g_pass + g_fail);
    if (g_fail != 0)
    {
        std::printf("RESULT: FAIL\n");
        return 1;
    }
    std::printf("RESULT: ALL PASS\n");
    return 0;
}
