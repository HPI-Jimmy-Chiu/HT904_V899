//---------------------------------------------------------------------------
//  aoutarm9045_2x3_6_14.h  --  out-arm 2x3_6_14 per-site-config variant header
//
//  Translation wave: W6.2c-OUT (out-arm per-site-config variant: 2x3_6_14).
//  Translator: AI(W6.2c-2x3_6_14) 20260627
//  Golden source: aoutarm9045_2x3_6_14.h (7 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports
//  DoOutArm_9045_2x3_6_14() and the THREE-parameter
//  MoveOutArmToShuttleIncludeZ_9045_2x3_6_14(int iSht, int iWhichKit, bool bZDown=false)
//  with their VERBATIM signatures / default args.  NOTE: this variant's
//  MoveOutArmToShuttleIncludeZ carries an extra iWhichKit parameter vs the other
//  six out-arm variants (which are 2-param) -- preserved EXACTLY here and in the
//  .cpp definition (where the formal is named `iKit`, golden file:line 268).
//
//  In addition the second engine-dispatched entry point
//  DoPickFromShuttle_9045_2x3_6_14(int) is declared here (the engine
//  aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
//  dispatch ladder, which routes iInArmType==e9045_2x3_6_14 here -- see the
//  ENUM->CALLEE map: 2x3_6_14 -> 2x3_6_14).  All names / values / signatures
//  preserved VERBATIM so the team can reverse-look-up the BCB6 version.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x3_6_14H
#define aoutarm9045_2x3_6_14H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x3_6_14();
bool MoveOutArmToShuttleIncludeZ_9045_2x3_6_14(int iSht, int iWhichKit, bool bZDown=false);

// engine-dispatched entry point (defined in aoutarm9045_2x3_6_14.cpp; the engine
// aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
// dispatch ladder, which routes iInArmType==e9045_2x3_6_14 here)
bool DoPickFromShuttle_9045_2x3_6_14(int iSht);
#endif
