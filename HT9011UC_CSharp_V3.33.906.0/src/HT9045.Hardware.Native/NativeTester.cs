// HT9045.Hardware.Native / NativeTester.cs
// Phase-3 placeholder: real implementation drives the GPIB/TCP tester bridge.
// BCB6 mirror: bFind / RunTestProgram / SendMSG_CMD / CloseGpibProgram / tTestResult (main.h)
using System;
using System.Collections.Generic;
using HT9045.HardwareAbstraction;

namespace HT9045.Hardware.Native
{
    /// <summary>
    /// Placeholder real tester adapter.
    /// Maps to BCB6 GPIB bridge interface (main.h: bFind, RunTestProgram, SendMSG_CMD,
    /// CloseGpibProgram, TStringList *tTestResult).
    /// HANDLEDMC->IntPtr mapping and MV/VM HWND fixed-width interop go in /Interop.
    /// </summary>
    public class NativeTester : ITesterInterface
    {
        public bool Find()
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public bool RunTestProgram(bool bNeedTest, bool[] bSiteOnOff)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public void SendMSG_CMD(int CMD)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public void CloseGpibProgram(string Src)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public IList<string> GetTesterResult()
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }
    }
}
