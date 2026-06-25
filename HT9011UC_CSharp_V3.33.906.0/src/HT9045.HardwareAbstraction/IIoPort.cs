using System;

namespace HT9045.HardwareAbstraction
{
    // Mirrors the BCB6 C++ TLaneIO class (MyLaneIo.h).
    // Multi Ring / IP / Port / Bit addressing with optional ISA base + alias.
    // The C++ Alias parameter defaults to "" and iISABase defaults to 0; kept
    // explicit here so adapter signatures match across Sim/Real implementations.
    public interface IIoPort
    {
        // void IOBitOn(int Ring, int IP, int Port, int Bit, int iISABase = 0, AnsiString Alias = "");
        void IOBitOn(int Ring, int IP, int Port, int Bit, int iISABase, string Alias);

        // void IOBitOff(int Ring, int IP, int Port, int Bit, int iISABase = 0, AnsiString Alias = "");
        void IOBitOff(int Ring, int IP, int Port, int Bit, int iISABase, string Alias);

        // bool IOByteOut(int Ring, int IP, int Port, unsigned int Byte, int iISABase = 0);
        bool IOByteOut(int Ring, int IP, int Port, uint Byte, int iISABase);

        // bool IOOutBitStatus(int Ring, int IP, int Port, int Bit, int iISABase = 0, AnsiString Alias = "");
        bool IOOutBitStatus(int Ring, int IP, int Port, int Bit, int iISABase, string Alias);

        // bool IOInputBit(int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias = "");
        bool IOInputBit(int Ring, int IP, int Port, int Bit, int iISABase, string Alias);

        // byte IOInputByte(int Ring, int IP, int Port, int iISABase = 0);
        byte IOInputByte(int Ring, int IP, int Port, int iISABase);
    }
}
