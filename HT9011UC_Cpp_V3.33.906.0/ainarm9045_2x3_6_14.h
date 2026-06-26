//---------------------------------------------------------------------------
//  ainarm9045_2x3_6_14.h  --  in-arm 2x3_6_14 per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 2x3_6_14, 2x3 two-row)
//  Translator: AI(W6.2c-2x3_6_14) 20260626
//  Golden source: ainarm9045_2x3_6_14.h (mirror name + relative path = ROOT)
//
//  FAITHFUL mirror of the golden header.  Names/signatures preserved VERBATIM for
//  BCB6 reverse-lookup.  The golden engine dispatch routes iInArmType==e9045_2x3_6_14
//  to DoInArm_9045_2x3_6_14() / DoInArm_9045_2x3_6_14_SuckerMap() (defined in the .cpp).
//
//  DEVIATION (forced-correctness, numeric semantics UNCHANGED):
//    Golden line `extern XPHSuckToSht_2x3_14_OutArm[4][8];` relies on BCB6 implicit-int
//    (JerryYang 20251124).  Standard C++17 has no implicit int, so the declaration is
//    made well-formed as `extern int XPHSuckToSht_2x3_14_OutArm[4][8];` (the .cpp
//    defines it as `int ...[4][8]={...}` -- the array TYPE/VALUES are unchanged).
//---------------------------------------------------------------------------
#ifndef ainarm9045_2x3_6_14H
#define ainarm9045_2x3_6_14H
//---------------------------------------------------------------------------

int GetShuttleState_2x3_6_14(int iSht, bool bPick);
void SetInOutArmParameter_2x3_6_14();
extern const int XPHSuckToSht_2x3_14[4][8];
extern int XPHSuckToSht_2x3_14_OutArm[4][8];                                    //JerryYang 20251124 : add  //AI(W6.2c-2x3_6_14) 20260626: add 'int' (golden BCB6 implicit-int -> well-formed C++17; values unchanged)

// Engine dispatch callees (defined in this .cpp; declared here for the engine extern block).
void DoInArm_9045_2x3_6_14();
void DoInArm_9045_2x3_6_14_SuckerMap();

#endif
