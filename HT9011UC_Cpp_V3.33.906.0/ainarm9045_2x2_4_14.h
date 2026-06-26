// =============================================================================
//  ainarm9045_2x2_4_14.h  --  In-Arm per-site variant (2x2_4_14 dual-row) header
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/ainarm9045_2x2_4_14.h (9 ln)
//  Translation wave: W6.2c (in-arm variant batch-2 -- 6 more layouts)
//  Translator: AI(W6.2c-INARM-2x2_4_14) 20260626
//
//  FAITHFUL TRANSLATION -- the golden header exports exactly 3 symbols (the rest
//  of the .cpp functions are file-local / consumed via the engine's dispatch
//  ladder).  Names preserved verbatim for BCB6 reverse-lookup.  The const array
//  XPHSuckToSht_2x2_14 is DEFINED in the .cpp (own translation unit) and only
//  EXTERN-declared here, exactly as golden.
//
//  The two engine-dispatch symbols DoInArm_9045_2x2_4_14() /
//  DoInArm_9045_2x2_4_14_SuckerMap() are file-local in golden; we keep them
//  file-local (the engine consumes them through its ladder externs) and do NOT
//  redeclare them here -- matching the committed sibling exemplars.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef ainarm9045_2x2_4_14H
#define ainarm9045_2x2_4_14H
//---------------------------------------------------------------------------

int GetShuttleState_2x2_4_14(int iSht, bool bPick);
extern const int XPHSuckToSht_2x2_14[3][8];
void SetInArmHasDropToShuttle_2x2_4_14(int SHT, int Row, int Col);
#endif
