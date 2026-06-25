using System;
using System.Collections.Generic;
using Xunit;
using HT9045.Core;
using HT9045.Hardware.Sim;
using HT9045.Views.Stub;

namespace HT9045.Tests
{
    // Constructs HandlerOrchestrator with Sim adapters + StubMainView and exercises
    // a full offline lot cycle. Zero hardware, zero native DLL.
    // Mirrors the "RunOneCycle" flow in HandlerOrchestrator:
    //   KeyPro gate -> HomeObject/HomeFlag -> SetSpeed/MoveToPos/MotionDone
    //   -> IOInputBit -> tester.Find/RunTestProgram/GetTesterResult -> view.ShowStatus.
    public class OfflineSmokeTests
    {
        // Factory: builds a default (all-passing) orchestrator and returns the
        // stub view so tests can make assertions on recorded output.
        private static HandlerOrchestrator BuildOrchestrator(
            out StubMainView view,
            out SimMotorPort motor,
            out SimTesterInterface tester)
        {
            motor = new SimMotorPort();
            var io = new SimIoPort();
            var keyPro = new SimKeyProPort();
            tester = new SimTesterInterface();
            view = new StubMainView();
            return new HandlerOrchestrator(motor, io, keyPro, tester, view);
        }

        [Fact]
        public void RunOneCycle_WithSimAdapters_ReturnsTrue()
        {
            StubMainView view;
            SimMotorPort motor;
            SimTesterInterface tester;
            HandlerOrchestrator orch = BuildOrchestrator(out view, out motor, out tester);

            bool result = orch.RunOneCycle();

            Assert.True(result, "RunOneCycle should return true with all-passing Sim adapters");
        }

        [Fact]
        public void RunOneCycle_StubViewRecordsPass()
        {
            StubMainView view;
            SimMotorPort motor;
            SimTesterInterface tester;
            HandlerOrchestrator orch = BuildOrchestrator(out view, out motor, out tester);

            orch.RunOneCycle();

            // The orchestrator calls view.ShowStatus("Cycle done, bins reported: N")
            // as its final step before raising TaskCompleted.
            Assert.True(
                view.StatusContains("Cycle done"),
                "StubMainView should have recorded a 'Cycle done' status message. " +
                "Log: " + string.Join("; ", view.StatusLog));
        }

        [Fact]
        public void RunOneCycle_MotorMovedToPickPosition()
        {
            StubMainView view;
            SimMotorPort motor;
            SimTesterInterface tester;
            HandlerOrchestrator orch = BuildOrchestrator(out view, out motor, out tester);

            orch.RunOneCycle();

            // After a successful cycle the motor's last position is the pick position (1000).
            Assert.Equal(1000, motor.ReadPos());
        }

        [Fact]
        public void RunOneCycle_RaisesTaskCompletedSuccess()
        {
            StubMainView view;
            SimMotorPort motor;
            SimTesterInterface tester;
            HandlerOrchestrator orch = BuildOrchestrator(out view, out motor, out tester);

            bool eventFired = false;
            bool eventSuccess = false;
            orch.TaskCompleted += (sender, e) =>
            {
                if (e.TaskName == "RunOneCycle")
                {
                    eventFired = true;
                    eventSuccess = e.Success;
                }
            };

            orch.RunOneCycle();

            Assert.True(eventFired, "TaskCompleted event should have fired");
            Assert.True(eventSuccess, "TaskCompleted.Success should be true");
        }

        [Fact]
        public void RunOneCycle_RaisesConfigChangedWithFileName()
        {
            StubMainView view;
            SimMotorPort motor;
            SimTesterInterface tester;
            HandlerOrchestrator orch = BuildOrchestrator(out view, out motor, out tester);

            string capturedFile = null;
            orch.ConfigChanged += (sender, e) => { capturedFile = e.CFileName; };

            orch.RunOneCycle();

            Assert.NotNull(capturedFile);
            Assert.False(string.IsNullOrEmpty(capturedFile));
        }

        [Fact]
        public void RunOneCycle_KeyProLevelTooLow_ReturnsFalse()
        {
            SimMotorPort motor = new SimMotorPort();
            var io = new SimIoPort();
            var keyPro = new SimKeyProPort();
            keyPro.SimLevel = 0; // Below the gate threshold.
            var tester = new SimTesterInterface();
            var view = new StubMainView();
            HandlerOrchestrator orch = new HandlerOrchestrator(motor, io, keyPro, tester, view);

            bool result = orch.RunOneCycle();

            Assert.False(result, "Cycle must fail when KeyPro level is below threshold");
        }

        [Fact]
        public void RunOneCycle_KeyProLevelTooLow_RaisesAlarm()
        {
            SimMotorPort motor = new SimMotorPort();
            var io = new SimIoPort();
            var keyPro = new SimKeyProPort();
            keyPro.SimLevel = 0;
            var tester = new SimTesterInterface();
            var view = new StubMainView();
            HandlerOrchestrator orch = new HandlerOrchestrator(motor, io, keyPro, tester, view);

            bool alarmFired = false;
            orch.AlarmOccured += (sender, e) => { alarmFired = true; };

            orch.RunOneCycle();

            Assert.True(alarmFired, "AlarmOccured event should fire when KeyPro gate fails");
        }

        [Fact]
        public void RunOneCycle_TesterBinReportedToView()
        {
            SimMotorPort motor = new SimMotorPort();
            var io = new SimIoPort();
            var keyPro = new SimKeyProPort();
            var tester = new SimTesterInterface();
            // Inject a known bin result so we can assert it appears in the view log.
            tester.SimResults = new System.Collections.Generic.List<string> { "PASS", "FAIL_OPEN" };
            var view = new StubMainView();
            HandlerOrchestrator orch = new HandlerOrchestrator(motor, io, keyPro, tester, view);

            orch.RunOneCycle();

            Assert.True(
                view.StatusContains("PASS"),
                "View log should contain the PASS bin result");
            Assert.True(
                view.StatusContains("FAIL_OPEN"),
                "View log should contain the FAIL_OPEN bin result");
        }

        [Fact]
        public void RunOneCycle_MotorUpdateSentToView()
        {
            StubMainView view;
            SimMotorPort motor;
            SimTesterInterface tester;
            HandlerOrchestrator orch = BuildOrchestrator(out view, out motor, out tester);

            orch.RunOneCycle();

            // HandlerOrchestrator calls UpdateMotorScreen at least once (after home and after move).
            Assert.True(
                view.MotorLog.Count >= 1,
                "UpdateMotorScreen should have been called at least once during a cycle");
        }

        [Fact]
        public void Constructor_NullMotor_ThrowsArgumentNullException()
        {
            var io = new SimIoPort();
            var keyPro = new SimKeyProPort();
            var tester = new SimTesterInterface();
            var view = new StubMainView();
            Assert.Throws<ArgumentNullException>(() =>
                new HandlerOrchestrator(null, io, keyPro, tester, view));
        }

        [Fact]
        public void Constructor_NullView_ThrowsArgumentNullException()
        {
            var motor = new SimMotorPort();
            var io = new SimIoPort();
            var keyPro = new SimKeyProPort();
            var tester = new SimTesterInterface();
            Assert.Throws<ArgumentNullException>(() =>
                new HandlerOrchestrator(motor, io, keyPro, tester, null));
        }
    }
}
