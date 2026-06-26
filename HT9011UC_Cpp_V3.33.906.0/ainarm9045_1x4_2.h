//---------------------------------------------------------------------------
//  ainarm9045_1x4_2.h  --  in-arm 1x4_2 per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 1x4_2 / e9045_1x4_2_14)
//  Translator: AI(W6.2c-INARM-1x4_2) 20260626
//  Golden source: ainarm9045_1x4_2.h (9 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header: the golden .h exports exactly 4 symbols
//  (GetShuttleState_1x4_2_14 / SetInOutArmParameter_1x4_2 / GetNeedSuckActive_1x4_2_14
//  / SetInArmHasDropToShuttle_1x4_2), transcribed VERBATIM below.  The remaining
//  functions in the .cpp are file-local / reached via the engine's DoInArm
//  dispatch ladder; the two engine dispatch symbols DoInArm_9045_1x4_2 /
//  DoInArm_9045_1x4_2_SuckerMap are also declared here (the engine references them
//  through ladder externs in ainarm9045.cpp).  Names/signatures preserved VERBATIM
//  for BCB6 reverse-lookup.
//---------------------------------------------------------------------------
#ifndef ainarm9045_1x4_2H
#define ainarm9045_1x4_2H
//---------------------------------------------------------------------------

int GetShuttleState_1x4_2_14(int iSht, bool bPick);
void SetInOutArmParameter_1x4_2();
int GetNeedSuckActive_1x4_2_14(int iSht, bool bPick);
void SetInArmHasDropToShuttle_1x4_2(int SHT,int Row,int Col);

// Engine dispatch symbols (definitions in ainarm9045_1x4_2.cpp):
void DoInArm_9045_1x4_2();
void DoInArm_9045_1x4_2_SuckerMap();

#endif
