//---------------------------------------------------------------------------
//  aoutarm9045_2x4_4.h  --  out-arm 2x4_4 per-site-config variant header
//
//  Translation wave: W6.2c-OUT batch-3 final (out-arm per-site-config variant: 2x4_4).
//  Translator: AI(W6.2c-2x4_4) 20260627
//  Golden source: aoutarm9045_2x4_4.h (7 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports
//  DoOutArm_9045_2x4_4() and the THREE-parameter
//  MoveOutArmToShuttleIncludeZ_9045_2x4_4(int iSht, int iKit, bool bZDown=false)
//  with their VERBATIM signatures / default args.  NOTE: this variant's
//  MoveOutArmToShuttleIncludeZ carries an extra iKit parameter vs the 2-param
//  out-arm variants -- preserved EXACTLY here and in the .cpp definition
//  (golden .h:6, golden .cpp:486).  This is the SIBLING export that
//  aoutarm9045_2x3_6_14.cpp forward-declares + calls VERBATIM (golden
//  aoutarm9045_2x3_6_14.cpp:352); it is DEFINED here, so the serial Integrate
//  phase REMOVES the temporary stub in aoutarm_shims.cpp.
//
//  In addition the second engine-dispatched entry point
//  DoPickFromShuttle_9045_2x4_4(int) is declared here (the engine
//  aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
//  dispatch ladder, which routes iInArmType==e9045_2x4_4 / _13 / _14 here --
//  see the ENUM->CALLEE map: 2x2_4_12|13|14 -> 2x2_4 ; 2x4_4_13|14 -> 2x4_4).
//  All names / values / signatures preserved VERBATIM so the team can
//  reverse-look-up the BCB6 version.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x4_4H
#define aoutarm9045_2x4_4H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x4_4();
bool MoveOutArmToShuttleIncludeZ_9045_2x4_4(int iSht, int iKit, bool bZDown=false);

// engine-dispatched entry point (defined in aoutarm9045_2x4_4.cpp; the engine
// aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
// dispatch ladder, which routes iInArmType==e9045_2x4_4 here)
bool DoPickFromShuttle_9045_2x4_4(int iSht);
#endif
