// ===========================================================================
//  tests/test_globals.cpp
//  W0-TAIL VERIFY gate (CTest target: globals)
//
//  Proves the translated GLOBAL define/type headers compile STANDALONE and
//  preserve the BCB6 contract values. This TU #includes every in-scope
//  translated global header and asserts (compile-time static_assert + a
//  runtime self-check) the define/enum -> value pairs cited in the Translate
//  note, plus sizeof()/offsetof() layout anchors for PROD_INFO_ST (the ~770
//  line master struct).
//
//  The successful #include + compile IS the core test; the assertions guard
//  against silent value/layout drift. Links ONLY against vclcompat (IntToHex,
//  AnsiString/TStringList/TDateTime value types) + myTimer.o (TQPF_Timer ctor,
//  fully translated this wave). It does NOT pull cprod.o/cmydef.o/cpublic.o
//  active-global ctors (W3/W6/W7-gated) -- header layout only.
//
//  LIMITATION: no Borland binary available -> values verified against the
//  9011UC golden SOURCE by inspection + these asserts, not a BCB6-compiled ref.
// ===========================================================================
#include <cstddef>   // offsetof
#include <cstdio>

#include "MachineType.h"
#include "Config.h"
#include "CosFunction.h"
#include "myTimer.h"
#include "cpublic.h"
#include "cprod.h"
#include "cmydef.h"
#include "MachineDefine.h"

// ---------------------------------------------------------------------------
//  (1) Preserved define / enum -> value pairs (compile-time)
//      Each pair is cited file:line from the 9011UC golden source.
// ---------------------------------------------------------------------------
// MachineType.h
static_assert(Type_HT9045      == 100, "eMachineType::Type_HT9045 (MachineType.h:426)");
static_assert(_8Site2X4        ==   9, "eTestMode::_8Site2X4 (MachineType.h:452)");
static_assert(e9045_2x8_32     ==  25, "eInArmType::e9045_2x8_32 (MachineType.h:1039)");
static_assert(OfsBottom2DID    ==  52, "eSECSOffset::OfsBottom2DID (MachineType.h:790)");
static_assert(eTrayCount       ==  33, "e6TrayName::eTrayCount (MachineType.h:1104)");
static_assert(ContactModeTotal ==  11, "eContactMode::ContactModeTotal (MachineType.h:535)");
static_assert(MAX_SOCKET_TOTAL ==  32, "#define MAX_SOCKET_TOTAL (MachineType.h:393)");
static_assert(CC_QUALCOMM      == 999, "#define CC_QUALCOMM (MachineType.h:377)");
static_assert(MAX_ARM_Row      ==   2, "#define MAX_ARM_Row (MachineType.h:387)");
static_assert(MAX_ARM_Col      ==   4, "#define MAX_ARM_Col (MachineType.h:388)");
static_assert(MAX_AUTO_TRAY    ==   6, "#define MAX_AUTO_TRAY (MachineType.h:401)");
// cmydef.h
static_assert(TOTAL_MOTOR      == 164, "#define TOTAL_MOTOR (cmydef.h:46)");

// ---------------------------------------------------------------------------
//  (2) PROD_INFO_ST layout anchors (the master ~770-line struct).
//      First member is char cFileName[256] (cprod.h:370) -> offset 0.
//      Second member is DATE_INFO LastEditDate (cprod.h:371); DATE_INFO is
//      11 ints (cprod.h:46-54) -> begins right after the 256-byte name buffer
//      at int alignment -> offset 256.
//      PROD_INFO_ST has no AnsiString/TStringList/TDateTime members (verified),
//      so it is a standard-layout aggregate and offsetof is well-defined.
// ---------------------------------------------------------------------------
static_assert(offsetof(PROD_INFO_ST, cFileName)    ==   0,
              "PROD_INFO_ST.cFileName must be first (offset 0)");
static_assert(offsetof(PROD_INFO_ST, LastEditDate) == 256,
              "PROD_INFO_ST.LastEditDate must follow cFileName[256]");
static_assert(sizeof(DATE_INFO) == 11 * sizeof(int),
              "DATE_INFO = 6 named ints + int[5] reserved");
// The master struct must be large (it carries the whole product setup); guard
// against an accidental field truncation collapsing it to a small size.
static_assert(sizeof(PROD_INFO_ST) > 50000,
              "PROD_INFO_ST sanity: must remain the large master setup struct");

// ---------------------------------------------------------------------------
//  (3) Value-struct headers (Config.h / CosFunction.h) instantiate cleanly
//      with their array-size deps pulled from MachineType.h.
// ---------------------------------------------------------------------------
static_assert(sizeof(HT9045_CONFIG)             > 0, "HT9045_CONFIG (Config.h:1498)");
static_assert(sizeof(HT9045_COUSTOMER_FUNCTION) > 0, "HT9045_COUSTOMER_FUNCTION (CosFunction.h:487)");

int main()
{
    int failures = 0;

    // Runtime mirror of the contract values (belt-and-suspenders: catches the
    // case where a macro is later redefined to a non-constant expression that
    // would slip past static_assert in a refactor).
    if (Type_HT9045      != 100) { ++failures; std::printf("FAIL Type_HT9045=%d\n", (int)Type_HT9045); }
    if (_8Site2X4        !=   9) { ++failures; std::printf("FAIL _8Site2X4=%d\n", (int)_8Site2X4); }
    if (e9045_2x8_32     !=  25) { ++failures; std::printf("FAIL e9045_2x8_32=%d\n", (int)e9045_2x8_32); }
    if (eTrayCount       !=  33) { ++failures; std::printf("FAIL eTrayCount=%d\n", (int)eTrayCount); }
    if (ContactModeTotal !=  11) { ++failures; std::printf("FAIL ContactModeTotal=%d\n", (int)ContactModeTotal); }
    if (MAX_SOCKET_TOTAL !=  32) { ++failures; std::printf("FAIL MAX_SOCKET_TOTAL=%d\n", (int)MAX_SOCKET_TOTAL); }
    if (TOTAL_MOTOR      != 164) { ++failures; std::printf("FAIL TOTAL_MOTOR=%d\n", (int)TOTAL_MOTOR); }
    if (CC_QUALCOMM      != 999) { ++failures; std::printf("FAIL CC_QUALCOMM=%d\n", (int)CC_QUALCOMM); }

    // Exercise the value types so the field layouts actually instantiate
    // (header-inline + the two .o-resident symbols pulled via the lib).
    TQPF_Timer timer; timer.SetMS(10); timer.On(); (void)timer.Off();

    IntUnionBit u; u.UnsignInt = 0xABu;
    // Hex(false) -> bare hex; Hex(true) -> "0x"-prefixed. IntToHex(0xAB,2)=="AB".
    if (u.Hex(false) != "AB" && u.Hex(false) != "ab") {
        // tolerate case to avoid coupling the test to the formatter's choice.
        ++failures; std::printf("FAIL IntUnionBit::Hex(0xAB) -> '%s'\n", u.Hex(false).c_str());
    }
    if (u.Hex(true) != "0xAB" && u.Hex(true) != "0xab") {
        ++failures; std::printf("FAIL IntUnionBit::Hex(0xAB,0x) -> '%s'\n", u.Hex(true).c_str());
    }

    TDateTime dt(0.0); (void)dt;
    HT9045_CONFIG cfg;             (void)cfg.bEnableRms;
    HT9045_COUSTOMER_FUNCTION cf;  (void)cf.bVerifyMode;

    if (failures == 0) {
        std::printf("test_globals: OK (all W0-TAIL contract values + PROD_INFO_ST layout preserved)\n");
        return 0;
    }
    std::printf("test_globals: %d FAILURE(S)\n", failures);
    return 1;
}
