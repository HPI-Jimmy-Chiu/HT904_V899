//---------------------------------------------------------------------------
#ifndef ainarm9045_2x8_8H
#define ainarm9045_2x8_8H
//---------------------------------------------------------------------------

//enum e2x8Mode                                                                   //Steven 20240430 : 整合2x8特殊模式     //JerryYang 20250711 : mark掉
//{
//    e2x8Standard        =0,
//    e2x8Step2           =1,
//    e2x8OneByOne        =2,
//    e2x8_STMMode        =3,
//    e2x8_TW153Mode      =4,
//    e2x8CloseEven       =5,
//    e2x8CloseEven1By1   =6,
//    e2x8CloseOdd        =7,
//    e2x8CloseOdd1By1    =8,
//    e2x8Run2x2_13       =9,
//    e2x8Run2x2_14       =10,
//    e2x8Run2x4Standard  =11,
//    e2x8Run2x4Step2     =12,
//    e2x8ModeTotal
//};

void SetInOutArmParameter_2x8_8();
int GetShuttleState_2x8_8(int iSht, bool bPick);
void CheckSTMMode_2x8_8();                                                      //Steven 20221006 : 確認STM模式
extern int iCloseSiteStep_2x8;
extern int iCloseSiteModeFor2x8;                                                //JerryYang 20190729 STM 8 site交錯模式
extern int XPHSuckToSht_2x8_8[e2x8ModeTotal][2][8];
void SetInArmHasDropToShuttle_2x8_8(int SHT, int Row, int Col);

#endif
