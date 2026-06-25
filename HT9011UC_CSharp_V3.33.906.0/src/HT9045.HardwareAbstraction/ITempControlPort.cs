namespace HT9045.HardwareAbstraction
{
    // Mirrors the BCB6 C++ temperature controller modules (TempCtrl/*: DT4848,
    // KT4H, TMC401, UT100, WT404). The concrete drivers differ per controller
    // brand but share this minimal read/set surface used by the soak logic.
    // Return convention follows the C++ 'ret' style: int < 0 = communication
    // / device fail.
    public interface ITempControlPort
    {
        // Open / initialise the controller channel. <0 = fail.
        int InitTempControl(int iChannel);

        // Set the target (soak) temperature, in 0.1 degC units per the GPIB
        // SETTEMP convention. <0 = fail.
        int SetTemp(int iChannel, int iTempTenthC);

        // Read the current process temperature, in 0.1 degC units.
        int ReadTemp(int iChannel);

        // True once the channel is within the soak band of its set point.
        bool IsTempReached(int iChannel);
    }
}
