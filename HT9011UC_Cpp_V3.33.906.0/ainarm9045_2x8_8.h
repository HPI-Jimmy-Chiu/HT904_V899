//---------------------------------------------------------------------------
//  ainarm9045_2x8_8.h  --  in-arm 2x8_8 per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 2x8_8, two-row 8-picker 32-site)
//  Translator: AI(W6.2c-2x8_8) 20260626
//  Golden source: ainarm9045_2x8_8.h (32 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  NOTE: the golden e2x8Mode special-mode
//  enum is COMMENTED OUT in the golden header (JerryYang 20250711 mark-out) because
//  the authoritative e2x8Mode/e2x8ModeTotal lives in MachineType.h (target
//  MachineType.h:1497-1510, in ht9045_globals); we preserve that comment-out
//  VERBATIM -- do NOT re-enable it (it would clash with the global definition).
//  The golden .h exports SetInOutArmParameter_2x8_8, GetShuttleState_2x8_8,
//  CheckSTMMode_2x8_8, SetInArmHasDropToShuttle_2x8_8 and the file-scope statics
//  iCloseSiteStep_2x8 / iCloseSiteModeFor2x8 / XPHSuckToSht_2x8_8[e2x8ModeTotal][2][8]
//  (defined in the .cpp).  In addition the two engine-dispatched entry points
//  DoInArm_9045_2x8_8() / DoInArm_9045_2x8_8_SuckerMap() are declared here (the
//  engine ainarm9045.cpp also declares them as externs for its DoInArm dispatch
//  ladder, which routes BOTH e9045_2x8_8 AND e9045_2x8_32 here).  All names/
//  values/signatures preserved VERBATIM.
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

// engine-dispatched entry points (defined in ainarm9045_2x8_8.cpp; the engine
// ainarm9045.cpp also declares these as externs for its DoInArm dispatch ladder,
// which routes BOTH iInArmType==e9045_2x8_8 AND iInArmType==e9045_2x8_32 here)
void DoInArm_9045_2x8_8();
void DoInArm_9045_2x8_8_SuckerMap();

#endif
