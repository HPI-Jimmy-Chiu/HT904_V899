//---------------------------------------------------------------------------

#ifndef LaserSensorInArmH
#define LaserSensorInArmH
//---------------------------------------------------------------------------
#include "MachineType.h"


extern int iInArmLaserInitTask;
extern int iInArmLaserCheckTask;
extern double dInArmCurrentValue[2][50][50];

class TInLaserCheck
{
    public:
        TInLaserCheck(int ip, int x, int y)
        {
            iPlate=ip;
            iX=x;
            iY=y;
        };

        int iPlate;
        int iX;
        int iY;
};

extern vector< TInLaserCheck* > LaserCheckPos;

bool InArmLaserInit(bool bReset=false);         //將讀頭歸零並取得每個位置的數值
bool CheckInArmFloating(bool bReset=false);     //確認每個位置的數值
#endif
