// =============================================================================
//  test_wb_datalayer.cpp -- pins the fact that the machine data layer is
//  DEFINED but never POPULATED.
//
//  AI(W906-WebBridge) 20260805.
//
//  WHY THIS EXISTS
//  Every machine-state global in this ported tree exists as a zero-initialised
//  object that nothing ever fills. A reader therefore cannot tell "nobody
//  loaded this" from "the machine really reads zero". For the web bridge that
//  distinction is the whole problem: publishing 0 for a value nobody loaded is
//  a screen that lies, and the browser renders null ("---") and 0 ("0.00")
//  differently on purpose.
//
//  This test does not celebrate that state -- it PINS it, so the day someone
//  fixes it the test fails loudly and this file gets inverted rather than
//  quietly continuing to describe a world that no longer exists.
//
//  WHAT WOULD FIX IT
//  database.cpp's ReadGeneralIni() has three calls gated out:
//  CustomerFunctionSelect() / ReadLastSetIni() / ReadEventLogAutoSaveInfo().
//  ReadLastSetIni() is the one that fills IniConfig (from config.ini),
//  CosFunction (via CustomerFunctionSelect) and LastSet (via ReadLastDataFile).
//  All three LINK today -- the gate comment claiming otherwise is stale -- but
//  ungating them makes ReadGeneralIni segfault. The full reasoning, and the
//  measurement that produced it, is in database.cpp at that gate.
//
//  WHY THIS TEST DOES NOT CALL ReadGeneralIni()
//  Because ReadGeneralIni WRITES to whatever the global asGeneralPath points
//  at. tests/test_ga1_readgeneralini.cpp exists to drive it and does so
//  correctly: it copies the real Gerneral.ini to a scratch path, repoints
//  asGeneralPath, and restores it afterwards. Duplicating that here would buy
//  nothing and risk the real config. This file only reads globals.
// =============================================================================
#include "database.h"
#include "cprod.h"
#include "Config.h"
#include "LastSet.h"
#include "cmydef.h"

#include <cstdio>

static int g_total = 0;
static int g_fail = 0;

static void check(bool ok, const char* what)
{
    ++g_total;
    if (!ok) {
        ++g_fail;
        std::printf("FAIL: %s\n", what);
    } else {
        std::printf("PASS: %s\n", what);
    }
}

int main()
{
    std::setvbuf(stdout, 0, _IONBF, 0);

    std::printf("machine data layer, as loaded by the ported tree today:\n");
    std::printf("  CUSTOMER_CODE            = %d\n", CUSTOMER_CODE);
    std::printf("  IniConfig.sMachineType   = \"%s\"\n", IniConfig.sMachineType.c_str());
    std::printf("  IniConfig.sGPIBMachineID = \"%s\"\n", IniConfig.sGPIBMachineID.c_str());
    std::printf("  IniConfig.RMSTesterID    = \"%s\"\n", IniConfig.RMSTesterID.c_str());
    std::printf("  LastSet.iRunStartMode    = %d\n", LastSet.iRunStartMode);
    std::printf("  LastSet.iTemperature     = %d\n", LastSet.iTemperature);
    std::printf("\n");

    // ---------------------------------------------------------------------
    //  THESE ASSERTIONS ARE EXPECTED TO FAIL ONE DAY. That is the point.
    //  When they do: the data layer finally loads, and this file should be
    //  rewritten to assert the loaded values instead of the empty ones.
    // ---------------------------------------------------------------------
    check(IniConfig.sMachineType.Length() == 0,
          "IniConfig.sMachineType is empty -- nothing populates IniConfig "
          "(cConfiguration.cpp is untranslated and ReadLastSetIni is gated)");

    check(IniConfig.sGPIBMachineID.Length() == 0,
          "IniConfig.sGPIBMachineID is empty -- same cause");

    check(LastSet.iRunStartMode == 0 && LastSet.iTemperature == 0,
          "LastSet is zero-initialised -- ReadLastDataFile never runs, so the "
          "575 reads of LastSet.iRunStartMode in this tree all see 0");

    check(CUSTOMER_CODE == 0,
          "CUSTOMER_CODE is 0 -- it is set inside ReadGeneralIni, which nothing "
          "in the application calls yet (only tests do)");

    std::printf("\ntest_wb_datalayer: %d checks, %d failure(s)\n", g_total, g_fail);
    std::printf("RESULT: %s\n", g_fail == 0 ? "PASS" : "FAIL");
    return g_fail == 0 ? 0 : 1;
}
