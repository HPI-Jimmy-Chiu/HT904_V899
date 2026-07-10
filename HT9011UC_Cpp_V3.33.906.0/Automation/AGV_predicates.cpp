// =============================================================================
//  Automation/AGV_predicates.cpp  --  3 pure AMR-mode predicates extracted
//                                      from the golden TfAGV VCL form
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/Automation/AGV.cpp:1315-1348
//  Translation wave: W5 (Automation network unit)
//  Translator: AI(W5-AMR-Translate) 20260710
//
//  See AGV_predicates.h for scope notes (this is NOT the full AGV.cpp -- that
//  ~1860-line E84 VCL form remains deferred).
//
//  Dependencies -- ALL already translated / part of ht9045_globals, zero new
//  shim needed:
//    USE_COVER_TRAYID          -- cmydef.h:5683 (extern int)
//    tCIDNotUse/tCID_Keyence/tCID_NFC -- MachineType.h:589-591 (enum)
//    IniConfig.bA65_BundleIDList / .bSPILFunction -- Config.h:355/133
//    CUSTOMER_CODE              -- cmydef.h:3181 (extern int)
//    CC_AMKOR_Korea             -- MachineType.h:347 (#define 971)
// =============================================================================
#include "AGV_predicates.h"
#include "cprod.h"      // Config.h (IniConfig) + MachineType.h (tCIDNotUse/tCID_Keyence/tCID_NFC/CC_AMKOR_Korea) via cprod.h's own includes
#include "cmydef.h"     // USE_COVER_TRAYID / CUSTOMER_CODE

//---------------------------------------------------------------------------
bool AGV_IsSPIL_AMR()                                                           //JerryYang 20250521 : For AMR
{
    bool bResult=false;
    if(USE_COVER_TRAYID==tCID_Keyence &&
       IniConfig.bA65_BundleIDList==true &&
       IniConfig.bSPILFunction==true)
    {
        bResult=true;
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool AGV_IsATK_AMR()                                                            //Steven 20251210 : ATK AMR
{
    bool bResult=false;
    if(USE_COVER_TRAYID==tCID_NFC &&
       IniConfig.bA65_BundleIDList==true &&
       CUSTOMER_CODE==CC_AMKOR_Korea)
    {
        bResult=true;
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool AGV_Use_AMR()                                                              //Steven 20251216 : for AMR
{
    bool bResult=false;
    if(USE_COVER_TRAYID!=tCIDNotUse &&
       IniConfig.bA65_BundleIDList==true)
    {
        bResult=true;
    }
    return bResult;
}
//---------------------------------------------------------------------------
