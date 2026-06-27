//---------------------------------------------------------------------------
//  aoutarm9045_1x4_4S.h  --  out-arm 1x4_4S per-site-config variant header
//
//  Translation wave: W6.2c (out-arm per-site-config variant: 1x4_4S).
//  Translator: AI(W6.2c-1x4_4S) 20260627
//  Golden source: aoutarm9045_1x4_4S.h (7 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports
//  DoOutArm_9045_1x4_4S() and MoveOutArmToShuttleIncludeZ_9045_1x4_4S(int,bool)
//  with their VERBATIM signatures / default args.  In addition the second engine-
//  dispatched entry point DoPickFromShuttle_9045_1x4_4S(int) is declared here (the
//  engine aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
//  dispatch ladder, which routes iInArmType==e9045_1x4_4_13 here -- see the
//  ENUM->CALLEE map: 1x4_4_13 -> 1x4_4S).  All names / values / signatures
//  preserved VERBATIM so the team can reverse-look-up the BCB6 version.
//
//  NAMING NOTE: unlike the sibling out-arm variants, this layout's pure helpers
//  carry the _9045_ infix (GetNowShuttleMode_9045_1x4_4S, OutArmZNeedDown_9045_1x4_4S,
//  CheckOutArmXYPitch_9045_1x4_4S) -- preserved VERBATIM from the golden source.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_1x4_4SH
#define aoutarm9045_1x4_4SH
//---------------------------------------------------------------------------
void DoOutArm_9045_1x4_4S();
bool MoveOutArmToShuttleIncludeZ_9045_1x4_4S(int iSht, bool bZDown=false);

// engine-dispatched entry point (defined in aoutarm9045_1x4_4S.cpp; the engine
// aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
// dispatch ladder, which routes iInArmType==e9045_1x4_4_13 here)
bool DoPickFromShuttle_9045_1x4_4S(int iSht);
#endif
