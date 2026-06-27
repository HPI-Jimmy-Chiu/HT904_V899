//---------------------------------------------------------------------------
//  aoutarm9045_1x2_4.h  --  out-arm 1x2_4 per-site-config variant header
//
//  Translation wave: W6.2c (out-arm per-site-config variant: 1x2_4 -- one row,
//  2 pickers, 4-column-shuttle layout).  Translator: AI(W6.2c-1x2_4) 20260627.
//  Golden source: aoutarm9045_1x2_4.h (7 lines) -- mirror name + relative path = ROOT.
//
//  FAITHFUL mirror of the golden header.  The golden .h exports the engine-
//  dispatched entry point DoOutArm_9045_1x2_4() plus the file-scope helper
//  MoveOutArmToShuttleIncludeZ_9045_1x2_4(int,bool=false).  The out-arm engine
//  (aoutarm9045.cpp) ALSO declares DoOutArm_9045_1x2_4() / DoPickFromShuttle_9045_1x2_4(int)
//  as externs for its DoOutArm_9045 / DoPickFromShuttle_9045 dispatch ladders,
//  which route iInArmType==e9045_1x2_4_Hot here.  DoPickFromShuttle_9045_1x2_4(int)
//  is defined in the .cpp; declared here so the engine's extern resolves at link.
//  All names / values / signatures / default args preserved VERBATIM.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_1x2_4H
#define aoutarm9045_1x2_4H
//---------------------------------------------------------------------------
void DoOutArm_9045_1x2_4();
bool MoveOutArmToShuttleIncludeZ_9045_1x2_4(int iSht, bool bZDown=false);
// engine-dispatched pick callee (defined in aoutarm9045_1x2_4.cpp; the engine
// aoutarm9045.cpp declares it as an extern for its DoPickFromShuttle_9045 ladder)
bool DoPickFromShuttle_9045_1x2_4(int iSht);
#endif
