using System;
using System.Collections.Generic;
using HT9045.HardwareAbstraction;
using HT9045.Presentation.Abstraction;

namespace HT9045.Core
{
    // Representative offline handler cycle. Drives the ports + main view through
    // one license-gate / home / move / read-bit / run-test / report-bin pass.
    // With Sim adapters + a Stub view this runs headless, with zero hardware and
    // zero native DLL.
    public class HandlerOrchestrator : IHandlerOrchestrator
    {
        // KeyPro function index that gates auto-run (representative).
        private const uint KEYPRO_FUNC_AUTORUN = 0;

        // The InArm X axis index used for the representative home/move step.
        private const int AXIS_INARM_X = 0;

        private readonly IMotorPort _motor;
        private readonly IIoPort _io;
        private readonly IKeyProPort _keyPro;
        private readonly ITesterInterface _tester;
        private readonly IMainView _view;
        private readonly ProdInfo _prod;

        public event EventHandler<ConfigChangedEventArgs> ConfigChanged;
        public event EventHandler<TaskCompletedEventArgs> TaskCompleted;
        public event EventHandler<AlarmOccuredEventArgs> AlarmOccured;

        public HandlerOrchestrator(
            IMotorPort motor,
            IIoPort io,
            IKeyProPort keyPro,
            ITesterInterface tester,
            IMainView view)
        {
            if (motor == null) throw new ArgumentNullException("motor");
            if (io == null) throw new ArgumentNullException("io");
            if (keyPro == null) throw new ArgumentNullException("keyPro");
            if (tester == null) throw new ArgumentNullException("tester");
            if (view == null) throw new ArgumentNullException("view");

            _motor = motor;
            _io = io;
            _keyPro = keyPro;
            _tester = tester;
            _view = view;
            _prod = new ProdInfo();
            _prod.CFileName = "OFFLINE_SMOKE.setup";
        }

        public bool RunOneCycle()
        {
            _view.ShowStatus("Cycle start: " + _prod.CFileName);
            RaiseConfigChanged(_prod.CFileName);

            // 1) License gate (KEYPRO_GET_LEVEL). Level < 1 stops auto-run.
            int level = _keyPro.GetLevel(KEYPRO_FUNC_AUTORUN);
            if (level < 1)
            {
                RaiseAlarm(60001, "KeyPro level too low: " + level);
                RaiseTaskCompleted("RunOneCycle", false);
                return false;
            }
            _view.ShowStatus("KeyPro level OK: " + level);

            // 2) Home the InArm X axis.
            if (!_motor.HomeObject())
            {
                RaiseAlarm(55555, "InArm X HomeObject failed");
                RaiseTaskCompleted("RunOneCycle", false);
                return false;
            }
            if (!_motor.HomeFlag())
            {
                RaiseAlarm(55555, "InArm X HomeFlag not set after home");
                RaiseTaskCompleted("RunOneCycle", false);
                return false;
            }
            _view.UpdateMotorScreen(AXIS_INARM_X, _motor.ReadPos());

            // 3) Move to a pick position and wait for motion done.
            const int pickPos = 1000;
            _motor.SetSpeed(50000, false);
            if (!_motor.MoveToPos(pickPos))
            {
                RaiseAlarm(55555, "InArm X MoveToPos failed");
                RaiseTaskCompleted("RunOneCycle", false);
                return false;
            }
            while (!_motor.MotionDone())
            {
                if (_motor.GetAlarm())
                {
                    RaiseAlarm(55555, "InArm X motor alarm during move");
                    RaiseTaskCompleted("RunOneCycle", false);
                    return false;
                }
            }
            _view.UpdateMotorScreen(AXIS_INARM_X, _motor.ReadPos());
            _view.ShowStatus("InArm X at pick position");

            // 4) Read an input bit (e.g. vacuum-present sensor) via the IO port.
            //    Ring 0 / IP 0 / Port 0 / Bit 0, no ISA base, no alias.
            bool icPresent = _io.IOInputBit(0, 0, 0, 0, 0, "");
            _view.ShowStatus("IC-present sensor = " + icPresent);

            // 5) Run the test program for the enabled sites and read results.
            if (!_tester.Find())
            {
                RaiseAlarm(60002, "GPIB tester not found");
                RaiseTaskCompleted("RunOneCycle", false);
                return false;
            }
            bool[] siteOn = _prod.SiteOn;
            siteOn[0] = true; // at least one site live for the smoke cycle.
            if (!_tester.RunTestProgram(true, siteOn))
            {
                RaiseAlarm(60003, "RunTestProgram failed");
                RaiseTaskCompleted("RunOneCycle", false);
                return false;
            }
            IList<string> results = _tester.GetTesterResult();

            // 6) Report bin(s) through the view.
            int reported = 0;
            if (results != null)
            {
                for (int i = 0; i < results.Count; i++)
                {
                    _view.ShowStatus("Site " + i + " bin = " + results[i]);
                    reported++;
                }
            }
            _view.ShowStatus("Cycle done, bins reported: " + reported);

            RaiseTaskCompleted("RunOneCycle", true);
            return true;
        }

        private void RaiseConfigChanged(string cFileName)
        {
            EventHandler<ConfigChangedEventArgs> h = ConfigChanged;
            if (h != null) h(this, new ConfigChangedEventArgs(cFileName));
        }

        private void RaiseTaskCompleted(string taskName, bool success)
        {
            EventHandler<TaskCompletedEventArgs> h = TaskCompleted;
            if (h != null) h(this, new TaskCompletedEventArgs(taskName, success));
        }

        private void RaiseAlarm(int code, string message)
        {
            _view.ShowStatus("ALARM " + code + ": " + message);
            EventHandler<AlarmOccuredEventArgs> h = AlarmOccured;
            if (h != null) h(this, new AlarmOccuredEventArgs(code, message));
        }
    }
}
