//---------------------------------------------------------------------------
//  ainarm9045_1x4_4_Back.h  --  in-arm 1x4_4_Back per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 1x4_4_Back; the
//  back-row variant of the 1x4_4 layout -- single physical InArm row, places to
//  Shuttle row 1, uses the InArmSuck Item[1][0..3] columns).
//  Translator: AI(W6.2c-1x4_4_Back) 20260626
//  Golden source: ainarm9045_1x4_4_Back.h (8 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header: the golden .h exports exactly 1 symbol
//  (SetShuttleStatus_1x4_4_Back -- declared in golden but NEVER defined or called
//  anywhere in the golden codebase; carried verbatim for BCB6 reverse-lookup).
//  The two engine-dispatched entry points DoInArm_9045_1x4_4_Back() /
//  DoInArm_9045_1x4_4_Back_SuckerMap() are added here so the externs declared in
//  the engine ainarm9045.cpp resolve to this TU; their definitions live in the .cpp.
//  All other functions in the .cpp are file-local / reached via the engine's
//  DoInArm dispatch ladder; names/signatures preserved VERBATIM.
//---------------------------------------------------------------------------
#ifndef ainarm9045_1x4_4_BackH
#define ainarm9045_1x4_4_BackH
//---------------------------------------------------------------------------

void SetShuttleStatus_1x4_4_Back(int iSht,int Row,int Col);

// AI(W6.2c-1x4_4_Back) 20260626: engine-dispatched entry points (defined in .cpp;
// declared here so the live externs in ainarm9045.cpp resolve to this TU).
void DoInArm_9045_1x4_4_Back();
void DoInArm_9045_1x4_4_Back_SuckerMap();

#endif
