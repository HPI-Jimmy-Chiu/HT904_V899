using System.Collections.Generic;
using HT9045.HardwareAbstraction;

namespace HT9045.Hardware.Sim
{
    // Simulation adapter for ITesterInterface.
    // Mirrors GPIB/Tester bridge (main.h: bFind/RunTestProgram/SendMSG_CMD/
    // CloseGpibProgram, tTestResult).
    // All sites return "PASS" (bin 1) by default. Tests can override SimResults.
    public class SimTesterInterface : ITesterInterface
    {
        // Override per-site results before calling RunTestProgram in a test.
        public List<string> SimResults;

        // Records the last CMD sent via SendMSG_CMD for test assertion.
        public int LastCmd = -1;

        // Records the last CloseGpibProgram source for test assertion.
        public string LastCloseSrc = null;

        public SimTesterInterface()
        {
            SimResults = new List<string> { "PASS" };
        }

        public bool Find()
        {
            // Sim: tester is always present.
            return true;
        }

        public bool RunTestProgram(bool bNeedTest, bool[] bSiteOnOff)
        {
            // Sim: test always succeeds.
            return true;
        }

        public void SendMSG_CMD(int CMD)
        {
            LastCmd = CMD;
        }

        public void CloseGpibProgram(string Src)
        {
            LastCloseSrc = Src;
        }

        public IList<string> GetTesterResult()
        {
            return SimResults;
        }
    }
}
