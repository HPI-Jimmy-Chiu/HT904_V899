using System;
using System.Collections.Generic;
using HT9045.Presentation.Abstraction;

namespace HT9045.Views.Stub
{
    // Headless implementation of IMainView for Phase-1 offline smoke runs.
    // Every call is recorded in public lists so tests can assert on them
    // without starting any GUI subsystem. All output is also echoed to
    // Console so 'dotnet run' gives a readable progress trace.
    //
    // BCB6 analogue: fMain (cMain.dfm / cMain.cpp) — the primary operator screen.
    // Phase-4 will replace this with a real WinForms form; the DI root switches.
    public class StubMainView : IMainView
    {
        // Accumulated ShowStatus messages in call order.
        // Tests read this to assert the orchestration trace.
        public readonly List<string> StatusLog = new List<string>();

        // Accumulated UpdateMotorScreen calls as (axis, pos) pairs.
        // Index mirrors order of calls; useful for asserting motor move sequences.
        public readonly List<MotorScreenEntry> MotorLog = new List<MotorScreenEntry>();

        // True after Show(), false after Close(). Tests can assert lifecycle.
        public bool IsVisible { get; private set; }

        // ------------------------------------------------------------------ IView

        public void Show()
        {
            IsVisible = true;
            Console.WriteLine("[StubMainView] Show()");
        }

        public void Close()
        {
            IsVisible = false;
            Console.WriteLine("[StubMainView] Close()");
        }

        // ------------------------------------------------------------------ IMainView

        // Mirrors BCB6 fMain status-bar update (Label1->Caption or similar).
        // msg is plain text; no format imposed here so callers control content.
        public void ShowStatus(string msg)
        {
            StatusLog.Add(msg);
            Console.WriteLine("[STATUS] " + msg);
        }

        // Mirrors BCB6 per-axis position readout on the motor monitor panel.
        // axis = zero-based axis index (0 = InArm X, matches AXIS_INARM_X in Core).
        // pos  = encoder counts, same unit as IMotorPort.ReadPos().
        public void UpdateMotorScreen(int axis, int pos)
        {
            MotorLog.Add(new MotorScreenEntry(axis, pos));
            Console.WriteLine("[MOTOR] axis=" + axis + "  pos=" + pos);
        }

        // ------------------------------------------------------------------ helpers

        // Returns true if any entry in StatusLog contains the given substring.
        // Convenience for test asserts: view.StatusContains("Cycle done") == true.
        public bool StatusContains(string substring)
        {
            if (substring == null) return false;
            for (int i = 0; i < StatusLog.Count; i++)
            {
                if (StatusLog[i] != null &&
                    StatusLog[i].IndexOf(substring, StringComparison.OrdinalIgnoreCase) >= 0)
                {
                    return true;
                }
            }
            return false;
        }

        // Clears both logs and resets IsVisible.
        // Useful when re-using one instance across multiple test cycles.
        public void Reset()
        {
            StatusLog.Clear();
            MotorLog.Clear();
            IsVisible = false;
        }
    }

    // Value type for one UpdateMotorScreen call.
    // Kept as a plain class (no struct / record — LangVersion 7.3, pre-modern idioms).
    public sealed class MotorScreenEntry
    {
        public readonly int Axis;
        public readonly int Pos;

        public MotorScreenEntry(int axis, int pos)
        {
            Axis = axis;
            Pos  = pos;
        }

        public override string ToString()
        {
            return "axis=" + Axis + " pos=" + Pos;
        }
    }
}
