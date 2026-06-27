//---------------------------------------------------------------------------
//  aoutarm9045_1x4_8.h  --  out-arm 1x4_8 per-site-config variant header
//
//  Translation wave: W6.2c (out-arm per-site-config variant: 1x4_8).
//  Translator: AI(W6.2c-1x4_8) 20260627
//  Golden source: aoutarm9045_1x4_8.h (7 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports
//  DoOutArm_9045_1x4_8() and MoveOutArmToShuttleIncludeZ_9045_1x4_8(int,bool)
//  with their VERBATIM signatures / default args (NOTE: 1x4_8 uses the 2-param
//  MoveOutArmToShuttleIncludeZ -- no iKit/iWhichKit -- unlike 1x4_2).  In
//  addition the second engine-dispatched entry point
//  DoPickFromShuttle_9045_1x4_8(int) is declared here (the engine aoutarm9045.cpp
//  also declares it as an extern for its DoPickFromShuttle dispatch ladder, which
//  routes iInArmType==e9045_1x4_8_Hot here -- see the ENUM->CALLEE map:
//  1x4_8_Hot -> 1x4_8).  All names / values / signatures preserved VERBATIM so the
//  team can reverse-look-up the BCB6 version.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_1x4_8H
#define aoutarm9045_1x4_8H
//---------------------------------------------------------------------------
void DoOutArm_9045_1x4_8();
bool MoveOutArmToShuttleIncludeZ_9045_1x4_8(int iSht, bool bZDown=false);

// engine-dispatched entry point (defined in aoutarm9045_1x4_8.cpp; the engine
// aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
// dispatch ladder, which routes iInArmType==e9045_1x4_8_Hot here)
bool DoPickFromShuttle_9045_1x4_8(int iSht);
#endif
