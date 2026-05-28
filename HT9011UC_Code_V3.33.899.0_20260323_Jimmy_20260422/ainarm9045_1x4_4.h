//---------------------------------------------------------------------------
#ifndef ainarm9045_1x4_4H
#define ainarm9045_1x4_4H
//---------------------------------------------------------------------------

enum e1x4Mode                                                                   //Steven 20241111 : 整合1x4特殊模式
{
    e1x4Standard            =0,
    e1x4Step2               =1,
    e1x4OneByOne            =2,
    e1x4CloseAbAc           =3,
    e1x4CloseAbAc1By1       =4,
    e1x4ModeTotal
};

void SetInOutArmParameter_1x4_4();
int GetShuttleState_1x4_4(int iSht, bool bPick);
void CheckCloseSiteMode_1x4_4();                                                //Steven 20241111 : for 1x4 close site
extern int iCloseSiteModeFor1x4;
extern int XPHSuckToSht_1x4_4[e1x4ModeTotal][8];
void SetInArmHasDropToShuttle_1x4_4(int SHT, int Row, int Col);
#endif
