// =============================================================================
//  ainarm9045_1x2_2.h  --  In-Arm per-site variant (1x2_2 dual-shuttle) header
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/ainarm9045_1x2_2.h (10 ln)
//  Translation wave: W6.2c (in-arm variant batch -- 6 more layouts)
//  Translator: AI(W6.2c-INARM-1x2_2) 20260626
//
//  FAITHFUL TRANSLATION -- the golden header exports exactly 3 symbols (the rest
//  of the .cpp functions are file-local / consumed via the engine's dispatch
//  ladder).  Names preserved verbatim for BCB6 reverse-lookup.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef ainarm9045_1x2_2H
#define ainarm9045_1x2_2H
//---------------------------------------------------------------------------

void SetInOutArmParameter_1x2_2();
int GetShuttleState_1x2_2(int iSht, bool bPick);
void SetInArmHasDropToShuttle_1x2_2(int SHT, int Row, int Col);

#endif
