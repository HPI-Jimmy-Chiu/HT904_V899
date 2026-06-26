// =============================================================================
//  ainarm9045_1x1_1.h  --  In-Arm per-site variant (single-site 1x1_1) header
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/ainarm9045_1x1_1.h (9 ln)
//  Translation wave: W6.2b1x1 (in-arm variant -- simplest single-site config)
//  Translator: AI(W6.2b-INARM-1x1_1) 20260626
//
//  FAITHFUL TRANSLATION -- the golden header exports exactly 3 symbols (the rest
//  of the .cpp functions are file-local / consumed via the engine's dispatch
//  ladder).  Names preserved verbatim for BCB6 reverse-lookup.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef ainarm9045_1x1_1H
#define ainarm9045_1x1_1H
//---------------------------------------------------------------------------

int GetShuttleState_1x1_1(int iSht, bool bPick);
void SetInOutArmParameter_1x1_1();
void SetInArmHasDropToShuttle_1x1_1(int SHT, int Row, int Col);
#endif
