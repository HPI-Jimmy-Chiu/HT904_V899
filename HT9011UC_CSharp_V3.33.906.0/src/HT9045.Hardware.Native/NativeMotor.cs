// HT9045.Hardware.Native / NativeMotor.cs
// Phase-3 placeholder: real implementation connects via HwInterop (CSMC / DMC32 / MN200 DLLs).
// All methods throw NotImplementedException until Phase 3 wiring is complete.
// BCB6 mirror: HTMotor (Motor/HTMotor.h)
using System;
using HT9045.HardwareAbstraction;

namespace HT9045.Hardware.Native
{
    /// <summary>
    /// Placeholder real motor adapter.
    /// Maps to BCB6 HTMotor class (Motor/HTMotor.h).
    /// [DllImport] declarations and SPEED_PAR StructLayout live in /Interop.
    /// </summary>
    public class NativeMotor : IMotorPort
    {
        // Axis index passed at construction (mirrors BCB6 per-axis HTMotor instance).
        private readonly int _axisIndex;

        public NativeMotor(int axisIndex)
        {
            _axisIndex = axisIndex;
        }

        public int InitMotor(int IoAddress)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public bool MoveToPos(int Tar)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public int ReadPos()
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public bool HomeObject()
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public bool HomeFlag()
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public void Stop()
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public bool MotionDone()
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public bool GetAlarm()
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public void SetSpeed(uint x, bool bSetJog)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public void ServerOnOff(bool bStatus)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public void SetSoftLimit(int iPLimit, int iNLimit)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public bool JogP()
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public bool JogN()
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public void ScanMotorStatus(bool[] Led)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }
    }
}
