//---------------------------------------------------------------------------

#ifndef LaserSensorShuttleH
#define LaserSensorShuttleH
//---------------------------------------------------------------------------
#include "MachineType.h"

extern int iShtLaserInitTask;
extern int iShtLaserCheckTask;
extern int iUseInArmShtLaserCheckTask;
extern int iUseOutArmShtLaserCheckTask;
extern double dCurrentValue[2][MAX_Index_Row][MAX_Index_Col];

bool ShtLaserInit(int iSht, bool bReset=false);         //將讀頭歸零並取得每個位置的數值

bool CheckShtFloating(int iSht, bool bReset=false);     //確認每個位置的數值
bool UseInArmCheckShtFloating(int iSht, bool bReset=false, bool bSetGold=false);     //確認每個位置的數值  3
bool UseOutArmCheckShtFloating(int iSht, bool bReset=false, bool bSetGold=false);
bool MoveInArmXYToSHLaserPos(int iShuttle, int iCol, int iRow);
bool MoveOutArmXYToSHLaserPos(int iShuttle, int iCol, int iRow);
#endif
