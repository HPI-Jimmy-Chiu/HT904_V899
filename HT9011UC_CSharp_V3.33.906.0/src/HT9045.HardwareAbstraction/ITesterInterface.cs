using System.Collections.Generic;

namespace HT9045.HardwareAbstraction
{
    // Mirrors the BCB6 C++ GPIB / Tester bridge entry points (main.h, InterfaceSYS.h).
    //   bFind / RunTestProgram(...) / SendMSG_CMD(int) / CloseGpibProgram(AnsiString)
    // GetTesterResult exposes the per-site test result list that the C++ side
    // keeps in TStringList *tTestResult.
    public interface ITesterInterface
    {
        // bool bFind;  // C++ keeps a flag; Find() drives the GPIB discovery action.
        bool Find();

        // bool RunTestProgram(bool bNeedTest, bool *bSiteOnOff = NULL);
        bool RunTestProgram(bool bNeedTest, bool[] bSiteOnOff);

        // void SendMSG_CMD(int CMD);
        void SendMSG_CMD(int CMD);

        // void CloseGpibProgram(AnsiString Src = "");
        void CloseGpibProgram(string Src);

        // Reads back per-site results (C++ TStringList *tTestResult).
        IList<string> GetTesterResult();
    }
}
