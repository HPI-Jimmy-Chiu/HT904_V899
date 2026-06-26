//---------------------------------------------------------------------------
//  ainarm9045_2x3_6.h  --  in-arm 2x3_6 per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 2x3_6, 6-site 2x3)
//  Translator: AI(W6.2c-2x3_6) 20260626
//  Golden source: ainarm9045_2x3_6.h (10 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header: the golden .h exports GetShuttleState_2x3_6,
//  SetInOutArmParameter_2x3_6 and the two const tables XPHSuckToSht_2x3_6[3][8] /
//  XPHSuckToSht_2x3_6_OutArm[3][8] (defined in the .cpp).  In addition the two
//  engine-dispatched entry points DoInArm_9045_2x3_6() / DoInArm_9045_2x3_6_SuckerMap()
//  are declared here (the golden engine declares them as externs; declaring them in
//  this variant's own header keeps the symbols self-documenting for BCB6 reverse-
//  lookup).  All names/signatures preserved VERBATIM.
//---------------------------------------------------------------------------
#ifndef ainarm9045_2x3_6H
#define ainarm9045_2x3_6H
//---------------------------------------------------------------------------

int GetShuttleState_2x3_6(int iSht, bool bPick);
void SetInOutArmParameter_2x3_6();
extern const int XPHSuckToSht_2x3_6[3][8];
extern const int XPHSuckToSht_2x3_6_OutArm[3][8];

// engine-dispatched entry points (defined in ainarm9045_2x3_6.cpp; the engine
// ainarm9045.cpp also declares these as externs for its DoInArm dispatch ladder)
void DoInArm_9045_2x3_6();
void DoInArm_9045_2x3_6_SuckerMap();

#endif
