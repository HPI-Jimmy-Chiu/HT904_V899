//---------------------------------------------------------------------------
//  aoutarm9045_2x2_4_14.h  --  out-arm 2x2_4_14 per-site-config variant header
//
//  Translation wave: W6.2c-OUT batch-4 (DEAD variant: 2x2_4_14).
//  Translator: AI(W6.2c-OUT-2x2_4_14) 20260627
//  Golden source: aoutarm9045_2x2_4_14.h (8 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports exactly three
//  symbols (VERBATIM signatures / default args):
//    void DoOutArm_9045_2x2_4_14();
//    bool SetOutArm_9045_2x2_4_14();
//    bool MoveOutArmToShuttleIncludeZ_9045_2x2_4_14(int iShuttle, bool bZDown=false);
//
//  DEAD VARIANT: the out-arm engine routes enum e9045_2x2_4_14 to the LIVE 2x2_4
//  callee in BOTH dispatch ladders (DoOutArm_9045_2x2_4 / DoPickFromShuttle_9045_2x2_4);
//  the _2x2_4_14 callees here are NEVER dispatched.  They carry the full _2x2_4_14
//  suffix so every symbol is UNIQUE and links as dead weight (no engine extern,
//  no dispatch arm).  The golden .h declares NO DoPickFromShuttle_9045_2x2_4_14(int)
//  -- the variant uses file-internal DoPickFromShuttle1/2_9045_2x2_4_14(bool)
//  helpers -- so this header does NOT declare one either (faithful mirror).
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x2_4_14H
#define aoutarm9045_2x2_4_14H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x2_4_14();
bool SetOutArm_9045_2x2_4_14();
bool MoveOutArmToShuttleIncludeZ_9045_2x2_4_14(int iShuttle, bool bZDown=false);    //kevin 20210612 Out arm add Z bZDown= true 往下
#endif
