//---------------------------------------------------------------------------
// aoutarm9045_1x4_4_Back.h -- Out-Arm per-site VARIANT (1x4_4_Back) header.
// Faithful translation of golden aoutarm9045_1x4_4_Back.h (BCB6, Big5/cp950).
// Translator: AI(W6.2c-OUT-1x4_4_Back) 20260627
// DEAD variant: enum e9045_1x4_4_Back is present but the engine DoOutArm ladder
// arm is an EMPTY //-body and the DoPickFromShuttle arm routes to the LIVE
// DoPickFromShuttle_9045_1x4_4; so these callees are never dispatched (dead
// weight, link-only). Mirrors golden .h exactly (3 decls, NO DoPickFromShuttle
// extern -- the pick is via file-internal DoPickFromShuttle1/2 helpers).
//---------------------------------------------------------------------------
#ifndef aoutarm9045_1x4_4_BackH
#define aoutarm9045_1x4_4_BackH
//---------------------------------------------------------------------------
void DoOutArm_9045_1x4_4_Back();
bool SetOutArm_9045_1x4_4_Back();
bool MoveOutArmToShuttleIncludeZ_9045_1x4_4_Back(int iShuttle, bool bZDown=false);                                      //kevin 20210612 Out arm add Z bZDown= true 往下
#endif
