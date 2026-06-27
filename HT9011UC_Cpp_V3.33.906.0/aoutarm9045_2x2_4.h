//---------------------------------------------------------------------------
//  aoutarm9045_2x2_4.h  --  out-arm 2x2_4 per-site-config variant header
//
//  Translation wave: W6.2c (out-arm per-site-config variant: 2x2_4).
//  Translator: AI(W6.2c-2x2_4) 20260627
//  Golden source: aoutarm9045_2x2_4.h (7 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports
//  DoOutArm_9045_2x2_4() and MoveOutArmToShuttleIncludeZ_9045_2x2_4(int,bool)
//  with their VERBATIM signatures / default args.  In addition the second engine-
//  dispatched entry point DoPickFromShuttle_9045_2x2_4(int) is declared here (the
//  engine aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
//  dispatch ladder, which routes iInArmType==e9045_2x2_4_12/13/14 here -- see the
//  ENUM->CALLEE map: 2x2_4_12|13|14 -> 2x2_4).  All names / values / signatures
//  preserved VERBATIM so the team can reverse-look-up the BCB6 version.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x2_4H
#define aoutarm9045_2x2_4H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x2_4();
bool MoveOutArmToShuttleIncludeZ_9045_2x2_4(int iSht, bool bZDown=false);

// engine-dispatched entry point (defined in aoutarm9045_2x2_4.cpp; the engine
// aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
// dispatch ladder, which routes iInArmType==e9045_2x2_4_12/13/14 here)
bool DoPickFromShuttle_9045_2x2_4(int iSht);
#endif
