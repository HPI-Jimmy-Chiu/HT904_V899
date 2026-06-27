//---------------------------------------------------------------------------
//  aoutarm9045_1x3_2_14.h  --  out-arm 1x3_2_14 per-site-config variant header
//
//  Translation wave: W6.2c (out-arm per-site-config variant: 1x3_2_14)
//  Translator: AI(W6.2c-1x3_2_14) 20260627
//  Golden source: aoutarm9045_1x3_2_14.h (8 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports the helper
//  MoveOutArmToShuttleIncludeZ_9045_1x3_2_14(int,int,bool=false) plus the engine-
//  dispatched entry point DoOutArm_9045_1x3_2_14().  In addition the engine
//  ladder in aoutarm9045.cpp dispatches DoPickFromShuttle_9045_1x3_2_14(int)
//  (golden declares that callee as an extern in the engine TU), so we declare it
//  here too -- this is the LIVE callee for iInArmType==e9045_1x3_2_14 for BOTH the
//  DoOutArm_9045 and the DoPickFromShuttle_9045 dispatch ladders.  All names /
//  signatures / default args preserved VERBATIM for BCB6 reverse-lookup.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_1x3_2_14H
#define aoutarm9045_1x3_2_14H
//---------------------------------------------------------------------------
void DoOutArm_9045_1x3_2_14();
bool MoveOutArmToShuttleIncludeZ_9045_1x3_2_14(int iSht, int iKit, bool bZDown=false);

// engine-dispatched pick callee (defined in aoutarm9045_1x3_2_14.cpp; the engine
// aoutarm9045.cpp also declares this as an extern for its DoPickFromShuttle_9045
// dispatch ladder, which routes iInArmType==e9045_1x3_2_14 here)
bool DoPickFromShuttle_9045_1x3_2_14(int iSht);

#endif
