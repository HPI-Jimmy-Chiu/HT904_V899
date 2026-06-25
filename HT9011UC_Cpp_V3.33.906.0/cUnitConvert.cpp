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
