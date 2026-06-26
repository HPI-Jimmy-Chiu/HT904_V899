//---------------------------------------------------------------------------
//  ainarm9045_2x6_8.h  --  in-arm 2x6_8 per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 2x6_8, 12-site 2x6)
//  Translator: AI(W6.2c-2x6_8) 20260626
//  Golden source: ainarm9045_2x6_8.h (26 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h owns the file-LOCAL
//  e2x6Mode special-mode enum (NOT in MachineType.h) and exports
//  SetInOutArmParameter_2x6_8, GetShuttleState_2x6_8, CheckSTMMode_2x6_8,
//  SetInArmHasDropToShuttle_2x6_8 and the file-scope statics iCloseSiteStep_2x6 /
//  iCloseSiteModeFor2x6 / XPHSuckToSht_2x6_6[e2x6ModeTotal][8] (defined in the
//  .cpp).  In addition the two engine-dispatched entry points
//  DoInArm_9045_2x6_8() / DoInArm_9045_2x6_8_SuckerMap() are declared here (the
//  golden engine declares them as externs for its DoInArm dispatch ladder;
//  declaring them in this variant's own header keeps the symbols self-documenting
//  for BCB6 reverse-lookup).  All names/values/signatures preserved VERBATIM.
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

// engine-dispatched entry points (defined in ainarm9045_2x6_8.cpp; the engine
// ainarm9045.cpp also declares these as externs for its DoInArm dispatch ladder)
void DoInArm_9045_2x6_8();
void DoInArm_9045_2x6_8_SuckerMap();

#endif
