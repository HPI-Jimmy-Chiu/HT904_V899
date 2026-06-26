//---------------------------------------------------------------------------
//  ainarm9045_1x4_8_Hot.h  --  in-arm 1x4_8_Hot per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 1x4_8_Hot,
//  1x4 layout, 8-picker high-temperature / HotPlate flow)
//  Translator: AI(W6.2c-INARM-batch2) 20260626
//  Golden source: ainarm9045_1x4_8_Hot.h (8 lines) -- mirror name + relative
//  path = ROOT
//
//  FAITHFUL mirror of the golden header: the golden .h exports exactly 1 symbol
//  (SetInOutArmParameter_1x4_8_Hot).  The remaining functions in the .cpp are
//  file-local / reached via the engine's DoInArm dispatch ladder; names/
//  signatures preserved VERBATIM for BCB6 reverse-lookup.  The two engine-
//  consumed dispatch entries DoInArm_9045_1x4_8_Hot() and
//  DoInArm_9045_1x4_8_Hot_SuckerMap() are declared here so the engine's extern
//  ladder can bind them (golden keeps them file-local; declared here for the
//  C++ build's cross-TU dispatch).
//---------------------------------------------------------------------------
#ifndef ainarm9045_1x4_8_HotH
#define ainarm9045_1x4_8_HotH
//---------------------------------------------------------------------------

void SetInOutArmParameter_1x4_8_Hot();

void DoInArm_9045_1x4_8_Hot();
void DoInArm_9045_1x4_8_Hot_SuckerMap();

#endif
