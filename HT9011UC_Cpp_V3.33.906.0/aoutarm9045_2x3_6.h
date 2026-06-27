//---------------------------------------------------------------------------
//  aoutarm9045_2x3_6.h  --  out-arm 2x3_6 per-site-config variant header
//
//  Translation wave: W6.2c-OUT (out-arm per-site-config variant: 2x3_6).
//  Translator: AI(W6.2c-2x3_6) 20260627
//  Golden source: aoutarm9045_2x3_6.h (7 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports
//  DoOutArm_9045_2x3_6() and the TWO-parameter
//  MoveOutArmToShuttleIncludeZ_9045_2x3_6(int iSht, bool bZDown=false)
//  with their VERBATIM signatures / default args.  NOTE: this variant's
//  MoveOutArmToShuttleIncludeZ is 2-param (unlike the 3-param 2x4_4 / 2x5_8 /
//  2x6_8 / 2x8_8 siblings) -- preserved EXACTLY here and in the .cpp definition.
//
//  In addition the second engine-dispatched entry point
//  DoPickFromShuttle_9045_2x3_6(int) is declared here (the engine
//  aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
//  dispatch ladder, which routes iInArmType==e9045_2x3_6 here -- see the
//  ENUM->CALLEE map: 2x3_6 -> 2x3_6).  All names / values / signatures preserved
//  VERBATIM so the team can reverse-look-up the BCB6 version.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x3_6H
#define aoutarm9045_2x3_6H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x3_6();
bool MoveOutArmToShuttleIncludeZ_9045_2x3_6(int iSht, bool bZDown=false);

// engine-dispatched entry point (defined in aoutarm9045_2x3_6.cpp; the engine
// aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
// dispatch ladder, which routes iInArmType==e9045_2x3_6 here)
bool DoPickFromShuttle_9045_2x3_6(int iSht);
#endif
