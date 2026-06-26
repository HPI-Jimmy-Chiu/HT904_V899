// =============================================================================
//  ainarm2.cpp  --  in-arm shared helpers (offline Sim-HAL translation)
//
//  Translation wave: W6.2b-canary (in-arm per-site-config variant dispatch)
//  Translator: AI(W6.2b-canary) 20260626
//
//  Mirror of the golden BCB6 ainarm2.cpp (D:/HT9045/
//  HT9011UC_Code_V3.33.906.0_20260618/ainarm2.cpp).  W6.2b only needs the two
//  in-arm helpers that the freshly-translated site-variant SMs
//  (ainarm9045_1x1_1 / _2x4_16 / _2x8_32) call into but that had no body yet:
//
//    * SetInArmUseSuckToHasNullIC(int, int)  -- golden ainarm2.cpp:201
//        Re-classifies the in-arm pick grid's NULL_IC nozzles to HAS_NULL_IC
//        (the "I am committed to picking here" marker) honouring the picker
//        kind (AxEx / AxxG step) and the 2x8 close-site sub-modes.  Pure grid
//        logic over the Sim HAL InArmSuck object -- NO hardware touched.
//    * InitInArmPickFromHotPlateTask()       -- golden ainarm2.cpp:623
//        Resets the HP-pick task cursor to 1.  (Its 340/50 siblings already
//        live in aHotPlateSubstrate.cpp.)
//
//  Both are FAITHFUL line-for-line translations -- names, formulas, branches and
//  comments preserved verbatim.  The declarations these realize already exist in
//  aHotPlateSubstrate.h:610/611 (so no header edit is needed).
//
//  Also hosts the single active definition of bDestoryOnSht (golden moved it
//  ainarm2 -> cmydef by RogerYang 20251021; cmydef.h:5868 declares it extern,
//  but the cmydef.cpp definition sits inside a #if 0 // TODO(W6) block, so the
//  one live definition is provided here in this linked TU -- exactly one
//  definition, no ODR dupes).
//
//  Big5: Chinese comments preserved as UTF-8 (cp950).  NO U+FFFD.
// =============================================================================
#include "cmydef.h"                 // iInArmType / MInArmX / NULL_IC / HAS_NULL_IC consts
#include "MachineType.h"            // eTestMode (SingleSite) / e9045_* picker enums
#include "cprod.h"                  // TestIF (iTestMode)
#include "ainarm9045.h"             // bUseAxExPicker / bUseAxxGPicker / GetJStep
#include "aHotPlateSubstrate.h"     // InArmSuck grid + iInArmPickFromHotPlateTask / iCloseSiteModeFor2x8 cursors

// =============================================================================
//  bDestoryOnSht  (golden cmydef.cpp:5955 -- RogerYang 20251021: 從inarm2移過來)
//  在Shuttle吹氣與資料交換的Flag (Steven 20170905 wei).  cmydef.cpp's definition
//  is gated #if 0 // TODO(W6); this is the single ACTIVE definition.
// =============================================================================
#ifndef HT9045_bDestoryOnSht_DEFINED
#define HT9045_bDestoryOnSht_DEFINED
bool bDestoryOnSht=false;                                                       //RogerYang 20251021 : 從inarm2移過來 //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
#endif

// =============================================================================
void SetInArmUseSuckToHasNullIC(int iSht, int iKit)
{
//    int iKit32=(iKit==1)?4:0;

    if(iInArmType==e9045_1x4_8_Hot ||                                           //Steven 20240119 : fixed for 2x4_16
       iInArmType==e9045_1x2_4_Hot ||
       iInArmType==e9045_2x2_8_Hot)
    {
//        iSht=0;
//        iKit32=0;
    }

    if(TestIF.iTestMode==SingleSite ||                                          //Steven 20140614 : for Auto Clean Single Site
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
    }
    else if(bUseAxExPicker() ||                                                 //Steven 20241102 : Fixed for HP
            bUseAxxGPicker())
    {
        for(int i=0; i<InArmSuck.iPickRow; i++)
        {
            for(int j=0; j<InArmSuck.iPickCol; j++)
            {
                int j2=j*GetJStep();
                if(j2<InArmSuck.iMaxCol &&
                   InArmSuck.Item[i][j2]==NULL_IC)
                {
                    InArmSuck.SetItemData(i, j2, HAS_NULL_IC);
                }
            }
        }
    }
    else
    {
        for(int i=0; i<InArmSuck.iPickRow; i++)
        {
            for(int j=0; j<InArmSuck.iPickCol; j++)
            {
                if(InArmSuck.Item[i][j]==NULL_IC)
                    InArmSuck.SetItemData(i, j, HAS_NULL_IC);
            }
        }
    }

    if(iCloseSiteModeFor2x8==e2x8Run2x2_13)
    {
        InArmSuck.SetItemData(0, 1, NULL_IC);
        InArmSuck.SetItemData(0, 3, NULL_IC);
        InArmSuck.SetItemData(1, 1, NULL_IC);
        InArmSuck.SetItemData(1, 3, NULL_IC);
    }
    else if(iCloseSiteModeFor2x8==e2x8Run2x2_14)
    {
        InArmSuck.SetItemData(0, 1, NULL_IC);
        InArmSuck.SetItemData(0, 2, NULL_IC);
        InArmSuck.SetItemData(1, 1, NULL_IC);
        InArmSuck.SetItemData(1, 2, NULL_IC);
    }
}
//==============================================================================
void InitInArmPickFromHotPlateTask()
{
    iInArmPickFromHotPlateTask=1;
}
//==============================================================================
