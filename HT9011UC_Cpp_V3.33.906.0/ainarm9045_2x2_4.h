//---------------------------------------------------------------------------
//  ainarm9045_2x2_4.h  --  in-arm 2x2_4 per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 2x2_4, dual-row 2x2)
//  Translator: AI(W6.2c-2x2_4) 20260626
//  Golden source: ainarm9045_2x2_4.h (10 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports the helper symbols
//  below (SetInOutArmParameter_2x2_4 / GetShuttleState_2x2_4 / the XPHSuckToSht_2x2_13
//  array extern / SetInArmHasDropToShuttle_2x2_4) verbatim.  The array is DEFINED in
//  this translation unit's .cpp; the extern here matches the golden declaration.
//
//  The engine (ainarm9045.cpp) reaches DoInArm_9045_2x2_4 / DoInArm_9045_2x2_4_SuckerMap
//  through its dispatch-ladder externs, so -- like the golden header -- they are NOT
//  declared here; they remain file-local definitions in the .cpp.  Names/signatures
//  preserved VERBATIM for BCB6 reverse-lookup.
//---------------------------------------------------------------------------
#ifndef ainarm9045_2x2_4H
#define ainarm9045_2x2_4H
//---------------------------------------------------------------------------

void SetInOutArmParameter_2x2_4();
int GetShuttleState_2x2_4(int iSht, bool bPick);
extern const int XPHSuckToSht_2x2_13[3][8];
void SetInArmHasDropToShuttle_2x2_4(int SHT, int Row, int Col);

#endif
