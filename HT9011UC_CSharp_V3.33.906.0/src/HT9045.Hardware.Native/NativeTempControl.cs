// HT9045.Hardware.Native / NativeTempControl.cs
// Phase-3 placeholder: real implementation drives DT4848/KT4H/TMC401/UT100/WT404 controllers.
// BCB6 mirror: TempCtrl/* (various temp controller classes)
using System;
using HT9045.HardwareAbstraction;

namespace HT9045.Hardware.Native
{
    /// <summary>
    /// Placeholder real temperature-control adapter.
    /// Maps to BCB6 TempCtrl/* (DT4848, KT4H, TMC401, UT100, WT404).
    /// Channel mapping and serial/RS-232 marshalling go in /Interop.
    /// Return convention: int &lt; 0 means failure (mirrors BCB6 error code practice).
    /// </summary>
    public class NativeTempControl : ITempControlPort
    {
        public int InitTempControl(int iChannel)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public int SetTemp(int iChannel, int iTempTenthC)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public int ReadTemp(int iChannel)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public bool IsTempReached(int iChannel)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }
    }
}
