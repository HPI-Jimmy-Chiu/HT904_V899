//---------------------------------------------------------------------------
//  ainarm9045_1x3_4.h  --  in-arm 1x3_4 per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 1x3_4, TSMC 1x3 Site)
//  Translator: AI(W6.2c-1x3_4) 20260626
//  Golden source: ainarm9045_1x3_4.h (9 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header: the golden .h exports exactly 2 symbols
//  (GetShuttleState_1x3_4 + SetInOutArmParameter_1x3_4).  The remaining functions
//  in the .cpp are file-local / reached via the engine's DoInArm dispatch ladder;
//  names/signatures preserved VERBATIM for BCB6 reverse-lookup.
//---------------------------------------------------------------------------
#ifndef ainarm9045_1x3_4H
#define ainarm9045_1x3_4H
//---------------------------------------------------------------------------

int GetShuttleState_1x3_4(int iSht, bool bPick);
void SetInOutArmParameter_1x3_4();

#endif
