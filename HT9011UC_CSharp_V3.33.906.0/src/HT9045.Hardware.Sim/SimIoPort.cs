using System.Collections.Generic;
using HT9045.HardwareAbstraction;

namespace HT9045.Hardware.Sim
{
    // Simulation adapter for IIoPort. Mirrors TLaneIO (MyLaneIo.h).
    // Bit state is stored in a dictionary keyed by (Ring,IP,Port,Bit).
    // Output bytes and input bytes are stored per (Ring,IP,Port).
    // All inputs default to false/0 until driven by a test.
    public class SimIoPort : IIoPort
    {
        private readonly Dictionary<string, bool> _bits = new Dictionary<string, bool>();
        private readonly Dictionary<string, byte> _outBytes = new Dictionary<string, byte>();

        // Allow a test to pre-seed an input bit (e.g. vacuum-present sensor = true).
        public void SetInputBit(int Ring, int IP, int Port, int Bit, bool value)
        {
            _bits[BitKey(Ring, IP, Port, Bit)] = value;
        }

        public void IOBitOn(int Ring, int IP, int Port, int Bit, int iISABase, string Alias)
        {
            _bits[BitKey(Ring, IP, Port, Bit)] = true;
        }

        public void IOBitOff(int Ring, int IP, int Port, int Bit, int iISABase, string Alias)
        {
            _bits[BitKey(Ring, IP, Port, Bit)] = false;
        }

        public bool IOByteOut(int Ring, int IP, int Port, uint Byte, int iISABase)
        {
            _outBytes[PortKey(Ring, IP, Port)] = (byte)(Byte & 0xFF);
            return true;
        }

        public bool IOOutBitStatus(int Ring, int IP, int Port, int Bit, int iISABase, string Alias)
        {
            bool v;
            _bits.TryGetValue(BitKey(Ring, IP, Port, Bit), out v);
            return v;
        }

        public bool IOInputBit(int Ring, int IP, int Port, int Bit, int iISABase, string Alias)
        {
            bool v;
            _bits.TryGetValue(BitKey(Ring, IP, Port, Bit), out v);
            return v;
        }

        public byte IOInputByte(int Ring, int IP, int Port, int iISABase)
        {
            byte v;
            _outBytes.TryGetValue(PortKey(Ring, IP, Port), out v);
            return v;
        }

        private static string BitKey(int ring, int ip, int port, int bit)
        {
            return ring + ":" + ip + ":" + port + ":" + bit;
        }

        private static string PortKey(int ring, int ip, int port)
        {
            return ring + ":" + ip + ":" + port;
        }
    }
}
