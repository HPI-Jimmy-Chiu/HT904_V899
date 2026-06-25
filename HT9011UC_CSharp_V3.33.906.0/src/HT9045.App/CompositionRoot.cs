using System;
using HT9045.Core;
using HT9045.HardwareAbstraction;
using HT9045.Presentation.Abstraction;
using HT9045.Hardware.Sim;
using HT9045.Hardware.Native;
using HT9045.Views.Stub;

namespace HT9045.App
{
    // Composition root: reads two config flags and wires the dependency graph.
    //
    //   HW flag  (args[0] or env HT9045_HW):  "Sim"  | "Real"     default = "Sim"
    //   UI flag  (args[1] or env HT9045_UI):  "Stub" | "WinForms"  default = "Stub"
    //
    // Phase 1: Sim+Stub is fully headless. Real+WinForms paths are wired but
    // NativeXxx adapters throw NotImplementedException -- correct: Phase 3 fills them.
    // WinForms view is not yet implemented; selecting it in Phase 1 throws.
    //
    // Mirrors BCB6 startup sequence in cKernel / TfMain (app entry + DI by hand).
    public static class CompositionRoot
    {
        public const string HW_ENV  = "HT9045_HW";
        public const string UI_ENV  = "HT9045_UI";

        // Resolve config flags from command-line args, then env, then defaults.
        public static HwMode ResolveHwMode(string[] args)
        {
            string raw = args != null && args.Length >= 1 ? args[0] : null;
            if (string.IsNullOrEmpty(raw))
                raw = Environment.GetEnvironmentVariable(HW_ENV);
            if (string.IsNullOrEmpty(raw))
                return HwMode.Sim;
            return string.Compare(raw, "Real", StringComparison.OrdinalIgnoreCase) == 0
                ? HwMode.Real
                : HwMode.Sim;
        }

        public static UiMode ResolveUiMode(string[] args)
        {
            string raw = args != null && args.Length >= 2 ? args[1] : null;
            if (string.IsNullOrEmpty(raw))
                raw = Environment.GetEnvironmentVariable(UI_ENV);
            if (string.IsNullOrEmpty(raw))
                return UiMode.Stub;
            return string.Compare(raw, "WinForms", StringComparison.OrdinalIgnoreCase) == 0
                ? UiMode.WinForms
                : UiMode.Stub;
        }

        // Build and return a fully-wired HandlerOrchestrator.
        // Throws for unsupported combinations (e.g. WinForms before Phase 4).
        public static HandlerOrchestrator Build(HwMode hw, UiMode ui)
        {
            IMotorPort    motor  = BuildMotorPort(hw);
            IIoPort       io     = BuildIoPort(hw);
            IKeyProPort   keyPro = BuildKeyProPort(hw);
            ITesterInterface tester = BuildTesterInterface(hw);
            IMainView     view   = BuildMainView(ui);

            return new HandlerOrchestrator(motor, io, keyPro, tester, view);
        }

        // ------------------------------------------------------------------ adapters

        private static IMotorPort BuildMotorPort(HwMode hw)
        {
            if (hw == HwMode.Sim)
                return new SimMotorPort();
            // Real: Phase-3 wiring. NativeMotor axis 0 = InArm X (AXIS_INARM_X).
            return new NativeMotor(0);
        }

        private static IIoPort BuildIoPort(HwMode hw)
        {
            if (hw == HwMode.Sim)
                return new SimIoPort();
            // Real: Phase-3 wiring.
            throw new NotImplementedException(
                "Real IIoPort adapter not yet wired. Set HT9045_HW=Sim for Phase-1.");
        }

        private static IKeyProPort BuildKeyProPort(HwMode hw)
        {
            if (hw == HwMode.Sim)
                return new SimKeyProPort();
            // Real: Phase-3 wiring.
            return new NativeKeyPro();
        }

        private static ITesterInterface BuildTesterInterface(HwMode hw)
        {
            if (hw == HwMode.Sim)
                return new SimTesterInterface();
            // Real: Phase-3 wiring.
            return new NativeTester();
        }

        private static IMainView BuildMainView(UiMode ui)
        {
            if (ui == UiMode.Stub)
                return new StubMainView();
            // WinForms: Phase-4 wiring. The real WinForms form lives in HT9045.Views.WinForms
            // which does not exist yet. Guard with a clear message.
            throw new NotImplementedException(
                "WinForms view not yet implemented. Set HT9045_UI=Stub for Phase-1.");
        }
    }

    public enum HwMode  { Sim, Real }
    public enum UiMode  { Stub, WinForms }
}
