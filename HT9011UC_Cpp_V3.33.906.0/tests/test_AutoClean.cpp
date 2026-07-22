// =============================================================================
//  test_AutoClean.cpp  --  W906-AutoCleanFoundation VERIFY
//
//  Translation wave: W906-AutoCleanFoundation
//  Author: AI(W906-AutoCleanFoundation) 20260721
//
//  PURPOSE
//  -------
//  Companion verify TU for the AutoClean foundation wave (same minimal
//  PASS/FAIL harness idiom as test_w6_2_inarm_canary.cpp / test_w6_canary.cpp).
//  Exercises every Part A pure-calc function with at least one representative
//  case, and pumps the HAL-only state-machine helpers against the Sim HAL
//  (MOT[MMAutoCleanKit] / InArmSuck / FLCarryKit / BLCarryKit) to prove they
//  link + run with no hardware and behave sanely. Also exercises the
//  FormsFacade InitialUnLoaderTask behaviour-change fix (Part B).
//
//  This wave's 4 named core engines (DoAutoCleanKit and friends) are
//  explicitly out of scope -- not exercised here.
// =============================================================================
#include "AutoClean/AutoClean.h"
#include "aHotPlateSubstrate.h"
#include "Motor/mymotor.h"
#include "cprod.h"
#include "cpublic.h"
#include "cmydef.h"
#include "FormsFacade.h"
#include "canary_support.h"
#include <cstdio>
#include <cstring>

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as the other W6/W906 verify TUs)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                   \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---------------------------------------------------------------------------
//  Reset the handful of globals every function under test reads, to a sane,
//  known baseline before each case (mirrors the "set the few globals each
//  asserted branch reads" idiom test_w6_2_inarm_canary.cpp uses).
// ---------------------------------------------------------------------------
// AI(W906-AutoCleanFoundation) 20260721: DISCOVERED PRE-EXISTING GAP (not
// introduced by this wave, and NOT fixed here -- see this wave's own report):
// InArmOffSet[]/OutArmOffSet[] (cprod.h) are declared as raw pointer arrays,
// but `ARM_OFFSET::ARM_OFFSET()`'s entire body sits inside cprod.cpp's own
// `#if 0 // TODO(W6): function bodies depend on untranslated state machines +
// globals` gate (cprod.cpp:184-1842, a pre-existing 1658-line deferred region
// unrelated to AutoClean) -- so ARM_OFFSET currently has NO linkable
// constructor anywhere in this tree, InArmOffSet[]/OutArmOffSet[] are
// permanently null pointers, and any code path that dereferences
// InArmOffSet[idx]->Get*() (e.g. MoveInArmXYPickCleanKit's unconditional
// GetInArmPitchY_9045/GetInArmPitchX_9045 calls) segfaults. This is a
// pre-existing cprod.cpp infrastructure gap (a future W6/W7-style wave's
// job to un-gate), not something an AutoClean-foundation-scoped fix should
// reach into. MoveInArmXYPickCleanKit is therefore deliberately NOT
// exercised below (translated + declared, just not smoke-run) -- every
// OTHER function this wave translates that reads InArmOffSet only does so
// behind a config-flag branch this test's defaults route around (verified:
// MoveInArmZToShuttlePlace/MoveInArmZ_Shuttle_Pick both PASS below).
static void ResetAutoCleanTestState()
{
    TestIF.iTestMode = QualSite1X4;
    TestIF_File.iTestMode = QualSite1X4;
    TestIF.iAutoClean_XDivision = 4;
    TestIF_File.iAutoClean_XDivision = 4;
    TestIF.iAutoClean_YDivision = 1;
    TestIF_File.iAutoClean_YDivision = 1;
    TestIF.iAutoClean_DeveicePices = 4;
    TestIF_File.iAutoClean_DeveicePices = 4;
    TestIF.dAutoClean_XPitch = 2000;
    TestIF_File.dAutoClean_XPitch = 2000;
    TestIF.dAutoClean_YPitch = 2000;
    TestIF_File.dAutoClean_YPitch = 2000;
    TestIF.iAutoClean_AlarmCount = 999999;   // effectively "never alarm" for most cases
    TestIF_File.iAutoClean_AlarmCount = 999999;
    TestIF.iAutoClean_Function = 1;
    TestIF_File.iAutoClean_Function = 1;
    TestIF.iAutoClean_Tray = eCKPos_CleanKit;
    TestIF_File.iAutoClean_Tray = eCKPos_CleanKit;
    TestIF.iAutoClean_SelectArm = 0;
    TestIF.iShuttleMode = 0;
    TestIF.iShuttle_Sel = 0;
    TestIF.bCleanIndexOtherArm = false;
    TestIF_File.iShuttleMode = 0;
    TestIF_File.iShuttle_Sel = 0;
    TestIF.bEnableAutoAlignment = false;
    TestIF_File.bEnableAutoAlignment = false;
    TestIF.bAutoClean_UseTray = false;
    TestIF_File.bAutoClean_UseTray = false;
    MACHINE_HAS_AUTO_ALIGNMENT_CCD = false;
    iInArmType = e9045_1x4_4;
    bUse8Picker = false;
    bUseTwoArm32Site = false;
    bRunAutoClean = false;
    iCloseSiteModeFor1x4 = 0;
    i1x2_4UseACEGPicker = 0;
    i1x2_4UseACEGPicker = 0;
    bCleanKitPitchLess4000 = false;
    bCleanKitPitchOver12000 = false;
    IniConfig.bE43AutoCleanUseHotplate = false;
    IniConfig.bEnableAutoCleanFunction = false;
    IniConfig.bE48_ShuttleUse4Offset_Autoclean = false;
    IniConfig.bAlarmNeedServoOff = false;
    CosFunction.bAutoCleanOffsetUseSingleSetting = false;
    CosFunction.bAutoCleanAutoSelIndexArm = false;
    CosFunction.bUseAutoCleanCloseSiteAlsoDo = false;
    CosFunction.bCleanCountAlarmByMin = false;
    CosFunction.bIndexJamInArmMoveSafePostionByAutoClaen = false;
    CosFunction.bDeviceMapTest = false;
    bPlaceToShuttleByAutoClean = false;
    bPickFromShuttleByAutoClean = false;
    bPlaceToCleanKit = false;
    bPickFromKitByAutoClean = false;

    // Give MOT[MMAutoCleanKit].Tray a real XY size + all-NULL_IC content so the
    // grid-scan HAL-only functions have somewhere sane to read/write.
    MOT[MMAutoCleanKit].Tray.SetXYItem(TestIF.iAutoClean_XDivision, TestIF.iAutoClean_YDivision);
    MOT[MMAutoCleanKit].Tray.ClearData();
    fMain->AutoCleanStringGrid->ColCount = 8;
    fMain->AutoCleanStringGrid->RowCount = 8;
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            fMain->AutoCleanStringGrid->Cells[x][y] = AnsiString("0");

    InArmSuck.ClearAll();
    FLCarryKit.ClearAll();
    BLCarryKit.ClearAll();
    FTestSuck.ClearAll();
    BTestSuck.ClearAll();
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 8; ++j)
        {
            InArmSuck.iAutoCleanRecX[i][j] = -1;
            InArmSuck.iAutoCleanRecY[i][j] = -1;
            FLCarryKit.iAutoCleanRecX[i][j] = -1;
            FLCarryKit.iAutoCleanRecY[i][j] = -1;
            BLCarryKit.iAutoCleanRecX[i][j] = -1;
            BLCarryKit.iAutoCleanRecY[i][j] = -1;
        }

    LastSet.iRealDummy = DUMMY;
    fCleaning->b1x2SiteAbClosePutDummy = false;
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("==== W906-AutoCleanFoundation verify ====\n");
    ResetAutoCleanTestState();

    // -----------------------------------------------------------------------
    //  Part A -- pure calc / config
    // -----------------------------------------------------------------------
    printf("[A] pure calc / config\n");

    // GetAutoCleanPickCount: default (bUseAxExPicker/AxxG false, not SingleSite,
    // not small pitch) -> 4 (golden else branch).
    ResetAutoCleanTestState();
    CHECK(GetAutoCleanPickCount() == 4, "GetAutoCleanPickCount default -> 4");
    TestIF.iTestMode = SingleSite;
    CHECK(GetAutoCleanPickCount() == 1, "GetAutoCleanPickCount SingleSite -> 1");

    // GetAutoCleanPickStep: SingleSite, col 0, no other-suck flags -> 0.
    ResetAutoCleanTestState();
    TestIF.iTestMode = SingleSite;
    CHECK(GetAutoCleanPickStep(0) == 0, "GetAutoCleanPickStep SingleSite col0 -> 0");
    // Non-picker default path: iSuckCol==iCol.
    ResetAutoCleanTestState();
    CHECK(GetAutoCleanPickStep(2) == 2, "GetAutoCleanPickStep default passthrough -> col");

    // CalculateAutoCleanXPitch: SingleSite -> 1.
    ResetAutoCleanTestState();
    TestIF.iTestMode = SingleSite;
    CHECK(CalculateAutoCleanXPitch() == 1, "CalculateAutoCleanXPitch SingleSite -> 1");
    // default path: ceil(XDivision/4.0)
    ResetAutoCleanTestState();
    TestIF.iTestMode = QualSite1X4;
    TestIF_File.iAutoClean_XDivision = 8;
    CHECK(CalculateAutoCleanXPitch() == 2, "CalculateAutoCleanXPitch default ceil(8/4)");

    // GetXPitchOfCleanKit_Kit / GetXPitchOfCleanKit dispatcher: SingleSite -> iXpitchMaxX3.
    ResetAutoCleanTestState();
    TestIF.iTestMode = SingleSite;
    CHECK(GetXPitchOfCleanKit_Kit() == iXpitchMaxX3, "GetXPitchOfCleanKit_Kit SingleSite -> iXpitchMaxX3");
    CHECK(GetXPitchOfCleanKit() == GetXPitchOfCleanKit_Kit(), "GetXPitchOfCleanKit dispatches to _Kit when HP disabled");
    IniConfig.bE43AutoCleanUseHotplate = true;
    CHECK(GetXPitchOfCleanKit() == GetXPitchOfCleanKit_HP(), "GetXPitchOfCleanKit dispatches to _HP when enabled");

    // GetYPitchOfCleanKit: bE43AutoCleanUseHotplate false -> TestIF.iARM_Y_PITCH passthrough.
    ResetAutoCleanTestState();
    TestIF.iARM_Y_PITCH = 12345;
    CHECK(GetYPitchOfCleanKit() == 12345, "GetYPitchOfCleanKit passthrough when HP-pitch disabled");

    // RunAutoCleanByArmPickArm2Test: all 3 gates default false -> false.
    ResetAutoCleanTestState();
    CHECK(RunAutoCleanByArmPickArm2Test() == false, "RunAutoCleanByArmPickArm2Test default false");
    IniConfig.bD58UseArm1PickPlaceArm2Test = true;
    TestIF_File.bArm1PickPlaceArm2Test = true;
    TestIF_File.bArm1PickPlaceArm2Test_RunAutoClean = true;
    CHECK(RunAutoCleanByArmPickArm2Test() == true, "RunAutoCleanByArmPickArm2Test all-3-gates true");

    // Special_2X6_Tray_XItem7: default state -> false (not e9045_2x6_8 etc.)
    ResetAutoCleanTestState();
    CHECK(Special_2X6_Tray_XItem7() == false, "Special_2X6_Tray_XItem7 default -> false");

    // GetInarmSuckRow: row 1 -> kitStep 0, suckRow 0; row 3 -> kitStep 4, suckRow 0.
    {
        int suckRow=-1, kitStep=-1;
        GetInarmSuckRow(1, suckRow, kitStep);
        CHECK(suckRow==0 && kitStep==0, "GetInarmSuckRow(1) -> suckRow=0,kitStep=0");
        GetInarmSuckRow(3, suckRow, kitStep);
        CHECK(suckRow==0 && kitStep==4, "GetInarmSuckRow(3) -> suckRow=0,kitStep=4");
        GetInarmSuckRow(2, suckRow, kitStep);
        CHECK(suckRow==1 && kitStep==0, "GetInarmSuckRow(2) -> suckRow=1,kitStep=0");
    }

    // -----------------------------------------------------------------------
    //  Part A -- HAL-only state-machine helpers
    // -----------------------------------------------------------------------
    printf("[A] HAL-only helpers\n");

    // Init task family -- each resets its own cursor(s) to 1.
    ResetAutoCleanTestState();
    iDoAutoCleanTask = 0; iDoShuttle1AutoCleanTask=0; iDoShuttle2AutoCleanTask=0;
    iDoShuttleAutoCleanTask=0; iDoIndexAutoCleanTask=0; iAutoCleanPickFromShuttleTask=0;
    iAutoCleanPlaceToShuttleTask=0;
    InitialAutoCleanTask();
    CHECK(iDoAutoCleanTask==1, "InitialAutoCleanTask resets iDoAutoCleanTask=1");
    InitialShuttleAutoCleanTask();
    CHECK(iDoShuttle1AutoCleanTask==1 && iDoShuttle2AutoCleanTask==1 && iDoShuttleAutoCleanTask==1,
          "InitialShuttleAutoCleanTask resets all 3 shuttle cursors");
    InitialIndexAutoCleanTask();
    CHECK(iDoIndexAutoCleanTask==1, "InitialIndexAutoCleanTask resets iDoIndexAutoCleanTask=1");
    InitPickFromShuttleTask();
    CHECK(iAutoCleanPickFromShuttleTask==1, "InitPickFromShuttleTask resets cursor=1");
    InitPlaceToShuttleTask();
    CHECK(iAutoCleanPlaceToShuttleTask==1, "InitPlaceToShuttleTask resets cursor=1");

    // CleanSetSpeed: both directions should just run without crashing (offline
    // motor-speed setters are no-ops/Sim HAL).
    CleanSetSpeed(true);
    CleanSetSpeed(false);
    CHECK(true, "CleanSetSpeed(true/false) runs without crash");

    // InOutArmSuckActiveSet: zeroes the whole grid.
    ResetAutoCleanTestState();
    bInArmSuckActive[0][0] = true; bInArmSuckActive[1][3] = true;
    InOutArmSuckActiveSet();
    CHECK(bInArmSuckActive[0][0]==false && bInArmSuckActive[1][3]==false, "InOutArmSuckActiveSet zeroes grid");

    // TrayHasCleanIC / TrayHasCleanICCount: empty grid -> false / 0.
    ResetAutoCleanTestState();
    CHECK(TrayHasCleanIC()==false, "TrayHasCleanIC empty grid -> false");
    CHECK(TrayHasCleanICCount()==0, "TrayHasCleanICCount empty grid -> 0");
    MOT[MMAutoCleanKit].SetTraySingleData(0, 0, HAS_CLEAN_IC);
    CHECK(TrayHasCleanIC()==true, "TrayHasCleanIC with one HAS_CLEAN_IC cell -> true");
    CHECK(TrayHasCleanICCount()==1, "TrayHasCleanICCount counts the one cell");

    // SetAutoCleanStringGrid + ReadWriteAutoCleanCount round-trip.
    ResetAutoCleanTestState();
    SetAutoCleanStringGrid(2, 1, AnsiString(7));
    CHECK(fMain->AutoCleanStringGrid->Cells[2][1] == AnsiString("7"), "SetAutoCleanStringGrid writes the cell");
    bRunAutoClean = false;   // ReadWriteAutoCleanCount's outer guard is bRunAutoClean==false -> proceeds
    ReadWriteAutoCleanCount(false, false);   // write path: should not crash
    CHECK(true, "ReadWriteAutoCleanCount(write) runs without crash");
    ReadWriteAutoCleanCount(true, false);    // read path: should not crash
    CHECK(true, "ReadWriteAutoCleanCount(read) runs without crash");

    // CheckAutoCleanCloseSite: bUseTwoArm32Site false -> false regardless of iSht.
    ResetAutoCleanTestState();
    CHECK(CheckAutoCleanCloseSite(0)==false, "CheckAutoCleanCloseSite false when bUseTwoArm32Site false");

    // DoInArmMoveToWaitPosByAutoClean: bIndexJamInArmMoveSafePostionByAutoClaen
    // false -> unconditionally true (and unlocks both locks).
    ResetAutoCleanTestState();
    bLockPlaceToShuttleByAutoClean = true; bLockPickFromShuttleByAutoClean = true;
    CHECK(DoInArmMoveToWaitPosByAutoClean()==true, "DoInArmMoveToWaitPosByAutoClean true when feature disabled");
    CHECK(bLockPlaceToShuttleByAutoClean==false && bLockPickFromShuttleByAutoClean==false,
          "DoInArmMoveToWaitPosByAutoClean clears both locks on the disabled path");

    // DoSocketSensorAlarm: should run without crash (iShowSocketSensor==0 default).
    ResetAutoCleanTestState();
    iShowSocketSensor = 0;
    CHECK(DoSocketSensorAlarm("test", 1) == true, "DoSocketSensorAlarm always returns true");

    // DoInArmPineRelease: bAlarmNeedServoOff false -> unconditionally true.
    ResetAutoCleanTestState();
    CHECK(DoInArmPineRelease()==true, "DoInArmPineRelease true when bAlarmNeedServoOff disabled");

    // CheckInSuckICFallDown / CheckInArmSuckFromCleanKitICFallDown: LastSet.iRealDummy==DUMMY -> false fast-path.
    ResetAutoCleanTestState();
    CHECK(CheckInSuckICFallDown(K_RETRY)==false, "CheckInSuckICFallDown false on DUMMY LastSet");
    CHECK(CheckInArmSuckFromCleanKitICFallDown(true)==false, "CheckInArmSuckFromCleanKitICFallDown false on DUMMY LastSet");

    // RestoreCleanKitData: empty InArmSuck/shuttles -> no crash, returns true.
    ResetAutoCleanTestState();
    CHECK(RestoreCleanKitData()==true, "RestoreCleanKitData returns true on empty state");

    // SearchCleanKitRowCol(int&,int&): empty tray -> false (no HAS_CLEAN_IC cell).
    ResetAutoCleanTestState();
    {
        int kr=-1, kc=-1;
        CHECK(SearchCleanKitRowCol(kr, kc)==false, "SearchCleanKitRowCol false over an all-NULL_IC tray");
        MOT[MMAutoCleanKit].SetTraySingleData(1, 0, HAS_CLEAN_IC);
        CHECK(SearchCleanKitRowCol(kr, kc)==true && kc==1 && kr==0,
              "SearchCleanKitRowCol finds the HAS_CLEAN_IC cell");
    }

    // DoPlaceToKitSwapData + PickFromCleanKit + PlaceToCleanKit: basic no-crash
    // smoke run over the Sim substrate (PlaceToCleanList is the offline-empty
    // uPlateInfo, so PlaceToCleanKit's GetHPFirstTeam takes the "no team" path).
    ResetAutoCleanTestState();
    DoPlaceToKitSwapData(bAutoPick, HAS_CLEAN_IC, 1, 0, 0, 0);
    CHECK(InArmSuck.Item[1][0]==HAS_CLEAN_IC, "DoPlaceToKitSwapData(bAutoPick) sets InArmSuck.Item");
    CHECK(PlaceToCleanKit()==true, "PlaceToCleanKit no-crash smoke run (empty PlaceToCleanList -> true)");

    // MoveInArmZToShuttlePlace / MoveInArmZ_Shuttle_Pick / MoveInOutArmZToKitPickPlace /
    // MoveInArmXYPickCleanKit: pure link+no-crash smoke runs over the Sim HAL
    // (InArmZMoveDown / InArmContinuousMove_9045 are the already-real Sim-HAL
    // motor movers other translated engines already exercise).
    ResetAutoCleanTestState();
    MoveInArmZToShuttlePlace(euShuttle1, 1);
    CHECK(true, "MoveInArmZToShuttlePlace runs without crash");
    MoveInArmZ_Shuttle_Pick(euShuttle1, 1);
    CHECK(true, "MoveInArmZ_Shuttle_Pick runs without crash");
    MoveInOutArmZToKitPickPlace(bAutoPick, true, euShuttle1, 1);
    CHECK(true, "MoveInOutArmZToKitPickPlace(pick) runs without crash");
    MoveInOutArmZToKitPickPlace(bAutoPlace, false, euShuttle1, 1);
    CHECK(true, "MoveInOutArmZToKitPickPlace(place, empty team list) runs without crash");
    // MoveInArmXYPickCleanKit deliberately NOT exercised here -- see the
    // ResetAutoCleanTestState() banner comment (pre-existing ARM_OFFSET ctor
    // gap in cprod.cpp; InArmOffSet[] is always null, and this function
    // unconditionally dereferences it via GetInArmPitchY_9045/X_9045).

    // GetShuttleState dispatcher: QualSite1X4 -> routes to GetShuttleState_1x4_4.
    ResetAutoCleanTestState();
    int stateViaDispatch = GetShuttleState(euShuttle1, true);
    (void)stateViaDispatch;
    CHECK(true, "GetShuttleState dispatcher runs without crash for QualSite1X4");

    // CheckShuttleSensor_Clean dispatcher: smoke run for a couple of modes.
    ResetAutoCleanTestState();
    CheckShuttleSensor_Clean(euShuttle1, false);
    TestIF.iTestMode = _16Site2X8;
    CheckShuttleSensor_Clean(euShuttle1, false);
    CHECK(true, "CheckShuttleSensor_Clean dispatcher runs without crash for 1x4/2x8");

    // SearchCleanKitRowCol(eWhichShuttle) / SearchCleanKitUpDown: smoke run.
    ResetAutoCleanTestState();
    MOT[MMAutoCleanKit].SetTraySingleData(0, 0, HAS_CLEAN_IC);
    SearchCleanKitRowCol(euShuttle1);
    CHECK(true, "SearchCleanKitRowCol(eWhichShuttle) runs without crash");
    SearchCleanKitUpDown(1, euShuttle1);
    CHECK(true, "SearchCleanKitUpDown runs without crash");

    // SetShuttleIcForSpecialMode: default close-site modes (all "OneByOne" /
    // none of the special enums) -> falls into the tail cleanup branch.
    ResetAutoCleanTestState();
    SetShuttleIcForSpecialMode(euShuttle1, NULL_IC);
    CHECK(true, "SetShuttleIcForSpecialMode runs without crash");

    // CleanPad_PlaceToShuttle / CleanPad_PickFromShuttle: smoke run, no crash.
    ResetAutoCleanTestState();
    CleanPad_PlaceToShuttle(euShuttle1);
    CHECK(true, "CleanPad_PlaceToShuttle runs without crash");
    CleanPad_PickFromShuttle(euShuttle1, 1);
    CHECK(true, "CleanPad_PickFromShuttle runs without crash");

    // AI(W906-AutoCleanFoundation-Review) 20260722: MoveSuckDataDiff is the MOVE
    // primitive CleanPad_PlaceToShuttle/CleanPad_PickFromShuttle rely on -- review
    // found a prior revision stopped after the target-side copy and silently
    // dropped golden's source-slot clear (MyKitSuck.cpp:1531-1560), leaving stale
    // data behind in the source grid.  Directly exercise the move semantics here
    // (both directions the two callers above use) rather than only smoke-testing
    // through the higher-level functions.
    {
        ResetAutoCleanTestState();
        InArmSuck.ClearAll();
        FLCarryKit.ClearAll();

        InArmSuck.Item[0][0] = HAS_IC;
        InArmSuck.iWhichSite[0][0] = 3;
        InArmSuck.bPass[0][0] = true;
        InArmSuck.cDeviceInf[0][0] = "ABC123";
        InArmSuck.PordRec[0][0].bUse = true;
        InArmSuck.PordRec[0][0].asBuffer->CommaText = "1,2,3";

        FLCarryKit.MoveSuckDataDiff(InArmSuck, 0, 0, 0, 0);

        CHECK(FLCarryKit.Item[0][0] == HAS_IC, "MoveSuckDataDiff target gets Item");
        CHECK(FLCarryKit.iWhichSite[0][0] == 3, "MoveSuckDataDiff target gets iWhichSite");
        CHECK(FLCarryKit.bPass[0][0] == true, "MoveSuckDataDiff target gets bPass");
        CHECK(FLCarryKit.cDeviceInf[0][0] == "ABC123", "MoveSuckDataDiff target gets cDeviceInf");
        CHECK(FLCarryKit.PordRec[0][0].bUse == true, "MoveSuckDataDiff target gets PordRec.bUse");
        CHECK(FLCarryKit.PordRec[0][0].asBuffer->CommaText == "1,2,3", "MoveSuckDataDiff target gets PordRec.asBuffer->CommaText");

        CHECK(InArmSuck.Item[0][0] == NULL_IC, "MoveSuckDataDiff source Item cleared to NULL_IC (golden :1531)");
        CHECK(InArmSuck.iWhichSite[0][0] == -1, "MoveSuckDataDiff source iWhichSite reset to -1 (golden :1540)");
        CHECK(InArmSuck.bPass[0][0] == false, "MoveSuckDataDiff source bPass reset to false (golden :1543)");
        CHECK(InArmSuck.cDeviceInf[0][0] == "", "MoveSuckDataDiff source cDeviceInf reset to empty (golden :1553)");

        InArmSuck.ClearAll();
        FLCarryKit.ClearAll();
    }

    // -----------------------------------------------------------------------
    //  Part D -- TfCleaning free-function translations
    // -----------------------------------------------------------------------
    printf("[D] TfCleaning free functions\n");

    // CleanPadCountCanSupport2Arm: QualSite1X4, DeveicePices=4 -> <=4 -> false.
    ResetAutoCleanTestState();
    CHECK(CleanPadCountCanSupport2Arm()==false, "CleanPadCountCanSupport2Arm false at the <=4 threshold");
    TestIF_File.iAutoClean_DeveicePices = 8;
    CHECK(CleanPadCountCanSupport2Arm()==true, "CleanPadCountCanSupport2Arm true above the threshold");

    // SetDeviceInTray: DualSite USE_PICKER_COUNT!=0 path with 2 devices on an
    // 8-col tray -- routes through SetCleanCellValue -> MOT[MMAutoCleanKit] HAL
    // (iMode==eAutoCleanUsed), no crash + writes at least one cell.
    ResetAutoCleanTestState();
    MOT[MMAutoCleanKit].Tray.SetXYItem(8, 1);
    MOT[MMAutoCleanKit].Tray.ClearData();
    SetDeviceInTray(8, 1, 2, eAutoCleanUsed);
    CHECK(TrayHasCleanICCount() >= 1, "SetDeviceInTray(eAutoCleanUsed) writes at least one HAS_CLEAN_IC cell");

    // -----------------------------------------------------------------------
    //  Part B -- FormsFacade InitialUnLoaderTask behaviour-change fix
    // -----------------------------------------------------------------------
    printf("[B] FormsFacade::InitialUnLoaderTask\n");
    fLotInfo->iUnloaderTask[1] = 0;
    fLotInfo->InitialUnLoaderTask(1);
    CHECK(fLotInfo->iUnloaderTask[1]==1, "InitialUnLoaderTask now REALLY sets iUnloaderTask[pos]=1 (was a no-op)");
    CHECK(fLotInfo->iUnloaderTask[0]==0 && fLotInfo->iUnloaderTask[2]==0,
          "InitialUnLoaderTask only touches the targeted index");

    // -----------------------------------------------------------------------
    printf("==== %d passed, %d failed ====\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
