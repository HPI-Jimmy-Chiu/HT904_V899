//---------------------------------------------------------------------------
//  aoutarm9045_1x1_1.h  --  out-arm 1x1_1 per-site-config variant header
//
//  Translation wave: W6.2c (out-arm per-site-config variant: 1x1_1, single-site).
//  Translator: AI(W6.2c-1x1_1) 20260627
//  Golden source: aoutarm9045_1x1_1.h (7 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports the engine-
//  dispatched entry point DoOutArm_9045_1x1_1() and the helper
//  MoveOutArmToShuttleIncludeZ_9045_1x1_1(int iSht, bool bZDown=false).  In
//  addition the engine aoutarm9045.cpp dispatch ladder routes BOTH
//  iInArmType==e9045_1x1_1 AND iInArmType==e9045_1x4_1_Ac to
//  DoOutArm_9045_1x1_1() / DoPickFromShuttle_9045_1x1_1(int) (this file's two
//  unique callees).  DoPickFromShuttle_9045_1x1_1(int) is declared here too so
//  the engine's DoPickFromShuttle_9045 dispatch arm links against the real func.
//  All names / values / signatures / default args preserved VERBATIM.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_1x1_1H
#define aoutarm9045_1x1_1H
//---------------------------------------------------------------------------
void DoOutArm_9045_1x1_1();
bool MoveOutArmToShuttleIncludeZ_9045_1x1_1(int iSht, bool bZDown=false);
// engine-dispatched pick callee (golden body lives in this .cpp; engine
// aoutarm9045.cpp also declares it extern for its DoPickFromShuttle_9045 ladder)
bool DoPickFromShuttle_9045_1x1_1(int iSht);
#endif
