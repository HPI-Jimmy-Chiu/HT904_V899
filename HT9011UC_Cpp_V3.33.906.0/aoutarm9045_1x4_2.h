//---------------------------------------------------------------------------
//  aoutarm9045_1x4_2.h  --  out-arm 1x4_2 per-site-config variant header
//
//  Translation wave: W6.2c (out-arm per-site-config variant: 1x4_2).
//  Translator: AI(W6.2c-1x4_2) 20260627
//  Golden source: aoutarm9045_1x4_2.h (7 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports
//  DoOutArm_9045_1x4_2() and MoveOutArmToShuttleIncludeZ_9045_1x4_2(int,int,bool)
//  with their VERBATIM signatures / default args.  In addition the second engine-
//  dispatched entry point DoPickFromShuttle_9045_1x4_2(int) is declared here (the
//  engine aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
//  dispatch ladder, which routes iInArmType==e9045_1x4_2_14 here -- see the
//  ENUM->CALLEE map: 1x4_2_14 -> 1x4_2).  All names / values / signatures
//  preserved VERBATIM so the team can reverse-look-up the BCB6 version.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_1x4_2H
#define aoutarm9045_1x4_2H
//---------------------------------------------------------------------------
void DoOutArm_9045_1x4_2();
bool MoveOutArmToShuttleIncludeZ_9045_1x4_2(int iSht, int iWhichKit, bool bZDown=false);

// engine-dispatched entry point (defined in aoutarm9045_1x4_2.cpp; the engine
// aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
// dispatch ladder, which routes iInArmType==e9045_1x4_2_14 here)
bool DoPickFromShuttle_9045_1x4_2(int iSht);
#endif
