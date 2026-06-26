//---------------------------------------------------------------------------
//  ainarm9045_All_1Pick.h  --  in-arm All_1Pick (single-picker / ep1Picker)
//                              per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: All_1Pick).
//  Translator: AI(W6.2c-All_1Pick) 20260626
//  Golden source: ainarm9045_All_1Pick.h (10 lines) -- mirror name + relative
//                 path = ROOT.
//
//  FAITHFUL mirror of the golden header: the golden .h exports exactly these 4
//  symbols (SetInOutArmParameter_All_1Pick + GetNowSiteKitMode_All_1Pick +
//  GetNowInShuttleRowCol_All_1Picker + InArmZNeedDown_All_1Pick).  The remaining
//  functions in the .cpp are file-local / reached via the engine's DoInArm
//  dispatch ladder; names/signatures preserved VERBATIM for BCB6 reverse-lookup.
//
//  Engine-facing dispatch symbols (declared here additionally so the engine's
//  externs in ainarm9045.cpp resolve at link):
//      DoInArm_9045_All_1Pick()           -- the ep1Picker FIRST ladder arm.
//      DoInArm_9045_All_1Pick_SuckerMap() -- DEFINED but the golden engine NEVER
//                                            dispatches it (faithful); declared so
//                                            the TU symbol resolves at link.
//---------------------------------------------------------------------------
#ifndef ainarm9045_All_1PickH
#define ainarm9045_All_1PickH
//---------------------------------------------------------------------------
void SetInOutArmParameter_All_1Pick();
extern int GetNowSiteKitMode_All_1Pick(int iSht, bool bPlace);
extern bool GetNowInShuttleRowCol_All_1Picker(int iSht, int *iUseRow, int *iUseCol, bool bPick);
extern void InArmZNeedDown_All_1Pick(int iSht, bool bPlace);

void DoInArm_9045_All_1Pick();
void DoInArm_9045_All_1Pick_SuckerMap();
#endif
