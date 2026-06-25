HT9045.Hardware.Native / Interop
================================
Phase 3 target: place all [DllImport] P/Invoke declarations and
marshalling helpers in this folder.

ONE FILE PER NATIVE DLL:
  HwInterop.Csmc.cs        -- CSMC.lib / CSmc.h (servo motor commands)
  HwInterop.Dmc32.cs       -- DMC32.dll / DMCCOM.H, DMCMLIB.H (Galil controller)
                              HANDLEDMC -> System.IntPtr
  HwInterop.Mn200.cs       -- MN200DLL.dll / mn200.h (Mitsubishi)
  HwInterop.KeyPro.cs      -- KeyProDLL.dll / HTKeyPro.h
                              char* + length marshalling (LPStr or manual byte[])
  HwInterop.LaneIo.cs      -- Lane IO ring/IP/Port native layer
  HwInterop.TempCtrl.cs    -- RS-232 helpers for DT4848/KT4H/TMC401/UT100/WT404

MARSHALLING NOTES:
  SPEED_PAR struct (HTMotor / CSmcdef.h):
    Use [StructLayout(LayoutKind.Sequential)] with explicit field types.
    Typical fields: uint SpeedHigh, SpeedLow, Acc, Dec (confirm against CSmc.h).

  MV / VM window handles (GPIB bridge):
    BCB6 HWND -> System.IntPtr (platform-size safe on both x86 and x64).
    Fixed-width fields in shared structs use explicit byte counts, not int/long.

  KeyPro char* output:
    Allocate fixed byte[] buffer, pin with GCHandle or use unsafe fixed block,
    pass as IntPtr; convert result with Encoding.Default (Big5 code page 950).

  DMC32 string commands:
    HANDLEDSMC (void*) -> IntPtr; command strings as ANSI (CharSet.Ansi).

PHASE 3 CHECKLIST:
  [ ] Map each BCB6 .lib/.dll to its [DllImport] entry point names
  [ ] Confirm calling convention (StdCall vs Cdecl) per vendor header
  [ ] Add struct StructLayout for all value-type parameter blocks
  [ ] Write thin safe wrappers that translate error codes to bool/int
      (mirrors BCB6 return-value convention: 0 or positive = ok, negative = fail)
  [ ] Update NativeMotor / NativeIoBackend / etc. to call these wrappers
      instead of throwing NotImplementedException
