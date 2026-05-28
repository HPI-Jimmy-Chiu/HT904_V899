//---------------------------------------------------------------------------
#ifndef ainarm9045_2x8_32H
#define ainarm9045_2x8_32H
//---------------------------------------------------------------------------
void DoInArm_9045_2x8_32();
void DoInArm_9045_2x8_32_SuckerMap();
void InArmZNeedDown_2x8_32(int iSht,bool bPlace);
void CheckXYPitch_2x8_32(int *iX,int *iY,int iSht,bool bPlace);
bool InArmSideAllClose_32(int iSht, int iKit);
bool DoPlaceToShuttle_9045_2x8_32();//kevin 20180312 define
bool MoveInArmXYPickHotPlate_2x8_32(bool OnlyRow2);    //kevin 20210208 add define
bool MoveArmXYToLoaderStage_9045_2x8_32();             //kevin 20210208
bool CleanPad_PlaceToShuttle_2x8_32(int iSht);
int GetShuttleState_4x8_32(int iSht, bool bPick);

bool DoArmPickFromLoadStage_9045_2x8_32();
bool DoPlaceToShuttle_9045_2x8_32();
#endif
