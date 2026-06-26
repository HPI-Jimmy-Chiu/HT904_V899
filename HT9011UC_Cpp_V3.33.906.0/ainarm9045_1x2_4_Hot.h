//---------------------------------------------------------------------------
//  ainarm9045_1x2_4_Hot.h  --  in-arm 1x2_4_Hot per-site-config variant
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 1x2_4_Hot)
//  Translator: AI(W6.2c-1x2_4_Hot) 20260626
//  Golden source: ainarm9045_1x2_4_Hot.h (mirror name + relative path = ROOT)
//
//  Faithful mirror of the golden header.  The golden .h declares only
//  SetInOutArmParameter_1x2_4_Hot() and the file-local-but-extern lookup table
//  XPHSuckToSht_1x2_4 (read by the engine ainarm9045.cpp dispatch).  The golden
//  .cpp additionally defines the per-layout in-arm state machines + helpers that
//  the engine dispatch ladder (DoInArm_9045 / DoInArm_9045_SuckerMap) routes to;
//  we add their prototypes here too so the engine + integrate stage can see them.
//  Names/signatures preserved VERBATIM for reverse lookup.
//---------------------------------------------------------------------------
#ifndef ainarm9045_1x2_4_HotH
#define ainarm9045_1x2_4_HotH
//---------------------------------------------------------------------------

void SetInOutArmParameter_1x2_4_Hot();
extern const int XPHSuckToSht_1x2_4[2][2][8];

// Defined in ainarm9045_1x2_4_Hot.cpp (golden file declares only the two above;
// these are routed to by the engine dispatch ladder + the integrate stage):
int  GetNowSiteKitMode_1x2_4_Hot(int iSht, bool bPlace);
void InArmZNeedDown_1x2_4_Hot(int iSht, bool bPlace);
void CheckXYPitch_1x2_4_Hot(int *iX, int *iY, int iSht, bool bPlace, int iMovePitchX, int iMovePitchY);
bool MoveInArm2XYToShuttle_9045_1x2_4_Hot(int iSht, bool IncludeZ, bool bPlace);
bool DoPlaceToHotPlate_9045_1x2_4_Hot();
bool DoInArmPlaceToShuttle_9045_1x2_4_Hot();
void DoInArm_9045_1x2_4_Hot_SuckerMap();
void DoInArm_9045_1x2_4_Hot();
bool DoInArmTryPickFromHotPlate_9045_1x2_4_Hot(bool bOneTimeCheckAll, bool bShowErrorMessage);

#endif
