namespace HT9045.HardwareAbstraction
{
    // Mirrors the Hontech KeyPro DLL (Public/HTKeyPro.h).
    // KEYPRO_GET_LEVEL(unsigned int iIndex) -> int : returns the licensed
    // function level for the given function index. Phase-1 Sim returns a fixed
    // level so the offline cycle can pass the license gate with no dongle.
    public interface IKeyProPort
    {
        // extern "C" int __stdcall KEYPRO_GET_LEVEL(unsigned int iIndex);
        int GetLevel(uint iIndex);
    }
}
