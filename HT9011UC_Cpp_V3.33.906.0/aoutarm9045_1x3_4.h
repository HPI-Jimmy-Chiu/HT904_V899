//---------------------------------------------------------------------------
//  aoutarm9045_1x3_4.h  --  out-arm 1x3_4 per-site-config variant header
//
//  Translation wave: W6.2c (out-arm per-site-config variant: 1x3_4,
//  single-row 3-picker over the 4-wide shuttle).
//  Translator: AI(W6.2c-1x3_4) 20260627
//  Golden source: aoutarm9045_1x3_4.h (7 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports the engine-
//  dispatched entry point DoOutArm_9045_1x3_4() and the file-local mover
//  MoveOutArmToShuttleIncludeZ_9045_1x3_4(int, bool=false).  In addition the
//  second engine-dispatched entry point DoPickFromShuttle_9045_1x3_4(int) is
//  declared here -- the out-arm engine aoutarm9045.cpp also declares it as an
//  extern for its DoPickFromShuttle_9045 dispatch ladder, which routes
//  iInArmType==e9045_1x3_4 here.  All names / signatures / default args
//  preserved VERBATIM.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_1x3_4H
#define aoutarm9045_1x3_4H
//---------------------------------------------------------------------------
void DoOutArm_9045_1x3_4();
bool DoPickFromShuttle_9045_1x3_4(int iSht);
bool MoveOutArmToShuttleIncludeZ_9045_1x3_4(int iSht, bool bZDown=false);
#endif
