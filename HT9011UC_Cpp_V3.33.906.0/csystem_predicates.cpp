// =============================================================================
//  csystem_predicates.cpp  --  the W6 cross-arm IC-presence PREDICATE API
//
//  Mirrors (partial) of: HT9011UC_Code_V3.33.906.0_20260618/csystem.cpp
//                        (the ~15 HasIC-family bodies, csystem.cpp:964-1047 +
//                         HasICUnderHotPlate at :12664).
//  Translation wave: W6.0 (freeze + implement the SM-to-SM decoupling contract)
//  Translator: AI(W6.0-SCAFFOLD) 20260626
//
//  WHY THIS FILE EXISTS
//  --------------------
//  csystem.h is frozen as the W6 INTERFACE.  Its load-bearing part is the
//  cross-arm IC-presence predicate family.  State machines never cross-call each
//  other's Do* functions; they only consult these predicates.  This .cpp gives
//  the frozen interface a linkable definition.
//
//  NOTE: the W6.1 canary (asendic_Empty) exercises ZERO of these predicates
//  (verified: 0 grep hits for any HasIC name in asendic_Empty.cpp).  They are
//  implemented now ONLY to lock the contract for later sub-waves and to keep the
//  frozen interface linkable.
//
//  FAITHFUL CALL-GRAPH, STUBBED LEAVES
//  -----------------------------------
//  The golden leaf predicates read the vacuum-suck / carry-kit grid objects
//  (FLCarryKit / BLCarryKit / FRCarryKit / BRCarryKit / FTestSuck / BTestSuck /
//  TestSocket / OutSht3Kit -- all TMyKitSuck/TMySucker).  That entire grid was
//  DEFERRED by the W4-IO HAL note ("Entire TMyKitSuck data grid ... deferred to
//  W6") and is NOT yet in the target.  So:
//    * the COMPOSITE predicates (InputShuttleHasIC / OutputShuttleHasIC /
//      ShuttleHasIC / TestHeadHasIC / IndexHasIC) delegate to the leaves
//      EXACTLY as the golden does -- the contract logic shape is preserved.
//    * the LEAF predicates' grid access (`Kit.UseSiteHasIC()`) is gated
//      `#if 0 // TODO(W6.x: TMyKitSuck grid)` with a conservative `return false`
//      active fallback.  When the KitSuck grid lands, flip the gate.
//  The IniConfig / TestIF_File arm-2 guards (which DO exist in the target) are
//  kept live so the Rear-variant branch structure stays faithful.
// =============================================================================
#include "csystem.h"
#include "cmydef.h"     // IniConfig, MOT[]-adjacent globals, consts
#include "cprod.h"      // Prod, TestIF_File

// When the TMyKitSuck grid is translated (W6.x), define this to re-activate the
// real Kit.UseSiteHasIC()/HasRealIC() reads.  Until then the leaves return the
// conservative "no IC present" answer.
#define HT9045_KITSUCK_GRID_AVAILABLE 0

//==============================================================================
//  Input shuttle (front carry-kit FLCarryKit)
//==============================================================================
bool InputShuttleFrontHasIC()
{
#if HT9045_KITSUCK_GRID_AVAILABLE
    return (FLCarryKit.UseSiteHasIC());
#else
    return false;   // TODO(W6.x): FLCarryKit.UseSiteHasIC() (TMyKitSuck grid)
#endif
}
//==============================================================================
bool InputShuttleRearHasIC()
{
    if(IniConfig.bIndexArm2SupplyLight==true ||                                 //jou 2012-10-19 Index Arm 2 supply light for CMOS
       TestIF_File.bForEgisTecTest==true     ||                                 //Steven 20140922 : Arm2 used for fingerprint test
       (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                         //kevin 20150127 Arm1 press, arm2 test
        TestIF_File.bArm1PickPlaceArm2Test==true))                              //Ifor 20200811 Fix: needs both conditions
    {
        return false;
    }

#if HT9045_KITSUCK_GRID_AVAILABLE
    return (BLCarryKit.UseSiteHasIC());
#else
    return false;   // TODO(W6.x): BLCarryKit.UseSiteHasIC() (TMyKitSuck grid)
#endif
}
//==============================================================================
bool InputShuttleHasIC()
{
    return (InputShuttleFrontHasIC() || InputShuttleRearHasIC());
}
//==============================================================================
//  Output shuttle (front/rear carry-kit FRCarryKit / BRCarryKit)
//==============================================================================
bool OutputShuttleFrontHasIC()
{
#if HT9045_KITSUCK_GRID_AVAILABLE
    return (FRCarryKit.UseSiteHasIC());
#else
    return false;   // TODO(W6.x): FRCarryKit.UseSiteHasIC() (TMyKitSuck grid)
#endif
}
//==============================================================================
bool OutputShuttleRearHasIC()
{
    if(IniConfig.bIndexArm2SupplyLight==true ||                                 //jou 2012-10-19 Index Arm 2 supply light for CMOS
       TestIF_File.bForEgisTecTest==true     ||                                 //Steven 20140922 : Arm2 used for fingerprint test
       (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                         //kevin 20150127 Arm1 press, arm2 test
        TestIF_File.bArm1PickPlaceArm2Test==true))                              //Ifor 20200811 Fix: needs both conditions
    {
        return false;
    }
#if HT9045_KITSUCK_GRID_AVAILABLE
    return (BRCarryKit.UseSiteHasIC());
#else
    return false;   // TODO(W6.x): BRCarryKit.UseSiteHasIC() (TMyKitSuck grid)
#endif
}
//==============================================================================
bool OutputShuttleHasIC()
{
#if HT9045_KITSUCK_GRID_AVAILABLE
    return (FRCarryKit.UseSiteHasIC() || BRCarryKit.UseSiteHasIC());
#else
    return false;   // TODO(W6.x): FRCarryKit/BRCarryKit.UseSiteHasIC() (TMyKitSuck grid)
#endif
}
//==============================================================================
bool ShuttleHasIC()
{
    return (InputShuttleHasIC() || OutputShuttleHasIC());
}
//==============================================================================
bool SortShuttleHasIC()                                                         //RogerYang 20250506 Add for 9046AU
{
#if HT9045_KITSUCK_GRID_AVAILABLE
    return (OutSht3Kit.UseSiteHasIC());
#else
    return false;   // TODO(W6.x): OutSht3Kit.UseSiteHasIC() (TMyKitSuck grid)
#endif
}
//==============================================================================
//  Test head / socket (FTestSuck / BTestSuck / TestSocket)
//==============================================================================
bool FrontTestHeadHasIC()
{
#if HT9045_KITSUCK_GRID_AVAILABLE
    return (FTestSuck.UseSiteHasIC());
#else
    return false;   // TODO(W6.x): FTestSuck.UseSiteHasIC() (TMyKitSuck grid)
#endif
}
//==============================================================================
bool RearTestHeadHasIC()
{
#if HT9045_KITSUCK_GRID_AVAILABLE
    return (BTestSuck.UseSiteHasIC());
#else
    return false;   // TODO(W6.x): BTestSuck.UseSiteHasIC() (TMyKitSuck grid)
#endif
}
//==============================================================================
bool TestHeadHasIC()
{
    return (FrontTestHeadHasIC() || RearTestHeadHasIC());
}
//==============================================================================
bool TestSocketHasIC()
{
#if HT9045_KITSUCK_GRID_AVAILABLE
    return (TestSocket.UseSiteHasIC());
#else
    return false;   // TODO(W6.x): TestSocket.UseSiteHasIC() (TMyKitSuck grid)
#endif
}
//==============================================================================
bool IndexHasIC()
{
    return (TestHeadHasIC() || TestSocketHasIC());
}
//==============================================================================
bool IndexHasRealIC()                                                           //Steven 20240217 : Fixed for one cycle / clean out hang up
{
#if HT9045_KITSUCK_GRID_AVAILABLE
    return (FTestSuck.HasRealIC() || BTestSuck.HasRealIC() || TestSocket.HasRealIC());
#else
    return false;   // TODO(W6.x): FTestSuck/BTestSuck/TestSocket.HasRealIC() (TMyKitSuck grid)
#endif
}
//==============================================================================
//  HasICUnderHotPlate (golden csystem.cpp:12664)
//  Golden walks the HotPlate occupancy via MOT[MHotPlate*].HasIC()/Prod hotplate
//  state.  The hotplate MOT-index map + Prod hotplate-occupancy accessor is not
//  wired this wave; conservative false until W6.x.
//==============================================================================
bool HasICUnderHotPlate()                                                       //Steven 20110826
{
    return false;   // TODO(W6.x): MOT[MHotPlate*].HasIC() / Prod hotplate occupancy
}
//==============================================================================
//  Cross-arm aggregate predicates (declared in csystem.h; golden bodies are the
//  big sHasICUnderMachine / HasAutoICInMachine surface, gated in csystem.cpp).
//  Provide the boolean aggregates the contract names; conservative until W6.x.
//==============================================================================
bool HasICUnderMachine()
{
    // TODO(W6.x): golden aggregates Loader/HotPlate/Shuttle/Index occupancy.
    return (ShuttleHasIC() || IndexHasIC() || HasICUnderHotPlate());
}
//==============================================================================
bool HasAutoICInMachine()
{
    // TODO(W6.x): golden walks Auto-tray IC occupancy (MOT[]/Prod).
    return false;
}
//==============================================================================
bool HasAnyICInMachine()                                                        //kevin 20150914 : whether any tray still in machine
{
    // TODO(W6.x): golden aggregates every IC-bearing position.
    return (HasICUnderMachine() || HasAutoICInMachine());
}
//==============================================================================
//  W6.2 additions: frozen csystem predicates/helpers the shared HP geometry
//  leaves (ainarm_SearchPickPlate/PlacePlate) consult.  These are declared in
//  csystem.h; the golden bodies live in csystem.cpp (gated #if 0).  Offline sim
//  bodies follow -- conservative, matching a handler with shuttles not docked
//  and the safe door closed.  AI(W6.2-INARM) 20260626.
//==============================================================================
//  InSHT1InLF / InSHT2InLF (golden csystem.cpp:413/498): the input shuttle is in
//  its left-feed position.  Offline there is no shuttle motion -> not in LF.
bool InSHT1InLF() { return false; }
bool InSHT2InLF() { return false; }
//------------------------------------------------------------------------------
//  CheckSafeDoorIsClosed (golden csystem.cpp:2599): offline (no PLC / no door
//  sensor) the door is treated as CLOSED so the #ifndef SOFT_SIMULTE guard the
//  leaves call is non-blocking.
bool CheckSafeDoorIsClosed() { return true; }
//------------------------------------------------------------------------------
//  DoAutoSiteMappingDropError (golden csystem.cpp:23361): only acts when
//  bRunAutoSiteMapping==true.  Offline ASM is off -> the golden body is a no-op;
//  keep the guard shape faithful (re-reads bRunAutoSiteMapping at the call).
void DoAutoSiteMappingDropError()
{
    // TODO(W6.x/W7): full ASM drop-error recovery (InitInArmTask + grid reset)
    // lands with the ainarm core.  Offline ASM is off, so this is a no-op.
}
//==============================================================================
