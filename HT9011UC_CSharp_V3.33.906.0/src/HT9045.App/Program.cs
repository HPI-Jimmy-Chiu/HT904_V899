using System;
using HT9045.Core;

namespace HT9045.App
{
    // Console entry point for Phase-1 offline smoke test.
    // Usage:
    //   HT9045.App.exe [Sim|Real] [Stub|WinForms]
    //   HT9045_HW=Sim HT9045_UI=Stub HT9045.App.exe
    //
    // Default (no args): Sim + Stub.
    // Returns exit code 0 on success, 1 on failure.
    //
    // BCB6 analogue: WinMain / TApplication->Run() in HT9045.cpp.
    // In Phase 4 this file will be replaced by a WinExe host that calls
    // Application.Run(new TfMain()) — the composition root stays separate.
    internal static class Program
    {
        private static int Main(string[] args)
        {
            try
            {
                HwMode hw = CompositionRoot.ResolveHwMode(args);
                UiMode ui = CompositionRoot.ResolveUiMode(args);

                Console.WriteLine("HT9045 Phase-1 offline smoke");
                Console.WriteLine("  HW  = " + hw);
                Console.WriteLine("  UI  = " + ui);
                Console.WriteLine();

                HandlerOrchestrator orchestrator = CompositionRoot.Build(hw, ui);

                // Subscribe to events so the console shows the full trace.
                orchestrator.ConfigChanged  += OnConfigChanged;
                orchestrator.TaskCompleted  += OnTaskCompleted;
                orchestrator.AlarmOccured   += OnAlarmOccured;

                bool ok = orchestrator.RunOneCycle();

                Console.WriteLine();
                if (ok)
                {
                    Console.WriteLine("OFFLINE SMOKE OK");
                    return 0;
                }
                else
                {
                    Console.WriteLine("OFFLINE SMOKE FAILED");
                    return 1;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("EXCEPTION: " + ex.GetType().Name + ": " + ex.Message);
                Console.WriteLine(ex.StackTrace);
                return 1;
            }
        }

        private static void OnConfigChanged(object sender, ConfigChangedEventArgs e)
        {
            Console.WriteLine("[EVENT] ConfigChanged: " + e.CFileName);
        }

        private static void OnTaskCompleted(object sender, TaskCompletedEventArgs e)
        {
            Console.WriteLine("[EVENT] TaskCompleted: " + e.TaskName + " success=" + e.Success);
        }

        private static void OnAlarmOccured(object sender, AlarmOccuredEventArgs e)
        {
            Console.WriteLine("[EVENT] AlarmOccured: code=" + e.AlarmCode + " msg=" + e.Message);
        }
    }
}
