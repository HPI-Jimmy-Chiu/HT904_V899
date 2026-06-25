using HT9045.HardwareAbstraction;

namespace HT9045.Hardware.Sim
{
    // Simulation adapter for IKeyProPort. Mirrors KEYPRO_GET_LEVEL (Public/HTKeyPro.h).
    // Returns SimLevel for every index so the license gate always passes in sim mode.
    public class SimKeyProPort : IKeyProPort
    {
        // Default level high enough to pass the gate in HandlerOrchestrator (>= 1).
        public int SimLevel = 3;

        public int GetLevel(uint iIndex)
        {
            return SimLevel;
        }
    }
}
