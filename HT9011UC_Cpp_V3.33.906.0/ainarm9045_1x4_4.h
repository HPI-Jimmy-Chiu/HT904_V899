//---------------------------------------------------------------------------
//  ainarm9045_1x4_4.h  --  in-arm 1x4_4 per-site-config variant header
//
//  Translation wave: W6.2c (in-arm per-site-config variant: 1x4_4, TSMC 1x4 Site)
//  Translator: AI(W6.2c-1x4_4) 20260626
//  Golden source: ainarm9045_1x4_4.h (22 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  The golden .h exports the e1x4Mode enum
//  plus the close-site / shuttle-state / parameter helpers and the two file-scope
//  tables (iCloseSiteModeFor1x4 / XPHSuckToSht_1x4_4).  The dispatch callees
//  DoInArm_9045_1x4_4() / DoInArm_9045_1x4_4_SuckerMap() are declared here so the
//  engine's DoInArm dispatch ladder can reach them; all names/signatures are
//  preserved VERBATIM for BCB6 reverse-lookup.
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

//AI(W6.2c-1x4_4) 20260626: dispatch callees made live at Integrate (engine reaches
//these via DoInArm_9045 / DoInArm_9045_SuckerMap once un-gated).
void DoInArm_9045_1x4_4();
void DoInArm_9045_1x4_4_SuckerMap();

#endif
