//---------------------------------------------------------------------------
//  ainarm9045_2x4_4.h  --  in-arm 2x4_4 per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 2x4_4).
//  Translator: AI(W6.2c-2x4_4) 20260626
//  Golden source: ainarm9045_2x4_4.h (8 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header: the golden .h exports exactly
//  GetShuttleState_2x4_4, SetInOutArmParameter_2x4_4 and the file-scope const
//  table XPHSuckToSht_2x4_14.  The two engine-dispatched entry points
//  DoInArm_9045_2x4_4() / DoInArm_9045_2x4_4_SuckerMap() are declared here as well
//  so the engine ainarm9045.cpp can extern them (the golden engine declares them
//  via its own extern block; we additionally expose them in this header for the
//  per-variant TU symbol contract).  All names/signatures preserved VERBATIM for
//  BCB6 reverse-lookup.
//---------------------------------------------------------------------------
#ifndef ainarm9045_2x4_4H
#define ainarm9045_2x4_4H
//---------------------------------------------------------------------------

int GetShuttleState_2x4_4(int iSht, bool bPick);
void SetInOutArmParameter_2x4_4();
extern const int XPHSuckToSht_2x4_14[3][8];

void DoInArm_9045_2x4_4();
void DoInArm_9045_2x4_4_SuckerMap();

#endif
