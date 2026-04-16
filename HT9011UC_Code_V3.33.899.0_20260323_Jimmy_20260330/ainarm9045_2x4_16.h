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

int  GetShuttleState_2x4_16(int iWhichShuttle, bool bPick);
void SearchCleanKitUpDown_2x4_16(int iACWhichShuttle, int bPick);
bool PickFromCleanKit_2x4_16();
bool PlaceToCleanKit_2x4_16();
bool CleanPad_PlaceToShuttle_2x4_16(int iWhichShuttle);
bool CleanPad_PickFromShuttle_2x4_16(int iWhichShuttle);
#endif
