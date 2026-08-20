// =============================================================================
//  tools/pci1203_linkprobe.cpp -- does the Advantech SDK actually LINK?
//
//  AI(W906-1203HAL-3) 20260820: new file. 1203HAL-1 proved the six
//  vendor-consumer TUs COMPILE under -DHAVE_PCI1203=1; the SDK audit
//  (docs/RECON_1203_SDK_linkability.md) proved the x86 ADVMOT.lib is a
//  standard COFF import library with GNU-ld-compatible stdcall decorations.
//  This probe closes the loop: a real executable that references real Acm_*
//  imports, linked against the real SDK .lib by the pinned MinGW toolchain.
//
//  BUILD-ONLY BY DESIGN. It is not a ctest and must never become one
//  lightly: without a physical PCIE-1203 card, Acm_GetAvailableDevs /
//  Acm_DevOpen BLOCK for 15+ seconds (measured 20260818), and running it at
//  all needs ADVMOT.dll resolvable at load time. The link succeeding is the
//  entire deliverable; the CMake target only exists when find_library
//  locates the SDK (machines without it skip this probe gracefully).
//
//  When a card IS present one day, running it with an argument prints the
//  device count -- deliberately trivial, no motion, no device opened.
// =============================================================================
#include <cstdio>
#include <cstring>

// The vendor headers assume Windows types (ULONG/DOUBLE/...) are already in
// scope -- the six production TUs supply them transitively; here explicitly.
#include <windows.h>

// The compat shim supplies ADVCMNAPI for MinGW and #undef's the vendor's
// poisonous `Direct` macro -- the one sanctioned include point (1203HAL-1).
#include "EtherCAT/AdvMotCompat.h"

int main(int argc, char** argv)
{
    // Referenced-but-not-called keeps the import REQUIRED at link time while
    // keeping the default invocation instant (no SDK calls, no 15s hang).
    U32 (ADVCMNAPI *pGetDevs)(DEVLIST*, U32, PU32) = &Acm_GetAvailableDevs;
    BOOL (ADVCMNAPI *pGetErr)(U32, PI8, U32)       = &Acm_GetErrorMessage;

    std::printf("pci1203_linkprobe: linked against ADVMOT (GetAvailableDevs=%p, GetErrorMessage=%p)\n",
                (void*)pGetDevs, (void*)pGetErr);

    if (argc > 1 && std::strcmp(argv[1], "--enumerate") == 0) {
        // Only on explicit request: this can block 15+ seconds without a card.
        DEVLIST devs[8];
        U32 out = 0;
        const U32 ret = Acm_GetAvailableDevs(devs, 8, &out);
        std::printf("Acm_GetAvailableDevs ret=%lu, devices=%lu\n",
                    (unsigned long)ret, (unsigned long)out);
    }
    return 0;
}
