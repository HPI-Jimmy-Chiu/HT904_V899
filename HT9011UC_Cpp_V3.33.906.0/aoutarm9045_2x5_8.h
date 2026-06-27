//---------------------------------------------------------------------------
// aoutarm9045_2x5_8.h  --  Out-Arm per-site VARIANT (2x5_8, two-row 5-col 8-pick)
//
// Faithful translation of golden aoutarm9045_2x5_8.h (BCB6, Big5/cp950).
// Translator: AI(W6.2c-2x5_8) 20260627  (translation wave W6.2c-OUT batch-3 final).
//
// Mirrors the golden .h declarations VERBATIM (golden aoutarm9045_2x5_8.h, 7 lines):
//   void DoOutArm_9045_2x5_8();
//   bool MoveOutArmToShuttleIncludeZ_9045_2x5_8(int iSht, int iWhichKit, bool bZDown=false);
//
// NOTE: the golden .h declares only those two; DoPickFromShuttle_9045_2x5_8(int)
// is defined in the .cpp and called by the out-arm engine ladder, so it is
// declared here as well to match the engine's active extern (aoutarm9045.cpp).
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x5_8H
#define aoutarm9045_2x5_8H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x5_8();
bool DoPickFromShuttle_9045_2x5_8(int iSht);
bool MoveOutArmToShuttleIncludeZ_9045_2x5_8(int iSht, int iWhichKit, bool bZDown=false);
#endif
