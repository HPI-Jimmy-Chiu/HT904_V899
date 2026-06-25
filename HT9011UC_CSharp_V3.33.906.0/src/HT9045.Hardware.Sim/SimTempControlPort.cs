using HT9045.HardwareAbstraction;

namespace HT9045.Hardware.Sim
{
    // Simulation adapter for ITempControlPort. Mirrors TempCtrl/* controllers.
    // Temperature is always at setpoint (IsTempReached = true) in simulation.
    public class SimTempControlPort : ITempControlPort
    {
        private const int MAX_CHANNELS = 8;
        private int[] _setTemp = new int[MAX_CHANNELS];
        private bool[] _initialized = new bool[MAX_CHANNELS];

        public int InitTempControl(int iChannel)
        {
            if (iChannel < 0 || iChannel >= MAX_CHANNELS) return -1;
            _initialized[iChannel] = true;
            _setTemp[iChannel] = 250; // 25.0 degC in 0.1 degC units
            return 0;
        }

        public int SetTemp(int iChannel, int iTempTenthC)
        {
            if (iChannel < 0 || iChannel >= MAX_CHANNELS) return -1;
            _setTemp[iChannel] = iTempTenthC;
            return 0;
        }

        public int ReadTemp(int iChannel)
        {
            if (iChannel < 0 || iChannel >= MAX_CHANNELS) return -9999;
            // Sim: temperature always matches setpoint.
            return _setTemp[iChannel];
        }

        public bool IsTempReached(int iChannel)
        {
            if (iChannel < 0 || iChannel >= MAX_CHANNELS) return false;
            // Sim: always at target.
            return true;
        }
    }
}
