// =============================================================================
//  ainarm9045_1x3_2_14.h  --  In-Arm per-site variant (1x3_2_14) public interface
//
//  Faithful translation of golden ainarm9045_1x3_2_14.h (BCB6, Big5/cp950).
//  Golden: HT9011UC_Code_V3.33.906.0_20260618/ainarm9045_1x3_2_14.h (11 ln)
//  Translator: AI(W6.2c-INARM-1x3_2_14) 20260626
//
//  FAITHFUL TRANSLATION -- the golden header exports exactly these 4 symbols
//  (the remaining .cpp functions are file-local / consumed via the engine's
//  dispatch ladder DoInArm_9045()/SuckerMap()).  Names preserved verbatim for
//  BCB6 reverse-lookup.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef ainarm9045_1x3_2_14H
#define ainarm9045_1x3_2_14H
//---------------------------------------------------------------------------

int GetShuttleState_1x3_2_14(int iSht, bool bPick);
void SetInOutArmParameter_1x3_2_14();
void CheckCloseSiteMode_1x3_14();                                               //Steven 20251113 : for 1x3 close site
void CheckCloseSiteMode_2x3N_14();

#endif
