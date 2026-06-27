//---------------------------------------------------------------------------
// aoutarm9045_2x2_8.h -- Out-Arm per-site VARIANT (2x2_8) header.
// Faithful translation of golden aoutarm9045_2x2_8.h (BCB6, Big5/cp950).
// Translator: AI(W6.2c-OUT-2x2_8) 20260627
//
// Golden .h declared: DoOutArm_9045_2x2_8() and
//   MoveOutArmToShuttleIncludeZ_9045_2x2_8(int iSht, bool bZDown=false).
// The engine-dispatched DoPickFromShuttle_9045_2x2_8(int) is defined in the
// golden .cpp but was NOT declared in the golden .h; it is declared here so the
// out-arm engine (aoutarm9045.cpp) extern resolves to the real translated func.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x2_8H
#define aoutarm9045_2x2_8H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x2_8();
bool DoPickFromShuttle_9045_2x2_8(int iSht);
bool MoveOutArmToShuttleIncludeZ_9045_2x2_8(int iSht, bool bZDown=false);
#endif
