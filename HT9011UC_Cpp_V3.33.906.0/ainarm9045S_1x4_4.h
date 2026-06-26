//---------------------------------------------------------------------------
//  ainarm9045S_1x4_4.h  --  in-arm 1x4_4_13 (HT-9045S AutoClean) per-site variant header
//
//  Translation wave: W6.2c batch-5 (S-family HT-9045S in-arm variant: 1x4_4_13)
//  Translator: AI(W6.2c-INARM-batch5) 20260626
//  Golden source: ainarm9045S_1x4_4.h (8 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header (declares ONLY SetInOutArmParameter_1x4_4_13
//  and the file-scope const map XPHSuckToSht_1x4_13[2][8]).  The golden .h does NOT
//  declare the engine-dispatched entry points DoInArm_9045S_1x4_4 /
//  DoInArm_9045S_1x4_4_SuckerMap -- those are consumed via the engine externs in
//  ainarm9045.cpp.  IRREGULAR func name: this file defines DoInArm_9045S_1x4_4
//  (S in func) though the enum is e9045_1x4_4_13.  All names/values preserved VERBATIM.
//---------------------------------------------------------------------------
#ifndef ainarm9045S_1x4_4H
#define ainarm9045S_1x4_4H
//---------------------------------------------------------------------------

void SetInOutArmParameter_1x4_4_13();
extern const int XPHSuckToSht_1x4_13[2][8];
#endif
