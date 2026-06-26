// =============================================================================
//  ainarm9045_2x4_8.h  --  In-Arm per-site variant (2x4_8 dual-shuttle) header
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/ainarm9045_2x4_8.h (10 ln)
//  Translation wave: W6.2c batch-4 (in-arm variant batch: 2x4_8)
//  Translator: AI(W6.2c-2x4_8) 20260626
//
//  FAITHFUL TRANSLATION -- the golden header exports exactly 4 symbols
//  (SetInOutArmParameter_2x4_8 / GetShuttleState_2x4_8 / XPHSuckToSht_2x4_8 /
//  SetInArmHasDropToShuttle_2x4_8).  The two engine-dispatch entry points
//  DoInArm_9045_2x4_8() and DoInArm_9045_2x4_8_SuckerMap() are declared here as
//  well so the engine's dispatch ladder can extern + call them at Integrate.
//  The remaining .cpp functions are file-local / consumed via the engine's
//  dispatch ladder.  Names/signatures preserved VERBATIM for BCB6 reverse-lookup.
//  XPHSuckToSht_2x4_8[3][8] is a file-defined const table exported here.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef ainarm9045_2x4_8H
#define ainarm9045_2x4_8H
//---------------------------------------------------------------------------

void SetInOutArmParameter_2x4_8();
int GetShuttleState_2x4_8(int iSht, bool bPick);
extern const int XPHSuckToSht_2x4_8[3][8];
void SetInArmHasDropToShuttle_2x4_8(int SHT, int Row, int Col);

//  engine dispatch entry points (DoInArm_9045 / DoInArm_9045_SuckerMap ladders)
void DoInArm_9045_2x4_8();
void DoInArm_9045_2x4_8_SuckerMap();

#endif
