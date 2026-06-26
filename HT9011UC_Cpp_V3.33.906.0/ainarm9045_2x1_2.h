// =============================================================================
//  ainarm9045_2x1_2.h  --  In-Arm per-site variant (2x1_2 dual-row) header
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/ainarm9045_2x1_2.h (8 ln)
//  Translation wave: W6.2c (in-arm variant batch -- 6 more layouts)
//  Translator: AI(W6.2c-INARM-2x1_2) 20260626
//
//  FAITHFUL TRANSLATION -- the golden header exports exactly 2 symbols
//  (SetInOutArmParameter_2x1_2 / SetInArmHasDropToShuttle_2x1_2); the rest of the
//  .cpp functions are file-local / consumed via the engine's dispatch ladder.
//  Names preserved verbatim for BCB6 reverse-lookup.  The two engine dispatch
//  symbols DoInArm_9045_2x1_2() / DoInArm_9045_2x1_2_SuckerMap() are declared
//  here so the serial Integrate stage can extern them into the ladder; golden
//  keeps them file-local, but the engine consumes them by name.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef ainarm9045_2x1_2H
#define ainarm9045_2x1_2H
//---------------------------------------------------------------------------

void SetInOutArmParameter_2x1_2();
void SetInArmHasDropToShuttle_2x1_2(int SHT, int Row, int Col);

// engine dispatch symbols (golden keeps file-local; declared for the ladder)
void DoInArm_9045_2x1_2();
void DoInArm_9045_2x1_2_SuckerMap();

#endif
