// =============================================================================
//  test_w906_autositemap_cleanout.cpp -- W906-AutoSiteMapCleanOut VERIFY:
//  InitCleanOutFunction's AutoSiteMap branch (newly un-gated) does REAL work.
//
//  Translation wave: W906-AutoSiteMapCleanOut (un-gate the AutoSiteMap branch
//                    of InitCleanOutFunction, golden csystem.cpp:15751-15785 --
//                    the last remaining piece of Wave16; siblings
//                    DoHotplateEdgeCylinderLoop + DoLoaderVibrateLoop landed
//                    20260722, commit 6f60737).
//  Author: AI(W906-AutoSiteMapCleanOut) 20260727
//  Suite name (add_test): W906_AutoSiteMapCleanOut
//
//  PURPOSE
//  -------
//  Before this wave, the AutoSiteMap branch of InitCleanOutFunction was
//  wrapped in `#if 0` -- calling InitCleanOutFunction() in AutoSiteMap mode
//  never touched iResetSiteMappingStep / bSiteMappingCHKOK / the per-plate
//  SiteMapData grid / bAutoSiteMapHotplateSave at all.  This TU proves the
//  now-active branch reproduces golden's REAL logic (golden csystem.cpp
//  :15751-15785), not just "compiles":
//    O1 GUARD false (iRunStartMode!=rsmAutoSiteMap, or Hot-temp/JCET guard
//       false) -> the whole branch is skipped: SiteMapData/iResetSiteMapping-
//       Step/bSiteMappingCHKOK/bAutoSiteMapHotplateSave all untouched. The
//       tail cursor resets (iHome/iReset/iCleanOut/iTrayFeed/bCleanoutStart)
//       still fire unconditionally (golden :15788-15792).
//    O2 GUARD true + bAutoSiteMapHasPickHP==true -> ONLY iResetSiteMappingStep
//       is set to 2 (golden :15757); the else-branch's SiteMapData zero-out /
//       bAutoSiteMapHotplateSave=false / bSiteMappingCHKOK are NOT touched.
//    O3 GUARD true + bAutoSiteMapHasPickHP==false -> the else-branch fires:
//       bSiteMappingCHKOK=true (golden :15761), and the nested
//       i<2 / j<HotPlateForm.XDivision / k<HotPlateForm.YDivision loop zeroes
//       MOT[MMPlate1+i].Tray.SiteMapData[j][k] for BOTH plates (golden
//       :15771-15780) -- proven by seeding non-zero cells both INSIDE and
//       OUTSIDE the XDivision/YDivision bounds on MMPlate1 AND MMPlate2, then
//       asserting only the in-bounds cells on both plates were zeroed (a
//       naive "wipe the whole array" translation would also zero the
//       out-of-bounds sentinels -- this test would catch that regression).
//       bAutoSiteMapHotplateSave is then set false (golden :15781) even when
//       seeded true beforehand.
//    O4 fMain->SetMainRunStartMode(...) / InitInArmTask() / ShowTestHeadComp()
//       are documented no-op stubs this wave (see FormsFacade.cpp /
//       aHotPlateSubstrate.cpp) -- calling the branch that reaches them must
//       not crash, and iAutoSiteMapRunStartMode (the value that selects which
//       constant is passed to the stub) is itself left unmutated by the call
//       (a stub has no side effect to observe here; this just documents the
//       gap is inert, not a hidden landmine).
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" == clean g++
//  compile/link + the branch's cursor/grid mutations match golden's
//  hand-derived guard/loop bounds exactly, with the fMain->SetMainRunStartMode
//  GAP stub proven inert (no crash, no unexpected mutation).
// =============================================================================
#include "csystem.h"               // InitCleanOutFunction
#include "canary_support.h"        // LastSet (LAST_GENERAL_SET)
#include "cmydef.h"                // rsmAutoSiteMap/rsmContinuStart/rsmContinuRetest,
                                    // Tempture_Hot, bAutoSiteMapHasPickHP,
                                    // iResetSiteMappingStep, bSiteMappingCHKOK,
                                    // iAutoSiteMapRunStartMode, bAutoSiteMapHotplateSave,
                                    // MMPlate1, iHome/iReset/iCleanOut/iTrayFeed/bCleanoutStart
#include "CosFunction.h"           // CosFunction.bUSEJCETSiteMapMode
#include "cprod.h"                 // HotPlateForm.XDivision/YDivision
#include "Motor/mymotor.h"         // MOT[]
#include <cstdio>

static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// Fill both plates' SiteMapData with a non-zero sentinel everywhere so any
// zero-out (in-bounds or out-of-bounds) is observable.
static void SeedSiteMapData(int sentinel)
{
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < _MAX_COL_ITEM; ++j)
            for (int k = 0; k < _MAX_ROW_ITEM; ++k)
                MOT[MMPlate1+i].Tray.SiteMapData[j][k] = sentinel;
}

int main()
{
    printf("==== W906-AutoSiteMapCleanOut verify (InitCleanOutFunction AutoSiteMap branch) ====\n");

    // --- save every global the branch / oracle touches ----------------------
    const int  saveRunStartMode      = LastSet.iRunStartMode;
    const int  saveTemperature       = LastSet.iTemperature;
    const bool saveJCETMode          = CosFunction.bUSEJCETSiteMapMode;
    const bool savePickHP            = bAutoSiteMapHasPickHP;
    const int  saveResetStep         = iResetSiteMappingStep;
    const bool saveSiteMapCHKOK      = bSiteMappingCHKOK;
    const int  saveAutoRunStartMode  = iAutoSiteMapRunStartMode;
    const bool saveHotplateSave      = bAutoSiteMapHotplateSave;
    const int  saveXDivision         = HotPlateForm.XDivision;
    const int  saveYDivision         = HotPlateForm.YDivision;
    const int  saveHome              = iHome;
    const int  saveReset             = iReset;
    const int  saveCleanOut          = iCleanOut;
    const int  saveTrayFeed          = iTrayFeed;
    const bool saveCleanoutStart     = bCleanoutStart;

    // Small, deterministic in-bounds window (well inside _MAX_COL_ITEM=30 /
    // _MAX_ROW_ITEM=70), so the seeded out-of-bounds sentinels below stay put.
    HotPlateForm.XDivision = 3;
    HotPlateForm.YDivision = 2;

    // =========================================================================
    //  O1 -- GUARD false: whole branch skipped, tail resets still fire
    // =========================================================================
    printf("[O1] guard false (iRunStartMode!=rsmAutoSiteMap) -> branch skipped (golden :15751-15753)\n");
    SeedSiteMapData(7);
    LastSet.iRunStartMode = rsmContinuStart;         // != rsmAutoSiteMap
    LastSet.iTemperature  = Tempture_Hot;
    CosFunction.bUSEJCETSiteMapMode = true;
    bAutoSiteMapHasPickHP = false;
    iResetSiteMappingStep = 111;                     // sentinel: must survive
    bSiteMappingCHKOK     = false;                   // sentinel: must survive
    bAutoSiteMapHotplateSave = true;                 // sentinel: must survive
    iHome = 9; iReset = 9; iCleanOut = 9; iTrayFeed = 9; bCleanoutStart = false;

    InitCleanOutFunction();

    CHECK(iResetSiteMappingStep == 111,
          "O1a guard false -> iResetSiteMappingStep untouched");
    CHECK(bSiteMappingCHKOK == false,
          "O1b guard false -> bSiteMappingCHKOK untouched");
    CHECK(bAutoSiteMapHotplateSave == true,
          "O1c guard false -> bAutoSiteMapHotplateSave untouched");
    CHECK(MOT[MMPlate1].Tray.SiteMapData[0][0] == 7 &&
          MOT[MMPlate1+1].Tray.SiteMapData[0][0] == 7,
          "O1d guard false -> SiteMapData untouched on both plates");
    CHECK(iHome == 0 && iReset == 0 && iCleanOut == 1 && iTrayFeed == 0 && bCleanoutStart == true,
          "O1e tail cursor resets still fire unconditionally (golden :15788-15792)");

    // =========================================================================
    //  O2 -- GUARD true + bAutoSiteMapHasPickHP==true -> ONLY iResetSiteMappingStep=2
    // =========================================================================
    printf("[O2] guard true + bAutoSiteMapHasPickHP==true -> only iResetSiteMappingStep=2 (golden :15755-15757)\n");
    SeedSiteMapData(7);
    LastSet.iRunStartMode = rsmAutoSiteMap;
    LastSet.iTemperature  = Tempture_Hot;
    CosFunction.bUSEJCETSiteMapMode = true;
    bAutoSiteMapHasPickHP = true;
    iResetSiteMappingStep = 0;
    bSiteMappingCHKOK     = false;                   // sentinel: must NOT become true on this path
    bAutoSiteMapHotplateSave = true;                 // sentinel: must NOT become false on this path

    InitCleanOutFunction();

    CHECK(iResetSiteMappingStep == 2,
          "O2a bAutoSiteMapHasPickHP==true -> iResetSiteMappingStep==2 (golden :15757)");
    CHECK(bSiteMappingCHKOK == false,
          "O2b bAutoSiteMapHasPickHP==true -> bSiteMappingCHKOK NOT touched (else-branch only, golden :15761)");
    CHECK(bAutoSiteMapHotplateSave == true,
          "O2c bAutoSiteMapHasPickHP==true -> bAutoSiteMapHotplateSave NOT touched (else-branch only, golden :15781)");
    CHECK(MOT[MMPlate1].Tray.SiteMapData[0][0] == 7 &&
          MOT[MMPlate1+1].Tray.SiteMapData[0][0] == 7,
          "O2d bAutoSiteMapHasPickHP==true -> SiteMapData NOT touched (else-branch only, golden :15771-15780)");

    // =========================================================================
    //  O3 -- GUARD true + bAutoSiteMapHasPickHP==false -> else-branch REAL work
    // =========================================================================
    printf("[O3] guard true + bAutoSiteMapHasPickHP==false -> else-branch fires (golden :15759-15784)\n");
    SeedSiteMapData(7);   // both plates, ALL cells (in- and out-of-bounds) = 7
    LastSet.iRunStartMode = rsmAutoSiteMap;
    LastSet.iTemperature  = Tempture_Hot;
    CosFunction.bUSEJCETSiteMapMode = true;
    bAutoSiteMapHasPickHP = false;
    bSiteMappingCHKOK        = false;
    bAutoSiteMapHotplateSave = true;
    iAutoSiteMapRunStartMode = 0;   // selects fMain->SetMainRunStartMode(rsmContinuStart) -- stub, inert

    InitCleanOutFunction();

    CHECK(bSiteMappingCHKOK == true,
          "O3a else-branch -> bSiteMappingCHKOK=true (golden :15761)");
    CHECK(bAutoSiteMapHotplateSave == false,
          "O3b else-branch -> bAutoSiteMapHotplateSave=false (golden :15781)");
    CHECK(iAutoSiteMapRunStartMode == 0,
          "O3c fMain->SetMainRunStartMode is a documented no-op stub -- selector left unmutated (no crash either)");

    // In-bounds cells (j<XDivision=3, k<YDivision=2) on BOTH plates must be
    // zeroed (golden :15771-15780 loops i<2).
    bool inBoundsZeroed = true;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < HotPlateForm.XDivision; ++j)
            for (int k = 0; k < HotPlateForm.YDivision; ++k)
                if (MOT[MMPlate1+i].Tray.SiteMapData[j][k] != 0)
                    inBoundsZeroed = false;
    CHECK(inBoundsZeroed,
          "O3d in-bounds SiteMapData[j<XDivision][k<YDivision] zeroed on BOTH plates (golden :15771-15780)");

    // Out-of-bounds sentinels (j>=XDivision or k>=YDivision) must survive --
    // a naive "wipe the whole array" mistranslation would fail this.
    CHECK(MOT[MMPlate1].Tray.SiteMapData[HotPlateForm.XDivision][0] == 7 &&
          MOT[MMPlate1].Tray.SiteMapData[0][HotPlateForm.YDivision] == 7 &&
          MOT[MMPlate1+1].Tray.SiteMapData[HotPlateForm.XDivision][0] == 7 &&
          MOT[MMPlate1+1].Tray.SiteMapData[0][HotPlateForm.YDivision] == 7,
          "O3e out-of-bounds SiteMapData sentinels survive on BOTH plates (exact XDivision/YDivision bounds preserved, not a full-array wipe)");

    // =========================================================================
    //  O3-alt -- iAutoSiteMapRunStartMode!=0 selects the OTHER stub constant
    //  (golden :15766-15769); still fully inert (documented GAP), no crash.
    // =========================================================================
    printf("[O3-alt] iAutoSiteMapRunStartMode!=0 -> other SetMainRunStartMode stub arg selected, still inert (golden :15766-15769)\n");
    SeedSiteMapData(9);
    bSiteMappingCHKOK        = false;
    bAutoSiteMapHotplateSave = true;
    iAutoSiteMapRunStartMode = 1;   // selects fMain->SetMainRunStartMode(rsmContinuRetest) -- stub, inert

    InitCleanOutFunction();

    CHECK(bSiteMappingCHKOK == true && bAutoSiteMapHotplateSave == false,
          "O3-alt else-branch real mutations fire the same regardless of which stub arg was selected");

    // --- restore every seeded global -----------------------------------------
    LastSet.iRunStartMode    = saveRunStartMode;
    LastSet.iTemperature     = saveTemperature;
    CosFunction.bUSEJCETSiteMapMode = saveJCETMode;
    bAutoSiteMapHasPickHP    = savePickHP;
    iResetSiteMappingStep    = saveResetStep;
    bSiteMappingCHKOK        = saveSiteMapCHKOK;
    iAutoSiteMapRunStartMode = saveAutoRunStartMode;
    bAutoSiteMapHotplateSave = saveHotplateSave;
    HotPlateForm.XDivision   = saveXDivision;
    HotPlateForm.YDivision   = saveYDivision;
    iHome = saveHome; iReset = saveReset; iCleanOut = saveCleanOut;
    iTrayFeed = saveTrayFeed; bCleanoutStart = saveCleanoutStart;

    printf("==== W906-AutoSiteMapCleanOut verify: %d passed, %d failed ====\n",
           g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
