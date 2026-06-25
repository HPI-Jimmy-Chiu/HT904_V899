// HT9045.Hardware.Native / NativeKeyPro.cs
// Phase-3 placeholder: real implementation calls KEYPRO_GET_LEVEL via KeyProDLL.dll.
// BCB6 mirror: KEYPRO_GET_LEVEL (Public/HTKeyPro.h)
using System;
using HT9045.HardwareAbstraction;

namespace HT9045.Hardware.Native
{
    /// <summary>
    /// Placeholder real license-key adapter.
    /// Maps to BCB6 KEYPRO_GET_LEVEL (Public/HTKeyPro.h).
    /// [DllImport] for KeyProDLL.dll + char* / len marshalling goes in /Interop.
    /// </summary>
    public class NativeKeyPro : IKeyProPort
    {
        public int GetLevel(uint iIndex)
        {
            throw new NotImplementedException("Connect real hardware in Phase 3 via HwInterop");
        }
    }
}
