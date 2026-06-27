//---------------------------------------------------------------------------
//  aoutarm9045_2x2_4_23.h  --  out-arm 2x2_4_23 per-site-config variant header (DEAD)
//
//  Translation wave: W6.2c-OUT batch-4 (out-arm per-site-config variant: 2x2_4_23).
//  Translator: AI(W6.2c-2x2_4_23) 20260627
//  Golden source: aoutarm9045_2x2_4_23.h (8 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports
//  DoOutArm_9045_2x2_4_23(), SetOutArm_9045_2x2_4_23() and
//  MoveOutArmToShuttleIncludeZ_9045_2x2_4_23(int,bool=false) with their VERBATIM
//  signatures / default args.  The golden has NO DoPickFromShuttle_9045_2x2_4_23(int)
//  engine entry point (it uses the file-internal DoPickFromShuttle1/2_9045_2x2_4_23(bool)
//  helpers) -- so none is declared here.
//
//  DEAD VARIANT: enum e9045_2x2_4_23 is ABSENT from MachineType.h entirely, so no
//  dispatch arm in either out-arm engine ladder references this callee.  Every
//  DoOutArm_9045_2x2_4_23 / SetOutArm_9045_2x2_4_23 / *_2x2_4_23 symbol carries the
//  full _2x2_4_23 suffix -> UNIQUE never-called symbols that link as dead weight
//  (like in-arm ainarm9045_2x4_16.cpp / ainarm9045_2x8_32.cpp).
//
//  All names / values / signatures preserved VERBATIM so the team can
//  reverse-look-up the BCB6 version.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x2_4_23H
#define aoutarm9045_2x2_4_23H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x2_4_23();
bool SetOutArm_9045_2x2_4_23();
bool MoveOutArmToShuttleIncludeZ_9045_2x2_4_23(int iShuttle, bool bZDown=false); //kevin 20210612 Out arm add Z bZDown= true 往下
#endif
