// =============================================================================
//  aArmHeader.h  --  SHIM (guard-only) for the InArm/OutArm god-header
//
//  Mirrored as a SHIM from: HT9011UC_Code_V3.33.906.0_20260618/aArmHeader.h
//  Translation wave: W6.0 (decoupling scaffold)
//  Translator: AI(W6.0-SCAFFOLD) 20260626
//
//  The golden aArmHeader.h is a pure GOD-HEADER: it #includes ~70 per-site-
//  config InArm/OutArm/SortArm SM headers (ainarm9045_*, aoutarm9045_*,
//  asortarm, SortingBinTray) plus uHGemHT9045 / AutoClean / AGV / common.
//  Pulling any of those drags the entire arm + SECS + AutoClean god-stack.
//
//  The W6.1 Empty-tray canary NEVER includes this header (verified -- it only
//  needs asendic.h + csystem.h + FormsFacade.h + mycylin.h + the substrate).
//  This shim exists so any TU that resolves `aArmHeader.h` literally gets a
//  valid include guard with NOTHING pulled in.  The whole god-header include
//  list is gated #if 0 // TODO(W6.x/W7) and restored per-site as those arm SMs
//  are translated.
// =============================================================================
#ifndef aArmHeaderH
#define aArmHeaderH

#if 0   // TODO(W6.x/W7): restore per-site arm SM headers as they are translated
//#include "MachineDefine.h"

#include "LaserSensorInArm.h"
#include "AutoClean.h"
#include "InOutArmZteach.h"
#include "ainarm_SearchPickPlate.h"
#include "ainarm_SearchPlacePlate.h"

#include "ainarm2.h"
#include "ainarm9045.h"
#include "ainarm9045_1x1_1.h"
#include "ainarm9045_1x2_2.h"
#include "ainarm9045_1x2_2_14.h"
#include "ainarm9045_1x2_4_Hot.h"
#include "ainarm9045_1x3_4.h"   //Frank 20160329 add for 1x3_4
#include "ainarm9045_1x3_2_14.h"    //Steven 20220425 : 1x3_14
#include "ainarm9045_1x4_2.h"
#include "ainarm9045_1x4_4.h"
#include "ainarm9045S_1x4_4.h"
#include "ainarm9045_1x4_4_Back.h"
#include "ainarm9045_1x4_8_Hot.h"
#include "ainarm9045_2x1_2.h"
#include "ainarm9045_2x2_4.h"
#include "ainarm9045_2x2_4_12.h"
#include "ainarm9045_2x2_4_14.h"
#include "ainarm9045_2x2_8_Hot.h"
#include "ainarm9045_2x3_6.h"  //ChungHung 20140115 add for 2x3_6
#include "ainarm9045_2x3_6_14.h"
#include "ainarm9045_2x4_4.h"
#include "ainarm9045S_2x4_4_13.h"//Ifor 20170113 (Steven) add 2x4_4_13 for HT-9045S
#include "ainarm9045_2x4_8.h"
#include "ainarm9045_2x5_8.h"
#include "ainarm9045_2x6_8.h"
#include "ainarm9045_2x8_8.h"
#include "ainarm9045_All_1Pick.h"
//#include "ainarm9045_2x8_32.h"

#include "aoutarm.h"
#include "aoutarm9045.h"
#include "aoutarm9045_1x1_1.h"
#include "aoutarm9045_1x2_2.h"
#include "aoutarm9045_1x2_4.h"
#include "aoutarm9045_1x3_4.h"      //Frank 20160329 add for 1x3_4
#include "aoutarm9045_1x3_2_14.h"   //Steven 20220425 : 1x3_14
#include "aoutarm9045_1x4_2.h"
#include "aoutarm9045_1x4_4.h"
#include "aoutarm9045_1x4_4S.h"
//#include "aoutarm9045_1x4_4_Back.h"
#include "aoutarm9045_1x4_8.h"
#include "aoutarm9045_2x1_2.h"
#include "aoutarm9045_2x2_4.h"
//#include "aoutarm9045_2x2_4_14.h"
//#include "aoutarm9045_2x2_4_23.h"
#include "aoutarm9045_2x2_8.h"
#include "aoutarm9045_2x3_6.h"      //ChungHung 20140115 add for 2x3_6
#include "aoutarm9045_2x3_6_14.h"
#include "aoutarm9045_2x4_4.h"
#include "aoutarm9045_2x4_8.h"
#include "aoutarm9045_2x5_8.h"
#include "aoutarm9045_2x6_8.h"
#include "aoutarm9045_2x8_8.h"
#include "aoutarm9045_All_1Picker.h"
//#include "aoutarm9045_2x8_32.h"

#include "asortarm.h"                                                           //RogerYang 20250516 Add for 9046AU

#include "SortingBinTray.h"                                                     //JerryYang 20150910 Auto Sorting BinTray by Out Arm when Clean Out

#include "uHGemHT9045.h"
#include "atester_ProcessCount.h"
#include "common.h"
#include "HTEditList.h"
#include "AGV.h"
extern bool CheckHeaterOK();                                                    //Steven 20250116 : confirm HeaterOK
#endif // TODO(W6.x/W7): god-header include list

#endif // aArmHeaderH
