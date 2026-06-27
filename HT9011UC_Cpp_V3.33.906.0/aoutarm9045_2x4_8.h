//---------------------------------------------------------------------------
//  aoutarm9045_2x4_8.h  --  out-arm 2x4_8 per-site-config variant header
//
//  Translation wave: W6.2c-OUT batch-3 final (out-arm per-site-config variant: 2x4_8).
//  Translator: AI(W6.2c-2x4_8) 20260627
//  Golden source: aoutarm9045_2x4_8.h (8 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports exactly two
//  symbols with VERBATIM signatures / default args:
//      void DoOutArm_9045_2x4_8();
//      bool MoveOutArmToShuttleIncludeZ_9045_2x4_8(int iSht, bool bZDown=false);
//  (this variant's MoveOutArmToShuttleIncludeZ is the 2-PARAM form -- no iKit,
//  unlike the sibling 2x4_4 / 2x5_8 / 2x6_8 / 2x8_8 variants).
//
//  In addition the second engine-dispatched entry point
//  DoPickFromShuttle_9045_2x4_8(int) is declared here (the engine
//  aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
//  dispatch ladder, which routes iInArmType==e9045_2x4_8 here -- see the
//  ENUM->CALLEE map: 2x4_8 -> 2x4_8).  All names / values / signatures preserved
//  VERBATIM so the team can reverse-look-up the BCB6 version.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x4_8H
#define aoutarm9045_2x4_8H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x4_8();
bool MoveOutArmToShuttleIncludeZ_9045_2x4_8(int iSht, bool bZDown=false);

// engine-dispatched entry point (defined in aoutarm9045_2x4_8.cpp; the engine
// aoutarm9045.cpp also declares it as an extern for its DoPickFromShuttle
// dispatch ladder, which routes iInArmType==e9045_2x4_8 here)
bool DoPickFromShuttle_9045_2x4_8(int iSht);
#endif
