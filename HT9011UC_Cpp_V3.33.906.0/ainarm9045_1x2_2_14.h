//---------------------------------------------------------------------------
// ainarm9045_1x2_2_14.h -- faithful translation of golden
//   D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\ainarm9045_1x2_2_14.h
// Translator: AI(W6.2c-INARM-1x2_2_14) 20260626
// VERBATIM public surface: the golden .h exports exactly 2 symbols
//   (GetShuttleState_1x2_2_14 + SetInArmHasDropToShuttle_1x2_2_14).
// The two dispatch entry points DoInArm_9045_1x2_2_14 /
// DoInArm_9045_1x2_2_14_SuckerMap are forward-declared here so the engine
// dispatch ladder (ainarm9045.cpp) can call them once Integrate un-gates the
// e9045_1x2_2_14 arm; their bodies live in ainarm9045_1x2_2_14.cpp.
// All other functions in the .cpp are file-local / reached via the engine;
// names/signatures preserved VERBATIM for BCB6 reverse-lookup.
//---------------------------------------------------------------------------
#ifndef ainarm9045_1x2_2_14H
#define ainarm9045_1x2_2_14H
//---------------------------------------------------------------------------

int GetShuttleState_1x2_2_14(int iSht, bool bPick);
void SetInArmHasDropToShuttle_1x2_2_14(int SHT, int Row, int Col);

void DoInArm_9045_1x2_2_14();
void DoInArm_9045_1x2_2_14_SuckerMap();
#endif
