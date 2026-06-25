using System;
using HT9045.HardwareAbstraction;

namespace HT9045.Hardware.Sim
{
    // Simulation adapter for IMotorPort. Mirrors HTMotor (Motor/HTMotor.h).
    // Position is an integer counter; motion is instantaneous (MotionDone always
    // true after MoveToPos). All operations succeed unless SimFailOnNextMove is set.
    public class SimMotorPort : IMotorPort
    {
        private int _pos;
        private bool _homed;
        private bool _alarmFlag;
        private uint _speed;

        // Set to true in a test to simulate a motor alarm on the next Move.
        public bool SimFailOnNextMove;

        public int InitMotor(int IoAddress)
        {
            _pos = 0;
            _homed = false;
            _alarmFlag = false;
            return 0; // 0 = ok (matches HTMotor convention)
        }

        public bool MoveToPos(int Tar)
        {
            if (SimFailOnNextMove)
            {
                _alarmFlag = true;
                return false;
            }
            _pos = Tar;
            return true;
        }

        public int ReadPos()
        {
            return _pos;
        }

        public bool HomeObject()
        {
            _pos = 0;
            _homed = true;
            return true;
        }

        public bool HomeFlag()
        {
            return _homed;
        }

        public void Stop()
        {
            // No-op in simulation.
        }

        public bool MotionDone()
        {
            // Sim: motion is instantaneous.
            return true;
        }

        public bool GetAlarm()
        {
            return _alarmFlag;
        }

        public void SetSpeed(uint x, bool bSetJog)
        {
            _speed = x;
        }

        public void ServerOnOff(bool bStatus)
        {
            // No-op in simulation. (BCB6 spelling: ServerOnOff)
        }

        public void SetSoftLimit(int iPLimit, int iNLimit)
        {
            // No-op in simulation.
        }

        public bool JogP()
        {
            _pos += 10;
            return true;
        }

        public bool JogN()
        {
            _pos -= 10;
            return true;
        }

        public void ScanMotorStatus(bool[] Led)
        {
            // 10 LED flags (matches HTMotor::ScanMotorStatus(bool*Led)).
            if (Led == null) return;
            int len = Led.Length < 10 ? Led.Length : 10;
            for (int i = 0; i < len; i++)
                Led[i] = false;
        }
    }
}
