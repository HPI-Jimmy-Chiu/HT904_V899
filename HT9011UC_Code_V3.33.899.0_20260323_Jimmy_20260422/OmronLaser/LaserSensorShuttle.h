//---------------------------------------------------------------------------

#ifndef LaserSensorShuttleH
#define LaserSensorShuttleH
//---------------------------------------------------------------------------
#include "MachineType.h"

extern int iShtLaserInitTask;
extern int iShtLaserCheckTask;
extern double dCurrentValue[2][MAX_Index_Row][MAX_Index_Col];

bool ShtLaserInit(int iSht, bool bReset=false);         //將讀頭歸零並取得每個位置的數值

bool CheckShtFloating(int iSht, bool bReset=false);     //確認每個位置的數值


#endif
