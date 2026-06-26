//---------------------------------------------------------------------------
//  ainarm9045_2x2_8_Hot.h  --  in-arm 2x2_8_Hot per-site-config variant header
//
//  Translation wave: W6.2c batch-4 (in-arm per-site-config variant: 2x2_8_Hot,
//  2x2 8-picker Hot mode).
//  Translator: AI(W6.2c-2x2_8_Hot) 20260626
//  Golden source: ainarm9045_2x2_8_Hot.h (9 lines) -- mirror name + relative
//  path = ROOT.
//
//  FAITHFUL mirror of the golden header: the golden .h exports exactly
//  SetInOutArmParameter_2x2_8_Hot + the file-scope const array XPHSuckToSht_2x2_8.
//  The two engine-dispatch entry points DoInArm_9045_2x2_8_Hot() and
//  DoInArm_9045_2x2_8_Hot_SuckerMap() are added here so the engine's dispatch
//  ladder (ainarm9045.cpp) can extern + call them as unique symbols.  Every
//  other function in the .cpp is file-local / reached via the engine ladder;
//  names/signatures preserved VERBATIM for BCB6 reverse-lookup.
//---------------------------------------------------------------------------
#ifndef ainarm9045_2x2_8_HotH
#define ainarm9045_2x2_8_HotH
//---------------------------------------------------------------------------

void SetInOutArmParameter_2x2_8_Hot();
extern const int XPHSuckToSht_2x2_8[8];

// engine-dispatch entry points (unique symbols; externed by ainarm9045.cpp)
void DoInArm_9045_2x2_8_Hot();
void DoInArm_9045_2x2_8_Hot_SuckerMap();

#endif
