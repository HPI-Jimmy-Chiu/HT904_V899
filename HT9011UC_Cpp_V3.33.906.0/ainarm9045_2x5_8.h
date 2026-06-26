//---------------------------------------------------------------------------
//  ainarm9045_2x5_8.h  --  in-arm 2x5_8 per-site-config variant header
//
//  Translation wave: W6.2c batch-4 (in-arm per-site-config variant: 2x5_8)
//  Translator: AI(W6.2c-2x5_8) 20260626
//  Golden source: ainarm9045_2x5_8.h (9 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header: the golden .h exports exactly 3 symbols
//  (GetShuttleState_2x5_8 + SetInOutArmParameter_2x5_8 + the file-owned const
//  XPHSuckToSht_2x5_8[3][8] table).  The remaining functions in the .cpp are
//  file-local / reached via the engine's DoInArm dispatch ladder; names/signatures
//  preserved VERBATIM for BCB6 reverse-lookup.  The two engine-dispatched entry
//  points DoInArm_9045_2x5_8 / DoInArm_9045_2x5_8_SuckerMap are declared here so
//  the Integrate phase can extern+wire them (matches committed sibling pattern).
//---------------------------------------------------------------------------
#ifndef ainarm9045_2x5_8H
#define ainarm9045_2x5_8H
//---------------------------------------------------------------------------

int GetShuttleState_2x5_8(int iSht, bool bPick);
void SetInOutArmParameter_2x5_8();
extern const int XPHSuckToSht_2x5_8[3][8];

// engine dispatch entry points (golden ainarm9045.cpp externs :4412 / :4672)
void DoInArm_9045_2x5_8();
void DoInArm_9045_2x5_8_SuckerMap();

#endif
