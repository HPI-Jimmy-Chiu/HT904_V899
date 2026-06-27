//---------------------------------------------------------------------------
//  aoutarm9045_1x2_2.h  --  out-arm 1x2_2 per-site-config variant header
//
//  Translation wave: W6.2c (out-arm per-site-config variant: 1x2_2, single-row
//  2-picker, "倆倆放" left/right two-shot shuttle pick).
//  Translator: AI(W6.2c-out-1x2_2) 20260627
//  Golden source: aoutarm9045_1x2_2.h (7 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  Names / signatures / default args
//  preserved VERBATIM so the team can reverse-look-up the BCB6 version.  The two
//  engine-dispatched entry points DoOutArm_9045_1x2_2() /
//  DoPickFromShuttle_9045_1x2_2(int) are the LIVE callees the out-arm engine
//  (aoutarm9045.cpp) routes iInArmType==e9045_1x2_2_13 / e9045_1x2_2_14 to.
//  MoveOutArmToShuttleIncludeZ_9045_1x2_2() carries the golden default arg
//  (bZDown=false) VERBATIM.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_1x2_2H
#define aoutarm9045_1x2_2H
//---------------------------------------------------------------------------
void DoOutArm_9045_1x2_2();
bool MoveOutArmToShuttleIncludeZ_9045_1x2_2(int iSht, bool bZDown=false);
#endif
