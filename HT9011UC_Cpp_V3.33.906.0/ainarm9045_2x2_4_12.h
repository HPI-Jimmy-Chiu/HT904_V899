//---------------------------------------------------------------------------
//  ainarm9045_2x2_4_12.h  --  in-arm 2x2_4_12 per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 2x2_4_12, 2x2 dual-row)
//  Translator: AI(W6.2c-2x2_4_12) 20260626
//  Golden source: ainarm9045_2x2_4_12.h (7 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header: the golden .h exports exactly 1 symbol
//  (the file-local const array XPHSuckToSht_2x2_12[8], DEFINED in this .cpp's
//  translation unit @golden :32).  The variant's per-site state machines
//  (GetNowSiteKitMode_2x2_4_12 / InArmZNeedDown_2x2_4_12 / CheckXYPitch_2x2_4_12 /
//  MoveInArm2XYToShuttle_9045_2x2_4_12 / DoPlaceToHotPlate_9045_2x2_4_12 /
//  DoInArmPlaceToShuttle_9045_2x2_4_12 / DoInArm_9045_2x2_4_12_SuckerMap /
//  DoInArm_9045_2x2_4_12) are file-local in the golden and reached via the
//  engine's DoInArm dispatch ladder; names/signatures preserved VERBATIM for
//  BCB6 reverse-lookup.  The two dispatch entry points DoInArm_9045_2x2_4_12()
//  and DoInArm_9045_2x2_4_12_SuckerMap() are declared here so the engine's
//  ladder externs resolve at link time (golden keeps them file-local; the
//  W6.2c integrate ladder consumes them via its own externs, so these two
//  declarations are additive-only and do not change golden behaviour).
//---------------------------------------------------------------------------
#ifndef ainarm9045_2x2_4_12H
#define ainarm9045_2x2_4_12H
//---------------------------------------------------------------------------

extern const int XPHSuckToSht_2x2_12[8];                                        //golden :32 (file-local const, DEFINED in the .cpp)

void DoInArm_9045_2x2_4_12();                                                    //golden :1524 (engine dispatch entry)
void DoInArm_9045_2x2_4_12_SuckerMap();                                         //golden :1499 (engine sucker-map dispatch entry)

#endif
