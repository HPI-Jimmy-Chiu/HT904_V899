// ===========================================================================
//  cUnitConvert.h
//  Standard C++ translation of BCB6 cUnitConvert.h (V3.33.906.0_20260618).
//
//  BCB6 source: HT9011UC_Code_V3.33.906.0_20260618/cUnitConvert.h (lines 1-10)
//
//  Translation scope (W2 partial extract):
//    TRANSLATED:   iUnitMultiply100, iUnitMultiply1000
//    SKIPPED/TODO: DoTestIFConvert, DoDeviceConvert (DeviceFormConvert),
//                  DoHotPlateConvert, DoArmOffsetConvert, DoArmSpeedConvert,
//                  DoDefFormConvert, DoStructUnitConvert
//                  -> all depend on untranslated globals from cprod.h / cmydef.h
//                     (TestIF/TestIF_File, DeviceForm/DeviceForm_File,
//                      HotPlateForm/HotPlateForm_File, InArmOffSet[],
//                      OutArmOffSet[], SortArmOffSet[], Offset/Offset_File,
//                      ArmSpeed/ArmSpeed_File, SHSpeed/MGSpeed, UserDefForm,
//                      IniConfig, InArmSuck, OutArm2Suck, Temperature, LastSet)
//                     and on VCL form pointers (fShowMessage, ATC_InterfaceForm).
//                  Revisit in W6 (cprod/cmydef de-VCL'd) and W7 (VCL forms).
//
//  Toolchain: MinGW g++ 6.3+, C++17, CMake 4.0.2 (MinGW Makefiles).
// ===========================================================================
#ifndef C_UNIT_CONVERT_H
#define C_UNIT_CONVERT_H

// ---------------------------------------------------------------------------
// iUnitMultiply100
//   Convert a millimetre (mm) config value to the internal integer unit
//   (0.01 mm = 1 count) using Delphi FormatFloat normalisation.
//
//   BCB6 source: cUnitConvert.cpp lines 13-20.
//
//   Algorithm:
//     1. Multiply by 100.0.
//     2. Format to 2 decimal places with FormatFloat("0.00", ...) -- this
//        prints e.g. "234.50" -- which normalises binary floating-point
//        noise that would otherwise cause floor-by-cast to undercount by 1
//        (e.g. 2.9999... becoming 2 instead of 3).
//     3. atoi() on the resulting string truncates the ".DD" fractional part,
//        yielding the rounded integer.
//
//   The vclcompat FormatFloat("0.00", v) delegates to snprintf("%.2f", v)
//   which provides the same normalisation and half-round behaviour.
//
//   Examples (match BCB6 at standard double precision):
//     iUnitMultiply100(2.344) -> FormatFloat("0.00", 234.4) -> "234.40" -> 234
//     iUnitMultiply100(2.345) -> FormatFloat("0.00", 234.5) -> "234.50" -> 234
//     iUnitMultiply100(0.030) -> FormatFloat("0.00", 3.0)   -> "3.00"   ->   3
//       (guards against 2.9999... binary artifact)
// ---------------------------------------------------------------------------
int iUnitMultiply100(double Data);

// ---------------------------------------------------------------------------
// iUnitMultiply1000
//   Convert a config value to 0.001-unit integers by direct double-to-int
//   truncation (no FormatFloat normalisation).
//
//   BCB6 source: cUnitConvert.cpp lines 22-25.
//   Formula: return (int)(Data * 1000.0)  (implicit double->int truncation)
//
//   Example: iUnitMultiply1000(1.234) -> 1234
// ---------------------------------------------------------------------------
int iUnitMultiply1000(double Data);

// ---------------------------------------------------------------------------
// TODO(W6): DoTestIFConvert -- copies TestIF_File -> TestIF and scales pitches.
//   Deps: TestIF, TestIF_File (cprod.h global structs); IniConfig (cmydef.h).
//   BCB6 source: cUnitConvert.cpp lines 27-56.
// ---------------------------------------------------------------------------

// TODO(W6): DoDeviceConvert (named DoDeviceFormConvert in .h) -- copies
//   DeviceForm_File -> DeviceForm and scales arm/index/dimension fields.
//   Deps: DeviceForm, DeviceForm_File (cprod.h).
//   BCB6 source: cUnitConvert.cpp lines 58-71.
// ---------------------------------------------------------------------------

// TODO(W6): DoHotPlateConvert -- copies HotPlateForm_File -> HotPlateForm.
//   Deps: HotPlateForm, HotPlateForm_File (cprod.h), TRAY_TYPE_PARA (cmydef.h).
//   BCB6 source: cUnitConvert.cpp lines 73-80.
// ---------------------------------------------------------------------------

// TODO(W6): DoArmOffsetConvert -- scales InArmOffSet[], OutArmOffSet[],
//   SortArmOffSet[] and Offset/Offset_File fields.
//   Deps: InArmOffSet[], OutArmOffSet[], SortArmOffSet[], InOfsTotal,
//         OutOfsTotal, SortOfsTotal, Offset, Offset_File (cprod.h/cmydef.h);
//         InArmSuck, OutArm2Suck (MyKitSuck.h globals);
//         USE_OUT_SORT_ARM (MachineType.h/MachineDefine.h).
//   BCB6 source: cUnitConvert.cpp lines 82-190.
// ---------------------------------------------------------------------------

// TODO(W6+W7): DoArmSpeedConvert -- copies ArmSpeed_File/SHSpeed_File/MGSpeed_File
//   and calls fShowMessage->ShowSpeed (VCL form).
//   Deps: ArmSpeed, SHSpeed, MGSpeed and _File variants (cprod.h/cmydef.h);
//         fShowMessage (W7 VCL form pointer); IniConfig.bG05ShowSpeedMessage.
//   BCB6 source: cUnitConvert.cpp lines 192-205.
// ---------------------------------------------------------------------------

// TODO(W6): DoDefFormConvert -- copies UserDefForm_File -> UserDefForm, scales
//   pitch/start/depth, clamps iPickUp, handles Cassette Z fields.
//   Deps: UserDefForm, UserDefForm_File (cprod.h/cmydef.h);
//         IniConfig.bC03UseCatchTray; USE_LdUldCassetteMode (MachineType.h).
//   BCB6 source: cUnitConvert.cpp lines 207-241.
// ---------------------------------------------------------------------------

// TODO(W6+W7): DoStructUnitConvert -- orchestrates all Do*Convert calls plus
//   ATC HotGun time adjustment via ATC_InterfaceForm->iATC_MODE_TYPE (W7 form).
//   Deps: all of the above + Temperature/LastSet (cmydef.h);
//         ATC_InterfaceForm (W7 VCL form pointer).
//   BCB6 source: cUnitConvert.cpp lines 243-260.
// ---------------------------------------------------------------------------

#endif // C_UNIT_CONVERT_H
