//---------------------------------------------------------------------------
#ifndef ainarm9045_2x6_8H
#define ainarm9045_2x6_8H
//---------------------------------------------------------------------------

enum e2x6Mode                                                                   //Steven 20240430 : 整合2x6特殊模式
{
    e2x6Standard            =0,
    e2x6Step2               =1,
    e2x6OneByOne            =2,
    e2x6CloseCenter2x4      =3,
    e2x6CloseCenter2x4Step2 =4,
    e2x6CloseCenter2x41By1  =5,
    e2x6Run2x4              =6,
    e2x6Run2x4Step2         =7,
    e2x6ModeTotal
};

void SetInOutArmParameter_2x6_8();
int GetShuttleState_2x6_8(int iSht, bool bPick);
void CheckSTMMode_2x6_8();                                                      //Steven 20221006 : 確認STM模式
extern int iCloseSiteStep_2x6;
extern int iCloseSiteModeFor2x6;                                                //Steven 20240417 : 12site關中間4site
extern int XPHSuckToSht_2x6_6[e2x6ModeTotal][8];
void SetInArmHasDropToShuttle_2x6_8(int SHT, int Row, int Col);
#endif
