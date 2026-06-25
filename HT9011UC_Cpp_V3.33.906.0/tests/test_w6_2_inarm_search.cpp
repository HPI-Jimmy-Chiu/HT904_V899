// =============================================================================
//  test_w6_2_inarm_search.cpp  --  W6.2 VERIFY: SearchPickPlate / SearchPlacePlate
//                                   Do*/Search*/geometry leaves over the Sim HAL
//
//  Translation wave: W6.2 (IN-ARM foundation -- shared HP pick/place geometry)
//  Author: AI(W6.2-INARM) 20260626
//
//  PURPOSE
//  -------
//  The task-mandated W6.2 *search* verify TU.  Where test_w6_2_inarm_canary.cpp
//  proves the two shared HP-geometry leaf modules + the W6.2 substrate LINK and
//  RUN, this TU sets a KNOWN Sim HAL state and drives the SearchPickPlate /
//  SearchPlacePlate Do*/Search*/geometry functions, asserting the HP-geometry
//  result and the SM stepping for known inputs USING THE GOLDEN FORMULA AS THE
//  ORACLE, then pumps the planners + SM over the Sim HAL without crash.
//
//  The asserted oracles are computed by hand from the golden source so a faithful
//  translation is pinned to a CONCRETE value (not just "does not crash"):
//
//    [A] GetPlaceToHotPlateCol(j)  (golden ainarm_SearchPlacePlate.cpp:972) --
//        default-picker (bUseAxExPicker/bUseAxxGPicker both false offline, not
//        1x1) branch, parameterised by HotPlateForm.XDivision:
//          XDiv==4 (iInArmType!=2x8_32) -> iPlacePlateX[0] + j        (:1076)
//          XDiv==6 (i8PickerHPMode!=iHPWideHP) -> iPlacePlateX[0] + j*3 (:1096)
//          XDiv==8 (!bPitchOver12000)   -> iPlacePlateX[0] + j*2      (:1104)
//          XDiv==16                     -> iPlacePlateX[0] + j*4      (:1133)
//
//    [B] GetPlaceToHotPlateSuckCol(j) (golden :859) -- default branch
//          (not 1x1, AxEx/AxxG false, XDiv!=6/10) -> j2 = j           (:966)
//        and the 1x1 branch -> 0 (bSingleUseOtherSuck false)          (:914)
//
//    [C] GetInArmToHPOffset_9045(iAction) (golden :1715) --
//          bEnableAutoAlignment==true                  -> -1          (:1717/1733)
//          AutoAlignment off, SameOne off, iP==0        -> InOfsHP2    (:1728)
//          AutoAlignment off, SameOne off, iP!=0        -> InOfsHP1    (:1730)
//          AutoAlignment off, SameOne on                -> InOfsInSh1  (:1723)
//        (iAction==iPlaceHP reads iPlacePlate[0]; else iPickPlate[0].)
//
//    [D] GetVariableYInHotPlateData() (golden :1736) --
//          USE_IN_Y_IS_AUTO_PITCH==false -> TestIF.iARM_Y_PITCH       (:1774)
//
//    [E] GetVariableXInHotPlateData(iAction) (golden :1613) --
//          iInArmType==e9045_1x1_1 -> iXpitchMaxX3, bPitchOver12000=false (:1620)
//
//    [F] ResetHotPlateSearchParameter() (golden :2204) -- post-state oracle:
//          all iPick*/iPlace* cursors == 0 (AutoClean off), OnlyRow2==false,
//          iForPlaceHPX6Step==0, iForPickHPX6Step==0, iPickHPOrder==0, iInArmOrder==0.
//
//    [G] SM DoInArmPickFromHotPlate_9045() (golden case 1, :757) -- offline empty
//        PickFromHPList so SearchPlateToPick()==false -> return true & cursor stays
//        at entry (1).  Pumped bounded; cursor stays in the documented set.
//
//    [H] PLANNERS over Sim HAL (no crash): SearchPlateToPlace(), the
//        SearchPlacePlateXItem* leaf the default ACEG path uses, and
//        CheckHasSpaceToPlace_9045() -- driven over the conservative Sim
//        substrate (empty kit grid / no-IC tray) to prove they pump.
//
//  OFFLINE: aHotPlateSubstrate.cpp supplies the conservative substrate.  Each
//  asserted branch sets only the globals it reads so the asserted value is the
//  one under test.  Pure functions are also checked for HAL purity (idempotent).
// =============================================================================
#include "ainarm_SearchPickPlate.h"
#include "ainarm_SearchPlacePlate.h"
#include "aHotPlateSubstrate.h"
#include "csystem.h"
#include "Motor/mymotor.h"
#include "cprod.h"
#include "cpublic.h"
#include "cmydef.h"
#include "canary_support.h"     // LastSet, ShowErrorMessage, etc.
#include "FormsFacade.h"        // fMain/fSortCT/fLotInfo/fOffSet/fSCKART
#include <cstdio>

// The pick SM owns this cursor in ainarm_SearchPickPlate.cpp; drive via extern.
extern int iInArmPickFromHotPlateTask;

// Place planners with EXTERNAL linkage that are NOT exported via a header
// (the per-site ainarm9045_* modules call them as forward-declared externs).
// Re-declare here to drive them over the Sim HAL.  CheckHasSpaceToPlace_9045 IS
// in ainarm_SearchPlacePlate.h.
extern void SearchPlateToPlace();             // golden ainarm_SearchPlacePlate.cpp:4516
extern void SearchPlacePlateXItem4_8Suck();   // golden ainarm_SearchPlacePlate.cpp:3248

// Extra search-state globals the planners / reset touch (declared in cmydef.h /
// substrate) -- referenced here only to pin the ResetHotPlateSearchParameter
// post-state and to set the deterministic geometry branches.
extern int  iForPlaceHPX6Step;
extern int  iForPickHPX6Step;
extern int  iPickHPOrder;
extern int  iInArmOrder;
extern bool OnlyRow2;

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as the other W6 verify TUs)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                   \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// DoInArmPickFromHotPlate_9045 documented cursor set (golden switch(Task)).
static bool pickCursorSane(int t)
{
    switch (t)
    {
        case 1: case 50: case 100: case 105: case 110: case 150: case 190:
        case 200: case 210: case 220: case 300: case 320: case 330: case 335:
        case 336: case 340: case 350: case 400:
            return true;
        default:
            return false;
    }
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);   // unbuffered so a crash doesn't eat output
    printf("==== W6.2 InArm Search/Do geometry verify (golden oracle) ====\n");

    // Common conservative substrate state: default (ACEG) picker layout, no
    // AutoClean, manual Y-pitch.  bUseAxExPicker()/bUseAxxGPicker() are offline
    // stubs returning false (substrate :202-203), so the default picker branch
    // of the Col/SuckCol geometry is the one under test.
    bRunAutoClean             = false;
    TestIF_File.iAutoClean_Function = 0;
    USE_IN_Y_IS_AUTO_PITCH    = false;
    i8PickerHPMode            = 0;        // != iHPWideHP
    bPitchOver12000           = false;
    iInArmType                = e9045_2x4_8;   // generic multi-picker (not 1x1)

    // -----------------------------------------------------------------------
    //  [A] GetPlaceToHotPlateCol(j) -- golden :972, default picker branch
    // -----------------------------------------------------------------------
    printf("[A] GetPlaceToHotPlateCol (golden :972)\n");
    iPlacePlateX[0] = 0;
    iPlacePlateY[0] = 0;

    // XDiv==4 (not 2x8_32) -> iPlacePlateX[0] + j   (golden :1076)
    HotPlateForm.XDivision = 4;
    iPlacePlateX[0] = 3;
    CHECK(GetPlaceToHotPlateCol(0) == 3, "Col XDiv4: PlateX=3,j=0 -> 3+0");
    CHECK(GetPlaceToHotPlateCol(2) == 5, "Col XDiv4: PlateX=3,j=2 -> 3+2");

    // XDiv==6 (i8PickerHPMode != iHPWideHP) -> iPlacePlateX[0] + j*3 (golden :1096)
    HotPlateForm.XDivision = 6;
    iPlacePlateX[0] = 1;
    CHECK(GetPlaceToHotPlateCol(2) == 7, "Col XDiv6: PlateX=1,j=2 -> 1+2*3");

    // XDiv==8 (!bPitchOver12000) -> iPlacePlateX[0] + j*2 (golden :1104)
    HotPlateForm.XDivision = 8;
    iPlacePlateX[0] = 2;
    CHECK(GetPlaceToHotPlateCol(3) == 8, "Col XDiv8: PlateX=2,j=3 -> 2+3*2");

    // XDiv==16 -> iPlacePlateX[0] + j*4 (golden :1133)
    HotPlateForm.XDivision = 16;
    iPlacePlateX[0] = 0;
    CHECK(GetPlaceToHotPlateCol(2) == 8, "Col XDiv16: PlateX=0,j=2 -> 0+2*4");

    // -----------------------------------------------------------------------
    //  [B] GetPlaceToHotPlateSuckCol(j) -- golden :859
    // -----------------------------------------------------------------------
    printf("[B] GetPlaceToHotPlateSuckCol (golden :859)\n");
    // default branch (not 1x1, AxEx/AxxG false, XDiv != 6/10) -> j2 = j (:966)
    HotPlateForm.XDivision = 4;
    iInArmType = e9045_2x4_8;
    CHECK(GetPlaceToHotPlateSuckCol(0) == 0, "SuckCol default XDiv4: j=0 -> 0");
    CHECK(GetPlaceToHotPlateSuckCol(3) == 3, "SuckCol default XDiv4: j=3 -> 3");

    // 1x1 branch (bSingleUseOtherSuck false) -> 0 (golden :914)
    iInArmType = e9045_1x1_1;
    TestIF_File.bSingleUseOtherSuck = false;
    Prod.bSingleInArmUseOtherSuck   = false;
    CHECK(GetPlaceToHotPlateSuckCol(5) == 0, "SuckCol 1x1 (no other-suck): j=5 -> 0");
    iInArmType = e9045_2x4_8;            // restore generic

    // -----------------------------------------------------------------------
    //  [C] GetInArmToHPOffset_9045(iAction) -- golden :1715
    // -----------------------------------------------------------------------
    printf("[C] GetInArmToHPOffset_9045 (golden :1715)\n");
    // bEnableAutoAlignment == true -> -1 (init iOffsetPos stays, golden :1717/1733)
    TestIF.bEnableAutoAlignment = true;
    CHECK(GetInArmToHPOffset_9045(iPlaceHP) == -1, "Offset: AutoAlignment on -> -1");

    // AutoAlignment off, SameOne off, iP==0 -> InOfsHP2 (golden :1728)
    TestIF.bEnableAutoAlignment = false;
    IniConfig.bE34InOutArmPitchZOffsetSameOne = false;
    iPlacePlate[0] = 0;
    CHECK(GetInArmToHPOffset_9045(iPlaceHP) == InOfsHP2, "Offset: placeP=0 -> InOfsHP2");

    // AutoAlignment off, SameOne off, iP!=0 -> InOfsHP1 (golden :1730)
    iPlacePlate[0] = 1;
    CHECK(GetInArmToHPOffset_9045(iPlaceHP) == InOfsHP1, "Offset: placeP=1 -> InOfsHP1");

    // iAction != iPlaceHP reads iPickPlate[0]; pickP=0 -> InOfsHP2 (golden :1718/1728)
    iPickPlate[0] = 0;
    CHECK(GetInArmToHPOffset_9045(iPickHP) == InOfsHP2, "Offset: pickP=0 -> InOfsHP2");

    // AutoAlignment off, SameOne ON -> InOfsInSh1 (golden :1723)
    IniConfig.bE34InOutArmPitchZOffsetSameOne = true;
    CHECK(GetInArmToHPOffset_9045(iPlaceHP) == InOfsInSh1, "Offset: SameOne on -> InOfsInSh1");
    IniConfig.bE34InOutArmPitchZOffsetSameOne = false;   // restore

    // -----------------------------------------------------------------------
    //  [D] GetVariableYInHotPlateData() -- golden :1736
    // -----------------------------------------------------------------------
    printf("[D] GetVariableYInHotPlateData (golden :1736)\n");
    USE_IN_Y_IS_AUTO_PITCH = false;
    TestIF.iARM_Y_PITCH = 3175;
    CHECK(GetVariableYInHotPlateData() == 3175, "VarY: manual pitch -> TestIF.iARM_Y_PITCH(3175)");
    TestIF.iARM_Y_PITCH = 6000;
    CHECK(GetVariableYInHotPlateData() == 6000, "VarY: manual pitch -> TestIF.iARM_Y_PITCH(6000)");

    // -----------------------------------------------------------------------
    //  [E] GetVariableXInHotPlateData(iAction) -- golden :1613, 1x1 branch
    // -----------------------------------------------------------------------
    printf("[E] GetVariableXInHotPlateData (golden :1613)\n");
    iInArmType = e9045_1x1_1;
    bPitchOver12000 = true;                 // 1x1 branch must clear it to false
    int vx = GetVariableXInHotPlateData(iPlaceHP);
    CHECK(vx == iXpitchMaxX3,           "VarX: 1x1 -> iXpitchMaxX3");
    CHECK(bPitchOver12000 == false,     "VarX: 1x1 side-effect bPitchOver12000=false");
    iInArmType = e9045_2x4_8;               // restore
    bPitchOver12000 = false;

    // -----------------------------------------------------------------------
    //  [F] ResetHotPlateSearchParameter() -- golden :2204 post-state oracle
    // -----------------------------------------------------------------------
    printf("[F] ResetHotPlateSearchParameter (golden :2204)\n");
    // dirty the cursors first so the reset is observable
    iPickPlate[0]=5;  iPickPlateX[0]=5;  iPickPlateY[0]=5;
    iPlacePlate[0]=5; iPlacePlateX[0]=5; iPlacePlateY[0]=5;
    iForPlaceHPX6Step=9; iForPickHPX6Step=9; iPickHPOrder=9; iInArmOrder=9;
    OnlyRow2=true;
    // AutoClean off so the eCKPos_HP2 special-case (golden :2215) is NOT taken
    TestIF_File.iAutoClean_Function = 0;
    ResetHotPlateSearchParameter();
    bool bResetOk =
        iPickPlate[0]==0  && iPickPlateX[0]==0  && iPickPlateY[0]==0  &&
        iPlacePlate[0]==0 && iPlacePlateX[0]==0 && iPlacePlateY[0]==0 &&
        iForPlaceHPX6Step==0 && iForPickHPX6Step==0 &&
        iPickHPOrder==0 && iInArmOrder==0 && OnlyRow2==false;
    CHECK(bResetOk, "ResetHotPlateSearchParameter: all cursors/flags cleared");

    // -----------------------------------------------------------------------
    //  [G] SM DoInArmPickFromHotPlate_9045() -- golden case 1 (:757) over Sim HAL
    // -----------------------------------------------------------------------
    printf("[G] DoInArmPickFromHotPlate_9045 SM stepping (golden case 1)\n");
    LastSet.iRunStartMode = 0;            // not rsmAutoSiteMap
    iInArmPickFromHotPlateTask = 1;
    bool bCursorSane = true, bStayEntry = true;
    for (int tick = 0; tick < 32; ++tick)
    {
        bool r = DoInArmPickFromHotPlate_9045();
        if (!pickCursorSane(iInArmPickFromHotPlateTask)) { bCursorSane = false; break; }
        // offline empty list: case 1 returns true and leaves cursor at 1
        if (iInArmPickFromHotPlateTask != 1) bStayEntry = false;
        (void)r;
    }
    CHECK(bCursorSane, "Pick SM: cursor stays in documented set");
    CHECK(bStayEntry,  "Pick SM: offline empty list -> stays at entry (Task==1)");

    // -----------------------------------------------------------------------
    //  [H] PLANNERS over the Sim HAL -- pump, no crash
    // -----------------------------------------------------------------------
    printf("[H] planners over Sim HAL (no crash)\n");
    // KNOWN, NON-DEGENERATE Sim HAL state.  The place planners run real geometry
    // (GetHotPlateYHalfPos + HotPlateYPitchCanPutAll + Row2CanPutHP), several of
    // whose branches do `TestIF.iARM_HP_Y_PITCH % iYPitch` (golden :166/:233);
    // a zero pitch there is an integer divide-by-zero.  So we seed the pitch
    // fields with realistic non-zero values and use iPickRow==1 -- which makes
    // GetHotPlateYHalfPos take the iYHalf=1 fast path (golden :144-147) and
    // HotPlateYPitchCanPutAll early-return false (golden :211) BEFORE any
    // `%iYPitch`, exactly as a real 1-row pick would.  This is "set a known Sim
    // HAL state", not a fudge: it mirrors the InArm engine's own pre-state.
    InitialOK              = true;          // GetHotPlateYHalfPos runs (not early-out)
    InArmSuck.iPickRow     = 1;             // iYHalf=1 fast path, no %iYPitch
    InArmSuck.iPickCol     = 1;
    HotPlateForm.XDivision = 4;
    HotPlateForm.YDivision = 8;
    HotPlateForm.YPitch    = 2000;          // non-zero divisor (defensive)
    HotPlateForm.XPitch    = 2000;
    Prod.HotPlateForm[0].iYPitch = 2000;    // HotPlateYPitchCanPutAll divisor
    TestIF.iARM_HP_Y_PITCH = 2000;
    InArmSuck.Item[0][0]   = HAS_NULL_IC;
    LastSet.iRunStartMode  = 0;
    // SearchPlateToPlace() is the place planner entry.
    SearchPlateToPlace();
    // A representative SearchPlacePlateXItem* leaf (8-suck) -- pumps geometry.
    SearchPlacePlateXItem4_8Suck();
    // CheckHasSpaceToPlace_9045() -- the place-space predicate (gated UI line is
    // #if 0; the rest runs).  Just assert it returns without crashing.
    bool bSpace = CheckHasSpaceToPlace_9045();
    (void)bSpace;
    CHECK(true, "SearchPlateToPlace / SearchPlacePlateXItem4_8Suck / CheckHasSpaceToPlace_9045: pumped, no crash");

    // PURITY re-check: the pure geometry leaf must be idempotent under repeat
    // calls (no HAL perturbation) for the pinned default branch.
    HotPlateForm.XDivision = 4;
    iInArmType = e9045_2x4_8;
    iPlacePlateX[0] = 2;
    int c1 = GetPlaceToHotPlateCol(1);
    int c2 = GetPlaceToHotPlateCol(1);
    CHECK(c1 == c2 && c1 == 3, "GetPlaceToHotPlateCol purity: idempotent (2+1)");

    // -----------------------------------------------------------------------
    printf("==== W6.2 search verify: %d passed, %d failed ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
