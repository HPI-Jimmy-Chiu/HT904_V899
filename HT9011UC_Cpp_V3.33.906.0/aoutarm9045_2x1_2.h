//---------------------------------------------------------------------------
//  aoutarm9045_2x1_2.h  --  out-arm 2x1_2 per-site-config variant header
//
//  Translation wave: W6.2c-OUT (out-arm per-site-config variant: 2x1_2,
//  two-row single-column / 2-site).  Translator: AI(W6.2c-2x1_2) 20260627
//  Golden source: aoutarm9045_2x1_2.h (8 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports only the two
//  entry points DoOutArm_9045_2x1_2() and MoveOutArmToShuttleIncludeZ_9045_2x1_2
//  (the engine-dispatched DoPickFromShuttle_9045_2x1_2(int) is declared by the
//  engine aoutarm9045.cpp extern block, not in this golden header).  Names/
//  values/signatures preserved VERBATIM.
//
//  engine-dispatched entry points (defined in aoutarm9045_2x1_2.cpp; the engine
//  aoutarm9045.cpp also declares DoOutArm_9045_2x1_2 / DoPickFromShuttle_9045_2x1_2
//  as externs for its two dispatch ladders, which route iInArmType==e9045_2x1_2_13
//  here).
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x1_2H
#define aoutarm9045_2x1_2H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x1_2();
bool MoveOutArmToShuttleIncludeZ_9045_2x1_2(int iSht, bool bZDown=false);
#endif
