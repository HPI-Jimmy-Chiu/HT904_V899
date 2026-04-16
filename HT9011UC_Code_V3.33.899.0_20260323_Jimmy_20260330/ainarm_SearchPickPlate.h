//---------------------------------------------------------------------------
#ifndef ainarm_SearchPickPlateH
#define ainarm_SearchPickPlateH

#include "cmydef.h"
//---------------------------------------------------------------------------
extern bool ArmRow1AllHasIC();
extern int  GetInArmToHPYPitch_9045();
extern bool DoInArmPickFromHotPlate_9045();

extern bool bPickFormHotplatePartOK;                                            //ChungHung 2012012 add
extern bool bInArmToPickHotPlatePos;                                            //ChungHung 20110323
extern bool bPickFormHotplateRetry;                                             //Ifor 20160616 add Hot plate Pickup error Retry
extern bool bInArmTryPickFromHotPlateFinish;                                    //ChungHung 20120206 Hotplate check

extern int  GetHeaterWaitTime();
extern bool HasHotReadyIC_9045();
extern bool InArmNeedCheckHotPlateOffset(bool bPlace);                          //Steven 20230531 : 簡化判斷式

extern bool MoveInArmXYPickHotPlate_9045(bool bNeedDown);

extern void TryHotClearRecBufMMPlate();                                         //ChungHung 20120206 Hotplate check
extern void BackupHotplatelocation();                                           //Ifor 20171006 (Steven) : add 備份Hotplate取料位置
extern void HotplateDataConversion(int iSuckR, int iSuckC, int iP, int iPlateR, int iPlateC, bool *flage1);//Ifor 20180518 : add 整合Hot plate 資料轉換
extern bool IsFLCarrKitAllHasIC();                                              //ChungHung 20111230 add
extern bool IsBLCarrKitAllHasIC();                                              //ChungHung 20111230 add
#endif
