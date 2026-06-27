//---------------------------------------------------------------------------
//  aoutarm9045_2x8_32.h  --  out-arm 2x8_32 per-site-config variant header (DEAD)
//
//  Translation wave: W6.2c-OUT batch-4 (DEAD) (out-arm per-site-config variant: 2x8_32).
//  Translator: AI(W6.2c-OUT-2x8_32) 20260627
//  Golden source: aoutarm9045_2x8_32.h (8 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports
//    void DoOutArm_9045_2x8_32();
//    bool SetOutArm_9045_2x8_32();
//    bool MoveOutArmToShuttleIncludeZ_9045_2x8_32(int iShuttle, int iWhichKit, bool bZDown=false);
//  with their VERBATIM signatures / default args.  NOTE the 3-parameter
//  MoveOutArmToShuttleIncludeZ_9045_2x8_32 (extra int iWhichKit vs the 2-parameter
//  sibling exports) -- kept verbatim, it is UNIQUE (no clash).
//
//  DEAD VARIANT: enum e9045_2x8_32 is PRESENT in MachineType.h but BOTH out-arm
//  dispatch ladders route it to the LIVE 2x8_8 callee (DoOutArm line e9045_2x8_8||
//  e9045_2x8_32 -> DoOutArm_9045_2x8_8 ; DoPickFromShuttle -> DoPickFromShuttle_9045_2x8_8).
//  So DoOutArm_9045_2x8_32 and the _2x8_32 helpers are NEVER dispatched -- they link as
//  unique dead-weight symbols.  There is NO DoPickFromShuttle_9045_2x8_32(int) in the
//  golden .h (the file uses internal DoPickFromShuttle1/2_9045_2x8_32(bool)); accordingly
//  this header declares ONLY the 3 golden exports and adds NO engine extern / dispatch arm.
//  All names / values / signatures preserved VERBATIM for BCB6 reverse-look-up.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x8_32H
#define aoutarm9045_2x8_32H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x8_32();
bool SetOutArm_9045_2x8_32();
bool MoveOutArmToShuttleIncludeZ_9045_2x8_32(int iShuttle, int iWhichKit, bool bZDown=false);                           //kevin 20210602 Out arm add Z bZDown= true 往下
#endif
