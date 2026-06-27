//---------------------------------------------------------------------------
//  aoutarm9045_2x6_8.h  --  out-arm 2x6_8 per-site-config variant header
//
//  Translation wave: W6.2c-OUT (out-arm per-site-config variant: 2x6_8, 12-site 2x6)
//  Translator: AI(W6.2c-OUT-2x6_8) 20260627
//  Golden source: aoutarm9045_2x6_8.h (8 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports the engine-
//  dispatched entry point DoOutArm_9045_2x6_8() and the per-site shuttle-pick
//  helper MoveOutArmToShuttleIncludeZ_9045_2x6_8(int iSht, int iWhichKit,
//  bool bZDown=false) -- the .cpp def names the 2nd param `iKit`, the golden .h
//  declares it `iWhichKit`; preserved VERBATIM.  The engine aoutarm9045.cpp also
//  declares DoOutArm_9045_2x6_8() + DoPickFromShuttle_9045_2x6_8(int) as externs
//  for its two dispatch ladders (iInArmType==e9045_2x6_8 -> both callees).
//  DoPickFromShuttle_9045_2x6_8(int) is NOT in the golden .h (it is engine-
//  declared); it is defined in the .cpp and declared here too for self-doc.
//  All names/values/signatures preserved VERBATIM.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x6_8H
#define aoutarm9045_2x6_8H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x6_8();
bool MoveOutArmToShuttleIncludeZ_9045_2x6_8(int iSht, int iWhichKit, bool bZDown=false);

// engine-dispatched pick entry point (defined in aoutarm9045_2x6_8.cpp; the
// engine aoutarm9045.cpp declares this as an extern for its DoPickFromShuttle
// dispatch ladder which routes iInArmType==e9045_2x6_8 here)
bool DoPickFromShuttle_9045_2x6_8(int iSht);
#endif
