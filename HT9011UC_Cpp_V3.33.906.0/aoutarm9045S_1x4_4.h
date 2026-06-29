//---------------------------------------------------------------------------
//  aoutarm9045S_1x4_4.h  --  out-arm S-PREFIX 1x4_4 per-site-config variant header
//
//  Translation wave: W6.2c batch-5 (out-arm per-site-config variant: S-prefix 1x4_4).
//  Translator: AI(W6.2c-9045S_1x4_4) 20260627
//  Golden source: aoutarm9045S_1x4_4.h (9 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports
//  DoOutArm_9045S_1x4_4(), SetOutArm_9045S_1x4_4(),
//  MoveOutArmToShuttleIncludeZ_9045S_1x4_4(int,bool=false) and
//  SetInOutArmParameter_1x4_4_4S() with their VERBATIM signatures / default args.
//
//  DEAD-BUT-COMPILED: DoOutArm_9045S_1x4_4() has ZERO external callers in the whole
//  golden tree (out-arm dispatch ladders route to the SUFFIX-S DoOutArm_9045_1x4_4S,
//  defined in the DIFFERENT, already-committed aoutarm9045_1x4_4S.cpp).  This file is
//  translated faithfully + added to CMake so it compiles+links; it is never dispatched.
//
//  NAMING NOTE: unlike most out-arm variants, this layout's TOP-LEVEL export and ALL
//  its helpers carry the irregular _9045S_1x4_4 infix (GetNowShuttleMode_9045S_1x4_4,
//  OutArmZNeedDown_9045S_1x4_4, CheckOutArmXYPitch_9045S_1x4_4,
//  MoveOutArmToShuttleIncludeZ_9045S_1x4_4, DoPickFromShuttle1_9045S_1x4_4,
//  DoPickFromShuttle2_9045S_1x4_4, DoMoveOutArmXYToPlace_9045S_1x4_4,
//  SearchUnLoadTrayUpDown_9045S_1x4_4, SetOutArm_9045S_1x4_4, DoPlaceToAuto_9045S_1x4_4,
//  GetOutShuttleStatus_9045S_1x4_4) -- preserved VERBATIM from the golden source.
//---------------------------------------------------------------------------
#ifndef aoutarm9045S_1x4_4H
#define aoutarm9045S_1x4_4H
//---------------------------------------------------------------------------
void DoOutArm_9045S_1x4_4();
bool SetOutArm_9045S_1x4_4();
bool MoveOutArmToShuttleIncludeZ_9045S_1x4_4(int iShuttle, bool bZDown=false);  //kevin 20210612 Out arm add Z bZDown= true 往下
void SetInOutArmParameter_1x4_4_4S();
#endif
