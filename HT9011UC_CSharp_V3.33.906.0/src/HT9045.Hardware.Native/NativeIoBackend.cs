// HT9045.Hardware.Native / NativeIoBackend.cs
// Phase-3 placeholder: real implementation connects via HwInterop (TLaneIO/ring IP/Port layer).
// BCB6 mirror: TLaneIO (MyLaneIo.h)
using System;
using HT9045.HardwareAbstraction;

namespace HT9045.Hardware.Native
{
    /// <summary>
    /// Placeholder real IO adapter.
    /// Maps to BCB6 TLaneIO class (MyLaneIo.h).
    /// Ring/IP/Port/iISABase topology and actual DLL calls go in /Interop.
    /// </summary>
    public class NativeIoBackend : IIoPort
    {
        public void IOBitOn(int Ring, int IP, int Port, int Bit, int iISABase, string Alias)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public void IOBitOff(int Ring, int IP, int Port, int Bit, int iISABase, string Alias)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public bool IOByteOut(int Ring, int IP, int Port, uint Byte, int iISABase)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public bool IOOutBitStatus(int Ring, int IP, int Port, int Bit, int iISABase, string Alias)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public bool IOInputBit(int Ring, int IP, int Port, int Bit, int iISABase, string Alias)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }

        public byte IOInputByte(int Ring, int IP, int Port, int iISABase)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }
    }
}
