// ===========================================================================
//  cUnitConvert.cpp
//  Standard C++ translation of BCB6 cUnitConvert.cpp (V3.33.906.0_20260618).
//
//  BCB6 source: HT9011UC_Code_V3.33.906.0_20260618/cUnitConvert.cpp
//
//  Translation scope (W2 partial extract):
//    TRANSLATED:   iUnitMultiply100  (BCB6 lines 13-20)
//                  iUnitMultiply1000 (BCB6 lines 22-25)
//    SKIPPED:      DoTestIFConvert, DoDeviceConvert, DoHotPlateConvert,
//                  DoArmOffsetConvert, DoArmSpeedConvert, DoDefFormConvert,
//                  DoStructUnitConvert
//                  -> See cUnitConvert.h for full per-function dependency notes
//                     and the targeted wave (W6 for cprod/cmydef globals,
//                     W7 for fShowMessage / ATC_InterfaceForm VCL forms).
//
//  Toolchain: MinGW g++ 6.3+, C++17, CMake 4.0.2 (MinGW Makefiles).
// ===========================================================================

#include "cUnitConvert.h"
#include "vclcompat/vcl_compat.h"   // FormatFloat, AnsiString (was: <vcl.h>)

#include <cstdlib>   // atoi

// ---------------------------------------------------------------------------
// iUnitMultiply100
//   BCB6 source: cUnitConvert.cpp lines 13-20.
//
//   Delphi FormatFloat("0.00", Data*100.00) formats to 2 decimal places,
//   e.g. 234.5 -> "234.50", 3.0 -> "3.00".  atoi() then discards the
//   fractional part, yielding the integer portion.
//
//   The FormatFloat detour (rather than a direct cast) is intentional:
//   it normalises binary floating-point noise.  For example:
//     0.03 * 100.0 = 2.9999999... in IEEE 754
//     (int)(2.9999...) = 2   -- WRONG
//     atoi(FormatFloat("0.00", 2.9999...)) = atoi("3.00") = 3  -- CORRECT
//
//   vclcompat FormatFloat delegates to snprintf("%.2f", v) which provides
//   the same 2-decimal-place rounding as BCB6's Delphi RTL.
// ---------------------------------------------------------------------------
int iUnitMultiply100(double Data)
{
    // BCB6 lines 15-19:
    //   AnsiString asString;
    //   asString=FormatFloat("0.00", Data*100.00);
    //   iReturn=atoi(asString.c_str());
    AnsiString asString;
    asString = FormatFloat("0.00", Data * 100.00);
    return atoi(asString.c_str());
}

// ---------------------------------------------------------------------------
// iUnitMultiply1000
//   BCB6 source: cUnitConvert.cpp lines 22-25.
//
//   Direct double-to-int implicit truncation (no FormatFloat normalisation).
//   BCB6 body: return Data*1000.0;
//   The return type is int, so the compiler performs an implicit narrowing
//   double -> int (truncation toward zero), identical to BCB6 behaviour.
//
//   Example: iUnitMultiply1000(1.234) -> (int)1234.0 = 1234
// ---------------------------------------------------------------------------
int iUnitMultiply1000(double Data)
{
    // BCB6 line 24: return Data*1000.0;
    return static_cast<int>(Data * 1000.0);
}

// ---------------------------------------------------------------------------
// TODO(W6): DoTestIFConvert
//   BCB6 source: cUnitConvert.cpp lines 27-56.
//   Copies TestIF_File -> TestIF (memcpy of full struct), then scales
//   dSiteXPitch, dSiteYPitch, dSiteYOffset, dShiftXPitch,
//   dSiteXCenterPitch, dMulti2DXPitch via iUnitMultiply100.
//   AutoClean X/Y pitch/start/division fields are conditionally copied from
//   _Tray or _Kit variants based on TestIF.bAutoClean_UseTray and
//   IniConfig.bE43AutoCleanUseHotplate.
//   Unblock when: TestIF/TestIF_File structs (cprod.h), IniConfig (cmydef.h).
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// TODO(W6): DoDeviceConvert  (declared as DoDeviceFormConvert in BCB6 .h)
//   BCB6 source: cUnitConvert.cpp lines 58-71.
//   Copies DeviceForm_File -> DeviceForm (memcpy), then scales
//   IndexArmPick[2], IndexPlace[2], IndexDrop[2], IndexContact[2],
//   IndexUp[2], XDimension, YDimension.
//   Unblock when: DeviceForm/DeviceForm_File (cprod.h).
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// TODO(W6): DoHotPlateConvert
//   BCB6 source: cUnitConvert.cpp lines 73-80.
//   Copies HotPlateForm_File -> HotPlateForm (memcpy of TRAY_TYPE_PARA),
//   then scales XPitch, YPitch, XStart, YStart.
//   Unblock when: HotPlateForm/HotPlateForm_File (cprod.h),
//                 TRAY_TYPE_PARA (cmydef.h).
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// TODO(W6): DoArmOffsetConvert
//   BCB6 source: cUnitConvert.cpp lines 82-190.
//   In/Out/Sort arm offset arrays: copies OneByOne flag, scales X/Y/PickUp/
//   Place/Variable/VariableY/Variable2/Variable3/Variable4 and per-picker
//   SingleOffSet dPosOffSetX/Y, dPickUpOffSet, dPlaceOffSet.
//   SortArm guarded by USE_OUT_SORT_ARM != eartUninstall.
//   Tray arm: Offset.iTrayArmX[MAX_TRACK], iTrayArmX_ART[MAX_TRACK].
//   Test offsets: Offset.iIndexArmPickUp/Place/Contact[2], iSHHalft[2],
//   iSHRightPod[2], iSHLeftPod[2], iSHLeft2D[2], iPreciserOpen/Close,
//   dTrayZseparate[MAX_TRACK].
//   Unblock when: InArmOffSet[], OutArmOffSet[], SortArmOffSet[], InOfsTotal,
//   OutOfsTotal, SortOfsTotal, Offset/Offset_File (cprod.h/cmydef.h);
//   InArmSuck, OutArm2Suck (MyKitSuck.h globals);
//   USE_OUT_SORT_ARM, eartUninstall, MAX_TRACK (MachineType.h/MachineDefine.h).
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// TODO(W6+W7): DoArmSpeedConvert
//   BCB6 source: cUnitConvert.cpp lines 192-205.
//   Copies ArmSpeed_File[SpeedPartTotal] -> ArmSpeed (memcpy ARM_CONDITION),
//   scales dRetryDown; copies SHSpeed_File -> SHSpeed (SHUTTLE_SPEED);
//   copies MGSpeed_File -> MGSpeed (MAGAZINE_SPEED).
//   W7 dep: fShowMessage->ShowSpeed(IniConfig.bG05ShowSpeedMessage) -- VCL form.
//   Unblock W6 body when: ArmSpeed/SHSpeed/MGSpeed and _File variants
//   (cprod.h/cmydef.h), SpeedPartTotal, ARM_CONDITION, SHUTTLE_SPEED,
//   MAGAZINE_SPEED, IniConfig.bG05ShowSpeedMessage.
//   W7: replace fShowMessage->ShowSpeed with abstracted interface call.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// TODO(W6): DoDefFormConvert
//   BCB6 source: cUnitConvert.cpp lines 207-241.
//   For i in 0..3: copies UserDefForm_File[i] -> UserDefForm[i] (memcpy
//   TRAY_TYPE_PARA), scales XPitch/YPitch/XStart/YStart/ZDepth,
//   clamps iPickUp (range 60-73 if bC03UseCatchTray else 30-110, default 65),
//   scales iPickUp/BlockXStart/BlockYStart/BlockPitchX/BlockPitchY/
//   BlockTraySize.
//   If USE_LdUldCassetteMode==1: also scales UserDefForm[0].dCassetteZPitch/
//   dCassetteZStart.
//   Unblock when: UserDefForm/UserDefForm_File (cprod.h/cmydef.h);
//   IniConfig.bC03UseCatchTray; USE_LdUldCassetteMode (MachineType.h).
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// TODO(W6+W7): DoStructUnitConvert
//   BCB6 source: cUnitConvert.cpp lines 243-260.
//   Orchestrates: DoTestIFConvert, DoDeviceConvert, DoHotPlateConvert,
//   DoArmOffsetConvert, DoArmSpeedConvert, DoDefFormConvert.
//   Then: if Temperature.bATCActiveCooling && LastSet.iTemperature==Tempture_Ambient
//   && ATC_InterfaceForm->iATC_MODE_TYPE in {ATC_TYPE_33, ATC_TYPE_35, ATC_TYPE_61}
//   && Temperature.bActiveHeatGun && Temperature.dATC_HotGunTime!=0:
//     ArmSpeed[OutArm].dWaitOnSH += Temperature.dATC_HotGunTime
//   W7 dep: ATC_InterfaceForm->iATC_MODE_TYPE -- VCL form pointer.
//   Unblock W6 body when all Do*Convert unblocked.
//   W7: replace ATC_InterfaceForm with abstracted interface.
// ---------------------------------------------------------------------------

// ###########################################################################
// #                                                                         #
// #   PT-W8 APPEND (20260811)  --  the seven Do*Convert bodies              #
// #                                                                         #
// ###########################################################################
//
//  NOTE ON THE FILE-HEAD BANNER ABOVE
//  ----------------------------------
//  Lines 10-15 ("SKIPPED: DoTestIFConvert, DoDeviceConvert, DoHotPlateConvert,
//  DoArmOffsetConvert, DoArmSpeedConvert, DoDefFormConvert,
//  DoStructUnitConvert") and the seven TODO(W6)/TODO(W6+W7) comment blocks at
//  lines 70-156 are SUPERSEDED by this section -- all seven are translated
//  below.  This wave was append-only on this mirror and therefore could not
//  rewrite them; retiring those two stale regions (and the matching ones in
//  cUnitConvert.h) is an explicit main-loop action item, reported.
//
//  ROLE
//  ----
//  cUnitConvert.cpp is the RECIPE-UNIT NORMALISER.  Every "<X>_File" struct in
//  cprod.h holds a value exactly as it was read off the recipe / teach files,
//  in millimetres (double).  Its non-"_File" twin holds the SAME value in the
//  machine's internal integer unit (1 count = 0.01 mm), produced by
//  iUnitMultiply100() above.  The seven functions below are that one-way
//  "_File -> live" conversion pass, plus one ATC hot-gun speed fix-up.
//
//  WHO PUMPS IT
//  ------------
//  Nothing pumps this on a timer; it is called explicitly, once, whenever a
//  recipe / site-map / teach change lands.  Golden call sites and their state
//  in THIS tree as of this wave:
//    golden cinitial.cpp:13497 (SetWorkParameter) -> port cinitial.cpp:7023, inside `#if 0` (N3-G8)
//    golden AutoClean DoAutoCleanKit case 1       -> port AutoClean/AutoClean.cpp:8320, resolves to the FILE-STATIC no-op at AutoClean.cpp:199
//    golden auto9045 ChangeSite tail              -> port Automation/auto9045.cpp:871, macro-renamed to W5FA_DoStructUnitConvert (auto9045.cpp:148-149)
//    golden ckernel SetWorkParameter              -> port ckernel_shims.cpp:282, macro-renamed to W7L2_DoStructUnitConvert (ckernel_shims.cpp:113)
//    golden csystem DoTrayFeedProcess             -> port csystem.cpp:9927 (DoArmSpeedConvert), inside GATE G4-3 `#if 0`
//    golden AutoClean SetAutoCleanICCount         -> port AutoClean/AutoClean.cpp:3708 (DoTestIFConvert), inside `#if 0`
//  ==> NOTHING in the linked tree reaches these bodies yet.  See the trap-1
//      answer in this wave's report: three separate TU-LOCAL stand-ins (one
//      `static` function + two `#define` renames) currently eat the need, so
//      the build stays green whether or not this file is linked in.  Retiring
//      them is a main-loop action item, listed in the report -- NOT done here
//      (those files belong to other agents this wave).
//
//  WAVE SCOPE (one line per golden function; golden file = cUnitConvert.cpp)
//  ------------------------------------------------------------------------
//    DoTestIFConvert       golden :27   ACTIVE (gate PTW8-UC-1 on the memcpy)
//    DoDeviceConvert       golden :58   ACTIVE (gate PTW8-UC-2 on the memcpy)
//    DoHotPlateConvert     golden :73   ACTIVE (no gate -- TRAY_TYPE_PARA is POD)
//    DoArmOffsetConvert    golden :82   ACTIVE (gate PTW8-UC-3: NULL ARM_OFFSET* guard)
//    DoArmSpeedConvert     golden :192  ACTIVE (no gate -- fShowMessage facade is real and non-NULL)
//    DoDefFormConvert      golden :207  ACTIVE (no gate)
//    DoStructUnitConvert   golden :243  ACTIVE (gate PTW8-UC-4: local ATC_TYPE_* constants)
//
//  GATE REGISTER
//  -------------
//  --- GATE PTW8-UC-1 -- golden cUnitConvert.cpp:29 -------------------------
//    GOLDEN LINE:
//      memcpy(&TestIF.iTestMode, &TestIF_File.iTestMode, sizeof(TestIF_File));
//    WHAT WAS DONE: replaced by the whole-object copy assignment
//      TestIF = TestIF_File;
//    WHY IT IS FAITHFUL: `iTestMode` is the FIRST member of SYSTEM_TEST_IF
//      (cprod.h:1651) and there is no base / vptr, so &TestIF_File.iTestMode ==
//      (char*)&TestIF_File; and sizeof(TestIF_File) == sizeof(SYSTEM_TEST_IF).
//      Golden's memcpy therefore covers EXACTLY the whole object, no more and
//      no less -- i.e. golden's intent is "copy every member", which is what
//      copy-assignment does.  Every subsequent read of every field yields the
//      same value under both forms.
//    WHY A RAW memcpy COULD NOT BE KEPT: the port's SYSTEM_TEST_IF carries 29
//      AnsiString members (cprod.h:1652 sTestMode, :1672 sDioName, :1885
//      sOcrText, ... :2040 asFTPPassword).  In THIS tree AnsiString owns a
//      std::string (vclcompat/AnsiString.h:167), a real heap-owning C++ object;
//      blitting raw bytes over it leaves source and destination believing they
//      own the same buffer (or, under SSO, leaves the destination's internal
//      pointer aimed INTO the source object) -- a double-free / heap-corruption
//      hazard, not a style point.  In BCB6 the same member is a single
//      ref-counted char*, so golden's memcpy is merely a refcount leak.  The
//      identical analysis was already written down independently at
//      AutoClean/AutoClean.cpp:3686-3706, which is why that call site is gated.
//    REAL-MACHINE DIFFERENCE: none observable.  Golden additionally leaks /
//      aliases the AnsiString refcounts; this does not.
//
//  --- GATE PTW8-UC-2 -- golden cUnitConvert.cpp:60 -------------------------
//    GOLDEN LINE:
//      memcpy(&DeviceForm.IndexArmPick[0], &DeviceForm_File.IndexArmPick[0], sizeof(DeviceForm_File));
//    Identical treatment and identical reasoning to PTW8-UC-1.  IndexArmPick[]
//    is the FIRST member of SYSTEM_DEVICE_FORM (cprod.h:1157); the struct
//    carries one AnsiString (cprod.h:1180 CalCCDIP).  Replaced by
//      DeviceForm = DeviceForm_File;
//    REAL-MACHINE DIFFERENCE: none observable.
//
//  --- GATE PTW8-UC-3 -- golden cUnitConvert.cpp:87 / :113 / :141 -----------
//    GOLDEN LINES: the three arm-offset loop bodies dereference
//      InArmOffSet[i]   / InArmOffSet_File[i]    (golden :87-107)
//      OutArmOffSet[i]  / OutArmOffSet_File[i]   (golden :113-133)
//      SortArmOffSet[i] / SortArmOffSet_File[i]  (golden :141-161)
//    ABSENCE CLAIM + COMMAND + TIME: all six arrays are defined with static
//      storage duration at port cprod.cpp:67-72 (`class ARM_OFFSET
//      *InArmOffSet[InOfsTotal];` etc.) and are therefore ALL-NULL, because
//      nothing in this tree ever allocates them:
//        grep -rn "new ARM_OFFSET" --include=*.cpp --include=*.h .
//          (from D:/HT9045/HT9011UC_Cpp_V3.33.906.0)  ->  0 matches
//          [first run 2026-08-11T11:24+0800; RE-RUN immediately before delivery
//           2026-08-11T11:57+0800 -- still 0]
//      Golden allocates them in a loop at golden main.cpp:2125-2138, and
//      main.cpp is one of the units not yet started.
//    WHAT WAS DONE: `if(<ptr>[i]==NULL || <ptr>_File[i]==NULL) continue;` at the
//      top of each of the three loop bodies -- the treatment
//      docs/PT_CAMPAIGN_PLAN.md section 8 prescribes for exactly this family
//      ("guard the CALL SITE with if(pointer) and leave a GA-3 hand-off note"),
//      same shape as the `if(elLaser)` guard that section records for
//      OmronLaser/LaserSensor.cpp.  NOTE these six arrays are NOT in that
//      section's table of 18 -- that census enumerated scalar `X = new T;`
//      sites and so missed six ARRAY allocation loops.  Reported.
//    WHY THE OFFLINE DEFAULT IS FAITHFUL: with no allocation there is no
//      recipe-side ARM_OFFSET object to convert FROM and no live one to convert
//      INTO, so "skip" is the only meaning "convert them" can have; the live
//      offset tables simply stay at their zero-initialised values, which is the
//      same state the offline handler is in before any recipe is loaded.
//    REAL-MACHINE DIFFERENCE: once GA-3 lands golden main.cpp:2125-2138 the
//      guard is always true and every loop runs golden's body verbatim.  Until
//      then, calling DoArmOffsetConvert WITHOUT this guard would SIGSEGV on the
//      very first statement of the very first iteration.
//
//  --- GATE PTW8-UC-4 -- golden cUnitConvert.cpp:253-255 --------------------
//    GOLDEN LINES: ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33 / _35 / _61
//    ABSENCE CLAIM + COMMAND + TIME: ATC_TYPE_33 / ATC_TYPE_35 / ATC_TYPE_61
//      are `#define`s in golden ATC/ATC_Handler_Side.h:24 / :25 / :30, and that
//      header is NOT translated -- port ATC/ holds only ATCInterface,
//      ATCSystem, ATC_WinWay and TCPData:
//        grep -rln "define ATC_TYPE_33" --include=*.h .
//          (from D:/HT9045/HT9011UC_Cpp_V3.33.906.0)  ->  0 matches
//          [first run 2026-08-11T11:20+0800; RE-RUN before delivery
//           2026-08-11T11:57+0800 -- still 0]
//    WHAT WAS DONE: file-local `#ifndef/#define` with golden's exact values,
//      following the precedent already set by aTester_Front.cpp:169-170,
//      aTester_Rear.cpp:415-419 and csystem.cpp:20227-20230 for the identical
//      constants.  Guarded with #ifndef so the day ATC_Handler_Side.h lands,
//      the real header wins with no edit here.
//    REAL-MACHINE DIFFERENCE: NONE from the constants themselves (values are
//      verbatim).  The BRANCH, however, is offline-dead for a different reason:
//      ATC_InterfaceForm is the TATC_InterfaceFormShim of acarry_shims.h:107-115
//      whose ctor sets iATC_MODE_TYPE(0) (acarry_shims.cpp:72) and which nothing
//      offline ever writes -- so the hot-gun `dWaitOnSH` bump at golden :258 can
//      never fire offline.  On a real ATC 3.3 / 3.5 / 6.1 machine in Ambient
//      with active cooling + heat gun it DOES fire, adding
//      Temperature.dATC_HotGunTime to the OutArm wait-on-shuttle time.
//
//  NON-GATE NOTES (things that look like gaps and are not)
//  ------------------------------------------------------
//   * fShowMessage (golden :204) is REAL here: forms/fShowMessage.h:22-28 +
//     forms/fShowMessage.cpp:11 `TfShowMessage *fShowMessage = new
//     TfShowMessage();`.  Non-NULL from static init, ShowSpeed() is an offline
//     no-op (golden pops a debug speed window).  Called unguarded, matching
//     golden and matching the four existing port call sites at cinitial.cpp:
//     5409 / 10675 / 16684 and AutoClean/AutoClean.cpp:980.
//   * TMyKitSuck InArmSuck / OutArm2Suck (golden :98 / :124 / :152): golden
//     includes "MyKitSuck.h"; THIS FILE DELIBERATELY DOES NOT.  Two headers
//     declare a class named TMyKitSuck with DIFFERENT LAYOUTS -- mykitsuck.h:274
//     and aHotPlateSubstrate.h:365 -- and mykitsuck.cpp is deliberately NOT
//     registered in CMakeLists (see CMakeLists.txt:2099-2112).  The definitions
//     that are actually linked are aHotPlateSubstrate.cpp:80 (InArmSuck) and
//     :85 (OutArm2Suck), so aHotPlateSubstrate.h is the ONLY correct header to
//     reach them from.  See the trap-5 answer in this wave's report.
//   * Offline InArmSuck.iMotRow / iMotCol / OutArm2Suck.* are 0 (static
//     zero-init; only cinitial's motor-table read sets them), so the per-nozzle
//     SingleOffSet loops execute zero times offline.  That is data state, not a
//     gate -- golden behaves identically on a machine whose motor table has not
//     been read yet.
//
//  Toolchain: MinGW g++ 6.3+, C++17; verified with
//    g++ -std=c++17 -fsyntax-only -DMN200DLL_EXPORTS -D_WIN32_WINNT=0x0601
//        -DWINVER=0x0601 -I. -IMotor -IMotor/vendor -IEtherCAT/vendor
//        -Ithird_party/sqlite3 cUnitConvert.cpp
// ===========================================================================

// Includes for the PT-W8 append.  Placed HERE rather than at the top of the
// file because this wave was append-only on this mirror; nothing above is
// affected (all of it is already compiled by the time these are seen).
#include "cprod.h"                  // TestIF/TestIF_File, DeviceForm(_File), HotPlateForm(_File),
                                    // UserDefForm(_File), Offset(_File), ArmSpeed(_File), SHSpeed(_File),
                                    // MGSpeed(_File), Temperature, ARM_OFFSET + the six offset arrays;
                                    // pulls MachineType.h (InOfsTotal/OutOfsTotal/SortOfsTotal/MAX_TRACK/
                                    // SpeedPartTotal/OutArm/eartUninstall) and Config.h (IniConfig)
#include "cmydef.h"                 // USE_OUT_SORT_ARM, USE_LdUldCassetteMode, Tempture_Ambient
#include "LastSet.h"                // LastSet.iTemperature
#include "aHotPlateSubstrate.h"     // golden "MyKitSuck.h" AS LINKED: TMyKitSuck InArmSuck (:624) /
                                    // OutArm2Suck (:629) -- see the ODR note in the banner above
#include "forms/fShowMessage.h"     // fShowMessage->ShowSpeed  (golden uShowMessage.h)
#include "acarry_shims.h"           // ATC_InterfaceForm        (golden ATC/ATC_Handler_Side.h)

#include <cstring>                  // memcpy

// GATE PTW8-UC-4 -- see the register above.  Values verbatim from golden
// ATC/ATC_Handler_Side.h:24 / :25 / :30.
#ifndef ATC_TYPE_33
#define ATC_TYPE_33         33                                                  // golden ATC/ATC_Handler_Side.h:24
#endif
#ifndef ATC_TYPE_35
#define ATC_TYPE_35         35                                                  // golden ATC/ATC_Handler_Side.h:25
#endif
#ifndef ATC_TYPE_61
#define ATC_TYPE_61         61                                                  // golden ATC/ATC_Handler_Side.h:30
#endif

// ---------------------------------------------------------------------------
//  DoTestIFConvert
//  golden cUnitConvert.cpp:27-56
//
//  Bulk-copies the recipe-side SYSTEM_TEST_IF onto the live one, then re-scales
//  the six pitch/offset fields into 0.01 mm counts, then picks the AutoClean
//  geometry from either the TRAY set or the KIT set.
// ---------------------------------------------------------------------------
void DoTestIFConvert()
{
#if 0   // GOLDEN VERBATIM -- golden cUnitConvert.cpp:29.  GATE PTW8-UC-1 (memcpy over 29 AnsiString members); the line below is the value-identical, memory-safe form.
    memcpy(&TestIF.iTestMode, &TestIF_File.iTestMode, sizeof(TestIF_File));
#endif
    TestIF = TestIF_File;                                                       // GATE PTW8-UC-1 -- golden :29
    TestIF.dSiteXPitch = iUnitMultiply100(TestIF_File.dSiteXPitch);
    TestIF.dSiteYPitch = iUnitMultiply100(TestIF_File.dSiteYPitch);
    TestIF.dSiteYOffset= iUnitMultiply100(TestIF_File.dSiteYOffset);            //Steven 20140512 : For HT-9047
    TestIF.dShiftXPitch= iUnitMultiply100(TestIF_File.dShiftXPitch);            //wei 20160226 TSMC X Shift
    TestIF.dSiteXCenterPitch=iUnitMultiply100(TestIF_File.dSiteXCenterPitch);   //Steven 20170706 (wei) : 2x4中間的Pitch不同 for SCC
    TestIF.dMulti2DXPitch = iUnitMultiply100(TestIF_File.dMulti2DXPitch);       //Steven 20240612 : for 對角2D

    //ChungHung 20130426 add
    if(TestIF.bAutoClean_UseTray && IniConfig.bE43AutoCleanUseHotplate==false)  //Steven 20160630 : fixed for AutoCleanUseHotplate
    {
        TestIF.dAutoClean_XPitch=TestIF_File.dAutoClean_XPitch_Tray;
        TestIF.dAutoClean_YPitch=TestIF_File.dAutoClean_YPitch_Tray;
        TestIF.dAutoClean_XStart=TestIF_File.dAutoClean_XStart_Tray;
        TestIF.dAutoClean_YStart=TestIF_File.dAutoClean_YStart_Tray;
        TestIF.iAutoClean_XDivision=TestIF_File.iAutoClean_XDivision_Tray;
        TestIF.iAutoClean_YDivision=TestIF_File.iAutoClean_YDivision_Tray;
    }
    else
    {
        TestIF.dAutoClean_XPitch=TestIF_File.dAutoClean_XPitch_Kit;             //ChungHung 20130708 TestIF--->TestIF_File
        TestIF.dAutoClean_YPitch=TestIF_File.dAutoClean_YPitch_Kit;
        TestIF.dAutoClean_XStart=TestIF_File.dAutoClean_XStart_Kit;
        TestIF.dAutoClean_YStart=TestIF_File.dAutoClean_YStart_Kit;
        TestIF.iAutoClean_XDivision=TestIF_File.iAutoClean_XDivision_Kit;
        TestIF.iAutoClean_YDivision=TestIF_File.iAutoClean_YDivision_Kit;
    }
}
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  DoDeviceConvert
//  golden cUnitConvert.cpp:58-71
//
//  NOTE: golden's own header (golden cUnitConvert.h) spells this
//  DoDeviceFormConvert; the DEFINITION is DoDeviceConvert.  The definition name
//  is what every caller links against, so the definition name is what is used
//  here -- golden's inconsistency is preserved, not fixed.
// ---------------------------------------------------------------------------
void DoDeviceConvert()
{
#if 0   // GOLDEN VERBATIM -- golden cUnitConvert.cpp:60.  GATE PTW8-UC-2 (memcpy over an AnsiString member).
    memcpy(&DeviceForm.IndexArmPick[0], &DeviceForm_File.IndexArmPick[0], sizeof(DeviceForm_File));
#endif
    DeviceForm = DeviceForm_File;                                               // GATE PTW8-UC-2 -- golden :60
    for(int i=0; i<2; i++)
    {
        DeviceForm.IndexArmPick[i]  =iUnitMultiply100(DeviceForm_File.IndexArmPick[i]);
        DeviceForm.IndexPlace[i]    =iUnitMultiply100(DeviceForm_File.IndexPlace[i]);
        DeviceForm.IndexDrop[i]     =iUnitMultiply100(DeviceForm_File.IndexDrop[i]);
        DeviceForm.IndexContact[i]  =iUnitMultiply100(DeviceForm_File.IndexContact[i]);
        DeviceForm.IndexUp[i]       =iUnitMultiply100(DeviceForm_File.IndexUp[i]);
    }
    DeviceForm.XDimension       =iUnitMultiply100(DeviceForm_File.XDimension);
    DeviceForm.YDimension       =iUnitMultiply100(DeviceForm_File.YDimension);
}
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  DoHotPlateConvert
//  golden cUnitConvert.cpp:73-80
//
//  TRAY_TYPE_PARA (cprod.h:1256-1298) is POD in this tree -- its only two
//  AnsiString members are commented out at cprod.h:1269 / :1273 -- so golden's
//  memcpy is kept VERBATIM here (no PTW8-UC-1 style hazard).  XPitch is the
//  first member (cprod.h:1258), so &HotPlateForm.XPitch == &HotPlateForm and
//  the sizeof(TRAY_TYPE_PARA) span is exactly the whole object.  Same shape as
//  the four existing verbatim TRAY_TYPE_PARA memcpys at cinitial.cpp:14438-14477.
// ---------------------------------------------------------------------------
void DoHotPlateConvert()
{
    memcpy(&HotPlateForm.XPitch,&HotPlateForm_File.XPitch,sizeof(TRAY_TYPE_PARA));
    HotPlateForm.XPitch = iUnitMultiply100(HotPlateForm_File.XPitch);
    HotPlateForm.YPitch = iUnitMultiply100(HotPlateForm_File.YPitch);
    HotPlateForm.XStart = iUnitMultiply100(HotPlateForm_File.XStart);
    HotPlateForm.YStart = iUnitMultiply100(HotPlateForm_File.YStart);
}
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  DoArmOffsetConvert
//  golden cUnitConvert.cpp:82-190
//
//  In-arm / out-arm / (9046AU) sort-arm ARM_OFFSET objects, then the tray-arm
//  and index/shuttle scalar offsets in RUN_OFFSET.
//
//  GATE PTW8-UC-3 (see the register in the banner): the three ARM_OFFSET loops
//  carry a NULL guard because nothing in this tree allocates the six pointer
//  arrays yet (golden main.cpp:2125-2138).  Offset / Offset_File are by-value
//  RUN_OFFSET structs (cprod.h:258-259) and need no guard.
//
//  GOLDEN QUIRK PRESERVED: the SORT-arm inner loops (golden :152 / :154) are
//  bounded by OutArm2Suck, while the OUT-arm inner loops (golden :124 / :126)
//  are bounded by InArmSuck -- NOT OutArmSuck.  That looks like a copy/paste
//  slip in golden but it is golden's behaviour and is reproduced exactly.
// ---------------------------------------------------------------------------
void DoArmOffsetConvert()
{
    //In Arm
    for(int i=0; i<InOfsTotal; i++)                                             //Steven 20140512
    {
        // GATE PTW8-UC-3 -- golden :87 has no guard; see the register above.
        if(InArmOffSet[i]==NULL || InArmOffSet_File[i]==NULL)  continue;
        InArmOffSet[i]->SetOneByOne(InArmOffSet_File[i]->GetOneByOne());
        InArmOffSet[i]->SetX(iUnitMultiply100(InArmOffSet_File[i]->GetX()));
        InArmOffSet[i]->SetY(iUnitMultiply100(InArmOffSet_File[i]->GetY()));
        InArmOffSet[i]->SetPickUp(iUnitMultiply100(InArmOffSet_File[i]->GetPickUp()));
        InArmOffSet[i]->SetPlace(iUnitMultiply100(InArmOffSet_File[i]->GetPlace()));
        InArmOffSet[i]->SetVariable(iUnitMultiply100(InArmOffSet_File[i]->GetVariable()));
        InArmOffSet[i]->SetVariableY(iUnitMultiply100(InArmOffSet_File[i]->GetVariableY()));                            //Steven 20140512 : XY變距
        InArmOffSet[i]->SetVariable2(iUnitMultiply100(InArmOffSet_File[i]->GetVariable2()));                            //Steven 20140512 : XY變距
        InArmOffSet[i]->SetVariable3(iUnitMultiply100(InArmOffSet_File[i]->GetVariable3()));
        InArmOffSet[i]->SetVariable4(iUnitMultiply100(InArmOffSet_File[i]->GetVariable4()));

        for(int j=0; j<InArmSuck.iMotRow; j++)
        {
            for(int k=0; k<InArmSuck.iMotCol; k++)
            {
                InArmOffSet[i]->SingleOffSet->dPosOffSetX[j][k]     = iUnitMultiply100(InArmOffSet_File[i]->SingleOffSet->dPosOffSetX[j][k]);
                InArmOffSet[i]->SingleOffSet->dPosOffSetY[j][k]     = iUnitMultiply100(InArmOffSet_File[i]->SingleOffSet->dPosOffSetY[j][k]);
                InArmOffSet[i]->SingleOffSet->dPickUpOffSet[j][k]   = iUnitMultiply100(InArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[j][k]);
                InArmOffSet[i]->SingleOffSet->dPlaceOffSet[j][k]    = iUnitMultiply100(InArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[j][k]);
            }
        }
    }

    //Out Arm
    for(int i=0; i<OutOfsTotal; i++)                                            //Steven 20140512
    {
        // GATE PTW8-UC-3 -- golden :113 has no guard; see the register above.
        if(OutArmOffSet[i]==NULL || OutArmOffSet_File[i]==NULL)  continue;
        OutArmOffSet[i]->SetOneByOne    (OutArmOffSet_File[i]->GetOneByOne());
        OutArmOffSet[i]->SetX           (iUnitMultiply100(OutArmOffSet_File[i]->GetX()));
        OutArmOffSet[i]->SetY           (iUnitMultiply100(OutArmOffSet_File[i]->GetY()));
        OutArmOffSet[i]->SetPickUp      (iUnitMultiply100(OutArmOffSet_File[i]->GetPickUp()));
        OutArmOffSet[i]->SetPlace       (iUnitMultiply100(OutArmOffSet_File[i]->GetPlace()));
        OutArmOffSet[i]->SetVariable    (iUnitMultiply100(OutArmOffSet_File[i]->GetVariable()));
        OutArmOffSet[i]->SetVariableY   (iUnitMultiply100(OutArmOffSet_File[i]->GetVariableY()));                       //Steven 20140512 : XY變距
        OutArmOffSet[i]->SetVariable2   (iUnitMultiply100(OutArmOffSet_File[i]->GetVariable2()));                       //Steven 20140512 : XY變距
        OutArmOffSet[i]->SetVariable3   (iUnitMultiply100(OutArmOffSet_File[i]->GetVariable3()));
        OutArmOffSet[i]->SetVariable4   (iUnitMultiply100(OutArmOffSet_File[i]->GetVariable4()));

        for(int j=0; j<InArmSuck.iMotRow; j++)
        {
            for(int k=0; k<InArmSuck.iMotCol; k++)
            {
                OutArmOffSet[i]->SingleOffSet->dPosOffSetX[j][k]    = iUnitMultiply100(OutArmOffSet_File[i]->SingleOffSet->dPosOffSetX[j][k]);
                OutArmOffSet[i]->SingleOffSet->dPosOffSetY[j][k]    = iUnitMultiply100(OutArmOffSet_File[i]->SingleOffSet->dPosOffSetY[j][k]);
                OutArmOffSet[i]->SingleOffSet->dPickUpOffSet[j][k]  = iUnitMultiply100(OutArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[j][k]);
                OutArmOffSet[i]->SingleOffSet->dPlaceOffSet[j][k]   = iUnitMultiply100(OutArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[j][k]);
            }
        }
    }

    if(USE_OUT_SORT_ARM!=eartUninstall)                                         //RogerYang 20250417 for HT9046AU add
    {
        //Sort Arm
        for(int i=0; i<SortOfsTotal; i++)
        {
            // GATE PTW8-UC-3 -- golden :141 has no guard; see the register above.
            if(SortArmOffSet[i]==NULL || SortArmOffSet_File[i]==NULL)  continue;
            SortArmOffSet[i]->SetOneByOne    (SortArmOffSet_File[i]->GetOneByOne());
            SortArmOffSet[i]->SetX           (iUnitMultiply100(SortArmOffSet_File[i]->GetX()));
            SortArmOffSet[i]->SetY           (iUnitMultiply100(SortArmOffSet_File[i]->GetY()));
            SortArmOffSet[i]->SetPickUp      (iUnitMultiply100(SortArmOffSet_File[i]->GetPickUp()));
            SortArmOffSet[i]->SetPlace       (iUnitMultiply100(SortArmOffSet_File[i]->GetPlace()));
            SortArmOffSet[i]->SetVariable    (iUnitMultiply100(SortArmOffSet_File[i]->GetVariable()));
            SortArmOffSet[i]->SetVariableY   (iUnitMultiply100(SortArmOffSet_File[i]->GetVariableY()));
            SortArmOffSet[i]->SetVariable2   (iUnitMultiply100(SortArmOffSet_File[i]->GetVariable2()));
            SortArmOffSet[i]->SetVariable3   (iUnitMultiply100(SortArmOffSet_File[i]->GetVariable3()));
            SortArmOffSet[i]->SetVariable4   (iUnitMultiply100(SortArmOffSet_File[i]->GetVariable4()));

            for(int j=0; j<OutArm2Suck.iMotRow; j++)
            {
                for(int k=0; k<OutArm2Suck.iMotCol; k++)
                {
                    SortArmOffSet[i]->SingleOffSet->dPosOffSetX[j][k]    = iUnitMultiply100(SortArmOffSet_File[i]->SingleOffSet->dPosOffSetX[j][k]);
                    SortArmOffSet[i]->SingleOffSet->dPosOffSetY[j][k]    = iUnitMultiply100(SortArmOffSet_File[i]->SingleOffSet->dPosOffSetY[j][k]);
                    SortArmOffSet[i]->SingleOffSet->dPickUpOffSet[j][k]  = iUnitMultiply100(SortArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[j][k]);
                    SortArmOffSet[i]->SingleOffSet->dPlaceOffSet[j][k]   = iUnitMultiply100(SortArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[j][k]);
                }
            }
        }
    }

    //Tray Arm
    for(int i=0; i<MAX_TRACK; i++)
    {
        Offset.iTrayArmX[i]     = iUnitMultiply100(Offset_File.iTrayArmX[i]);
        Offset.iTrayArmX_ART[i] = iUnitMultiply100(Offset_File.iTrayArmX_ART[i]);                                       //kevin 20170831 (Steven) add ART offset
    }

    //Test
    for(int i=0; i<2; i++)
    {
        Offset.iIndexArmPickUp[i]   = iUnitMultiply100(Offset_File.iIndexArmPickUp[i]);
        Offset.iIndexArmPlace[i]    = iUnitMultiply100(Offset_File.iIndexArmPlace[i]);
        Offset.iIndexArmContact[i]  = iUnitMultiply100(Offset_File.iIndexArmContact[i]);
        Offset.iSHHalft[i]          = iUnitMultiply100(Offset_File.iSHHalft[i]);
        Offset.iSHRightPod[i]       = iUnitMultiply100(Offset_File.iSHRightPod[i]);
        Offset.iSHLeftPod[i]        = iUnitMultiply100(Offset_File.iSHLeftPod[i]);
        Offset.iSHLeft2D[i]         = iUnitMultiply100(Offset_File.iSHLeft2D[i]);                                       //Steven 20151218 : Offset for 2d reader
    }
    Offset.iPreciserOpen       = iUnitMultiply100(Offset_File.iPreciserOpen);   //JerryYang 20180820 add
    Offset.iPreciserClose      = iUnitMultiply100(Offset_File.iPreciserClose);

    for(int i=0; i<MAX_TRACK; i++)                                              //JerryYang 20200805 fix tray z motor offset
    {
        Offset.dTrayZseparate[i]=iUnitMultiply100(Offset_File.dTrayZseparate[i]);
    }
}
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  DoArmSpeedConvert
//  golden cUnitConvert.cpp:192-205
//
//  ARM_CONDITION / SHUTTLE_SPEED / MAGAZINE_SPEED are all POD in this tree (no
//  AnsiString / TStringList members -- verified field-by-field over
//  cprod.h:2821-2874 / :2927-2943 / :2947-2956), and in each case the memcpy's
//  base member is the struct's FIRST member (iBodySP :2823, iSH1Sp :2929,
//  iCYSp :2949), so all three memcpys are kept VERBATIM.
//
//  GOLDEN QUIRK PRESERVED: the memcpy at golden :197 copies dRetryDown along
//  with everything else, and the very next line immediately overwrites it with
//  the scaled value.  Redundant, harmless, kept.
// ---------------------------------------------------------------------------
void DoArmSpeedConvert()
{
    int i;
    for(i=0; i<SpeedPartTotal; i++)                                             //Steven 20140428 : For Rotate 4 --> SpeedPartTotal
    {
        memcpy(&ArmSpeed[i].iBodySP, &ArmSpeed_File[i].iBodySP, sizeof(ARM_CONDITION));
        ArmSpeed[i].dRetryDown = iUnitMultiply100(ArmSpeed_File[i].dRetryDown);
    }
    memcpy(&SHSpeed.iSH1Sp, &SHSpeed_File.iSH1Sp, sizeof(SHUTTLE_SPEED));

    memcpy(&MGSpeed.iCYSp, &MGSpeed_File.iCYSp, sizeof(MAGAZINE_SPEED));        //JerryYang 20220909 : add magazine

    fShowMessage->ShowSpeed(IniConfig.bG05ShowSpeedMessage);
}
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  DoDefFormConvert
//  golden cUnitConvert.cpp:207-241
//
//  The four user-defined tray forms.  TRAY_TYPE_PARA is POD (see
//  DoHotPlateConvert above), so the memcpy is verbatim.
//
//  GOLDEN QUIRK PRESERVED: the iPickUp range clamp at golden :220-227 writes
//  back into UserDefForm_File[i] -- the RECIPE-side struct -- not into the live
//  one.  That is an in-place sanitisation of the recipe value (so it survives
//  the next save), and it is reproduced exactly.  Note it also makes the clamp
//  sticky across repeated calls.
//
//  INTEGER/FLOAT NOTE: iPickUp is `double iPickUp;` (cprod.h:1270) despite the
//  `i` prefix, so `<60`, `>73`, `<30`, `>110` and `=65` are all double compares
//  / a double store, exactly as in golden.  No int truncation is introduced.
// ---------------------------------------------------------------------------
void DoDefFormConvert()
{
    for(int i=0; i<4; i++)
    {
        memcpy(&UserDefForm[i].XPitch, &UserDefForm_File[i].XPitch, sizeof(TRAY_TYPE_PARA));
        UserDefForm[i].XPitch       = iUnitMultiply100(UserDefForm_File[i].XPitch);
        UserDefForm[i].YPitch       = iUnitMultiply100(UserDefForm_File[i].YPitch);
        UserDefForm[i].XStart       = iUnitMultiply100(UserDefForm_File[i].XStart);
        UserDefForm[i].YStart       = iUnitMultiply100(UserDefForm_File[i].YStart);
        UserDefForm[i].ZDepth       = iUnitMultiply100(UserDefForm_File[i].ZDepth);

        if(IniConfig.bC03UseCatchTray)                                          //kevin 20220302 夾TRAY 模組 限定 TRAY 寬度避免 被 IN OUT ARM 撞機
        {
            if(UserDefForm_File[i].iPickUp<60 || UserDefForm_File[i].iPickUp>73)
                UserDefForm_File[i].iPickUp=65;
        }
        else
        {
            if(UserDefForm_File[i].iPickUp<30 || UserDefForm_File[i].iPickUp>110)
                UserDefForm_File[i].iPickUp=65;
        }
        UserDefForm[i].iPickUp          = iUnitMultiply100(UserDefForm_File[i].iPickUp);                                //Steven 20220523 : Unmark
        UserDefForm[i].BlockXStart      = iUnitMultiply100(UserDefForm_File[i].BlockXStart);
        UserDefForm[i].BlockYStart      = iUnitMultiply100(UserDefForm_File[i].BlockYStart);
        UserDefForm[i].BlockPitchX      = iUnitMultiply100(UserDefForm_File[i].BlockPitchX);
        UserDefForm[i].BlockPitchY      = iUnitMultiply100(UserDefForm_File[i].BlockPitchY);
        UserDefForm[i].BlockTraySize    = iUnitMultiply100(UserDefForm_File[i].BlockTraySize);
    }

    if(USE_LdUldCassetteMode==1)                                                //RogerYang 20260203 : Add for HT9046CR
    {
        UserDefForm[0].dCassetteZPitch  = iUnitMultiply100(UserDefForm_File[0].dCassetteZPitch);
        UserDefForm[0].dCassetteZStart  = iUnitMultiply100(UserDefForm_File[0].dCassetteZStart);
    }
}
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  DoStructUnitConvert
//  golden cUnitConvert.cpp:243-260
//
//  The orchestrator every caller actually names.  Runs the six converters in
//  golden's order, then applies the ATC hot-gun allowance to the OutArm
//  wait-on-shuttle time (gate PTW8-UC-4 for the ATC_TYPE_* constants; the
//  branch is offline-dead because the ATC shim's iATC_MODE_TYPE is fixed at 0).
// ---------------------------------------------------------------------------
void DoStructUnitConvert()
{
    DoTestIFConvert();
    DoDeviceConvert();
    DoHotPlateConvert();
    DoArmOffsetConvert();
    DoArmSpeedConvert();
    DoDefFormConvert();

    if(Temperature.bATCActiveCooling==true && LastSet.iTemperature==Tempture_Ambient &&
      (ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33 ||
       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35 ||
       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_61) &&
       Temperature.bActiveHeatGun && Temperature.dATC_HotGunTime!=0)
    {
        ArmSpeed[OutArm].dWaitOnSH=ArmSpeed[OutArm].dWaitOnSH+Temperature.dATC_HotGunTime;
    }
}
//---------------------------------------------------------------------------
