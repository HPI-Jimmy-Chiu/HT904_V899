// ===========================================================================
//  tests/w0tail_headers_compile.cpp
//  W0-TAIL compile gate: the 5 global define/type headers + their value-struct
//  / timer deps must compile + link STANDALONE (MinGW g++ 6.3, C++17).
//
//  This TU includes every translated W0-TAIL header. It links against ONLY the
//  vclcompat layer + myTimer.o (whose TQPF_Timer ctor is fully translated this
//  wave). It does NOT pull cprod.o/cmydef.o/cpublic.o (whose active global
//  definitions construct objects whose ctors live in the W6-gated .cpp regions
//  -- those resolve in later waves). This is the "header-only TU links" gate.
// ===========================================================================
#include "MachineType.h"
#include "Config.h"
#include "CosFunction.h"
#include "myTimer.h"
#include "cpublic.h"
#include "cprod.h"
#include "cmydef.h"
#include "MachineDefine.h"

int main()
{
    // Spot-check a few preserved enum / #define values (compile-time asserts).
    static_assert(Type_HT9045        == 100, "eMachineType::Type_HT9045");
    static_assert(_8Site2X4          == 9,   "eTestMode::_8Site2X4");
    static_assert(e9045_2x8_32       == 25,  "eInArmType::e9045_2x8_32");
    static_assert(OfsBottom2DID      == 52,  "eSECSOffset::OfsBottom2DID");
    static_assert(eTrayCount         == 33,  "e6TrayName::eTrayCount");
    static_assert(ContactModeTotal   == 11,  "eContactMode::ContactModeTotal");
    static_assert(MAX_SOCKET_TOTAL   == 32,  "#define MAX_SOCKET_TOTAL");
    static_assert(TOTAL_MOTOR        == 164, "#define TOTAL_MOTOR (cmydef.h)");
    static_assert(CC_QUALCOMM        == 999, "#define CC_QUALCOMM");

    // Exercise the value types so the headers' field layouts instantiate.
    // (TQPF_Timer ctor -> myTimer.o; IntUnionBit::Hex -> IntToHex in vclcompat;
    //  the rest are header-inline.)
    TQPF_Timer t; t.SetMS(10); t.On(); (void)t.Off();
    IntUnionBit u; u.UnsignInt = 0xABu; (void)u.Hex(true);
    TDateTime d(0.0); (void)d;
    // Instantiate the pure-data W0 contract structs locally (layout check only;
    // the real extern globals are defined in cprod.cpp / Config is value-struct).
    HT9045_CONFIG cfg; (void)cfg.bEnableRms;
    HT9045_COUSTOMER_FUNCTION cf; (void)cf.bVerifyMode;
    return 0;
}
