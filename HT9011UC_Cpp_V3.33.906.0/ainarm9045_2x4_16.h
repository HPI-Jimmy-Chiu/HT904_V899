//---------------------------------------------------------------------------
//  ainarm9045_2x4_16.h  --  in-arm 16-site (2x4_16) per-site-config variant
//
//  Translation wave: W6.2b (in-arm per-site-config variant: 2x4_16, 16-site)
//  Translator: AI(W6.2b-2x4_16) 20260626
//  Golden source: ainarm9045_2x4_16.h (mirror name + relative path = ROOT)
//
//  Faithful mirror of the golden header.  The golden .h declares 16 prototypes;
//  the golden .cpp additionally defines DoInArmPickFromHotPlate_9045_2x4_16 and
//  InArmHasICForSht_2x4_16 (used by the engine dispatch / sibling code), so we
//  add them here too.  Names/signatures preserved VERBATIM for reverse lookup.
//---------------------------------------------------------------------------
#ifndef ainarm9045_2x4_16H
#define ainarm9045_2x4_16H
//---------------------------------------------------------------------------
void DoInArm_9045_2x4_16();
void DoInArm_9045_2x4_16_SuckerMap();
bool DoArmPickFromLoadStage_9045_2x4_16();
bool DoPlaceToShuttle_9045_2x4_16();

bool InArmSideAllClose_4x4_16(int iShuttle);
void InArmZNeedDown_2x4_16(int iShuttle, bool bPlace);

void SetInOutArmParameter_2x4_16();

int  GetNowSiteKitMode_2x4_16(int iShuttle,bool bPlace);
void CheckXYPitch_2x4_16(int *iX, int *iY, int iShuttle, bool bPlace, int iMovePitchX, int iMovePitchY);
bool MoveInArm2XYToShuttle_9045_2x4_16(int iShuttle, bool IncludeZ, bool bPlace);

bool ArmFinishLoader_2x4_16();
bool DoInArmPickFromHotPlate_9045_2x4_16(bool &bDownSpeed);
bool InArmHasICForSht_2x4_16(int iSht);
#endif
